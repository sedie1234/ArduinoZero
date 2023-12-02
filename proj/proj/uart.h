/*
 * uart.h
 *
 * Created: 2023-11-29 오후 5:53:59
 *  Author: hwan
 */ 


#ifndef UART_H_
#define UART_H_

void USART_setup();
void Print(char* str, int size);
void PrintNum(uint32_t num);
/****Command List****/
//0. run
//1. go
//2. stop
//3. turn
/**0. run command**/
//run default_speed(10000~20000) diff_speed(0000~5000) turning_speed(00000~20000) threshold_distance(mm)(0000) end_condition(00)
//run 00000 0000 00000 0000 00

/**1. go command**/
//go speed(00000~20000) time(ms)
//go 00000 0000
//go params[0] = speed
//go params[1] = time

/**2. stop command**/
//stop 

/**3. turn command**/
//turn speed(00000~20000) time(ms)
//turn 00000 0000
//turn params[0] = speed
//turn params[1] = time

void Parse();
int Str2Int(char* str, int size);

#if BLE_USE
void SERCOM0_Handler();
#else
void SERCOM5_Handler();

#endif




#endif /* UART_H_ */