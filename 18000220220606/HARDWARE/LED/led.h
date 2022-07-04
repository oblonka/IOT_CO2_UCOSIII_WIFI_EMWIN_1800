#ifndef _LED_H
#define _LED_H
#include "sys.h"




//LED端口定义
#define LED0 PBout(3)
#define LED1 PBout(4)
#define LED2 PBout(5)
#define LED3 PBout(6)
#define LED4 PBout(7)




void LED_Init(void);  //初始化
#endif
