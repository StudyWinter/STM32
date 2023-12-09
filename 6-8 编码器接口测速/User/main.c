#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "EnCoder.h"

int16_t speed;


int main()
{
	OLED_Init();								// 初始化OLED
	EnCoder_Init();
	Timer_Init();								// 初始化定时器
	OLED_ShowString(1, 1, "speed:");   			// 显示字符串
	
	while (1)
	{
		OLED_ShowSignedNum(1, 7, speed, 5);    		 // 显示CNT计数器
	}
}


// 中断函数
void TIM2_IRQHandler(void)
{
	// 检测中断标志位，确保是设置的中断源触发的这个函数
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		// 中断处理
		speed = Encoder_Get();
		// 清除中断标志
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

