#include "timer.h"

//u8 TIM1CH1_CAPTURE_STA = 0;	//ͨ��1���벶���־������λ�������־����6λ�������־		
//u16 TIM1CH1_CAPTURE_UPVAL;
//u16 TIM1CH1_CAPTURE_DOWNVAL;

//u8 TIM1CH2_CAPTURE_STA = 0;	//ͨ��2���벶���־������λ�������־����6λ�������־		
//u16 TIM1CH2_CAPTURE_UPVAL;
//u16 TIM1CH2_CAPTURE_DOWNVAL;

//u8 TIM1CH3_CAPTURE_STA = 0;	//ͨ��3���벶���־������λ�������־����6λ�������־		
//u16 TIM1CH3_CAPTURE_UPVAL;
//u16 TIM1CH3_CAPTURE_DOWNVAL;

//u8 TIM1CH4_CAPTURE_STA = 0;	//ͨ��4���벶���־������λ�������־����6λ�������־		
//u16 TIM1CH4_CAPTURE_UPVAL;
//u16 TIM1CH4_CAPTURE_DOWNVAL;

//u32 TIM1_T1;
//u32 TIM1_T2;
//u32 TIM1_T3;
//u32 TIM1_T4;
////��ʱ��2���벶������
//int pwmout1, pwmout2, pwmout3, pwmout4; 				//���ռ�ձ�
///**************************************************************************
//�������ܣ���ģң�س�ʼ������
//��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
//����  ֵ����
//**************************************************************************/
//void TIM1_Cap_Init(u16 arr, u16 psc)
//{
////	GPIO_InitTypeDef GPIO_InitStructure1;
////	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
////	NVIC_InitTypeDef NVIC_InitStructure;
////  TIM_ICInitTypeDef TIM_ICInitStructure;
////	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ��TIM1ʱ��
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOA ʱ��

////	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 ; 
////	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD; //���� 
////	GPIO_InitStructure1.GPIO_Speed=GPIO_Speed_2MHz;
////	GPIO_Init(GPIOA, &GPIO_InitStructure1);
////	GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 | GPIO_Pin_11);//PA8 9 11  ����   ����
//// 	
////	//��ʼ����ʱ�� TIM1	 
////	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
////	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ԥ��Ƶ�� 
////	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
////	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
////	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

////	//��ʼ��TIM3���벶����� ͨ��1
////	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� 
////	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
////	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //
////	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
////	TIM_ICInitStructure.TIM_ICFilter = 0x0F;	  //IC1F=0000 ���������˲��� ���˲�
////	TIM_ICInit(TIM1, &TIM_ICInitStructure);

////	//��ʼ��TIM3���벶����� ͨ��2
////	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ�������  
////	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
////	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //
////	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
////	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
////	TIM_ICInit(TIM1, &TIM_ICInitStructure);

////	//��ʼ��TIM3���벶����� ͨ��3
////	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ�������  
////	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
////	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //
////	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
////	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
////	TIM_ICInit(TIM1, &TIM_ICInitStructure);

////	//��ʼ��TIM3���벶����� ͨ��4
////	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� 
////	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
////	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //
////	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
////	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
////	TIM_ICInit(TIM1, &TIM_ICInitStructure);

////	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
////	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

////	//�жϷ����ʼ��
////	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM3�ж�
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
////	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

////	TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,	ENABLE);   //����������жϣ�����CC1IE,CC2IE,CC3IE,CC4IE�����ж�	
////	TIM_CtrlPWMOutputs(TIM1,ENABLE); //�߼���ʱ���������ʹ�����		
////	TIM_Cmd(TIM1, ENABLE); 		//ʹ�ܶ�ʱ��

//}

///**************************************************************************
//�������ܣ���ģң�ؽ����ж�
//��ڲ�������
//����  ֵ����
//**************************************************************************/
////void TIM1_CC_IRQHandler(void)
////{
////   	u16 tsr;
////		tsr=TIM1->SR;
////	if(Remote_ON_Flag==0) CAN_ON_Flag=0,Usart_ON_Flag3=0,PS2_ON_Flag=0,Remote_ON_Flag=1;
////	if ((TIM1CH1_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
////	{ 
////			     if(tsr&0x02)//����1���������¼�		
////				{       TIM1->SR&=~(1<<1);//����жϱ�־λ 	 
////								if (TIM1CH1_CAPTURE_STA & 0X40)		//����һ���½���
////								{
////										
////									TIM1CH1_CAPTURE_DOWNVAL = TIM1->CCR1;//��¼�´�ʱ�Ķ�ʱ������ֵ
////									if (TIM1CH1_CAPTURE_DOWNVAL < TIM1CH1_CAPTURE_UPVAL)
////									{
////										TIM1_T1 = 65535;
////									}
////									else
////									TIM1_T1 = 0;
////									Remoter_Ch1 = TIM1CH1_CAPTURE_DOWNVAL - TIM1CH1_CAPTURE_UPVAL
////									+ TIM1_T1;		//�õ��ܵĸߵ�ƽ��ʱ��
////									if(abs(Remoter_Ch1-L_Remoter_Ch1)>500)Remoter_Ch1=L_Remoter_Ch1;
////									L_Remoter_Ch1=Remoter_Ch1;
////									TIM1CH1_CAPTURE_STA = 0;		//�����־λ����
////									TIM1->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
////								}
////								else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
////								{ 
////									TIM1CH1_CAPTURE_UPVAL =TIM1->CCR1;		//��ȡ����������
////									TIM1CH1_CAPTURE_STA |= 0X40;		//����Ѳ���������
////									TIM1->CCER|=1<<1;			//CC1P=0 ����Ϊ�½��ز���
////								}
////				}
////	}
////	if ((TIM1CH2_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
////	{ 
////			     if(tsr&0x04)//����2���������¼�		
////				{       TIM1->SR&=~(1<<2);//����жϱ�־λ 	 
////								if (TIM1CH2_CAPTURE_STA & 0X40)		//����һ���½���
////								{
////										
////									TIM1CH2_CAPTURE_DOWNVAL = TIM1->CCR2;//��¼�´�ʱ�Ķ�ʱ������ֵ
////									if (TIM1CH2_CAPTURE_DOWNVAL < TIM1CH2_CAPTURE_UPVAL)
////									{
////										TIM1_T2 = 65535;
////									}
////									else
////									TIM1_T2 = 0;
////									Remoter_Ch2 = TIM1CH2_CAPTURE_DOWNVAL - TIM1CH2_CAPTURE_UPVAL
////									+ TIM1_T2;		//�õ��ܵĸߵ�ƽ��ʱ��
////								  if(abs(Remoter_Ch2-L_Remoter_Ch2)>500)Remoter_Ch2=L_Remoter_Ch2;
////									L_Remoter_Ch2=Remoter_Ch2;
////									TIM1CH2_CAPTURE_STA = 0;		//�����־λ����
////									TIM1->CCER&=~(1<<5);			//CC1P=0 ����Ϊ�����ز���
////								}
////								else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
////								{ 
////									TIM1CH2_CAPTURE_UPVAL =TIM1->CCR2;		//��ȡ����������
////									TIM1CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
////									TIM1->CCER|=1<<5;			//CC1P=0 ����Ϊ�½��ز���
////								}
////				}
////	}
////		if ((TIM1CH3_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
////	{ 
////			     if(tsr&0x08)//����3���������¼�		
////				{       TIM1->SR&=~(1<<3);//����жϱ�־λ 	 
////								if (TIM1CH3_CAPTURE_STA & 0X40)		//����һ���½���
////								{
////										
////									TIM1CH3_CAPTURE_DOWNVAL = TIM1->CCR3;//��¼�´�ʱ�Ķ�ʱ������ֵ
////									if (TIM1CH3_CAPTURE_DOWNVAL < TIM1CH3_CAPTURE_UPVAL)
////									{
////										TIM1_T3 = 65535;
////									}
////									else
////									TIM1_T3 = 0;
////									Remoter_Ch3 = TIM1CH3_CAPTURE_DOWNVAL - TIM1CH3_CAPTURE_UPVAL
////									+ TIM1_T3;		//�õ��ܵĸߵ�ƽ��ʱ��
////									if(abs(Remoter_Ch3-L_Remoter_Ch3)>500)Remoter_Ch3=L_Remoter_Ch3;
////									L_Remoter_Ch3=Remoter_Ch3;
////									TIM1CH3_CAPTURE_STA = 0;		//�����־λ����
////									TIM1->CCER&=~(1<<9);			//CC1P=0 ����Ϊ�����ز���
////								}
////								else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
////								{ 
////									TIM1CH3_CAPTURE_UPVAL =TIM1->CCR3;		//��ȡ����������
////									TIM1CH3_CAPTURE_STA |= 0X40;		//����Ѳ���������
////									TIM1->CCER|=1<<9;			//CC1P=0 ����Ϊ�½��ز���
////								}
////				}
////	}
////		if ((TIM1CH4_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
////	{ 
////			     if(tsr&0x10)//����4���������¼�		
////				{       TIM1->SR&=~(1<<4);//����жϱ�־λ 	 
////								if (TIM1CH4_CAPTURE_STA & 0X40)		//����һ���½���
////								{
////										
////									TIM1CH4_CAPTURE_DOWNVAL = TIM1->CCR4;//��¼�´�ʱ�Ķ�ʱ������ֵ
////									if (TIM1CH4_CAPTURE_DOWNVAL < TIM1CH4_CAPTURE_UPVAL)
////									{
////										TIM1_T4 = 65535;
////									}
////									else
////									TIM1_T4 = 0;
////									Remoter_Ch4 = TIM1CH4_CAPTURE_DOWNVAL - TIM1CH4_CAPTURE_UPVAL
////									+ TIM1_T4;		//�õ��ܵĸߵ�ƽ��ʱ��
////									if(abs(Remoter_Ch4-L_Remoter_Ch4)>500)Remoter_Ch4=L_Remoter_Ch4;
////									L_Remoter_Ch4=Remoter_Ch4;
////									TIM1CH4_CAPTURE_STA = 0;		//�����־λ����
////									TIM1->CCER&=~(1<<13);			//CC1P=0 ����Ϊ�����ز���
////								}
////								else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
////								{ 
////									TIM1CH4_CAPTURE_UPVAL =TIM1->CCR4;		//��ȡ����������
////									TIM1CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
////									TIM1->CCER|=1<<13;			//CC1P=0 ����Ϊ�½��ز���
////								}
////				}
////	}
////}
////void TIM1_UP_IRQHandler(void) 
////{ 
////TIM1->SR&=~(1<<0);//����жϱ�־λ 	    
////}
