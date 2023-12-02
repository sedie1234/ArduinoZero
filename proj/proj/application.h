/*
 * application.h
 *
 * Created: 2023-12-02 오후 9:20:21
 *  Author: hwan
 */ 


#ifndef APPLICATION_H_
#define APPLICATION_H_


/****applictions****/
//end_condition : whenever meet wall, +1. it must be turns x 4
void AppRun(int default_speed, int diff_speed, int turning_speed, int threshould, int end_condition);
void AppInit();


/******functions******/
//motor functions
void FuncGo(int speed);
void FuncGoBack(int speed);
void FuncStop();
void FuncTurnLeft(int speed);
void FuncTurnRight(int speed);
void FuncGoLeft(int low_speed, int high_speed);
void FuncGoRight(int low_speed, int high_speed);

#endif /* APPLICATION_H_ */