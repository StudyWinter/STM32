#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint8_t MID;
uint16_t DID;

uint8_t arrayWrite[] = {0xA1, 0xB2, 0xC3, 0xD4};
uint8_t arrayRead[4];

int main()
{
	OLED_Init();								// 初始化OLED
	W25Q64_Init();
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	
	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	
	W25Q64_SectorErase(0x000000);							// 擦除扇区
	W25Q64_PageProgram(0x000000, arrayWrite, 4);			// 写入
	
	W25Q64_ReadData(0x000000, arrayRead, 4);				// 读数据
	
	// 测试写
	OLED_ShowHexNum(2, 3, arrayWrite[0], 2);
	OLED_ShowHexNum(2, 6, arrayWrite[1], 2);
	OLED_ShowHexNum(2, 9, arrayWrite[2], 2);
	OLED_ShowHexNum(2, 12, arrayWrite[3], 2);
	// 测试读
	OLED_ShowHexNum(3, 3, arrayRead[0], 2);
	OLED_ShowHexNum(3, 6, arrayRead[1], 2);
	OLED_ShowHexNum(3, 9, arrayRead[2], 2);
	OLED_ShowHexNum(3, 12, arrayRead[3], 2);
	
	while (1)
	{
		
	}
}
