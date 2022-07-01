
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"

#include "PRTC.h"





/*
2022-4-11 14:47:32

DS1302Z_SCLK      PE3
DS1302Z_IO        PE4
DS1302Z_CE        PE5

io具有双向的收发功能     实际是SPI 的全双工的接口

要让ds1302走时，必须先将秒置零，否则秒单元就是0x80，一直不走。


*/




//uint8_t Miao = 0;                   

//Canshu_Time DS1302_Time;
//Canshu_Time DS1302_SetTime;






//void Time_Init(void)
//		{
//			DS1302_SetTime.Year   = 2022;
//			DS1302_SetTime.Month  = 4;
//			DS1302_SetTime.Day    = 12;
//			
//			DS1302_SetTime.Hour   = 16;
//			DS1302_SetTime.Minute = 45;
//		 //DS1302_WriteBYTE(0x80,0x00,0);   //启动时钟的操作
//		}






//void DS1302_Delay( int n)//这个延时需要修改的时间    时序不对
//		{
//			uint16_t i = 0;
//			uint16_t j = 0;

//			for(i = 0;i<n;i++)
//				{
//						for(j = 0;j<1000;j++);//从100修改为1000
//				}
//		}



///*********************************************时钟芯片的数据位的输入，输出的初始化函数*************************************************/
//void DS1302_SDA_IN(void)
//{
//	
////	GPIO_InitTypeDef GPIO_InitStruct = {0};
////	 
////	GPIO_InitStruct.Pin = DS_IO_Pin;
////  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
////  GPIO_InitStruct.Pull = GPIO_NOPULL;
////  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
////  HAL_GPIO_Init(DS_IO_GPIO_Port, &GPIO_InitStruct);
//	 
//	
//	GPIO_InitTypeDef GPIO_InitStructure = {0};
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);     //使能GPIOE的时钟
//	
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_IN;          //输入的模式
//	//GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;       //推挽输出
//	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
//	GPIO_Init(GPIOE,&GPIO_InitStructure); 
//	 
//}




//void DS1302_SDA_OUT(void)
//{
////	GPIO_InitTypeDef GPIO_InitStruct = {0};
////	
////	GPIO_InitStruct.Pin = DS_IO_Pin;
////  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
////  GPIO_InitStruct.Pull = GPIO_NOPULL;
////  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
////  HAL_GPIO_Init(DS_IO_GPIO_Port, &GPIO_InitStruct);
//	
//	
//  GPIO_InitTypeDef GPIO_InitStructure  = {0};
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);     //使能GPIOB的时钟
//	
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;         //输出
//	GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	
//	
//}




///*********************************************时钟芯片的时钟的输入，输出的初始化函数*************************************************/
//void DS1302_SCK_OUT(void)
//{
////	GPIO_InitTypeDef GPIO_InitStruct = {0};
////	
////	GPIO_InitStruct.Pin = DS_SCLK_Pin;
////  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
////  GPIO_InitStruct.Pull = GPIO_NOPULL;
////  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
////  HAL_GPIO_Init(DS_SCLK_GPIO_Port, &GPIO_InitStruct);
//	
//	
//	
//	GPIO_InitTypeDef GPIO_InitStructure = {0};
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);     //使能GPIOB的时钟
//	
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;         //输出
//	GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	
//}





//void DS1302_SCK_IN(void)
//{
////	GPIO_InitTypeDef GPIO_InitStruct = {0};
////	 
////	GPIO_InitStruct.Pin = DS_SCLK_Pin;
////  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
////  GPIO_InitStruct.Pull = GPIO_NOPULL;
////  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
////  HAL_GPIO_Init(DS_SCLK_GPIO_Port, &GPIO_InitStruct);
//	
//	
//	GPIO_InitTypeDef GPIO_InitStructure = {0};
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);     //使能GPIOB的时钟
//	
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_IN;         //输出
//	//GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	
//	
//	
//	
//}





///*********************************************时钟芯片的CE的输入，输出的初始化函数*************************************************/
//void DS1302_CE_OUT(void)
//{
////	GPIO_InitTypeDef GPIO_InitStruct = {0};
////	
////	GPIO_InitStruct.Pin = DS_CE_Pin;
////  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
////  GPIO_InitStruct.Pull = GPIO_NOPULL;
////  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
////  HAL_GPIO_Init(DS_CE_GPIO_Port, &GPIO_InitStruct);
//	
//	
//	GPIO_InitTypeDef GPIO_InitStructure = {0};
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);     //使能GPIOB的时钟
//	
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;         //输出
//	GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	
//	
//	
//}







//void DS1302_CE_IN(void)
//{
////	GPIO_InitTypeDef GPIO_InitStruct = {0};
////	 
////	GPIO_InitStruct.Pin = DS_CE_Pin;
////  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
////  GPIO_InitStruct.Pull = GPIO_NOPULL;
////  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
////  HAL_GPIO_Init(DS_CE_GPIO_Port, &GPIO_InitStruct);
//	
//	
//	
//	GPIO_InitTypeDef GPIO_InitStructure = {0};
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);     //使能GPIOB的时钟
//	
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_IN;         //输出
////GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
//	GPIO_Init(GPIOE,&GPIO_InitStructure);

//}


///*********************************************时钟芯片的时钟的输入，输出的初始化函数*************************************************/
///************************************************
//void DS1302_WriteBYTE(unsigned char Adr,unsigned char Data,unsigned char CorR)
//函数功能：往DS1302里面写字节
//参数说明：
//1、Adr：指示的是寄出器地址；
//2、Data：写入的数据；
//2、CorR: 指示是时钟日期数据 或者是 RAM数据
//              0：指示的是时钟日期数据
//              1：指示的是RAM数据
//3、WorR: 指示的是读操作或者写操作
//               0：指示的是写操作
//               1：指示的是读操作
//****************************************************************/
//void DS1302_WriteBYTE(uint8_t Adr,uint8_t Data,uint8_t CorR)
//{
//  uint8_t BL = 0;
//	uint8_t i  = 0;
//    
//	DS1302_SDA_OUT();
//	DS1302_SCK_OUT();
//	DS1302_CE_OUT();
//	
//	DS1302_CE_L;	
//	
//	DS1302_Delay(1);//这个延时的时间根据时钟修改
//	
//	DS1302_CE_H;
//	
//	DS1302_Delay(1);//这个延时的时间根据时钟修改

//	DS1302IO_SDA_L;
//	
//	DS1302_Delay(1);//这个延时的时间根据时钟修改
//	
//	
//  //写地址
//	DS1302CLK_L;
//	
//	DS1302_Delay(1);//这个延时的时间根据时钟修改	

//	for(i = 0; i<8; i++)
//	{
//		DS1302CLK_L;
//		DS1302_Delay(2);//这个延时的时间根据时钟修改	
//			
//		BL = Adr>>i;
//			
//		if(BL & BIT0)
//			{
//			DS1302IO_SDA_H;
//		  }
//		else
//			{
//			DS1302IO_SDA_L;
//		  }
//			
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//		DS1302CLK_H;
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//			
//	}
//    
//	//写数据
//	for(i = 0; i<8; i++)
//	{
//		DS1302CLK_L;
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//		
//		BL = Data>>i;
//		
//		if(BL & BIT0)
//			{
//				DS1302IO_SDA_H;
//		  }
//		else
//			{
//			DS1302IO_SDA_L;
//		  }
//		
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//			
//		DS1302CLK_H;
//			
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//			
//	}

//	DS1302CLK_L;
//	DS1302_Delay(1);//这个延时的时间根据时钟修改	
//}

///************************************************
//void DS1302_ReadBYTE(unsigned char Adr,unsigned char Data,unsigned char CorR,unsigned char WorR)
//函数功能：往DS1302里面写字节
//参数说明：
//1、Adr：指示的是寄出器地址；
//2、Data：写入的数据；
//2、CorR: 指示是时钟日期数据 或者是 RAM数据
//              0：指示的是时钟日期数据
//              1：指示的是RAM数据
//3、WorR: 指示的是读操作或者写操作
//               0：指示的是写操作
//               1：指示的是读操作
//****************************************************************/
//uint8_t DS1302_ReadBYTE(uint8_t Adr,uint8_t Data,uint8_t CorR)
//{
//	uint8_t Data_BIT;
//	uint8_t BL = 0;
//	uint8_t i  = 0;
//    
//	DS1302_SDA_OUT();
//	DS1302_SCK_OUT();
//	DS1302_CE_OUT();

//	DS1302IO_SDA_L;
//	DS1302_CE_L;
//	DS1302_Delay(1);//这个延时的时间根据时钟修改	
//	
//	DS1302_CE_H;

//  //写地址
//  DS1302CLK_L;    
//  DS1302_Delay(1);//这个延时的时间根据时钟修改	
//    
//	for(i = 0; i<8; i++)
//	{
//		DS1302CLK_L;
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	

//		BL = Adr>>i;

//		if(BL & BIT0)
//			{
//			DS1302IO_SDA_H;
//		  }
//		else
//			{
//			DS1302IO_SDA_L;
//		  }

//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//		DS1302CLK_H;
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//	}	
//	
//	
//	DS1302_SDA_IN();

//	Data = 0;
//    //读数据
//	for(i = 0; i<8; i++)
//	{
//		DS1302CLK_L;
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//		
//		if(DS1302_SDA_READ == 1) 
//			{ 
//				Data_BIT = 1;
//		  }
//		else 
//			  Data_BIT = 0;

//		BL = Data_BIT << i;

//		Data = BL + Data;

//		DS1302CLK_H;
//		DS1302_Delay(1);//这个延时的时间根据时钟修改	
//	}
//    
//   DS1302CLK_L;
//    
//   return Data;
//}





//void DS1302Z_Read(void)
//{
//	  uint8_t temp = 0;

//		
//		//小时  24小时制
//		temp = DS1302_ReadBYTE(0x85,0,0);
//		DS1302_Time.Hour = ((temp>>4) &(0x0f))*10 + (temp &(0x0f));
//		
//		//分
//		temp = DS1302_ReadBYTE(0x83,0,0);
//		DS1302_Time.Minute = ((temp>>4) &(0x0f))*10 + (temp &(0x0f));
//		
//	  
//	  //秒    2022-4-11 16:10:28
//		//temp = DS1302_ReadBYTE(0x81,0,0);
//		//DS1302_Time.Minute = ((temp>>4) &(0x0f))*10 + (temp &(0x0f));
//	  
//	
//		//日
//		temp = DS1302_ReadBYTE(0x87,0,0);
//		DS1302_Time.Day = ((temp>>4) &(0x0f))*10 + (temp &(0x0f));
//		
//		//月
//		temp = DS1302_ReadBYTE(0x89,0,0);
//		DS1302_Time.Month = ((temp>>4) &(0x0f))*10 + (temp &(0x0f));
//		
//		//年
//		temp = DS1302_ReadBYTE(0x8d,0,0);		
//		DS1302_Time.Year = 2000 + ((temp>>4) &(0x0f))*10 + (temp &(0x0f));
//		
//		
//			if(DS1302_Time.Year == 2000)
//			  {
//				Time_Init();
//				DS1302Z_WriteQ();
//			  }
//			
//			
//		//测试时钟的输出   2022-4-11 15:39:24
//	  printf("\r\n DS1302_Time.Year:%d\r\n",DS1302_Time.Year);	
//		printf("\r\n DS1302_Time.Month:%d\r\n",DS1302_Time.Month);		
//		printf("\r\n DS1302_Time.Day:%d\r\n",DS1302_Time.Day);		
//		printf("\r\n DS1302_Time.Hour:%d\r\n",DS1302_Time.Hour);		
//		printf("\r\n DS1302_Time.Minute:%d \r\n\r\n ",DS1302_Time.Minute);	
//		//测试时钟的输出   2022-4-11 15:39:24
//			
//}



//void DS1302Z_WriteQ(void)  //一次全写入
//{
//	uint8_t temp = 0;

//	DS1302_WriteBYTE(0x8E,0x00,0);
//	DS1302_WriteBYTE(0x80,0x30,0);

//  //分钟
//	
//	temp = (DS1302_SetTime.Minute/10)<<4;
//	temp = temp + (DS1302_SetTime.Minute%10);		
//	DS1302_WriteBYTE(0x82,temp,0); 
//	
//	//小时  24小时制
//	temp = (DS1302_SetTime.Hour/10)<<4;
//	temp = temp + (DS1302_SetTime.Hour%10);		
//	DS1302_WriteBYTE(0x84,temp,0);
//	
//	//日
//	temp = (DS1302_SetTime.Day/10)<<4;
//	temp = temp + (DS1302_SetTime.Day%10);	
//	DS1302_WriteBYTE(0x86,temp,0);
//	
//	
//	//月
//	temp = (DS1302_SetTime.Month/10)<<4;
//	temp = temp + (DS1302_SetTime.Month%10);
//	DS1302_WriteBYTE(0x88,temp,0);
//	
//	//年
//	temp = ((DS1302_SetTime.Year/10)%10)<<4;
//	temp = temp + (DS1302_SetTime.Year%10);
//	DS1302_WriteBYTE(0x8C,temp,0);

//	
//	//写保护
//	DS1302_WriteBYTE(0x8E,0x80,0);
//	DS1302_CE_L;
//}








//测试3

/*
#include "includes.h"	
#include "stm32f4xx.h"
#include "PRTC.h"
#include "sys.h"
#include "delay.h"
 
 
typedef uint32_t u32;
typedef uint16_t u16;
typedef   uint8_t u8; 
 
 
 
 
struct TIMEData TimeData;

u8 read_time[7];
 
 
 
 
 
void ds1302_gpio_init()//CE,SCLK端口初始化    
		{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_5; //PE5   CE
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;         //输出
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;         //推挽输出	
		GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 5
		GPIO_ResetBits(GPIOE,GPIO_Pin_5); 
		 
			
			
		GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_3; //PE3  SCLK
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;         //输出
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;         //推挽输出	
			
		GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 3
		GPIO_ResetBits(GPIOE,GPIO_Pin_3); 
		}
 



void ds1032_DATAOUT_init()//配置双向I/O端口为输出态
		{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		 
		GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_4; //PE4 DATA
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;         //输出
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;         //推挽输出		
		GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 4
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
		}
 



void ds1032_DATAINPUT_init()//配置双向I/O端口为输入态
		{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4; //PE4 DATA
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
		GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4
		}
 
		
		
		
 
void ds1302_write_onebyte(u8 data)//向DS1302发送一字节数据
		{
		//ds1032_DATAOUT_init();
		u8 count=0;
		ds1032_DATAOUT_init();
			
		SCLK_L;
		//delay_us(2);	
		for(count=0;count<8;count++)
			{	
				SCLK_L;
				if(data&0x01)
				{
				DATA_H;
				}
				else
        {
				DATA_L;
				}        //先准备好数据再发送
				SCLK_H;  //拉高时钟线，发送数据
				data>>=1;
			}
		}
 
		
		
		
void ds1302_wirte_rig(u8 address,u8 data)//向指定寄存器地址发送数据
		{
		u8 temp1 = address;
		u8 temp2 = data;
			
		CE_L;
		//delay_us(2);//2022-4-24 14:16:07	
		SCLK_L;
		delay_us(1);//1   
			
		CE_H;
		delay_us(2);
			
		ds1302_write_onebyte(temp1);
		ds1302_write_onebyte(temp2);
			
		CE_L;
		//delay_us(2);//1   2022-4-24 14:16:07		
		SCLK_L;
		delay_us(2);
			
		}
 
		
		
u8 ds1302_read_rig(u8 address)//从指定地址读取一字节数据
		{
		u8 temp3=address;
		u8 count=0;
		u8 return_data=0x00;
			
		CE_L;
		//delay_us(3);//2022-4-24 14:16:07	
		SCLK_L;
		delay_us(3);
			
		CE_H;
		delay_us(3);
			
		ds1302_write_onebyte(temp3);
		ds1032_DATAINPUT_init();//配置I/O口为输入
		delay_us(2);
			
		for(count=0;count<8;count++)
			{
			delay_us(2);//使电平持续一段时间
			return_data>>=1;
				
			SCLK_H;
			delay_us(4);//使高电平持续一段时间4    
			SCLK_L;
			delay_us(14);//延时14us后再去读取电压，更加准确
				
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))
			  {
				return_data=return_data|0x80;
				}
			
			}
			
		delay_us(2);
		CE_L;
		delay_us(1);//2022-4-24 14:16:07
		DATA_L;
			
		return return_data;
		}
 


void ds1032_init()
		{
		ds1302_wirte_rig(0x8e,0x00);//关闭写保护功能
			
		ds1302_wirte_rig(0x80,0x37);//seconds37秒
		ds1302_wirte_rig(0x82,0x58);//minutes58分
		ds1302_wirte_rig(0x84,0x23);//hours23时
		ds1302_wirte_rig(0x86,0x30);//date30日
		ds1302_wirte_rig(0x88,0x09);//months9月
		ds1302_wirte_rig(0x8a,0x07);//days星期日
		ds1302_wirte_rig(0x8c,0x20);//year2020年
			
		ds1302_wirte_rig(0x8e,0x80);//打开写保护功能
		}
 
		
		
void ds1032_read_time()
		{
		read_time[0]=ds1302_read_rig(0x81);//读秒
		read_time[1]=ds1302_read_rig(0x83);//读分
		read_time[2]=ds1302_read_rig(0x85);//读时
		read_time[3]=ds1302_read_rig(0x87);//读日
		read_time[4]=ds1302_read_rig(0x89);//读月
		read_time[5]=ds1302_read_rig(0x8B);//读星期
		read_time[6]=ds1302_read_rig(0x8D);//读年
			
		printf("\r\n 读秒:%d \r\n ",read_time[0]);		
		printf("\r\n 读分:%d \r\n ",read_time[1]);		
		printf("\r\n 读时:%d \r\n ",read_time[2]);		
		printf("\r\n 读日:%d \r\n ",read_time[3]);		
		printf("\r\n 读月:%d \r\n ",read_time[4]);		
		printf("\r\n 读年:%d \r\n ",read_time[6]);	

			
		}
 
		
		
void ds1032_read_realTime()
		{
		ds1032_read_time();  //BCD码转换为10进制
				
			
		TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
		TimeData.minute=((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
		TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
		TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
		TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
		TimeData.week=read_time[5];
		TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;
			
			
		printf("\r\n year:%d \r\n ",TimeData.year);		
		printf(" month:%d \r\n ",TimeData.month);		
		printf(" day:%d \r\n ",TimeData.day);		
		printf(" hour:%d \r\n ",TimeData.hour);		
		printf(" minute:%d \r\n\r\n ",TimeData.minute);		
		printf(" second:%d \r\n\r\n ",TimeData.second);	
		}


*/



//测试4            

#include "includes.h"	
#include "stm32f4xx.h"
#include "PRTC.h"
#include "sys.h"
#include "delay.h"
 
 
typedef uint32_t u32;
typedef uint16_t u16;
typedef   uint8_t u8; 


uchar time_data[6]={0};//读取DS1302当前时间缓存矩阵

 /*****************************************
 * 函数名：void DS1302_GPIOInit(void)
 * 描述  ：    DS1302 GPIO配置
 * 输入  ：
 * 输出  ：无
 * 调用  ：
        CLK---PB5,
        IO--->PB6,
        RES--->PB7,
 *************************************/
void DS1302_GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructre;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);            /*open GPIO  clock*/
  GPIO_InitStructre.GPIO_Pin= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructre.GPIO_Speed=GPIO_Speed_50MHz;
  //GPIO_InitStructre.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructre.GPIO_Mode   = GPIO_Mode_OUT;         //输出
	GPIO_InitStructre.GPIO_OType  = GPIO_OType_PP;         //推挽输出
	
  GPIO_Init(DS1302_PORT, &GPIO_InitStructre);
}





 /*****************************************
 * 函数名：void DS1302_IO_GPIO(void)
 * 描述  ：    DS1302 之 IO GPIO 输入输出配置
 * 输入  ：FLAG标志位
 * 输出  ：无
* 调用  ：OUT:表示输出，IN:表示输入
          FLAG：
 *************************************/
void DS1302_IO_GPIO(uchar FLAG)
{
  GPIO_InitTypeDef GPIO_InitStructre;
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);            /*open GPIO  clock*/
    /**********配置数据IO端口 输出 **********/
    if(FLAG==0x01)
  {
    GPIO_InitStructre.GPIO_Pin= GPIO_Pin_4;//配置IO_GPIO
    GPIO_InitStructre.GPIO_Speed=GPIO_Speed_50MHz;
    //GPIO_InitStructre.GPIO_Mode=GPIO_Mode_Out_PP;
	  GPIO_InitStructre.GPIO_Mode   = GPIO_Mode_OUT;         //输出
		GPIO_InitStructre.GPIO_OType  = GPIO_OType_PP;         //推挽输出	
		
		
  GPIO_Init(DS1302_PORT, &GPIO_InitStructre);
  }
    /**********配置数据IO端口 输入**********/
    else if(FLAG==0x00)
  {
     GPIO_InitStructre.GPIO_Pin= GPIO_Pin_4;//配置IO_GPIO
   GPIO_InitStructre.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructre.GPIO_Mode=GPIO_Mode_IN;  //配置上拉输入  GPIO_Mode_IN      GPIO_Mode_IPU
   GPIO_Init(DS1302_PORT, &GPIO_InitStructre);
   }
}





  /*****************************************
 * 函数名：void DS1302_delay(u8 dd)
 * 描述  ：简单延时
 * 输入  ：
 * 输出  ：无
 * 调用  ：
 *************************************/
void DS1302_delay(u8 dd)
{
    u8 i;
    for(;dd>0;dd--)
  for(i=110;i>0;i--);
}






  /*****************************************
 * 函数名：void DS1302_Write(uchar add,uchar dat)
 * 描述  ：DS1302写指令和数据
*  输入  ：add:发送地址，dat：所在数据
 * 输出  ：无
 * 调用  ：
 *************************************/
 void DS1302_Write(uchar add,uchar dat)
 {
 uchar i,temp1,temp2;
 temp1=add;
 temp2=dat;
 RES_Set_1;//RET=1;
//发送地址
for(i=0;i<8;i++)
 {
        if(temp1&0x01)
         {IO_Set_1;  }     //IO=1;
        else
        { IO_Reset_0;}   //IO=0;
        temp1=temp1>>1;
        CLK_Set_1;            //CLK=1;
        DS1302_delay(2);
        CLK_Reset_0;    //CLK=0;
  }

	
	
	
	
	
//发送数据
for(i=0;i<8;i++)
    {
        /*IO=(temp2>>i)&0x01;这一句代替下面屏蔽的内容  */
        if(temp2&0x01)
         {IO_Set_1;  }     //IO=1;
        else
         { IO_Reset_0;}    //IO=0;
            temp2=temp2>>1;
            CLK_Set_1;            //CLK=1;
            DS1302_delay(2);
            CLK_Reset_0;    //CLK=0;
    }
RES_Reset_0;// RET=0;
 }




 
/*****************************************
 * 函数名：uchar DS1302_Read(uchar add)
 * 描述  ：DS1302读数据
 *  输入  ：add:发送所在地址
 * 输出  ：
 * 调用  ：
 *************************************/
uchar DS1302_Read(uchar add)
{
  uchar i,suf,temp,mm,nn,value;
   temp=add;
   RES_Set_1;//RET=1;
    //写地址
  for(i=0;i<8;i++)
      {
       if(temp&0x01)
       {IO_Set_1;  }     //IO=1;
      else
         { IO_Reset_0;}    //IO=0;
      temp=temp>>1;
        CLK_Set_1;            //CLK=1;
        DS1302_delay(2);
        CLK_Reset_0;    //CLK=0;
      }
 
			
			
			
//读数据
DS1302_IO_GPIO(IN);//配置IO为输入
  for(i=0;i<8;i++)
    {
     suf=suf>>1;//读数据变量
     if(IO_Read)    //IO=1
     {
         suf=suf|0x80;
     }
     else     //IO=0
     {
     suf=suf&0x7f;
     }

        CLK_Set_1;            //CLK=1;
        DS1302_delay(2);
        CLK_Reset_0;    //CLK=0;
    }
    RES_Reset_0;    // RET=0;

DS1302_IO_GPIO(OUT);//配置IO为输出,恢复正常状态
//数据处理转化十进制
mm=suf/16;
nn=suf%16;
value=mm*10+nn;
return value;
}





/*****************************************
 * 函数名：void DS1302_SetTime(uchar *ad)
 * 描述  ：DS1302 写入设置时间
 *  输入  ：add:发送所在地址
 * 输出  ：
 * 调用  ：
 *************************************/
void DS1302_SetTime(uchar *ad)
    {
     DS1302_Write(0x8e,0x00);   //WP=0 允许数据写入DS1302
		
     /**********以下对时分秒的初始化*************/
			
     DS1302_Write(ds1302_sec_addr,ad[5]/10*16+ad[5]%10);    //秒
     DS1302_Write(ds1302_min_addr,ad[4]/10*16+ad[4]%10);    //分
     DS1302_Write(ds1302_hour_addr,ad[3]/10*16+ad[3]%10);   //时
			
     /**********以下对年月日的初始化*************/
		
     DS1302_Write(ds1302_day_addr,ad[2]/10*16+ad[2]%10);    //日
     DS1302_Write(ds1302_month_addr,ad[1]/10*16+ad[1]%10);  //月
		 DS1302_Write(ds1302_year_addr,ad[0]/10*16+ad[0]%10);   //年   +2000  2022-4-26 14:29:27

     DS1302_Write(0x8e,0x80);   //0x8e控制字节地址,bit7=WP WP=1 禁止数据写入DS1302
    }


	
	
	
/*****************************************
 * 函数名： void DS1302_OFF(void)
 * 描述  ：DS1302时间禁止走时
 *  输入  ：
 * 输出  ：
 * 调用  ：
 *************************************/
  void DS1302_OFF(void)
  {
   uchar temp;
   temp=DS1302_Read(0x81);//读取表地址时间
   DS1302_Write(0x8e,0x00);//WP=0 允许数据写入DS1302
   temp=temp|(1<<7);
   DS1302_Write(0x80,temp);//WP=1 禁止数据写入DS1302

  }
	
	
	
	
	
/*****************************************
 * 函数名： void DS1302_ON(void)
 * 描述  ：DS1302时间开始运行，走时
 *  输入  ：
 * 输出  ：
 * 调用  ：
 *************************************/
  void DS1302_ON(void)
  {
   uchar temp;
   temp=DS1302_Read(0x81);
   DS1302_Write(0x8e,0x00);//WP=0 允许数据写入DS1302
   temp=temp|(0<<7);
   DS1302_Write(0x80,temp);//WP=0 允许数据写入DS1302

  }






 /*****************************************
 * 函数名：void DS1302_init(uchar *time)
 * 描述  ：    DS1302初始化
 * 输入  ：无
 * 输出  ：无
* 调用  ：
 *************************************/
void DS1302_init(uchar *time)
{
    DS1302_GPIOInit();//GPIO初始化配置
    DS1302_delay(2);
	
    RES_Reset_0;//RET=0;
    CLK_Reset_0;//CLK=0;
    //下面是对DS1302启动电池，不掉电   设置时间
    DS1302_Write(0x8e,0x00);//WP=0 允许数据写入DS1302
//    DS1302_Write(0x90,0xA7);//充电(1个二极管+8K电阻)
//    DS1302_Write(0x8E,0X80);//开启保护 WP=1
    if(DS1302_Read(0X81)&0X80)//查询DS302时钟是否启动,如果时钟停止走动：启动时钟+初始化时钟
    {
        DS1302_SetTime(time);//设置设置初始时钟
    }
    //否则跳过

}





 /*****************************************
 * 函数名：void DS1302_Readtime(void)
 * 描述  ：    DS1302时间读出
 * 输入  ：无
 * 输出  ：无
* 调用  ：
 *************************************/
void DS1302_Readtime(void)//
{
			time_data[0]=DS1302_Read( 0x8D);//年
			time_data[1]=DS1302_Read( 0x89);//月
			time_data[2]=DS1302_Read( 0x87);//日
	
			time_data[3]=DS1302_Read( 0x85);//时
			time_data[4]=DS1302_Read( 0x83);//分
			time_data[5]=DS1302_Read( 0x81);//秒
	
	
		printf("\r\n year:%d \r\n ",time_data[0]);		
		printf(" month:%d \r\n ",time_data[1]);		
		printf(" day:%d \r\n ",time_data[2]);		
		printf(" hour:%d \r\n ",time_data[3]);		
		printf(" minute:%d \r\n ",time_data[4]);		
		printf(" second:%d \r\n\r\n ",time_data[5]);	
	
}

