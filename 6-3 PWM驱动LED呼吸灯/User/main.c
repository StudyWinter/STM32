#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint8_t i;

int main()
{
	OLED_Init();					// 初始化OLED
	PWM_Init();						// PWM初始化
	OLED_ShowString(1, 1, "Hello");
	while (1)
	{
		// 点亮
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		
		// 熄灭
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(100 - i);
			Delay_ms(10);
		}
	}
}
