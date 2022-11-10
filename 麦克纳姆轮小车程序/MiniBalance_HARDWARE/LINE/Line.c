#include "Line.h"
u16 temp_data[10] = { 0 };       //���ݻ�����
u16 MaxError=330,MinError=0;
int error = 0;         //ƫ��ֵ
float P=1.6,I = 0,D =0.5;		 //����ϵ����΢��ϵ����ȡֵ��Χ0.01-0.9   //0.35  0.01  3
extern u8 chalu_flag;
void function(int error)
{
	static float Integral_error,Last_error;

	Integral_error+=error;
	
	Move_Z=error*P+Integral_error*I+(error-Last_error)*D;

	Last_error=error;
	
	
	if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //�ٶȿ����޷�
	if(Move_X>RC_Velocity)  Move_X=RC_Velocity;	     
	if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;	
	if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;	 
	if(Move_Z<-RC_Velocity) Move_Z=-RC_Velocity;	
	if(Move_Z>RC_Velocity)  Move_Z=RC_Velocity;	 


}

void Stop(void)
{
	Move_X=0;
	Move_Y=0;
	Move_Z=0;		
}
//ƽ�ƣ�   0-����  1-����
void Translation(char dir,char speed)
{
	if(dir)
	{
		Move_X=speed;
		Move_Y=0;
		Move_Z=0;	
	}
	else
	{
		Move_X=-speed;
		Move_Y=0;
		Move_Z=0;	
	}
}

//ƽ�ƣ�   0-ǰ��  1-����
void StraightBack(char dir,char speed)
{
	if(dir)
	{
		Move_X=0;
		Move_Y=-speed;
		Move_Z=0;	
	}
	else
	{
		Move_X=0;
		Move_Y=speed;
		Move_Z=0;	
	}
}

void function_back(void)
{
	
	static float Integral_error,Last_error;
	PCout(10)=0;
	PCout(1)=1;
	
	Move_Y=-10;               //ǰ���ٶ�
	Move_X=0;
	
//	int L_Pwm,R_Pwm;			 //�������ٶ�
	
  error=((temp_data[1]*4)-MinError)*100/(MaxError-MinError);
	
	if(temp_data[0]==1) //ƫ��
	{
	  error = error;
	}
	
	else if (temp_data[0]==2) //ƫ��
	{
	  error =-error;
	}
	
//	else if (temp_data[0]==0)                      // ƫ��Ϊ0
//	{
//		error = error;
//	}
	else if (temp_data[0]==5)  //�ҳ���
	{
		error=100;
	}
	else  if(temp_data[0]==6)  // �����
	{
		error=-100;
	}
	 
	Integral_error+=error;
	
	Move_Z=error*P+Integral_error*I+(error-Last_error)*D;

	Last_error=error;
	
	if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //�ٶȿ����޷�
	if(Move_X>RC_Velocity)  Move_X=RC_Velocity;	     
	if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;	
	if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;	 
	if(Move_Z<-RC_Velocity) Move_Z=-RC_Velocity;	
	if(Move_Z>RC_Velocity)  Move_Z=RC_Velocity;	 

	
	if (temp_data[0]==4) //ȫѹ
	{
		Move_Y=0;
		Move_Z=0;		
	}
	

}

