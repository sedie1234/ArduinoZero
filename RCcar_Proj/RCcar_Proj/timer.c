/*
 * timer.c
 *
 * Created: 2023-11-30 오전 10:43:37
 *  Author: hwan
 */ 
#ifndef TIMER_C_
#define TIMER_C_

#include "sam.h"
#include "timer.h"
#include "uart.h"

void RTC_setup(){
    GCLK->CLKCTRL.bit.ID = 4; // ID #4 (RTC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!

    RTC->MODE0.CTRL.bit.ENABLE = 0; // Disable first
	RTC->MODE0.CTRL.bit.MODE = 0; // Mode 0
	RTC->MODE0.CTRL.bit.MATCHCLR = 1; // match clear
	RTC->MODE0.INTENSET.bit.CMP0 = 1; // compare 0 interrupt is enabled

	RTC->MODE0.COMP->reg = 0x800000; // compare register to set up the period
	//RTC->MODE0.COMP->reg = 0x10000; // compare register	to set up the peroid
	RTC->MODE0.COUNT.reg = 0x0; // initialize the counter to 0
	RTC->MODE0.CTRL.bit.ENABLE = 1; // Enable
}

void RTC_Handler()
{
	char str[100] = "interrupt!\r\n";
	Print(str, sizeof(str));
	
	
	RTC->MODE0.INTFLAG.bit.CMP0 = 1; // clear overflow interrupt flag
}

#endif