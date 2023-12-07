#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

uint8_t keyNum;
int8_t speed;

int main()
{
	OLED_Init();					// 初始化OLED
	OLED_ShowString(1, 1, "Speed:");
	Motor_Init();
	KEY_Init();
	
	while (1)
	{
		keyNum = KEY_GetNum();
		if (keyNum == 1)
		{
			speed += 20;
			if (speed >= 100)
			{
				speed = -100;
			}
		}
		Motor_Speed(speed);
		OLED_ShowSignedNum(1, 7, speed, 3);
	}
}
