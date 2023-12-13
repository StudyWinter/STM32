#ifndef __AD_H__
#define __AD_H__

void AD_Init(void);									// AD初始化函数
void AD_GetValue(void);								// 获取转换结果
extern uint16_t AD_Value[4];

#endif
