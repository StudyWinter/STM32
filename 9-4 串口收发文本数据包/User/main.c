#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <string.h>


int main()
{
	OLED_Init();       						// 初始化
	Serial_Init();
	LED_Init();
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");

	while (1)
	{
		// 接收到数据包
		if (Serial_RxFlag == 1)
		{
			OLED_ShowString(4, 1, "                ");			// 先清除
			OLED_ShowString(4, 1, Serial_RxPacket);				// 显示接收到的数据
			
			// 如果相等，LED1灯点亮
			if (strcmp(Serial_RxPacket, "LED_ON") == 0 )
			{
				LED1_ON();
				Serial_SendString("LED_ON_OK\r\n");			   // 回传数据
				OLED_ShowString(2, 1, "                ");     // 16个空格
				OLED_ShowString(2, 1, Serial_RxPacket);
			}
			 else if (strcmp(Serial_RxPacket, "LED_OFF") == 0 )
			{
				LED1_OFF();
				Serial_SendString("LED_OFF_OK\r\n");
				OLED_ShowString(2, 1, "                ");     // 16个空格
				OLED_ShowString(2, 1, Serial_RxPacket);
			} 
			else
			{
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "                ");     // 16个空格
				OLED_ShowString(2, 1, "ERROR_COMMAND");
			}
			Serial_RxFlag = 0;
			
		}
	}
}
