/*
 * application.h
 *
 * Created: 2023-12-02 오후 9:20:21
 *  Author: hwan
 */ 


#ifndef APPLICATION_H_
#define APPLICATION_H_


/****applictions****/
void AppGo();


/******functions******/
//motor functions
void FuncGo();
void FuncGoBack();
void FuncTurnLeft(int speed, int time);
void FuncTurnRight(int speed, int time);

#endif /* APPLICATION_H_ */