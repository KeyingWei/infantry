#include "headfile.h"

/****************************
��������BeepConfig
��ڲ�������
���ڲ�������
���ܣ���ʼ��������
˵����PB4--BEEP
			�ߵ�ƽ����Ч
******************************/
void BeepConfig(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PB4                                                                                                                                                                                                                                                                                                          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}