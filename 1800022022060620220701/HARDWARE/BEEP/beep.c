#include "beep.h" 

#include "sys.h"
#include "usart.h"	

#include "WIFI.h"
#include "MQTT.h"
#include "TCP.h"
#include "includes.h"	
#include "AIR.h"

//初始化PB14为输出口		    
//BEEP IO初始化



void BEEP_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  
  //初始化蜂鸣器对应引脚GPIOB14
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;        //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;       //下拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //初始化GPIO
	
	
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	delay_ms(500);
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);                    //蜂鸣器对应引脚GPIOF8拉低， 
	
	
}

