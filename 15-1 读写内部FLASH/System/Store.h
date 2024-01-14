#ifndef __STORE_H__
#define __STORE_H__


extern uint16_t store_Data[];// 1024字节
void Store_Init(void); 		// 初始化，没初始化的时候，全部都是FF；初始化后，第一个半字是A5A5，其余全是0
void Store_Save(void);		// 备份保存
void Store_Clear(void);		// 清除

#endif
