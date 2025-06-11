#include "headfile.h"

int16 temp_right_pluse = 0;

//读编码器数据
void encoderValue_get(void)
{
	temp_right_pluse = ctimer_count_read(SPEEDR_PLUSE);

	//计数器清零
	ctimer_count_clean(SPEEDR_PLUSE);

	//采集方向信息
	if (1 == SPEEDR_DIR)           //右边电机
	{
		temp_right_pluse = -temp_right_pluse;   //正转
	}
	else
	{
		temp_right_pluse = temp_right_pluse;  //反转
	}
}