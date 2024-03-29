#include "stm32f4xx.h"
#include "stdio.h"
#include "common.h"

//static uint8_t  fac_us=0; //us延时倍乘数
//static uint16_t fac_ms=0; //ms延时倍乘数


/*******************************************************************************
* 函数名称       :  
* 功能描述       : Printf支持代码   printf("变量a的值是: %d\r\n\r\n",a);
* 入口参数       : 无
* 出口参数       : 无
*******************************************************************************/
#if 1
#pragma import(__use_no_semihosting)             
                
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
 
_sys_exit(int x) //定义_sys_exit()以避免使用半主机模式   
{ 
	x = x; 
} 

//int fputc(int ch, FILE *f)//重定义fputc函数 
//{      
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
//    USART_SendData(USART1,(uint8_t)ch);   
//	return ch;
//}
#endif 


static __IO u32 TimingDelay;
 
/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))
	{ 
		/* Capture error */ 
		while (1);
	}
}

/**
  * @brief   us延时程序,10us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
  * @retval  无
  */
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}


///*******************************************************************************
//* 函数名称       : Delay_Init
//* 功能描述       : 延时初始化函数
//* 入口参数       : 无
//* 出口参数       : 无
//*******************************************************************************/
//void Delay_Init(void)
//{
//	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
//	fac_us=9;		    
////	fac_us=SYSCLK/8;
//	fac_ms=(uint16_t)fac_us*1000;
//}								    

///*******************************************************************************
//* 函数名称       : Delay_ms
//* 功能描述       : 延时函数，72M条件下ms<=1864
//* 入口参数       : ms 延时大小
//* 出口参数       : 无
//*******************************************************************************/
//void Delay_ms(uint16_t ms)
//{	 		  	  
//	uint32_t temp;		   
//	SysTick->LOAD=(uint32_t)ms*fac_ms; //时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;            //清空计数器
//	SysTick->CTRL=0x01 ;           //开始倒数  
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16))); //等待时间到达   
//	SysTick->CTRL=0x00;                //关闭计数器
//	SysTick->VAL =0X00;                //清空计数器	  	    
//}   

///*******************************************************************************
//* 函数名称       : Delay_us
//* 功能描述       : 延时函数，72M条件下ms<=1864000
//* 入口参数       : us 延时大小
//* 出口参数       : 无
//*******************************************************************************/
//void Delay_us(uint32_t us)
//{		
//	uint32_t temp;	    	 
//	SysTick->LOAD=us*fac_us; //时间加载	  		 
//	SysTick->VAL=0x00;        //清空计数器
//	SysTick->CTRL=0x01 ;      //开始倒数 	 
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16))); //等待时间到达   
//	SysTick->CTRL=0x00;                //关闭计数器
//	SysTick->VAL =0X00;                //清空计数器	 
//}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/*******************************************************************************
* 函数名称       : USART_Config
* 功能描述       : 配置串口
* 入口参数       : USARTx 串口号 
                   Baud   波特率
* 出口参数       : 无
*******************************************************************************/
//void USART_Config(USART_TypeDef* USARTx,u32 Baud)
//{
//	USART_InitTypeDef USART_InitStructure;
//	USART_InitStructure.USART_BaudRate =Baud;	//波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1位
//	USART_InitStructure.USART_Parity = USART_Parity_No; //无校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

//	USART_Init(USARTx, &USART_InitStructure);	//配置串口参数函数
//	
//#ifdef EN_USART_RX
//	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);  //使能接收中断
//#endif
//	//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE); //使能发送缓冲空中断
//	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //使能发送完成中断

//	/* Enable the USART1 */
//	USART_Cmd(USARTx, ENABLE); //使能串口	
//	/*CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
//	如下语句解决第1个字节无法正确发送出去的问题  */
//	USART_ClearFlag(USARTx, USART_FLAG_TXE);/* 清发送完成标志，Transmission Complete flag */ 
//}


/*******************************************************************************
* 函数名称       : UartSendData
* 功能描述       : 发送一个字节到串口
* 入口参数       : USARTx 串口号，数据
* 出口参数       : 返回发送的数据
*******************************************************************************/
u8 UartSendData(USART_TypeDef* USARTx,uint8_t ch)
{
	/* Write a character to the USART */
	USART_SendData(USARTx, ch);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
	return ch;
}

//发送一个字符串
void UartSendString(USART_TypeDef* USARTx,char *str)
{

	 while(*str!='\0')
	{
		USART_SendData(USARTx, *str++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
	}
}	


