#include "stm32f10x.h"                  // Device header

// PWM初始化函数
void PWM_Init(void)
{
	// 1RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);
	
	// 2配置时基单元(预分频器、自动重装器、计数模式等)
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 时钟分频，影响不大
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式，向上计数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复计数器的值
	// 关键参数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				// ARR自动重装器的值 两个合起来计数0.1秒
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				// PSC预分频器的值，72分频，得到100k计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	// 3配置输出比较单元(CCR的值、输出比较模式、极性选择、输出使能)
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);               					 // 先初始化，后面再按需修改
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;     					 // 输出比较模式,PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                // 输出比较极性，高级性，极性不反转，有效电平是高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;            // 输出状态输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;            							 // CCR的值w为50，先设置为0，后面变化
	// 这个函数的选择按照GPIO口需求来，PA1口对应的是第二个输出比较通道
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	// 输入PWM信号要求：周期为20ms(50Hz)，高电平宽度为0.5ms~2.5ms,占空比[2.5%, 12.5%]
	/**
	PWM分辨率：	Reso = 1 / (ARR + 1)
	PWM占空比：	Duty = CCR / (ARR + 1)
	PWM频率：	Freq = CK_PSC(72M) / (PSC + 1) / (ARR + 1)
	PSC = 72 - 1
	ARR = 20000 - 1
	CCR的范围是[500, 2500]
	*/
	
	// 4配置GPIO(初始化为复用推挽输出，参考引脚定义表)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				// 复用推挽输出。定时器控制引脚，输出控制权转移给片上外设
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 5运行控制，启动计数器
	TIM_Cmd(TIM2, ENABLE);
}

// 更改通道2的CCR值
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}
