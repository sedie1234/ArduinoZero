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
        if(dir&0x01){
            SetHigh(0, 20);
        }else{
            SetLow(0, 20);
        }

        if(dir&0x02){
            SetHigh(0, 21);
        }else{
            SetLow(0, 21);
        }
    }else if(sel==1){
        if(dir&0x01){
            SetHigh(0, 2);
        }else{
            SetLow(0, 2);
        }

        if(dir&0x02){
            SetHigh(0, 14);
        }else{
            SetLow(0, 14);
        }
    }
}

#endif 