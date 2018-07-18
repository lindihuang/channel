
#include <stdio.h>
#include "stm32f4xx_gpio.h"
#include "ADS1256.h"
//#include "common.h"
#include "bsp_led.h"
#include "bsp_SysTick.h"

//***************************
//		Pin assign	   	
//			STM32			ADS1256		
//		GPIOB_Pin_11 		<--- DRDY
//		GPIOB_Pin_12 		---> CS
//		GPIOB_Pin_13 		---> SCK
//		GPIOB_Pin_14(MISO)  <--- DOUT
//		GPIOB_Pin_15(MOSI)  ---> DIN
//***************************	

/*端口定义*/ 

#define RCC_DRDY			RCC_AHB1Periph_GPIOB
#define PORT_DRDY			GPIOB
#define PIN_DRDY			GPIO_Pin_12

#define PORT_CS				GPIOA
#define PIN_CS				GPIO_Pin_4

//#define ads1256_SPI_SCK_GPIO_PORT                 GPIOB
//#define ads1256_SPI_SCK_AF                       GPIO_AF_SPI2
//#define ads1256_SPI_SCK_SOURCE                   GPIO_PinSource13

//#define ads1256_SPI_MISO_GPIO_PORT                 GPIOB
//#define ads1256_SPI_MISO_AF                       GPIO_AF_SPI2
//#define ads1256_SPI_MISO_SOURCE                   GPIO_PinSource14

//#define ads1256_SPI_MOSI_GPIO_PORT                 GPIOB
//#define ads1256_SPI_MOSI_AF                       GPIO_AF_SPI2
//#define ads1256_SPI_MOSI_SOURCE                   GPIO_PinSource15

#define ads1256_SPI_SCK_GPIO_PORT                 GPIOA
#define ads1256_SPI_SCK_AF                       GPIO_AF_SPI1
#define ads1256_SPI_SCK_SOURCE                   GPIO_PinSource5

#define ads1256_SPI_MISO_GPIO_PORT                 GPIOA
#define ads1256_SPI_MISO_AF                       GPIO_AF_SPI1
#define ads1256_SPI_MISO_SOURCE                   GPIO_PinSource6

#define ads1256_SPI_MOSI_GPIO_PORT                 GPIOA
#define ads1256_SPI_MOSI_AF                       GPIO_AF_SPI1
#define ads1256_SPI_MOSI_SOURCE                   GPIO_PinSource7

#define CS_0()				GPIO_ResetBits(PORT_CS, PIN_CS);
#define CS_1()				GPIO_SetBits(PORT_CS, PIN_CS);
#define ADS1256_DRDY  		(PORT_DRDY->IDR & PIN_DRDY)


void SPI1_Init(void)
{
 SPI_InitTypeDef  SPI_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;
 /****Initial SPI1******************/
 
 /* Enable SPI1 and GPIOB clocks */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

// RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	/* 连接 引脚源*/
  GPIO_PinAFConfig(ads1256_SPI_SCK_GPIO_PORT,ads1256_SPI_SCK_SOURCE,ads1256_SPI_SCK_AF);
  GPIO_PinAFConfig(ads1256_SPI_MISO_GPIO_PORT,ads1256_SPI_MISO_SOURCE,ads1256_SPI_MISO_AF);
  GPIO_PinAFConfig(ads1256_SPI_MOSI_GPIO_PORT,ads1256_SPI_MOSI_SOURCE,ads1256_SPI_MOSI_AF);
	
  /* SPI1 configuration */ 
 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI1设置为两线全双工
 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                    //设置SPI1为主模式
 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //SPI发送接收8位帧结构
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                   //串行时钟在不操作时，时钟为低电平
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                 //第一个时钟沿开始采样数据
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                  //NSS信号由软件（使用SSI位）管理
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为8
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;       //数据传输从MSB位开始
 SPI_InitStructure.SPI_CRCPolynomial = 7;         //CRC值计算的多项式
 SPI_Init(SPI1, &SPI_InitStructure);
 /* Enable SPI1  */
// SPI_Cmd(SPI1, ENABLE); 
SPI_Cmd(SPI1, ENABLE);
}  

//初始化ADS1256 GPIO
void Init_ADS1256_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_DRDY , ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); 

	GPIO_InitStructure.GPIO_Pin = PIN_DRDY; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(PORT_DRDY, &GPIO_InitStructure);
	 //SPI1 NSS 
	CS_1();
    GPIO_InitStructure.GPIO_Pin = PIN_CS;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PORT_CS, &GPIO_InitStructure);
//	 LED_BLUE;		
	CS_1();
	SPI1_Init();
}


//-----------------------------------------------------------------//
//	功    能：  模拟SPI通信
//	入口参数: /	发送的SPI数据
//	出口参数: /	接收的SPI数据
//	全局变量: /
//	备    注: 	发送接收函数
//-----------------------------------------------------------------//
unsigned char SPI_WriteByte(unsigned char TxData)
{
  unsigned char RxData=0;
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
  SPI_I2S_SendData(SPI1,TxData);
//	Delay_us(5);
	Delay_us(25);
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
  RxData=SPI_I2S_ReceiveData(SPI1);
//	Delay_us(5);
  return RxData;
	
} 

//-----------------------------------------------------------------//
//	功    能：ADS1256 写数据
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注: 向ADS1256中地址为regaddr的寄存器写入一个字节databyte
//-----------------------------------------------------------------//
void ADS1256WREG(unsigned char regaddr,unsigned char databyte)
{
    
	CS_0();
//	Delay_us(10);
	while(ADS1256_DRDY);//当ADS1256_DRDY为低时才能写寄存器
	  //向寄存器写入数据地址
    SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x0F));
    //写入数据的个数n-1
    SPI_WriteByte(0x00);
    //向regaddr地址指向的寄存器写入数据databyte
    SPI_WriteByte(databyte);
	
	CS_1();
//	Delay_us(10);
}


//初始化ADS1256
void ADS1256_Init(void)
{
	//*************自校准****************
	
  while(ADS1256_DRDY);
	LED_BLUE;	
	CS_0();
	SPI_WriteByte(ADS1256_CMD_SELFCAL);
	while(ADS1256_DRDY);
	CS_1();
	//**********************************
 
	ADS1256WREG(ADS1256_STATUS,0x06);               // 高位在前、使用缓冲
//	ADS1256WREG(ADS1256_STATUS,0x04);               // 高位在前、不使用缓冲

//	ADS1256WREG(ADS1256_MUX,0x08);                  // 初始化端口A0为‘+’，AINCOM位‘-’
	ADS1256WREG(ADS1256_ADCON,ADS1256_GAIN_4);                // 放大倍数4
//	ADS1256WREG(ADS1256_ADCON,ADS1256_GAIN_8);                // 放大倍数8 高压用
	ADS1256WREG(ADS1256_DRATE,ADS1256_DRATE_100SPS);  // 数据100sps    

	ADS1256WREG(ADS1256_IO,0x00);               

	//*************自校准****************
	while(ADS1256_DRDY);
	CS_0();
	SPI_WriteByte(ADS1256_CMD_SELFCAL);
	while(ADS1256_DRDY);
	CS_1(); 
	//**********************************
}

//读取AD值
signed int ADS1256ReadData(unsigned char channel)  
{

  unsigned int sum=0;
	while(ADS1256_DRDY);//当ADS1256_DRDY为低时才能写寄存器 
	ADS1256WREG(ADS1256_MUX,channel);		//设置通道
	CS_0();
	
	SPI_WriteByte(ADS1256_CMD_SYNC);
	SPI_WriteByte(ADS1256_CMD_WAKEUP);	               
	SPI_WriteByte(ADS1256_CMD_RDATA);
	Delay_us(8);
   	sum |= (SPI_WriteByte(0xff) << 16);
	sum |= (SPI_WriteByte(0xff) << 8);
	sum |= SPI_WriteByte(0xff);
	CS_1();

	if (sum>0x7FFFFF)           // if MSB=1, 
	{
		sum -= 0x1000000;       // do 2's complement

	}
    return sum;
}

