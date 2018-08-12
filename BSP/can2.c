#include "headfile.h"
/*----CAN2_TX-----PB13----*/
/*----CAN2_RX-----PB12----*/

/******************************
��������CAN2_Gpio_Config
��ڲ�������
���ڲ�������
���ܣ���ʼ��CAN2_GPIO
˵����PB13:CAN2_TX
      PB12:CAN2_RX
*******************************/
static void CAN2_Gpio_Config(void)
{
	GPIO_InitTypeDef       gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
  gpio.GPIO_Mode = GPIO_Mode_AF;//��ȥ���ù���
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpio);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);//PB12����ΪCAN2_RX
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); //PB13����ΪCAN2_TX
	
}

/******************************
��������CAN2NVIC_Config
��ڲ�������
���ڲ�������
���ܣ�����CAN2�Ľ����ж�
*******************************/
static void CAN2NVIC_Config()
{
	NVIC_InitTypeDef  nvic;
	
	nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
	
	nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
}

/******************************
��������CAN2_Config
��ڲ�������
���ڲ�������
���ܣ�����CAN1��λʱ������ͨ�Ų����ʺ�CAN1��ɸѡ��
�����ʼ��� baud(Mbps) =��45/(CAN_SJW_1tq+CAN_BS2_6tq+CAN_BS1_8tq)��/CAN_Prescaler 
*******************************/
static void CAN2_Config(void)
{
	CAN_InitTypeDef        can;
  CAN_FilterInitTypeDef  can_filter;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	CAN_DeInit(CAN2);
  CAN_StructInit(&can);

  can.CAN_TTCM = DISABLE;
  can.CAN_ABOM = DISABLE;    
  can.CAN_AWUM = DISABLE;    
  can.CAN_NART = DISABLE;    
  can.CAN_RFLM = DISABLE;    
  can.CAN_TXFP = ENABLE;     
  can.CAN_Mode = CAN_Mode_Normal; 
  can.CAN_SJW  = CAN_SJW_1tq;
  can.CAN_BS1 = CAN_BS2_6tq;
  can.CAN_BS2 = CAN_BS1_8tq;
  can.CAN_Prescaler = 3;   //CAN BaudRate 45/(1+8+6)/3=1Mbps
  CAN_Init(CAN2, &can);
    
  can_filter.CAN_FilterNumber=14;		//CAN2ѡ��ɸѡ����14
  can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
  can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
  can_filter.CAN_FilterIdHigh=0x0000;
  can_filter.CAN_FilterIdLow=0x0000;
  can_filter.CAN_FilterMaskIdHigh=0x0000;  //��������ID
  can_filter.CAN_FilterMaskIdLow=0x0000;
  can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo1
  can_filter.CAN_FilterActivation=ENABLE;//����ɸѡ��
  CAN_FilterInit(&can_filter);
    
  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);		//�����ж�
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE); 		//�����ж�
}

/*****************************************
��������CAN2Config
��ڲ�������
���ڲ�������
���ܣ�����CAN2��GPIO,NVIC,ɸѡ����λʱ��
******************************************/
void CAN2Config(void)
{
	CAN2_Gpio_Config();
	CAN2NVIC_Config();
	CAN2_Config();
}




