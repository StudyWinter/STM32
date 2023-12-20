#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h" 

uint8_t keyNum;

int main()
{
	OLED_Init();       						// 初始化
	Serial_Init();
	KEY_Init();
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	// 发送的数据
	Serial_TxPacket[0] = 0x01;
	Serial_TxPacket[1] = 0x02;
	Serial_TxPacket[2] = 0x03;
	Serial_TxPacket[3] = 0x04;

	while (1)
	{
		keyNum = KEY_GetNum();
		if (keyNum == 1)
		{
			Serial_TxPacket[0]++;
			Serial_TxPacket[1]++;
			Serial_TxPacket[2]++;
			Serial_TxPacket[3]++;
			// 发送
			Serial_SendPacket();		
			// 显示
			OLED_ShowHexNum(2, 1, Serial_TxPacket[0], 2); 
			OLED_ShowHexNum(2, 4, Serial_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, Serial_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, Serial_TxPacket[3], 2);
		}
		
		// 收到数据包，显示
		if (Serial_GetRxFlag() == 1) {
			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2); 
			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		}
	}
}
