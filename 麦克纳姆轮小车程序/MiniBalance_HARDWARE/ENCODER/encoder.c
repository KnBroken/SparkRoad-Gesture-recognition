#include "encoder.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// 需要使能AFIO时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//引脚重映射
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}
/**************************************************************************
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PA端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM3,0);
  TIM_Cmd(TIM3, ENABLE); 
}

/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM4,0);
  TIM_Cmd(TIM4, ENABLE); 
}

/**************************************************************************
函数功能：把TIM5初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM5(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//使能定时器的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PA端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM5,0);
  TIM_Cmd(TIM5, ENABLE); 
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;   TIM2 -> CNT=0; break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;   TIM3 -> CNT=0; break;
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;   TIM4 -> CNT=0;  break;	
		 case 5:  Encoder_TIM= (short)TIM5 -> CNT;   TIM5 -> CNT=0;  break;	
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：TIM3中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM5->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM5->SR&=~(1<<0);//清除中断标志位 	    
}

void Init_TIM1(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//使能定时器2的时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 199; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 25;                            //设置待装入捕获比较寄存器的脉冲值				//24是前   15是平移
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器

	TIM_Cmd(TIM1, ENABLE); 
}
int s;
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx更新中断标志 
		s++;
		/*写入执行的操作*/	
	}
	
}
