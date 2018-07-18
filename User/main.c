/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用1.5.1版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
 #include "bsp_exti.h" 
#include "stm32f4xx.h"
#include "./usart/bsp_rs232_usart.h" 
#include "bsp_led.h"   
#include "string.h"
#include "Timer5.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "relay.h"
#include "bsp_SysTick.h"

#include "bsp_debug_usart.h"
#include "string.h"
#include "math.h"
#include "modbus.h"
#include <float.h>
#include <stdlib.h>

#include "KEY.h" 
#include "Timer1.h"
#include "new.h"
#include "data_processing.h" 
#include "ADS1256.h"
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
	
uint16_t Start_Over_Flag_1;

//USHORT   usSRegInBuf[100];
//union LianHeTi test_data1;
void pianyiliangchange(uint8_t *array,uint32_t num);
void Setzero_Init(void);
void set_zero(void);
void STOP_INTERRUP(void);
//const 	uint16_t chushizhi=3000;  //初始大气压下的ad值
//uint16_t pianyiliang=0;   //调零偏移量
void out_start_stop(void);
void Press_z_f(void);
uint16_t press_z_flag=0;
uint16_t press_f_flag=0;
int standard_AD=3000;

int main(void)
{
	

	uint16_t len;
//	int16_t unit, polarity, point;
//	float data = 0, point_t;
//	const char str_read_datas[]  = {'$', 'H', X1+0x30, 'E',  X2+0x30, 'L', 'C',  'D', 'M'};//读取数据
	char *pbuf;
	press_f_flag=0;
	press_z_flag=0;
	Timer5_cout=0;

//	uint16_t usRxCount=0; 
		/* 程序来到main函数之前，启动文件：statup_stm32f429xx.s已经调用
		* SystemInit()函数把系统时钟初始化成180MHZ
		* SystemInit()在system_stm32f4xx.c中定义
		* 如果用户想修改系统时钟，可自行编写程序修改
		*/
		  /* add your code here ^_^. */
	  /* 配置SysTick 为10us中断一次,时间到后触发定时中断，
	*进入stm32fxx_it.c文件的SysTick_Handler处理，通过数中断次数计时
	*/
	SysTick_Init();
	
  /*初始化USART1*/
  DEBUG_USART_Config();
	


	
	/*继电器初始化*/
	Relay_Config();
	
	/*定时器5初始化*/
	Timer5_config();
	

	
	/*定时器1初始化*/
	TIMx_Configuration();
//	TIM4_Configuration();
//	
	/*LED初始化*/
	LED_GPIO_Config();
//	EXTI_Key_Config();
////	/*按键初始化*/
	Key_GPIO_Config();
	
	eMBInit(MB_RTU, 1, 1, 9600, MB_PAR_NONE);

	eMBEnable();
	
	Init_ADS1256_GPIO(); //初始化ADS1256 GPIO管脚 

  Delay_ms(200);

	ADS1256_Init();
	
	Delay_ms(200);
	
	ADS1256_Init();
	
	Delay_ms(200);
	
	ADS1256_Init();
	
	Delay_ms(200);
	
	ADS1256_Init();
	
	Delay_ms(200);
	
	ADS1256_Init();
	
	Delay_ms(200);
	
	Setzero_Init();//启动·调零
	ucSCoilBuf[0]&=(~(1<<2));
	Start_Over_Flag_1=0;

  ucSCoilBuf[0]=0x00;		


  GS_Flag=0;
   
  while(1)
	{

		set_zero();



			STOP_INTERRUP();

			out_start_stop();	
			
			Press_z_f();

       ( void)eMBPoll(); 

///***************************************************MPM270*************************************************************/		 
			  caijitongdao(1,disable,0,0);
///***************************************************MPM270*************************************************************/
			 Step();
		   if(!(ucSCoilBuf[0]&0x02))
			 {
				 ucSCoilBuf[1]|=0x02;
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
				TIM_SetCounter(TIM3, 0);
				 Timer3_cout=0;
			 }
			 AUTO_ZERO();
					Relay_control();
	
//			 
			 
   }

		
}

//**********************************************************************************
//函数定义: void Setzero_Init(void)	
//描    述：外部按键灯控制

//备    注：
//**********************************************************************************
void STOP_INTERRUP(void)
{
////		if((!GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_4))||(!(ucSCoilBuf[5]&0x01)))
////		{
////			
////			ucSCoilBuf[0]&=0xfd;
////			relay11_OFF;
////			relay12_OFF;
////			relay13_OFF;
////			relay14_OFF;
////			relay15_OFF;
////			relay16_OFF;
	  if(ucSCoilBuf[4]&0x01)
		{
			relay9_ON;
			Timer5_cout=0;
		}
		else 
		{
			relay9_OFF;
		}
		if(ucSCoilBuf[4]&0x02)
		{
			relay10_ON;
	
		}
		else 
		{
			relay10_OFF;
		}

////		}
}  
//**********************************************************************************
//函数定义: static void Delay(__IO uint32_t nCount)	
//描    述：简单的延时函数

//备    注：
//**********************************************************************************
static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

//**********************************************************************************
//函数定义: void out_start_stop(void)
//描    述：外部启动停止按键

//备    注：
//**********************************************************************************

void out_start_stop(void)
{
		if(( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )&&(GS_Flag==0))
		{
			/*LED1反转*/


      ucSCoilBuf[7]|=0x01;
			
			
		}   
    
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
		{
			/*LED2反转*/

			if(ucSCoilBuf[0]&0x02)
			{

				ucSCoilBuf[0]|=0x01; 
				ucSCoilBuf[0]&=0xf1; 
				ucSCoilBuf[7]&=0xfe;            //参数准备状态归零
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
				TIM_SetCounter(TIM3, 0);
				Timer3_cout=0;

	
		   }   
			else
			{
				relay5_ON;
				relay6_ON;
				Delay_ms(500);
				relay6_OFF;
				Delay_ms(100);
				relay5_OFF;
			}
		}
}
//**********************************************************************************
//函数定义: void out_start_stop(void)
//描    述：压力设置正负压

//备    注：
//**********************************************************************************
void Press_z_f(void) 
{
		if((ucSCoilBuf[5]&0x08)&&(!(ucSCoilBuf[5]&0x10)))  //正压
	{
	    press_z_flag=1;
			relay1_OFF;
			relay2_ON;	
			relay4_OFF;	
			relay6_ON;
		  ucSCoilBuf[3]&=(~0x09);
		
			ucSCoilBuf[3]|=0x22;

	}
	if((!(ucSCoilBuf[5]&0x08))&&(press_z_flag==1))
	{
			relay1_OFF;
			relay2_OFF;	
			relay4_OFF;	
			relay6_OFF;
			press_z_flag=0;
		ucSCoilBuf[3]&=(~0x22);
		
	}
	
		if((!(ucSCoilBuf[5]&0x08))&&(ucSCoilBuf[5]&0x10) ) //负压
	{
	    press_f_flag=1;
			relay1_ON;
			relay2_OFF;	
			relay4_ON;	
			relay6_ON;
			ucSCoilBuf[3]&=(~0x02);
			ucSCoilBuf[3]|=0x29;

	}
	if((!(ucSCoilBuf[5]&0x10))&&(press_f_flag==1))
	{
			relay1_OFF;
			relay2_OFF;	
			relay4_OFF;	
			relay6_OFF;
			press_f_flag=0;
		  ucSCoilBuf[3]&=(~0x29);
		
	}
}

/*********************************************END OF FILE**********************/

