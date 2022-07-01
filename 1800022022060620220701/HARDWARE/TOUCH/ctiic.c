#include "ctiic.h"
#include "delay.h"	 
#include "cst148.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//控制I2C速度的延时
void CST_Delay(void)
{
	delay_us(1);
} 


//电容触摸芯片IIC接口初始化
void CST_IIC_Init(void)
		{					     
				GPIO_InitTypeDef  GPIO_InitStructure;
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);      //使能GPIOB时钟

				GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11; //PB0设置为推挽输出
				GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;             //输出模式GPIO_Mode_OUT
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //100MHz
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;              //上拉
				GPIO_Init(GPIOB, &GPIO_InitStructure);                     //初始化

			
			  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;               //PB13 中断输入   设置为上拉输入  20220302
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;              //输入模式
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;         //100MHz
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;              //上拉
				GPIO_Init(GPIOB, &GPIO_InitStructure);                     //初始化
					
			
			
				GPIO_SetBits(GPIOB,GPIO_Pin_10);						               //输出1
				GPIO_SetBits(GPIOB,GPIO_Pin_11);					                 //输出1
			
			
			
		}


//产生IIC起始信号
void CST_IIC_Start(void)
{
	CST_SDA_OUT();//sda线输出
	CST_IIC_SDA=1;	  	  
	CST_IIC_SCL=1;
	CST_Delay();
 	CST_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CST_Delay();
	CST_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  


//产生IIC停止信号
void CST_IIC_Stop(void)
{ 
	CST_SDA_OUT();//sda线输出
	CST_IIC_SCL=0;
	CST_IIC_SDA=0;
	CST_Delay();
	CST_IIC_SCL=1;
	CST_Delay();
	CST_IIC_SDA=1;//STOP:when CLK is high DATA change form low to high 
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CST_IIC_Wait_Ack(void)
{
	u16 ucErrTime=0;
	CST_SDA_IN();      //SDA设置为输入  
	CST_IIC_SDA=1;delay_us(1);	   
	while(CST_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>20000)
		{
			CST_IIC_Stop();
			return 1;
		} 
	}
	CST_IIC_SCL=1;delay_us(4);	 //20220118
	CST_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 


//产生ACK应答
void CST_IIC_Ack(void)
{
	CST_IIC_SCL=0;
	CST_SDA_OUT();
	CST_IIC_SDA=0;
	CST_Delay();
	delay_us(2);
	CST_IIC_SCL=1;
	CST_Delay();
	CST_IIC_SCL=0;
}


//不产生ACK应答		    
void CST_IIC_NAck(void)
{
	CST_IIC_SCL=0;
	CST_SDA_OUT();
	CST_IIC_SDA=1;
	CST_Delay();
	CST_IIC_SCL=1;
	CST_Delay();
	CST_IIC_SCL=0;
}					 				   


//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void CST_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		CST_SDA_OUT(); 	    
    CST_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        CST_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	      
				CST_IIC_SCL=1;
				CST_Delay();
				CST_IIC_SCL=0;	
				CST_Delay();
    }	 
} 	


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 CST_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
	CST_IIC_SCL=0;
 	CST_SDA_IN();//SDA设置为输入
	delay_us(1);
  for(i=0;i<8;i++ )
	{
    CST_IIC_SCL=0;
    CST_Delay();    
		CST_IIC_SCL=1; 
		delay_us(2);		
		receive<<=1;
		if(CST_READ_SDA)receive++;
	}	  				 
	if (!ack)CST_IIC_NAck();//发送nACK
	else CST_IIC_Ack(); //发送ACK
 	return receive;
}



u8 CST148_WR_Reg(u16 reg,u8 *buf,u8 len)//读148寄存器操作，地址，存储，长度
{
	u8 i;
	u8 ret=0;
	
	CST_IIC_Start();	
	
 	CST_IIC_Send_Byte(CST148_CMD_WR);   //发送写命令 	 
	CST_IIC_Wait_Ack();
	if(ret)
		{
		 printf("1 CT_IIC_Wait_Ack error . \r\r\n");
	  }
	else{
	printf("\r\n\r\n 1 ack ok. \r\r\n");
	};
	
	CST_IIC_Send_Byte(reg>>8);   	      //发送高8位地址
	CST_IIC_Wait_Ack(); 
  if(ret){
		printf("1 CT_IIC_Wait_Ack error . \r\r\n");
	} 	
	else{
	printf(" 2 ack ok. \r\r\n");
	};
	CST_IIC_Send_Byte(reg&0XFF);   	    //发送低8位地址
	CST_IIC_Wait_Ack();  
	if(ret){
		printf("1 CT_IIC_Wait_Ack error . \r\r\n");
	} 
	else{
	printf(" 3 ack ok. \r\r\n");
	};
	
	for(i=0;i<len;i++)
	{	   
    CST_IIC_Send_Byte(buf[i]);  	    //发数据
		ret=CST_IIC_Wait_Ack();
		if(ret)break;  
	}
    CST_IIC_Stop();					          //产生一个停止条件	    
	return ret; 
}





void CST148_RD_Reg(u16 reg,u8 *buf,u8 len)//写148寄存器操作，地址，存储，长度
{
	u8 i; 
 	CST_IIC_Start();	
 	CST_IIC_Send_Byte(CST148_CMD_WR);           //发送写命令 	 
	CST_IIC_Wait_Ack();
 	CST_IIC_Send_Byte(reg>>8);   	              //发送高8位地址
	CST_IIC_Wait_Ack(); 	 										  		   
 	CST_IIC_Send_Byte(reg&0XFF);      	        //发送低8位地址
	CST_IIC_Wait_Ack();  
 	CST_IIC_Start();  	 	   
	CST_IIC_Send_Byte(CST148_CMD_RD);           //发送读命令		   
	CST_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    buf[i]=CST_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CST_IIC_Stop();                           //产生一个停止条件    
}




