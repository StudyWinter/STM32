#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

// AD初始化函数
void AD_Init(void)
{
	// 1开启RCC时钟，ADC、GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);			// 72M / 6 = 12M
	
	// 2配置GPIO模拟输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // 模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3配置多路开关，选择规则组的输入通道，指定通道/规则组序列器的次序/通道采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	// 4配置ADC转换器
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					// 右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		// 外部触发选择，不使用外部触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						// 工作模式(独立/双ADC)
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						// 连续转换
	ADC_InitStructure.ADC_NbrOfChannel = 4;									// 通道数目
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;							// 扫描转换
	// 单次转换非扫描模式
	// 单次转换/连续转换  扫描/非扫描
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ADC扫描模式+DMA数据搬运
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);			// 外设站点的起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// 外设站点的数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 外设站点是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;					// 存储器站点的起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// 存储器站点的数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 存储器站点是否自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							// 传输方向，外设作为源到存储器
	DMA_InitStructure.DMA_BufferSize = 4;										// 缓冲区大小，传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// 传输模式，是否自动重装，不重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								// 软件触发还是硬件触发，硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						// 优先级
	// DM1的通道1
	DMA_Init(DMA1_Channel1 ,&DMA_InitStructure);
	
	// DMA_Cmd，先失能，在转运中使能
	DMA_Cmd(DMA1_Channel1, DISABLE);
	// 开启ADC到DMA的输出
	ADC_DMACmd(ADC1, ENABLE);
	
	// 5开关控制
	ADC_Cmd(ADC1, ENABLE);
	
	// 6校准
	ADC_ResetCalibration(ADC1);								// 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);		// 等待复位校准完成
	ADC_StartCalibration(ADC1);								// 开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);			// 等待校准完成
}

// 获取转换结果
void AD_GetValue(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);            		// DMA先失能
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);    		// 赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);						// 使能
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);             // 触发一次
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	// 等待转运完成，没有完成一直等待
	DMA_ClearFlag(DMA1_FLAG_TC1);           			// 清除标志位
	
	
}
