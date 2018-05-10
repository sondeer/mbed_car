
/**
 * @file odev_ticker.cpp
 * @author rcq
 * @version 1.1
 * @date 2017-07-4
 * @brief 由于测试发现用定时器过程中出现异常，这里为避免长时间无反应，
 *		表情和开关机用tick，其他部分不变
 */
#include "mbed.h"
#include "odev_myticker.h"

	volatile uint32_t MyTickCnt=0;

	void my_ticker_count(void)
	{
		MyTickCnt++;  
	}

	uint32_t my_ticker_read(void)
	{
		return MyTickCnt;
	}
	



