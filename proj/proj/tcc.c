/*
 * tcc.c
 *
 * Created: 2023-11-29 오후 7:09:08
 *  Author: hwan
 */ 
#ifndef TCC_C_
#define TCC_C_

#include "tcc.h"
#include "sam.h"
#include "configs.h"

void TCC0_setup(){

    //power manager set
    PM->APBCMASK.bit.TCC0_ = 1;

    //GCLK set
    GCLK->CLKCTRL.bit.ID = 0x1A;	//ID = TCC0, TCC1
	GCLK->CLKCTRL.bit.GEN = 0;
	GCLK->CLKCTRL.bit.CLKEN = 1;

    //Port set : PA18-TCC0
	PORT->Group[0].PINCFG[18].reg = 0x41;
	PORT->Group[0].PMUX[9].bit.PMUXE = 0x05;

    PORT->Group[0].PINCFG[8].reg = 0x41;
	PORT->Group[0].PMUX[4].bit.PMUXE = 0x04; //function F

    PORT->Group[0].PINCFG[9].reg = 0x41;
	PORT->Group[0].PMUX[4].bit.PMUXO = 0x04; //function F
	
    //TCC set
	TCC0->CTRLA.bit.PRESCALER = 0x03; //DIV8 : 8MHz -> 1MHz
	TCC0->WAVE.bit.WAVEGEN = 0x02;
	
	TCC0->PER.reg = 20000; //1MHz x 20000 = 20ms
	TCC0->CC[0].reg = 0; // 1ms //motor ENB
    TCC0->CC[1].reg = 0; // 1ms //motor EnA
    TCC0->CC[2].reg = 1000; // 1ms //servo motor
	TCC0->CTRLA.bit.ENABLE = 1;
	
}

void TC4_setup(){
	PM->APBCMASK.bit.TC4_ = 1 ; // Clock Enable (APBC clock) for TC4

	GCLK->CLKCTRL.bit.ID = 0x1C; // ID #ID (TC4, TC5)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for TC4, TC5
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to TC4, TC5
	
	TC4->COUNT32.CTRLA.bit.MODE = 0x2; //count32 mode
	TC4->COUNT32.CTRLA.bit.PRESCALER = 3; // prescaler = 8 : 1Mhz clock, 1count = 1us
	TC4->COUNT32.COUNT.reg = 0; //counter init
	TC4->COUNT32.CTRLA.bit.ENABLE = 1;

}

#endif