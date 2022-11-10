#ifndef __LINE_H
#define __LINE_H

#include "sys.h"

void function(int error);
void Stop(void);
void Translation(char dir,char speed);
void StraightBack(char dir,char speed);
void function_back(void);
extern u16 temp_data[10];       //数据缓存区
extern u16 MaxError,MinError;
extern int error ;         //偏差值
extern float P,I ,D ;
#endif
