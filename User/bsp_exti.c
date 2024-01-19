/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_exti.h"
#include "bsp_SysTick.h"

// /**
//  * @brief  ����Ƕ�������жϿ�����NVIC
//  * @param  ��
//  * @retval ��
//  */
//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* ����NVICΪ���ȼ���1 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* �����ж�Դ������1 */
//  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
//  /* ������ռ���ȼ���1 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  /* ���������ȼ���1 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* ʹ���ж�ͨ�� */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
//  /* �����ж�Դ������2������ʹ������������� */  
//  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
//  NVIC_Init(&NVIC_InitStructure);
//}

// /**
//  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
//  * @param  ��
//  * @retval ��
//  */
//void EXTI_Key_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	EXTI_InitTypeDef EXTI_InitStructure;
//  
//	/*��������GPIO�ڵ�ʱ��*/
//	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK|KEY2_INT_GPIO_CLK ,ENABLE);
//  
//  /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//  
//  /* ���� NVIC */
//  NVIC_Configuration();
//  
//	/* ѡ�񰴼�1������ */ 
//  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
//  /* ��������Ϊ����ģʽ */ 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
//  /* �������Ų�����Ҳ������ */
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  /* ʹ������Ľṹ���ʼ������ */
//  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure); 

//	/* ���� EXTI �ж�Դ ��key1���� */
//  SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);

//  /* ѡ�� EXTI �ж�Դ */
//  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
//  /* �ж�ģʽ */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  /* �½��ش��� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
//  /* ʹ���ж�/�¼��� */
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  
//	
//		/* ѡ�񰴼�1������ */ 
//  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
//  /* ��������Ϊ����ģʽ */ 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
//  /* �������Ų�����Ҳ������ */
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//  /* ʹ������Ľṹ���ʼ������ */
//  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure); 
//  /* ѡ�񰴼�2������ */ 
//  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;  
//  /* ����������������ͬ */
//  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      

//	/* ���� EXTI �ж�Դ ��key2 ���� */
//  SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);

//  /* ѡ�� EXTI �ж�Դ */
//  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  /* �����ش��� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//}


/// ����ȷ����ʱ
void Key_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*��������GPIO�ڵ�ʱ��*/
	RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY9_GPIO_CLK,ENABLE);
	
  /*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN; 
  
  /*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  
  /*�������Ų�����Ҳ������*/
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
  /*ʹ������Ľṹ���ʼ������*/
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);  
  
  /*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN; 
  
  /*ʹ������Ľṹ���ʼ������*/
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);  
	
	/*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Pin = KEY9_PIN; 
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	  /*ʹ������Ľṹ���ʼ������*/
	GPIO_Init(KEY9_GPIO_PORT, &GPIO_InitStructure); 
}

/**
  * @brief   ����Ƿ��а�������     
  *	@param 	GPIOx:����Ķ˿�, x�����ǣ�A...K�� 
	*	@param 	GPIO_PIN:����Ķ˿�λ�� ������GPIO_PIN_x��x������0...15��
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 	
		/*�ȴ������ͷ� */
		Delay_ms(10); 
		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
		{			
			return 	KEY_ON;	
		}			
	}
	else
	{
		return KEY_OFF;
	}
}
