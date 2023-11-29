#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"

uint8_t keyNum;

int main()
{
	LED_Init();
	KEY_Init();
	
	while (1)
	{
		keyNum = KEY_GetNum();
		// 按键1按下，点亮LED1
		if (keyNum == 1)
		{
			LED1_Turn();
		}
		// 按键2按下，熄灭LED1
		if (keyNum == 2)
		{
			LED2_Turn();
		}
	}
}
