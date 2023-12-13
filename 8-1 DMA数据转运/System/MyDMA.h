#ifndef __MYDMA_H__
#define __MYDMA_H__

void MyDMA_Init(uint32_t addrA, uint32_t addrB, uint32_t size);		// DMA初始化
void MYDMA_Transfer(void);											// 传输函数

#endif
