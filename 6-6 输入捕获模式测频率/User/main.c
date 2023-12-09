#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
 
 
int main()
{
	OLED_Init();					// 初始化OLED
	PWM_Init();						// PWM初始化
	IC_Init();						// IC初始化
	
	OLED_ShowString(1, 1, "Freq:00000Hz");   	// 显示字符串
	
	PWM_SetPrescaler(720 - 1);		// Freq = 72M / (PSC + 1) / (ARR + 1 = 100) = 1000Hz
	PWM_SetCompare1(50);			// 设置CCR的值，占空比Duty = CCR / (ARR + 1 = 100) = 50%
	// 上面这里输出信号是1000Hz的PWM波
	
	while (1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
	}
}