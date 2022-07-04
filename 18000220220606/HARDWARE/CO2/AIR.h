#ifndef __AIR_H
#define __AIR_H



#include "includes.h"	
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"

#include "WIFI.h"//含有自定义串口的函数



//声明结构体union 的，防止匿名定义报错
#if defined ( __CC_ARM   )
#pragma anon_unions
#endif



//整个二氧化碳接收数据的结构体的函数
#define CO2_RX_BUF_MAX_LEN    1024       //最大字节数     1024


#define CO2_USART(fmt, ...)    USART_printf (USART1, fmt, ##__VA_ARGS__) 






//相关宏定义
#define    Data_Head1           0x16     //数据帧头1为0xFF
#define    Data_Head2           0x05     //数据帧头2为0x86
#define    Data_Head3           0x01     //数据帧头2为0x86
#define	   Data_Length            8      //一帧有9个数据





extern uint16_t    CO2_Measure_data;










extern struct STRUCT_USART_Fram_CO2   //CO2数据帧      结构体
						{
								char Data_RX_BUF_CO2[CO2_RX_BUF_MAX_LEN];//接收数据的buffer数组数据    长度
								union 
								{
										__IO u16 InfAll_CO2;
										struct 
												{
												__IO u16 FramLength_CO2       :15;                               // 14:0 
												__IO u16 FramFinishFlag_CO2   :1;                                // 15 
												}InfBit_CO2;
								}; 
						}CO2_Fram_Record_Struct;


						
						
						
#define DE_CO2_NoRX_Num 15            //3个
#define DE_CO2_Rest_Time 100          //100*5ms = 500ms

						

void CO2_VCC_CTR_Init(void);          //CO2电源的控制引脚
extern void uart1_init_CO2(u32 bound);//usart上的串口初始化
void CO2_Sensor_VCC_Contrl(uint8_t n);//电源引脚控制
void CO2_Send_Data(void);	            //发送测量的数据，返回CO2浓度数值   11,01,01,ed
void Read_CO2_ppm(void);

void CO2_Tx();

uint8_t SINGLE_CO2_Calibration(uint16_t Data);
uint8_t CO2_AUTO_CONTRL_Cal_ON_OFF(uint8_t ON_OFF);





uint8_t  Fifo_RXInsert(uint8_t Fifo_Data);








/*
//包含头文件 

#include "sys.h"  

// 相关宏定义 
#define    USART2_RX_EN 			   1			 //0,不接收;1,接收.
#define    USART_REC_LEN  			200 


//外部变量 
extern  u16  CO2_data;
extern  u16  USART_BUF[USART_REC_LEN]; 

//函数声明
//void send_code(void);
void CO2_Tx(void);                  //发送检测命令
void USART1_Init(u32 bound);				//串口2初始化 
*/










#endif




