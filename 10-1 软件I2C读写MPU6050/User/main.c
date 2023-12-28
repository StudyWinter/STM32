#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

int16_t AX, AY, AZ, GX, GY, GZ;

int main()
{
	OLED_Init();								// 初始化OLED
	MPU6050_Init();
	OLED_ShowString(1, 1, "ID:");
	OLED_ShowHexNum(1, 4, MPU6050_GetID(), 6);
	
	while (1)
	{
		
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GX, 5);
		OLED_ShowSignedNum(4, 8, GX, 5);
	}
}
