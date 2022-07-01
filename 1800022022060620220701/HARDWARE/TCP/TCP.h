#ifndef __TCP_H
#define __TCP_H 			   
//#include "stm32f10x.h"
#include "stm32f4xx.h"

/*
*以下参数需要用户自行修改才能测试用过
*/

#define User_ESP32S_SSID             "miot_default"    //wifi名
#define User_ESP32S_PWD              "123456789x"      //wifi密码

#define User_ESP32S_TCPServer_IP     "192.168.31.16"   //服务器IP
#define User_ESP32S_TCPServer_PORT   "8888"            //服务器端口号


extern volatile uint8_t TcpClosedFlag;  //连接状态标志

void ESP32S_STA_TCPClient_Test(void);

#endif





