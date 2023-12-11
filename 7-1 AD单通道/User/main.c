#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float voltage;

int main()
{
	OLED_Init();								// 初始化OLED
	AD_Init();
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Voltage:0.00v");		// 将转换结果换算成电压值
	
	
	while (1)
	{
		ADValue = AD_GetValue();
		voltage = (float)ADValue / 4095 * 3.3;						// 实际电压值
		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(2, 9, voltage, 1);								// 显示整数部分
		OLED_ShowNum(2, 11, (uint16_t)(voltage * 100) % 100, 2);	// 显示小数
		Delay_ms(100);
	}
}
