/*
 * timer.h
 *
 * Created: 2023-11-30 오전 10:43:25
 *  Author: hwan
 */ 


#ifndef TIMER_H_
#define TIMER_H_

void RTC_setup();
void RTC_Handler();
void TimerDelay(int us);

#endif /* TIMER_H_ */