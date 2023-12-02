#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "EnCoder.h"

int16_t num;

int main()
{
	EnCoder_Init();										// 初始化计数配置
	OLED_Init();										// 初始化OLED
	OLED_ShowString(1, 1, "Data:");   					// 显示字符串
	while (1)
	{
		num += EnCoder_Get();
		OLED_ShowSignedNum(1, 6, num, 5);				// 显示数据
	}
}
