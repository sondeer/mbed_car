
/**
 * @file odev_ticker.cpp
 * @author rcq
 * @version 1.1
 * @date 2017-07-4
 * @brief ���ڲ��Է����ö�ʱ�������г����쳣������Ϊ���ⳤʱ���޷�Ӧ��
 *		����Ϳ��ػ���tick���������ֲ���
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
	



