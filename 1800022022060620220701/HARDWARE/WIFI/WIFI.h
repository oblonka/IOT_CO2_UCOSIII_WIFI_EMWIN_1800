#ifndef __WIFI_H
#define __WIFI_H 			   

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8; 


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


//ESP32S模式选择
typedef enum
			{
					STA,
					AP,
					STA_AP  
			}ENUM_Net_ModeTypeDef;





//网络传输层协议，枚举类型
typedef enum
			{
				 enumTCP,
				 enumUDP,
			} ENUM_NetPro_TypeDef;
			
			
			
//连接号，指定为该连接号可以防止其他计算机访问同一端口而发生错误
typedef enum
			{
				Multiple_ID_0 = 0,
				Multiple_ID_1 = 1,
				Multiple_ID_2 = 2,
				Multiple_ID_3 = 3,
				Multiple_ID_4 = 4,
				Single_ID_0   = 5,
		} ENUM_ID_NO_TypeDef;




//这些接口的时候

//#define ESP32S_RST_Pin                       GPIO_Pin_4               //复位管脚
//#define ESP32S_RST_Pin_Port                  GPIOA                    //复位 
//#define ESP32S_RST_Pin_Periph_Clock          RCC_AHB1Periph_GPIOB     //复位时钟  RCC_APB2Periph_GPIOA

//#define ESP32S_CH_PD_Pin                     GPIO_Pin_5               //使能管脚
//#define ESP32S_CH_PD_Pin_Port                GPIOA                    //使能端口
//#define ESP32S_CH_PD_Pin_Periph_Clock        RCC_AHB1Periph_GPIOB     //使能时钟   RCC_APB2Periph_GPIOA





#define ESP32S_RST_Pin_SetH       GPIO_SetBits(ESP32S_RST_Pin_Port,ESP32S_RST_Pin)
#define ESP32S_RST_Pin_SetL       GPIO_ResetBits(ESP32S_RST_Pin_Port,ESP32S_RST_Pin)


#define ESP32S_CH_PD_Pin_SetH     GPIO_SetBits(ESP32S_CH_PD_Pin_Port,ESP32S_CH_PD_Pin)
#define ESP32S_CH_PD_Pin_SetL     GPIO_ResetBits(ESP32S_CH_PD_Pin_Port,ESP32S_CH_PD_Pin)





#define ESP32S_USART(fmt, ...)    USART_printf (USART2, fmt, ##__VA_ARGS__)    
#define PC_USART(fmt, ...)         printf(fmt, ##__VA_ARGS__)       //这是串口打印函数，串口2，执行printf后会自动执行fput函数，重定向了printf。



#define RX_BUF_MAX_LEN   8192      //4096  ssid列表解析出来的数据字符串量很大    最大字节数  1024  



//使用的数据帧结构体的   结构体函数     结构体嵌套共用体，嵌套结构体
//共用体占用的内存等于最长的成员占用的内存。共用体使用了内存覆盖技术，同一时刻只能保存一个成员的值，如果对新的成员赋值，就会把原来成员的值覆盖掉。
extern struct STRUCT_USART_Fram   //数据帧结构体
						{
								char Data_RX_BUF[RX_BUF_MAX_LEN];//接收数据的buffer数组数据    长度
							
							
								union 
								{
										__IO u16 InfAll;
										struct 
										{
												__IO u16 FramLength       :15;                               // 14:0 
												__IO u16 FramFinishFlag   :1;                                // 15 
										}InfBit;
								}; 
								
								
							
						}ESP32S_Fram_Record_Struct;





//初始化和TCP功能函数
void ESP32S_Init(u32 bound);
void ESP32S_AT_Test(void);
bool ESP32S_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time);
void ESP32S_Rst(void);
bool ESP32S_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
						
bool ESP32S_JoinAP( char * pSSID, char * pPassWord );
						
						
						
//返回的ack内容是不定的内容，直接通过发送解析内容	  2022.05					
bool ESP32S_JoinAP_connect_list_parse( char *cmd,u32 time);//ap 连接列表	
//bool ESP32S_JoinAP_connect_list_parse();					
bool ESP32S_GET_RealTime_parse( char *cmd,u32 time);//获取SNTP网络授时实时						
						
						
						
						
bool ESP32S_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
bool ESP32S_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool ESP32S_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
bool ESP32S_UnvarnishSend ( void );
void ESP32S_ExitUnvarnishSend ( void );
u8 ESP32S_Get_LinkStatus ( void );
void USART_printf( USART_TypeDef * USARTx, char * Data, ... );




//MQTT功能函数
bool ESP32S_MQTTUSERCFG(char * pClient_Id, char * pUserName,char * PassWord);

bool ESP32S_MQTTCONN(char * Ip, int Num_MQTTCONN);//NUM这个的变量有重复定义的地方在USMART    ,修改为Num_MQTTCONN

bool ESP32S_MQTTSUB(char * Topic);

bool ESP32S_MQTTPUB(char * Topic,char *temp);

bool ESP32S_MQTTCLEAN(void);

bool MQTT_SendString(char * pTopic,char *temp2);




#endif



/*
串口解析出来的SSID，扫描出来的有空白的ssid，解析这个隐藏的名称，去掉
OK
AT+CWLAP
+CWLAP:(3,"",-47,"18:3c:b7:df:49:99",6,-1,-1,4,4,7,1)
+CWLAP:(3,"",-47,"18:3c:b7:df:49:95",6,-1,-1,4,4,7,1)
+CWLAP:(4,"shian",-48,"18:3c:b7:df:49:94",6,-1,-1,4,4,7,1)
+CWLAP:(4,"shian_Wi-Fi5",-48,"18:3c:b7:ff:49:99",6,-1,-1,4,4,7,1)
+CWLAP:(4,"bjzjhyjz622",-48,"00:94:ec:40:4f:cc",11,-1,-1,4,4,7,1)
+CWLAP:(3,"",-48,"00:94:ec:40:4f:cd",11,-1,-1,4,4,7,1)
+CWLAP:(3,"",-48,"00:94:ec:40:4f:d1",11,-1,-1,4,4,7,1)
+CWLAP:(3,"CU_5TMN",-51,"72:d8:3d:66:66:38",3,-1,-1,4,4,7,1)
+CWLAP:(4,"656",-52,"30:fc:68:eb:e6:03",6,-1,-1,4,4,7,0)
+CWLAP:(4,"XCYL",-59,"9c:a6:15:fa:3d:a3",1,-1,-1,4,4,7,0)
+CWLAP:(4,"GBSN",-64,"7c:b5:9b:59:e1:11",7,-1,-1,5,3,7,0)
+CWLAP:(4,"CNCEC",-65,"14:51:7e:67:65:be",11,-1,-1,4,4,7,0)
+CWLAP:(0,"",-65,"14:51:7e:67:65:c0",11,-1,-1,0,0,7,0)
+CWLAP:(0,"",-69,"14:51:7e:67:55:1c",11,-1,-1,0,0,7,0)
+CWLAP:(4,"bmkbmkbmk",-70,"54:a7:03:54:d2:8e",1,-1,-1,4,4,7,0)
+CWLAP:(4,"CNCEC",-70,"14:51:7e:67:55:1a",11,-1,-1,4,4,7,0)
+CWLAP:(3,"DIRECT-82-HP Laser 178nw",-74,"06:0e:3c:81:2c:82",1,-1,-1,4,4,6,0)
+CWLAP:(4,"",-74,"46:59:43:3c:45:78",4,-1,-1,4,4,7,0)
+CWLAP:(4,"kzl528",-75,"0c:4b:54:4a:76:62",1,-1,-1,4,4,7,0)
+CWLAP:(4,"CU-4578",-75,"44:59:43:7c:45:78",4,-1,-1,4,4,7,1)
+CWLAP:(3,"CU_LVbG",-75,"9c:69:65:1f:ca:bc",13,-1,-1,4,4,7,0)
+CWLAP:(3,"1",-76,"78:85:f4:7f:56:2c",1,-1,-1,4,4,7,1)
+CWLAP:(4,"KZL",-76,"cc:08:fb:d8:a1:26",1,-1,-1,4,4,7,0)
+CWLAP:(4,"CU_uyz4",-76,"54:46:17:03:05:c8",8,-1,-1,4,4,7,0)
+CWLAP:(3,"CU_T322",-77,"72:60:8c:03:44:a4",1,-1,-1,4,4,7,1)
+CWLAP:(4,"ahead",-81,"d8:3d:cc:03:2b:35",1,-1,-1,5,3,7,1)
+CWLAP:(3,"CU_76a4",-87,"38:3e:5b:28:c4:88",13,-1,-1,4,4,7,0)
+CWLAP:(4,"6209",-88,"d8:3d:cc:03:80:05",11,-1,-1,5,3,7,1)
+CWLAP:(4,"zh",-88,"78:53:0d:c9:81:dd",11,-1,-1,5,3,7,1)
+CWLAP:(3,"",-90,"78:2c:29:07:fc:70",6,-1,-1,4,4,7,0)
+CWLAP:(3,"",-91,"78:2c:29:07:fb:90",6,-1,-1,4,4,7,0)
+CWLAP:(4,"CU_rNDR",-91,"e0:38:3f:52:ab:31",9,-1,-1,4,4,7,0)
+CWLAP:(3,"",-92,"8c:5a:c1:99:84:b9",6,-1,-1,4,4,7,1)
+CWLAP:(3,"",-92,"78:2c:29:07:fa:30",11,-1,-1,4,4,7,0)
+CWLAP:(4,"TP-LINK_B849",-93,"60:3a:7c:51:b8:49",6,-1,-1,4,4,7,0)
+CWLAP:(3,"CU_t9jE",-93,"58:d0:61:2d:0e:34",9,-1,-1,4,4,7,1)
+CWLAP:(3,"",-94,"78:2c:29:07:fc:d0",6,-1,-1,4,4,7,0)
+CWLAP:(4,"XKJH",-94,"ec:6c:9f:b4:ff:06",11,-1,-1,5,3,7,0)
+CWLAP:(3,"",-94,"6c:b7:49:2c:fa:fd",11,-1,-1,4,4,7,0)
+CWLAP:(3,"zhang",-95,"6c:b7:49:2c:fa:fc",11,-1,-1,4,4,7,1)
+CWLAP:(3,"",-96,"78:2c:29:07:fc:10",11,-1,-1,4,4,7,0)

OK

 ecn = 151587081
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 151587081
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 151587081
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 151587081
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 151587081
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 151587081
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 3
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 3
 ssid =o?
 rssi =3
 mac =
 channel = 9
==========================
 ecn = 4
 ssid =shian
 rssi =-48
 mac =18:3c:b7:df:49:94
 channel = 6
==========================
 ecn = 4
 ssid =shian_Wi-Fi5
 rssi =-48
 mac =18:3c:b7:ff:49:99
 channel = 6
==========================
 ecn = 4
 ssid =bjzjhyjz622
 rssi =-48
 mac =00:94:ec:40:4f:cc
 channel = 11
==========================
 ecn = 3
 ssid =bjzjhyjz622
 rssi =-48
 mac =00:94:ec:40:4f:cc
 channel = 11
==========================
 ecn = 3
 ssid =bjzjhyjz622
 rssi =-48
 mac =00:94:ec:40:4f:cc
 channel = 11
==========================
 ecn = 3
 ssid =CU_5TMN
 rssi =-51
 mac =72:d8:3d:66:66:38
 channel = 3
==========================
 ecn = 4
 ssid =656
 rssi =-52
 mac =30:fc:68:eb:e6:03
 channel = 6
==========================
 ecn = 4
 ssid =XCYL
 rssi =-59
 mac =9c:a6:15:fa:3d:a3
 channel = 1
==========================
 ecn = 4
 ssid =GBSN
 rssi =-64
 mac =7c:b5:9b:59:e1:11
 channel = 7
==========================
 ecn = 4
 ssid =CNCEC
 rssi =-65
 mac =14:51:7e:67:65:be
 channel = 11
==========================
 ecn = 0
 ssid =CNCEC
 rssi =-65
 mac =14:51:7e:67:65:be
 channel = 11
==========================
 ecn = 0
 ssid =CNCEC
 rssi =-65
 mac =14:51:7e:67:65:be
 channel = 11
==========================
 ecn = 4
 ssid =bmkbmkbmk
 rssi =-70
 mac =54:a7:03:54:d2:8e
 channel = 1
==========================
 ecn = 4
 ssid =CNCEC
 rssi =-70
 mac =14:51:7e:67:55:1a
 channel = 11
==========================
 ecn = 3
 ssid =DIRECT-82-HP Laser 178nw
 rssi =-74
 mac =06:0e:3c:81:2c:82
 channel = 1
==========================
 ecn = 4
 ssid =DIRECT-82-HP Laser 178nw
 rssi =-74
 mac =06:0e:3c:81:2c:82
 channel = 1
==========================
 ecn = 4
 ssid =kzl528
 rssi =-75
 mac =0c:4b:54:4a:76:62
 channel = 1
==========================
 ecn = 4
 ssid =CU-4578
 rssi =-75
 mac =44:59:43:7c:45:78
 channel = 4
==========================
 ecn = 3
 ssid =CU_LVbG
 rssi =-75
 mac =9c:69:65:1f:ca:bc
 channel = 13
==========================
 ecn = 3
 ssid =1
 rssi =-76
 mac =78:85:f4:7f:56:2c
 channel = 1
==========================
 ecn = 4
 ssid =KZL
 rssi =-76
 mac =cc:08:fb:d8:a1:26
 channel = 1
==========================
 ecn = 4
 ssid =CU_uyz4
 rssi =-76
 mac =54:46:17:03:05:c8
 channel = 8
==========================
 ecn = 3
 ssid =CU_T322
 rssi =-77
 mac =72:60:8c:03:44:a4
 channel = 1
==========================
 ecn = 4
 ssid =ahead
 rssi =-81
 mac =d8:3d:cc:03:2b:35
 channel = 1
==========================
 ecn = 3
 ssid =CU_76a4
 rssi =-87
 mac =38:3e:5b:28:c4:88
 channel = 13
==========================
 ecn = 4
 ssid =6209
 rssi =-88
 mac =d8:3d:cc:03:80:05
 channel = 11
==========================
 ecn = 4
 ssid =zh
 rssi =-88
 mac =78:53:0d:c9:81:dd
 channel = 11
==========================
 ecn = 3
 ssid =zh
 rssi =-88
 mac =78:53:0d:c9:81:dd
 channel = 11
==========================
 ecn = 3
 ssid =zh
 rssi =-88
 mac =78:53:0d:c9:81:dd
 channel = 11
==========================
 ecn = 4
 ssid =CU_rNDR
 rssi =-91
 mac =e0:38:3f:52:ab:31
 channel = 9
==========================
 ecn = 3
 ssid =CU_rNDR
 rssi =-91
 mac =e0:38:3f:52:ab:31
 channel = 9
==========================
 ecn = 3
 ssid =CU_rNDR
 rssi =-91
 mac =e0:38:3f:52:ab:31
 channel = 9
==========================
 ecn = 4
 ssid =TP-LINK_B849
 rssi =-93
 mac =60:3a:7c:51:b8:49
 channel = 6
==========================
 ecn = 3
 ssid =CU_t9jE
 rssi =-93
 mac =58:d0:61:2d:0e:34
 channel = 9
==========================
 ecn = 3
 ssid =CU_t9jE
 rssi =-93
 mac =58:d0:61:2d:0e:34
 channel = 9
==========================
 ecn = 4
 ssid =XKJH
 rssi =-94
 mac =ec:6c:9f:b4:ff:06
 channel = 11
==========================
 ecn = 3
 ssid =XKJH
 rssi =-94
 mac =ec:6c:9f:b4:ff:06
 channel = 11
==========================
 ecn = 3
 ssid =zhang
 rssi =-95
 mac =6c:b7:49:2c:fa:fc
 channel = 11
==========================
 ecn = 3
 ssid =zhang
 rssi =-95
 mac =6c:b7:49:2c:fa:fc
 channel = 11
==========================









*/




