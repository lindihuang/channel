#ifndef __DATA_PROCESSING_H
#define	__DATA_PROCESSING_H
#include "stm32f4xx.h"

#include "stm32f4xx.h"
#include "ads1256.h"
#include "./usart/bsp_debug_usart.h"
//#include "common.h"

#define cunchugeshu     12
#define enable          1
#define disable         0
extern int64_t channel[4][cunchugeshu]; //将读取的部分ad值存储在这个数组中
extern int32_t pianyiliang[4];   //调零偏移量
extern const 	int32_t chushizhi[4];
void Setzero_Init(void);
long double lvbo(int64_t weilvadzhi,uint8_t channels);
void Delays1(uint32_t x);
void caijitongdao(char channel_0,char channel_1,char channel_2,char channel_3);
void caijitongdao1(char channel_0,char channel_1,char channel_2,char channel_3);
void set_zero(void);


#endif /* __LED_H */
