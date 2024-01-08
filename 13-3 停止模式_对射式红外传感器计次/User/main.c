#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main()
{
	CountSensor_Init();									// 初始化计数配置
	OLED_Init();										// 初始化OLED
	OLED_ShowString(1, 1, "Count:");   					// 显示字符串
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	// 开启时钟
	while (1)
	{
		OLED_ShowNum(1, 7, CountSensor_GetCount(), 5);	// 显示计数
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		// 进入停止模式
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
		SystemInit();			// 重新配置主频
	}
}
