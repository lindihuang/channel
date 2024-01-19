#include "mb.h"
#include "stm32f4xx.h"
#include "bsp_led.h" 
#include "relay.h"
#include "modbus.h"
#include "Timer5.h"
#include "mbrtu.h"
#include "main.h"
#include "bsp_SysTick.h"
#include "new.h"

float  Timer5_cout;
float  Timer4_cout;
float  Timer3_cout;
float  Timer1_cout;



uint16_t GS_Flag=0;

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        1
#define S_DISCRETE_INPUT_NDISCRETES   16
#define S_COIL_START                  1
#define S_COIL_NCOILS                 104
#define S_REG_INPUT_START             1
#define S_REG_INPUT_NREGS             100
#define S_REG_HOLDING_START           1
#define S_REG_HOLDING_NREGS           100
//�ӻ�ģʽ���ڱ��ּĴ����У�������ַ��Ӧ�Ĺ��ܶ���
#define          S_HD_RESERVE                     0		  //����
#define          S_HD_CPU_USAGE_MAJOR             1         //��ǰCPU�����ʵ�����λ
#define          S_HD_CPU_USAGE_MINOR             2         //��ǰCPU�����ʵ�С��λ

//�ӻ�ģʽ��������Ĵ����У�������ַ��Ӧ�Ĺ��ܶ���
#define          S_IN_RESERVE                     0		  //����

//�ӻ�ģʽ������Ȧ�У�������ַ��Ӧ�Ĺ��ܶ���
#define          S_CO_RESERVE                     2		  //����

//�ӻ�ģʽ������ɢ�����У�������ַ��Ӧ�Ĺ��ܶ���
#define          S_DI_RESERVE                     1		  //����
/*------------------------Slave mode use these variables----------------------*/
//////Slave mode:DiscreteInputs variables
USHORT   usSDiscInStart                               = S_DISCRETE_INPUT_START;
#if S_DISCRETE_INPUT_NDISCRETES%8
UCHAR    ucSDiscInBuf[S_DISCRETE_INPUT_NDISCRETES/8+1];
#else
UCHAR    ucSDiscInBuf[S_DISCRETE_INPUT_NDISCRETES/8]  ;
#endif
//Slave mode:Coils variables
USHORT   usSCoilStart                                 = S_COIL_START;
#if S_COIL_NCOILS%8
UCHAR    ucSCoilBuf[S_COIL_NCOILS/8+1]                ;
#else
 UCHAR    ucSCoilBuf[14]                  ;
#endif
//Slave mode:InputRegister variables
USHORT   usSRegInStart                                = S_REG_INPUT_START;
USHORT   usSRegInBuf[100];  
//float   usSRegInBuf[S_REG_INPUT_NREGS];


//Slave mode:HoldingRegister variables
USHORT   usSRegHoldStart                              = S_REG_HOLDING_START;
USHORT   usSRegHoldBuf[250]           ;


//UCHAR    ucSCoilBuf[8] ;
//******************************����Ĵ����ص�����**********************************
//��������: eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//��    ��������Ĵ�����صĹ��ܣ�������������
//* ��    �ܣ���ȡ����Ĵ�������Ӧ�������� 04 eMBFuncReadInputRegister
//��ڲ�����pucRegBuffer : �ص�������Modbus�Ĵ����ĵ�ǰֵд��Ļ�����
//			usAddress    : �Ĵ�������ʼ��ַ������Ĵ����ĵ�ַ��Χ��1-65535��
//			usNRegs      : �Ĵ�������
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS   3��
//**********************************************************************************
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;
	USHORT *        pusRegInputBuf;
//	    float *        pusRegInputBuf;
	UCHAR           REG_INPUT_START;
	UCHAR           REG_INPUT_NREGS;
	UCHAR           usRegInStart;

	pusRegInputBuf = usSRegInBuf;
	REG_INPUT_START = S_REG_INPUT_START;
	REG_INPUT_NREGS = S_REG_INPUT_NREGS;
	usRegInStart = usSRegInStart;
	
	if((usAddress >= REG_INPUT_START)
	&& (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
	{
		iRegIndex = ( int )( usAddress - usRegInStart );
		
		while( usNRegs > 0 )
		{
			*pucRegBuffer++ = ( unsigned char )( pusRegInputBuf[iRegIndex] >> 8 );
			*pucRegBuffer++ = ( unsigned char )( pusRegInputBuf[iRegIndex] & 0xFF );

			iRegIndex++;
			usNRegs--;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	return eStatus;
}
//******************************���ּĴ����ص�����**********************************
//��������: eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
//��    �������ּĴ�����صĹ��ܣ�������������д������д��
//* ��    �ܣ���Ӧ�������У�06 д���ּĴ��� eMBFuncWriteHoldingRegister 
//*													16 д������ּĴ��� eMBFuncWriteMultipleHoldingRegister
//*													03 �����ּĴ��� eMBFuncReadHoldingRegister
//*													23 ��д������ּĴ��� eMBFuncReadWriteMultipleHoldingRegister
//��ڲ�����pucRegBuffer : �����Ҫ�����û��Ĵ�����ֵ���������������ָ���µļĴ�����ֵ��
//                         ���Э��ջ��֪����ǰ����ֵ���ص��������뽫��ǰֵд�����������
//			usAddress    : �Ĵ�������ʼ��ַ��
//			usNRegs      : �Ĵ�������
//          eMode        : ����ò���ΪeMBRegisterMode::MB_REG_WRITE���û���Ӧ����ֵ����pucRegBuffer�еõ����¡�
//                         ����ò���ΪeMBRegisterMode::MB_REG_READ���û���Ҫ����ǰ��Ӧ�����ݴ洢��pucRegBuffer��
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS   4��
//**********************************************************************************
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;
	USHORT *        pusRegHoldingBuf;
	UCHAR           REG_HOLDING_START;
	UCHAR           REG_HOLDING_NREGS;
	UCHAR           usRegHoldStart;


	pusRegHoldingBuf = usSRegHoldBuf;
	REG_HOLDING_START = S_REG_HOLDING_START;
	REG_HOLDING_NREGS = S_REG_HOLDING_NREGS;
	usRegHoldStart = usSRegHoldStart;
	
	if( ( usAddress >= REG_HOLDING_START ) &&
			( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ))
	{
		iRegIndex = ( int )( usAddress - usRegHoldStart );
		
		switch (eMode)
		{
			/* Pass current register values to the protocol stack. */
			case MB_REG_READ:
				while(usNRegs > 0)
				{
					*pucRegBuffer++ = ( unsigned char )( pusRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( unsigned char )( pusRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;
				}
				break;

			/* Update current register values with new values from the
			 * protocol stack. */
			case MB_REG_WRITE:
				while( usNRegs > 0 )
				{
						pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
						pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
						iRegIndex++;
						usNRegs--;
				}
				break;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
//****************************��Ȧ״̬�Ĵ����ص�����********************************
//��������: eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
//��    ������Ȧ״̬�Ĵ�����صĹ��ܣ�������������д������д��
//* ��    �ܣ���Ӧ�������У�01 ����Ȧ eMBFuncReadCoils
//*													05 д��Ȧ eMBFuncWriteCoil
//*													15 д�����Ȧ eMBFuncWriteMultipleCoils
//��ڲ�����pucRegBuffer : λ���һ���ֽڣ���ʼ�Ĵ�����Ӧ��λ���ڸ��ֽ�pucRegBuffer�����λLSB��
//                         ����ص�����Ҫд�����������û���õ�����Ȧ�����粻��8��һ�����Ȧ״̬����Ӧ��λ����ֵ��������λ0��
//			usAddress    : ��һ����Ȧ��ַ��
//			usNCoils     : �������Ȧ����
//          eMode        ������ò���ΪeMBRegisterMode::MB_REG_WRITE���û���Ӧ����ֵ����pucRegBuffer�еõ����¡�
//                         ����ò���ΪeMBRegisterMode::MB_REG_READ���û���Ҫ����ǰ��Ӧ�����ݴ洢��pucRegBuffer��
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS   0��
//**********************************************************************************
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex , iRegBitIndex , iNReg;

	iNReg =  usNCoils / 8 + 1;        //ռ�üĴ�������

	if(( usAddress >= S_COIL_START ) &&
			(usAddress + usNCoils <= S_COIL_START + S_COIL_NCOILS))
	{
		iRegIndex    = (int)( usAddress - S_COIL_START ) / 8 ;    //ÿ���Ĵ�����8��
		iRegBitIndex = (int)( usAddress - usSCoilStart ) % 8 ;	  //����ڼĴ����ڲ���λ��ַ
		
		switch (eMode)
		{
			/* Pass current coil values to the protocol stack. */
			case MB_REG_READ:
					while(iNReg > 0)
					{
						*pucRegBuffer++ = xMBUtilGetBits(&ucSCoilBuf[iRegIndex++] , iRegBitIndex , 8);
						iNReg --;
					}
					pucRegBuffer --;
					usNCoils = usNCoils % 8;                         //���µ���Ȧ��	
					*pucRegBuffer = *pucRegBuffer << (8 - usNCoils); //��λ����
					*pucRegBuffer = *pucRegBuffer >> (8 - usNCoils);
					break;

			/* Update current coil values with new values from the protocol stack. */
			case MB_REG_WRITE:
					while(iNReg > 1)									 //���������������������
					{
						xMBUtilSetBits(&ucSCoilBuf[iRegIndex++] , iRegBitIndex  , 8 , *pucRegBuffer++);
						iNReg--;
					}
					usNCoils = usNCoils % 8;                            //���µ���Ȧ��
					if (usNCoils != 0)                                  //xMBUtilSetBits���� �ڲ���λ����Ϊ0ʱ����bug
					{
						xMBUtilSetBits(&ucSCoilBuf[iRegIndex++], iRegBitIndex, usNCoils,
								*pucRegBuffer++);
					}
					break;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
//****************************��ɢ����Ĵ����ص�����********************************
//��������: eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
//��    ������ɢ����Ĵ�����صĹ��ܣ�������������
//* ��    �ܣ���ȡ��ɢ�Ĵ�������Ӧ�������У�02 ����ɢ�Ĵ��� eMBFuncReadDiscreteInputs
//��ڲ�����pucRegBuffer : �õ�ǰ����Ȧ���ݸ�������Ĵ�������ʼ�Ĵ�����Ӧ��λ���ڸ��ֽ�pucRegBuffer�����λLSB��
//                         ����ص�����Ҫд�����������û���õ�����Ȧ�����粻��8��һ�����Ȧ״̬����Ӧ��λ����ֵ��������Ϊ0��
//			usAddress    : ��ɢ�������ʼ��ַ
//			usNDiscrete  : ��ɢ���������
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS      1��
//**********************************************************************************
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex , iRegBitIndex , iNReg;
	UCHAR *         pucDiscreteInputBuf;
  UCHAR           DISCRETE_INPUT_START;
  UCHAR           DISCRETE_INPUT_NDISCRETES;
  UCHAR           usDiscreteInputStart;
	iNReg =  usNDiscrete / 8 + 1;        //ռ�üĴ�������
    
	pucDiscreteInputBuf = ucSDiscInBuf;
	DISCRETE_INPUT_START = S_DISCRETE_INPUT_START;
	DISCRETE_INPUT_NDISCRETES = S_DISCRETE_INPUT_NDISCRETES;
	usDiscreteInputStart = usSDiscInStart;

	pucDiscreteInputBuf[0] = 0x07;
	
	if(( usAddress >= DISCRETE_INPUT_START)
			&& (usAddress + usNDiscrete <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES))
	{
		iRegIndex    = (int)( usAddress - usDiscreteInputStart ) / 8 ;    //ÿ���Ĵ�����8��
		iRegBitIndex = (int)( usAddress - usDiscreteInputStart ) % 8 ;	  //����ڼĴ����ڲ���λ��ַ

		while(iNReg > 0)
		{
			*pucRegBuffer++ = xMBUtilGetBits(&pucDiscreteInputBuf[iRegIndex++] , iRegBitIndex , 8);
			iNReg --;
		}
		pucRegBuffer --;
		usNDiscrete = usNDiscrete % 8;                      //���µ���Ȧ��
		*pucRegBuffer = *pucRegBuffer << (8 - usNDiscrete); //��λ����
		*pucRegBuffer = *pucRegBuffer >> (8 - usNDiscrete);
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	return eStatus;
}

//**********************************************************************************
//��������: void Step(void)
//��    ��:��դ���
//ucSCoilBuf[2]=0x0   (0        0            0         0)


//ͨ����ַ��          20       19            18
//* ��    �ܣ���ȡ��ɢ�Ĵ�������Ӧ�������У�02 ����ɢ�Ĵ��� eMBFuncReadDiscreteInputs
//��    ע��
//*******************************************************************************
union LianHeTi test_data27;
void Step(void)
{	
	//20206.17�޸�  02=0ʱ�Թ�դ��־λ������0������ֹ��դʱ�������� 
	if(!(ucSCoilBuf[0] & 0x02))
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		TIM_SetCounter(TIM3, 0);
		Timer3_cout = 0;
	}
	
	//20206.17�޸Ľ���
	//2021-3-19����դ�ź��߸�Ϊ�����ߣ����ź�Ϊ1ʱ��ʾ����״̬���ź�Ϊ0ʱ��ʾ��դʧЧ���դ��⵽�ϰ���
	if(ucSCoilBuf[5] & 0x01)
	{
		//2021-3-19����դ�ź��߸�Ϊ�����ߣ����ź�Ϊ1ʱ��ʾ����״̬���ź�Ϊ0ʱ��ʾ��դʧЧ���դ��⵽�ϰ���
		if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1))
////			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)
		{
//			GS_Flag=1;
			 ucSCoilBuf[8] &= 0xef;            //û���ϰ���
//			LED_PURPLE;
		}		
		else
		{
//			GS_Flag=0;	
			ucSCoilBuf[8] |= 0x10;            //���ϰ���
		}
		
		if((ucSCoilBuf[0] & 0x02))
		{		
			if(ucSCoilBuf[0] & 0x04)//�豸0_��д00003    �ж�ѭ������
			{
				//2021-3-19����դ�ź��߸�Ϊ�����ߣ����ź�Ϊ1ʱ��ʾ����״̬���ź�Ϊ0ʱ��ʾ��դʧЧ���դ��⵽�ϰ���
				if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == 0) && (ucSCoilBuf[5] & 0x01))//��⵽�ϰ���
				{ 
					ucSCoilBuf[8] |= 0x10;            //���ϰ���
					relay11_OFF;//���ȹرն���1
					ucSCoilBuf[4] &= 0xf7;//���ȹرն���1
					ucSCoilBuf[0] |= 0x01; 
					ucSCoilBuf[0] &= 0xf9; 
					ucSCoilBuf[7] &= 0xfe;            //����׼��״̬����
//					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//					TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
//					TIM_SetCounter(TIM3, 0);
//					Timer3_cout=0;
//					(void)eMBPoll();
				}
			}
			else
			{
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
				//printf("Timer3_cout:::%f \n",Timer3_cout);
				TIM_Cmd(TIM3, ENABLE);
				test_data27.value[0] = usSRegHoldBuf[34];
				test_data27.value[1] = (usSRegHoldBuf[34] >> 8);
				test_data27.value[2] = usSRegHoldBuf[33];
				test_data27.value[3] = (usSRegHoldBuf[33] >> 8);
				test_data27.d = test_data27.d * 10;
				
				if((Timer3_cout <= test_data27.d) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == 0)
					&& (ucSCoilBuf[5] & 0x01))//��⵽�ϰ���
				{
					ucSCoilBuf[8] |= 0x10;            //���ϰ���
					relay11_OFF;//���ȹرն���1
					ucSCoilBuf[4] &= 0xf7;//���ȹرն���1
					ucSCoilBuf[0] |= 0x01; 
					ucSCoilBuf[0] &= 0xf9; 
					ucSCoilBuf[7] &= 0xfe;            //����׼��״̬����
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
					TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
					TIM_SetCounter(TIM3, 0);
					Timer3_cout = 0;				
				}
				
				if(Timer3_cout > test_data27.d)
				{
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
					TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
					TIM_SetCounter(TIM3, 0);
				}
			}
		}
	}
	else
	{
		ucSCoilBuf[8] &= 0xef;            //û���ϰ���
	}
}

//**********************************************************************************
//��������: void Press_Set(void)
//��    �����ǲ��Թ��̵̼�������
//* ��    �ܣ�
//��    ע��Editor��
//**********************************************************************************
void  Relay_control(void)
{
//	if(!(ucSCoilBuf[0]&0x20))
//	{
	
	if((ucSCoilBuf[3] & 0x01))
	{
		relay1_ON;
	}
	else 
	{
		relay1_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x02))
	{
		relay2_ON;
	}
	else 
	{
		relay2_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x04))
	{
		relay3_ON;
	}
	else 
	{
		relay3_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x08))
	{
		relay4_ON;
	}
	else 
	{
		relay4_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x10))
	{
		relay5_ON;
	}
	else 
	{
		relay5_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x20))
	{
		relay6_ON;
	}
	else 
	{
		relay6_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x40))
	{
		relay7_ON;
	}
	else 
	{
		relay7_OFF;
	}
	
	if((ucSCoilBuf[3] & 0x80))
	{
		relay8_ON;
	}
	else 
	{
		relay8_OFF;
	}
		
	if((ucSCoilBuf[4] & 0x08))
	{
		relay11_ON;
	}
	else 
	{
		relay11_OFF;
	}
	
	if((ucSCoilBuf[4] & 0x10))
	{
		relay12_ON;
	}
	else 
	{
		relay12_OFF;
	}
	
	if((ucSCoilBuf[4] & 0x20))
	{
		relay13_ON;
	}
	else 
	{
		relay13_OFF;
	}
	
	if(ucSCoilBuf[4] & 0x40)
	{
		relay14_ON;
	}
	else 
	{
		relay14_OFF;
	}
	
	if(ucSCoilBuf[4] & 0x80)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM5, ENABLE);
		relay15_ON;
	}
	else 
	{
		relay15_OFF;
	}
	
	if(ucSCoilBuf[5] & 0x01)
	{
		relay16_ON;
	}
	else 
	{
		relay16_OFF;
	}
}
//**********************************************************************************
//��������: void AUTO_ZERO(void)
//��    �����Զ�����
//* ��    �ܣ�
//��    ע��Editor��
//**********************************************************************************
uint16_t time=0;  //�Զ�����ɨ�����
void AUTO_ZERO(void)
{
	if(ucSCoilBuf[8] & 0x01)
	{
		time++;
		
		if(time > 2)
		{
			time = 0;
			Setzero_Init();
			time = 0;
			ucSCoilBuf[8] &= 0xf0;
		}
	}
}
