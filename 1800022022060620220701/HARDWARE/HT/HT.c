#include "sys.h"
#include "delay.h"

//#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"

#include "HT.h"//   2022-4-7 15:00:20
#include "HTIIC.h"
#include "usart.h" //20220223  usart6的打印输出调试接口     



/**************************************************上次测试，暂时屏蔽*************************************************************/

//float_data humidity;    //存储的湿度数据
//float_data temperature; //存储的温度数据


//u8 status[3];//状态寄存器
//u8 status_crc;//CRC校验计算
//u8 crc_check;//crc结果
////如果读取不到数据，有可能是读取的时间设得太短导致，我这里的时间是最小可实现读取的时间！！！


///*
//*********************************************************************************************************
//*	函 数 名: i2c_Delay
//*	功能说明: I2C总线延迟，最快400KHz
//*	形    参：无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void i2c_Delay(void)
//{
//    __NOP();__NOP();__NOP();__NOP();    __NOP();__NOP();__NOP();__NOP();

//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_Start
//*	功能说明: CPU发起I2C总线启动信号
//*	形    参：无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void i2c_Start(void)
//{
//    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
//    I2C_SDA_1();
//    I2C_SCL_1();
//    i2c_Delay();
//    I2C_SDA_0();
//    i2c_Delay();
//    I2C_SCL_0();
//    i2c_Delay();
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_Start
//*	功能说明: CPU发起I2C总线停止信号
//*	形    参：无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void i2c_Stop(void)
//{
//    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
//    I2C_SDA_0();
//    I2C_SCL_1();
//    i2c_Delay();
//    I2C_SDA_1();
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_SendByte
//*	功能说明: CPU向I2C总线设备发送8bit数据
//*	形    参：_ucByte ： 等待发送的字节
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void i2c_SendByte(uint8_t _ucByte)
//{
//    uint8_t i;

//    /* 先发送字节的高位bit7 */
//    for (i = 0; i < 8; i++)
//    {
//        if (_ucByte & 0x80)
//        {
//            I2C_SDA_1();
//        }
//        else
//        {
//            I2C_SDA_0();
//        }
//        i2c_Delay();
//        I2C_SCL_1();
//        i2c_Delay();
//        I2C_SCL_0();
//        if (i == 7)
//        {
//            I2C_SDA_1(); // 释放总线
//        }
//        _ucByte <<= 1;	/* 左移一个bit */
//        i2c_Delay();
//    }
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_ReadByte
//*	功能说明: CPU从I2C总线设备读取8bit数据
//*	形    参：无
//*	返 回 值: 读到的数据
//*********************************************************************************************************
//*/
//uint8_t i2c_ReadByte(u8 ack)
//{
//    uint8_t i;
//    uint8_t value;

//    /* 读到第1个bit为数据的bit7 */
//    value = 0;
//    for (i = 0; i < 8; i++)
//    {        
//        value <<= 1;
//        I2C_SCL_1();
//        i2c_Delay();
//        if (I2C_SDA_READ())
//        {
//            value++;
//        }
//        I2C_SCL_0();
//        i2c_Delay();
//    }
//    if(ack==0)
//        i2c_NAck();
//    else
//        i2c_Ack();
//    return value;
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_WaitAck
//*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
//*	形    参：无
//*	返 回 值: 返回0表示正确应答，1表示无器件响应
//*********************************************************************************************************
//*/
//uint8_t i2c_WaitAck(void)
//{
//    uint8_t try_time = 100;//连接超时次数

//    I2C_SDA_1();	        //CPU释放SDA总线
//    i2c_Delay();
//    I2C_SCL_1();	        //CPU驱动SCL = 1, 此时器件会返回ACK应答
//    i2c_Delay(); 

//    while(I2C_SDA_READ())//等待SHT30应答
//    {
//        try_time--;
//        i2c_Delay(); 
//        if(try_time==0)//超时，无响应
//        {
//            I2C_SCL_0();
//            i2c_Delay();
//            return 1;
//        }
//    }
//    I2C_SCL_0();
//    i2c_Delay();    
//    return 0;
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_Ack
//*	功能说明: CPU产生一个ACK信号
//*	形    参：无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void i2c_Ack(void)
//{
//    I2C_SDA_0();	/* CPU驱动SDA = 0 */
//    i2c_Delay();
//    I2C_SCL_1();	/* CPU产生1个时钟 */
//    i2c_Delay();
//    I2C_SCL_0();
//    i2c_Delay();
//    I2C_SDA_1();	/* CPU释放SDA总线 */
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_NAck
//*	功能说明: CPU产生1个NACK信号
//*	形    参：无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void i2c_NAck(void)
//{
//    I2C_SDA_1();	/* CPU驱动SDA = 1 */
//    i2c_Delay();
//    I2C_SCL_1();	/* CPU产生1个时钟 */
//    i2c_Delay();
//    I2C_SCL_0();
//    i2c_Delay();
//}

///*
//*********************************************************************************************************
//*	函 数 名: i2c_GPIO_Config
//*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
//*	形    参：无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void i2c_GPIO_Config(void)
//{
//    //个人觉得程序初始化就是要通俗易懂，所以习惯用库函数进行
//    //但是像改变IIC引脚电平状态，由于需要频繁操作，所以最好还是使用寄存器操作
//    //GPIO_InitTypeDef GPIO_InitStrcuture;
//    
//	  //RCC_AHB1PeriphClockCmd(IIC_CLOCK,ENABLE);//2022-4-7 13:58:23

//    //GPIO_InitStrcuture.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出
//    //GPIO_InitStrcuture.GPIO_Pin = IIC_SDA_PIN|IIC_SCL_PIN;
//    //GPIO_InitStrcuture.GPIO_Speed = GPIO_Speed_50MHz;
//    //GPIO_Init(IIC_GPIO,&GPIO_InitStrcuture);
//	
//	
//	
//	
//		GPIO_InitTypeDef  GPIO_InitStructure;
//		RCC_AHB1PeriphClockCmd(IIC_CLOCK, ENABLE);                 //使能GPIOB时钟     RCC_AHB1Periph_GPIOB

//		GPIO_InitStructure.GPIO_Pin   = IIC_SDA_PIN | IIC_SCL_PIN; //PB设置为推挽输出
//		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;             //输出模式GPIO_Mode_OUT
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;             //推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //50MHz
//		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;              //上拉
//		GPIO_Init(IIC_GPIO, &GPIO_InitStructure);                  //初始化
//	
//	  //printf("\r\n HT_INIT \r\n");
//}
///*
//*********************************************************************************************************
//*	函 数 名: i2c_CheckDevice
//*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
//*	形    参：_Address：设备的I2C总线地址
//*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
//*********************************************************************************************************
//*/
//uint8_t i2c_CheckDevice(uint8_t _Address)
//{
//    uint8_t ucAck;
//    i2c_GPIO_Config();		/* 配置GPIO */
//    i2c_Start();		/* 发送启动信号 */

//    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
//    i2c_SendByte(_Address|I2C_WR);
//    ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

//    i2c_Stop();			/* 发送停止信号 */
//    return ucAck;
//}










////CRC校验
//static u8 CRC_Check(u8 *check_data, u8 num, u8 check_crc)
//{
//    uint8_t bit;        // bit mask
//    uint8_t crc = 0xFF; // calculated checksum
//    uint8_t byteCtr;    // byte counter
//    
// // calculates 8-Bit checksum with given polynomial x8+x5+x4+1
//    for(byteCtr = 0; byteCtr < num; byteCtr++)
//    {
//        crc ^= (*(check_data+byteCtr));
//        //crc校验，最高位是1就^0x31
//        for(bit = 8; bit > 0; --bit)
//        {
//            if(crc & 0x80)
//                crc = (crc << 1) ^ 0x31;
//            else
//                crc = (crc << 1);
//        }
//    }
//    if(crc==check_crc)
//        return 1;
//    else 
//        return 0;
//}

////根据所选获取数据的方式初始化
//void SHT30_Init(void)
//{
//    i2c_GPIO_Config();
//    SHT30_General_RESET();//软复位设备
//    delay_ms(10);
//}

////发送两字节指令，stop确定是否发送停止信号
//void SHT30_SendBytes(u16 cmd,u8 stop)
//{
//    i2c_Start();
//    i2c_SendByte(SHT30_ADDR<<1 | I2C_WR);//写7位I2C设备地址加0作为写取位,1为读取位
//    i2c_WaitAck();
//    i2c_SendByte(cmd>>8);
//    i2c_WaitAck();
//    i2c_SendByte(cmd&0xFF);
//    i2c_WaitAck();
//    if(stop)
//        i2c_Stop();
//}

////软复位
//void SHT30_SOFTRESET(void)
//{
//    SHT30_SendBytes(0x30A2,1);
//}

////通用复位
//void SHT30_General_RESET(void)
//{
//    i2c_Start();
//    i2c_SendByte(0x00);
//    i2c_WaitAck();
//    i2c_SendByte(0x06);
//    i2c_WaitAck();
//}

////获取状态寄存器数据
//void SHT30_Get_Status(u8 *buffer)
//{
//    SHT30_SendBytes(0xF32D,0);
//    delay_ms(3);
//    i2c_Start();
//    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//写7位I2C设备地址加0作为写取位,1为读取位
//    if(i2c_WaitAck()==0)
//    {
//        buffer[0]=i2c_ReadByte(1);
//        buffer[1]=i2c_ReadByte(1);
//        buffer[2]=i2c_ReadByte(1);
//        i2c_Stop();
//    }
//}

////清空状态寄存器
//void SHT30_Clear_Status(void)
//{
//    SHT30_SendBytes(0x3041,1);
//}

////返回0代表状态寄存器读取成功，1代表读取错误
//u8 SHT30_Status(void)
//{
//    SHT30_Get_Status(status);
//    
//    if(CRC_Check(status,2,*(status+2)))
//        return 0;//正确
//    else
//        return 1;//错误
//}

////单次读取数据
//void SHT30_Single_Shot(u8 *buffer)
//{
//    u8 try_time=100;
//    
//    SHT30_SendBytes(0x2C06,1);
//    
//    //delay_ms(20);//很重要！不然读不出数据
//	
//	  delay_ms(100);//2022-4-8 14:21:57
//	
//	
//    i2c_Start();
//    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//写7位I2C设备地址加0作为写取位,1为读取位

//    while(i2c_WaitAck())
//    {
//        try_time--;
//        delay_us(50);
//        if(try_time==0)
//            return;
//    }
//    buffer[0]=i2c_ReadByte(1);
//    buffer[1]=i2c_ReadByte(1);
//    buffer[2]=i2c_ReadByte(1);
//    buffer[3]=i2c_ReadByte(1);
//    buffer[4]=i2c_ReadByte(1);
//    buffer[5]=i2c_ReadByte(0);
//    i2c_Stop();
//}

////开启/关闭加热器
//void SHT30_Heater(u8 enable)
//{
//    if(enable)
//        SHT30_SendBytes(0x306D,1);
//    else
//        SHT30_SendBytes(0x3066,1);
//}

////加速响应指令
//void SHT30_ART(void)
//{
//    SHT30_SendBytes(0x2B32,0);
//}

////配置周期读取重复性和采样速率
//void SHT30_Periodic_SetRepeat(void)
//{
//    SHT30_SendBytes(0x2737,0);//高重复度，mps为10
//}

////配置周期读取总配置
//void SHT30_Periodic_Config(void)
//{
//    //配置
//    SHT30_Periodic_SetRepeat();
//    SHT30_ART();
//}
////周期读取数据 如果使用就要在初始化时加上SHT30_Periodic_Config()函数
//void SHT30_Periodic(u8 *buffer)
//{
//    u8 try_time=100;
//    
//    //获取数据
//    SHT30_SendBytes(0xE000,0);
//    delay_ms(3);//很重要！不然读不出数据

//    i2c_Start();
//    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//写7位I2C设备地址加0作为写取位,1为读取位
//    
//    while(i2c_WaitAck())
//    {
//        try_time--;
//        delay_us(50);
//        if(try_time==0)
//            return;
//    }
//    buffer[0]=i2c_ReadByte(1);
//    buffer[1]=i2c_ReadByte(1);
//    buffer[2]=i2c_ReadByte(1);
//    buffer[3]=i2c_ReadByte(1);
//    buffer[4]=i2c_ReadByte(1);
//    buffer[5]=i2c_ReadByte(0);
//    i2c_Stop();
//}

////中断指令/停止周期获取数据功能
//void SHT30_Stop_Break(void)
//{
//    SHT30_SendBytes(0x3093,1);
//    delay_ms(15);
//}

////获取SHT3x温湿度
//void SHT30_Read(void)
//{    
//    u8 buff[6];   //获取raw数据
//    u16 tem,hum;  //拼接温湿度数据
//    u8 crcT,crcH; //温度和湿度的CRC校验

//    float Temperature=0;//转换后的温湿度
//    float Humidity=0;

//    //SHT30_Single_Shot(buff);
//    SHT30_Periodic(buff);
//    
//    tem = ((buff[0]<<8) | buff[1]);//温度拼接
//    hum = ((buff[3]<<8) | buff[4]);//湿度拼接

//    //计算温度和湿度CRC校验码
//    crcT = CRC_Check(buff,2,buff[2]);   //温度
//    crcH = CRC_Check(buff+3,2,buff[5]); //湿度
//    
//	

//	
//	  
//		//根据手册计算公式计算
//			Temperature = (175.0*(float)tem/65535.0-45.0) ;  // T = -45 + 175 * tem / (2^16-1)
//			Humidity    = (100.0*(float)hum/65535.0);           // RH = hum*100 / (2^16-1)
//			if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤超出测量范围的错误数据
//			{
//					temperature.fdata=Temperature;
//					humidity.fdata=Humidity;
//			}
//	
//	  printf("\r\n\r\n tem:%d\r\n",tem);
//		printf(" hum:%d\r\n",hum);
//    
//		printf(" crcT:%d\r\n",crcT);
//		printf(" crcH:%d\r\n",crcH);	
//		 	
//		printf(" tem:%lf\r\n",Temperature);
//		printf(" Hum:%lf\r\n",Humidity);		
//				
//			
//			
//			
//			
//			
//	  printf(" sht30_ceshi_front\r\n");
//			
//    if(crcT&&crcH)//判断CRC校验是否对
//    {
//        //根据手册计算公式计算
//        Temperature = (175.0*(float)tem/65535.0-45.0) ;  // T = -45 + 175 * tem / (2^16-1)
//        Humidity    = (100.0*(float)hum/65535.0);           // RH = hum*100 / (2^16-1)
//        if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤超出测量范围的错误数据
//        {
//            temperature.fdata=Temperature;
//            humidity.fdata=Humidity;
//        }
//					//测试
//					printf("\r\n temperature:%lf\r\n",Temperature);
//					printf("\r\n Humidity:%lf\r\n",Humidity);
//     }
//		
//		printf(" sht30_ceshi_back\r\n\r\n");
//		
//		
//}


/**************************************************上次测试，暂时屏蔽*************************************************************/


				 

unsigned short int   sht30_data_buffer[6];

float SHT30_temperature;
float SHT30_humidity;




//初始化SHT30
void SHT30_Init(void)
{
    delay_ms(250); 
   	HTIIC_Start();
   	HTIIC_Send_Byte(0x88);
   	HTIIC_Wait_Ack();
   	HTIIC_Send_Byte(0x21);
   	HTIIC_Wait_Ack();
   	HTIIC_Send_Byte(0x30);
   	HTIIC_Wait_Ack();
    HTIIC_Stop();
   
    delay_ms(150); 
}




//读取温湿度
void sht30_read_temp_humi(u16 *p)
{
   HTIIC_Start();
   HTIIC_Send_Byte(0x88);
   HTIIC_Wait_Ack();
   HTIIC_Send_Byte(0xe0);
   HTIIC_Wait_Ack();
   HTIIC_Send_Byte(0x00);
   HTIIC_Wait_Ack();

	 HTIIC_Start();
   HTIIC_Send_Byte(0x89);
   HTIIC_Wait_Ack();
   
   p[0] = HTIIC_Read_Byte(1);
   p[1] = HTIIC_Read_Byte(1);
   p[2] = HTIIC_Read_Byte(1);
   p[3] = HTIIC_Read_Byte(1);
   p[4] = HTIIC_Read_Byte(1);
   p[5] = HTIIC_Read_Byte(0);
	
   HTIIC_Stop();
}







//处理读取的数据
int sht30_data_process(void)
{
  u8 temporary[3];
  u16 data;
  u8 crc_result;
	
  sht30_read_temp_humi(sht30_data_buffer);
  temporary[0]=sht30_data_buffer[0];
  temporary[1]=sht30_data_buffer[1];
  temporary[2]=sht30_data_buffer[2];
  crc_result=sht30_crc8_check(temporary,2,temporary[2]);
	
	
if(crc_result==0)
		{
		data=((u16)temporary[0] << 8) | temporary[1];
		SHT30_temperature = (int)((175.0 * ((float)data) / 65535.0 - 45.0) *10.0);
		}
else
		{
		return 1;
		}
  temporary[0]=sht30_data_buffer[3];
  temporary[1]=sht30_data_buffer[4];
  temporary[2]=sht30_data_buffer[5];
  crc_result=sht30_crc8_check(temporary,2,temporary[2]);
	
	
		
if(crc_result==0)
		{
		data=((u16)temporary[0] << 8) | temporary[1];
		SHT30_humidity = (int)((100.0 * (float)data / 65535.0) *10.0); 
		return 0;
		}
else
		{
		return 2;
		}
	
	
	//=2022-4-13 13:13:45
	printf("temperature:%6.1fC\r\nhumidity:%6.1f\r\n",SHT30_temperature/10,SHT30_humidity/10);
	
}








//下面两个函数为CRC校验
 int  crc8_compute(u8 *check_data, u8 num_of_data)
{
 	uint8_t bit;        // bit mask
 uint8_t crc = 0xFF; // calculated checksum
 uint8_t byteCtr;    // byte counter

 // calculates 8-Bit checksum with given polynomial
 for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) {
     crc ^= (check_data[byteCtr]);
     for(bit = 8; bit > 0; --bit) {
         if(crc & 0x80) {
             crc = (crc << 1) ^ 0x31;
         }  else {
             crc = (crc << 1);
         }
     }
 }
 return crc;
}







int sht30_crc8_check(u8 *p,u8 num_of_data,u8 CrcData)
{
  uint8_t crc;
  crc = crc8_compute(p, num_of_data);// calculates 8-Bit checksum
 if(crc != CrcData) 
 {   
     return 1;           
 }
 return 0;
}





















