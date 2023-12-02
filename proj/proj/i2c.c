/*
 * i2c.c
 *
 * Created: 2023-11-29 오후 11:46:32
 *  Author: hwan
 */ 
#ifndef I2C_C_
#define I2C_C_

#include "sam.h"
#include "i2c.h"
#include "configs.h"

void I2C_setup() {

	//
	// PORT setup for PA22 and PA23 (I2C)
	//

	PORT->Group[0].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[0].PMUX[11].bit.PMUXE = 0x02; // peripheral function C selected
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x02; // peripheral function C selected

	// Power Manager
	PM->APBCMASK.bit.SERCOM3_ = 1 ; // Clock Enable for I2C			
	
	//
	// * SERCOM3: I2C
	// * Generator #0 is feeding I2C

	GCLK->CLKCTRL.bit.ID = 0x17; // ID #17 (SERCOM3: I2C)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for I2C
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to I2C!
	
	// I2C Setup (Host mode)
	SERCOM3->I2CM.CTRLA.bit.SWRST = 1 ; // software reset
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 0 ; // Disable
	//	SERCOM3->I2CM.CTRLA.bit.LOWTOUTEN = 1 ; // SCL Low Timeout Enable

	SERCOM3->I2CM.CTRLA.bit.MODE = 0x5 ; // Host mode
	//SERCOM3->I2CM.BAUD.bit.BAUD = 0x27 ; // 100KHz SCL (0x27 = d'39)
	SERCOM3->I2CM.BAUD.bit.BAUD = 0x20 ; // 100KHz SCL (0x20 = d'32)
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 1 ; // Enable
	SERCOM3->I2CM.STATUS.bit.BUSSTATE = 1 ; // IDLE state	
}


#endif 