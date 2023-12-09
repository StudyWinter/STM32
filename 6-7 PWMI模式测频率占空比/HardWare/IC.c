#include "stm32f10x.h"                  // Device header


// 输入捕获初始化
void IC_Init(void)
{
	// 1RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2GPIO初始化（输入模式，上拉/浮空）
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;         // 上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// 选择内部时钟
	TIM_InternalClockConfig(TIM3);
	
	// 3配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;                    	// PSC预分频器的值，72分频，通过PSC来调节
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;                     	// ARR自动重装器的值 两个合起来计数1秒，固定
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;      	// 向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			// 不分频
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;                	// 重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// 4配置输入捕获单元，修改成两个通道同时捕获同一个引脚的模式
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;					// 选择通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;								// 输入捕获滤波器，数越大，滤波效果越好
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 		// 上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;				// 不分频，触发信号分频器，不分频就是每次都有效
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;		// 选择触发信号从哪个引脚输入，直连通道/交叉通道
	// 法2，直接把另一个通道置成相反的配置，通道2/交叉/下降沿
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);							// 配置PWMI模式
	
	// 法1
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;					// 选择通道1
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;								// 输入捕获滤波器，数越大，滤波效果越好
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; 		// 下降沿触发
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;				// 不分频，触发信号分频器，不分频就是每次都有效
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;	// 选择触发信号从哪个引脚输入，交叉通道
//	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	
	// 5选择从模式的触发源(TI1FP1)
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	
	// 6选择触发之后的操作(从模式reset操作)
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	// 7开启定时器
	TIM_Cmd(TIM3, ENABLE);
}

// 获得频率函数
uint32_t IC_GetFreq(void)
{
	// fx = fc / N
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

// 获取占空比函数
uint32_t IC_GetDuty(void)
{
	// CCR2 / CCR1
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
