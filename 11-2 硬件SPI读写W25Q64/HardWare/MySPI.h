#ifndef __MYSPI_H__
#define __MYSPI_H__

void MySPI_Init(void);							// 初始化
void MySPI_Start(void);							// 起始信号
void MySPI_Stop(void);							// 终止信号
uint8_t MySPI_SwapByte(uint8_t byteSend);		// 交换一个字节，模式0，参数是主机要交换的数据，返回值是从机要交换的数据


#endif
