/*
 * RCcar_Proj.c
 *
 * Created: 2023-11-28 오후 7:10:26
 * Author : hwan
 */ 


#include "sam.h"
#include "uart.h"
#include "tcc.h"
#include "gpio.h"

void GCLK_setup();

int main(void)
{

    SystemInit();
	GCLK_setup();
    USART_setup();
    TCC0_setup();
    GPIO_setup();
    EIC_setup();

//uart interrupt
#if BLE_USE
    SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    NVIC_EnableIRQ(SERCOM0_IRQn);
#else
    SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    NVIC_EnableIRQ(SERCOM5_IRQn);
#endif

    NVIC->ISER[0] |= 1 << 4 ;  // Interrupt Set Enable for EIC
	NVIC->IP[1] = 0x40 << 0 ; // priority for EIC: IP1[7:0] = 0x40 (=b0100_0000, 2-bit MSBs)	

	PORT->Group[0].OUT.reg |= 1 << 10;

    /* Replace with your application code */
    while (1) {

    }
}

void GCLK_setup() {
	
// OSC8M
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0; 
	SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable
}