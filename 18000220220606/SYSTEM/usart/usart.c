#include "sys.h"
#include "usart.h"	

#include "WIFI.h"
#include "MQTT.h"
#include "TCP.h"
#include "includes.h"	
#include "AIR.h"



//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucosiii 使用	  
#endif
 	  
 

 
//USART6   串口6  
//   PC6   TXD
//   PC7   RXD




extern u16  CO2_data;
extern u16  USART_BUF_co2[USART_REC_LEN];
extern uint16_t  CO2_TxBuffer[4];













//加入以下代码,支持printf函数,而不需要选择use MicroLIB	printf通过usart6来打印调试的log 
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    



void _sys_exit(int x) 
{ 
	x = x; 
} 



//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//     USART6  循环发送,直到发送完毕   
	USART6->DR = (u8) ch;       //     USART6
	return ch;
}
#endif
 


//调试串口的初始化     串口6
/*******************************************************************************************/
/**********************串口6   DEBUG********************************************************/
/*******************************************************************************************/
#if EN_USART6_RX   //如果使能了接收     初始化串口6  
//串口6中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];      //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	


//初始化IO 串口6
//bound:波特率
void uart_init(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);     //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);    //使能USART6时钟   串口6
 
	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);  //GPIOA9复用为USART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);  //GPIOA10复用为USART6
	
	//USART6端口配置
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7; //GPIOC6与GPIOC7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	       //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);                    //初始化PC6，PC7

  //USART6 初始化设置
	USART_InitStructure.USART_BaudRate            = bound;                           //波特率设置
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;             //字长为8位数据格式
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;                //一个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No;                 //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	 //收发模式
  USART_Init(USART6, &USART_InitStructure);                                        //初始化串口6
	
  USART_Cmd(USART6, ENABLE);                                                       //使能串口6
	
	//USART_ClearFlag(USART6, USART_FLAG_TC);
	
	
	
	
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart6 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;   //串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;             //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;		          //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                      //根据指定的参数初始化VIC寄存器、

#endif
	
}


//串口6中断服务程序

void USART6_IRQHandler(void)                	//串口6中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART6);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

 








//wifi的串口初始化   wifi
/*******************************************************************************************/
/*****************************USART2的初始化   WIFI*****************************************/
/*******************************************************************************************/
#if EN_USART2_RX   //如果使能了接收     初始化串口2
//串口6中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART2_RX_STA=0;                //接收状态标记	

u16 USART2_RX_STA_WIFI=0;             //接收状态标记	


//初始化IO 串口2 
//bound:波特率
void uart2_init_wifi(u32 bound)
	{
  //GPIO端口设置
  GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);     //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);    //使能USART2时钟   串口2的时钟是不一样的   RCC_APB1Periph_USART2   2022-4-14 17:46:34
  //这个时钟的修改前后要一致     APB1的这个时钟树的接口    APB1
		
		
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);  //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);  //GPIOA3复用为USART2
	
	//USART2端口配置
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	       //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                    //初始化PA2，PA3

  //USART2初始化设置
	USART_InitStructure.USART_BaudRate            = bound;                           //波特率设置
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;             //字长为8位数据格式
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;                //一个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No;                 //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	 //收发模式
  USART_Init(USART2, &USART_InitStructure);                   //初始化串口2
	
	//2022-4-18 11:24:46     接收中断和总线空闲中断
	USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//配置了接收中断和总线空闲中断
	
	
  USART_Cmd(USART2, ENABLE);                                  //使能串口2
	
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	
	
	
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	//建议进行更换中断优先级的测试，比如从高到低试一下3，2，1，0
  //数字越小，优先级越高
	
  NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3   0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;//子优先级3  0
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                         //根据指定的参数初始化VIC寄存器、

#endif
	
}



















void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	 u8 Res_USART2;
	
	
	 u8 ucCh;
	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	
	
	
	
	/*
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res_USART2 =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		
		if((USART2_RX_STA_WIFI&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA_WIFI&0x4000)//接收到了0x0d
			{
				if(Res_USART2!=0x0a)USART2_RX_STA_WIFI=0;//接收错误,重新开始
				else USART2_RX_STA_WIFI|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res_USART2==0x0d)USART2_RX_STA_WIFI|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA_WIFI&0X3FFF]=Res_USART2 ;
					USART2_RX_STA_WIFI++;
					
					if(USART2_RX_STA_WIFI>(USART2_REC_LEN-1))    
						USART2_RX_STA_WIFI=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
	*/
	
	
//2022.04   串口2的接收中断
//该函数不仅会判断标志位是否置1，同时还会判断是否使能了相应的中断。
//所以在串口中断函数中，如果要获取中断标志位，通常使用该函数。
//------串口中断函数中使用。
if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
	{
	ucCh  = USART_ReceiveData( USART2 );
	USART_SendData(USART6,ucCh);//改为串口6  测试发送接口   暂时屏蔽掉  2022年5月28日
		
	if(ESP32S_Fram_Record_Struct.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
		{
		 ESP32S_Fram_Record_Struct.Data_RX_BUF[ESP32S_Fram_Record_Struct.InfBit.FramLength ++]  = ucCh;   
		}                      
	}


//2022.04	  串口2接收空闲中断
//如果总线空闲   　该函数不仅会判断标志位是否置1，
//同时还会判断是否使能了相应的中断。
//所以在串口中断函数中，如果要获取中断标志位，通常使用该函数。
//------串口中断函数中使用。
if( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )  
	{
	 ESP32S_Fram_Record_Struct.InfBit.FramFinishFlag = 1;

	 ucCh = USART_ReceiveData( USART2 );                                                              //由软件序列清除中断标志位（先读USART_SR,然后读USART_DR）
	 USART_SendData(USART6,ucCh);//改为串口6   测试发送接口  暂时屏蔽掉 2022年5月28日
	 TcpClosedFlag = strstr(ESP32S_Fram_Record_Struct.Data_RX_BUF, "CLOSED\r\n") ? 1 : 0;
	}  

	
	
	
	
	

#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	















//二氧化碳的串口初始化    CO2
/*******************************************************************************************/
/*****************************USART1的初始化   CO2******************************************/
/*******************************************************************************************/
#if EN_USART1_RX   //如果使能了接收     初始化串口1


//串口6中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART1_RX_STA_CO2=0;            //接收状态标记	

u16 USART1_RX_STA_CO2=0;              //接收状态标记	


//初始化IO 串口1
//bound:波特率
void uart1_init_CO2(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);     //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);    //使能USART1时钟   串口1的时钟是不一样的   RCC_APB1Periph_USART1   2022-4-14 17:46:34
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //GPIOA2复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);  //GPIOA3复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	       //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                    //初始化PA2，PA3

  //USART1初始化设置
	USART_InitStructure.USART_BaudRate            = bound;                           //波特率设置
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;             //字长为8位数据格式
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;                //一个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No;                 //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	 //收发模式
  USART_Init(USART1, &USART_InitStructure);                //初始化串口1
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断   2022.06
	USART_ITConfig(USART1, USART_IT_RXNE|USART_IT_IDLE, ENABLE);
	 //USART_GetFlagStatus(USART1,USART_FLAG_TC);
  USART_Cmd(USART1, ENABLE);                               //使能串口1
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	
	
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3    0   1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;		//子优先级3      0   1
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}


//USART_ClearFlag(USART1,USART_FLAG_TC);   //在发送第一个数据之前加入一条
void USARTX_Send_data(USART_TypeDef * USARTx,u8 *s)
{
 while(*s!='\0')
 { 
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC )==RESET); 
  USART_SendData(USARTx,*s);
  s++;
 }
}






void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	
	u8 CO2_DATA_Receive;    //接收数据
	
	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	
	
//2022.04   串口1的接收中断
//该函数不仅会判断标志位是否置1，同时还会判断是否使能了相应的中断。
//所以在串口中断函数中，如果要获取中断标志位，通常使用该函数。
//------串口中断函数中使用。

if(USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET )//接收中断
	{
		
	 //USART_ClearFlag(USART1, USART_FLAG_TC);
	 USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志.
		
		
	 CO2_DATA_Receive  = USART_ReceiveData( USART1 );
	 //USART_SendData(USART6,CO2_DATA_Receive);//改为串口6  测试发送接口   暂时屏蔽掉  2022年5月28日
		
	 //在规定的数组长度下，将接收到的数据放到这个数组里面。接收的数组用来进行指令的返回的接收，解析
	if(CO2_Fram_Record_Struct.InfBit_CO2.FramLength_CO2 < ( CO2_RX_BUF_MAX_LEN - 1 ) ) 
		{
		 CO2_Fram_Record_Struct.Data_RX_BUF_CO2[ CO2_Fram_Record_Struct.InfBit_CO2.FramLength_CO2 ++ ]  = CO2_DATA_Receive;   
		}                      
	}


	
//2022.04	  串口1接收空闲中断
//如果总线空闲   　该函数不仅会判断标志位是否置1，
//同时还会判断是否使能了相应的中断。
//所以在串口中断函数中，如果要获取中断标志位，通常使用该函数。
//------串口中断函数中使用。

/*
if( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )//空闲中断  
	{
	 CO2_Fram_Record_Struct.InfBit_CO2.FramFinishFlag_CO2 = 1;

	 CO2_DATA_Receive = USART_ReceiveData( USART1 ); //由软件序列清除中断标志位（先读USART_SR,然后读USART_DR）
	 //USART_SendData(USART6,CO2_DATA_Receive);//改为串口6   测试发送接口  暂时屏蔽掉 2022年5月28日
		
	 //TcpClosedFlag = strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
	}  
*/
		

	
	
	
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	




