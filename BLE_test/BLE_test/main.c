/*
 * GccApplication_lab_prep.c
 *
 * Created: 7/27/2023 10:26:32 AM
 * Author : suhtw
 */ 

#pragma GCC target ("thumb")

#include "sam.h"



void GCLK_setup();
void PORT_setup();
void USART_setup();
void SERCOM0_Handler();
char rx_data;
char rx_buf[100];

int main()
{
	/* Initialize the SAM system */
	char msg[100] = "1234";
	
	
    SystemInit();
	
	GCLK_setup();
	
	PORT_setup();
	
	USART_setup();	

	SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
#if 1	
	for(int i=0; i<sizeof(msg); i++){
		SERCOM0->USART.DATA.reg	= msg[i];
	}
#endif

	NVIC_EnableIRQ(SERCOM0_IRQn);
	while(1){
		
	}

	return (0);
}


void GCLK_setup() {
	
// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0; 
	SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable
	
// Power Manager
	PM->APBCMASK.bit.SERCOM0_ = 1 ; // Clock Enable (APBC clock)

	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable

	GCLK->CLKCTRL.bit.ID = 0x14; // ID #0x14 (SERCOM0: USUART)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!
}


void PORT_setup() {
	
	PORT->Group[0].PINCFG[6].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PINCFG[7].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[0].PMUX[3].bit.PMUXE = 0x03; // peripheral function D selected	
	PORT->Group[0].PMUX[3].bit.PMUXO = 0x03; // peripheral function D selected	
}


void USART_setup() {
		
	//
	// USART setup
	//
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
	
}

void SERCOM0_Handler(){

	if(SERCOM0->USART.INTFLAG.bit.RXC){
		static int cnt;
		rx_data	= SERCOM0->USART.DATA.reg;
		rx_buf[cnt++]=rx_data;
		if(cnt == 90){
			cnt = 0;
		}
		SERCOM0->USART.DATA.reg	= rx_data;
	}
	
}
