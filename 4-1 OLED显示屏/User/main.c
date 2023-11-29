#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main()
{
	OLED_Init();								// 初始化OLED
	OLED_ShowChar(1, 1, 'A');					// 显示字符'A'
	OLED_ShowString(1, 3, "HelloWorld!");   	// 显示字符串
	OLED_ShowNum(2, 1, 12345, 5);           	// 显示十进制12345，长的话，补0；短的话切掉高位
	OLED_ShowSignedNum(2, 7, -66, 2);       	// 显示有符号
	OLED_ShowHexNum(3, 1, 0xAA55, 4);       	// 显示16进制
	OLED_ShowBinNum(4, 1, 0xAA55, 16);      	// 显示二进制
	
//	OLED_Clear();								// 清屏
	while (1)
	{
		
	}
}
