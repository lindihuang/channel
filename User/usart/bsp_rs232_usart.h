#ifndef __RS232_USART_H
#define	__RS232_USART_H

#include "stm32f4xx.h"
#include <stdio.h>






//引脚定义
/*******************************************************/


#define _485_USART                             	USART3
#define _485_USART_CLK                         	RCC_APB1Periph_USART3
#define _485_USART_BAUDRATE                    	38400       //115200     //38400

#define _485_USART_RX_GPIO_PORT                	GPIOB
#define _485_USART_RX_GPIO_CLK                 	RCC_AHB1Periph_GPIOB
#define _485_USART_RX_PIN                      	GPIO_Pin_11
#define _485_USART_RX_AF                       	GPIO_AF_USART3
#define _485_USART_RX_SOURCE                   	GPIO_PinSource11

#define _485_USART_TX_GPIO_PORT                	GPIOB
#define _485_USART_TX_GPIO_CLK                 	RCC_AHB1Periph_GPIOB
#define _485_USART_TX_PIN                      	GPIO_Pin_10
#define _485_USART_TX_AF                       	GPIO_AF_USART3
#define _485_USART_TX_SOURCE                   	GPIO_PinSource10


#define _485_RE_GPIO_PORT												GPIOD
#define _485_RE_GPIO_CLK												RCC_AHB1Periph_GPIOD
#define _485_RE_PIN															GPIO_Pin_11

#define _485_INT_IRQ                 						USART3_IRQn
#define _485_IRQHandler                         USART3_IRQHandler
/************************************************************/
	/// 不精确的延时
static void _485_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 
/*控制收发引脚*/
//进入接收模式,必须要有延时等待485处理完数据
#define _485_RX_EN()			_485_delay(1000); GPIO_ResetBits(_485_RE_GPIO_PORT,_485_RE_PIN);  _485_delay(1000);
//进入发送模式,必须要有延时等待485处理完数据
#define _485_TX_EN()			_485_delay(1000); GPIO_SetBits(_485_RE_GPIO_PORT,_485_RE_PIN);  _485_delay(1000);

/*debug*/

#define _485_DEBUG_ON          1
#define _485_DEBUG_ARRAY_ON   1
#define _485_DEBUG_FUNC_ON    1
   
   
// Log define
#define _485_INFO(fmt,arg...)           printf("<<-_485-INFO->> "fmt"\n",##arg)
#define _485_ERROR(fmt,arg...)          printf("<<-_485-ERROR->> "fmt"\n",##arg)
#define _485_DEBUG(fmt,arg...)          do{\
																					 if(_485_DEBUG_ON)\
																					 printf("<<-_485-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
																				 }while(0)

#define _485_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(_485_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-_485-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)

#define _485_DEBUG_FUNC()               do{\
                                         if(_485_DEBUG_FUNC_ON)\
                                         printf("<<-_485-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)
////引脚定义
///*******************************************************/
//#define RS232_USART                             USART1
//#define RS232_USART_CLK                         RCC_APB2Periph_USART1

//#define RS232_USART_RX_GPIO_PORT                GPIOA
//#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
//#define RS232_USART_RX_PIN                      GPIO_Pin_3
//#define RS232_USART_RX_AF                       GPIO_AF_USART1
//#define RS232_USART_RX_SOURCE                   GPIO_PinSource3

//#define RS232_USART_TX_GPIO_PORT                GPIOA
//#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
//#define RS232_USART_TX_PIN                      GPIO_Pin_2
//#define RS232_USART_TX_AF                       GPIO_AF_USART1
//#define RS232_USART_TX_SOURCE                   GPIO_PinSource2


//#define RS232_USART_IRQHandler                   USART1_IRQHandler
//#define RS232_USART_IRQ                 					USART1_IRQn
///************************************************************/


//串口波特率
#define RS232_USART_BAUDRATE                    38400



//void Debug_USART_Config(void);
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );
void UartClearBuffer(void);
void Uart_sendBuffer(u8 *p,int len);
void NVIC_Configuration(void);
//int fputc(int ch, FILE *f);

#endif /* __USART1_H */
