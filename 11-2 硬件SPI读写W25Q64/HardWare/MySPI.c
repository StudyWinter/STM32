#include "stm32f10x.h"                  // Device header

// 从机选择，输出引脚，即写SS的引脚
void MySPI_W_SS(uint8_t bitValue)
{
	// CS对应PA4
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)bitValue);
}

// 初始化
void MySPI_Init(void)
{
	// 1开启SPI/GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// 开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);		// 开启SPI1的时钟

	// 2初始化GPIO口，SCK和MOSI是由硬件外设控制的输出信号，配置成复用推挽输出，
	// MISO是硬件控制的输入信号，上拉输入；ss引脚通用推挽输出
	// SS从机选择
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				// 通用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// SCK和MOSI复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					// 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// MISO上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					// 上拉输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3配置SPI外设
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 双线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// SPI的模型，主/从模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// 8/16数据帧
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// SPI模式0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// 软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;// SCK时钟的频率72M/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// 高/低位先行
	SPI_InitStructure. SPI_CRCPolynomial =	7;							// CRC校验
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// 4开关控制
	SPI_Cmd(SPI1, ENABLE);
	MySPI_W_SS(1);														// SS置为高电平
}

// 起始信号
void MySPI_Start(void)
{
	MySPI_W_SS(0);					// SS置为低电平
}

// 终止信号
void MySPI_Stop(void)
{
	MySPI_W_SS(1);					// SS置为高电平
}

// 交换一个字节，模式0，参数是主机要交换的数据，返回值是从机要交换的数据
// （1）等待TXE为1；（2）写入发送的数据到TDR；（3）等待TXNE为1；（4）读取RDR接收的数据。
uint8_t MySPI_SwapByte(uint8_t byteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);			// 等待TXE为1，不用手动清除
	SPI_I2S_SendData(SPI1, byteSend);										// 写入发送的数据到TDR
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);			// 等待TXNE为1，不用手动清除
	return SPI_I2S_ReceiveData(SPI1);										// 读取RDR接收的数据
}
