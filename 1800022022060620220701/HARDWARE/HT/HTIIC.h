#ifndef __HTIIC_H
#define __HTIIC_H
						 
#include "sys.h"
#include "delay.h"	
#include "usart.h" 
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8; 
						 
//IO方向设置
#define HTIIC_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define HTIIC_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

//IO操作函数	 
#define HTIIC_SCL        PBout(8) //SCL
#define HTIIC_SDA        PBout(9) //输出SDA	 
#define HTIIC_READ_SDA   PBin(9)  //输入SDA 

//IIC所有操作函数
void HTIIC_Init(void);                   //初始化IIC的IO口				 
void HTIIC_Start(void);				           //发送IIC开始信号
void HTIIC_Stop(void);	  			         //发送IIC停止信号
void HTIIC_Send_Byte(u8 txd);			       //IIC发送一个字节
  u8 HTIIC_Read_Byte(unsigned char ack); //IIC读取一个字节
  u8 HTIIC_Wait_Ack(void); 				       //IIC等待ACK信号
void HTIIC_Ack(void);					           //IIC发送ACK信号
void HTIIC_NAck(void);				           //IIC不发送ACK信号

void HTIIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
  u8 HTIIC_Read_One_Byte(u8 daddr,u8 addr);	  



#endif

