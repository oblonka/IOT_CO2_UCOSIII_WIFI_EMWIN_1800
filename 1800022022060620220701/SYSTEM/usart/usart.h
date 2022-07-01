#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"  

//串口1   CO2        [PA9 TX]   [PA10 RX]
//串口2   WiFi       [PA2 TX]   [PA3  RX]
//串口6   调试串口   [PC6 TX]   [PC7  RX]



#define USART_REC_LEN  			  200   //定义最大接收字节数 200
#define USART2_REC_LEN  			200  	//定义最大接收字节数 200     00点37分                                                                               
#define USART1_REC_LEN  			200



#define EN_USART6_RX 			   1		//使能（1）/禁止（0）     串口6接收
#define EN_USART2_RX 			   1		//使能（1）/禁止（0）     串口2接收          2022-4-14 17:22:34
#define EN_USART1_RX 			   1		//使能（1）/禁止（0）     串口1接收          2022-4-17 15:00:07	



extern u8  USART_RX_BUF[USART_REC_LEN];   //6   接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //2   接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //1




extern u16 USART_RX_STA;         		        //6接收状态标记	
extern u16 USART2_RX_STA_WIFI;         		  //2接收状态标记    2022-4-14 17:42:03
extern u16 USART1_RX_STA_CO2;         		  //2接收状态标记




//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);      //串口6的初始化
void uart2_init_wifi(u32 bound);//串口2         2022-4-14 17:41:00
void uart1_init_CO2(u32 bound); //串口1

//标准库默认的是单字符的发送，修改位多字节的发送
void USARTX_Send_data(USART_TypeDef * USARTx,u8 *s);//多字符串的发送

void CO2_Tx();



#endif








