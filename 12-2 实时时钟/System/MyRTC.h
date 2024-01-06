#ifndef __MYRTC_H__
#define __MYRTC_H__

void MyRTC_Init(void);				// 初始化
void MyRTC_SetTime(void);			// 设置时间，把数组的时间转换为秒数，写到CNT中
void MyRTC_ReadTime(void);			// 读取时间的函数

extern uint16_t myRTC_Time[];

#endif
