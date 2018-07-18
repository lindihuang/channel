#include "relay.h"
#include "mb.h"




void Relay_Config(void)
{
	 		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOG, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_11|GPIO_Pin_2;
		GPIO_Init(GPIOI, &GPIO_InitStructure);  
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
//		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
//		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_12;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_13|GPIO_Pin_15;
		GPIO_Init(GPIOH, &GPIO_InitStructure);
    
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
		GPIO_Init(GPIOG, &GPIO_InitStructure);

				/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
      
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);  
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		relay1_OFF;
		relay2_OFF;
		relay3_OFF;
		relay4_OFF;
		relay5_OFF;
		relay6_OFF;
		relay7_OFF;
	  relay8_OFF;
		relay9_OFF;
		relay10_OFF;
		relay11_OFF;
		relay12_OFF;
		relay13_OFF;
		relay14_OFF;
		relay15_OFF;
		relay16_OFF;
		

}

