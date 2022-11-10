#include "sys.h"
#include "usart.h"	  

  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);// 
	USART2->DR = (u8) ch;      
	return ch;
}
#endif 
int Usart_Receive;
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ֽ�
*********************************************************************************/

u8 a;
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
	
}

void uart_init(u32 bound)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USARTʱ��

	//USART_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
    //USART_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    //UsartNVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);     //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1

}
/**************************************************************************
�������ܣ�����1�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
char openmvdata[128] = { 0 };       
u16 num = 0;              //�������ݵĵ�ǰλ��
char openmvstate;
char dataform = 0;//1-Բ�� 2-���� 3-��ɫ 4-��ɫ
char update,blueupdate,paperupdate;//���ݸ��±�־
int error_num;
u8 turnleft_flag,turnright_flag;
u8 stop_flag;

#define center_x 300
#define center_y 200

u8 x_point[2];
u8 y_point[2];

u16 x_locate;
u16 y_locate;
int x_location;
int y_location;
u32 Pow_16(u8 x)
{
	u32 pow = 1;
	u8  i;
	for(i = 0;i < x;i++)
	{
		pow *= 2;
	}
	return pow;
}
u8 fpga_done;
u8 cache;
void USART1_IRQHandler(void)
{	
	u8 temp;
	u8 i;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{	  	      
		openmvdata[num++]=USART_ReceiveData(USART1);
		if(num==128)
			num =0;
		if(openmvdata[num-2] == 0x0D && openmvdata[num-1] == 0x0A && openmvstate == 0)
		{
			openmvstate = 1;
			num = 0;
			fpga_done = 0;
		}
		if(openmvstate)
		{
			x_locate = 0;
			y_locate = 0;
			x_point[0] = openmvdata[0];
			x_point[1] = openmvdata[1];
			y_point[0] = openmvdata[2];
			y_point[1] = openmvdata[3];
			
			cache = x_point[1];	
			for(i = 0;i<8;i++)
			{
				if(cache & 0x01)
				{
					x_locate += Pow_16(i);
				}
				cache >>= 1;	
			}
			cache = x_point[0];		
			for(i = 0;i<8;i++)
			{
				if(cache & 0x01)
				{
					x_locate += Pow_16(i+8);
				}
				cache >>= 1;	
			}
			
			cache = y_point[1];	
			for(i = 0;i<8;i++)
			{
				if(cache & 0x01)
				{
					y_locate += Pow_16(i);
				}
				cache >>= 1;	
			}
			cache = y_point[0];		
			for(i = 0;i<8;i++)
			{
				if(cache & 0x01)
				{
					y_locate += Pow_16(i+8);
				}
				cache >>= 1;	
			}
			
			y_location =  center_y - y_locate;
			x_location =  x_locate - center_x;
			fpga_done = 1;
			openmvstate = 0;
		}

		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }

}
char CircleX,CircleY,RectsX,RectsY,YellowX,YellowY,WhiteX,WhiteY,BlueX,BlueY;
void Openmv_Decode(void)
{
		switch(dataform)
		{
			case 1://Բ��
					if(openmvdata[4]==',')//һλ��
					{
						CircleX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//һλ��
							CircleY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//��λ��
							CircleY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							CircleY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//��λ��
					{
						CircleX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//һλ��
							CircleY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							CircleY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							CircleY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//��λ��
					{
						CircleX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//һλ��
							CircleY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							CircleY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//��λ��
							CircleY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 2://����
					if(openmvdata[4]==',')//һλ��
					{
						RectsX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//һλ��
							RectsY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//��λ��
							RectsY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							RectsY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//��λ��
					{
						RectsX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//һλ��
							RectsY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							RectsY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							RectsY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//��λ��
					{
						RectsX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//һλ��
							RectsY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							RectsY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//��λ��
							RectsY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 3://��ɫ
					if(openmvdata[4]==',')//һλ��
					{
						YellowX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//һλ��
							YellowY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//��λ��
							YellowY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							YellowY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//��λ��
					{
						YellowX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//һλ��
							YellowY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							YellowY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							YellowY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//��λ��
					{
						YellowX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//һλ��
							YellowY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							YellowY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//��λ��
							YellowY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 4://��ɫ
					if(openmvdata[4]==',')//һλ��
					{
						WhiteX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//һλ��
							WhiteY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//��λ��
							WhiteY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							WhiteY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//��λ��
					{
						WhiteX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//һλ��
							WhiteY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							WhiteY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							WhiteY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//��λ��
					{
						WhiteX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//һλ��
							WhiteY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							WhiteY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//��λ��
							WhiteY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 5://��ɫ
					if(openmvdata[4]==',')//һλ��
					{
						BlueX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//һλ��
							BlueY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//��λ��
							BlueY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							BlueY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//��λ��
					{
						BlueX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//һλ��
							BlueY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//��λ��
							BlueY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							BlueY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//��λ��
					{
						BlueX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//һλ��
							BlueY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//��λ��
							BlueY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//��λ��
							BlueY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
			default:break;
		}
}
//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart2����һ���ֽ�
*********************************************************************************/
void usart2_send(u8 data)
{
	ok=0;
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}


u8 ok;
void uart2_init(u32 bound)
{  	 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USARTʱ��
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
   
  //USART2_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3  
   //USART ��ʼ������

	  //UsartNVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 
	
}
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	 {	  
		 ok=USART_ReceiveData(USART2);
	   USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		 
	 }
}


/**************************************************************************
�������ܣ�����ɨ��
**************************************************************************/
//u8 click_RC (void)
//{
//			static u8 flag_key=1;//�������ɿ���־
//	    u8 temp;
//			if(flag_key&&Usart_Receive!=0x5A)
//			{
//			flag_key=0;
//		  if(Usart_Receive>=0x01&&Usart_Receive<=0x08)temp=Usart_Receive;
//		  else	if(Usart_Receive>=0x41&&Usart_Receive<=0x48)temp=Usart_Receive-0x40;	
//		//	else 	temp=0;
//			return temp;	// ��������
//			}
//			else if(Usart_Receive==0x5A)			flag_key=1;
//			return 0;//�ް�������
//}

