#include "show.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************
************************************************/
extern int error_num;
extern u8 turnleft_flag;
extern int road_num,road_num_2,road_num_3;
extern u8 chalu_flag;
extern u8 road_detch;
extern u8 stop_flag;
void oled_show(void)
{
  u8 str[20];

//	  OLED_ShowString(0,0,"X:");
//		if(Pitch<0)		OLED_ShowNumber(15,0,Pitch+360,3,12); //X轴角度值
//		else					OLED_ShowNumber(15,0,Pitch,3,12);	
//		OLED_ShowString(40,0,"Y:");
//		if(Roll<0)		OLED_ShowNumber(55,0,Roll+360,3,12);  //Y轴角度值
//		else					OLED_ShowNumber(55,0,Roll,3,12);	
//	   OLED_ShowString(80,0,"Z:");
//		if(Yaw<0)		  OLED_ShowNumber(95,0,Yaw+360,3,12);   //Z轴角度值
//		else					OLED_ShowNumber(95,0,Yaw,3,12);


///////////////////////////////////////////////////////////////////////////////////
//*****************	显示4路电机的目标值和编码器测量值****************************//
///////////////////////////////////////////////////////////////////////////////////	
		//=============显示电机A的状态=======================//	
/*	  if( Motor_A<0)		    OLED_ShowString(00,10,"-"),
		                      OLED_ShowNumber(15,10,-Motor_A,5,12);
		else                 	OLED_ShowString(0,10,"+"),
		                      OLED_ShowNumber(15,10, Motor_A,5,12); 
		
		if( Encoder_A<0)		  OLED_ShowString(80,10,"-"),
		                      OLED_ShowNumber(95,10,-Encoder_A,4,12);
		else                 	OLED_ShowString(80,10,"+"),
		                      OLED_ShowNumber(95,10, Encoder_A,4,12);
		//=============显示电机B的状态=======================//	
		  if( Motor_B<0)		  OLED_ShowString(00,20,"-"),
		                      OLED_ShowNumber(15,20,-Motor_B,5,12);
		else                 	OLED_ShowString(0,20,"+"),
		                      OLED_ShowNumber(15,20, Motor_B,5,12); 
		  
		if( Encoder_B<0)		  OLED_ShowString(80,20,"-"),
		                      OLED_ShowNumber(95,20,-Encoder_B,4,12);
		else                 	OLED_ShowString(80,20,"+"),
		                      OLED_ShowNumber(95,20, Encoder_B,4,12);
 		//=============显示电机C的状态=======================//	
		  if( Motor_C<0)		  OLED_ShowString(00,30,"-"),
		                      OLED_ShowNumber(15,30,-Motor_C,5,12);
		else                 	OLED_ShowString(0,30,"+"),
		                      OLED_ShowNumber(15,30, Motor_C,5,12); 
		  
		if( Encoder_C<0)		  OLED_ShowString(80,30,"-"),
		                      OLED_ShowNumber(95,30,-Encoder_C,4,12);
		else                 	OLED_ShowString(80,30,"+"),
		                      OLED_ShowNumber(95,30, Encoder_C,4,12);	
		//=============显示电机D的状态=======================//	
		  if( Motor_D<0)	  	OLED_ShowString(00,40,"-"),
		                      OLED_ShowNumber(15,40,-Motor_D,5,12);
		else                 	OLED_ShowString(0,40,"+"),
		                      OLED_ShowNumber(15,40, Motor_D,5,12); 
		
		if( Encoder_D<0)		    OLED_ShowString(80,40,"-"),
		                      OLED_ShowNumber(95,40,-Encoder_D,4,12);
		else                 	OLED_ShowString(80,40,"+"),
		                      OLED_ShowNumber(95,40, Encoder_D,4,12);*/
	sprintf(str,"stop:%d",stop_flag);
	OLED_ShowString(0,0,str);
		
	sprintf(str,"road:%d",road_detch);
	OLED_ShowString(0,10,str);
	
	sprintf(str,"roadnum:%d",road_num);
	OLED_ShowString(0,20,str);
	
	sprintf(str,"chalu:%d",chalu_flag);
	OLED_ShowString(0,30,str);
	
	sprintf(str,"turnleft:%d",turnleft_flag);
	OLED_ShowString(0,40,str);
		
	sprintf(str,"error:%d",error_num);
	OLED_ShowString(0,50,str);



			//=============第六行显示=======================//

//	if(PS2_ON_Flag==1)             OLED_ShowString(0,50,"PS2");
//	else if(Usart_ON_Flag1==1)     OLED_ShowString(0,50,"UART1");
//	else if(Usart_ON_Flag3==1)     OLED_ShowString(0,50,"UART3");
//		sprintf((char *)str ,"%d,%.4d ",temp_data[0],temp_data[1]*4);
//		OLED_ShowString(0,50,str);
//    sprintf((char *)str ,"a:%.4d,i:%.4d",MaxError,MinError);
//		OLED_ShowString(0,0,str);
//		//sprintf((char *)str ,"C:%d ,D:%d",(int)Motor_C,(int)Motor_D);
//		//sprintf((char *)str ,"CX:%.3d,CY:%.3d",CircleX,CircleY);
//		sprintf((char *)str ,"RX:%.3d,RY:%.3d",RectsX,RectsY);
//		OLED_ShowString(0,10,str);
//		sprintf((char *)str ,"CX:%.3d,CY:%.3d",CircleX,CircleY);
//		OLED_ShowString(0,30,str);
////		sprintf((char *)str ,"L: %.2d ",LineState);
////		OLED_ShowString(0,30,str);
//	  sprintf((char *)str ,"BX:%.3d,BY:%.3d ",BlueX,BlueY);
//		OLED_ShowString(0,40,str);



		
//							OLED_ShowString(58,50,".");
//							OLED_ShowString(82,50,"V");
//							OLED_ShowNumber(45,50,Voltage/100,2,12);
//							OLED_ShowNumber(70,50,Voltage%100,2,12);
//	if(Voltage%100<10)		OLED_ShowNumber(64,50,0,2,12); //电压显示
	
//	if(Flag_Stop==0)		OLED_ShowString(103,50,"O-N");
//	if(Flag_Stop==1)		OLED_ShowString(103,50,"OFF");  //按键使能开关
	
		//=============刷新=======================//
		OLED_Refresh_Gram();	

	}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void APP_Show(void)
{    
	  static u8 flag;
	  int app_2,app_3,app_4;
	  app_4=(Voltage-1100)*1/3;	
	  if(app_4>100)app_4=100;   //对电压数据进行处理
	  app_2=Move_X*0.7;  if(app_2<0)app_2=-app_2;			                   //对编码器数据就行数据处理便于图形化
		app_3=Move_Y*0.7;  if(app_3<0)app_3=-app_3;
	   if(Run_Flag==1)app_2=0,app_3=0;
	  flag=!flag;
	   USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	 	if(PID_Send==1)//发送PID参数
	  {
		DMA_printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI,0,0,0,0,0,0);//打印到APP上面	
		PID_Send=0;	
	  }	

   else	if(flag==0)// 
   DMA_printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,0); //打印到APP上面
	 else
	 DMA_printf("{B%d:%d:%d:%d}$",(int)Pitch,(int)Roll,(int)Yaw,app_4);//打印到APP上面 显示波形
}
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   
	if(Run_Flag==0)  //速度模式
	{
		DataScope_Get_Channel_Data( Encoder_A, 1 );       //显示目标值
		DataScope_Get_Channel_Data(Encoder_B, 2 );       //显示实际值，便于PID参数调节  
		DataScope_Get_Channel_Data(Encoder_C, 3 );                
  	DataScope_Get_Channel_Data(Encoder_D, 4 );     

	}

 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(4);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}

void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t)
{ 
	 u8 i=0;
	OLED_DrawPoint(x,y,t);
	OLED_DrawPoint(x,y,t);
	  for(i = 0;i<8; i++)
  {
      OLED_DrawPoint(x,y+i,t);
  }
}



//开机显示一次的内容
void oled_show_once(void) // 需添加
{
   OLED_ShowString(0,00,"Turn Right Wheel");
   OLED_ShowString(0,10,"TO Select Mode");
	 OLED_ShowString(0,20,"Current Mode Is");
//	if(Flag_Way==0)         OLED_ShowString(20,30,"Defult-Mode");
////	if(Flag_Way==1)         OLED_ShowString(50,30,"PS2");
//	if(Flag_Way==2)				  OLED_ShowString(20,30,"CCD-Tracing");
//	if(Flag_Way==3)				  OLED_ShowString(20,30,"ELE-Tracing");
  if (Flag_Way==0)        OLED_ShowString(20,30,"HUIDU-Tracing");//灰度巡线模式 （自加）
	OLED_ShowString(0,40,"Press User Key");
	OLED_ShowString(0,50,"TO End Selection");
		OLED_Refresh_Gram();	
	}


