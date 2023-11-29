#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main()
{
	// 1使用RCC开启GPIO的时钟，点亮PA0口的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2使用GPIO_Init函数初始化GPIO[推挽输出50MHz]
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;			// GPIOA的0号外设
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while (1)
	{
		// 3使用GPIO的输入输出函数控制GPIO
//		GPIO_Write(GPIOA,  ~0x0001);   // 0000 0000 0000 0001  低电平点亮
//		Delay_ms(500);
//    
//		GPIO_Write(GPIOA,  ~0x0002);   // 0000 0000 0000 0010  低电平点亮
//		Delay_ms(500);
//    
//		GPIO_Write(GPIOA,  ~0x0004);   // 0000 0000 0000 0100  低电平点亮
//		Delay_ms(500);
//		
//		GPIO_Write(GPIOA,  ~0x0008);   // 0000 0000 0000 1000  低电平点亮
//		Delay_ms(500);
//		
//		GPIO_Write(GPIOA,  ~0x0010);   // 0000 0000 0001 0000  低电平点亮
//		Delay_ms(500);
//		
//		GPIO_Write(GPIOA,  ~0x0020);   // 0000 0000 0010 0000  低电平点亮
//		Delay_ms(500);
//		
//		GPIO_Write(GPIOA,  ~0x0040);   // 0000 0000 0100 0000  低电平点亮
//		Delay_ms(500);
//		
//		GPIO_Write(GPIOA,  ~0x0080);   // 0000 0000 1000 0000  低电平点亮
//		Delay_ms(500);
		
		
		for (unsigned int i = 0; i < 8; i++)
		{
			GPIO_Write(GPIOA, ~(0x0001 << i));
			Delay_ms(500);
		}
			
		
	}

}
