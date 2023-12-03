#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

// 中断函数
void TIM2_IRQHandler(void);

uint16_t num;


int main()
{
	OLED_Init();								// 初始化OLED
	Timer_Init();								// 初始化定时器
	OLED_ShowString(1, 1, "Num:");   			// 显示字符串
	OLED_ShowString(2, 1, "Cnt:");   			// 显示字符串
	
	while (1)
	{
		OLED_ShowNum(1, 5, num, 5);   	    			// 显示计数
		OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5);    // 显示CNT计数器，最大值9999
	}
}


// 中断函数
void TIM2_IRQHandler(void)
{
	// 检测中断标志位，确保是设置的中断源触发的这个函数
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		// 中断处理
		num++;
		// 清除中断标志位
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

