#include "cst148.h"
#include "ctiic.h"

#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
//#include "lcd.h"
#include "ILI93XX.h"



//   20220125
u8 TouchBuf[61]={0};//后面的HYN_CST148_Scan(void)



 //5个触控点的颜色(电容触摸屏用)												 
const u16 POINT_COLOR_TBL[CST_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  POINT_COLOR=RED;//设置画笔蓝色 
}





//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}





//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  





//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  






//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   







//电容触摸屏测试函数
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//最后一次的数据 
	while(1)
	{
		cstp_dev.scan();
		for(t=0;t<5;t++)
		{
			//if((cstp_dev.sta)&(1<<t))
			if((1))
			{
				if(cstp_dev.x[t]<lcddev.width&&cstp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = cstp_dev.x[t];
						lastpos[t][1] = cstp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],cstp_dev.x[t],cstp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=cstp_dev.x[t];
					lastpos[t][1]=cstp_dev.y[t];
					if(cstp_dev.x[t]>(lcddev.width-24)&&cstp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0){}
		//LED0=!LED0;
		//printf("\r\n CTP TEST OK. \r\r\n");
	}	
}
	









//这些函数适应demo发送触摸坐标的函数。
static int cst3xx_i2c_read( unsigned char *buf, int len)
{
	int ret = 0;
	u8 i; 
	
 	CST_IIC_Start(); 
	CST_IIC_Send_Byte(CST148_CMD_RD);//发送写命令 	 
	CST_IIC_Wait_Ack(); 
	
	for(i=0;i<len;i++)
		{	   
		 buf[i]=CST_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
		} 
  CST_IIC_Stop();//产生一个停止条件 
	return ret;
}








static int cst3xx_i2c_write( unsigned char *buf, int len)
{
	int ret = -1;
	u8 i;

	CST_IIC_Start();	
	CST_IIC_Send_Byte(CST148_CMD_WR);	//发送写命令 	 
	ret=CST_IIC_Wait_Ack();
	if(ret)
		{
			//printf("1 CT_IIC_Wait_Ack error . \r\r\n");  //2022-4-7 14:25:51
	  };  
	for(i=0;i<len;i++)
	{	   
     CST_IIC_Send_Byte(buf[i]);  	//发数据
		 ret=CST_IIC_Wait_Ack();   //0:normal 1: error
		 if(ret)
			 {	
			//	printf("2 CT_IIC_Wait_Ack error . \r\r\n");			 //2022-4-7 14:25:51 
				break;  
		   }
	}
  CST_IIC_Stop();					//产生一个停止条件	    
	if(ret) ret=-1;
	return ret;
}








static int cst3xx_i2c_read_register( unsigned char *buf, int len)
					{
							int ret = -1;

							ret = cst3xx_i2c_write( buf, 2);
							if(ret < 0)
								{
								// printf("write err\r\n");  //2022-4-7 14:25:51
								}
							ret = cst3xx_i2c_read( buf, len);

							return ret;
					}



					
					
					
					
static void cst3xx_reset_ic(unsigned int ms)
					{
						unsigned char buf[4];
						int ret;
						
						buf[0] = 0xD1;
						buf[1] = 0x04;	//0x0E  cst148 是D104
						
						ret = cst3xx_i2c_write(buf, 2);
						if(ret < 0)
								// printf(" hyn cst3xx_reset_ic error.\r\r\n");	     //2022-4-7 14:25:51
						
						delay_ms(ms);
					}


					
					
					
					

static void cst3xx_ClearCMD()
					{
						u8 ret;
						u8 tempbuf[3];
						tempbuf[0] = 0xD0;
						tempbuf[1] = 0x00;
						tempbuf[2] = 0xAB;
						
						ret = cst3xx_i2c_write( tempbuf, 3);
							
						if (ret < 0) 
							{
							// printf(" linc send read touch info ending failed.\r\n");   //2022-4-7 14:25:51
							cst3xx_reset_ic(20);
							}
					}


					
					
					
u8 TP_Init(void)
{		
	CST_IIC_Init();
	cstp_dev.scan=HYN_CST148_Scan;	
	return 1; 									 
}



					

u8 HYN_CST148_Scan(void)//获取单点触控的坐标值   u8 TouchBuf[61]={0};
	{
		
	unsigned char finger_id, sw;  
	unsigned int  input_x = 0;    
	unsigned int  input_y = 0;   
	unsigned int  input_w = 0;//这个w数据？  
	int i2c_len;
		
         u8 ret = 0;
  u8 TouchPoint = 0;
   u8 tempPoint = 0;
           u8 i = 0;
           u8 j = 0;
        u8 temp = 0;
     u8 temp_id = 0;
        u8 flag = 0;
    u8 temp_sta = 0x06;
	     int sum1 = 0;
			 int sum2 = 0;
	    u8 offset = 0;
	  //   printf(" \r\n linc send read touch info enter\r\n\r\n");   //2022-4-7 14:25:51
		
		
	  TouchBuf[0] = 0xD0;//第一个手指的ID     高4位
	  TouchBuf[1] = 0x00;//第一个手指的状态   低4位  按下是0X06 或者是抬起
	          ret = cst3xx_i2c_read_register( TouchBuf, 7);
	
	
	  if(TouchBuf[0] == 0xAB)
	  	{
		   cst3xx_ClearCMD();//同步读操作,写D0 00 AB
		   return 0;
	    }
		
	  TouchPoint = TouchBuf[5]&0x0F;
		
		
		if(TouchPoint == 1)
			{
			//cst3xx_ClearCMD();
			TouchBuf[5] = 0xD0;
			TouchBuf[6] = 0x07;
			cst3xx_i2c_read_register( &TouchBuf[5], 2);
			sum1=TouchBuf[5] << 8|TouchBuf [6];
			} 
	  else if (TouchPoint > 1)
					{
					 TouchBuf[5] = 0xD0;
					 TouchBuf[6] = 0x07;
					 cst3xx_i2c_read_register( &TouchBuf[5], 5*(TouchPoint - 1)+3);
					 sum1=TouchBuf[5*TouchPoint+1] << 8|TouchBuf[5*TouchPoint+2];
					}
					
	  cst3xx_ClearCMD();//同步读操作，写D0 00 AB
					
					
		for(j=0 ;j < 5*TouchPoint;j++)//j=0
				{
				sum2+=TouchBuf[j];
				}
	//  printf(" linc send read touch sum1:%d,sum2:%d\r\n\r\n",sum1,sum2);  //2022-4-7 14:25:51
			  
		if(sum1 != sum2) 
		TouchPoint=0;
	//	printf(" linc send read touch info point:%d.\r\n\r\n",TouchPoint);  //2022-4-7 14:25:51
		
		if(TouchPoint > 0)
			{
				for(i = 0; i < TouchPoint; i++)
					 {
					if(i == 0)
						{offset = 0;}
					else
						{offset = 4;}
						
					 cstp_dev.tp_id[i] = (TouchBuf[i*5+offset]>>4) & 0x0F;
					cstp_dev.tp_sta[i] = TouchBuf[i*5+offset] & 0x0F;
						
					if(cstp_dev.tp_sta[i] == 0x06)//被按下
						{cstp_dev.tp_sta[i] = 0x07;}
					else
						{cstp_dev.tp_sta[i] = 0x06;}
						
					input_x = (unsigned int)((TouchBuf[i*5 + 1 + offset] << 4) | ((TouchBuf[i*5 + 3 + offset] >> 4) & 0x0F));
					input_y = (unsigned int)((TouchBuf[i*5 + 2 + offset] << 4) | (TouchBuf[i*5 + 3 + offset] & 0x0F));	
					input_w = (unsigned int)(TouchBuf[i + 4 + offset]);
					//cstp_dev.x[i] = (720-(u16)input_x) * 4095/720;
					//cstp_dev.y[i] = (1280-(u16)input_y)* 4095/1280;
					
					cstp_dev.x[i] =(u16)input_x;//320*480这个是直接输出的触摸的 x坐标 的数据
					cstp_dev.y[i] =(u16)input_y;//320*480这个是直接输出的触摸的 y坐标 的数据
					 }
			 }
		else
			 {//TouchPoint = 0;是释放按下的点
										TouchPoint = 1;
						 cstp_dev.tp_id[0] = (TouchBuf[0]>>4) & 0x0F;
						cstp_dev.tp_sta[0] = 0x06;
				 
						input_x = (unsigned int)((TouchBuf[0*5 + 1] << 4) | ((TouchBuf[0*5 + 3] >> 4) & 0x0F));
						input_y = (unsigned int)((TouchBuf[0*5 + 2] << 4) | (TouchBuf[0*5 + 3] & 0x0F));	
						input_w = (unsigned int)(TouchBuf[0 + 4]);
				 
						//cstp_dev.x[0] = (720-(u16)input_x) * 4095/720;
						//cstp_dev.y[0] = (1280-(u16)input_y)* 4095/1280;
				 
					  cstp_dev.x[0] =(u16)input_x;////这个是直接输出的触摸的坐标的数据
						cstp_dev.y[0] =(u16)input_y;
			 }
		
		if(TouchPoint == 0x00)
			{
			//printf("1:{[%d, %d, %d, %d]}\r\n",cstp_dev.tp_id[0],cstp_dev.tp_sta[0],cstp_dev.x[0],cstp_dev.y[0]);
			////2022-4-7 14:25:51
			}
		else
			{
				//printf("{");  //2022-4-7 14:25:51
				
				for(i = 0; i < TouchPoint; i++)
				{	
				//printf("i < TouchPoint:[%d, %d, %d, %d],",cstp_dev.tp_id[i],cstp_dev.tp_sta[i],cstp_dev.x[i],cstp_dev.y[i]);
				//2022-4-7 14:25:51	
					
				}		
				
				//printf("}\r\n");    //2022-4-7 14:25:51
			}
		
		return TouchPoint;
	}






//cst148初始化，扫描，电容屏。结构体初始化。
_cst_tp_dev cstp_dev=
					{
						CST_IIC_Init,     //指向 init 结构体函数指针 
						HYN_CST148_Scan,  //指向 scan 结构体函数指针 
						//HYN_CST148_Scan_Scan,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,	
						0,	
						0
					};

					
					

void MulTouch_Send(u16 *x,u16 *y,u8 *s,u8 *id,u8 n)
			{
			 u8 i;
			 u8 cv;	
				
			 u8 ReportBuf[14];
			 u32 m;
			 if(n == 0) return;
			 if(n > MAX_TOUCH_POINT) 
			 {
				n = MAX_TOUCH_POINT;
			 }
			 ReportBuf[0] = 0x02; 
			 for(i = 0; i < n;) 
			 {
				ReportBuf[1] = s[i]; 
				ReportBuf[2] = id[i]; 
				ReportBuf[3] = x[i] & 0xFF; 
				ReportBuf[4] = (x[i] >> 8) & 0xFF;
				ReportBuf[5] = y[i] & 0xFF; 
				ReportBuf[6] = (y[i] >> 8) & 0xFF; 
				if(i == 0) 
				{
				 ReportBuf[13] = n; 
				}
				else 
				{
				 ReportBuf[13] = 0;
				}
				i ++;
				if(i < n) 
				{
				 ReportBuf[7] = s[i]; 
				 ReportBuf[8] = id[i]; 
				 ReportBuf[9] = x[i] & 0xFF; 
				 ReportBuf[10] = (x[i] >> 8) & 0xFF; 
				 ReportBuf[11] = y[i] & 0xFF; 
				 ReportBuf[12] = (y[i] >> 8) & 0xFF; 
				 i ++;
				}
				else 
				{
				 u8 j;
				 for(j = 7; j < 13; j++)
				 {
				 ReportBuf[j] = 0;
				 }
				}
				
				//while(cst_PrevXfer_Complete==0)
				//{
					
				//}
				
				//PrevXferComplete=0;	
				
				
				//UserToPMABufferCopy(ReportBuf, GetEPTxAddr(ENDP1), 14);
				/*
				for(cv=0;cv<14;cv++)
				{
				printf(" ReportBuf[cv]:%#X\r\n",ReportBuf[cv]);	
				}	
				*/
				
			}

			}

			
			
			


const u16 CST_TPX_TBL[5]={CST_TP1_REG,CST_TP2_REG,CST_TP3_REG,CST_TP4_REG,CST_TP5_REG};
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 HYN_CST148_Scan_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	
	unsigned int  input_x = 0;    
	unsigned int  input_y = 0;   
	unsigned int  input_w = 0;//这个w数据？
	
	
	
	static u8 t=0; //控制查询间隔,从而降低cpu占用率
	t++;
	if((t%10)==0 || t<10) //空闲时,每进入10次CTP_Scan函数才检测一次,从而节省CPU使用率
	{
		CST148_RD_Reg(CST_TP1_REG,&mode,1);//读取触摸点的状态 
		if(mode&0x1F)  //当有触摸点按下时
		{	
			cstp_dev.sta = (mode&0x1F)|CSTP_PRES_DOWN|CSTP_CATH_PRES;
			for(i=0;i<5;i++) //获取触摸值
			{
				if(cstp_dev.sta&(1<<i)) //触摸有效？
				  {
					CST148_RD_Reg(CST_TPX_TBL[i],buf,4);//读取触摸值
						
					if(cstp_dev.touchtype&0x01) //横屏
							{
								
								//cstp_dev.y[i] = (((u16)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
								//cstp_dev.x[i] = 800-(((u16)buf[0]<<8)+buf[1])*OTT_SCAL_X;
								
								//input_x = (((u16)buf[2]<<8)+buf[3]);
								//input_y = 800-(((u16)buf[0]<<8)+buf[1]);
								
								
								
								//input_x =         // 2022.06.17注释的代码
								//input_y =         // 2022.06.17注释的代码
								
								
								
								cstp_dev.x[i] =(u16)input_x;//320*480这个是直接输出的触摸的 x坐标 的数据
								cstp_dev.y[i] =(u16)input_y;//320*480这个是直接输出的触摸的 y坐标 的数据
								
								
							}
					else  //竖屏
							{
								//cstp_dev.x[i] = (((u16)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
								//cstp_dev.y[i] = (((u16)buf[0]<<8)+buf[1])*OTT_SCAL_X;
								
								//input_x = (((u16)buf[2]<<8)+buf[3]);
								//input_y = 800-(((u16)buf[0]<<8)+buf[1]);
								
								
								
								
								

								cstp_dev.x[i] =(u16)input_x;//320*480这个是直接输出的触摸的 x坐标 的数据
								cstp_dev.y[i] =(u16)input_y;//320*480这个是直接输出的触摸的 y坐标 的数据
							}
				  }
			}
			
			res =1;
			if(cstp_dev.x[0]==0 && cstp_dev.y[0]==0)   mode=0;//读到的数据都是0,则忽略此次数据
			
			t = 0;//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	
	
	if((mode&0X1F) == 0) //无触摸点按下
	{
		if(cstp_dev.sta & CSTP_PRES_DOWN) //之前是被按下的
		{
			cstp_dev.sta &= ~(1<<7); //标记按键松开
		}
		else //之前就没有按下
		{
			cstp_dev.x[0] = 0Xffff;
			cstp_dev.y[0] = 0Xffff;
			cstp_dev.sta &= 0XE0; //清除点有效标记
		}
	}
	if(t>240)   t=10;//重新从10开始计数
	return res;
}
		
			
			
			
