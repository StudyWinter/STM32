#include "stm32f10x.h"                  // Device header

// 从机选择，输出引脚，即写SS的引脚
void MySPI_W_SS(uint8_t bitValue)
{
	// CS对应PA4
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)bitValue);
}

// 写时钟函数CLK
void MySPI_W_SCK(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)bitValue);
}

// 主机输出引脚MOSI
void MySPI_W_MOSI(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)bitValue);
}


// 主机输入引脚MISO
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}



// 初始化
void MySPI_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// 开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					// 输出引脚推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						// PA6输入引脚为上拉（浮空）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 置初始化之后引脚的默认电平
	MySPI_W_SS(1);					// SS置为高电平，默认不选择从机
	MySPI_W_SCK(0);					// 使用SPI的模式0
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
uint8_t MySPI_SwapByte(uint8_t byteSend)
{
	uint8_t i, byteReceive = 0x00;
	
//	// 先SS下降沿或者SCK下降沿，再数据移出；再SCK上升沿，再移入数据
//	MySPI_W_MOSI(byteSend & 0x80);					// 主机写，主机移数据
//	MySPI_W_SCK(1);									// SCK上升沿
//	// 主机读数据，存最高位
//	if (MySPI_R_MISO() == 1)
//	{
//		byteReceive |= 0x80;
//	}
//	MySPI_W_SCK(0);									// SCK下降沿，主机移出下一位
//	MySPI_W_MOSI(byteSend & (0x40));				// 主机写，主机移数据
	
	
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_MOSI(byteSend & 0x80);
//		byteSend <<= 1;
//		MySPI_W_SCK(1);
//		if (MySPI_R_MISO() == 1)
//		{
//			byteSend |= 0x01;
//		}
//		MySPI_W_SCK(0);	
//	}
//	return byteSend;
	
	for (i = 0; i < 8; i++)
	{
		MySPI_W_MOSI(byteSend & (0x80 >> i));
		MySPI_W_SCK(1);
		if (MySPI_R_MISO() == 1)
		{
			byteReceive |= (0x80 >> i);
		}
		MySPI_W_SCK(0);	
	}
	return byteReceive;
}

// 模式1
//uint8_t MySPI_SwapByte(uint8_t byteSend)
//{
//	uint8_t i, byteReceive = 0x00;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_SCK(1);
//		MySPI_W_MOSI(byteSend & (0x80 >> i));
//		MySPI_W_SCK(0);	
//		if (MySPI_R_MISO() == 1)
//		{
//			byteReceive |= (0x80 >> i);
//		}
//	}
//	return byteReceive;
//}

// 模式3
//uint8_t MySPI_SwapByte(uint8_t byteSend)
//{
//	uint8_t i, byteReceive = 0x00;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_SCK(0);
//		MySPI_W_MOSI(byteSend & (0x80 >> i));
//		MySPI_W_SCK(1);	
//		if (MySPI_R_MISO() == 1)
//		{
//			byteReceive |= (0x80 >> i);
//		}
//	}
//	return byteReceive;
//}

// 模式2
//uint8_t MySPI_SwapByte(uint8_t byteSend)
//{
//	uint8_t i, byteReceive = 0x00;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_MOSI(byteSend & (0x80 >> i));
//		MySPI_W_SCK(0);
//		if (MySPI_R_MISO() == 1)
//		{
//			byteReceive |= (0x80 >> i);
//		}
//		MySPI_W_SCK(1);	
//	}
//	return byteReceive;
//}
