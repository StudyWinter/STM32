#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "KEY.h"

int main()
{
	OLED_Init();								// 初始化OLED
	KEY_Init();
	OLED_ShowString(1, 1, "WWDG TEST");
	// 区分窗口看门狗的复位和普通复位
	// 窗口看门狗导致的
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "WWDG RST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "        ");
		Delay_ms(100);
		
		// 清除标志位
		RCC_ClearFlag();
	}
	else
	{
		OLED_ShowString(3, 1, "SET");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	// 初始化窗口看门狗
	// 1开启窗口看门狗APB1的时钟；
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// 2配置各个寄存器（预分频、窗口值），窗口看门狗没有写保护。窗口看门狗没有重装值，通过直接写入寄存器实现的
	// 超时时间50ms = 1/36m * 4096 * (2^3=8) * (54 + 1)
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	// 窗口时间30ms = 1/36m * 4096 * (2 ^ 3 = 8)  * (54 -  21)
	WWDG_SetWindowValue(21 + 0x40);	// W5~W0，W6位也要设置1
	WWDG_Enable(54 + 0x40);			// T5~T0，T6位也要设置1，第一次喂狗
	
	// 3写入控制寄存器CR（包括看门狗使能位、计数器溢出标志位和计数器有效位）；
	
	while (1)
	{
		KEY_GetNum();				// 按住按键不放，程序阻塞超时喂狗，窗口看门狗复位
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		
		// 过快喂狗
//		Delay_ms(30);
		// 超时喂狗
//		Delay_ms(50);
		
		// 4不断向计数器写入想要的重装值（喂狗）。喂狗的时间要在30ms~50ms
		WWDG_SetPrescaler(54 + 0x40);
	}
}
