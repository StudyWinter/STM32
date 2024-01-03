#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

// 初始化函数
void W25Q64_Init(void)
{
	MySPI_Init();
}

// 读设备ID，厂商ID和设备ID
void W25Q64_ReadID(uint8_t* MID, uint16_t* DID)
{
	MySPI_Start();								// 开始
	MySPI_SwapByte(W25Q64_JEDEC_ID);			// 交换0x9f，主机读这个ID号
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 参数无意义，得到厂商ID
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 参数无意义，得到设备ID高8位
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 参数无意义，得到设备ID低8位
	MySPI_Stop();								// 终止
}

// 写使能
void W25Q64_WriteEnable(void)
{
	MySPI_Start();								// 开始
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);		// 写使能
	MySPI_Stop();								// 终止
}

// 等待busy位0，即不忙
// 读状态寄存器1，判断芯片是不是忙状态（最低位是不是busy/1）
void W25Q64_WaitBusy(void)
{
	uint32_t timeOut = 100000;
	MySPI_Start();													// 开始
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);					// 读状态寄存器1
	// 参数无意义，得到状态寄存器1
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		timeOut--;
		if (timeOut == 0)
		{
			break;
		}
	}
	MySPI_Stop();													// 终止
}

// 页编程函数，数据范围是0~256
// 地址，写数据，写数据的数量
void W25Q64_PageProgram(uint32_t address, uint8_t* dataArray, uint16_t count)
{
	W25Q64_WriteEnable();											// 写使能
	MySPI_Start();													// 开始
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);							// 页编程
	// 发地址
	MySPI_SwapByte(address >> 16);									// 高位地址
	MySPI_SwapByte(address >> 8);									// 中间地址
	MySPI_SwapByte(address);										// 最低位地址
	// 发数据
	for (uint16_t i = 0; i < count; i++)
	{
		MySPI_SwapByte(dataArray[i]);
	}
	MySPI_Stop();													// 终止
	W25Q64_WaitBusy();												// 等待busy结束再退出，事后等待；也可以事前等待
}

// 擦除的功能(扇区擦除)
void W25Q64_SectorErase(uint32_t address)
{
	W25Q64_WriteEnable();											// 写使能
	MySPI_Start();													// 开始
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);						// 擦除扇区
	// 发地址
	MySPI_SwapByte(address >> 16);									// 高位地址
	MySPI_SwapByte(address >> 8);									// 中间地址
	MySPI_SwapByte(address);										// 最低位地址
	MySPI_Stop();													// 终止
	W25Q64_WaitBusy();												// 等待busy结束再退出，事后等待；也可以事前等待
}

// 读数据
// dataArray输出参数
void W25Q64_ReadData(uint32_t address, uint8_t* dataArray, uint32_t count)
{
	MySPI_Start();													// 开始
	MySPI_SwapByte(W25Q64_READ_DATA);								// 读数据
	// 发地址
	MySPI_SwapByte(address >> 16);									// 高位地址
	MySPI_SwapByte(address >> 8);									// 中间地址
	MySPI_SwapByte(address);										// 最低位地址
	// 发数据
	for (uint32_t i = 0; i < count; i++)
	{
		dataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);			// 发送FF，置换回有用的数据
	}
	MySPI_Stop();													// 终止
}





