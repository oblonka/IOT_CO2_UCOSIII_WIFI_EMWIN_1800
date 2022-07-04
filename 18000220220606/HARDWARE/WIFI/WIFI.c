#include "WIFI.h"
#include "usart.h"
#include "delay.h"
#include <stdarg.h>
#include "includes.h"	
#include "sys.h"  

struct STRUCT_USART_Fram ESP32S_Fram_Record_Struct = { 0 };  //定义了一个数据帧结构体


char SSID_str[10][64] = {0};//全局


char Extract_ssid[256]={0};//全局变量 
uint32_t  offset_list_parse=0;//2022.05




//关于wifi ssid的解析的变量的定义，全局
	      int                ecn;
				char          ssid[64];
				int               rssi;
				char           mac[64];
				int            channel; 
				char*       ssid_list_ptr = NULL;
				int         list_offset = 0;
			  
			  char* str_buffer=NULL;//这个用来保存串口接收到的数组，   ESP32S_Fram_Record_Struct.Data_RX_BUF[]
				
				
				char  text_wifi[80];//char*类型 












void ESP32S_Init(u32 bound)
		{
     //修改前的IO的初始化			     里面加的是使能的引脚和复位的IO 的引脚，这个后面要加上的是直接IO控制，现在的调试的时候用的是按键进行外部的复位
      
			
			/*		  GPIO_InitTypeDef GPIO_InitStructure;

				RCC_APB2PeriphClockCmd(ESP32S_RST_Pin_Periph_Clock|ESP32S_CH_PD_Pin_Periph_Clock, ENABLE);

				GPIO_InitStructure.GPIO_Pin = ESP32S_RST_Pin;             
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //复用推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
				GPIO_Init(ESP32S_RST_Pin_Port, &GPIO_InitStructure);

				GPIO_InitStructure.GPIO_Pin = ESP32S_CH_PD_Pin;               
				GPIO_Init(ESP32S_CH_PD_Pin_Port, &GPIO_InitStructure);
*/
			
	

				
		   /*	 
			 GPIO_InitTypeDef GPIO_InitStructure;
			 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);     //使能GPIOB的时钟
			 GPIO_InitStructure.GPIO_Pin    =  ESP32S_RST_Pin;
			 GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;         //输出
			 GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;         //推挽输出
			 GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;      //上拉输出
			 GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;     //高速GPIO
			 GPIO_Init(ESP32S_RST_Pin_Port,&GPIO_InitStructure);
			 
			 
			 GPIO_InitStructure.GPIO_Pin = ESP32S_CH_PD_Pin;               
			 GPIO_Init(ESP32S_CH_PD_Pin_Port, &GPIO_InitStructure);
		   */	
			 
			 
			 
			
			
				uart2_init_wifi(bound);     //暂时屏蔽     
		    //		ESP32S_Rst();              //目前是手动复位，便于调试，后面加IO的调试。
		}






//对ESP32S模块发送AT指令        2022-4-20 10:05:21  命令的发送字符与收到的字符的  验证回复与操作               
// cmd 待发送的指令
// ack1,ack2;期待的响应，为NULL表不需响应，两者为或逻辑关系
// time 等待响应时间
//返回1发送成功， 0失败
bool ESP32S_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time)
{ 
	
	  OS_ERR err;//系统延时后加
	
	
    ESP32S_Fram_Record_Struct .InfBit .FramLength = 0; //重新接收新的数据包
    ESP32S_USART("%s\r\n", cmd);//这个是通过串口2,进行配置与 数据的转发的接口
	  //printf("%s\r\n", cmd);//通过串口6调试接口，进行调试输出打印log	
    if(ack1==0&&ack2==0)     //不需要接收数据
    {
    return true;
    }
		
		
    //delay_ms(time);   //延时   2022-4-20 09:37:20
		//delay_ms(1000);   //   2022-4-20 09:37:33
		
	  OSTimeDlyHMSM(0,0,0,time,OS_OPT_TIME_PERIODIC,&err);//延时 time ms 
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时 time ms 
		
		
    ESP32S_Fram_Record_Struct.Data_RX_BUF[ESP32S_Fram_Record_Struct.InfBit.FramLength ] = '\0';
		
    printf("%s",ESP32S_Fram_Record_Struct.Data_RX_BUF);
		
    if(ack1!=0&&ack2!=0)
    {
        return ( ( bool ) strstr ( ESP32S_Fram_Record_Struct.Data_RX_BUF, ack1 ) || 
                         ( bool ) strstr ( ESP32S_Fram_Record_Struct.Data_RX_BUF, ack2 ) );
    }
    else if( ack1 != 0 )  //strstr(s1,s2);检测s2是否为s1的一部分，是返回该位置，否则返回false，它强制转换为bool类型了
        return ( ( bool ) strstr ( ESP32S_Fram_Record_Struct.Data_RX_BUF, ack1 ) );

    else
        return ( ( bool ) strstr ( ESP32S_Fram_Record_Struct.Data_RX_BUF, ack2 ) );

}


//复位重启     暂时是硬件的按键的重启  调试 打印log
/*
void ESP32S_Rst(void)
{
    ESP32S_RST_Pin_SetL;
    delay_ms(500); 
    ESP32S_RST_Pin_SetH;
}

*/








//发送恢复出厂默认设置指令将模块恢复成出厂设置
void ESP32S_AT_Test(void)
{
	
    char count=0;
	
	  OS_ERR err;
	  OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时500ms    2022-4-20 09:39:34
    //delay_ms(1000); //OS_ERR err;   OSTimeDlyHMSM(0,0,0,1500,OS_OPT_TIME_PERIODIC,&err);//延时500ms   
    while(count < 10)
    {
        if(ESP32S_Send_AT_Cmd("AT+RESTORE","OK",NULL,500)) 
        {
            printf("OK\r\n");
            return;
        }
        ++ count;
    }
}





//选择ESP32S的工作模式,连接的MQTT 的云服务的话，直接使用MQTT的模式进行，设置WIFI的模式
// enumMode 模式类型
//成功返回true，失败返回false
bool ESP32S_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
    switch ( enumMode )
    {
        case STA:
            return ESP32S_Send_AT_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 

        case AP:
            return ESP32S_Send_AT_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 

        case STA_AP:
            return ESP32S_Send_AT_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 

        default:
          return false;
    }       
}





//ESP32S连接外部的WIFI
//pSSID WiFi帐号
//pPassWord WiFi密码
//设置成功返回true 反之false
bool ESP32S_JoinAP( char * pSSID, char * pPassWord)
{
    char cCmd [120];
	
    sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
    return ESP32S_Send_AT_Cmd( cCmd, "OK", NULL, 5000 );
}


//ESP32S 透传使能
//enumEnUnvarnishTx  是否多连接，bool类型
//设置成功返回true，反之false
bool ESP32S_Enable_MultipleId (FunctionalState enumEnUnvarnishTx )
{
    char cStr [20];

    sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );

    return ESP32S_Send_AT_Cmd ( cStr, "OK", 0, 500 );

}


//ESP32S 连接服务器
//enumE  网络类型
//ip ，服务器IP
//ComNum  服务器端口
//id，连接号，确保通信不受外界干扰
//设置成功返回true，反之fasle
bool ESP32S_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
    char cStr [100] = { 0 }, cCmd [120];

    switch (  enumE )
    {
        case enumTCP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
          break;

        case enumUDP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
          break;

        default:
            break;
    }

    if ( id < 5 )
        sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

    else
        sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

    return ESP32S_Send_AT_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );

}


//透传使能
//设置成功返回true， 反之false
bool ESP32S_UnvarnishSend ( void )
{
    if (!ESP32S_Send_AT_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ))
        return false;

    return 
        ESP32S_Send_AT_Cmd( "AT+CIPSEND", "OK", ">", 500 );

}


//ESP32S发送字符串
//enumEnUnvarnishTx是否使能透传模式
//pStr字符串
//ulStrLength字符串长度
//ucId 连接号
//设置成功返回true， 反之false
bool ESP32S_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
    char cStr [20];
    bool bRet = false;


    if ( enumEnUnvarnishTx )
    {
        ESP32S_USART ( "%s", pStr );

        bRet = true;

    }

    else
    {
        if ( ucId < 5 )
            sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

        else
            sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );

        ESP32S_Send_AT_Cmd ( cStr, "> ", 0, 1000 );

        bRet = ESP32S_Send_AT_Cmd ( pStr, "SEND OK", 0, 1000 );
  }

    return bRet;

}


//ESP32S退出透传模式
void ESP32S_ExitUnvarnishSend ( void )
{
    
	  OS_ERR err;
	  OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时500ms 
	
	  //delay_ms(1000);  //用操作系统的函数替代
    ESP32S_USART( "+++" );
    //delay_ms( 500 ); //用操作系统的函数替代 
	
	  OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms 
	
}


//ESP32S 检测连接状态
//返回0：获取状态失败
//返回2：获得ip
//返回3：建立连接 
//返回4：失去连接 
u8 ESP32S_Get_LinkStatus ( void )
{
    if (ESP32S_Send_AT_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
    {
        if ( strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, "STATUS:2\r\n" ) )
            return 2;

        else if ( strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, "STATUS:3\r\n" ) )
            return 3;

        else if ( strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, "STATUS:4\r\n" ) )
            return 4;       

    }

    return 0;
}

static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
    const char *s;
    int d;   
    char buf[16];


    va_list ap;
    va_start(ap, Data);

    while ( * Data != 0 )     // 判断数据是否到达结束符
    {                                         
        if ( * Data == 0x5c )  //'\'
        {                                     
            switch ( *++Data )
            {
                case 'r':                                     //回车符
                USART_SendData(USARTx, 0x0d);
                Data ++;
                break;

                case 'n':                                     //换行符
                USART_SendData(USARTx, 0x0a);   
                Data ++;
                break;

                default:
                Data ++;
                break;
            }            
        }

        else if ( * Data == '%')
        {                                     
            switch ( *++Data )
            {               
                case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++) 
                {
                    USART_SendData(USARTx,*s);
                    while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
                }
                Data++;
                break;

                case 'd':           
                    //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++) 
                {
                    USART_SendData(USARTx,*s);
                    while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
                }
                     Data++;
                     break;
                default:
                     Data++;
                     break;
            }        
        }
        else USART_SendData(USARTx, *Data++);
        while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );

    }
}















/*

bool ESP32S_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time)
{ 
	
	  OS_ERR err;//系统延时后加
	
    ESP32S_Fram_Record_Struct .InfBit .FramLength = 0; //重新接收新的数据包
    ESP32S_USART("%s\r\n", cmd);//这个是通过串口2,进行配置与 数据的转发的接口
	
    if(ack1==0&&ack2==0)     //不需要接收数据
    {
    return true;
    }
		
    //delay_ms(time);   //延时   2022-4-20 09:37:20
		//delay_ms(1000);   //   2022-4-20 09:37:33
		
	  OSTimeDlyHMSM(0,0,0,time,OS_OPT_TIME_PERIODIC,&err);//延时 time ms 
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时 time ms 
		
    ESP32S_Fram_Record_Struct.Data_RX_BUF[ESP32S_Fram_Record_Struct.InfBit.FramLength ] = '\0';
		
    printf("%s",ESP32S_Fram_Record_Struct .Data_RX_BUF);
    if(ack1!=0&&ack2!=0)
    {
        return ( ( bool ) strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, ack1 ) || 
                         ( bool ) strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
    }
    else if( ack1 != 0 )  //strstr(s1,s2);检测s2是否为s1的一部分，是返回该位置，否则返回false，它强制转换为bool类型了
        return ( ( bool ) strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, ack1 ) );

    else
        return ( ( bool ) strstr ( ESP32S_Fram_Record_Struct .Data_RX_BUF, ack2 ) );

}

*/


//解析可用WiFi 列表
//解析连接WiFi后时间的获取
/*
[16:08:11.785]收←◆AT+CWLAP

+CWLAP:(3,"loong",-36,"7e:c8:7e:35:08:73",11,-1,-1,4,4,7,0)
+CWLAP:(4,"TP-12345678",-76,"34:96:72:b1:d8:a9",11,-1,-1,4,4,7,0)
+CWLAP:(4,"MERCURY_4F5E",-81,"bc:5f:f6:4e:4f:5e",13,-1,-1,4,4,7,0)

OK
*/


/*
[16:01:55.398]收←◆AT+CIPSNTPTIME?
+CIPSNTPTIME:Sat May 28 16:01:58 2022
OK
*/





//wifi_con   获取WiFi列表   
/*
正则表达式提取不定长字符
AT命令返回结果为：
+CWLAP:(4,"TP-LINK_PocketAP_DFBCD4",-57,"38:83:45:df:bc:d4",6)
+CWLAP:(3,"LSTM",-61,"8c:21:0a:bc:8e:70",13)
+CWLAP:(3,"LabWiFi",-62,"68:3b:78:8f:16:23",1)
+CWLAP:(0,"GuestWiFi",-64,"68:3b:78:8f:16:22",1)

OK -
AT+CWLAP

先找到+CWLAP：
strstr(buffer+offset, "+CWLAP:");
使用sscanf+正则表达式提取ssid、mac、rssi：
sscanf(str+strlen("+CWLAP:"), "(%d,\"%[^\"]\",%d,\"%[^\"]\",", &ecn, ssid, &rssi, mac);
读取之后，将str指针移动到该行结束，进行下一轮的查找：

while (*str++ != '\n')
	 {
	offset++;
	}

*/



//bool ESP32S_JoinAP_connect_list_parse( char * pSSID, char * pPassWord)
//bool ESP32S_GET_RealTime_parse( char * pSSID, char * pPassWord)



//ESP32S连接外部的WIFI
//pSSID WiFi帐号
//pPassWord WiFi密码
//设置成功返回true 反之false

/*
这里只需要解析出对应的SSID就可以。
显示，GUI处理对应显示的链接的wifi






*/


bool ESP32S_JoinAP_connect_list_parse( char *list_parse_cmd,  u32 time)
//bool ESP32S_JoinAP_connect_list_parse()
		{
			char i = 0;
				/*
				AT+CWLAP
				
				+CWLAP:(4,"TP-LINK_PocketAP_DFBCD4",-57,"38:83:45:df:bc:d4",6)
				+CWLAP:(3,"LSTM",-61,"8c:21:0a:bc:8e:70",13)
				+CWLAP:(3,"LabWiFi",-62,"68:3b:78:8f:16:23",1)
				+CWLAP:(0,"GuestWiFi",-64,"68:3b:78:8f:16:22",1)
					
				
				rssi <= 0 && rssi >= -50       //信号最好
				rssi < -50 && rssi >= -70      //信号较好
				rssi < -70 && rssi >= -80      //信号一般
				rssi <-80  && rssi >= -100     //信号较差
				rssi没有在0～（-100)之间，     //表示无信号
				
			
				int day, year;
				char weekday[20], month[20], dtm[100];
				strcpy( dtm,"+loong:Thuriday May 12 2022" );
				sscanf( dtm, "%s %s %d  %d", weekday, month, &day, &year );
				
				//ret = sscanf(str, "+CSQ:%d,%d", &rssi, &ber);
				
				printf("%s %d, %d = %s\n", month, day, year, weekday );  
				return(0);
				
			  const char haystack[20] = "RUNOOB";
        const char needle[10] = "NOOB";
        char *ret;
        ret = strstr(haystack+2, needle);//返回需要的字符串 
        printf("子字符串是： %s\n", ret);
        return(0);
	      
				*/				
				
				
				
				OS_ERR err;//系统延时后加
				 
				//uint32_t  offset_list_parse=0;//2022.05
																        //&ecn, ssid, &rssi, mac    
				//uint32_t         ecn;         //数字，类型            //2022.05
				// int32_t        rssi;         //数字，信号强度        //2022.05
				//		char  ssid_list[32];      //字符串    名称        //2022.05
				//		char        mac[32];      //字符串  mac地址       //2022.05
			  //    char *str_ret=NULL;//指针的初始化一定要明确指向,初始化不能为空。
				//		uint32_t scan_num;
				//char Extract_ssid[256];
			
			
			
			
		/*	//实际使用的定义   测试全局变量
				int                ecn;
				char          ssid[32];
				int               rssi;
				char           mac[32];
				int            channel; 
				char*       ssid_list_ptr = NULL;
				int         list_offset = 0;
			  
			  char* str_buffer=NULL;//这个用来保存串口接收到的数组，   ESP32S_Fram_Record_Struct.Data_RX_BUF[]
				
				
				char  text_wifi[80];//char*类型 
			*/	
				
				
				
				
				
				
				
				
/*				
char* str = 
"+CWLAP:(4,\"TP-LINK_PocketAP_DFBCD4\",-57,\"38:83:45:df:bc:d4\",6)\r\n\
+CWLAP:(3,\"LSTM\",-61,\"8c:21:0a:bc:8e:70\",13)\r\n\
+CWLAP:(3,\"LabWiFi\",-62,\"68:3b:78:8f:16:23\",1)\r\n\
+CWLAP:(0,\"GuestWiFi\",-64,\"68:3b:78:8f:16:22\",1)\r\n\
\r\n\
OK\r\n";

int main()
{
     while(1){
         ptr = strstr(str+offset,"+CWLAP");
         if(!ptr)break;
         sscanf(ptr,"+CWLAP:(%d,\"%[^\"]\",%d,\"%[^\"]\",%d",&ecn, ssid, &rssi, mac,&channel);
         printf(" ecn = %d\r\n ssid =%s\r\n rssi =%d\r\n mac =%s\r\n channel = %d\r\n",ecn, ssid, rssi, mac,channel);
         printf("==========================\r\n");
         while (*ptr++ != '\n') 
         {  
             offset++; 
         }
     }
     system("pause");
     return 0;
 
 }     2022.06

*/
		    delay_ms(3000);
				//**********1
				ESP32S_USART("%s","+++");//退出透传的指令，不用换行的字符串    2022.06
				delay_ms(5000); 
				printf("\r\n\r\n");
				
				
				//**********2
				ESP32S_AT_Test();//先打印复位的指令，打印输出的OK ，在函数的里面    2022.06
				delay_ms(3000);  //2022-06
				delay_ms(3000);  
				
				
				ESP32S_Fram_Record_Struct.InfBit.FramLength = 0; //重新接收新的数据包
				
				
				//**********3     AT+CWMODE=1      AT+CWMODE=3 
				ESP32S_Net_Mode_Choose(STA);//先执行：AT+CWMODE=1 或 AT+CWMODE=3                                                           
				delay_ms(3000);
				

				//**********4    测试WIFI的连接的情况
				ESP32S_Send_AT_Cmd("AT+CWSTATE?","OK",NULL,500);//测试WIFI的连接的情况   2022.6
				printf("\r\n%s\r\n",ESP32S_Fram_Record_Struct.Data_RX_BUF);
				
				
				//**********5
				ESP32S_USART("%s\r\n", list_parse_cmd);//这个是通过串口2,进行配置与 数据的转发的接口  OK   2022.06
				delay_ms(5000);   //2022-4-20 09:37:33
			  printf("\r\n%s\r\n",ESP32S_Fram_Record_Struct.Data_RX_BUF); //调试口的打印输出   OK
				delay_ms(5000);   //2022-4-20 09:37:33
				delay_ms(5000);   //2022-4-20 09:37:33
				
				
				ESP32S_Fram_Record_Struct.Data_RX_BUF[ESP32S_Fram_Record_Struct.InfBit.FramLength] = '\0';
				
				
			  //Extract_ssid[offset_list_parse]=*ssid_list; //通过偏移量将提取到的ssid存到数组
				//这里提取10组为一个单位，界面显示的列表
				//printf("\r\n %d : %c \r\n",offset_list_parse,Extract_ssid[offset_list_parse]); //打印字符串
				//调用这个数组[0],直接调用			 
			   
				str_buffer=ESP32S_Fram_Record_Struct.Data_RX_BUF;//2022.06将wifi返回的接收数组地址赋予指针
				//int buffer[5],*pa;   pa = buffer;  /* pa = &buffer[0]; */ 
				//测试用循环的扫描的解析SSID,后面改成调用一次扫描一次，显示一次的结果。 
				while(1)
							 { //2022.06
								 ssid_list_ptr = strstr(str_buffer+list_offset,"+CWLAP");
								 //ptr = strstr(ESP32S_Fram_Record_Struct.Data_RX_BUF+list_offset,"+CWLAP");
								 
								 if(!ssid_list_ptr)break;
								 sscanf(ssid_list_ptr,"+CWLAP:(%d,\"%[^\"]\",%d,\"%[^\"]\",%d",&ecn, ssid, &rssi, mac,&channel);
								 
								 //sscanf(SSID_str[i],"%s",ssid);
								 
								 
								 printf(" ecn = %d\r\n ssid =%s\r\n rssi =%d\r\n mac =%s\r\n channel = %d\r\n",ecn, ssid, rssi, mac,channel);
								 printf("===========================\r\n");
								  while (*ssid_list_ptr++ != '\n') 
												 {  
													 list_offset++; 
												 }
									
delay_ms(3000);

										i++;		 
								}	
			return 0;		 		 	
}
								 





								 //数组 str     if(str[0]=='\0') 就是空
								 //指针         if(*p=='\0') 就是空 
								 //if( str == NULL || strlen(str) == 0 ) 
								 //if(!str || !*str)puts("str是空");
								 //if(str && *str)puts("str不是空");
								 
								 //if(ssid[0]!='\0')//判断是否ssid的处理是非空的，扫描测试的有显示不可见的wifi,显示出的是空的字符串。处理
								 
								 
								 //if( (*ssid != NULL) && (strlen(ssid) != 0) )//SSID的非空显示判断 
								 //if( (*ssid != NULL)这样的判断容易引起错误的指示
								/* 
								 if(str != NULL)
									 {
                    if(strlen(str) == 0) 
							        {
                      // it is empty string
                      }
                   }
								 */
								 
								 
								 //if(*ssid != NULL)
									// {
                  //  if(strlen(ssid) != 0) 
								     // if(strlen(ssid) != 0) 
								 	  // { 
									  //  printf(" ecn = %d\r\n ssid =%s\r\n rssi =%d\r\n mac =%s\r\n channel = %d\r\n",ecn, ssid, rssi, mac,channel);
									  //  printf("===========================\r\n");
								 	  // }
								 
								  // }
								 //sprintf(text_wifi, "%s", ssid);
								 
								 
								 
											// while (*ssid_list_ptr++ != '\n') 
											//			 {  
											//				 list_offset++; 
											//			 }
						   // }	
		//	return 0;		 		 
			
//}

		
		
		
		

bool ESP32S_GET_RealTime_parse(char *cmd,u32 time)
		 {
			 /*
			 [16:01:55.398]收←◆AT+CIPSNTPTIME?
				+CIPSNTPTIME:Sat May 28 16:01:58 2022
				OK
			 */
			 
        OS_ERR err;//系统延时后加
				 
				uint32_t offset_RealTime_parse=0;//2022.05
													                
			  /* &ecn, ssid, &rssi, mac */
			  char     *SNTP_week;     //SNTP_week     字符串  mac地址
			  char    *SNTP_month;     //SNTP_month    字符串    名称
			  int32_t    SNTP_day;     //&SNTP_day
			  int32_t   SNTP_time;     //&SNTP_time    数字，信号强度  //2022.05
				uint32_t  SNTP_year;     //&SNTP_year    数字，类型      //2022.05
				
				char Extract_realtime[256];
				
				ESP32S_Fram_Record_Struct.InfBit .FramLength = 0; //重新接收新的数据包
				ESP32S_USART("\r\n%s\r\n", cmd);//这个是通过串口2,进行配置与 数据的转发的接口
			
				delay_ms(1000);   //   2022-4-20 09:37:33
				OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时 time ms 
				
				ESP32S_Fram_Record_Struct.Data_RX_BUF[ESP32S_Fram_Record_Struct.InfBit.FramLength ] = '\0';
				
				printf("\r\n%s\r\n",ESP32S_Fram_Record_Struct.Data_RX_BUF);
						 
				//先找到+CWLAP：
				//strstr(buffer+offset, "+CWLAP:");
				strstr(ESP32S_Fram_Record_Struct.Data_RX_BUF+offset_RealTime_parse, "+CIPSNTPTIME:");
						
				//使用sscanf+正则表达式提取ssid、mac、rssi：   +CIPSNTPTIME:Sat May 28 16:01:58 2022    "(%d,\"%[^\"]\",%d,\"%[^\"]\","    
				sscanf(ESP32S_Fram_Record_Struct.Data_RX_BUF+strlen("+CIPSNTPTIME:"), "(\"%[^\"]\" \"%[^\"]\" %d %d %d", SNTP_week, SNTP_month, &SNTP_day, &SNTP_time,&SNTP_year);
				//读取之后，将str指针移动到该行结束，进行下一轮的查找：

				while ((*ESP32S_Fram_Record_Struct.Data_RX_BUF)++ != '\n')
						 {
							
							//Extract_realtime[offset]=*ssid; //通过偏移量将提取到的ssid存到数组
							 
							 Extract_realtime[0]=*SNTP_week;
							 Extract_realtime[1]=*SNTP_month;
							 Extract_realtime[2]=SNTP_day;
							 Extract_realtime[3]=SNTP_time;
							 Extract_realtime[4]=SNTP_year;
							 
							 printf("\r\n%c\r\n",Extract_realtime[offset_RealTime_parse]); //打印字符串
							 
							//调用这个数组[0],直接调用
							 
							offset_RealTime_parse++;//从0开始，保存一次，然后偏移量加1进行自增
						 }
		 }



		 
		 
		 
		 
		 






//下面为ESP32SMQTT功能指令

/*
*MQTT配置用户属性
*LinkID 连接ID,目前只支持0
*scheme 连接方式，这里选择MQTT over TCP,这里设置为1
*client_id MQTTclientID 用于标志client身份
*username 用于登录 MQTT 服务器 的 username
*password 用于登录 MQTT 服务器 的 password
*cert_key_ID 证书 ID, 目前支持一套 cert 证书, 参数为 0
*CA_ID 目前支持一套 CA 证书, 参数为 0
*path 资源路径，这里设置为""
*设置成功返回true 反之false
*/
bool ESP32S_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"", pClient_Id,pUserName,PassWord );
    return ESP32S_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
}


/*
*连接指定的MQTT服务器
*LinkID 连接ID,目前只支持0
*IP：MQTT服务器上对应的IP地址
*ComNum MQTT服务器上对应的端口号，一般为1883
*设置成功返回true 反之false
*/
bool ESP32S_MQTTCONN( char * Ip, int  Num)
{
    char cCmd [120];
    sprintf ( cCmd,"AT+MQTTCONN=0,\"%s\",%d,0", Ip,Num);
    return ESP32S_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
}

/*
*订阅指定连接的 MQTT 主题, 可重复多次订阅不同 topic
*LinkID 连接ID,目前只支持0
*Topic 订阅的主题名字，这里设置为Topic
*Qos值：一般为0，这里设置为1
*设置成功返回true 反之false
*/
bool ESP32S_MQTTSUB(char * Topic)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTSUB=0,\"%s\",1",Topic );
    return ESP32S_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
}


/*
*在LinkID上通过 topic 发布数据 data, 其中 data 为字符串消息
*LinkID 连接ID,目前只支持0
*Topic 订阅的主题名字，这里设置为Topic
*data：字符串信息
*设置成功返回true 反之false
*/
bool ESP32S_MQTTPUB( char * Topic,char *temp)
{
    char cCmd [120];
    sprintf (cCmd, "AT+MQTTPUB=0,\"%s\",\"%s\",1,0", Topic ,temp);
    return ESP32S_Send_AT_Cmd( cCmd, "OK", NULL, 1000 );
}

/*
*关闭 MQTT Client 为 LinkID 的连接, 并释放内部占用的资源
*LinkID 连接ID,目前只支持0
*Topic 订阅的主题名字，这里设置为Topic
*data：字符串信息
*设置成功返回true 反之false
*/
bool ESP32S_MQTTCLEAN(void)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTCLEAN=0");
    return ESP32S_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
}

//ESP32S发送字符串
//enumEnUnvarnishTx是否使能透传模式
//pStr字符串
//ulStrLength字符串长度
//ucId 连接号
//设置成功返回true， 反之false
bool MQTT_SendString(char * pTopic,char *temp2)
{
	  OS_ERR err;
	  //OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时500ms 
	
    bool bRet = false;
    ESP32S_MQTTPUB(pTopic,temp2);
	  
	  //delay_ms(1000);//这个用系统的延时函数替代        
	  OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms     2022-4-20 09:41:12
	  
	
    bRet = true;
    return bRet;

}

