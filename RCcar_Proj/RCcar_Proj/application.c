/*
 * application.c
 *
 * Created: 2023-12-02 오후 9:20:05
 *  Author: hwan
 */ 
#ifndef APPLICATION_C_
#define APPLICATION_C_

#include "sam.h"
#include "application.h"
#include "util_motor.h"
#include "configs.h"
#include "timer.h"

void AppGo(){

}

void FuncGo(int speed){

    MotorSpeedSet(0, 0);
    DirectionSet(0, 1);
    DirectionSet(1, 1);
    MotorSpeedSet(2, speed);
    
}

void FuncGoBack(int speed){

    MotorSpeedSet(0, 0);
    DirectionSet(0, 2);
    DirectionSet(1, 2);
    MotorSpeedSet(2, speed);
    
}

void FuncTurnLeft(int speed, int time){
    MotorSpeedSet(2, 0);
    DirectionSet(Left, 2);
    DirectionSet(Rihgt, 1);
    MotorSpeedSet(2, speed);
    TimerDelay(time);
    MotorSpeedSet(2, 0);
}

void FuncTurnRight(int speed, int time){
    MotorSpeedSet(2, 0);
    DirectionSet(Left, 1);
    DirectionSet(Rihgt, 2);
    MotorSpeedSet(2, speed);
    TimerDelay(time);
    MotorSpeedSet(2, 0);
}
#endif