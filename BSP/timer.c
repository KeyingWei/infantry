#include "headfile.h"

/******************************
��������TIM2_Config
��ڲ�������
���ڲ�������
���ܣ�����TIM2������Ԥ��Ƶֵ����װ��ֵ
��ʱʱ��time=(90/psc)*arr )us=4294
*******************************/
void TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    tim.TIM_Period = 0xFFFFFFFF;
    tim.TIM_Prescaler = 90 - 1;	          //1M ��ʱ��  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	//����Ƶ
    tim.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
    TIM_ARRPreloadConfig(TIM2, ENABLE);	//ʹ����װ��
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_Cmd(TIM2,ENABLE);	
}
/******************************
��������TIM3_Config
��ڲ�������
���ڲ�������
���ܣ�����TIM2������Ԥ��Ƶֵ����װ��ֵ
��ʱʱ��time=5ms
*******************************/
void TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 5000-1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 90-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);  
}

/******************************
��������TIM4_Config
��ڲ�������
���ڲ�������
���ܣ�����TIM2������Ԥ��Ƶֵ����װ��ֵ
��ʱʱ��time=(90/psc)*arr )us=1ms
*******************************/
void TIM4_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 1000-1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 90-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);  
}


/********************************************
��������TIMExNVIC_Config
��ڲ�������
���ڲ�������
���ܣ�����TIM3,4�жϣ�������Ӧ���ȼ�����ռ���ȼ�
˵��������ԽС���ȼ�Խ�ߣ���ռ���ȼ��ߵĿɴ����
�ȼ��͵��жϣ���ռ���ȼ���ͬʱ���жϲ��ɴ�ϣ�ͬ
ʱ�����ж�ʱ����Ӧ���ȼ��ߵĿ����Ƚ����ж�
**********************************************/
void TIMExNVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�������ȼ����2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //���ö�ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //������ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //������Ӧ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;   //���ö�ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //������ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //������Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}
/********************************************
��������TimConfig
��ڲ�������
���ڲ�������
���ܣ�����TIM2,TIM3,TIM4
**********************************************/
void TimConfig(void)
{
	TIM2_Config();
	TIM3_Config();
	TIM4_Config();
	TIMExNVIC_Config();
}

/********************************************
��������TIM2_IRQHandler
��ڲ�������
���ڲ�������
���ܣ� Ԥ��
**********************************************/
void TIM2_IRQHandler(void)
{
	  if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
		{
			  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);
			  //BOTH_LED_TOGGLE();
		}
} 

uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}





/********************************************
��������TIM3_IRQHandler
��ڲ�������
���ڲ�������
���ܣ�Ԥ��
**********************************************/
void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET) //����ж�
	{
      void TerminalCommand_receive_AUTO();
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //����жϱ�־λ
}
 
void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET) //����ж�
	{
	
		GimbalMotorOutput();
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //����жϱ�־λ
}




