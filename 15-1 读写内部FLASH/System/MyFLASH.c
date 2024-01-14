#include "stm32f10x.h"                  // Device header

// 以字为单位读取
uint32_t MyFLASH_ReadWord(uint32_t address)
{
	return *((__IO uint32_t *)(address));
}
// 以半字为单位读取
uint16_t MyFLASH_ReadHalfWord(uint32_t address)
{
	return *((__IO uint16_t *)(address));
}
// 以字节为单位读取
uint8_t MyFLASH_ReadByte(uint32_t address)
{
	return *((__IO uint8_t *)(address));
}

// 全擦除
void MyFLASH_EraseAllPages(void)
{
	// 解锁
	FLASH_Unlock();
	FLASH_EraseAllPages();
	// 加锁
	FLASH_Lock();
}
// 页擦除
void MyFLASH_ErasePage(uint32_t pageAddress)
{
	// 解锁
	FLASH_Unlock();
	FLASH_ErasePage(pageAddress);
	// 加锁
	FLASH_Lock();
}

// 编程字
void MyFLASH_ProgramWord(uint32_t address, uint32_t data)
{
	// 解锁
	FLASH_Unlock();
	FLASH_ProgramWord(address, data);
	// 加锁
	FLASH_Lock();
}
// 编程半字
void MyFLASH_ProgramHalfWord(uint32_t address, uint16_t data)
{
	// 解锁
	FLASH_Unlock();
	FLASH_ProgramHalfWord(address, data);
	// 加锁
	FLASH_Lock();
}

