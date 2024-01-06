#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main()
{
	OLED_Init();								// 初始化OLED
	MyRTC_Init();
	OLED_ShowString(1, 1, "Date:XXXX-XX-XX");
	OLED_ShowString(2, 1, "Time:XX:XX:XX");
	OLED_ShowString(3, 1, "CNT:");
	OLED_ShowString(4, 1, "DIV:");				// 余数计数器
	
	while (1)
	{
		MyRTC_ReadTime();
		OLED_ShowNum(1, 6, myRTC_Time[0], 4);			// 年
		OLED_ShowNum(1, 11, myRTC_Time[1], 2);			// 月
		OLED_ShowNum(1, 14, myRTC_Time[2], 2);			// 日
		OLED_ShowNum(2, 6, myRTC_Time[3], 2);			// 时
		OLED_ShowNum(2, 9, myRTC_Time[4], 2);			// 分
		OLED_ShowNum(2, 12, myRTC_Time[5], 2);			// 秒
		
		OLED_ShowNum(3, 6, RTC_GetCounter(), 10);		// 显示CNT的值
		OLED_ShowNum(4, 6, RTC_GetDivider(), 10);		// 显示DIV的值 范围32767~0
//		OLED_ShowNum(4, 6, (32767 - RTC_GetDivider()) / 32767.0 * 999, 10);        // 显示DIV的值。显示毫秒0-999
	}
}
