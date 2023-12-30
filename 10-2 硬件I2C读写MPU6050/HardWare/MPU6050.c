#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 	0xD0


// 封装I2C_CheckEvent函数，加上超时，防止卡死
void MPU6050_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t timeOut = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		timeOut--;
		if (timeOut == 0)
		{
			break;
		}
	}
}

// 指定地址写
// 参数是8位地址和8位数据
void MPU6050_WriteReg(uint8_t regAddress, uint8_t data)
{
	
	// 2控制外设电路，实现指定地址写的时序
	I2C_GenerateSTART(I2C2, ENABLE);						// 生成起始条件
	// 等待EV5事件
//	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// 发送从机地址，自带接收应答，同样接收数据也自带发送应答
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	// 等待EV6事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, regAddress);			// 直接写入DR，发送数据
	// 等待EV8事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, data);				// 直接写入DR，发送数据
	// 连续发送过程等待EV8，最后一位等待EV8_2事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);				// 生成终止条件
}

// 指定地址读
uint8_t MPU6050_ReadReg(uint8_t regAddress)
{
	// 3控制外设电路，实现指定地址读的时序
	I2C_GenerateSTART(I2C2, ENABLE);						// 生成起始条件
	// 等待EV5事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// 发送从机地址，自带接收应答，同样接收数据也自带发送应答
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	// 等待EV6事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, regAddress);			// 直接写入DR，发送数据
	// 等待EV8事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);						// 生成起始条件
	// 等待EV5事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// 接收数据也自带发送应答
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	// 等待EV6事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);					// 配置ACK位
	I2C_GenerateSTOP(I2C2, ENABLE);							// 生成终止条件
	
	// 等待EV7事件
	MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	uint8_t data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);					// 配置ACK位
	return data;
}

// 初始化MPU6050
// 配置的是：解除睡眠，选择陀螺仪时钟，6个轴均不待机，采样分频位10，滤波参数最大
// 陀螺仪和加速度计都选择最大量程
void MPU6050_Init(void)
{
//	MyI2C_Init();													// 初始化I2C
	// 1配置I2C外设，对I2C2外设进行初始化
	// (1)开启I2C外设和对应GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// (2)把I2C外设对应的GPIO口初始化为复用开漏模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	// (3)使用结构体对整个I2C进行配置
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;						// I2C的模式
	I2C_InitStructure.I2C_ClockSpeed = 50000;						// 时钟速度
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 				// 时钟占空比，小于于100KHz是1:1
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;						// ACK应答位
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		// 7位地址
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;						// 自身地址，用不到
	I2C_Init(I2C2, &I2C_InitStructure);
	// (4)I2C_Cmd，使能I2C
	I2C_Cmd(I2C2, ENABLE);
	
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

