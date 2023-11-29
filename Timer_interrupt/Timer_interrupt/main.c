#include "sam.h"

void GCLK_setup();
void PORT_setup();
void USART_setup();
void RTC_setup();
void SERCOM5_Handler();
void RTC_handler(void);
void Print(char* str, int size);

char rx_data;

int main(void)
{
	char str[100] = "start\r\n";
	
	SystemInit();
	
	GCLK_setup();
	
	PORT_setup();
	
	USART_setup();
	
	RTC_setup();
	
	SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
	NVIC_EnableIRQ(SERCOM5_IRQn);
	NVIC->ISER[0] = 1 << 3 ;
	NVIC->IP[0] = 0x40 << 24 ;

	Print(str, sizeof(str));
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

void GCLK_setup() {

	// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;
	//SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable

	PM->APBCMASK.bit.SERCOM5_ = 1 ;

	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	GCLK->GENCTRL.bit.OE = 1 ;  // Output Enable: GCLK_I
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable
	
	GCLK->CLKCTRL.bit.ID = 0x19; // ID #0x19 (SERCOM5: USUART)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!
	
	GCLK->CLKCTRL.bit.ID = 4; // ID #4 (RTC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!
	

}

void PORT_setup(){
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

	SERCOM5->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM5->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM5->USART.CTRLA.bit.ENABLE = 1;
	
}

void Print(char* str, int size){
	int i=0;
	while(1){
		if (SERCOM5->USART.INTFLAG.bit.DRE == 1) {
			SERCOM5->USART.DATA.reg	= *(str+i);
			i++;		
		}
		
		if(i==size)
			break;
	}
	return;
}

void SERCOM5_Handler(){

	if(SERCOM5->USART.INTFLAG.bit.RXC){
		rx_data	= SERCOM5->USART.DATA.reg;
		SERCOM5->USART.DATA.reg	= rx_data;
	}
	
}

void RTC_setup() {

	//
	// RTC setup: MODE0 (32-bit counter) with COMPARE 0
	//
	RTC->MODE0.CTRL.bit.ENABLE = 0; // Disable first
	RTC->MODE0.CTRL.bit.MODE = 0; // Mode 0
	RTC->MODE0.CTRL.bit.MATCHCLR = 1; // match clear
	RTC->MODE0.INTENSET.bit.CMP0 = 1; // compare 0 interrupt is enabled

	RTC->MODE0.COMP->reg = 0x800000; // compare register to set up the period
	//RTC->MODE0.COMP->reg = 0x10000; // compare register	to set up the peroid
	RTC->MODE0.COUNT.reg = 0x0; // initialize the counter to 0
	RTC->MODE0.CTRL.bit.ENABLE = 1; // Enable
}

void RTC_Handler(void)
{
	char str[100] = "interrupt!\r\n";
	Print(str, sizeof(str));
	
	
	RTC->MODE0.INTFLAG.bit.CMP0 = 1; // clear overflow interrupt flag
}
