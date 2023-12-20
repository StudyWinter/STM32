#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <stdio.h>

extern uint8_t Serial_TxPacket[];						// 发送数据包
extern uint8_t Serial_RxPacket[];						// 接收数据包

void Serial_Init(void);									// 串口初始化
void Serial_SendByte(uint8_t byte);						// 发送一个字节
void Serial_SendArray(uint8_t* array, uint16_t len);	// 发送一个数组
void Serial_SendString(char* str);						// 发送字符串
uint32_t Serial_Pow(uint32_t x, uint32_t y);			// 次方函数
void Serial_SendNumber(uint32_t number, uint8_t len);	// 发送数字，从高位到低位以此发送
void Serial_Sprintf(char* format, ...);					// 封装sprintf函数


uint8_t Serial_GetRxFlag(void);							// 读后清除标志位函数
void Serial_SendPacket(void);							// 发送数据包



#endif
