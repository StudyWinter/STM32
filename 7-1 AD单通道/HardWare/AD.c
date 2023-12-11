#include "stm32f10x.h"                  // Device header

// AD初始化函数
void AD_Init(void)
{
	// 1开启RCC时钟，ADC、GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);			// 72M / 6 = 12M
	
	// 2配置GPIO模拟输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // 模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3配置多路开关，选择规则组的输入通道，指定通道/规则组序列器的次序/通道采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// 4配置ADC转换器
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					// 右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		// 外部触发选择，不使用外部触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						// 工作模式(独立/双ADC)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						// 单次/连续转换
	ADC_InitStructure.ADC_NbrOfChannel = 1;									// 通道数目
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							// 扫描/非扫描转换
	// 单次转换非扫描模式
	// 单次转换/连续转换  扫描/非扫描
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 5开关控制
	ADC_Cmd(ADC1, ENABLE);
	
	// 6校准
	ADC_ResetCalibration(ADC1);								// 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);		// 等待复位校准完成
	ADC_StartCalibration(ADC1);								// 开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);			// 等待校准完成
}

// 获取转换结果
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  				// 软件触发转换
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	// 等待转换完成
	return ADC_GetConversionValue(ADC1);					// 返回转换值
}
