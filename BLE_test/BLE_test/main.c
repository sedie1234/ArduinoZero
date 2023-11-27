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
void SERCOM5_Handler();
char rx_data;

int main()
{
   
   	unsigned count, msg_size;
	char msg[] = {"Hello World!"};
	
	_Bool	rxc_flag;
	// _Bool	led, rxc_flag;
	
	/* Initialize the SAM system */
    SystemInit();
	
	GCLK_setup();
	
	PORT_setup();
	
	USART_setup();	

	//NVIC_EnableIRQ(SERCOM5_IRQn);

//
// Send 'Hello World' message 
//	
	count = 0;
	msg_size = sizeof(msg);
	
	while (1) {
		if (SERCOM5->USART.INTFLAG.bit.DRE == 1) {
			if (count == msg_size) {
				SERCOM5->USART.DATA.reg	= 10; // Line Feed
				while (!SERCOM5->USART.INTFLAG.bit.DRE); // wait until data register is empty
				SERCOM5->USART.DATA.reg	= 13; // Carriage Return
				break;				
			} else {
				SERCOM5->USART.DATA.reg	= msg[count];
				count++;
			}
		}
	};

//
// Take a character input from UART
//
	// led = 0;
	
	while(1) {
		rxc_flag = SERCOM5->USART.INTFLAG.bit.RXC; // check out RXC (Receive Complete) flag
		if (rxc_flag == 1)	{
			rx_data	= SERCOM5->USART.DATA.reg; // Read the received data
			SERCOM5->USART.DATA.reg	= rx_data; // Write the received data (echo back to PC)
			if (rx_data == 'a') { 
				// PORT->Group[0].OUT.reg = led << 17; // Turn on Built-in LED: Output register
				// led = led ^ 1; // toggle
			}
		}
			
	};

	return (0);
}


void GCLK_setup() {
	
// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0; 
	//SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable
	
// Power Manager
	PM->APBCMASK.bit.SERCOM5_ = 1 ; // Clock Enable (APBC clock)
	
//
// * Generator #0 is taking the clock source #6 (OSC8M: 8MHz clock input) as an input 
//
	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	// GCLK->GENCTRL.bit.OE = 1 ;  // Output Enable: GCLK_IO
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable

	// GCLK->GENDIV.bit.ID = 1 ; // Generator #1 to check DIV value in debugger

//
// * SERCOM5: USART
// * Generator #0 is feeding USART as well
//
	GCLK->CLKCTRL.bit.ID = 0x19; // ID #0x19 (SERCOM5: USUART)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!
}


void PORT_setup() {
	
//
// PORT setup for Built-in LED output (PA17)
//
// 	PORT->Group[0].PINCFG[17].reg = 0x0; // peripheral mux enable = 0
// 	PORT->Group[0].DIR.reg = 0x1 << 17; // Direction: Output

// //
// // PORT setup for PA14 (GCLK_IO[0]) to check out clock output using logic analyzer
// //
// 	PORT->Group[0].PINCFG[14].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
// 	PORT->Group[0].PMUX[7].bit.PMUXE = 0x07; // peripheral function H selected

//
// PORT setup for PB22 and PB23 (USART)
//
	PORT->Group[1].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[1].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[1].PMUX[11].bit.PMUXE = 0x03; // peripheral function D selected	
	PORT->Group[1].PMUX[11].bit.PMUXO = 0x03; // peripheral function D selected	
}


void USART_setup() {
		
	//
	// USART setup
	//
	SERCOM5->USART.CTRLA.bit.MODE = 1 ; // Internal Clock
	SERCOM5->USART.CTRLA.bit.CMODE = 0 ; // Asynchronous UART
	SERCOM5->USART.CTRLA.bit.RXPO = 3 ; // PAD3
	SERCOM5->USART.CTRLA.bit.TXPO = 1 ; // PAD2
	SERCOM5->USART.CTRLB.bit.CHSIZE = 0 ; // 8-bit data
	SERCOM5->USART.CTRLA.bit.DORD = 1 ; // LSB first
	//SERCOM5->USART.CTRLA.bit.SAMPR = 1 ; //

	SERCOM5->USART.BAUD.reg = 0Xc504 ; // 115,200 bps (baud rate) with 8MHz input clock
	//SERCOM5->USART.BAUD.reg = 0Xe282 ; // 115,200 bps (baud rate) with 16MHz input clock
	//SERCOM5->USART.BAUD.reg = 0Xec57 ; // 115,200 bps (baud rate) with 24MHz input clock
	//SERCOM5->USART.BAUD.reg = 0Xf62b ; // 115,200 bps (baud rate) with 48MHz input clock

	SERCOM5->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM5->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM5->USART.CTRLA.bit.ENABLE = 1;
	
}

void SERCOM5_Handler(){

	if(SERCOM5->USART.INTFLAG.bit.RXC){
		rx_data	= SERCOM5->USART.DATA.reg;
		SERCOM5->USART.DATA.reg	= rx_data;
	}
	
}
