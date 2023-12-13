#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};	// 源数组
uint8_t DataB[] = {0x00, 0x00, 0x00, 0x00};	// 目的数组


int main()
{
	OLED_Init();									    // 初始化OLED
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);	// 初始化DMA
	OLED_ShowString(1, 1, "DataA:");
	OLED_ShowString(3, 1, "DataB:");
	OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);          // 显示地址
	OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);			// 显示地址
	
	// 第二行显示dataA
	OLED_ShowHexNum(2, 1, DataA[0], 2);
	OLED_ShowHexNum(2, 4, DataA[1], 2);
	OLED_ShowHexNum(2, 7, DataA[2], 2);
	OLED_ShowHexNum(2, 10, DataA[3], 2);
	
	// 第四行显示dataB
	OLED_ShowHexNum(4, 1, DataB[0], 2);
	OLED_ShowHexNum(4, 4, DataB[1], 2);
	OLED_ShowHexNum(4, 7, DataB[2], 2);
	OLED_ShowHexNum(4, 10, DataB[3], 2);
	
	while (1)
	{
		DataA[0]++;         // 数据自增
		DataA[1]++;
		DataA[2]++;
		DataA[3]++;
		// 第二行显示dataA
		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
	
		// 第四行显示dataB
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		Delay_ms(1000);
		MYDMA_Transfer();          // 转运
		
		// 第二行显示dataA
		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
	
		// 第四行显示dataB
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		Delay_ms(1000);
	}
}
