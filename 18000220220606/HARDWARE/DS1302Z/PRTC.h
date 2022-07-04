#ifndef __PRTC_H
#define __PRTC_H

/*

一、硬件方面可能存在的问题


1、电路焊接错误，有虚焊，接错线。

2、DS1302是坏的，假的，这种可能性不是太大，我试过多种国产的芯片，完全不能用的是少数，
但有些只是不稳定，误差大而已。


3、这一条是最重要的，就是32.768K的晶振用的不对，
DS1302要使用专用的晶振，即内部的电容是6P的。
世面上有多种32.768K的晶振，但内部电容有12P、20P等，
这些晶振用在DS1302上是不能起振的，我就为这个问题折腾过很长时间，
代码检查无数遍，电路检查无数遍，DS1302换过多个，
就是不走，最后仔细看DS1302的规格书，才发现要6P的晶振，
化几毛钱换了个6P的（淘宝上有的是），立马走起来了。
如果不走时，也可以试用手指按住电路板上晶振的两个焊点，
有时就会走起来，那可以肯定是晶振问题了。


二、软件问题
1、有不少朋友的代码是从书上或网上下的，经过改编来用的，
这里面要特别注意一个问题，就是各个函数里的延时函数，
如有一个延时函数delay_ms(5); 要求延时5MS,但是原来的程序的使用条件是单片机用4M晶振，
你移植后用在8M单片机环境中，那延时时间肯定不够了，
这时DS1302就可能不走。这种情况也经常出现在LCD显示屏中，
轻则出乱码，重则不显示，还有在红外遥控中也有这样的情况，
插在代码中的延时时间不对，也会不能接收到遥控信号，
所以提醒大家，在进行代码移植时，一定要注意晶振的大小，
如和自己的晶振不同时，要尝试修改延时函数的延时时间。


2、首次使用DS1302 时，要先按下面的步骤检查：先设置好分、秒，
然后看显示的时间和自己设置的时间是否一致，如果一致，
说明时序等没有问题，再看是否走时，不走时，
用手按住晶振的焊点看有没有变化，或用万用表的交流毫伏档量晶振两脚，
应有2V以上的读数，否则换6P的晶振。


3、一定要开启时钟，DS1302的秒寄存器，
在程序初始化时一定要有l类似write_ds 1302(0x80,0x00)的语句，
其中0x80是秒寄存器的地址，0x00 是写入的数据，
这个数据的最高位一定要是0，是0 才能开启时钟走时。


说走快了，就在晶振上并小电容，走慢了就窜个小电容

快并慢串


typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8; 

*/





////#define DS_SCLK_Pin         GPIO_Pin_3//GPIO_Pin_4
////#define DS_SCLK_GPIO_Port   GPIOE

////#define DS_IO_Pin           GPIO_Pin_4//GPIO_Pin_4
////#define DS_IO_GPIO_Port     GPIOE

////#define DS_CE_Pin           GPIO_Pin_4//GPIO_Pin_4
////#define DS_CE_GPIO_Port     GPIOE



//#include "sys.h"
//#include "delay.h"
//#include "usart.h" 
//#include "includes.h"	
//#include "stm32f4xx.h"

//typedef uint32_t u32;
//typedef uint16_t u16;
//typedef uint8_t u8; 




//#define BIT0    0x0001           
//#define BIT1    0x0002           
//#define BIT2    0x0004           
//#define BIT3    0x0008           
//#define BIT4    0x0010            
//#define BIT5    0x0020            
//#define BIT6    0x0040            
//#define BIT7    0x0080            
//#define BIT8    0x0100            
//#define BIT9    0x0200            
//#define BITA    0x0400           
//#define BITB    0x0800           
//#define BITC    0x1000            
//#define BITD    0x2000           
//#define BITE    0x4000          
//#define BITF    0x8000  




//typedef struct
//			{
//				uint16_t Year;
//				 uint8_t Month;
//				 uint8_t Day;
//				 uint8_t Hour;
//				 uint8_t Minute;	
//			}Canshu_Time;






////DS1302Z的简化版的SPI的接口初始化
////修改寄存器函数
//			
//#define DS1302CLK_H         GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_SET)        //时钟的高  GPIO_WriteBit
//#define DS1302CLK_L         GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_RESET)      //时钟的低  GPIO_WriteBit

//#define DS1302IO_SDA_H      GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_SET)        //数据的高  GPIO_WriteBit
//#define DS1302IO_SDA_L      GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_RESET)      //数据的低  GPIO_WriteBit

//#define DS1302_SDA_READ     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)          //数据口的读     GPIO_ReadInputDataBit
//#define DS1302_SCL_READ     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)          //时钟口的读     GPIO_ReadInputDataBit

//#define DS1302_CE_H         GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_SET)        //GPIO_WriteBit
//#define DS1302_CE_L         GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_RESET)      //GPIO_WriteBit






//void DS1302_Delay( int n);

//void DS1302_SDA_IN(void);
//void DS1302_SDA_OUT(void);

//void DS1302_SCK_OUT(void);
//void DS1302_SCK_IN(void);

//void DS1302_CE_OUT(void);
//void DS1302_CE_IN(void);


//void DS1302Z_WriteQ(void);  //一次全写入
//void DS1302Z_Read(void);

//void Time_Init(void);

//uint8_t DS1302_ReadBYTE(uint8_t Adr,uint8_t Data,uint8_t CorR);
//void DS1302_WriteBYTE(uint8_t Adr,uint8_t Data,uint8_t CorR);










//后来的侧测试2
// 
//#include "sys.h" 
//#include "delay.h"
//#include "usart.h" 
//#include "includes.h"	

//#include "stm32f4xx.h"


//typedef uint32_t u32;
//typedef uint16_t u16;
//typedef uint8_t u8; 



//#define CE_L    GPIO_ResetBits(GPIOE,GPIO_Pin_5)//E5拉低使能位
//#define CE_H      GPIO_SetBits(GPIOE,GPIO_Pin_5)//E5拉高使能位

//#define SCLK_L  GPIO_ResetBits(GPIOE,GPIO_Pin_3)//E3拉低时钟线
//#define SCLK_H    GPIO_SetBits(GPIOE,GPIO_Pin_3)//E3拉高时钟线

//#define DATA_L  GPIO_ResetBits(GPIOE,GPIO_Pin_4)//E4拉低数据线
//#define DATA_H    GPIO_SetBits(GPIOE,GPIO_Pin_4)//E4拉高数据线
// 
// 
// 
//struct TIMEData
//{
//	uint16_t year;
//	uint8_t  month;
//	uint8_t  day;
//	uint8_t  hour;
//	uint8_t  minute;
//	uint8_t  second;
//	uint8_t  week;
//};//创建TIMEData结构体方便存储时间日期数据



//extern struct TIMEData TimeData;                     //全局变量

//void ds1302_gpio_init();                             //ds1302端口初始化
//void ds1302_write_onebyte(uint8_t data);             //向ds1302发送一字节数据
//void ds1302_wirte_rig(uint8_t address,uint8_t data); //向指定寄存器写一字节数据
//uint8_t ds1302_read_rig(uint8_t address);            //从指定寄存器读一字节数据


//void ds1032_init();                                  //ds1302初始化函数
//void ds1032_DATAOUT_init();                          //IO端口配置为输出
//void ds1032_DATAINPUT_init();                        //IO端口配置为输入

//void ds1032_read_time();                             //从ds1302读取实时时间（BCD码）
//void ds1032_read_realTime();                         //将BCD码转化为十进制数据
// 








//测试3


 /*
 
#include "sys.h" 
#include "delay.h"
#include "usart.h" 
#include "includes.h"	

#include "stm32f4xx.h"


typedef uint32_t u32;
typedef uint16_t u16;
typedef  uint8_t  u8; 



#define CE_L    GPIO_ResetBits(GPIOE,GPIO_Pin_5)  //拉低使能位  E5
#define CE_H      GPIO_SetBits(GPIOE,GPIO_Pin_5)  //拉高使能位

#define SCLK_L  GPIO_ResetBits(GPIOE,GPIO_Pin_3)  //拉低时钟线  E3
#define SCLK_H    GPIO_SetBits(GPIOE,GPIO_Pin_3)  //拉高时钟线

#define DATA_L  GPIO_ResetBits(GPIOE,GPIO_Pin_4)  //拉低数据线  E4
#define DATA_H    GPIO_SetBits(GPIOE,GPIO_Pin_4)  //拉高数据线
 
 
 
struct TIMEData
			{
				u16 year;
				u8  month;
				u8  day;
				u8  hour;
				u8  minute;
				u8  second;
				u8  week;
			};//创建TIMEData结构体方便存储时间日期数据



extern struct TIMEData TimeData;//全局变量

void ds1302_gpio_init();//ds1302端口初始化
void ds1302_write_onebyte(u8 data);//向ds1302发送一字节数据
void ds1302_wirte_rig(u8 address,u8 data);//向指定寄存器写一字节数据
u8 ds1302_read_rig(u8 address);//从指定寄存器读一字节数据
void ds1032_init();//ds1302初始化函数
void ds1032_DATAOUT_init();//IO端口配置为输出
void ds1032_DATAINPUT_init();//IO端口配置为输入
void ds1032_read_time();//从ds1302读取实时时间（BCD码）
void ds1032_read_realTime();//将BCD码转化为十进制数据
 
*/





//测试4




#include "sys.h" 
#include "delay.h"
#include "usart.h" 
#include "includes.h"	

#include "stm32f4xx.h"


typedef uint32_t u32;
typedef uint16_t u16;
typedef  uint8_t  u8; 



#define uchar unsigned char
#define uint unsigned int
/***DS1302时钟芯片****************************/
//CLK--->PE3
//IO---->PE4
//RES--->PE5
/****************************/

#define OUT   0x01
#define IN    0x00
extern uchar time_data[6];//时间存放数组

#define DS1302_PORT   GPIOE

#define CLK_Reset_0  GPIO_ResetBits(GPIOE,GPIO_Pin_3);          //GPIO_ResetBits(GPIOE,GPIO_Pin_4);
#define CLK_Set_1    GPIO_SetBits(GPIOE,GPIO_Pin_3);

#define IO_Reset_0   GPIO_ResetBits(GPIOE,GPIO_Pin_4);
#define IO_Set_1     GPIO_SetBits(GPIOE,GPIO_Pin_4);

#define RES_Reset_0  GPIO_ResetBits(GPIOE,GPIO_Pin_5);
#define RES_Set_1    GPIO_SetBits(GPIOE,GPIO_Pin_5);

#define IO_Read   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
/*************************/
#define Time_24_Hour      0x00       //24小时制
#define Time_Start        0x00       //开始走时
//DS1302寄存器操作指令定义时间地址
#define ds1302_sec_addr   0x80       //秒
#define ds1302_min_addr   0x82       //分
#define ds1302_hour_addr  0x84       //时
#define ds1302_day_addr   0x86       //日
#define ds1302_month_addr 0x88       //月
#define ds1302_year_addr  0x8c       //年
/********************/
void DS1302_GPIOInit(void);
void DS1302_IO_GPIO(uchar FLAG);//配置IO的方向
void DS1302_delay(u8 dd);
void DS1302_Write(uchar add,uchar dat);
uchar DS1302_Read(uchar add);
void DS1302_SetTime(uchar *ad);
void DS1302_OFF(void);
void DS1302_ON(void);
void DS1302_init(uchar *time);
void DS1302_Readtime(void);



#endif

