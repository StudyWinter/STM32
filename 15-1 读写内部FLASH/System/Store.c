#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"
#define STORE_START_ADDRESS 0x0800FC00
#define STORE_COUNT 512

// 利用SRAM缓存数组来管理最后一页，实现参数的任意读写和保存
uint16_t store_Data[STORE_COUNT];			// 1024字节

// 初始化，没初始化的时候，全部都是FF；初始化后，第一个半字是A5A5，其余全是0
void Store_Init(void)
{
	// 利用标志位判断是否是第一次使用（第一次使用）
	if (MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)
	{
		MyFLASH_ErasePage(STORE_START_ADDRESS);					// 擦除最后一页
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);	// 写入标志位
		// 把剩余的空间置为0
		for (uint16_t i = 1; i < STORE_COUNT; i++)
		{
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);	// 把剩余的空间置为0
		}
	}
	// 上电的时候，把闪存的数据全部转移到SRAM数组中，上电恢复数据，数据掉电不丢失
	for (uint16_t i = 0; i < STORE_COUNT; i++)
	{
		store_Data[i] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
	}
}

// 备份保存
void Store_Save(void)
{
	// 擦除最后一页
	MyFLASH_ErasePage(STORE_START_ADDRESS);
	// 把数组备份到闪存
	for (uint16_t i = 0; i < STORE_COUNT; i++)
	{
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, store_Data[i]);
	}
}

// 清除
void Store_Clear(void)
{
	for (uint16_t i = 1; i < STORE_COUNT; i++)
	{
		store_Data[i] = 0x0000;
	}
	//保存更改
	Store_Save();
}
