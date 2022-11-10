#include "stm32f10x.h"
#include "sys.h"
void turn_180circle(void);
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //����ң����صı���
u8 Flag_Stop=1,Flag_Show=0;                 //ֹͣ��־λ(1:�� 0����)�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //���������������
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID������ر���
int Encoder_A_EXTI;                       //ͨ���ⲿ�ж϶�ȡ�ı���������                       
long int Motor_A,Motor_B,Motor_C,Motor_D;        //���PWM����
long int Target_A,Target_B,Target_C,Target_D;     //���Ŀ��ֵ
int Voltage;                             //��ص�ѹ������صı���
float Show_Data_Mb;                      //ȫ����ʾ������������ʾ��Ҫ�鿴������                         
u8 delay_50,delay_flag;                          //��ʱ��ر���
u8 Run_Flag=0,Flag_Way,Flag_Next;  						 //����ң����ر���������״̬��־λ
u8 rxbuf[8],Urxbuf[8],t2xbuf[129],t3xbuf[21],SENT[17]; //����2���͵Ļ���������; 
u8 CAN_ON_Flag=0,Usart_ON_Flag1=0,Usart_ON_Flag3=0,PS2_ON_Flag=0,Remote_ON_Flag=0,Usart_Flag,APP_ON_Flag=1,Usart_Flag,PID_Send,Flash_Send;  //CAN�ʹ��ڿ�����ر���

float Pitch,Roll,Yaw,Move_X,Move_Y,Move_Z;   //����ǶȺ�XYZ��Ŀ���ٶ�
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash�������
float	Position_KP=40,Position_KI=0,Position_KD=40;  //λ�ÿ���PID����
float Velocity_KP=10,Velocity_KI=10;	          //�ٶȿ���PID����
int RC_Velocity=50,RC_Position=3000;         //����ң�ص��ٶȺ�λ��ֵ
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;  
extern u8 turnleft_flag;
extern int road_num,road_num_2,road_num_3;
extern u8 turnleft_true_flag;
u8 onthe_chalu;
extern int error_num;
u8 chalu_flag;
u8 road_detch;
extern u8 stop_flag;
extern u8 Gameover;
extern u8 fpga_done;
extern int x_location;
extern int y_location;
int main(void)
{
		MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
		delay_init();	    	            //=====��ʱ������ʼ��	
		JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
		LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
		KEY_Init();                     //=====������ʼ��
		MiniBalance_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ�������������  TIM8
		Stop();
		OLED_Init();  		//=====OLED��ʼ��
		uart_init(115200);
		uart2_init(115200);               //=====����2��ʼ��
		uart3_init(115200);               //=====����3��ʼ�� 
		Encoder_Init_TIM2();            //=====��ʼ��������A
		Encoder_Init_TIM3();            //=====��ʼ��������B
		Encoder_Init_TIM4();            //=====��ʼ��������C
		Encoder_Init_TIM5();            //=====��ʼ��������D
		Init_TIM1();  //1S  
	
		Adc_Init();                     //=====adc��ʼ��
		IIC_Init();                     //=====IIC��ʼ��
		MPU6050_initialize();           //=====MPU6050��ʼ��	
		DMP_Init();                     //=====��ʼ��DMP     

        gpio_init();
		MiniBalance_EXTI_Init();        //=====MPU6050 5ms��ʱ�жϳ�ʼ��
//	Move_X = 20;	
	while(1)
	{		
		if(fpga_done)
		{
			if(y_location*0.25>=15)
				Move_Y = 15;
			else if(y_location*0.25<=-15)
				Move_Y = -15;
			else
				Move_Y = y_location*0.25;
			if(x_location*0.25>=15)
				Move_X = 15;
			else if(x_location*0.25<=-15)
				Move_X = -15;
			else
				Move_X = x_location*0.25;
		}
//		oled_show();
	}
}