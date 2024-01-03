#ifndef __W25Q64_H__
#define __W25Q64_H__

void W25Q64_Init(void);								// 初始化函数
void W25Q64_ReadID(uint8_t* MID, uint16_t* DID);	// 读设备ID，厂商ID和设备ID
void W25Q64_WriteEnable(void);						// 写使能
void W25Q64_WaitBusy(void);
void W25Q64_PageProgram(uint32_t address, uint8_t* dataArray, uint16_t count);
void W25Q64_SectorErase(uint32_t address);
void W25Q64_ReadData(uint32_t address, uint8_t* dataArray, uint32_t count);


#endif
