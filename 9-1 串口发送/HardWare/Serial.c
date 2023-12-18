#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

// 串口初始化
void Serial_Init(void)
{
	// 1开启时钟,USART和GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2GPIO初始化,TX配置成复用推挽输出，RX配置成输入
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 3配置USRT
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;											// 波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		// 硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;										// 发送模式
	USART_InitStructure.USART_Parity = USART_Parity_No;									// 校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								// 停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							// 8位字长
	USART_Init(USART1, &USART_InitStructure);
	
	// 4开启（发送）；中断（接收）
	USART_Cmd(USART1, ENABLE);
}

// 发送一个字节
void Serial_SendByte(uint8_t byte)
{
	USART_SendData(USART1, byte);
	// 等待标志位
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

// 发送一个数组
void Serial_SendArray(uint8_t* array, uint16_t len)
{
	uint16_t i;
	for (i = 0; i < len; i++) {
		Serial_SendByte(array[i]);
	}
}

// 发送字符串
void Serial_SendString(char* str) {
	uint8_t i;
	for (i = 0; str[i] != '\0'; i++) {
		Serial_SendByte(str[i]);
	}
}

// 次方函数
uint32_t Serial_Pow(uint32_t x, uint32_t y)
{
	uint32_t res = 1;
	while (y--) {
		res *= x;
	}
	return res;
}

// 发送数字，从高位到低位以此发送
void Serial_SendNumber(uint32_t number, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++)
	{
		Serial_SendByte(number / Serial_Pow(10, len - i - 1) % 10 + '0');
	}
}

// 重写fputc函数。fputc是printf函数的底层
int fputc(int ch, FILE* f)
{
	Serial_SendByte(ch);
	return ch;
}

// 封装sprintf函数
void Serial_Sprintf(char* format, ...)
{
	char str[100];
	va_list arg;					// 类型名 变量名
	va_start(arg, format);			// 从format位置开始接收参数表，放在arg中
	vsprintf(str, format, arg);
	va_end(arg);
	Serial_SendString(str);
}

