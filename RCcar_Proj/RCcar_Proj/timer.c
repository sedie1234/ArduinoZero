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
#include "gpio.h"
#include "configs.h"

void RTC_setup(){
    GCLK->CLKCTRL.bit.ID = 4; // ID #4 (RTC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!

    RTC->MODE0.CTRL.bit.ENABLE = 0; // Disable first
	RTC->MODE0.CTRL.bit.MODE = 0; // Mode 0
	RTC->MODE0.CTRL.bit.MATCHCLR = 1; // match clear
	RTC->MODE0.INTENSET.bit.CMP0 = 1; // compare 0 interrupt is enabled

	//RTC->MODE0.COMP->reg = 400000; // compare register to set up the period //8MHz x400,000 = 50ms
	RTC->MODE0.COMP->reg = 16000000; // compare register	to set up the peroid
	RTC->MODE0.COUNT.reg = 0x0; // initialize the counter to 0
	RTC->MODE0.CTRL.bit.ENABLE = 1; // Enable
}

void RTC_Handler()
{
    //static int cnt;
	char str[100] = "int\r\n";
	Print(str, strlen(str));
    //PrintNum(cnt);
	
    //ultra sonic
    SetLow(0, 10);
    TimerDelay(10);
    SetHigh(0, 10);
    TimerDelay(10);
    SetLow(0, 10);
    //cnt++;

    unsigned char buffer[6];
    short x_mg, y_mg, z_mg;
    //if(cnt%4 == 0){
    if(0){
        //every 200ms, Accelerometer
        LIS2DH_I2C_read_multiple_data(0x28, 6, buffer); //
        bytes_to_ints(buffer, &x_mg, &y_mg, &z_mg);
    }
	
	RTC->MODE0.INTFLAG.bit.CMP0 = 1; // clear overflow interrupt flag
}

void TimerDelay(int us){
    int curr_time = TC4->COUNT32.COUNT.reg;
    while(TC4->COUNT32.COUNT.reg < curr_time + us);
}

#endif