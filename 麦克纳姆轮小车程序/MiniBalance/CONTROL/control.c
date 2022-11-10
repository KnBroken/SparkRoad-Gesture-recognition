#include "control.h"	
#include "filter.h"	
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

u8 Flag_Target,Flag_Change;                             //��ر�־λ
u8 temp1;                                               //��ʱ����
float Voltage_Count,Voltage_All; 											 //��ѹ������ر���
float Gyro_K=0.004;     				  											//�����Ǳ���ϵ��
int j,sum;
#define a_PARAMETER          (0.095f)               
#define b_PARAMETER          (0.086f)         
/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ�����X Y Z �����ٶȻ���λ��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float Vx,float Vy,float Vz)
{
	Target_A   = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
	Target_B   = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
	Target_C   = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	Target_D   = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
extern int error_num;
extern u8 turnleft_flag,turnright_flag;
int road_num,road_num_2,road_num_3;
u8 turnleft_true_flag;
extern u8 onthe_chalu;
extern u8 chalu_flag;
u8 Gameover;
extern u8 stop_flag;
int EXTI15_10_IRQHandler(void) 
{    
	if(INT==0)		
	{     
		  EXTI->PR=1<<15;                                                      //���LINE5�ϵ��жϱ�־λ  		
		 //===5ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ���������� 
		
		
		Encoder_A=Read_Encoder(2);                                         		 //===��ȡ��������ֵ
		Encoder_B=Read_Encoder(3);                                         		 //===��ȡ��������ֵ
		Encoder_C=-Read_Encoder(4);                                         		//===��ȡ��������ֵ
		Encoder_D=-Read_Encoder(5);                                            //===��ȡ��������ֵ
		
		if(turnleft_flag && chalu_flag == 0)
		{
			road_num += Encoder_A;
		}
		if(road_num >= 1900)
		{
			turnleft_true_flag = 1;
			road_num = 0;
			turnleft_flag = 0;
		}
		if(chalu_flag == 1&&turnleft_flag)
		{
			road_num_2 += Encoder_A;
		}
		if(chalu_flag == 2)
		{
			road_num_3 += Encoder_A;
		}
		if(stop_flag)
		{
			road_num += Encoder_A;
		}	
		
		if(delay_flag==1)
		 {
			 if(++delay_50==10)	 delay_50=0,delay_flag=0;                     //���������ṩ50ms�ľ�׼��ʱ
		 }
			 Flag_Target=!Flag_Target;		
		  if(Flag_Target==1)                                                  //10ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ
			{
				if(Usart_Flag==0&&(Usart_ON_Flag3==1||Usart_ON_Flag1==1))  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//��������˴��ڿ��Ʊ�־λ�����봮�ڿ���ģʽ
				Read_DMP();      //===������̬	
				Key();//ɨ�谴���仯	
			return 0;	                                               
			}
		
			Led_Flash(100);                                                        //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬	
			//Voltage_All+=Get_battery_volt();                                       //��β����ۻ�
			//if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//��ƽ��ֵ ��ȡ��ص�ѹ	 
			if(chalu_flag!=2&&!Gameover)
			{
				function(error_num);	 
			}
				

			
			Get_RC();
			Motor_A=Incremental_PI_A(Encoder_A,Target_A);                         //===�ٶȱջ����Ƽ�����A����PWM
			Motor_B=Incremental_PI_B(Encoder_B,Target_B);                         //===�ٶȱջ����Ƽ�����B����PWM
			Motor_C=Incremental_PI_C(Encoder_C,Target_C);                         //===�ٶȱջ����Ƽ�����C����PWM
			Motor_D=Incremental_PI_D(Encoder_D,Target_D);                         //===�ٶȱջ����Ƽ�����C����PWM
		
			Xianfu_Pwm(8000);                     //===PWM�޷�
			Set_Pwm(Motor_A,Motor_B,Motor_C,Motor_D);     //===��ֵ��PWM�Ĵ���

		 
 }
	 return 0;	 
} 

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d)
{
	    int siqu=0;
    	if(motor_a>0)			PWMA=motor_a+siqu, INA=0;
			else  	            PWMA=7200+motor_a-siqu,INA=1;
		
		  if(motor_b>0)			PWMB=7200-motor_b-siqu,INB=1;
			else 	            PWMB=-motor_b+siqu,INB=0;
	
	    if(motor_c>0)			PWMC=7200-motor_c-siqu,INC=1;
			else 	            PWMC=-motor_c+siqu,INC=0;
	
	   if(motor_d>0)			PWMD=motor_d+siqu,IND=0;
			else 	            PWMD=7200+motor_d-siqu,IND=1;
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_A<-amplitude) Motor_A=-amplitude;	
		if(Motor_A>amplitude)  Motor_A=amplitude;	
	  if(Motor_B<-amplitude) Motor_B=-amplitude;	
		if(Motor_B>amplitude)  Motor_B=amplitude;		
	  if(Motor_C<-amplitude) Motor_C=-amplitude;	
		if(Motor_C>amplitude)  Motor_C=amplitude;		
	  if(Motor_D<-amplitude) Motor_D=-amplitude;	
	  if(Motor_D>amplitude)  Motor_D=amplitude;		
}

/**************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click_N_Double(50); 
	if(tmp==2)Flag_Show=!Flag_Show;//˫��������ʾģʽ  
	if(tmp==1)Flag_Stop=!Flag_Stop;
//	if(tmp==1)P=P+0.5;//�������Ƶ����ͣ	
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<1110)//��ص�ѹ����/ʹ�ܿ���ΪOFF �رյ��
			{	                                                
      temp=1;      
      PWMA=0;
      PWMB=0;
      PWMC=0;
	    PWMD=0;
      INA=0;
      INB=0;
      INC=0;		
	    IND=0;								
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����long int
����  ֵ��unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	  u32 temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}

/**************************************************************************
�������ܣ�ͨ������ָ���С������ң��
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_RC(void) //�����
{

   Kinematic_Analysis(Move_X,Move_Y,Move_Z);//�õ�����Ŀ��ֵ�������˶�ѧ����
}


/**************************************************************************
�������ܣ�����CCDȡ��ֵ
��ڲ�������
����  ֵ����
**************************************************************************/
//void  Find_CCD_Zhongzhi(void)
//{ 
//	 static u16 i,j,Left,Right,Last_CCD_Zhongzhi;
//	 static u16 value1_max,value1_min;
//	
//	   value1_max=ADV[0];  //��̬��ֵ�㷨����ȡ������Сֵ
//     for(i=5;i<123;i++)   //���߸�ȥ��5����
//     {
//        if(value1_max<=ADV[i])
//        value1_max=ADV[i];
//     }
//	   value1_min=ADV[0];  //��Сֵ
//     for(i=5;i<123;i++) 
//     {
//        if(value1_min>=ADV[i])
//        value1_min=ADV[i];
//     }
//   CCD_Yuzhi=(value1_max+value1_min)/2;	  //���������������ȡ����ֵ
//	 for(i = 5;i<118; i++)   //Ѱ�����������
//	{
//		if(ADV[i]>CCD_Yuzhi&&ADV[i+1]>CCD_Yuzhi&&ADV[i+2]>CCD_Yuzhi&&ADV[i+3]<CCD_Yuzhi&&ADV[i+4]<CCD_Yuzhi&&ADV[i+5]<CCD_Yuzhi)
//		{	
//			Left=i;
//			break;	
//		}
//	}
//	 for(j = 118;j>5; j--)//Ѱ���ұ�������
//  {
//		if(ADV[j]<CCD_Yuzhi&&ADV[j+1]<CCD_Yuzhi&&ADV[j+2]<CCD_Yuzhi&&ADV[j+3]>CCD_Yuzhi&&ADV[j+4]>CCD_Yuzhi&&ADV[j+5]>CCD_Yuzhi)
//		{	
//		  Right=j;
//		  break;	
//		}
//  }
//	CCD_Zhongzhi=(Right+Left)/2;//��������λ��
//	if(myabs(CCD_Zhongzhi-Last_CCD_Zhongzhi)>90)   //�������ߵ�ƫ����̫��
//	CCD_Zhongzhi=Last_CCD_Zhongzhi;    //��ȡ��һ�ε�ֵ
//	Last_CCD_Zhongzhi=CCD_Zhongzhi;  //������һ�ε�ƫ��
//}


//void CAN_N_Usart_Control(void)
//{
//	 int flag_1, flag_2,flag_3,i,error=1;
//	 u8 check;
//	 check=0;
//	 for(i=0;i<7;i++)
//	  {
//	    check=rxbuf[i]^check; //������ڼ�������Ƿ����
//    }
//	  if(check==rxbuf[7]) error=0; //����ɹ�
//	  else error=1;
//		
//				 if(error==0 )
//					{
//							 if((rxbuf[6]&0x04)==0)flag_1=1;  else flag_1=-1;  //�������λ
//							 if((rxbuf[6]&0x02)==0)flag_2=1;  else flag_2=-1;  //�������λ
//							 if((rxbuf[6]&0x01)==0)flag_3=1;  else flag_3=-1;  //�������λ
//						
//							 Move_X=flag_1*(rxbuf[0]*256+rxbuf[1]);
//							 Move_Y=flag_2*(rxbuf[2]*256+rxbuf[3]);	
//							 Move_Z=flag_3*(rxbuf[4]*256+rxbuf[5]);						
//			
//				 } 

//}
///**************************************************************************
//�������ܣ���ģң�ؽ��մ�����
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void Remote_Control(void)
//{
//	static u8 thrice=50;
//	int Yuzhi=50;
//	int LX,LY,RX,RC;
//				if(Remoter_Ch1<1000)Remoter_Ch1=1000;
//     	  if(Remoter_Ch1>2000)Remoter_Ch1=2000;
//				if(Remoter_Ch2<1000)Remoter_Ch2=1000;
//     	  if(Remoter_Ch2>2000)Remoter_Ch2=2000;
//				if(Remoter_Ch3<1000)Remoter_Ch3=1000;
//     	  if(Remoter_Ch3>2000)Remoter_Ch3=2000;
//				if(Remoter_Ch4<1000)Remoter_Ch4=1000;
//     	  if(Remoter_Ch4>2000)Remoter_Ch4=2000;
//	
//				LY=Remoter_Ch2-1500;
//				LX=Remoter_Ch4-1500;
//				RX=Remoter_Ch1-1500;//��ת
//				RC=Remoter_Ch3-900;//����

//				if(LX>-Yuzhi&&LX<Yuzhi)LX=0;
//				if(LY>-Yuzhi&&LY<Yuzhi)LY=0;
//				if(RX>-Yuzhi&&RX<Yuzhi)RX=0;//
//				 Move_X=LX*RC_Velocity/500;
//				 Move_Y=LY*RC_Velocity/500;	
//				 Move_Z=-RX*RC_Velocity/500;		
//	
//			if(thrice>0) Move_X=0,Move_Y=0,Move_Z=0,thrice--;//�������κ�ģң�ظ������ϵ�ʱ��ĸ�����Ϣ
//      
//			//Kinematic_Analysis(Move_X,Move_Y,Move_Z);
//}

