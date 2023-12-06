#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"
#include "Servo.h"

uint8_t keyNum;
float angle;


int main()
{
	OLED_Init();       						// 初始化
	OLED_ShowString(1, 1, "angle:");   		// 显示字符串
	Servo_Init();
	KEY_Init();

	while (1)
	{
		keyNum = KEY_GetNum();				// 读按键
		if (keyNum == 1)
		{
			angle += 30;
			if (angle > 180)
			{
				angle = 0;
			}
		}
		Servo_SetAngle(angle);				// 设置舵机角度(设置CCR的值)
		OLED_ShowNum(1, 7, angle, 3);
		
	}
}
