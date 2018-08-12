#include "headfile.h"

volatile unsigned char sbus_rx_buffer[25];

 
/****************************
��������USART1_Gpio_Config
��ڲ�������
���ڲ�������
���ܣ���ʼ������1GPIO 
˵���� PB7---USART1_RX
******************************/
void USART1_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	                                                                                                                                                                                                                                                                                                         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7 ,GPIO_AF_USART1);
}	
/****************************
��������USART1_Config
��ڲ�������
���ڲ�������
���ܣ���ʼ������1����
˵���������ʣ�100K
			8λ����λ
			1��ֹͣλ
      żУ��
******************************/
void USART1_Config(void)
{
	USART_InitTypeDef USART_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = 100000;	//SBUS 100K baudrate
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);											
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	
}
/****************************
��������DMA2NVIC_Config
��ڲ�������
���ڲ�������
���ܣ�����DMA2������5��ӦUSART1_RX�ж�
      ��ռ���ȼ���0
			��Ӧ���ȼ���3
******************************/
void DMA2NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/****************************
��������DMA2_Config
��ڲ�������
���ڲ�������
���ܣ�����DMA2������5��ͨ���ģ�USART1_TX��
˵�������ݳ��ȣ�8
			�������赽�ڴ�  
******************************/
void DMA2_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 , ENABLE);
	
	DMA_Cmd(DMA2_Stream5, DISABLE);						
  while (DMA2_Stream5->CR & DMA_SxCR_EN);
	
	DMA_DeInit(DMA2_Stream5);	
	DMA_InitStructure.DMA_Channel= DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;			
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;									
	DMA_InitStructure.DMA_BufferSize = 18;																
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;												
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;							
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5,&DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
  DMA_Cmd(DMA2_Stream5,ENABLE);
}
/****************************
��������DbusConfig
��ڲ�������
���ڲ�������
���ܣ����ô���1����DT7ң��������
******************************/
void DbusConfig(void)
{
	USART1_Gpio_Config();
	USART1_Config();
	DMA2NVIC_Config();
	DMA2_Config();
}

