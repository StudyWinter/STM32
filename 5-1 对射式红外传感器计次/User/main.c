#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main()
{
	CountSensor_Init();									// 初始化计数配置
	OLED_Init();										// 初始化OLED
	OLED_ShowString(1, 1, "Count:");   					// 显示字符串
	while (1)
	{
		OLED_ShowNum(1, 7, CountSensor_GetCount(), 5);	// 显示计数
	}
}
