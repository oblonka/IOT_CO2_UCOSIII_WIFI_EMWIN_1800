#ifndef __MQTT_H
#define __MQTT_H 			   



#include "stm32f4xx.h"



/*
*配置以下的参数连接  2.4G的WiFi

本地测试的MQTT服务器的参数

"shian" 
"shian623625"

"admin"
"public" 
"192.168.1.4"
1883
"ESP32S/online"




*/






#define User_ESP32S_SSID              "shian"             //shian           TP-12345678    wifi名    "miot_default"    2.4G
#define User_ESP32S_PWD               "shian623625"       //shian623625     12345678       wifi密码  "123456789x"



#define User_ESP32S_client_id         "aithinker"         //MQTTclientID 用于标志client身份  最长256字节
#define User_ESP32S_username          "admin"						  //用于登录 MQTT 服务器 的 username, 最长 64 字节	
#define User_ESP32S_password			    "public"            //用于登录 MQTT 服务器 的 password, 最长 64 字节

#define User_ESP32S_MQTTServer_IP     "192.168.1.3"       //MQTT本地服务器IP     局域网IP    服务器  
#define User_ESP32S_MQTTServer_PORT   1883                //服务器端口号
#define User_ESP32S_MQTTServer_Topic  "ESP32S/online"     //订阅MQTT主题   ESP32S/EMQX   "topic"  ESP32S/online
                                                          //ESP32S/online    这个是设备端发往MQTT服务器的数据      上行的数据
                                                          //ESP32S/EMQX      这个的是MQTTT服务器发往设备端的数据   下行的数据


void ESP32S_STA_MQTTClient_Test_SA(void);//测试MQTT服务器的测试

void ESP32S_STA_MQTTClient_Test_SA_test(char * test_pSSID, char * test_pPassWord);//配置wifi    STA 


#endif


