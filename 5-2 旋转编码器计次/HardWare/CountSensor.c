#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint16_t CountSensor_Count;

// CountSensor初始化模块
void CountSensor_Init(void)
{
	// 配置外部中断
	// 1配置GPIO/AFIO，把时钟外设都打开
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// EXTI和NVIC的时钟都打开着不手动开启
	
	// 2配置GPIO，选择端口为输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 3配置AFIO。数据选择器输入为GPIOB的14号引脚，输出端固定连接的是EXTI14
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// 4配置EXTI，选择边沿触发方式（上升沿/下降沿/双边沿），选择响应方式（中断/事件）
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;						// 指定配置中断线14
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				// 选择中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;			// 上升沿触发，遮挡的时候触发
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;			// 下降沿触发，离开的时候触发
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// 遮挡和离开的时候触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						// 开启EXTI
	EXTI_Init(&EXTI_InitStructure);
	
	
	// 配置NVIC，给中断一个优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					// 2位抢占，2位响应，整个工程只有一种
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			// 指定中断通道，选的是14
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		// 指定抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				// 指定响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// 使能
	NVIC_Init(&NVIC_InitStructure);
}



// 中断函数，无参无返回值
void EXTI15_10_IRQHandler(void)
{
	// 先判断中断标志位，确保是设置的中断源触发的这个函数
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		// 执行中断程序
//		Delay_ms(1000);			// 消除抖动
		CountSensor_Count++;	// 计数
		// 中断程序结束后，清除中断标志位；不清除就会一直申请中断，卡死在中断函数里
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}

// 返回计数
uint16_t CountSensor_GetCount(void)
{
	return CountSensor_Count;
}
