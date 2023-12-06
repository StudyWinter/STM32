#include "stm32f10x.h"                  // Device header
#include "PWM.h"


// 舵机模块
void Servo_Init(void)
{
	PWM_Init();   // 初始化PWM模块
}



// 设置舵机角度。即改CCR的值
void Servo_SetAngle(float angle)
{
	// 0°   500
	// 180  2500
	PWM_SetCompare2(angle / 180 * 2000 + 500);
}
