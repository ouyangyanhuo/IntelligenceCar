#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "headfile.h"

//定义脉冲引脚
#define SPEEDR_PLUSE   CTIM0_P34
//定义方向引脚
#define SPEEDR_DIR     P35

void encoderValue_get(void);
#endif
