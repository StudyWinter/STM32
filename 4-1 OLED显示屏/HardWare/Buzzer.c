#include "stm32f10x.h"                  // Device header

// 蜂鸣器初始化
void Buzeer_Init(void)
{
	// 开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 配置端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// 推挽输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 初始化不叫状态
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

// 蜂鸣器发声
void Buzeer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

// 蜂鸣器不发声
void Buzeer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

// 蜂鸣器反转
void Buzeer_Turn(void)
{
	// 读取当前端口输出状态，如果输出是0，就置1；否则置0
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	}
}
