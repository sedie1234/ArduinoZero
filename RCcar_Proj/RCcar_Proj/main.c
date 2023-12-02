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
#include "i2c.h"
#include "timer.h"
#include "mems.h"

void GCLK_setup();
void ServoAngle(int angle);

int main(void)
{
	
    SystemInit();
	GCLK_setup();
    USART_setup();
    TCC0_setup();
    GPIO_setup();
    EIC_setup();
    I2C_setup();
	RTC_setup();
    TC4_setup();

//uart interrupt
#if BLE_USE
    SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    NVIC_EnableIRQ(SERCOM0_IRQn);
#else
    SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    NVIC_EnableIRQ(SERCOM5_IRQn);
#endif
	NVIC->ISER[0] |= 1 << 3 ;
	NVIC->IP[0] = 0x40 << 24 ;

    NVIC->ISER[0] |= 1 << 4 ;  // Interrupt Set Enable for EIC
	NVIC->IP[1] = 0x40 << 0 ; // priority for EIC: IP1[7:0] = 0x40 (=b0100_0000, 2-bit MSBs)	

    LIS2DH_I2C_write(0x20, 0x57); // write 0x50 to 0x20 (=CTRL_REG1) register



    /* Replace with your application code */
    while (1) {
        // if(cmd == -1){
        //     //동작 x
        // }else if(cmd == 0){
        //     //run 동작 //->main
        ServoAngle(150);
        // }else if (cmd == 1){
        //     //run 동작 < go , stop, rotation <실험용
        // }else if(cmd == 2){
        //     //go
        // }

    }
}

void GCLK_setup() {
	
// OSC8M
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0; 
	SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable
}