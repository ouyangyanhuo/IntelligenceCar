#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "headfile.h"

//定义脉冲引脚
#define SPEEDR_PLUSE   CTIM3_P04
//定义方向引脚
#define SPEEDR_DIR     P53

void encoderValue_get(void);

#endif
