#include "stm32f10x.h"                  // Device header

// 编码器接口初始化函数
void EnCoder_Init(void)
{
	// 1开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;         		// 上拉输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 3配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;                    	// PSC预分频器的值，不分频
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;                     	// ARR自动重装器的值 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;      	// 向上计数，没有用
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			// 不分频
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;                	// 重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// 4配置输入捕获单元(只有极性和滤波器两个参数有用)
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);                               // 结构体初始化
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;					   // 通道1
	TIM_ICInitStruct.TIM_ICFilter = 0xF;							   // 滤波器
//	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;		   // 和后面重复
//	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;       // 无作用
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                 // 无作用
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	//	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;	   // 和后面重复
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	// 5配置编码器接口模式
	// TIM_ICPolarity_Rising这个通道不反向，TIM_ICPolarity_Falling这个通道反向
	// 后两个参数相反就是方向相反
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	// 6启动定时器
	TIM_Cmd(TIM3, ENABLE);
}

// 获取CNT的值
int16_t Encoder_Get(void)
{
//    return TIM_GetCounter(TIM3);
	// 读取cnt，把cnt清零的逻辑
	int16_t temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return temp;
}

