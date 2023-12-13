#include "stm32f10x.h"                  // Device header

uint16_t MYDMA_Size;

// DMA初始化
void MyDMA_Init(uint32_t addrA, uint32_t addrB, uint32_t size)
{
	MYDMA_Size = size;
	// 1开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 2调用DMA_Init
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = addrA;							// 外设站点的起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		// 外设站点的数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;				// 外设站点是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = addrB;								// 存储器站点的起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				// 存储器站点的数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 存储器站点是否自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							// 传输方向，外设作为源到存储器
	DMA_InitStructure.DMA_BufferSize = size;									// 缓冲区大小，传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// 传输模式，是否自动重装，不重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;									// 软件触发还是硬件触发，软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						// 优先级
	// DM1的通道1
	DMA_Init(DMA1_Channel1 ,&DMA_InitStructure);
	
	// 3DMA_Cmd，先失能，在转运中使能
	DMA_Cmd(DMA1_Channel1, DISABLE);
}


// 传输函数
void MYDMA_Transfer(void)
{
	// 先DMA失能，再给传输计数器赋值
	DMA_Cmd(DMA1_Channel1, DISABLE);					// 失能
	DMA_SetCurrDataCounter(DMA1_Channel1, MYDMA_Size);	// 赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);						// 使能
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	// 等待转运完成，没有完成一直等待
	DMA_ClearFlag(DMA1_FLAG_TC1);						// 清除标志位
}
