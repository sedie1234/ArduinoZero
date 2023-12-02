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

extern int state;
extern int LT0_flag;
extern int LT1_flag;
extern int LT2_flag;
extern int stop_cnt;

extern int LT0;
extern int LT1;
extern int LT2;

extern int distance;
extern int runstart;

void AppRun(int default_speed, int diff_speed, int turning_speed, int threshould, int end_condition){

    runstart = 1;

    while(1){
        if(state == INIT_STATE){
            FuncGo(default_speed);
            state = GO_STATE;
        }else if(state == STOP_STATE){
            stop_cnt++;
            FuncStop();
            if(stop_cnt < end_condition){
                state = TURN_STATE;
            }else{
                break;
            }
        }else if(state == GO_STATE){

            if(LT0_flag){
                if(LT0){
                    FuncGoLeft(default_speed-diff_speed, default_speed);
                }
                LT0_flag=0;
            }

            if(LT1_flag){
                if(LT1){
                    FuncGo(default_speed);
                }
                LT1_flag=0;
            }

            if(LT2_flag){
                if(LT2){
                    FuncGoRight(default_speed-diff_speed, default_speed);
                }
                LT2_flag=0;
            }

            if(distance < threshould){
                state = PREV_STOP_STATE;
            }

        }else if(state == PREV_STOP_STATE){
            static int condition1;
            static int condition2;
            static int condition3;

            if(LT0_flag){
                if(LT0==1){
                    condition2 = 1;
                }else{
                    if(condition2 == 1){
                        condition3 = 1;
                    }
                }
                LT0_flag = 0;
            }

            if(LT1_flag){
                if(LT1==0){
                    condition1 = 1;
                }else{
                    condition1 = 0;
                }
                LT1_flag = 0;
            }
            
            if(LT2_flag){
                LT2_flag = 0;
            }

            if(condition1 && condition2 && condition3){
                condition1 = 0;
                condition2 = 0;
                condition3 = 0;
                state = STOP_STATE;
            }

        }else if(state == TURN_STATE){
            static int condition1;
            static int condition2;
            static int condition3;

            FuncTurnLeft(turning_speed);
            if(LT0_flag){
                if(LT0){
                    condition1 = 1;
                }else{
                    condition2 = 1;
                }
                LT0_flag = 0;
            }
            if(LT1_flag){
                if(LT1){
                    condition3 = 1;
                }
                LT1_flag = 0;
            }
            if(LT2_flag){
                LT2_flag = 0;
            }    

            if(condition1 & condition2 & condition3){
                FuncStop();
                condition1 = 0;
                condition2 = 0;
                condition3 = 0;
                state = GO_STATE;
            }
        }else{
            state = STOP_STATE;
            stop_cnt = 99999;
        }    
    }
    runstart = 0;

}

void AppInit(){
    state = INIT_STATE;
    LT0_flag = 0;
    LT1_flag = 0;
    LT2_flag = 0;
    stop_cnt = 0;
    runstart = 0;
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

void FuncStop(){

    MotorSpeedSet(0, 0);
    DirectionSet(0, 2);
    DirectionSet(1, 2);
    MotorSpeedSet(2, 0);
    
}

void FuncTurnLeft(int speed){
    MotorSpeedSet(2, 0);
    DirectionSet(LEFT, 2);
    DirectionSet(RIGHT, 1);
    MotorSpeedSet(2, speed);
}

void FuncTurnRight(int speed){
    MotorSpeedSet(2, 0);
    DirectionSet(LEFT, 1);
    DirectionSet(RIGHT, 2);
    MotorSpeedSet(2, speed);
    }

void FuncGoLeft(int low_speed, int high_speed){
    MotorSpeedSet(LEFT, low_speed);
    MotorSpeedSet(RIGHT, high_speed);
}

void FuncGoRight(int low_speed, int high_speed){
    MotorSpeedSet(RIGHT, low_speed);
    MotorSpeedSet(LEFT, high_speed);
}
#endif