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
#include "util_motor.h"
#include "configs.h"
#include "application.h"

extern int cmd;
extern int test;
extern int params[PARAM_SIZE];

void GCLK_setup();

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

    //LIS2DH_I2C_write(0x20, 0x57); // write 0x50 to 0x20 (=CTRL_REG1) register



    /* Replace with your application code */
    while (1) {
        if(cmd == -1){
            while(test);
            test = 1;
        //none
        }else if(cmd == 0){
        //run command
            AppInit();
			AppRun(params[0], params[1], params[2], params[3], params[4]);
            while(test);
            test = 1;
        }else if (cmd == 1){
        //go command
            FuncGo(params[0]);
            TimerDelay(params[1]*1000);
            FuncStop();
            while(test);
            test = 1;
        }else if (cmd == 2){
        //stop command
            while(test);
            test = 1;
        }else if (cmd == 3){
        //turn command
            FuncTurnLeft(params[0]);
			TimerDelay(params[1]*1000);
            FuncStop();
            while(test);
            test = 1;
        }
    }
}

void GCLK_setup() {
	
// OSC8M
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0; 
	SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable
}