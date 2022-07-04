#include "includes.h"	
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"

#include "AIR.h"

#include "KEYa.h"
#include "KEYAA.h"

#include "KEYnum.h"

#include "WIFIcon.h"

#include "ButtonUse.h"


#include "WIFI.h"//含有串口的自定义  


/*
串口1
TX  PA9
RX  PA10


1） 波特率：9600, DataBits: 8, StopBits: 1, Parity: No, Flow Control: No
2） 本协议数据，均为16 进制数据。如“46”为十进制的[70]；
3） [xx]为单字节数据(无符号，0-255)；双字节数据高字节在前，低字节在后；

TX 串口发送端（+3.3V）
RX 串口接收端（+3.3V）

上位机发送格式
起始符长度命令号数据1 …… 数据n 校验和
HEAD LEN CMD DATA1 …… DATAn CS
11H XXH XXH XXH …… XXH XXH

协议格式详细说明
起始符上位机发送固定为[11H]，模块应答固定为[16H]
长度帧字节长度，=数据长度+1（包括CMD+DATA）
命令号指令号
数据读取或者写入的数据，长度可变
校验和数据累加和，=256-(HEAD+LEN+CMD+DATA)

编号功能名称命令号
1 读取CO2 测量结果0x01
2 CO2 浓度值单点校准0x03
3 读取软件版本号0x1E
4 开启/关闭零点自校准以及零点自校准参数设置0x10
5 查询仪器编号0x1F


4.1 读取CO2 测量结果

发送：11 01 01 ED
应答：16 05 01 DF1- DF4 [CS]

功能：读取CO2 测量结果（单位：ppm）
说明：CO2 测量值= DF1*256 + DF2


注意：DF3-DF4 预留

应答实例：
应答：16 05 01 02 58 00 00 8B


说明：
十六进制换算为十进制：02 即02；58 即88
CO2 测量值= 02*256 + 88=600ppm



4.2 CO2 浓度值单点校准

发送：11 03 03 DF1 DF2 CS

应答：16 01 03 E6
功能：CO2 浓度值单点校准


说明：
1、单点校准目标值= DF1*256 + DF2。单位为ppm，范围为（400 ～ 1500 ppm）
2、进行CO2 单点校准之前，请确认当前环境CO2 值为单点校准目标值，稳定时间最少2 分钟以上。


例如：
当需要将模块单点校准到600ppm 时，发送命令：11 03 03 02 58 8F
十六进制换算为十进制：02 即02；58 即88
CO2 测量值= 02*256 + 88=600ppm


说明：
字符说明
DF1 预留（默认100）
DF2 校准使能（0：开启；2：关闭）
DF3 校准周期（1——15 可选，一般默认为7）
DF4 基准值高位（2 个字节）
DF5 基准值低位（2 个字节）
DF6 预留（一般默认100）
注意：DF4 和DF5 默认值为400，即DF4：01；DF5：90


4.4.1 开启零点自校准并设置参数
发送：11 07 10 64 00 07 01 90 64 78
应答：16 01 10 D9


4.4.2 关闭零点自校准
发送：11 07 10 64 02 07 01 90 64 76
应答：16 01 10 D9


*/




/* 私有变量 ------------------------------------------------------------------*/
u16  CO2_data;
u16  USART_BUF_co2[USART_REC_LEN];
uint16_t  CO2_TxBuffer[4]={0x11,0x01,0x01,0xed};
 

 struct STRUCT_USART_Fram_CO2 CO2_Fram_Record_Struct = { 0 };  //定义了串口接收CO2的数据结构体，一个数据帧结构体

//char RX_buffer_CO2_data[CO2_RX_BUF_MAX_LEN];//接收数据的buffer数组数据    长度

uint8_t         CO2_WorkMode = 0;   //读取数值   0：正常工作状态    1：校准工作状态
uint8_t               RX_Num = 0;
uint16_t              RX_CRC = 0;
uint16_t    CO2_Measure_data = 0;
uint8_t            RS232_One = 0;



//uint8_t RX_buffer_CO2_data[16];

/*需要发送的数据，串口的数据发送，解析计算返回的数据*/

//读取CO2 测量结果
uint8_t AIR_CO2_Read_data[4] = {0x11,0x01,0x01,0xed};        

//CO2浓度单点校准的
uint8_t AIR_CO2_Cali_data[6] = {0x11,0x03,0x03,0x01,0x90,0x58};


//开启零点自校准并设置参数
uint8_t  AIR_CO2_AutoCali_data_On[10] = {0x11,0x07,0x10,0x64,0x00,0x07,0x01,0x90,0x64,0x78}; 
//关闭零点自校准
uint8_t AIR_CO2_AutoCali_data_OFF[10] = {0x11,0x07,0x10,0x64,0x02,0x07,0x01,0x90,0x64,0x76};




uint8_t AIR_CO2_transmit_txrx_ERR = 0;   //CO2传感器错误  1:为传感器错误E1   2:为回复期
uint8_t AIR_CO2_transmit_NoRX_Num = 0;   //有多少次没有接收到CO2的数值的时候   显示E1


uint16_t  AIR_CO2_Rest_Time  = 0;  //重启时间
uint8_t  AIR_CO2_Rest_Flag  = 0;  //重启标识



void CO2_VCC_CTR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);     //使能GPIOD的时钟
	
	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;         //输出
	GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
	GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//GPIO_SetBits(GPIOD,GPIO_Pin_6);               //GPIOD 高电平
}



/*
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
*/





/*
uint8_t  Fifo_RXInsert(uint8_t Fifo_Data)    //接收插入数据
				{
					HAL_UART_Receive_IT(&huart2, &RS232_One, 1);
					RX_Shuzu[RX_Num] =  Fifo_Data;
					RX_Num++;
					if(RX_Num > sizeof(RX_Shuzu))
					{
					RX_Num = 0;
					}
					return 1;	
				}
*/


//CO2电源的开启和关闭的函数，复位CO2传感器的电源
void CO2_Sensor_VCC_Contrl(uint8_t n)
		{
			if(n == 1)
			  {	GPIO_SetBits(GPIOD,GPIO_Pin_6);}   //打开开关，给CO2传感器提供电源 	
      else if(n == 0)
			  {	GPIO_ResetBits(GPIOD,GPIO_Pin_6);} //关闭开关，给CO2传感器断开电源    	
			
		}

		
		
		

		
		
		
		
		//测量CO2的指令，确认发送完成      串口发送指令
void CO2_Send_Data(void)
		{	
		  //USARTX_Send_data(USART_TypeDef * USARTx,u8 *s);  函数原型
			
			
			USART_GetFlagStatus(USART1, USART_FLAG_TC);
			USART_ClearFlag(USART1,USART_FLAG_TC);
			//USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_ClearFlag(USART1, USART_FLAG_TC); //防止出现stm32第一个字符丢失的现象，添加这一句
			CO2_USART("%s", AIR_CO2_Read_data);
			delay_ms(5000);
			
			
			//USARTX_Send_data(USART1,AIR_CO2_Read_data);//这个是多字节的发送函数这个的，修改的多字节的函数发送
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET)
			//			{
			//			}
				
			
			
      //USART_ClearFlag(USART1, USART_FLAG_TC);

						
			//HAL_UART_Receive_IT(&huart2, &RS232_One, 1);
			//while(HAL_UART_Transmit(&huart2,CO2_ReadCon,4,0xFFFF) != HAL_OK )
			//{		
			//}
						
		}

//
//发送：11 01 01 ED
//应答：16 05 01 DF1- DF4 [CS]

//功能：读取CO2 测量结果（单位：ppm）
//说明：CO2 测量值=  DF1*256 + DF2
//		
		
		
		
//串口读取  ，发送指令的返回数据		
void Read_CO2_ppm(void)
{
	//uint8_t j = 0;
	
	   char j = 0;
	uint8_t i = 0;
	
	
	
	char* CO2_str_buffer=NULL;
	
	
	
	CO2_Send_Data(); 
	//delay_ms(50000);
  //delay_ms(50000);
	
	CO2_Fram_Record_Struct.InfBit_CO2.FramLength_CO2 = 0; //重新接收新的数据包
				
	CO2_Fram_Record_Struct.Data_RX_BUF_CO2[CO2_Fram_Record_Struct.InfBit_CO2.FramLength_CO2] = '\0';
	
	CO2_str_buffer= CO2_Fram_Record_Struct.Data_RX_BUF_CO2;
	
	
	
	
	//ESP32S_Fram_Record_Struct.Data_RX_BUF[ESP32S_Fram_Record_Struct.InfBit.FramLength] = '\0';
	
	//if(AIR_CO2_transmit_txrx_ERR != 1)
	//{
	//	AIR_CO2_transmit_NoRX_Num++;
	
	//for(j = 0;j<sizeof(CO2_str_buffer);j++)
	for(j = 0;j<8;j++)
		{			
			//delay_ms(1000);	
			printf("\r\n CO2_str_buffer[ %d ]=  %02X  \r\n",j,CO2_str_buffer[j]);
		  //delay_ms(1000);	
			
			//if((CO2_str_buffer[j] == 0x16)&&(CO2_str_buffer[j+1] == 0x05)&&(CO2_str_buffer[j+2] == 0x01))
			
		if((CO2_str_buffer[j+1] == 0x05)&&(CO2_str_buffer[j+2] == 0x01))	
			{
//				RX_CRC = 0;
//				for(i= 0;i<7;i++)
//						{
//						RX_CRC = RX_CRC + CO2_str_buffer[j+i];
//						}
//						
//				RX_CRC = (unsigned int)0x100 - (unsigned char)RX_CRC;
//						
//				if(RX_CRC == CO2_str_buffer[j+7])
						{
							CO2_Measure_data = CO2_str_buffer[j+3] * 256 + CO2_str_buffer[j+4];  		
							//AIR_CO2_transmit_NoRX_Num = 0;	
							//AIR_CO2_transmit_txrx_ERR = 0;	
							
							//delay_ms(1000);	
							printf("\r\n CO2_Measure_data:%d \r\n",CO2_Measure_data);	//2022.06
							//delay_ms(1000);
							
						}
			}	
			
			/*
			if(CO2_NoRX_Num >= DE_CO2_NoRX_Num)   //错误数达到的时候  显示E1
			{
				CO2_TXRX_ERR = 1;
				CO2_Rest_Time = 0;        //重启时间
				CO2_Rest_Flag = 1;        //重启标识		
				CO2Sensor_VCCContrl(0);   //重启CO2
				CO2_NoRX_Num = 0;
			}
      */
			
		}
		//RX_Num = 0;
		//CO2_Send_Data(); 
	}
//}






/*
void Read_CO2Concentration(void)
{
	uint8_t j = 0;
	uint8_t i = 0;

	for(j = 0;j<sizeof(RX_buffer_data);j++)
	{
		if((RX_buffer_data[j] == 0x16)&&(RX_buffer_data[j+1] == 0x05)&&(RX_buffer_data[j+2] == 0x01))
		{
			RX_CRC = 0;
			for(i= 0;i<7;i++)
			{
				RX_CRC = RX_CRC + RX_buffer_data[j+i];
			}
			
			RX_CRC = (unsigned int)0x100 - (unsigned char)RX_CRC;
			
			if(RX_CRC == RX_buffer_data[j+7])
			{
				GasCO2_Measure = RX_buffer_data[j+3] * 256 + RX_buffer_data[j+4];  				
			}
		}	
	}


	RX_Num = 0;
	Read_CO2_ppm(); 
}
*/


uint8_t SINGLE_CO2_Calibration(uint16_t Data)
{
uint8_t CRC_Data = 0;
uint8_t i = 0;
//uint8_t j = 0;
char j = 0;
	
	
uint8_t k = 0;
	
uint8_t CO2_Calibration_Com[6] = {0x11,0x03,0x03,0x01,0x90,0x58};
	
CO2_Calibration_Com[3] = Data/256;
CO2_Calibration_Com[4] = Data%256;
	
CO2_WorkMode = 1;
	
//HAL_UART_Receive_IT(&huart2, &RS232_One, 1);
	
	
for(i = 0;i<5;i++)
		{
		CRC_Data = CRC_Data + CO2_Calibration_Com[i];
		}
	CO2_Calibration_Com[5] = 0x100 - (uint16_t)CRC_Data;
		
for(k = 0;k <10;k++)
	{
				RX_Num = 0;
				
		
				//HAL_UART_Receive_IT(&huart2, &RS232_One, 1);		
				//while(HAL_UART_Transmit(&huart2,CO2_CalibrationCom,6,0xFFFF) != HAL_OK )
				//{		}
				//Wait_Operation_Delay(50);
		
		
				delay_ms(1000);//50
				
	for(j = 0;j<sizeof(CO2_Fram_Record_Struct .Data_RX_BUF_CO2)-1;j++)
		{	
		if((CO2_Fram_Record_Struct .Data_RX_BUF_CO2[j] == 0x16)\
			&&(CO2_Fram_Record_Struct .Data_RX_BUF_CO2[j+1] == 0x01)\
		  &&(CO2_Fram_Record_Struct .Data_RX_BUF_CO2[j+2] == 0x03)\
		  &&(CO2_Fram_Record_Struct .Data_RX_BUF_CO2[j+3] == 0xe6))\
		
		  {	
			return 1;
			}
		}	
	}
	return 0;	
}


				
				
				
				
				
/*				
uint8_t CO2_AUTO_CONTRL_Cal_ON_OFF(uint8_t ON_OFF)
				{	
					uint8_t j = 0;
					
					if(ON_OFF == 0)  //开自动校准
						{
							//while(HAL_UART_Transmit(&huart2,CO2_AutoCalibrationOn,10,0xFFFF) != HAL_OK )
							{		
							}
						}
					else//关自动校准
						{
							//while(HAL_UART_Transmit(&huart2,CO2_AutoCalibrationOFF,10,0xFFFF) != HAL_OK )
							{		
							}
						}		
					
					for(j = 0;j<sizeof(RX_Shuzu);j++)
						{
							if((RX_Shuzu[j] == 0x16)&&(RX_Shuzu[j+1] == 0x01)&&(RX_Shuzu[j+2] == 0x10)&&(RX_Shuzu[j+3] == 0xd9))
							{
								return 1;
							}	
						}
						
					return 0;	
				}
*/











