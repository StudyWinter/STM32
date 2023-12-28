#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// 封装SCL的写函数
void MyI2C_W_SCL(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)bitValue);
	Delay_us(10);
}

// 封装SDA的写函数
void MyI2C_W_SDA(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)bitValue);
	Delay_us(10);
}

// 封装SDA的读函数
uint8_t MyI2C_R_SDA(void)
{
	uint8_t bitValue;
	bitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return bitValue;
}


// I2C初始化
void MyI2C_Init(void)
{
	// 软件I2C只需要用GPIO的读写就可以了，不看库函数
	// 1把SCL和SDA初始化成开漏输出模式
	// 2把SCL和SDA置高电平
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		// 开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;			// 将PA1和PA2引脚初始化为开漏输出（输出低电平+浮空输入）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);				// 设置PA1和PA2引脚为高电平
}

// 起始条件
void MyI2C_Start(void)
{
	// 确保SCL和SDA都释放，先拉低SDA，再拉低SCL
	MyI2C_W_SDA(1);			// 释放SDA（在前）
	MyI2C_W_SCL(1);			// 释放SCL
	MyI2C_W_SDA(0);			// 先拉低SDA
	MyI2C_W_SCL(0);			// 再拉低SCL
}


// 终止条件
void MyI2C_Stop(void)
{
	// 先拉低SDA，再释放SCL，再释放SDA
	MyI2C_W_SDA(0);			// 先拉低SDA
	MyI2C_W_SCL(1);			// 释放SCL
	MyI2C_W_SDA(1);			// 释放SDA
}

// 发送一个字节
void MyI2C_SendByte(uint8_t byte)
{
	// 发送一个字节开始时，SCL是低电平
	// 趁SCL是低电平，先把byte的最高位放在SDA线上
//	MyI2C_W_SDA(byte & 0x80);	// 取决byte的最高位
//	MyI2C_W_SCL(1);				// 释放SCL
//	MyI2C_W_SCL(0);				// 拉低SCL
//	
//	MyI2C_W_SDA(byte & 0x40);	// 取决byte的次高位
//	MyI2C_W_SCL(1);				// 释放SCL
//	MyI2C_W_SCL(0);				// 拉低SCL
	
	uint8_t i = 0;
	for (i = 0; i < 8; i++)
	{
		MyI2C_W_SDA(byte & (0x80 >> i));	// 取决byte的i高位
		MyI2C_W_SCL(1);						// 释放SCL
		MyI2C_W_SCL(0);						// 拉低SCL
	}
}


// 接收一个字节
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t byte = 0x00,i = 0;
	// SCL低电平，SDA写数据，SCL高电平，读SDA的数据
	MyI2C_W_SDA(1);						// 释放SDA
//	MyI2C_W_SCL(1);						// 释放SCL
//	// 读数据
//	if (MyI2C_R_SDA() == 1)
//	{
//		byte |= 0x80;					// 把最高位置1
//	}
//	MyI2C_W_SCL(0);						// 拉低SCL
	
	for (i = 0; i < 8; i++)
	{
		MyI2C_W_SCL(1);						// 释放SCL
		// 读数据
		if (MyI2C_R_SDA() == 1)
		{
			byte |= (0x80 >> i);			// 把高位置1
		}
		MyI2C_W_SCL(0);						// 拉低SCL
	}
	return byte;
}


// 发送应答（发送一个字节的简化版）
void MyI2C_SendAck(uint8_t ackByte)
{
	MyI2C_W_SDA(ackByte);			// 取决ackByte
	MyI2C_W_SCL(1);					// 释放SCL
	MyI2C_W_SCL(0);					// 拉低SCL
}

// 接收应答（接收一个字节的简化版）
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t ackByte;
	MyI2C_W_SDA(1);						// 释放SDA
	MyI2C_W_SCL(1);						// 释放SCL
	ackByte = MyI2C_R_SDA();			// 读数据
	MyI2C_W_SCL(0);						// 拉低SCL
	return ackByte;
}

