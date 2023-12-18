#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main()
{
	OLED_Init();								// 初始化OLED
	Serial_Init();
	
	Serial_SendByte('A');
	uint8_t array[] = {'B', 'C', 'D'};
	Serial_SendArray(array, 3);
	
	char* str = "Hello Wolrd\r\n";
	Serial_SendString(str);
	Serial_SendNumber(456789, 6);
	
	printf("Num = %d\r\n", 666);
	
//	char str[100];
//	sprintf(str, "Num = %d\r\n", 777);
//	Serial_SendString(str);
	
	Serial_Sprintf("你好");
	
	
	while (1)
	{
		
	}
}
