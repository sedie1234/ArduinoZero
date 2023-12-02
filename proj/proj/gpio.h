/*
 * gpio.h
 *
 * Created: 2023-11-29 오후 8:09:03
 *  Author: hwan
 */ 


#ifndef GPIO_H_
#define GPIO_H_


void GPIO_setup();
void EIC_setup();
void EIC_Handler();
void SetHigh(int group, int port);
void SetLow(int group, int port);
int ReadPin(int group, int port);


#endif /* GPIO_H_ */