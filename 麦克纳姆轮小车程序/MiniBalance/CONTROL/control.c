#include "control.h"	
#include "filter.h"	
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

u8 Flag_Target,Flag_Change;                             //相关标志位
u8 temp1;                                               //临时变量
float Voltage_Count,Voltage_All; 											 //电压采样相关变量
float Gyro_K=0.004;     				  											//陀螺仪比例系数
int j,sum;
#define a_PARAMETER          (0.095f)               
#define b_PARAMETER          (0.086f)         
/**************************************************************************
函数功能：小车运动数学模型
入口参数：X Y Z 三轴速度或者位置
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float Vx,float Vy,float Vz)
{
	Target_A   = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
	Target_B   = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
	Target_C   = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	Target_D   = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	
}

/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步				 
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
		  EXTI->PR=1<<15;                                                      //清除LINE5上的中断标志位  		
		 //===5ms控制一次，为了保证M法测速的时间基准，首先读取编码器数据 
		
		
		Encoder_A=Read_Encoder(2);                                         		 //===读取编码器的值
		Encoder_B=Read_Encoder(3);                                         		 //===读取编码器的值
		Encoder_C=-Read_Encoder(4);                                         		//===读取编码器的值
		Encoder_D=-Read_Encoder(5);                                            //===读取编码器的值
		
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
			 if(++delay_50==10)	 delay_50=0,delay_flag=0;                     //给主函数提供50ms的精准延时
		 }
			 Flag_Target=!Flag_Target;		
		  if(Flag_Target==1)                                                  //10ms读取一次陀螺仪和加速度计的值
			{
				if(Usart_Flag==0&&(Usart_ON_Flag3==1||Usart_ON_Flag1==1))  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//如果解锁了串口控制标志位，进入串口控制模式
				Read_DMP();      //===更新姿态	
				Key();//扫描按键变化	
			return 0;	                                               
			}
		
			Led_Flash(100);                                                        //===LED闪烁;常规模式 1s改变一次指示灯的状态	
			//Voltage_All+=Get_battery_volt();                                       //多次采样累积
			//if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//求平均值 获取电池电压	 
			if(chalu_flag!=2&&!Gameover)
			{
				function(error_num);	 
			}
				

			
			Get_RC();
			Motor_A=Incremental_PI_A(Encoder_A,Target_A);                         //===速度闭环控制计算电机A最终PWM
			Motor_B=Incremental_PI_B(Encoder_B,Target_B);                         //===速度闭环控制计算电机B最终PWM
			Motor_C=Incremental_PI_C(Encoder_C,Target_C);                         //===速度闭环控制计算电机C最终PWM
			Motor_D=Incremental_PI_D(Encoder_D,Target_D);                         //===速度闭环控制计算电机C最终PWM
		
			Xianfu_Pwm(8000);                     //===PWM限幅
			Set_Pwm(Motor_A,Motor_B,Motor_C,Motor_D);     //===赋值给PWM寄存器

		 
 }
	 return 0;	 
} 

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
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
函数功能：限制PWM赋值 
入口参数：幅值
返回  值：无
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
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click_N_Double(50); 
	if(tmp==2)Flag_Show=!Flag_Show;//双击控制显示模式  
	if(tmp==1)Flag_Stop=!Flag_Stop;
//	if(tmp==1)P=P+0.5;//单击控制电机启停	
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<1110)//电池电压过低/使能开关为OFF 关闭电机
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
函数功能：绝对值函数
入口参数：long int
返回  值：unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	  u32 temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}

/**************************************************************************
函数功能：通过串口指令对小车进行遥控
入口参数：无
返回  值：无
**************************************************************************/
void Get_RC(void) //需添加
{

   Kinematic_Analysis(Move_X,Move_Y,Move_Z);//得到控制目标值，进行运动学分析
}


/**************************************************************************
函数功能：线性CCD取中值
入口参数：无
返回  值：无
**************************************************************************/
//void  Find_CCD_Zhongzhi(void)
//{ 
//	 static u16 i,j,Left,Right,Last_CCD_Zhongzhi;
//	 static u16 value1_max,value1_min;
//	
//	   value1_max=ADV[0];  //动态阈值算法，读取最大和最小值
//     for(i=5;i<123;i++)   //两边各去掉5个点
//     {
//        if(value1_max<=ADV[i])
//        value1_max=ADV[i];
//     }
//	   value1_min=ADV[0];  //最小值
//     for(i=5;i<123;i++) 
//     {
//        if(value1_min>=ADV[i])
//        value1_min=ADV[i];
//     }
//   CCD_Yuzhi=(value1_max+value1_min)/2;	  //计算出本次中线提取的阈值
//	 for(i = 5;i<118; i++)   //寻找左边跳变沿
//	{
//		if(ADV[i]>CCD_Yuzhi&&ADV[i+1]>CCD_Yuzhi&&ADV[i+2]>CCD_Yuzhi&&ADV[i+3]<CCD_Yuzhi&&ADV[i+4]<CCD_Yuzhi&&ADV[i+5]<CCD_Yuzhi)
//		{	
//			Left=i;
//			break;	
//		}
//	}
//	 for(j = 118;j>5; j--)//寻找右边跳变沿
//  {
//		if(ADV[j]<CCD_Yuzhi&&ADV[j+1]<CCD_Yuzhi&&ADV[j+2]<CCD_Yuzhi&&ADV[j+3]>CCD_Yuzhi&&ADV[j+4]>CCD_Yuzhi&&ADV[j+5]>CCD_Yuzhi)
//		{	
//		  Right=j;
//		  break;	
//		}
//  }
//	CCD_Zhongzhi=(Right+Left)/2;//计算中线位置
//	if(myabs(CCD_Zhongzhi-Last_CCD_Zhongzhi)>90)   //计算中线的偏差，如果太大
//	CCD_Zhongzhi=Last_CCD_Zhongzhi;    //则取上一次的值
//	Last_CCD_Zhongzhi=CCD_Zhongzhi;  //保存上一次的偏差
//}


//void CAN_N_Usart_Control(void)
//{
//	 int flag_1, flag_2,flag_3,i,error=1;
//	 u8 check;
//	 check=0;
//	 for(i=0;i<7;i++)
//	  {
//	    check=rxbuf[i]^check; //异或，用于检测数据是否出错
//    }
//	  if(check==rxbuf[7]) error=0; //检验成功
//	  else error=1;
//		
//				 if(error==0 )
//					{
//							 if((rxbuf[6]&0x04)==0)flag_1=1;  else flag_1=-1;  //方向控制位
//							 if((rxbuf[6]&0x02)==0)flag_2=1;  else flag_2=-1;  //方向控制位
//							 if((rxbuf[6]&0x01)==0)flag_3=1;  else flag_3=-1;  //方向控制位
//						
//							 Move_X=flag_1*(rxbuf[0]*256+rxbuf[1]);
//							 Move_Y=flag_2*(rxbuf[2]*256+rxbuf[3]);	
//							 Move_Z=flag_3*(rxbuf[4]*256+rxbuf[5]);						
//			
//				 } 

//}
///**************************************************************************
//函数功能：航模遥控接收处理函数
//入口参数：无
//返回  值：无
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
//				RX=Remoter_Ch1-1500;//自转
//				RC=Remoter_Ch3-900;//油门

//				if(LX>-Yuzhi&&LX<Yuzhi)LX=0;
//				if(LY>-Yuzhi&&LY<Yuzhi)LY=0;
//				if(RX>-Yuzhi&&RX<Yuzhi)RX=0;//
//				 Move_X=LX*RC_Velocity/500;
//				 Move_Y=LY*RC_Velocity/500;	
//				 Move_Z=-RX*RC_Velocity/500;		
//	
//			if(thrice>0) Move_X=0,Move_Y=0,Move_Z=0,thrice--;//用于屏蔽航模遥控刚连接上的时候的干扰信息
//      
//			//Kinematic_Analysis(Move_X,Move_Y,Move_Z);
//}

