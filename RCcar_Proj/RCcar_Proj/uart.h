/*
 * uart.h
 *
 * Created: 2023-11-29 오후 5:53:59
 *  Author: hwan
 */ 


#ifndef UART_H_
#define UART_H_

#define BLE_USE         0 //BEL <-> PC_COM ON/OFF

void USART_setup();

#if BLE_USE
void SERCOM0_Handler();
#else
void SERCOM5_Handler();

#endif




#endif /* UART_H_ */