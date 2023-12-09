#ifndef __PWM_H__
#define __PWM_H__

void PWM_Init(void);						// PWM初始化函数
void PWM_SetCompare1(uint16_t Compare);		// 更改CCR的值
void PWM_SetPrescaler(uint16_t prescaler);  // 修改PSC

#endif
