/*
 * RCcar_Proj.c
 *
 * Created: 2023-11-28 오후 7:10:26
 * Author : hwan
 */ 


#include "sam.h"
#include "uart.h"

void GCLK_setup();

int main(void)
{

    SystemInit();
	GCLK_setup();
    USART_setup();	

//uart interrupt
#if BLE_USE
    SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    NVIC_EnableIRQ(SERCOM0_IRQn);
#else
    SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    NVIC_EnableIRQ(SERCOM5_IRQn);
#endif

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