#include <stc12.h>
#include "systimer.h"

static bool g_bTimer0Elapsed = false;

void Timer0Init(void)
{
    g_bTimer0Elapsed = false;

    ET0 = 0; // 禁止Timer0中断
    TR0 = 0; // 停止Timer0

	AUXR &= 0x7F; //定时器时钟12T模式
    TMOD &= 0xF0; //清除Timer0设置
	TMOD |= 0x01; //设置定时器0为16bit模块
	TF0 = 0; //清除TF0标志
}

void Timer0Restart(uint16_t ui16MicroSeconds)
{
// 定义器为12T模式
#define TIMER0_RELOAD_VALUE(us) (65536U - (us))

    g_bTimer0Elapsed = false;

    ui16MicroSeconds = TIMER0_RELOAD_VALUE(ui16MicroSeconds);
	TL0 = (uint8_t)ui16MicroSeconds;		//设置定时初值
	TH0 = (uint8_t)(ui16MicroSeconds >> 8);	//设置定时初值

    TF0 = 0;
    ET0 = 1; // 使能Timer0中断

    TR0 = 1; //定时器0开始计时
}

bool Timer0IsElapsed(void)
{
    return g_bTimer0Elapsed;
}

void Timer0Handler(void) __interrupt 1
{
    ET0 = 0; // 禁止Timer0中断
    TR0 = 0; // 停止Timer0

    TL0 = 0;
	TH0 = 0;

    g_bTimer0Elapsed = true;
}
