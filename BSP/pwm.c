#include "headfile.h"

/******************************
��������TIM1_PWM_Config
��ڲ�������
���ڲ�������
���ܣ����ö�ʱ��1CH1,CH2,CH3,CH4,������·ռ�ձȿɵ���PWM��
��Ƶϵ����19000
PWM���ڣ�20ms
��ʼռ�ձȣ�0
*******************************/
void TIM1_PWM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;           //GPIOA8,A9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      
	GPIO_Init(GPIOA,&GPIO_InitStructure);           
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;           //GPIOE13,E14
	GPIO_Init(GPIOE,&GPIO_InitStructure);             
	
	TIM_TimeBaseStructure.TIM_Prescaler = 18000-1; //100us
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 200-1;  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);//PA8
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);//PA9
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);//PE13
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);//PE14
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}


/******************************
��������TIM12_PWM_Config
��ڲ�������
���ڲ�������
���ܣ����ö�ʱ��12_CH1,CH2,������·ռ�ձȿɵ���PWM��
��Ƶϵ����19000
PWM���ڣ�21.1ms
��ʼռ�ձȣ�0
*******************************/
void TIM12_PWM_Config(void)             
{		 					 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); 	

	
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource6, GPIO_AF_TIM12);
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource9, GPIO_AF_TIM12); 

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;           //GPIOH6.H9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      
	GPIO_Init(GPIOH,&GPIO_InitStructure);            
	  
		
	TIM_TimeBaseStructure.TIM_Prescaler = 900-1;  //10us
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period = 2110;  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);  //PH6
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);  //PH9

	
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable); 
  TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);  

	
  TIM_ARRPreloadConfig(TIM12,ENABLE);
	
	TIM_Cmd(TIM12, ENABLE);  								  
}  

/******************************
��������PwmConfig
��ڲ�������
���ڲ�������
���ܣ�TIM1,TIM12����6·PWM��
*******************************/
void PwmConfig(void)
{
	TIM1_PWM_Config();
	TIM12_PWM_Config();
}

