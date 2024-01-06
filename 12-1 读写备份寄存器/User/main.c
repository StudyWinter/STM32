#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

// 写入和读出数组
uint16_t arrayWrite[] = {0x1234, 0x5678};
uint16_t arrayRead[2];

uint8_t keyNum;


int main()
{
	OLED_Init();								// 初始化OLED
	KEY_Init();									// 初始化按键
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	// 1初始化：分两步
	// (1)设置RCC_APB1ENR的PWREN和BKPEN，使能PWR和BKP时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);			// 开启PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);			// 开启BKP的时钟
	// (2)设置PWR_CR的DBP，使能对BKP和RTC的访问
	PWR_BackupAccessCmd(ENABLE);
	
	while (1)
	{
		keyNum = KEY_GetNum();
		if (keyNum == 1)
		{
			arrayWrite[0]++;
			arrayWrite[1]++;
			// 写数据
			BKP_WriteBackupRegister(BKP_DR1, arrayWrite[0]);
			BKP_WriteBackupRegister(BKP_DR2, arrayWrite[1]);
			// 显示
			OLED_ShowHexNum(1, 3, arrayWrite[0], 4);
			OLED_ShowHexNum(1, 8, arrayWrite[1], 4);
		}
		// 读数据
		arrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		arrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		// 显示
		OLED_ShowHexNum(2, 3, arrayRead[0], 4);
		OLED_ShowHexNum(2, 8, arrayRead[1], 4);
		
	}
}
