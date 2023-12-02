/*
 * util_motor.c
 *
 * Created: 2023-11-30 오전 12:15:22
 *  Author: hwan
 */ 
#ifndef UTIL_MOTOR_C_
#define UTIL_MOTOR_C_

#include "sam.h"
#include "util_motor.h"
#include "gpio.h"
#include "configs.h"
#include "timer.h"

void MotorSpeedSet(int sel, int speed){

    if(sel == 0){
        TCC0->CC[1].reg = speed;
    }else if(sel == 1){
        TCC0->CC[0].reg = speed;
    }else if(sel == 2){
        TCC0->CC[0].reg = speed;
        TCC0->CC[1].reg = speed;
    }

}

void ServoAngle(int angle){
    int cc = (1000/180) * angle + 1000;

    TCC0->CC[2].reg = cc;

}

void DirectionSet(int sel, int dir){
    if(sel == 0){
        if(dir == 1){
            SetHigh(0, 20);
            SetLow(0, 21);
        }else if(dir == 2){
            SetLow(0, 20);
            SetHigh(0, 21);
        }else if(dir == 3){
            SetHigh(0, 20);
            SetHigh(0, 21);
        }
    }else if(sel==1){
        if(dir == 1){
            SetLow(0, 2);
            SetHigh(0, 14);
        }else if(dir == 2){
            SetHigh(0, 2);
            SetLow(0, 14);
        }
        else if(dir == 3){
            SetHigh(0, 2);
            SetHigh(0, 14);
        }
    }
}

#endif 