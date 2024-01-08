#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;

int main()
{
	OLED_Init();								// 初始化OLED
	Serial_Init();
	OLED_ShowString(1, 1, "RxData:");
	
	while (1)
	{
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);                     // 回传
 			OLED_ShowHexNum(1, 8, RxData, 2);
			// 不需要再清除标志位了
		}
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		__WFI();			// CPU进入睡眠
//		__WFE();			// 唤醒方式不同。__WFI中断唤醒，__WFE事件唤醒
	}
}
