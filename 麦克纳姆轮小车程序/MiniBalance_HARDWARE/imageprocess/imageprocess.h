#ifndef __IMAGEPROCESS_H
#define __IMAGEPROCESS_H

#include "sys.h"

void Imageprocee_Line(void);
u8 Auto_Move(char INX,char INY,char X,char Y,char Thre);
u8 Auto_Move2(char INX,char INY,char X,char Y,char Thre);
u8 Right_Judge(void);
extern char Right_num;
extern char LineState;
#endif

