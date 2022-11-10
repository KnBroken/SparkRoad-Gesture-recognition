#include "stm32f10x.h"
#include "sys.h"
void turn_180circle(void);
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //蓝牙遥控相关的变量
u8 Flag_Stop=1,Flag_Show=0;                 //停止标志位(1:关 0：开)和 显示标志位 默认停止 显示打开
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //编码器的脉冲计数
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID控制相关变量
int Encoder_A_EXTI;                       //通过外部中断读取的编码器数据                       
long int Motor_A,Motor_B,Motor_C,Motor_D;        //电机PWM变量
long int Target_A,Target_B,Target_C,Target_D;     //电机目标值
int Voltage;                             //电池电压采样相关的变量
float Show_Data_Mb;                      //全局显示变量，用于显示需要查看的数据                         
u8 delay_50,delay_flag;                          //延时相关变量
u8 Run_Flag=0,Flag_Way,Flag_Next;  						 //蓝牙遥控相关变量和运行状态标志位
u8 rxbuf[8],Urxbuf[8],t2xbuf[129],t3xbuf[21],SENT[17]; //串口2发送的缓存数据组; 
u8 CAN_ON_Flag=0,Usart_ON_Flag1=0,Usart_ON_Flag3=0,PS2_ON_Flag=0,Remote_ON_Flag=0,Usart_Flag,APP_ON_Flag=1,Usart_Flag,PID_Send,Flash_Send;  //CAN和串口控制相关变量

float Pitch,Roll,Yaw,Move_X,Move_Y,Move_Z;   //三轴角度和XYZ轴目标速度
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash相关数组
float	Position_KP=40,Position_KI=0,Position_KD=40;  //位置控制PID参数
float Velocity_KP=10,Velocity_KI=10;	          //速度控制PID参数
int RC_Velocity=50,RC_Position=3000;         //设置遥控的速度和位置值
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
		MY_NVIC_PriorityGroupConfig(2);	//=====设置中断分组
		delay_init();	    	            //=====延时函数初始化	
		JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
		LED_Init();                     //=====初始化与 LED 连接的硬件接口
		KEY_Init();                     //=====按键初始化
		MiniBalance_PWM_Init(7199,0);   //=====初始化PWM 10KHZ，用于驱动电机  TIM8
		Stop();
		OLED_Init();  		//=====OLED初始化
		uart_init(115200);
		uart2_init(115200);               //=====串口2初始化
		uart3_init(115200);               //=====串口3初始化 
		Encoder_Init_TIM2();            //=====初始化编码器A
		Encoder_Init_TIM3();            //=====初始化编码器B
		Encoder_Init_TIM4();            //=====初始化编码器C
		Encoder_Init_TIM5();            //=====初始化编码器D
		Init_TIM1();  //1S  
	
		Adc_Init();                     //=====adc初始化
		IIC_Init();                     //=====IIC初始化
		MPU6050_initialize();           //=====MPU6050初始化	
		DMP_Init();                     //=====初始化DMP     

        gpio_init();
		MiniBalance_EXTI_Init();        //=====MPU6050 5ms定时中断初始化
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