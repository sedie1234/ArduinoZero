/*
 * gpio.c
 *
 * Created: 2023-11-29 오후 8:09:15
 *  Author: hwan
 */ 
#ifndef GPIO_C_
#define GPIO_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"
#include "uart.h"
#include "sam.h"
#include "configs.h"

extern int distance;

extern int state;
extern int LT0_flag;
extern int LT1_flag;
extern int LT2_flag;
extern int LT0;
extern int LT1;
extern int LT2;
extern int stop_cnt;
extern int runstart;

void GPIO_setup(){

    //GCLK set : EIC
    GCLK->CLKCTRL.bit.ID = 5; // ID #5 (EIC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!	

    //output ports
    //PA10 - UltraSonic Trigger
    PORT->Group[0].PINCFG[10].reg = 0;
    PORT->Group[0].DIR.reg |= 0x1 << 10;
	PORT->Group[0].OUT.reg = 0 << 10 ;

    //PA17 - LIS2DW12 CS
	PORT->Group[0].PINCFG[17].reg = 0;
    PORT->Group[0].DIR.reg |= 0x1 << 17;
    
    //PA20 - DC IN1
	PORT->Group[0].PINCFG[20].reg = 0;
    PORT->Group[0].DIR.reg |= 0x1 << 20;

    //PA21 - DC IN2
	PORT->Group[0].PINCFG[21].reg = 0;
    PORT->Group[0].DIR.reg |= 0x1 << 21;

    //PA02 - DC IN3
	PORT->Group[0].PINCFG[2].reg = 0;
    PORT->Group[0].DIR.reg |= 0x1 << 2;

    //PA14 - DC IN4
	PORT->Group[0].PINCFG[14].reg = 0;
    PORT->Group[0].DIR.reg |= 0x1 << 14;


    //input ports(interrupt)
    //PA11 - Line Tracer //EXTINT[11]
    //PORT->Group[0].DIRCLR.reg |= 0x01 << 11;
    PORT->Group[0].PINCFG[11].reg = 0x41;
	PORT->Group[0].PMUX[5].bit.PMUXO = 0x0;//function A : EIC

    //PA19 - Line Tracer //EXTINT[3]
    PORT->Group[0].PINCFG[19].reg = 0x41;
	PORT->Group[0].PMUX[8].bit.PMUXO = 0x0;//function A : EIC

    //PA15 - Line Tracer //EXTINT[15]
    PORT->Group[0].PINCFG[15].reg = 0x41;
	PORT->Group[0].PMUX[7].bit.PMUXO = 0x0;//function A : EIC

    //PA16 - Line Tracer //EXTINT[0]
    PORT->Group[0].PINCFG[16].reg = 0x41;
	PORT->Group[0].PMUX[8].bit.PMUXE = 0x0;//function A : EIC

}

void EIC_setup(){
    EIC->CONFIG[1].bit.FILTEN3 = 1;    // filter is enabled
	EIC->CONFIG[1].bit.SENSE3 = 0x3;   // Both-edges detection
	EIC->INTENSET.bit.EXTINT11 = 1;    // External Interrupt 11 is enabled

    EIC->CONFIG[0].bit.FILTEN3 = 1;    // filter is enabled
	EIC->CONFIG[0].bit.SENSE3 = 0x3;   // Both-edges detection
	EIC->INTENSET.bit.EXTINT3 = 1;     // External Interrupt 3 is enabled
	
	EIC->CONFIG[1].bit.FILTEN7 = 1;    // filter is enabled
	EIC->CONFIG[1].bit.SENSE7 = 0x3;   // Both-edges detection
	EIC->INTENSET.bit.EXTINT15 = 1;    // External Interrupt 15 is enabled

    EIC->CONFIG[0].bit.FILTEN0 = 1;    // filter is enabled
	EIC->CONFIG[0].bit.SENSE0 = 0x3;   // Both-edges detection
	EIC->INTENSET.bit.EXTINT0 = 1;    // External Interrupt 15 is enabled

	EIC->CTRL.bit.ENABLE = 1;          // EIC is enabled	
}

void EIC_Handler(void)
{
	
	char str[100] = "EIC handler\r\n";
	int flag = EIC->INTFLAG.reg;
	static int prev_us;
	static int curr_us;
	static uint32_t cnt;
	//static int Num_EIC_interrupts;
    //Print(str, strlen(str));
	//PA11 - INT11 	
	if(flag&(1<<11)){
		EIC->INTFLAG.bit.EXTINT11 = 1 ; // Clear the EXTINT3 interrupt flag
		// curr_us = TC4->COUNT32.COUNT.reg;
		// distance = (curr_us - prev_us) * 0.17 ; // distance in mm
		cnt++;
		curr_us = RTC->MODE0.COUNT.reg;
		if(cnt==2){
			distance = (curr_us - prev_us) /8 * 0.17; // distance in mm
			cnt = 0;
		}
		prev_us = curr_us;
	}

	if(runstart){
		//PA19 - INT3 -left
		if(flag&(1<<3)){
			LT0_flag = 1;
			if((PORT->Group[0].IN.reg>>19)&0x01){
				LT0 = 1;
			}else{
				LT0 = 0;
			}
		}

		//PA15 - INT15 -mid
		if(flag&(1<<15)){
			LT1_flag = 1;
			if((PORT->Group[0].IN.reg>>15)&0x01){
				LT1 = 1;
			}else{
				LT1 = 0;
			}
		}

		//PA16 - INT0 -right
		if(flag&(1<<0)){
			LT2_flag = 1;
			if((PORT->Group[0].IN.reg>>16)&0x01){
				LT2 = 1;
			}else{
				LT2 = 0;
			}
		}
	}
	
	EIC->INTFLAG.bit.EXTINT11 = 1 ;
	EIC->INTFLAG.bit.EXTINT3 = 1 ;
	EIC->INTFLAG.bit.EXTINT15 = 1 ;
	EIC->INTFLAG.bit.EXTINT0 = 1 ;
	
}

//group : A = 0 // B = 1
void SetHigh(int group, int port){

	PORT->Group[group].OUT.reg |= 1 << port;

}

void SetLow(int group, int port){

	int value = 0;
	value |= 1 << port;
	value = ~value;
	PORT->Group[group].OUT.reg &= value;

}

int ReadPin(int group, int port){
	return (PORT->Group[group].IN.reg>>port)&0x01;
}

#endif 