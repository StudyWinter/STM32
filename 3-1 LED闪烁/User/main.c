#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main()
{
	// 1使用RCC开启GPIO的时钟，点亮PA0口的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2使用GPIO_Init函数初始化GPIO[推挽输出50MHz]
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			// GPIOA的0号外设
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3使用GPIO的输入输出函数控制GPIO
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);					// 将GPIOA的0号端口设置高电平（不亮）
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);					// 将GPIOA的0号端口设置低电平（亮）
	
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);			// 高电平（不亮）
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);		// 低电平（亮）
	
	// 强转
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);    	// 点亮
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);    	// 熄灭
	
	while (1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);			// 高电平（不亮）
		Delay_ms(500);										// 延迟
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);		// 低电平（亮）
		Delay_ms(500);										// 延迟
	}

}
