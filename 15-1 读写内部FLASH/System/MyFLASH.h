#ifndef __MYFLASH_H__
#define __MYFLASH_H__


uint32_t MyFLASH_ReadWord(uint32_t address);  					// 以字为单位读取
uint16_t MyFLASH_ReadHalfWord(uint32_t address);				// 以半字为单位读取
uint8_t MyFLASH_ReadByte(uint32_t address);						// 以字节为单位读取
void MyFLASH_EraseAllPages(void);								// 全擦除
void MyFLASH_ErasePage(uint32_t pageAddress);					// 页擦除
void MyFLASH_ProgramWord(uint32_t address, uint32_t data);  	// 编程字
void MyFLASH_ProgramHalfWord(uint32_t address, uint16_t data);  // 编程半字



#endif
