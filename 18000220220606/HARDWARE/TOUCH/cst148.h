#ifndef __CST148_H
#define __CST148_H

#include <stdint.h>
#include <string.h>
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "ctiic.h"
//#include "lcd.h"
#include "ILI93XX.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t   u8;

//I2C读写命令	
#define CST148_CMD_WR 		0X34   //写命令
#define CST148_CMD_RD 		0X35	 //读命令

//进入模式的寄存器	
#define MODE_INFO 	    0XD101   //

//进入模式寄存器的查询寄存器
#define STYLE_ID 	      0XD204   //
#define STYLE_ID_jj 	  0XD205   //
#define STYLE_ID_IC 	  0XD206   //
#define STYLE_ID_hh 	  0XD207   //
#define STYLE_ID_FF     0XD208   //

//boot 寄存器
#define BOOTLOADER 	    0XD1FE   //
#define BOOTLOADER_TG   0XD1FF   //


//寄存器地址
#define CST_GSTID_REG 	0X0000   //当前检测到的触摸情况     ？？？



//五个触摸点的寄存器的判断
#define CST_TP1_REG 	  0XD000  	//第一个触摸点数据地址     这个就是单指的，当前检测到的触摸的情况，只需要单指的操作的寄存器。
#define CST_TP2_REG 	  0XD007		//第二个触摸点数据地址
#define CST_TP3_REG 	  0XD00C		//第三个触摸点数据地址
#define CST_TP4_REG 	  0XD011		//第四个触摸点数据地址
#define CST_TP5_REG 	  0XD016		//第五个触摸点数据地址  


//分辨率设置寄存器地址
#define CST_SET_REG   	0XD1F8   	//分辨率设置寄存器地址


//测试固定输出寄存器
#define Num             0XD005
#define Guding          0XD006    //固定值输出测试




#define CSTP_PRES_DOWN  0x80   //触屏被按下	  
#define CSTP_CATH_PRES  0x40   //有按键按下了 




#define CST_MAX_TOUCH    5     //10    电容屏支持的点数,固定为5点
#define MAX_TOUCH_POINT 0x0a   //最大的触摸点数是10个，固件中是5个的数字量



 //__IO uint8_t cst_PrevXfer_Complete=1;//
 
 
 
//触摸屏控制器
typedef struct
{
	         void (*init)(void);//初始化触摸屏控制器
	           u8 (*scan)(void);//扫描触摸屏.0,屏幕扫描;1,物理坐标;	  u8 mode
	           //u8 (*scan)(u8 mode);
	
	       u16 x[CST_MAX_TOUCH];//当前坐标
	       u16 y[CST_MAX_TOUCH];//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								              //x[4],y[4]存储第一次按下时的坐标. 
	
	    u8 tp_id[CST_MAX_TOUCH];//触摸手指id，区分多手指          
	   u8 tp_sta[CST_MAX_TOUCH];//触摸状态
	                    u8  sta;//笔的状态     0000 0000
								              //b7:按下1/松开0; 
	                            //b6:0,没有按键按下;1,有按键按下. 
								              //b5:保留
								              //b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
	
	
                 u8 touchtype;//触控的类型，备用
	
  u16   last_x[CST_MAX_TOUCH];
  u16   last_y[CST_MAX_TOUCH];
  u8   last_id[CST_MAX_TOUCH];
  u8  last_sta[CST_MAX_TOUCH];
	
}_cst_tp_dev;



extern _cst_tp_dev cstp_dev;	//触屏控制器在touch.c里面定义



void Load_Drow_Dialog(void);
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
u16 my_abs(u16 x1,u16 x2);
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);
void ctp_test(void);



static int  cst3xx_i2c_read( unsigned char *buf, int len);
static int  cst3xx_i2c_write( unsigned char *buf, int len);
static int  cst3xx_i2c_read_register( unsigned char *buf, int len);
static void cst3xx_reset_ic(unsigned int ms);




u8 TP_Init(void);
u8 HYN_CST148_Scan(void);
void MulTouch_Send(u16 *x,u16 *y,u8 *s,u8 *id,u8 n);




u8 HYN_CST148_Scan_Scan(u8 mode);



#endif

