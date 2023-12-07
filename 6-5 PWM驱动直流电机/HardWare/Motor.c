#include "stm32f10x.h"                  // Device header
#include "PWM.h"

// 电机初始化
void Motor_Init(void)
{
	// 还有两个角需要初始化（控制电机方向）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// 配置端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// 初始化PWM
	PWM_Init();
}

// 设置速度函数
void Motor_Speed(int8_t speed)
{
	// 正转
	if (speed >= 0)
	{
		// 设置方向
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		// 速度，RCC的值，即调节占空比
		PWM_SetCompare3(speed);
	}
	else
	{
		// 设置方向
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		// 速度，RCC的值，即调节占空比
		PWM_SetCompare3(-speed);
	}
		
}

