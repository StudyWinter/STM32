#include "stm32f10x.h"                  // Device header

// 定时器初始化
void Timer_Init(void)
{
	// 第一步RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 第二步选择时基单元的时钟源（外部时钟）
	// 不分频，不反向(高电平/上升沿有效)，外部触发滤波器：不用
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
	
	// 第三步配置时基单元(预分频器、自动重装器、计数模式等)
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 时钟分频，影响不大
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式，向上计数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复计数器的值
	// 关键参数，如果想定时1S
//	CK_CNT_OV = CK_CNT / (ARR + 1)
//			  = CK_PSC / (PSC + 1) / (ARR + 1)
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;					// ARR自动重装器的值，0-9
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;				// PSC预分频器的值，不分频
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                           // 防止复位从1开始
	// 第四步配置输出中断控制，允许更新中断输出到NVIC
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 第五步配置NVIC，在NVIC中打开定时器中断通道，并分配一个优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        			// 分组
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;           			// 定时器2的通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;			// 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					// 响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;					// 使能
	NVIC_Init(&NVIC_InitStruct);
	
	// 第六步启动计数器
	TIM_Cmd(TIM2, ENABLE);
}

// 获取计数器的值
uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}


// 中断函数
//void TIM2_IRQHandler(void)
//{
//	// 检测中断标志位，确保是设置的中断源触发的这个函数
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//	
//		// 清除中断标志位
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}

