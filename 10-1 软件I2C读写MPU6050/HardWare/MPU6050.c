#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"


#define MPU6050_ADDRESS 	0xD0

// 指定地址写
// 参数是8位地址和8位数据
void MPU6050_WriteReg(uint8_t regAddress, uint8_t data)
{
	MyI2C_Start();								// i2c起始
	MyI2C_SendByte(MPU6050_ADDRESS);			// 发送MPU6050的地址
	MyI2C_ReceiveAck();							// 接收应答
	
	MyI2C_SendByte(regAddress);					// 发送指定寄存器地址
	MyI2C_ReceiveAck();							// 接收应答
	
	MyI2C_SendByte(data);						// 指定写入寄存器的数据 
	MyI2C_ReceiveAck();							// 接收应答
	
	MyI2C_Stop();								// 终止
}

// 指定地址读
uint8_t MPU6050_ReadReg(uint8_t regAddress)
{
	uint8_t data;
	
	MyI2C_Start();								// i2c起始
	MyI2C_SendByte(MPU6050_ADDRESS);			// 发送MPU6050的地址
	MyI2C_ReceiveAck();							// 接收应答
	
	MyI2C_SendByte(regAddress);					// 发送指定寄存器地址
	MyI2C_ReceiveAck();							// 接收应答
	
	MyI2C_Start();								// i2c起始
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);		// 读数据 1101 0000写/1101 0001读
	MyI2C_ReceiveAck();							// 接收应答
	
	data = MyI2C_ReceiveByte();					// 总线控制权给从机，从机开始发送一个字节
	MyI2C_SendAck(1);							// 发送应答
	// 读取最后一个字节给非应答MyI2C_SendAck(0)，之前都给应答
	
	MyI2C_Stop();								// 终止
	return data;
}

// 初始化MPU6050
// 配置的是：解除睡眠，选择陀螺仪时钟，6个轴均不待机，采样分频位10，滤波参数最大
// 陀螺仪和加速度计都选择最大量程
void MPU6050_Init(void)
{
	MyI2C_Init();													// 初始化I2C
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);						// 配置电源管理寄存器1，结合数据手册写
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);						// 配置电源管理寄存器2，结合数据手册写
	
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);						// 采样率分频寄存器，10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);							// 配置寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);					// 陀螺仪配置寄存器
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);					// 加速度计寄存器	
}

// 获取数据寄存器，返回6个数据，形参是指针
void MPU6050_GetData(int16_t* accX, int16_t* accY, int16_t* accZ, 
					 int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ)
{
	uint8_t dataH, dataL;
	dataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);							// 读取加速度寄存器X轴的高8位
	dataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);							// 读取加速度寄存器X轴的低8位
	*accX = (dataH << 8) | dataL;											// 	通过指针返回回去
	dataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);							// 读取加速度寄存器y轴的高8位
	dataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);							// 读取加速度寄存器y轴的低8位
	*accY = (dataH << 8) | dataL;											// 	通过指针返回回去
	dataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);							// 读取加速度寄存器z轴的高8位
	dataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);							// 读取加速度寄存器z轴的低8位
	*accZ = (dataH << 8) | dataL;											// 	通过指针返回回去
	
	
	dataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);							// 读取陀螺仪寄存器x轴的高8位
	dataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);							// 读取陀螺仪寄存器x轴的低8位
	*GyroX = (dataH << 8) | dataL;											// 	通过指针返回回去
	dataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);							// 读取陀螺仪寄存器y轴的高8位
	dataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);							// 读取陀螺仪寄存器y轴的低8位
	*GyroY = (dataH << 8) | dataL;											// 	通过指针返回回去
	dataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);							// 读取陀螺仪寄存器z轴的高8位
	dataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);							// 读取陀螺仪寄存器z轴的低8位
	*GyroZ = (dataH << 8) | dataL;											// 	通过指针返回回去
}

// 获取ID号
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

