﻿/*
 * uart.c
 *
 * Created: 2023-11-29 오후 5:54:14
 *  Author: hwan
 */ 
#ifndef UART_C_
#define UART_C_

#include "sam.h"

char rx_data;

void USART_setup(){

#if BLE_USE
    //power manager set
    PM->APBCMASK.bit.SERCOM0_ = 1 ; // Clock Enable (APBC clock)

    //GCLK set
    GCLK->CLKCTRL.bit.ID = 0x14; // ID #0x14 (SERCOM0: USUART)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!

    //port set
    //PA6, PA7, function D, PAD2,3
    PORT->Group[0].PINCFG[6].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PINCFG[7].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[0].PMUX[3].bit.PMUXE = 0x03; // peripheral function D selected	
	PORT->Group[0].PMUX[3].bit.PMUXO = 0x03; // peripheral function D selected	
	
    //sercom set
	SERCOM0->USART.CTRLA.bit.MODE = 1 ; // Internal Clock
	SERCOM0->USART.CTRLA.bit.CMODE = 0 ; // Asynchronous UART
	SERCOM0->USART.CTRLA.bit.RXPO = 3 ; // PAD3
	SERCOM0->USART.CTRLA.bit.TXPO = 1 ; // PAD2
	SERCOM0->USART.CTRLB.bit.CHSIZE = 0 ; // 8-bit data
	SERCOM0->USART.CTRLA.bit.DORD = 1 ; // LSB first
	//SERCOM5->USART.CTRLA.bit.SAMPR = 1 ; //
	
	//SERCOM0->USART.BAUD.reg = 0Xfb15 ; //9600 in 8MHz
	SERCOM0->USART.BAUD.reg = 0Xc504 ; //115200 in 8MHz

	SERCOM0->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM0->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM0->USART.CTRLA.bit.ENABLE = 1;

#else
    //power manager set
    PM->APBCMASK.bit.SERCOM5_ = 1 ; // Clock Enable (APBC clock)

    //GCLK set
    GCLK->CLKCTRL.bit.ID = 0x19; // ID #0x14 (SERCOM0: USUART)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!

    //port set
    //PB22, PB23, function D, PAD2,3
    PORT->Group[1].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[1].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[1].PMUX[11].bit.PMUXE = 0x03; // peripheral function D selected
	PORT->Group[1].PMUX[11].bit.PMUXO = 0x03; // peripheral function D selected

    //sercom set
    SERCOM5->USART.CTRLA.bit.MODE = 1 ; // Internal Clock
	SERCOM5->USART.CTRLA.bit.CMODE = 0 ; // Asynchronous UART
	SERCOM5->USART.CTRLA.bit.RXPO = 3 ; // PAD3
	SERCOM5->USART.CTRLA.bit.TXPO = 1 ; // PAD2
	SERCOM5->USART.CTRLB.bit.CHSIZE = 0 ; // 8-bit data
	SERCOM5->USART.CTRLA.bit.DORD = 1 ; // LSB first

    SERCOM5->USART.BAUD.reg = 0Xc504 ; // 115,200 bps (baud rate) with 8MHz input clock

	SERCOM5->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM5->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM5->USART.CTRLA.bit.ENABLE = 1;
#endif

}

#if BLE_USE
void SERCOM0_Handler(){
	if(SERCOM0->USART.INTFLAG.bit.RXC){
		rx_data	= SERCOM0->USART.DATA.reg;
		SERCOM0->USART.DATA.reg	= rx_data;
	}
}

#else
void SERCOM5_Handler(){
    if(SERCOM5->USART.INTFLAG.bit.RXC){
		rx_data	= SERCOM5->USART.DATA.reg;
		SERCOM5->USART.DATA.reg	= rx_data;
	}
}

#endif

#endif