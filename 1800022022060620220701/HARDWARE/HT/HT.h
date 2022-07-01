#ifndef __HT_H
#define __HT_H
#include "HTIIC.h"

#include "sys.h"
#include "delay.h"	
#include "usart.h" 

/****************************************************上次测试，暂时屏蔽*********************************************************/
//#include <stdbool.h>
//#include <stdlib.h>
//#include <inttypes.h>


//#include "sys.h"
//#include "delay.h"
//#include "usart.h"
//#include "ILI93xx.h"

//#define IIC_CLOCK       RCC_AHB1Periph_GPIOB   //RCC_AHB1Periph_GPIOB
//#define IIC_GPIO        GPIOB
//#define IIC_SCL_PIN     GPIO_Pin_8
//#define IIC_SDA_PIN     GPIO_Pin_9

///* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */

//#define I2C_SCL_1()     PBout(8)=1
//#define I2C_SCL_0()     PBout(8)=0

//#define I2C_SDA_1()		  PBout(9)=1
//#define I2C_SDA_0()     PBout(9)=0

//#define I2C_SDA_READ()  GPIO_ReadInputDataBit(IIC_GPIO,IIC_SDA_PIN)

//#define I2C_WR	0		/* 写控制bit */
//#define I2C_RD	1		/* 读控制bit */



//#define SHT30_ADDR      0x44 //新地址

//typedef union 
//{
//    float fdata;
//    unsigned char cdata[4];
//}float_data;//定义联合体存储float数据，float类型的存储符合IEEE标准，可用于传输数据






//void i2c_Start(void);
//void i2c_Stop(void);
//void i2c_SendByte(uint8_t _ucByte);
//uint8_t i2c_ReadByte(uint8_t ack);
//uint8_t i2c_WaitAck(void);
//void i2c_Ack(void);
//void i2c_NAck(void);
//uint8_t i2c_CheckDevice(uint8_t _Address);
//void i2c_GPIO_Config(void);




//void SHT30_Init(void);
//void SHT30_SOFTRESET(void);
//void SHT30_General_RESET(void);
//void SHT30_Stop_Break(void);

//void SHT30_Read(void);
//void SHT30_SendBytes(u16 cmd,u8 stop);
//void SHT30_ART(void);
//void SHT30_Single_Shot(u8 *buffer);
//void SHT30_Periodic(u8 *buffer);
//void SHT30_Heater(u8 enable);
/**************************************************上次测试，暂时屏蔽*************************************************************/





void SHT30_Init(void);

void sht30_read_temp_humi(u16 *p);

int sht30_data_process(void);

int  crc8_compute(u8 *check_data, u8 num_of_data);

int sht30_crc8_check(u8 *p,u8 num_of_data,u8 CrcData);




#endif

