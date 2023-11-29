#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main()
{
	// 1使用RCC开启GPIO的时钟，点亮PB12口的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 2使用GPIO_Init函数初始化GPIO[推挽输出50MHz]
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			// GPIOB的12号外设
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while (1)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(100);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(100);
		
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(500);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(500);
	}
}
