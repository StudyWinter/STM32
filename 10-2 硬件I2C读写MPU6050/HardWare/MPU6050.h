#ifndef __MPU6050_H__
#define __MPU6050_H__

void MPU6050_WriteReg(uint8_t regAddress, uint8_t data);			// 指定地址写
uint8_t MPU6050_ReadReg(uint8_t regAddress);						// 指定地址读
void MPU6050_Init(void);											// 初始化MPU6050
void MPU6050_GetData(int16_t* accX, int16_t* accY, int16_t* accZ, 
					 int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ);// 获取数据寄存器，返回6个数据，形参是指针

uint8_t MPU6050_GetID(void);										// 获取ID号

#endif
