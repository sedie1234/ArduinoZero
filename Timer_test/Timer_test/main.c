#include "sam.h"


void Print(char* str, int size);

void PORT_setup();
void GCLK_setup();
void USART_setup();
void TCC0_setup();
void SERCOM5_Handler();

char rx_data;

int main()
{
	char str[100] = "start\r\n";
	
	/* Initialize the SAM system */
    SystemInit();
	
	GCLK_setup();
	
	PORT_setup();
	
	USART_setup();
	
	TCC0_setup();
	
	SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
	NVIC_EnableIRQ(SERCOM5_IRQn);

	Print(str, sizeof(str));
	while(1) {
		
	}
}



void GCLK_setup() {

	// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;
	//SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable

	PM->APBCMASK.bit.SERCOM5_ = 1 ;
	PM->APBCMASK.bit.TCC0_ = 1 ;

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
	
	GCLK->CLKCTRL.bit.ID = 0x1A;	//ID = TCC0, TCC1
	GCLK->CLKCTRL.bit.GEN = 0;
	GCLK->CLKCTRL.bit.CLKEN = 1;

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

void TCC0_setup(){
	
	//PORT->Group[0].DIRSET.reg = PORT_PA18;
	//PORT->Group[0].OUTCLR.reg = PORT_PA18;
	
	PORT->Group[0].PINCFG[18].reg = 0x41;
	PORT->Group[0].PMUX[9].bit.PMUXE = 0x05;
	
	TCC0->CTRLA.bit.PRESCALER = 0x03; //DIV8 : 8MHz -> 1MHz
	//TCC0->CTRLA.bit.PRESCSYNC = 1;
	TCC0->WAVE.bit.WAVEGEN = 0x02;
	//TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;

	TCC0->PER.reg = 20000; //1MHz x 20000 = 20ms
	TCC0->CC[2].reg = 1000; // 1ms
	TCC0->CTRLA.bit.ENABLE = 1;
	
	
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
		if(rx_data == 'i'){
			TCC0->CC[2].reg +=10;
		}else if(rx_data == 'd'){
			TCC0->CC[2].reg -=10;
		}
	}
	
}