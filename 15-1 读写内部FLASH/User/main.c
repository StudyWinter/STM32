#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Store.h"
#include "KEY.h"

uint8_t keyNum;
int main()
{
	OLED_Init();								// 初始化OLED
	KEY_Init();
	Store_Init();
	OLED_ShowString(1, 1, "Flag:");
	OLED_ShowString(2, 1, "Data:");
	
	while (1)
	{
		keyNum = KEY_GetNum();
		if (keyNum == 1)
		{
			store_Data[1]++;
			store_Data[2] += 2;
			store_Data[3] += 3;
			store_Data[4] += 4;
			Store_Save();				// 保存数据
		}
		if (keyNum == 2)
		{
			Store_Clear();
		}
		
		OLED_ShowHexNum(1, 6, store_Data[0], 4);			// 标志位
		OLED_ShowHexNum(3, 1, store_Data[1], 4);
		OLED_ShowHexNum(3, 6, store_Data[2], 4);
		OLED_ShowHexNum(4, 1, store_Data[3], 4);
		OLED_ShowHexNum(4, 6, store_Data[4], 4);
	}
}
