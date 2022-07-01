/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include "GUI.h"
//#include "touch.h"
#include "ILI93xx.h"
#include "usart.h"

#include "cst148.h"
#include "ctiic.h"
#include "beep.h" 


                                

void GUI_TOUCH_X_ActivateX(void) 
			{
			 // XPT2046_WriteCMD(0x90);
				
			}

			

void GUI_TOUCH_X_ActivateY(void)
			{
			//XPT2046_WriteCMD(0xd0);
				
			}

			

int  GUI_TOUCH_X_MeasureX(void) //EMWIN读取X轴命令
		{
				int32_t xvalue;
			
				if(lcddev.id == 0X7796)//按照屏幕型号选择触摸的坐标获取
				{
				cstp_dev.scan();//20220223
			  xvalue=cstp_dev.x[0];
			  //BEEP=~BEEP;
			  //printf("GUI_X_o:%d，\r\n\r\n",xvalue);//具体的X坐标，可以读出  //2022-4-7 14:25:51
				return xvalue;
				}
				
				else
				{
				cstp_dev.scan();//20220223
			  xvalue=cstp_dev.x[0];
			  //BEEP=~BEEP;
			  //printf("GUI_X_o:%d，\r\n\r\n",xvalue);//具体的X坐标，可以读出  ////2022-4-7 14:25:51
				return xvalue;	
						
				}
				
				
				
				/*
				if((lcddev.id == 0X5510) || (lcddev.id == 0X1963)|| (lcddev.id == 0X7796)) //电容屏的触摸值获取(使用NT5510和SSD1963芯片的4.3寸和7寸屏都是电容屏)
				{
					cstp_dev.scan();//20220223
					xvalue=cstp_dev.x[0];
					return xvalue;
				}
				else				//电阻屏
				{
					//turn TP_Read_XOY(0XD0);  //CMD_RDX=0XD0   20220223
					
					cstp_dev.scan();     //20220224
					xvalue=cstp_dev.x[0];
					return xvalue;
				}
				*/
		}



int  GUI_TOUCH_X_MeasureY(void)  //EMWIN读取Y轴命令
		{	
				  int32_t yvalue;
				
			if(lcddev.id == 0X7796)//按照屏幕型号选择触摸的坐标获取
			{
				  cstp_dev.scan();//20220224
					yvalue = cstp_dev.y[0];
			    //BEEP=~BEEP;
			    //printf("GUI_Y_o:%d，\r\n\r\n",yvalue);//具体的Y坐标，可以读出   //2022-4-7 14:25:51
					return yvalue;
			}
			
			else
			{
			    cstp_dev.scan();//20220224
					yvalue = cstp_dev.y[0];
			   //BEEP=~BEEP;
			   // printf("GUI_Y_o:%d，\r\n\r\n",yvalue);//具体的Y坐标，可以读出  //2022-4-7 14:25:51
					return yvalue;
			
			
			
			}
			
			
			
			
			
			
				/*
				if((lcddev.id == 0X5510) || (lcddev.id == 0X1963)|| (lcddev.id == 0X7796))//电容屏的触摸值获取(使用NT5510和SSD1963芯片的4.3寸和7寸屏都是电容屏)
				{
					cstp_dev.scan();//20220224
					yvalue = cstp_dev.y[0];
					return yvalue;
				}
				else				//电阻屏
				{
					//turn TP_Read_XOY(0X90);  //CMD_RDX=0XD0 20220223
					
					cstp_dev.scan();//20220224
					yvalue = cstp_dev.y[0];
					return yvalue;
				}
				*/		
		}
		
		
		

		
		
		
		

			
		
			
		