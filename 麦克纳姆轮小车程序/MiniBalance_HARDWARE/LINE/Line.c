#include "Line.h"
u16 temp_data[10] = { 0 };       //数据缓存区
u16 MaxError=330,MinError=0;
int error = 0;         //偏差值
float P=1.6,I = 0,D =0.5;		 //积分系数，微分系数，取值范围0.01-0.9   //0.35  0.01  3
extern u8 chalu_flag;
void function(int error)
{
	static float Integral_error,Last_error;

	Integral_error+=error;
	
	Move_Z=error*P+Integral_error*I+(error-Last_error)*D;

	Last_error=error;
	
	
	if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //速度控制限幅
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
//平移：   0-左移  1-右移
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

//平移：   0-前进  1-后退
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
	
	Move_Y=-10;               //前进速度
	Move_X=0;
	
//	int L_Pwm,R_Pwm;			 //左右轮速度
	
  error=((temp_data[1]*4)-MinError)*100/(MaxError-MinError);
	
	if(temp_data[0]==1) //偏右
	{
	  error = error;
	}
	
	else if (temp_data[0]==2) //偏左
	{
	  error =-error;
	}
	
//	else if (temp_data[0]==0)                      // 偏差为0
//	{
//		error = error;
//	}
	else if (temp_data[0]==5)  //右出线
	{
		error=100;
	}
	else  if(temp_data[0]==6)  // 左出线
	{
		error=-100;
	}
	 
	Integral_error+=error;
	
	Move_Z=error*P+Integral_error*I+(error-Last_error)*D;

	Last_error=error;
	
	if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //速度控制限幅
	if(Move_X>RC_Velocity)  Move_X=RC_Velocity;	     
	if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;	
	if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;	 
	if(Move_Z<-RC_Velocity) Move_Z=-RC_Velocity;	
	if(Move_Z>RC_Velocity)  Move_Z=RC_Velocity;	 

	
	if (temp_data[0]==4) //全压
	{
		Move_Y=0;
		Move_Z=0;		
	}
	

}

