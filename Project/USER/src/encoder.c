#include "headfile.h"

int16 temp_right_pluse = 0;

//������������
void encoderValue_get(void)
{
	temp_right_pluse = ctimer_count_read(SPEEDR_PLUSE);

	//����������
	ctimer_count_clean(SPEEDR_PLUSE);

	//�ɼ�������Ϣ
	if (1 == SPEEDR_DIR)           //�ұߵ��
	{
		temp_right_pluse = -temp_right_pluse;   //��ת
	}
	else
	{
		temp_right_pluse = temp_right_pluse;  //��ת
	}
}