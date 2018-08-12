#include "headfile.h"
//VP230---CAN_TX---PA12(CANTX) 
//VP230---CAN_RX---PA11(CANRX) 

/*------------------------------------------------------------------------------
                             CAN1���յ������
���Ե�ƽ��Ӧ�߼�0��CAN_HΪ3.5v,CAN_LΪ1.5v,ѹ��2v��
���Ե�ƽ��Ӧ�߼�1��CAN_H��CAN_L��Ϊ2.5v,ѹ��0v
STM32F4��28��ɸѡ���飬һ��ɸѡ������2��32λ�ļĴ�����1��32�ļĴ����ɲ��Ϊ2��16�ļĴ���
ɸѡ��ģʽ���趨Ϊλ����ģʽ���б�ģʽ������λģʽ��Ϊ1��32λ������ID��һ��32λ�����룬��
��2��16λ������ID������16λ�����룬ɸѡ��ֻ��������Ϊ1��λ��
�б�ģʽ���趨2��32λ������ID��4��16λ������ID��Ҫ�������ȫƥ��

------------------------------------------------------------------------------*/
/******************************
��������CAN1_Gpio_Config
��ڲ�������
���ڲ�������
���ܣ���ʼ��CAN1_GPIO
˵����PD1:CAN1_TX
      PD0:CAN1_RX
*******************************/
static void CAN1_Gpio_Config(void)
{
	GPIO_InitTypeDef       gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//����GPIODʱ��
	
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  gpio.GPIO_Mode = GPIO_Mode_AF;		       //�������ù���										
  gpio.GPIO_Speed = GPIO_Speed_100MHz; //����IO�ٶ�
	GPIO_Init(GPIOD, &gpio);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);//PDOӳ��ΪCAN1_L
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);//PD0ӳ��ΪCAN1_H
}
/******************************
��������CANxNVIC_Config
��ڲ�������
���ڲ�������
���ܣ�����CAN1�Ľ����ж�
*******************************/
static void CANxNVIC_Config(void)
{
	NVIC_InitTypeDef       nvic;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����2
	
	nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;					//����CAN1�����ж�
  nvic.NVIC_IRQChannelPreemptionPriority = 1;   //��Ӧ���ȼ�1
  nvic.NVIC_IRQChannelSubPriority = 1;          //��ռ���ȼ�2
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
	
  nvic.NVIC_IRQChannel = CAN1_TX_IRQn;					//����CAN1�����ж�
  nvic.NVIC_IRQChannelPreemptionPriority = 0;   //��Ӧ���ȼ�1
  nvic.NVIC_IRQChannelSubPriority = 1;          //��ռ���ȼ�2
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
}
/******************************
��������CAN1_Config
��ڲ�������
���ڲ�������
���ܣ�����CAN1��λʱ������ͨ�Ų����ʺ�CAN1��ɸѡ��
�����ʼ��� baud(Mbps) =��45/(CAN_SJW_1tq+CAN_BS2_6tq+CAN_BS1_8tq)��/CAN_Prescaler 
*******************************/
static void CAN1_Config(void)
{
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	CAN_DeInit(CAN1);
  CAN_StructInit(&can);
  /************CAN���ߵ�����*******************/
  can.CAN_TTCM = DISABLE;			//��ʱ�䴥��ͨ��ģʽ
  can.CAN_ABOM = DISABLE;			//����Զ����߹���ģʽ
  can.CAN_AWUM = DISABLE;			//�Զ�����ģʽ��˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  can.CAN_NART = DISABLE;			//���Զ��ش���ģʽ����ֹ�����Զ����� 
  can.CAN_RFLM = DISABLE;			//����FIFO����ģʽ�����Ĳ�����,�µĸ��Ǿɵ� 
  can.CAN_TXFP = ENABLE;			//����FIFO���ȼ������ȼ��ɱ��ı�ʶ������ 
  can.CAN_Mode = CAN_Mode_Normal;		//ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ;
  can.CAN_SJW  = CAN_SJW_1tq; 
  can.CAN_BS1 = CAN_BS2_6tq;
  can.CAN_BS2 = CAN_BS1_8tq;
  can.CAN_Prescaler = 3;   //CAN BaudRate 45/(1+6+8)/3=1Mbps
  CAN_Init(CAN1, &can);
	
	can_filter.CAN_FilterNumber=0;//ѡ��ɸѡ����0��F4����28��ɸѡ����
  can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
  can_filter.CAN_FilterScale=CAN_FilterScale_32bit;//32λ����ģʽ��ɸѡ����R1�Ĵ����������������ID,R2������룬ɸѡ��ֻ��������Ϊ1��λ
  can_filter.CAN_FilterIdHigh=0x0000;
  can_filter.CAN_FilterIdLow=0x0000;
  can_filter.CAN_FilterMaskIdHigh=0x0000;                      //ѡ��������е�ID
  can_filter.CAN_FilterMaskIdLow=0x0000;
  can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
  can_filter.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&can_filter);			//�˲�����ʼ��
    
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);		//�����ж�
  CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 		//�����ж�
}
/*****************************************
��������CAN1Config
��ڲ�������
���ڲ�������
���ܣ�����CAN1��GPIO,NVIC,ɸѡ����λʱ��
******************************************/
void CAN1Config(void)
{
	CAN1_Gpio_Config();
	CANxNVIC_Config();
	CAN1_Config();
}


