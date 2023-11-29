#include "stm32f10x.h"                  // Device header


int main()
{
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// 配置端口模型
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// 设置端口的高低电平
	
	// 低电平点亮
//	GPIO_SetBits(GPIOC, GPIO_Pin_13);       // 高电平  灭
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);       // 低电平  亮
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	while (1)
	{
		
	
	}

}
