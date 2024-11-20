///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32F12K
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/

// ********************************************************************************************************************/
// * 此中断系统仅用到TM1和TM2
// * TM1为元素处理
// * TM2为频率处理
// ********************************************************************************************************************/

#include "headfile.h"

//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF
		if(wireless_module_uart_handler != NULL)
		{
			// 该函数为函数指针
			// 再初始化无线模块的时候，设置该函数地址
			wireless_module_uart_handler(S2BUF);
		}
	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;
		//接收数据寄存器为：S4BUF;


	}
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//点亮LED
}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}

void TM0_Isr() interrupt 1
{

}                                          

extern long  Left_Adc, Right_Adc;						//横电感值
extern long  Left_Adc2, Right_Adc2;						//竖电感值
extern int16 temp_right_pluse;							//右电机反馈值

uint16 IN_island_encoder = 0, OUT_island_encoder = 5;   //出入环岛编码计数值

uint16 OUT_accelerate_encoder = 0;						//出环岛加速编码器值
uint8 out_island_accelerate = 0;						//出环岛加速标志位

uint16 GO_accelerate = 0;								//发车加速
uint8 GO_accelerate_flag = 1;

uint16 Time_outtrack_cnt = 0;							//出赛道计数
uint8 outtrack_flag = 0;								//出赛道标志

uint16 encoder_GO_accelerate = 6000, encoder_OUT_accelerate = 5000;

uint16 out_island_accelerate_flag_encoder = 0, out_island_accelerate_flag = 0;
uint8 ZHIZOU = 0;

/*TM1部分代码开始*/
void TM1_Isr() interrupt 3
{
	ADC_Final_Read_Deal();   //读取ADC值
	encoderValue_get();  //编码器数据读取

	/**********  1.停车处理-出赛道处理 *********/
	if (Left_Adc < 5 && Right_Adc2 < 5 && Left_Adc2 < 5 && Right_Adc < 5) Time_outtrack_cnt++;
	if (Time_outtrack_cnt > 80)
	{
		Time_outtrack_cnt = 0;
		outtrack_flag = 1;
	}
	/**********  1.停车处理-出赛道处理 *********/

	/************ 2.出入环岛处理 ***********/
	if(Island_t.nolonger ==0){//不再判断环
		if (Left_Adc == 100 && Right_Adc > 75 && Left_Adc2 >= 2){
			Island_t.in_cnt++;				//计数一定值认为有环岛
			if (Island_t.in_cnt >= 5){
				Island_t.in_cnt = 0;
				Island_t.into_flag = 1;		//置位环岛标志
			}
		}
	}
	//确定环岛后通过累计编码值来清空标志位，
	if (Island_t.into_flag == 1)  IN_island_encoder += temp_right_pluse;  //记到一定值了认为已经进入环岛
	if (IN_island_encoder >= ENCODER_ISLAND_INTO+2000){			//路程够了
		Island_t.into_flag = 0;				//清空环岛标志位
		IN_island_encoder = 0;
		Island_t.out_flag = 1;
		Island_t.nolonger = 1;
	}
	//和入环岛一样
	if (Island_t.out_flag == 1) OUT_island_encoder += temp_right_pluse;
	if (OUT_island_encoder >= ENCODER_ISLAND_OUT+3000){
		Island_t.out_flag = 0;
		OUT_island_encoder = 0;
		out_island_accelerate_flag = 1;		//出环标志位加速
		gpio_mode(P3_2, GPIO);				// 将P3.2设置为弱上拉
	}
	/************  2.出入环岛处理 ***********/

	/************  3.直道提速 ***********/
	if (GO_accelerate_flag == 1) GO_accelerate += temp_right_pluse;
	if (GO_accelerate >= encoder_GO_accelerate) {
		GO_accelerate = 0;
		GO_accelerate_flag = 0;			//发车回归正常
	}
	//出环岛延迟系统
	if (out_island_accelerate_flag == 1) out_island_accelerate_flag_encoder += temp_right_pluse;
	if (out_island_accelerate_flag_encoder >= 2000) {
		out_island_accelerate_flag_encoder = 0;
		out_island_accelerate_flag = 0;					//标志位置回
		out_island_accelerate = 1;						//开始加速标志位
	}

	if (out_island_accelerate == 1) OUT_accelerate_encoder += temp_right_pluse;
	if (OUT_accelerate_encoder >= 1000) ZHIZOU = 1;		//出环岛后暂时不检测电感，保持垂直出岛的稳定
	if (OUT_accelerate_encoder >= encoder_OUT_accelerate) {
		OUT_accelerate_encoder = 0;
		out_island_accelerate = 0;		//出环回归正常
	}

	/************  3.直道提速 ***********/

	//控制系统
	Motor_PWM_Final_Control();	//电机控制
	Car_Control();				//舵机控制
}


/*TM2部分代码开始*/
void TM2_Isr() interrupt 12
{
	/*


	
	*/
	//TIM2_CLEAR_FLAG;  //清除中断标志
	
}



void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}

void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //清除中断标志
	ccd_collect();	 //CCD采集数据
//	pit_callback();
}

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;