#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main()
{
	OLED_Init();								// 初始化OLED
	MyRTC_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);			// 开启PWR的时钟
	
	
	OLED_ShowString(1, 1, "CNT");				// 计数器
	OLED_ShowString(2, 1, "ALT");				// 闹钟值
	OLED_ShowString(3, 1, "ALRF:");				// 闹钟标志位
	PWR_WakeUpPinCmd(ENABLE);					// 测试wakeup引脚
	
	// 写事件之前先存下来
	uint32_t alarm = RTC_GetCounter() + 10;
	// 在每次复位后设置闹钟值
	RTC_SetAlarm(alarm);				// 闹钟在10s后
	OLED_ShowNum(2, 6, alarm, 10);		// 显示CNT的值
	while (1)
	{
		OLED_ShowNum(1, 6, RTC_GetCounter(), 10);						// 显示CNT的值
		OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);		    // 显示闹钟标志位的值
		// 计数器=闹钟值时，闹钟标志位置1
		
		OLED_ShowString(4, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(4, 1, "       ");
		Delay_ms(100);
		
		OLED_ShowString(4, 1, "STANDBY");
		Delay_ms(1000);
		OLED_ShowString(4, 1, "       ");
		Delay_ms(100);
		OLED_Clear();							// 模拟关闭外设
		
		// 进入待机模式
		PWR_EnterSTANDBYMode();
		// 之后的代码就执行不到了
	}
}
