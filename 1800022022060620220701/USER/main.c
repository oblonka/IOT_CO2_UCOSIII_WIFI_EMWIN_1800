#include "includes.h"	



#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
//#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"//修改的显示屏





#include "led.h"
#include "beep.h"
#include "timer.h"
//#include "touch.h"
#include "sdio_sdcard.h" //    
#include "GUI.h"
#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"
#include "includes.h"
#include "ButtonUse.h"
#include "WM.h"
#include "DIALOG.h" 


#include "cst148.h"//20220223  触摸屏数据坐标处理
#include "ctiic.h" //20220223  触摸屏的IIC通信的
#include "usart.h" //20220223  usart6的打印输出调试接口     

#include "HTIIC.h"//2022-4-11 15:27:46
#include "HT.h"   //2022-4-11 15:27:46
#include "PRTC.h" //2022-4-11 15:27:49
#include "AIR.h"  //2022-4-11 15:27:52
#include "WIFI.h" //2022-4-12 15:06:47
#include "TCP.h"  //2022-4-12 15:06:50
#include "MQTT.h" //2022-4-12 15:06:54

#include "DMA.h"



#include "KEYa.h"
#include "KEYAA.h"

#include "KEYnum.h"

#include "WIFIcon.h"

#include "KeyPad.h"

const u8 gSystemDate[12] = __DATE__;//注意，是两个下划线__
const u8 gSystemTime[16] = __TIME__;
	//printf("date:%s \r\n",gSystemDate); 
  //printf("time:%s  \r\n",gSystemTime);

  //printf("date:%s \r\n",__DATE__);
  //printf("time:%s \r\n",__TIME__);





extern u16  CO2_data;


extern uint16_t    CO2_Measure_data;


extern u16 wifi_ap_flag;

extern U8  Key_Data[26];


extern uchar top_button_flag;


/************************************************
 UCOSIII中以下优先级用户程序不能使用
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 
 //TOUCH任务
 //LED0任务
 //EMWINUI任务
 //SHT30任务
 //CO2任务
 //PRTC任务
 //WIFI任务
 
************************************************/


/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/


/*********************************************************起始任务*************************************************************/
//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				1024
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);





/*********************************************************触摸任务*************************************************************/
//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4//
//任务堆栈大小
#define TOUCH_STK_SIZE				1024//128  修改了触摸的堆栈大小位256     20220224 2022-2-24 15:59:39
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);





/*********************************************************LED,ADC，蜂鸣器，控制引脚，唤醒IO，任务******************************/
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 			4//5
//任务堆栈大小
#define LED0_STK_SIZE				256
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0任务
void led0_task(void *p_arg);



/*********************************************************GUI任务*************************************************************/
//EMWINUI任务
//设置任务优先级
#define EMWINUI_TASK_PRIO			4//6
//任务堆栈大小
#define EMWINUI_STK_SIZE			2048//2048
//任务控制块
OS_TCB EmwinuiTaskTCB;
//任务堆栈
CPU_STK EMWINUI_TASK_STK[EMWINUI_STK_SIZE];
//emwinui_task任务
void emwinui_task(void *p_arg);




/*********************************************************温湿度任务*************************************************************/

//SHT30任务
//设置任务优先级
#define SHT30_TASK_PRIO 			4//5
//任务堆栈大小
#define SHT30_STK_SIZE				1024
//任务控制块
OS_TCB SHT30TaskTCB;
//任务堆栈
CPU_STK SHT30_TASK_STK[SHT30_STK_SIZE];
//SHT30任务
void SHT30_task(void *p_arg);

/*********************************************************CO2任务*************************************************************/

//CO2任务
//设置任务优先级
#define CO2_TASK_PRIO 			4//5
//任务堆栈大小
#define CO2_STK_SIZE				1024
//任务控制块
OS_TCB CO2TaskTCB;
//任务堆栈
CPU_STK CO2_TASK_STK[CO2_STK_SIZE];
//SHT30任务
void CO2_task(void *p_arg);



/*********************************************************实时时钟任务*************************************************************/


//PRTC任务
//设置任务优先级
#define PRTC_TASK_PRIO 			4//5
//任务堆栈大小
#define PRTC_STK_SIZE				1024
//任务控制块
OS_TCB PRTCTaskTCB;
//任务堆栈
CPU_STK PRTC_TASK_STK[PRTC_STK_SIZE];
//SHT30任务
void PRTC_task(void *p_arg);

/*********************************************************WIFI任务*************************************************************/

//WIFI任务
//设置任务优先级
#define WIFI_TASK_PRIO 			4//5
//任务堆栈大小
#define WIFI_STK_SIZE				1024
//任务控制块
OS_TCB WIFITaskTCB;
//任务堆栈
CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];
//SHT30任务
void WIFI_task(void *p_arg);

/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/


//uchar time_data[6]={0};//读取DS1302当前时间缓存矩阵

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	uchar init_time[6]={22,6,20,14,14,06};//初始化时间参数   手动配置时间的设置
	
	
	delay_init(168);//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	
	uart_init(115200);    	  //调试串口6波特率设置
	uart2_init_wifi(115200);  //WIFI设置
	uart1_init_CO2(9600);     //CO2串口波特率设置    2022.05   9600
	
	LCD_Init();			          //初始化TFT LCD
	
	
	//W25QXX_Init();			    //初始化W25Q128
	LED_Init();   			      //LED初始化
	BEEP_Init();			        //初始化蜂鸣器
  CO2_VCC_CTR_Init();       //二氧化碳电平IO初始化
	
	
	//FSMC_SRAM_Init(); 		  //SRAM初始化	
	mem_init(SRAMIN); 		    //内部RAM初始化
	//mem_init(SRAMEX); 		  //备用外部RAM初始化
	mem_init(SRAMCCM);		    //CCM初始化
	
	
	exfuns_init();			      //为fatfs文件系统分配内存
	f_mount(fs[0],"0:",1);	  //挂载SD卡
  //f_mount(fs[1],"1:",1);	//挂载FLASH

  TP_Init();                //这个位置触摸初始化，放到这个位置，任务执行，开始时间片的轮询的工作。
	cstp_dev.init();          //触摸屏初始化
	
	
	HTIIC_Init();
  SHT30_Init();             //温湿度
	
	
	
	
	
	DS1302_init(init_time);
	DS1302_SetTime(init_time);
	//Time_Init();
	//DS1302_WriteBYTE(0x80,0x00,0);
	
	
	
	//ds1302_gpio_init();delay_ms(5);
	//ds1032_DATAOUT_init();delay_ms(5);
	//ds1032_DATAINPUT_init();delay_ms(5);
	//ds1032_init();delay_ms(5);//时钟
	//ds1032_read_realTime();delay_ms(5);	
		
		
		
		
		
	//delay_init();
	//uart3_Init(115200);
	ESP32S_Init(115200);	    //WiFi
		
		
		
	OSInit(&err);			        //初始化UCOSIII
	OS_CRITICAL_ENTER();	    //进入临界区
	
	
	
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		      //任务控制块
				 (CPU_CHAR	* )"start task", 		          //任务名字
                 (OS_TASK_PTR )start_task, 			  //任务函数
                 (void		* )0,					          //传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					        //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					          //当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					          //用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				        //存放该函数错误时的返回值
								 
	OS_CRITICAL_EXIT();	                            //退出临界区	 
	OSStart(&err);                                  //开启UCOSIII
								 
	while(1);
								 
}


/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/


//开始任务函数
void start_task(void *p_arg)
{
	
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
	
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err); //统计任务                
#endif
	
	
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN	 //如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

	
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//开启CRC时钟   emwin内部需要
	
	WM_SetCreateFlags(WM_CF_MEMDEV);                  //启动所有窗口的存储设备   这个修改16bitBP的函数  
	GUI_Init();  			                                //STemWin初始化

	
	
	OS_CRITICAL_ENTER();	                            //进入临界区  任务保护
	
	
	
/*********************************************************************/	
/*****************Emwin TASK******************************************/
/*********************************************************************/
	OSTaskCreate((OS_TCB*     )&EmwinuiTaskTCB,		
				 (CPU_CHAR*   )"Emwinui task", 		
                 (OS_TASK_PTR )emwinui_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINUI_TASK_PRIO,     
                 (CPU_STK*    )&EMWINUI_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINUI_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINUI_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
								 
					
								 
/*********************************************************************/								 
/*****************Touch TASK******************************************/
/*********************************************************************/								 
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);		

				
								 
/********************************************************************/								 
/*****************LED TASK*******************************************/
/********************************************************************/								 
	OSTaskCreate((OS_TCB*     )&Led0TaskTCB,		
				 (CPU_CHAR*   )"Led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);		

								 
								 
/*******************************************************************/								 
/*****************SHT30 TASK****************************************/
/*******************************************************************/								 
	OSTaskCreate((OS_TCB*     )&SHT30TaskTCB,		
				 (CPU_CHAR*   )"SHT30 task", 		
                 (OS_TASK_PTR )SHT30_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )SHT30_TASK_PRIO,     
                 (CPU_STK*    )&SHT30_TASK_STK[0],	
                 (CPU_STK_SIZE)SHT30_STK_SIZE/10,	
                 (CPU_STK_SIZE)SHT30_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);									 
								 
			
								 
/*******************************************************************/								 
/*****************PRTC TASK*****************************************/
/*******************************************************************/								 
	OSTaskCreate((OS_TCB*     )&PRTCTaskTCB,		
				 (CPU_CHAR*   )"PRTC task", 		
                 (OS_TASK_PTR )PRTC_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )PRTC_TASK_PRIO,     
                 (CPU_STK*    )&PRTC_TASK_STK[0],	
                 (CPU_STK_SIZE)PRTC_STK_SIZE/10,	
                 (CPU_STK_SIZE)PRTC_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);					 
								 
				
								 
/*******************************************************************/				
/*****************CO2 TASK******************************************/
/*******************************************************************/								 
	OSTaskCreate((OS_TCB*     )&CO2TaskTCB,		
				 (CPU_CHAR*   )"CO2 task", 		
                 (OS_TASK_PTR )CO2_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )CO2_TASK_PRIO,     
                 (CPU_STK*    )&CO2_TASK_STK[0],	
                 (CPU_STK_SIZE)CO2_STK_SIZE/10,	
                 (CPU_STK_SIZE)CO2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
								 
		
								 
/*******************************************************************/								 
/*****************WIFI TASK*****************************************/
/*******************************************************************/								 
	OSTaskCreate((OS_TCB*     )&WIFITaskTCB,		
				 (CPU_CHAR*   )"WIFI task", 		
                 (OS_TASK_PTR )WIFI_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )WIFI_TASK_PRIO,     
                 (CPU_STK*    )&WIFI_TASK_STK[0],	
                 (CPU_STK_SIZE)WIFI_STK_SIZE/10,	
                 (CPU_STK_SIZE)WIFI_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
								 							 
								 
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	                            //退出临界区  保护						 
}





/************************************************************************************************************************/
/************************************************************************************************************************/
/*********************************************************|| 任务分解 ||*************************************************/
/************************************************************************************************************************/
/************************************************************************************************************************/


/*
1.GUI TASK
2.TOUCH TASK
3.LED TASK
4.SHT30 TASK
5.PREC TASK
6.CO2 TASK
7.WIFI TASK


*/



/************************************************************************************************************************/
/********************************************************| EMWINDEMO任务 |***********************************************/
/************************************************************************************************************************/
void emwinui_task(void *p_arg)
{
	
	
	
	OS_ERR err;
	
	//GUI_CURSOR_Show(); 
	
	//更换皮肤
	/*
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX); 
	*/
	
	
	while(1)
	{
		
		//单独的调试各个界面的线程。
		
		
		//printf("\r\n GUI-test \r\n");	
		
		
		//在这里单独与调用各个需要切换显示的界面 
		//单独的显示这个 界面     
		
		
		
		
		
//ESP32S_JoinAP_connect_list_parse("AT+CWLAP",500);
		
		
		
/*
主界面  wifi_ssid的列表显示界面   输入密码的小写界面   大写界面  数字界面 
emwin里面有一个检测触摸的频率，就是多久检测一次是否有触摸时间，他原来的历程这个频率是很低很低的，把它改高了就好了
		
解析出来的列表显示SSID 对应一个字符串的接口  char*  输入到User_ESP32S_SSID  连接AP的函数   ESP32S_JoinAP(User_ESP32S_SSID, User_ESP32S_PWD); 
输入进去的 password  	 对应一个字符串的接口  char*  输入到User_ESP32S_PWD   连接AP的函数   ESP32S_JoinAP(User_ESP32S_SSID, User_ESP32S_PWD); 
		
		
ESP32S_JoinAP(User_ESP32S_SSID, User_ESP32S_PWD); 是单独的函数 	  MQTT  TCP 不同平台平台对接的必要的连接步骤	
		
		
	
ssid      的解析是		串口的解析完成的串口的数组        ESP32S_Fram_Record_Struct.Data_RX_BUF                                   
password  的解析是    emwin     edit输入的字符串        Edit_buff                 		
		
*/		







/*

下面的是各个界面的测试界面

这个任务的函数只显示主界面，由主界面引出其他的界面。


*/



	  ButtonUse_Demo();  //主界面
		
	 //	if(top_button_flag==1)
	 //	{
	 //	
	 //	MainTask_WIFIcon();
	 //	
	 //	}
		
		
		
    //KEY_a();  //小写的键盘的界面，加edit的输入的代码
		//KEY_num();//数字和特殊字符的界面，加edit的输入的代码
		//KEYAA();    //大写的键盘的界面，加edit的输入的代码
		//KEY_WIFIcon();//显示wifi列表的界面，加edit的输入的代码
		
		
		
		
		
		
		//MainTask_WIFIcon();
		//MainTask_a();
		//MainTask_AA();
		//MainTask_num();
		
		//CreateWindow123();
		//MainTask_allin();
		
		//printf("\r\n Emwin_UI_test \r\n");
		//delay_ms(1000);
		
		
		
		
		
		//printf("date:%s \r\n",gSystemDate); 
    //printf("time:%s  \r\n",gSystemTime);
		
		
		
	  //GUI_SetColor(GUI_RED);
    //GUI_SetBkColor(GUI_BLUE);
    //GUI_SetFont(&GUI_Font24_ASCII);
    //GUI_Clear();
    //GUI_DispStringAt("Hello World",10,10); 	
		
		
		OSTimeDlyHMSM(0,0,0,1500,OS_OPT_TIME_PERIODIC,&err);//延时5ms
		
	}
}





/************************************************************************************************************************/
/********************************************************| TOUCH任务 |***************************************************/
/************************************************************************************************************************/
void touch_task(void *p_arg)
{
	/*
	u8 cst_num;
	
	u8 cst_1;
	u8 cst_2;
	u8 cst_3;
	u8 cst_4;
	u8 cst_5;
	
	u8 yy[2];
	u8 XX[2];
	
	u8 tt;
	u8 RR;
	u8 WW;
	u8 AA;
	u8 II;
	
	u8 hh;
	u8 jj;
	u8 ll;
	u8 zz;
	u8 nn;
	
	u8 cc;
	u8 ee;
	
	u8 kk = 0X00;
	u8 i;
	u8 SS;
	u8 vv[2]={0XD1,0X01};
	*/
	
	OS_ERR err;
	
	while(1)
	{
	/*	
	//先写命令  ，然后发送0XD101,再读命令，进入寄存器0XD204	，读取数值
	CST_IIC_Init();
  delay_ms(60);
	//解析触摸到点的坐标，检测到触摸状态，点坐标 
		
	CST148_WR_Reg(MODE_INFO,yy,1);	
		
	CST148_RD_Reg(BOOTLOADER,&AA,1);      //0XD1FE 
	printf(" \r\n 0XD1FE:%#X\r\n",AA);			
		
	CST148_RD_Reg(BOOTLOADER_TG,&II,1);   //0XD1FF 
	printf(" 0XD1FF:%#X\r\n",II);	
		
		
	CST148_RD_Reg(STYLE_ID,&RR,1);        //0XD204  0XA0        
	printf(" \r\n 0XD204:%#X\r\n",RR);
		            
  CST148_RD_Reg(STYLE_ID_jj,&cc,1);     //0XD205  0X31        
	printf(" 0XD205:%#X\r\n",cc);
		
	CST148_RD_Reg(STYLE_ID_IC,&tt,1);     //0XD206  0X94           
	printf(" 0XD206:%#X\r\n",tt);		
	
	CST148_RD_Reg(STYLE_ID_hh,&ee,1);     //0XD207  0
	printf(" 0XD207:%#X\r\n",ee);	
	
	CST148_RD_Reg(STYLE_ID_FF,&WW,1);     //0XD208  0X1
	printf(" 0XD208:%#X\r\n",WW);	
	
		
	CST148_RD_Reg(Guding,&kk,1);          //正常，固定的触摸读取
	printf(" \r\n Guding:%#X\r\n",kk);	
	CST148_RD_Reg(Num,&SS,1);             //正常，测试触摸点的个数     5个触点
	
	printf(" Num:%#X\r\n",((SS>>8)&0x00FF));
	
	printf(" Num:%#X\r\n",SS);
	printf(" Num:%d\r\n",SS);
	
  if(SS>3)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			delay_ms(15);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		}
  
  CST148_RD_Reg(CST_TP1_REG,&hh,1);     //正常，测试触摸点的个数     5个触点
	printf(" TP1:%#X\r\n",hh);		        //CST_TP1_REG
	
	
	CST148_RD_Reg(CST_TP2_REG,&jj,1);     //正常，测试触摸点的个数     5个触点
	printf(" TP2:%#X\r\n",jj);		        //CST_TP2_REG
		
	CST148_RD_Reg(CST_TP3_REG,&ll,1);     //正常，测试触摸点的个数     5个触点
	printf(" TP3:%#X\r\n",ll);		        //CST_TP3_REG

	CST148_RD_Reg(CST_TP4_REG,&zz,1);     //正常，测试触摸点的个数     5个触点
	printf(" TP4:%#X\r\n",zz);		        //CST_TP4_REG
	
	CST148_RD_Reg(CST_TP5_REG,&nn,1);     //正常，测试触摸点的个数     5个触点
	printf(" TP5:%#X\r\n",nn);		        //CST_TP5_REG

	cst_num=cstp_dev.scan(); //触摸屏扫描  
	printf("cstp_dev.scan---num:%d\r\n",cst_num);		     //CST_TP5_REG	
		
	if(cst_num > 0)
		{
		MulTouch_Send(cstp_dev.x,\
			            cstp_dev.y,\
			            cstp_dev.tp_sta,\
			            cstp_dev.tp_id,\
			            cst_num);
			
		//printf("cstp_dev.x:%d\r\n",cstp_dev.x);	
		//printf("cstp_dev.y:%d\r\n",cstp_dev.y);	
		//printf("cstp_dev.tp_sta:%d\r\n",cstp_dev.tp_sta);	
		//printf("cstp_dev.tp_id:%d\r\n",cstp_dev.tp_id);	
		//printf("cst_num:%d\r\n\r\n\r\n\r\n",cst_num);		
			
		}
		delay_ms(10);	
	
	//ctp_test();	
	
  //lcd_draw_bline(130, 75, 124, 190,5,RED);//画线
	
	delay_ms(3000);	
		*/
		
		//ctp_test();
		
		//printf("\r\n TOUCH-test \r\n");	
		
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//10这个测试删除ok   延时5ms    1500    改为10，缩小扫描周期
		
		//BEEP=~BEEP;
	}
}





/************************************************************************************************************************/
/********************************************************| LED0任务 |****************************************************/
/************************************************************************************************************************/
void led0_task(void *p_arg)
{
	OS_ERR err;
	
	
	while(1)
	{
		LED1 = !LED1;
		LED3 = !LED3;

		
		//printf("\r\n LED-test \r\n");	
		
		
		OSTimeDlyHMSM(0,0,0,1500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}





/************************************************************************************************************************/
/********************************************************| SHT30任务 |***************************************************/
/************************************************************************************************************************/
void SHT30_task(void *p_arg)
{
	
	OS_ERR err;
	
  extern float SHT30_temperature;
  extern float SHT30_humidity;
	
	while(1)
	{
		
		
		/*
		delay_ms(1000);
		
		printf("\r\n qian:ceshi \r\n");
		delay_ms(1000);
		sht30_data_process();
		delay_ms(1000);
		printf("\r\n hou:ceshi \r\n");	
		
		//=2022-4-13 13:13:45
	  printf("temperature:%6.1f 'C\r\n humidity:%6.1f \r\n",SHT30_temperature/10,SHT30_humidity/10);
		
	*/
		//sht30_data_process();
		
		
		OSTimeDlyHMSM(0,0,0,1500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}





/************************************************************************************************************************/
/*******************************************************| PRTC任务 |*****************************************************/
/************************************************************************************************************************/
void PRTC_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		
		//printf("\r\n  PRTC-test \r\n");	
		
    //DS1302_WriteBYTE(0x80,0x00,0);
		//DS1302Z_WriteQ();
		//DS1302Z_Read();
	
		
		
		//ds1032_init();
		//ds1302_wirte_rig(0x80,0x00);
		//ds1032_read_realTime();
		
		
		
		
		
		//DS1302_Readtime();
		
		
		
		
		delay_ms(1000);
		
		OSTimeDlyHMSM(0,0,0,1500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}





/************************************************************************************************************************/
/*******************************************************| CO2任务 |******************************************************/
/************************************************************************************************************************/
void CO2_task(void *p_arg)
{
	OS_ERR err;
	
	extern int  CO2_ppm;
	
	
	
	
  //extern float SHT30_humidity;
	
	while(1)
	{
		//测试串口1  CO2
		//USART_SendData(USART1, 1111);                         //向串口1发送数据
	  //while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		
		//printf("\r\n CO2_data_test_start \r\n");	
		//delay_ms(1000);
		//CO2_Send_Data(); 
		//Read_CO2_ppm();//读取实时的二氧化碳的数据
		//printf("实际测试的浓度_CO2_ppm :%d\r\n",CO2_ppm);//串口6的调试打印输出数据测试
		//delay_ms(1000);
	
		
		//CO2_Send_Data(); 
		
		//printf("\r\n CO2_data_test_start \r\n");	
		
		//CO2_Send_Data();
		
		
		Read_CO2_ppm();
		//printf("\r\n CO2=  %d ppm \r\n",CO2_Measure_data);
		delay_ms(5000);	
		
		
		//CO2_Tx();
		//printf("CO2=  %d ppm \r\n",CO2_data);
		//delay_ms(1000);	
		
		
		//printf("\r\n CO2_data_test_end \r\n");	
		
		//printf("date:%s \r\n",__DATE__);
    //printf("time:%s \r\n",__TIME__);
		
		
		OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}





/************************************************************************************************************************/
/*******************************************************| WIFI任务 |*****************************************************/
/************************************************************************************************************************/
void WIFI_task(void *p_arg)
{
	OS_ERR err;
	
	u8 t;
	u8 len;	
	
	while(1)
	{
		
		
		
//测试串口2   WiFi
//			if(USART2_RX_STA_WIFI&0x8000)
//		{					   
//			len=USART2_RX_STA_WIFI&0x3fff;//得到此次接收到的数据长度
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART2, USART2_RX_BUF[t]);             //向串口1发送数据
		
		
		    //USART_SendData(USART2, 2222);                         //向串口2发送数据
				//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
//			}
//			USART2_RX_STA_WIFI=0;
		
			
		//ESP32S_AT_Test();
			
		//printf("\r\n WIFI-test-start \r\n");	
		//delay_ms(1000);
		
		
		
		
		
		//下面是测试解析WIFI列表的函数，解析出的SSID打印输出到串口显示。
		//delay_ms(2000);
		//ESP32S_JoinAP_connect_list_parse("AT+CWLAP",500);//解析SSID的命令的函数,不定长的数据解析。
	  //delay_ms(2000);
		//解析WIFI列表的函数
		
		
		//U8  Key_Data[26] = { 0 };
		
		//ssid是获取的扫描的字符串   password是输入的字符串获取
	

if(wifi_ap_flag ==1)
			{
			ESP32S_STA_MQTTClient_Test_SA_test(User_ESP32S_SSID, Key_Data);
			}
		
	

		
	
/*
通过解析出来的SSID对应接口字符串      char*   
1.显示解析的SSID列表
2.button的问题处理
3.ssid 与 password的接口的对接        直接对接到MQTT的接口那里。          
*/
		
		
		
		//下面是完整的测试MQTT的代码，加上+++退出透传的指令2022.06.10测试
		//delay_ms(1000);
		//ESP32S_STA_TCPClient_Test();//测试TCP通讯
		//ESP32S_STA_MQTTClient_Test_SA();//测试MQTT通讯    测试OK
		//delay_ms(1000);
		
		//测试MQTT的代码
		
		OSTimeDlyHMSM(0,0,10,5000,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}


/*
log:

*/

