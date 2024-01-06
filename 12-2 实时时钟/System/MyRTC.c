#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t myRTC_Time[] = {2023, 1, 1, 23, 59, 55};

// 初始化
void MyRTC_Init(void)
{
	// 1初始化：分两步
	// (1)设置RCC_APB1ENR的PWREN和BKPEN，使能PWR和BKP时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);			// 开启PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);			// 开启BKP的时钟
	// (2)设置PWR_CR的DBP，使能对BKP和RTC的访问
	PWR_BackupAccessCmd(ENABLE);
	
	// 防止重复初始化和时间重置。在BKP_DR1写入0xA5A5，如果备用电池不断电，则BKP_DR1中还是0xA5A5
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		// 2启动RTC的时钟，使用LSE作为系统时钟，需要开启LSE的时钟（默认是关闭的），等待启动完成
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
		
		// 3配置RTCCLK数据选择器，指定LSE为RTCCLK
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);			// 32768Hz
		RCC_RTCCLKCmd(ENABLE);
		
		// 4等待函数，等待同步；等待上一次写入操作完成
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();

		// 5配置预分频器，给PRL重装寄存器一个合适的分频值（确保输出是1Hz）
		// 需要进入配置模式，但是不用写代码
		RTC_SetPrescaler(32768 - 1);
		RTC_WaitForLastTask();
		
		// 6配置CNT的值，闹钟/中断
		RTC_SetCounter(1672588795);				// 2023-1-1 23:59:55
	//	MyRTC_SetTime();
		RTC_WaitForLastTask();
		// CNT的值就会以1672588795这个值开始，以1s的频率开始自增，读取CNT的值就能获取时间了
		
		// 在寄存器BKP_DR1写入0xA5A5
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}


// 设置时间，把数组的时间转换为秒数，写到CNT中
void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	// 填充时间
	time_date.tm_year = myRTC_Time[0] - 1900;
	time_date.tm_mon = myRTC_Time[1] - 1;
	time_date.tm_mday = myRTC_Time[2];
	time_date.tm_hour = myRTC_Time[3];
	time_date.tm_min = myRTC_Time[4];
	time_date.tm_sec = myRTC_Time[5];
	
	// 日期时间到秒数的转换，北京时间-8
	time_cnt = mktime(&time_date) - 8 * 60 * 60;
	
	// 把指定的秒数写入到CNT中
	RTC_SetCounter(time_cnt);	
	RTC_WaitForLastTask();
}

// 读取时间的函数
void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	// 获取秒数，北京时间+8
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	
	// 得到日期
	time_date = *localtime(&time_cnt);
	
	// 日期时间转移到数组里
	myRTC_Time[0] = time_date.tm_year + 1900;
	myRTC_Time[1] = time_date.tm_mon + 1;
	myRTC_Time[2] = time_date.tm_mday;
	myRTC_Time[3] = time_date.tm_hour;
	myRTC_Time[4] = time_date.tm_min;
	myRTC_Time[5] = time_date.tm_sec;
}




