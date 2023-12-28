#ifndef __MY_I2C_H__
#define __MY_I2C_H__

void MyI2C_W_SCL(uint8_t bitValue);			// 封装SCL的写函数
void MyI2C_W_SDA(uint8_t bitValue);			// 封装SDA的写函数
uint8_t MyI2C_R_SDA(void);					// 封装SDA的读函数
void MyI2C_Init(void);						// I2C初始化
void MyI2C_Start(void);						// 起始条件
void MyI2C_Stop(void);						// 终止条件
void MyI2C_SendByte(uint8_t byte);			// 发送一个字节
uint8_t MyI2C_ReceiveByte(void);			// 接收一个字节
void MyI2C_SendAck(uint8_t ackByte);		// 发送应答（发送一个字节的简化版）
uint8_t MyI2C_ReceiveAck(void);				// 接收应答（接收一个字节的简化版）

#endif
