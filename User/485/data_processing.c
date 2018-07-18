#include "modbus.h"
#include "data_processing.h" 
#include "mb.h"
#include "bsp_led.h"  
#include "bsp_SysTick.h"
#include "relay.h"
const 	int32_t chushizhi[4]={3890,0,0,0};  //mpm270 大气压ad值

int8_t quyanggeshu=0;  //统计取样个数
int64_t channel[4][cunchugeshu]; //将读取的部分ad值存储在这个数组中
int32_t pianyiliang[4]={0};   //调零偏移量
union LianHeTi test_data1;
union LianHeTi test_data20;
union LianHeTi test_data21;
union LianHeTi test_data22;
union LianHeTi test_data23;
union LianHeTi test_data24;
union LianHeTi test_data25;
union LianHeTi test_data26;

int Adc[4]={0};
int biaozun_AD;
int AD_10;
int AD_100;
float yalizhi_10;
float yalizhi_100;
float liangcheng_1001;
int biaozun_AD1=3890;
float liangcheng_101=0.1162655;
float fenzixishu1=-0.00000000030936;
long double lvbo(int64_t weilvadzhi,uint8_t channels)
{
	  int8_t j;
	  int64_t adzhi1;
    long double result;	
	
	  adzhi1=weilvadzhi;
	
//   result=0.995*result+(1-0.995)*adzhi1;  //一阶低通滤波 滤波系数0.15
	 
	   if(quyanggeshu<cunchugeshu)
		 {
			 for(j=quyanggeshu;j<cunchugeshu;j++)
			 {
		   channel[channels][j]=adzhi1;
			 }
			 quyanggeshu++;
		 }
		 else
		 {
		    for(j=0;j<cunchugeshu-1;j++)  //存储数组a向前移动一位
			  channel[channels][j]=channel[channels][j+1];
			  
			  channel[channels][j]=adzhi1;
		 }
		 
		 result=channel[channels][11]*0.16+channel[channels][10]*0.1536+channel[channels][9]*0.1420+channel[channels][8]*0.1262+channel[channels][7]*0.1078+channel[channels][6]*0.0886+channel[channels][5]*0.0696+channel[channels][4]*0.0534+channel[channels][3]*0.0392+channel[channels][2]*0.0276+channel[channels][1]*0.0192+channel[channels][0]*0.0128;  //高斯滤波
	
//		 printf(" %d a[0]=%d %d %d %d %d %d ",quyanggeshu,a[0],a[1],a[2],a[3],a[4],a[5]);
		 return result;
}

void Setzero_Init(void)
{
	  long ulResult;
	  long ulSum=0;
	  int i;
//	  pianyiliang[0]=-2371;
    
//			if(ucSCoilBuf[0]&0x40)
//		{
//			relay5_ON;
//			relay6_ON;
			for(i=0;i<10;i++)
	    {
			  ulResult = ADS1256ReadData( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AIN1);	
			  printf(" %ld 校零操作成功！\n",ulResult);
        if(i>=9)				
				ulSum = ulSum + ulResult ;
				Delays1(0xffff);			
		  }
			ulResult =ulSum /1;
			pianyiliang[0]=ulResult-biaozun_AD1;
			printf("%d %ld \n",pianyiliang[0],ulResult);
//			relay6_OFF;

	    Delays1(0xffff);
//			relay5_OFF;
//		  ucSCoilBuf[0]&=0xBF;

//		}
}
//**********************************************************************************
//函数定义: void Setzero_Init(void)	
//描    述：MPM270程序启动初始调零

//备    注：
//**********************************************************************************

//**********************************************************************************
//函数定义: void Setzero_Init(void)	
//描    述：手动按键调零

//备    注：
//**********************************************************************************
void set_zero(void)
{
	  long ulResult;
	  long ulSum=0;
	  int i;
//	  pianyiliang[0]=-2371;
    
			if(ucSCoilBuf[2]&0x80)
		{
			for(i=0;i<10;i++)
	    {
			  ulResult = ADS1256ReadData( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AIN1);	
			  printf(" %ld 校零操作成功！\n",ulResult);
        if(i>=9)				
				ulSum = ulSum + ulResult ;
				Delays1(0xffff);			
		  }
			ulResult =ulSum /1;
			//			pianyiliang[0]=ulResult-biaozun_AD1;   6月12日16:09
			pianyiliang[0]=ulResult-biaozun_AD;
			printf("pianyiliang %d %ld \n",pianyiliang[0],ulResult);
	
	    Delays1(0xffff);
			LED_BLUE;
		  ucSCoilBuf[2]&=0x7f;  //关闭手动调零

		}
}
void caijitongdao(char channel_0,char channel_1,char channel_2,char channel_3)  //使能需要的交叉采集通道 
{
		
		float Pressure;
	  char first=0;  
	  char last=0;  //统计使能通道个数

	if(channel_0==1)
	  last++;
	if(channel_1==1) 
		last++;
	if(channel_2==1) 
		last++;
			if(ucSCoilBuf[5]&0x20)
		{		
////			LED_CYAN;
////			test_data21.value[0]=usSRegHoldBuf[21];
////			test_data21.value[1]=(usSRegHoldBuf[21]>>8);
////			test_data21.value[2]=usSRegHoldBuf[20];
////			test_data21.value[3]=(usSRegHoldBuf[20]>>8);
////			biaozun_AD=test_data21.d;//+pianyiliang ;
////			printf("biaozun_AD:::%d  \n",biaozun_AD);
//		
			test_data21.value[0]=usSRegHoldBuf[31];
			test_data21.value[1]=(usSRegHoldBuf[31]>>8);
			test_data21.value[2]=usSRegHoldBuf[30];
			test_data21.value[3]=(usSRegHoldBuf[30]>>8);
			biaozun_AD=test_data21.d;//+pianyiliang ;
//			printf("biaozun_AD:::%d  \n",biaozun_AD);
//			yalizhi_10=test_data26.d ;
//			printf("yalizhi_10:::%f \n",yalizhi_10);
//		
			test_data23.value[0]=usSRegHoldBuf[25];
			test_data23.value[1]=(usSRegHoldBuf[25]>>8);
			test_data23.value[2]=usSRegHoldBuf[24];
			test_data23.value[3]=(usSRegHoldBuf[24]>>8);
			yalizhi_100=test_data23.d*1000 ;
			yalizhi_10=yalizhi_100*0.1;
////			yalizhi_10=10000;
//			printf("yalizhi_100:::%f \n",yalizhi_100);
//			printf("yalizhi_10:::%f \n",yalizhi_10);
//			
			test_data24.value[0]=usSRegHoldBuf[27];
			test_data24.value[1]=(usSRegHoldBuf[27]>>8);
			test_data24.value[2]=usSRegHoldBuf[26];
			test_data24.value[3]=(usSRegHoldBuf[26]>>8);
			AD_10=test_data24.d ;//+pianyiliang;
//			printf("AD_10:::%d \n",AD_10);
//			
			test_data25.value[0]=usSRegHoldBuf[29];
			test_data25.value[1]=(usSRegHoldBuf[29]>>8);
			test_data25.value[2]=usSRegHoldBuf[28];
			test_data25.value[3]=(usSRegHoldBuf[28]>>8);
			AD_100=test_data25.d ;//+pianyiliang;
//			printf("AD_100:::%d \n",AD_100);
//			
//					
			biaozun_AD1=biaozun_AD;
			liangcheng_101=yalizhi_10/(AD_10-biaozun_AD);
			liangcheng_1001=yalizhi_100/(AD_100-biaozun_AD);
			fenzixishu1=(liangcheng_1001-liangcheng_101)/(AD_100-AD_10);
//			printf("biaozun_AD1:::%d \n",biaozun_AD1);
//			printf("liangcheng_101:::%.20f \n",liangcheng_101);
//			printf("fenzixishu1:::%.20f \n",fenzixishu1);
//			
//			printf("*********************************************************** \n");
		}
		
		
//			Adc[last-1] = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 差分方式*/
//			Adc[first]=lvbo(Adc[first],0)-pianyiliang[0];
      

		printf("  0 channel:%d %.3f\n",Adc[first],Pressure);
		
	if(channel_3==1) 
	  last++;
	
  if(channel_0==1)   //如果通道0使能
	{
		/*差分采集方式*/
			Adc[last-1] = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 差分方式*/
			Adc[first]=lvbo(Adc[first],0)-pianyiliang[0];
		
		/*6.12 16:14*/
		 Pressure=(Adc[first]-biaozun_AD1)*(liangcheng_101+(Adc[first]-AD_10)*fenzixishu1)/1000;    //mpm270 0-100KPa  4X
	
//		Pressure=(Adc[first]+2503)*(0.686087667+(Adc[first]-48919)*0.00000000355789772)/1000;  //mpm270 0-350Kpa 4倍
//		Pressure=(Adc[first]+2371)*(1.58339113-(Adc[first]-42328)*0.0000000356142249)/1000;  //mpm270 0-600Kpa  8倍
		
		
		  test_data20.d =test_data1.d =Adc[first];
					usSRegInBuf[18]=test_data20.value[3];
					usSRegInBuf[18]=(usSRegInBuf[18]<<8)+test_data20.value[2];
					usSRegInBuf[19]=test_data20.value[1];
					usSRegInBuf[19]=(usSRegInBuf[19]<<8)+test_data20.value[0];
		
					test_data1.d =Pressure;   
					usSRegInBuf[0]=test_data1.value[3];
					usSRegInBuf[0]=(usSRegInBuf[0]<<8)+test_data1.value[2];
					usSRegInBuf[1]=test_data1.value[1];
					usSRegInBuf[1]=(usSRegInBuf[1]<<8)+test_data1.value[0];
		
		
//		printf("pianyiliang %d  \n",pianyiliang[0]);
	}
	if(channel_1==1)
	{
	  Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN2|ADS1256_MUXN_AIN3);
		first++;
		Adc[first]=lvbo(Adc[first],1)-pianyiliang[1];
    Pressure=(Adc[first]-3890)*(0.1162655+(Adc[first]-89900)*(-0.00000000030936))/1000;
	  printf("  1 channel:%d %.3f\n",Adc[first],Pressure);
		
	}
	if(channel_2==1)
	{
	   Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN4|ADS1256_MUXN_AIN5);
		 first++;
		 Adc[first]=lvbo(Adc[first],2)-pianyiliang[2];
//		 Pressure=(Adc-3890)*(0.1162655+(Adc-89900)*(-0.00000000030936))/1000;
		 Pressure = Adc[first]*0.000000598;
		 printf("  2 channel:%d %.3f",Adc[first],Pressure);
		
	}
	if(channel_3==1)
	{
	   Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN6|ADS1256_MUXN_AIN7);
		 first++;
		 Adc[first]=lvbo(Adc[first],3)-pianyiliang[3];
     Pressure=(Adc[first]-3890)*(0.1162655+(Adc[first]-89900)*(-0.00000000030936))/1000;
	   printf("  3 channel:%d %.3f",Adc[first],Pressure);
		 
	}
}

void caijitongdao1(char channel_0,char channel_1,char channel_2,char channel_3)  //使能需要的交叉采集通道 
{
	float Pressure;
	char first=0;
	
	if(channel_0==1)   //如果通道0使能
	{
		Delay_ms(1);
		/*差分采集方式*/
			Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 差分方式*/
		Delay_us(5);
		  Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1);
			Adc[first]=lvbo(Adc[first],0)-pianyiliang[0];
      Pressure=(Adc[first]-3890)*(0.1162655+(Adc[first]-89900)*(-0.00000000030936))/1000;
	
		printf("  0 channel:%d %.3f",Adc[first],Pressure);
		
	}
	if(channel_1==1)
	{
		Delay_ms(1);
		first++;
	  Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN2|ADS1256_MUXN_AIN3);
		Delay_us(5);
	  Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN2|ADS1256_MUXN_AIN3);
		Adc[first]=lvbo(Adc[first],1)-pianyiliang[1];
    Pressure=(Adc[first]-3890)*(0.1162655+(Adc[first]-89900)*(-0.00000000030936))/1000;
	  printf("  1 channel:%d %.3f",Adc[first],Pressure);
		
	}
	if(channel_2==1)
	{
		 Delay_ms(1);
		 first++;
	   Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN4|ADS1256_MUXN_AIN5);
		 Delay_us(5);
	   Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN4|ADS1256_MUXN_AIN5);
		 Adc[first]=lvbo(Adc[first],2)-pianyiliang[2];
//		 Pressure=(Adc-3890)*(0.1162655+(Adc-89900)*(-0.00000000030936))/1000;
		 Pressure = Adc[first]*0.000000598;
		 printf("  2 channel:%d %.3f",Adc[first],Pressure);
		
	}
	if(channel_3==1)
	{
		 Delay_ms(1);
		 first++;
	   Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN6|ADS1256_MUXN_AIN7);
		 Delay_us(5);
	   Adc[first] = ADS1256ReadData( ADS1256_MUXP_AIN6|ADS1256_MUXN_AIN7);
		 Adc[first]=lvbo(Adc[first],3)-pianyiliang[3];
//     Pressure=(Adc[first]-3890)*(0.1162655+(Adc[first]-89900)*(-0.00000000030936))/1000;
		 Pressure = Adc[first]*0.000000598;
	   printf("  3 channel:%d %.3f",Adc[first],Pressure);
		 
	}
}


void Delays1(uint32_t x)
{
	uint32_t nCount;
  for(nCount=x; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/
