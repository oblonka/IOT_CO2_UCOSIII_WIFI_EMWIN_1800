#include "MQTT.h"
#include "WIFI.h"
#include "includes.h"	

#include "delay.h"


/*
1.这个函数加的delay 的时间   delay_ms(3000);
2.ESP32S_JoinAP的函数，只执行一次的话，配置delay 3000
3.加ESP32S_JoinAP的执行的循环的判断


1.最后



*/




/*
void ESP32S_STA_MQTTClient_Test_SA(void)//配置wifi    STA     
		{
				OS_ERR err;//通过系统的延时  
				
				char str[1024];
				printf("正在配置ESP32S参数\r\n");
				
			
			  //后面的设置不成功的原因，可能是前面测试的时候，已经配置好的这个MQTT这种透传的模式，每次连接的时候先退出透传的模式。
			  ESP32S_USART("%s","+++");//退出透传的指令，不用换行的字符串    这个指令是后来加的，暂时屏蔽。。
			                           //2022年6月10日   测试加上退出透传的指令进行发送 。
				printf("\r\n%s\r\n",ESP32S_Fram_Record_Struct.Data_RX_BUF);
			  delay_ms(3000);
			
			
				//1     AT+RESTORE 
				ESP32S_AT_Test();//恢复出厂默认模式                                                    
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(5000);
			
				//2     AT+CWMODE=1
				ESP32S_Net_Mode_Choose(STA);                                                           
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(5000);
			
				//3     AT+CWJAP="shian","shian623625"
				//单独加delay的延时，这个没有问题
				ESP32S_JoinAP(User_ESP32S_SSID, User_ESP32S_PWD); 
				//反复判断是否连接上的返回1
				//while(!ESP32S_JoinAP(User_ESP32S_SSID, User_ESP32S_PWD)); //while(0)执行一次就退出
				//延时1000ms
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);
				delay_ms(5000);
			
				//4     AT+MQTTUSERCFG=0,1,"aithinker","admin","public",0,0,""
				ESP32S_MQTTUSERCFG(User_ESP32S_client_id,User_ESP32S_username,User_ESP32S_password);   
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(3000);
				delay_ms(3000);
				
				
				//5     AT+MQTTCONN=0,"192.168.1.4",1883,0
				ESP32S_MQTTCONN( User_ESP32S_MQTTServer_IP, User_ESP32S_MQTTServer_PORT);              
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(3000);
				delay_ms(3000);
				
				
				//6     AT+MQTTPUB=0,"ESP32S/online","shian13564",0,0
				//AT+MQTTSUB=0,"ESP32S/EMQX",0
				ESP32S_MQTTSUB( User_ESP32S_MQTTServer_Topic);//MQTT订阅的主题                         
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(5000);//ESP32S/online
				//delay_ms(3000);																																			
																								
																																																
				printf("\r\n MQTT配置完成 ");   
				
while(1)
				{
				//测试的 数据的格式    SA_wifi_1800	
				//格式化发送字符串到MQTT服务器	
				sprintf(str,"SA_wifi_1800");  
				//发送数据到MQTT服务器  
				//发送到这个服务器的数据，字符串aithinker	
				MQTT_SendString(User_ESP32S_MQTTServer_Topic,str);             
				//设备端给emqx的MQTT服务器发送数据，这个就是订阅的是sub的那个主题，测试可知
				//emqx给设备端的发送数据的话是，是message的那个主题。   
				//发送完的话，串口可以接收到这个数据
				//通过这个与MQTT   云端发送数据
					
				//上面的测试调试的字符串的配置
				//后面加---温湿度数据的加载		 输出到MQTT,AZURE服务器端 ESP32S，500ms刷新一次
				//后面加---时钟的数据输出	     输出到MQTT,AZURE服务器端，连接到wifi后是实时的时间，500ms刷新一次
				//后面加---二氧化碳的数据输出  输出到MQTT服务器端，500ms刷新一次 	 
					
					
				}
		}
			
	*/	
			
			
			
			
			
			
void ESP32S_STA_MQTTClient_Test_SA_test(char * test_pSSID, char * test_pPassWord)//配置wifi    STA     
		{
				OS_ERR err;//通过系统的延时  
				
				char str[1024];
				printf("正在配置ESP32S参数\r\n");
				
			
			  //后面的设置不成功的原因，可能是前面测试的时候，已经配置好的这个MQTT这种透传的模式，每次连接的时候先退出透传的模式。
			  ESP32S_USART("%s","+++");//退出透传的指令，不用换行的字符串    这个指令是后来加的，暂时屏蔽。。
			                           //2022年6月10日   测试加上退出透传的指令进行发送 。
				printf("\r\n%s\r\n",ESP32S_Fram_Record_Struct.Data_RX_BUF);
			  delay_ms(3000);
			
			
				//1     AT+RESTORE 
				ESP32S_AT_Test();//恢复出厂默认模式                                                    
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(5000);
			
				//2     AT+CWMODE=1
				ESP32S_Net_Mode_Choose(STA);                                                           
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(5000);
			
				//3     AT+CWJAP="shian","shian623625"
				//单独加delay的延时，这个没有问题
				ESP32S_JoinAP(test_pSSID, test_pPassWord); 
				//反复判断是否连接上的返回1
				//while(!ESP32S_JoinAP(User_ESP32S_SSID, User_ESP32S_PWD)); //while(0)执行一次就退出
				//延时1000ms
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);
				delay_ms(5000);
			
				//4     AT+MQTTUSERCFG=0,1,"aithinker","admin","public",0,0,""
				ESP32S_MQTTUSERCFG(User_ESP32S_client_id,User_ESP32S_username,User_ESP32S_password);   
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(3000);
				delay_ms(3000);
				
				
				//5     AT+MQTTCONN=0,"192.168.1.4",1883,0
				ESP32S_MQTTCONN( User_ESP32S_MQTTServer_IP, User_ESP32S_MQTTServer_PORT);              
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(3000);
				delay_ms(3000);
				
				
				//6     AT+MQTTPUB=0,"ESP32S/online","shian13564",0,0
				//AT+MQTTSUB=0,"ESP32S/EMQX",0
				ESP32S_MQTTSUB( User_ESP32S_MQTTServer_Topic);//MQTT订阅的主题                         
				//OSTimeDlyHMSM(0,0,6,5000,OS_OPT_TIME_PERIODIC,&err);//延时1000ms
				delay_ms(5000);//ESP32S/online
				//delay_ms(3000);																																			
																								
																																																
				printf("\r\n MQTT配置完成 ");   
				
while(1)
				{
				//测试的 数据的格式    SA_wifi_1800	
				//格式化发送字符串到MQTT服务器	
				sprintf(str,"SA_wifi_1800");  
				//发送数据到MQTT服务器  
				//发送到这个服务器的数据，字符串aithinker	
				MQTT_SendString(User_ESP32S_MQTTServer_Topic,str);             
				//设备端给emqx的MQTT服务器发送数据，这个就是订阅的是sub的那个主题，测试可知
				//emqx给设备端的发送数据的话是，是message的那个主题。   
				//发送完的话，串口可以接收到这个数据
				//通过这个与MQTT   云端发送数据
					
				//上面的测试调试的字符串的配置
				//后面加---温湿度数据的加载		 输出到MQTT,AZURE服务器端 ESP32S，500ms刷新一次
				//后面加---时钟的数据输出	     输出到MQTT,AZURE服务器端，连接到wifi后是实时的时间，500ms刷新一次
				//后面加---二氧化碳的数据输出  输出到MQTT服务器端，500ms刷新一次 	 
					
					
				}
		}
			
			
			
			
			
			
			
			
			
			
			