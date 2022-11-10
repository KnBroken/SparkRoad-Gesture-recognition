#include "sys.h"
#include "usart.h"	  

  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);// 
	USART2->DR = (u8) ch;      
	return ch;
}
#endif 
int Usart_Receive;
/**************************实现函数**********************************************
*功    能:		usart1发送一个字节
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIO时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART时钟

	//USART_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
    //USART_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    //UsartNVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//收发模式
  USART_Init(USART1, &USART_InitStructure);     //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1

}
/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
char openmvdata[128] = { 0 };       
u16 num = 0;              //接收数据的当前位置
char openmvstate;
char dataform = 0;//1-圆形 2-方形 3-黄色 4-白色
char update,blueupdate,paperupdate;//数据更新标志
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
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
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
			case 1://圆形
					if(openmvdata[4]==',')//一位数
					{
						CircleX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//一位数
							CircleY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//两位数
							CircleY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//三位数
							CircleY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//两位数
					{
						CircleX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//一位数
							CircleY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//两位数
							CircleY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//三位数
							CircleY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//三位数
					{
						CircleX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//一位数
							CircleY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//两位数
							CircleY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//三位数
							CircleY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 2://方形
					if(openmvdata[4]==',')//一位数
					{
						RectsX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//一位数
							RectsY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//两位数
							RectsY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//三位数
							RectsY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//两位数
					{
						RectsX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//一位数
							RectsY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//两位数
							RectsY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//三位数
							RectsY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//三位数
					{
						RectsX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//一位数
							RectsY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//两位数
							RectsY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//三位数
							RectsY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 3://黄色
					if(openmvdata[4]==',')//一位数
					{
						YellowX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//一位数
							YellowY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//两位数
							YellowY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//三位数
							YellowY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//两位数
					{
						YellowX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//一位数
							YellowY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//两位数
							YellowY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//三位数
							YellowY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//三位数
					{
						YellowX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//一位数
							YellowY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//两位数
							YellowY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//三位数
							YellowY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 4://白色
					if(openmvdata[4]==',')//一位数
					{
						WhiteX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//一位数
							WhiteY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//两位数
							WhiteY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//三位数
							WhiteY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//两位数
					{
						WhiteX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//一位数
							WhiteY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//两位数
							WhiteY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//三位数
							WhiteY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//三位数
					{
						WhiteX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//一位数
							WhiteY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//两位数
							WhiteY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//三位数
							WhiteY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
				case 5://蓝色
					if(openmvdata[4]==',')//一位数
					{
						BlueX = openmvdata[3]-0x30;
						
						if(openmvdata[6]==')')//一位数
							BlueY =(openmvdata[5]-0x30);
						else if(openmvdata[7]==')')//两位数
							BlueY =(openmvdata[5]-0x30)*10+(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//三位数
							BlueY =(openmvdata[5]-0x30)*100+(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
					}
					else if(openmvdata[5]==',')//两位数
					{
						BlueX = (openmvdata[3]-0x30)*10+(openmvdata[4]-0x30);
						
						if(openmvdata[7]==')')//一位数
							BlueY =(openmvdata[6]-0x30);
						else if(openmvdata[8]==')')//两位数
							BlueY =(openmvdata[6]-0x30)*10+(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//三位数
							BlueY =(openmvdata[6]-0x30)*100+(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
					}
					else if(openmvdata[6]==',')//三位数
					{
						BlueX = (openmvdata[3]-0x30)*100+(openmvdata[4]-0x30)*10+(openmvdata[5]-0x30);
						
						if(openmvdata[8]==')')//一位数
							BlueY =(openmvdata[7]-0x30);
						else if(openmvdata[9]==')')//两位数
							BlueY =(openmvdata[7]-0x30)*10+(openmvdata[8]-0x30);
						else if(openmvdata[10]==')')//三位数
							BlueY =(openmvdata[7]-0x30)*100+(openmvdata[8]-0x30)*10+(openmvdata[9]-0x30);
					}
					break;
			default:break;
		}
}
//////////////////////////////////////////////////////////////////
/**************************实现函数**********************************************
*功    能:		usart2发送一个字节
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
 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART时钟
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3  
   //USART 初始化设置

	  //UsartNVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2 
	
}
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	 {	  
		 ok=USART_ReceiveData(USART2);
	   USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		 
	 }
}


/**************************************************************************
函数功能：串口扫描
**************************************************************************/
//u8 click_RC (void)
//{
//			static u8 flag_key=1;//按键按松开标志
//	    u8 temp;
//			if(flag_key&&Usart_Receive!=0x5A)
//			{
//			flag_key=0;
//		  if(Usart_Receive>=0x01&&Usart_Receive<=0x08)temp=Usart_Receive;
//		  else	if(Usart_Receive>=0x41&&Usart_Receive<=0x48)temp=Usart_Receive-0x40;	
//		//	else 	temp=0;
//			return temp;	// 按键按下
//			}
//			else if(Usart_Receive==0x5A)			flag_key=1;
//			return 0;//无按键按下
//}

