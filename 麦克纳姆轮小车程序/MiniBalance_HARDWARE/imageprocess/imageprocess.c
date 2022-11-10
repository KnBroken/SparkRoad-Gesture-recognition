#include "imageprocess.h"

char LineState=15   ;//巡线标志位

char zhituanX=160,zhituanY=120,zhituanthresh=20;		//纸团第一次阈值
char zhituanX2=215,zhituanY2=80,zhituanthresh2=20;  //纸团第二次阈值
char zhituanX3=180,zhituanY3=160,zhituanthresh3=20;
char zhituanX4=135,zhituanY4=80,zhituanthresh4=20;
char bluetuanX=85,bluetuanY=80,bluetuanthresh=10;		//蓝色阈值

char laji=0;//0-纸团
/*void Imageprocee_Line(void)
{ 
	switch(LineState)
	{
		case 0://停车
					Stop();break;
//		case 1://巡线+找纸团
//					function();
//					if((CircleX >(zhituanX-zhituanthresh) && CircleX <(zhituanX+zhituanthresh)) && (CircleY >(zhituanY-zhituanthresh) && CircleY <(zhituanY+zhituanthresh)) )
//					{
//						Stop();
//						LineState = 2;
//					}
//					break;
//					
//		case 1://巡线+找纸团
//					function();
//					if((RectsX >(zhituanX-zhituanthresh) && RectsX <(zhituanX+zhituanthresh)) && (RectsY >(zhituanY-zhituanthresh) && RectsY <(zhituanY+zhituanthresh)) )
//					{
//						Stop();
//						LineState = 2;
//					}
//					break;
		case 1:
//					function();
					if(update)
					{
						Stop();
						LineState = 2;
					}
					break;
		case 2://自动修正
					//if(!Auto_Move(CircleX,CircleY,zhituanX2,zhituanY2,zhituanthresh2))
					if(!Auto_Move(RectsX,zhituanY,zhituanX,zhituanY,zhituanthresh))	
					{	
						Stop();
						LineState = 3;
					}	
					break;
		case 3://机械臂下移
					usart2_send('1');
					LineState = 4;
					while(ok!='O');
					ok=0;
					update = 0;
				
					break;
		case 4://左移+第二次找纸团
			    Translation(0,5);
					//if((CircleX >(zhituanX2-zhituanthresh2) && CircleX <(zhituanX2+zhituanthresh2)) && (CircleY >(zhituanY2-zhituanthresh2) && CircleY <(zhituanY2+zhituanthresh2)) ) 
					if(update) 
					{	
						Stop();
						LineState = 5;
					}	
					break;
		case 5://自动修正
					//if(!Auto_Move(CircleX,CircleY,zhituanX2,zhituanY2,zhituanthresh2))
					if(!Auto_Move(RectsX,RectsY,zhituanX2,zhituanY2,zhituanthresh2))	
					{	
						Stop();
						LineState = 6;
					}	
					break;
		case 6://抓取
					usart2_send('2');
					LineState = 7;
					while(ok!='O');
					break; 
		case 7://平移回去+加找线
					if(temp_data[0]==1||temp_data[0]==2)
					{
						Stop();
						LineState = 8;
						ok=0;
					}
					Translation(1,15);
					break;
		case 8://送垃圾
					function();
					if(ok=='1')
					{
					  Stop();
						LineState = 9;
					}
					break;
		case 9://按照颜色找垃圾
					switch(laji)
					{
						case 0:
						  LineState = 10;
						  blueupdate=0;
						  Right_num=0;
						  break;//纸团
					}
					break;
//		case 9://左移找蓝色
//					Translation(0,10);
//					//if((CircleX >(zhituanX2-zhituanthresh2) && CircleX <(zhituanX2+zhituanthresh2)) && (CircleY >(zhituanY2-zhituanthresh2) && CircleY <(zhituanY2+zhituanthresh2)) )
//					if(blueupdate)
//					{	
//						Stop();
//						LineState = 10;
//					}		
//					break;
//		case 10://自动修正
//					if(!Auto_Move2(BlueX,BlueY,bluetuanX,bluetuanY,bluetuanthresh))
//					{	
//						Stop();
//						LineState = 11;
//					}	
//					break;
		case 10://左移找蓝色(炮筒)
					Translation(0,20);
					if(Right_Judge()==1 )
					{	
						Stop();
						LineState = 11;
					}		
					break;
		
					
		case 11://送垃圾
					usart2_send('3');
					while(ok!='O');
					LineState = 12;
					break;
		case 12: //恢复初始状态
					usart2_send('0');
					while(ok!='O');	
					LineState = 13;
					break;
		case 13://平移找线
					if(ok=='6')
					{
						//Stop();
						LineState = 14;
						ok=0;
					}
					Translation(1,10);
					break;
		case 14://灰度找线
					if(temp_data[0]==1||temp_data[0]==2)
					{
					  Stop();
						LineState = 15;
						ok=0;
					}	
					Translation(1,10);
					break;
		case 15:
					function_back();
					if(paperupdate)
					{
						Stop();
						LineState = 16;
					}
					break;
		case 16:
					if(!Auto_Move(CircleX,zhituanY3,zhituanX3,zhituanY3,zhituanthresh3))	
					{	
						Stop();
						LineState = 17;
					}	
					break;
					
		case 17://机械臂下移
					usart2_send('1');
					LineState = 18;
					while(ok!='O');
					ok=0;
					paperupdate = 0;
				
					break;
		case 18://左移+第二次找纸团
			    Translation(0,5);
					//if((CircleX >(zhituanX2-zhituanthresh2) && CircleX <(zhituanX2+zhituanthresh2)) && (CircleY >(zhituanY2-zhituanthresh2) && CircleY <(zhituanY2+zhituanthresh2)) ) 
					if(paperupdate) 
					{	
						Stop();
						LineState = 19;
					}	
					break;
		case 19://自动修正
					//if(!Auto_Move(CircleX,CircleY,zhituanX2,zhituanY2,zhituanthresh2))
					if(!Auto_Move(CircleX,CircleY,zhituanX4,zhituanY4,zhituanthresh4))	
					{	
						Stop();
						LineState = 20;
					}	
					break;
		case 20://抓取
					usart2_send('4');
					LineState = 21;
					while(ok!='O');
					break;
		case 21://平移回去+加找线
					if(temp_data[0]==1||temp_data[0]==2)
					{
						Stop();
						LineState = 22;
						ok=0;
					}
					Translation(1,15);
					break;
		case 22://送垃圾
					function();
					if(ok=='1')
					{
					  Stop();
						LineState = 23;
					}
					break;
		case 23://按照颜色找垃圾
					switch(laji)
					{
						case 0:
						  LineState = 24;
						  blueupdate=0;
						  Right_num=0;
						  break;//纸团
					}
					break;
		case 24://左移找蓝色(炮筒)
					Translation(0,15);
					if(Right_Judge()==4 )
					{	
						Stop();
						LineState = 25;
					}		
					break;
		
					
		case 25://送垃圾
					usart2_send('5');
					while(ok!='O');
					LineState = 26;
					break;
		case 26: //恢复初始状态
					usart2_send('0');
					while(ok!='O');	
					LineState = 27;
					break;
		case 27://平移找线
					if(ok=='6')
					{
						//Stop();
						LineState = 28;
						ok=0;
					}
					Translation(1,10);
					break;
		case 28://灰度找线
					if(temp_data[0]==1||temp_data[0]==2)
					{
					  Stop();
						LineState = 29;
						ok=0;
					}	
					Translation(1,10);
					break;
		case 29:
					function_back();
					
					break;
					
	}
}*/
/*
抓取垃圾调整(右边) 
*/

u8 Auto_Move(char INX,char INY,char X,char Y,char Thre)
{
	//X方向
	if(INY > Y+Thre)
		Translation(0,2);
	else if(INY < Y-Thre)
		Translation(1,2);
	//Y方向
	else if(INX > X+Thre)
		StraightBack(0,2);
	else if(INX < X-Thre)
		StraightBack(1,2);
	else
	{
		Stop();
		return 0;
	}
	return 1;
}
////openmv自动调节
//u8 Auto_Move2(char INX,char INY,char X,char Y,char Thre)
//{
//	
//	//X方向
//	if(INY > Y+Thre)
//		Translation(0,4);
//	else if(INY < Y-Thre)
//		Translation(1,4);
//	//Y方向
//	else if(INX > X+Thre)
//		StraightBack(1,4);
//	else if(INX < X-Thre)
//		StraightBack(0,4);
//	else
//	{
//		Stop();
//		return 0;
//	}
//	return 1;
//}
/*
送垃圾平移调整(右边)
*/
//u8 Auto_Move2(char INX,char INY,char X,char Y,char Thre)
//{
//	
//	//X方向
//	if(INX > X+Thre)
//		Translation(1,5);
//	else if(INX < X-Thre)
//		Translation(0,5);
//	//Y方向
//	else if(INY > Y+Thre)
//		StraightBack(1,5);
//	else if(INY < Y-Thre)
//		StraightBack(0,5);
//	else
//	{
//		Stop();
//		return 0;
//	}
//	return 1;
//}

char Right_num=0;
u8 Right_Judge(void)
{

	switch(Right_num)
	{
		case 0:
			while(ok!='3')
				{ok=0;}
			Right_num=1;
			break;
//		case 1:
//			while(ok!='3');
//			while(ok!='2')
//			{ok=0;}
//			Right_num=2;
//			break;
		case 1:
			while(ok!='2');
			while(ok!='3')
			{ok=0;}
			Right_num=2;
			break;
		case 2:
			while(ok!='2');
			while(ok!='3')
				{ok=0;}
			Right_num=3;
			break;
		case 3:
			while(ok!='2');
			while(ok!='3')
				{ok=0;}
			Right_num=4;
			break;
		case 4:
			
			Right_num=4;
		  Right_num=0;
			break;
		default:
			break;
	}
	
	return Right_num;
}











