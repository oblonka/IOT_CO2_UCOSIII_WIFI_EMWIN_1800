#ifndef __CTIIC_H
#define __CTIIC_H


#include "sys.h"	    
#include "cst148.h"
				 
				 
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t   u8;	 
				  
					
//IO方向设置
#define  CST_SDA_IN()  {GPIOB->MODER&=~(3<<(2*11));GPIOB->MODER|=0<<2*11;}	//PB11输入模式
#define CST_SDA_OUT()  {GPIOB->MODER&=~(3<<(2*11));GPIOB->MODER|=1<<2*11;} 	//PB11输出模式


//触摸屏的IO口定义 
#define CST_IIC_SCL    PBout(10) 	//SCL     
#define CST_IIC_SDA    PBout(11) 	//SDA	 
#define CST_READ_SDA    PBin(11)  //输入SDA 

#define CST_IIC_RST		 PAout(4)   //148复位引脚		PA4   默认是单片机的复位的引脚   20220302
#define CST_IIC_INT 	 PBin(13)		//148中断引脚 未使用   PB13                      20220302




//IIC所有操作函数
void CST_IIC_Init(void);                   //初始化IIC的IO口		
void CST_IIC_Start(void);				           //发送IIC开始信号
void CST_IIC_Stop(void);	  				       //发送IIC停止信号

void CST_IIC_Send_Byte(u8 txd);			       //IIC发送一个字节
  u8 CST_IIC_Read_Byte(unsigned char ack); //IIC读取一个字节

  u8 CST_IIC_Wait_Ack(void); 				       //IIC等待ACK信号
void CST_IIC_Ack(void);					           //IIC发送ACK信号
void CST_IIC_NAck(void);					         //IIC不发送ACK信号

  u8 CST148_WR_Reg(u16 reg,u8 *buf,u8 len);//触摸 写入命令          寄存器地址  数据  数据长度
void CST148_RD_Reg(u16 reg,u8 *buf,u8 len);//触摸 读取寄存器地址    寄存器地址  数据  数据长度


#endif



