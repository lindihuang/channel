#include "Timer5.h"
#include "mb.h"
//#include "modbus.h"

void Timer5_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  TIM_DeInit(TIM5);

	/*
	3.5���ַ�ʱ�����ֲ�ͬ��֡�������յ��������ַ�֮��ʱ����С��3.5���ַ�
	ʱ��ʱ��Ϊ��ͬһ��֡�ģ�����������3.5���ַ�ʱ������Ϊ�ǲ�ͬ֡��
	��һ��Ĵ���ͨ���У�����1���ַ���Ҫ��1λ��ʼλ��8λ����λ��1λУ��λ(����),
	1λֹͣλ,�ܹ� 1+8+1+1 = 11λ��3.5���ַ�ʱ����� 3.5 * 11 = 38.5λ��
	���粨������9600,��ô����1λ��ʱ����1000/9600 = 0.10416667(ms) ,
	������3.5���ַ�ʱ��ʹ�Լ�� 4 ms ,����ʱ����Ҫ���ж�ʱ��
	*/

  // �������Ԥ��Ƶϵ��9000/180M = 0.00005,��ÿ50us����ֵ��1
  //50us x 2000 = 100ms,��100ms�ж�һ��	
  TIM_TimeBaseStructure.TIM_Period = 2000-1;
  TIM_TimeBaseStructure.TIM_Prescaler = (9000 - 1);	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
		//Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM5, ENABLE);
	//====================================�жϳ�ʼ��===========================
	//����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	

		//�������жϱ�־λ
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	//��ʱ��3����жϹر�
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	//��ʱ��3����
	TIM_Cmd(TIM5, DISABLE);
}

