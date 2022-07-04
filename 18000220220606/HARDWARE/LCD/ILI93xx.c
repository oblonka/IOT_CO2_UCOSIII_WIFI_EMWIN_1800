#include "ILI93XX.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	
//#include "image.h"

#include "sys.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8; 

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR =0x0000;	//画笔颜色
u16 BACK_COLOR  =0xFFFF;  //背景色 
  
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
	 
//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 regval)
{   
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 data)
{	 
	LCD->LCD_RAM=data;		 
}
//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(15);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	u16 color;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回		   
	LCD_SetCursor(x,y);	  
  if(lcddev.id==0X9341)
	{
		LCD_WR_REG(0X2E);//9341 发送读GRAM指令
		r=LCD_RD_DATA();								//dummy Read	   
		opt_delay(2);	  
		r=LCD_RD_DATA();  		  						//实际坐标颜色
		//9341要分2次读出
		opt_delay(2);	  
		b=LCD_RD_DATA(); 
		g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
		return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341需要公式转换一下
	}		
	else if(lcddev.id==0X9486||lcddev.id==0X7796)
	{
		LCD_WR_REG(0X2E);//9486 发送读GRAM指令
		color=LCD_RD_DATA();								//读出颜色值	 
		opt_delay(2);	  
		color=LCD_RD_DATA();  		  						//实际坐标颜色
		opt_delay(2);	   
		return 	color;	
	}
	else
	{
		return 0;
	}
	
}		 
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9486||lcddev.id==0X7796||lcddev.id==0X9341||lcddev.id==0X7796)LCD_WR_REG(0X29);	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9486||lcddev.id==0X7796||lcddev.id==0X9341)LCD_WR_REG(0X28);	//关闭显示
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	if(lcddev.id==0X9486||lcddev.id==0X7796)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 		
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 				 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF);
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF);
	}
	else if(lcddev.id==0X9341||lcddev.id==0X5310)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 

	}
} 		 
//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963等IC已经实际测试	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	if(lcddev.dir==1)//横屏时，对6804和1963不改变扫描方向！竖屏时1963改变方向
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0X9486||lcddev.id==0X7796||lcddev.id==0x9341)//9341/6804/5310/5510/1963,特殊处理
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08; 
		if(lcddev.id==0X6804)regval|=0x02;//6804的BIT6和9341的反了	   
		LCD_WriteReg(dirreg,regval);
		
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		
  	}

}     
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD->LCD_RAM=POINT_COLOR; 
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{
	if(lcddev.id==0X9486||lcddev.id==0X7796)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
		
	}
	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 
	}			 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir=0;	//竖屏
		lcddev.width=240;//
		lcddev.height=320;
		if(lcddev.id==0X9486||lcddev.id==0X7796||lcddev.id==0X9341)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X9486||lcddev.id==0X7796)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}
	}
	else 				//横屏
	{	  				
		lcddev.dir=1;	//横屏
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9486||lcddev.id==0X7796||lcddev.id==0X9341)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}
		if(lcddev.id==0X9486||lcddev.id==0X7796)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}	 
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9486||lcddev.id==0X7796||lcddev.id==0X9341)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}
}
//初始化lcd 

/*
例如TFTLCD屏(16位宽存储器)的RS脚接到FMC_A18上面，使用FMC_NE1(就是使用BANK1的子bank1),  命令为0  数据为1

外部存储器地址线寻址到A18为0时也即是FMC_A[24：0]=0x3FFFF，
左移一位付给内部总线HADDR[25:1]=00 0000 0111 1111 1111 1111 1110=0x7fffe，
所以内部总线读写外部存储器命令(REG)的地址(寻址)就变为0x60000000+0X7FFFE(FMC_A18即RS为0时);

部存储器地址线寻址到A18为1时也即是FMC_A[24：0]=0x40000，
左移一位付给内部总线HADDR[25:1]=00 0000 1000 0000 0000 0000 0000 =0x8000，
内部总线读写外部存储器数据(RAM)的地址变为0x60000000+0X80000(FMC_A18即RS为1时)。
*/






void LCD_Init(void)
{ 					
	vu32 i=0;
	
  GPIO_InitTypeDef  GPIO_InitStructure;                //GPIO配置寄存器
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;  //片选控制寄存器
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;      //片选时序寄存器
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;          //写入时序寄存器
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能PD,PE时钟  
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);                       //使能FSMC时钟  
	
 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;        //PE6 推挽输出,控制背光
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;     //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      //上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);             //初始化 //PE6 推挽输出,控制背光
	
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;//PD8 9 10 14 15 0 1 4 5 7 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      //复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      //上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);             //初始化  
	
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PE2 7 8 9 10 11 12 13 14 15
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      //复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      //上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);             //初始化  


  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);  //PD8
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);  //PD9
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC); //PD10
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC); //PD14
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC); //PD15
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);  //PD0
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);  //PD1
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);  //PD4
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);  //PD5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);  //PD7
	
 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource2,GPIO_AF_FSMC);  //PE2
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);  //PE7
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);  //PE8 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);  //PE9
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC); //PE10
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC); //PE11
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC); //PE12
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC); //PE13
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC); //PE14
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC); //PE15
 

  readWriteTiming.FSMC_AddressSetupTime      = 2;	               //0XF 地址建立时间（ADDSET）为16个HCLK 1/168M=6ns*16=96ns	
  readWriteTiming.FSMC_AddressHoldTime       = 10;	             //10 0x00 地址保持时间（ADDHLD）模式A未用到	
  readWriteTiming.FSMC_DataSetupTime         = 2;			           //60 数据保存时间为60个HCLK	=6*60=360ns
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;             //0x00
  readWriteTiming.FSMC_CLKDivision           = 0;                //16  0x00
  readWriteTiming.FSMC_DataLatency           = 0;                //17  0x00
  readWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_A;//模式A 
    

	writeTiming.FSMC_AddressSetupTime          = 2;	               //9 地址建立时间（ADDSET）为9个HCLK =54ns 
  writeTiming.FSMC_AddressHoldTime           = 10;	             //10 0x00 地址保持时间（A		
  writeTiming.FSMC_DataSetupTime             = 2;		             //8 数据保存时间为6ns*9个HCLK=54ns
  writeTiming.FSMC_BusTurnAroundDuration     = 0x00;             //0x00
  writeTiming.FSMC_CLKDivision               = 0;                //16  0x00
  writeTiming.FSMC_DataLatency               = 0;                //17  0x00
  writeTiming.FSMC_AccessMode                = FSMC_AccessMode_A;//模式A 

 
  FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;                  // 板子是NE1 6000 0000  - 63FF FFFF  .这里我们使用NE4 ，也就对应BTCR[6],[7]。
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;          // 不复用数据地址
  FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                 // FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;             //存储器数据宽度为16bit   
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;         // FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;	         //  存储器写使能
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Enable;             // 读写使用不同的时序
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;                     //读写时序
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &writeTiming;                         //写时序

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);                                                //初始化FSMC配置

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);                                                //板子是NE1  使能BANK1 
	
		
 
//	delay_ms(50); 					// delay 50 ms 
//  	lcddev.id=LCD_ReadReg(0x0000);	//读ID（9320/9325/9328/4531/4535等IC） 
//    printf(" LCD ID:%x\r\n",lcddev.id); //打印LCD ID 
//  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
//	{	
// 		//尝试9341 ID的读取		
//		LCD_WR_REG(0XD3);				   
//		lcddev.id=LCD_RD_DATA();	//dummy read 	
// 		lcddev.id=LCD_RD_DATA();	//读到0X00
//  		lcddev.id=LCD_RD_DATA();   	//读取93	或者94							   
// 		lcddev.id<<=8;
//		lcddev.id|=LCD_RD_DATA();  	//读取41 	 	或者86或者88
//	} 
	
// 	printf(" LCD ID:%x\r\n",lcddev.id); //打印LCD ID 

	lcddev.id = 0X7796;
	if(lcddev.id==0X7796)
	{
		delay_ms(500);//120
		LCD_WR_REG(0X11);
		delay_ms(500);//120

		LCD_WR_REG(0Xf0);
		LCD_WR_DATA(0xc3);

		LCD_WR_REG(0Xf0);
		LCD_WR_REG(0x96);

		LCD_WR_REG(0X36);  
		LCD_WR_DATA(0x00);     //调整显示方向

		LCD_WR_REG(0x3A);
		LCD_WR_DATA(0x05);

		LCD_WR_REG(0Xe8);
		LCD_WR_DATA(0x40);
		LCD_WR_DATA(0x82);
		LCD_WR_DATA(0x07);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x27);
		LCD_WR_DATA(0x0a);
		LCD_WR_DATA(0xb6);
		LCD_WR_DATA(0x33);

		LCD_WR_REG(0Xc5);
		LCD_WR_DATA(0x27);

		LCD_WR_REG(0Xc2);
		LCD_WR_DATA(0xa7);

		LCD_WR_REG(0Xe0);
		LCD_WR_DATA(0xf0);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x0f);
		LCD_WR_DATA(0x12);
		LCD_WR_DATA(0x1d);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x54);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x0c);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x13);
		LCD_WR_DATA(0x15);

		LCD_WR_REG(0Xe1);
		LCD_WR_DATA(0xf0);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x0a);
		LCD_WR_DATA(0x0b);
		LCD_WR_DATA(0x07);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x0c);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x13);
		LCD_WR_DATA(0x16);

		LCD_WR_REG(0Xf0);
		LCD_WR_DATA(0x3c);

		LCD_WR_REG(0Xf0);
		LCD_WR_DATA(0x69);
		delay_ms(1000);//120

		LCD_WR_REG(0X29);
	}
 
	
	LCD_Display_Dir(0);		//默认为竖屏     初始化屏幕的显示方向
	//LCD_LED=1;				//点亮背光
	LCD_Clear(GREEN);
}  
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
//	printf(" width:%x\r\n",lcddev.width); //打印LCD ID 
	printf(" width:%x\r\n",lcddev.width); //打印LCD ID 
	printf(" height:%x\r\n",lcddev.height); //打印LCD ID 
	printf(" totalpoint:%d\r\n",totalpoint); //打印LCD ID 
	if((lcddev.id==0X9486)||(lcddev.id==0X7796))
	{			
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0>>8);LCD_WR_DATA(0&0XFF); 		
		LCD_WR_DATA(lcddev.width>>8);LCD_WR_DATA(lcddev.width&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0>>8);LCD_WR_DATA(0&0XFF);
		LCD_WR_DATA(lcddev.height>>8);LCD_WR_DATA(lcddev.height&0XFF);  
	}
	else LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	
//		printf(" color:%x\r\n",index); //打印LCD ID 
	}
	
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
//	u16 temp;
	if(lcddev.id==0X9486||lcddev.id==0X7796)
	{
			 for(j=sy;j<ey;j++)
			 {				 
					for(i=sx;i<ex;i++)
				 {					 
							LCD_Fast_DrawPoint(i,j,color);
				 }

			 }
	}
	else
	{
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//设置光标位置 
			LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
			for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//显示颜色 	    
		}
	}	 
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//写入数据 
	}		  
}  
  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
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
		LCD_DrawPoint(uRow,uCol);//画点 
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
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}





































