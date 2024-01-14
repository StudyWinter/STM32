#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "KEY.h"

int main()
{
	OLED_Init();								// 初始化OLED
	KEY_Init();
	OLED_ShowString(1, 1, "IWDG TEST");
	// 区分独立看门狗的复位和普通复位
	// 独立看门狗导致的
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "IWDG RST");
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
	
	// 初始化独立看门狗
	// 1开启LSI时钟(不需要手动开启)
	// 2解除写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	// 3写入预分频器和重装载寄存器
	// 设置超时时间1000ms，喂狗时间间隔不能超过1000ms
	// 1000ms = 0.025ms * 16 * (2499 + 1)
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499);
	
	IWDG_ReloadCounter();						// 先喂狗
	
	// 4启动独立看门狗
	IWDG_Enable();
	
	
	while (1)
	{
		KEY_GetNum();				// 按住按键不放，程序阻塞超时喂狗，独立看门狗复位
		// 5主程序不断喂狗
		IWDG_ReloadCounter();
		
		OLED_ShowString(4, 1, "FEED");
//		Delay_ms(800);				// 不超时
//		Delay_ms(1000);				// 临界值
//		Delay_ms(1200);				// 超时喂狗，显示看门狗复位
		Delay_ms(600);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(200);
	}
}
