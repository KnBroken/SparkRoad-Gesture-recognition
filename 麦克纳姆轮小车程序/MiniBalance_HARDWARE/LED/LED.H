#ifndef __LED_H
#define __LED_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
//LED 端口定义
#define LED PBout(13)  
void LED_Init(void);  //初始化
void Led_Flash(u16 time);
void gpio_init(void);
//#define D5 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
//#define D6  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#endif
