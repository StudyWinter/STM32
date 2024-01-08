#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main()
{
	OLED_Init();								// 初始化OLED
	
	OLED_ShowString(1, 1, "SYSTEM:");
	OLED_ShowNum(1, 8, SystemCoreClock, 8);		// 72000000
	
	while (1)
	{
		OLED_ShowString(2, 1, "Running");		// 闪烁周期是1s
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
}
