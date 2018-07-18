#ifndef __COMMON_H
#define __COMMON_H

//#include "bitband.h"
#include "stm32f4xx.h"

#define EN_USART_RX         //使能串口1接收中断
#define EN_USART3_RX
//#define X0  	PAin(2)
//#define Y0  	PBout(12)

//#define LED1		BIT_ADDR(GPIOB_ODR_Addr,0)
//#define LED1_READ	BIT_ADDR(GPIOB_IDR_Addr,0)

//#define LED2		BIT_ADDR(GPIOB_ODR_Addr,1)
//#define LED2_READ	BIT_ADDR(GPIOB_IDR_Addr,1)

#define KEY			BIT_ADDR(GPIOB_IDR_Addr,2)



extern uint8_t USART_RxDat;       //接收数据
extern uint8_t USART_RxFlag;      //接收标志位
//void Delay_Init(void);       //延时初始化函数
//void Delay_ms(uint16_t ms);       //延时X毫秒函数
//void Delay_us(uint32_t us);       //延时X微妙函数
//void USART_Config(USART_TypeDef* USARTx,uint32_t Baud);  //串口初始化
void RTC_Configuration(void);
uint8_t UartSendData(USART_TypeDef* USARTx,uint8_t ch);
void UartSendString(USART_TypeDef* USARTx,char *str);
void Delay(vu32 nCount);

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
void TimingDelay_Decrement(void);
#define Delay_ms(x) Delay_us(1000*x)	 //单位ms

#endif




