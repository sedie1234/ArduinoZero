/*
 * configs.c
 *
 * Created: 2023-12-02 오후 10:11:52
 *  Author: hwan
 */ 
#ifndef CONFIGS_C_
#define CONFIGS_C_

#include "configs.h"

int distance;

//for command
int cmd = -1;
int test = 1;
int params[PARAM_SIZE];

//for uart
char rx_data;
char rx_buf[200];
int rx_cnt=0;

//condition flags
int state = STOP_STATE;
int LT0_flag = 0;
int LT1_flag = 0;
int LT2_flag = 0;
int LT0=0;
int LT1=1;
int LT2=0;
int stop_cnt = 0;
int runstart = 0;


#endif