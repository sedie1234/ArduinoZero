/*
 * util_motor.h
 *
 * Created: 2023-11-30 오전 12:15:37
 *  Author: hwan
 */ 


#ifndef UTIL_MOTOR_H_
#define UTIL_MOTOR_H_

//sel : 0 = motorA, 1 = motorB, 2 = both
//speed : 0~20000
void MotorSpeedSet(int sel, int speed);

//angle : 0~180
void ServoAngle(int angle);

//sel : 0 = motorA, 1 = motorB
//dir : 1 = forward, 2 = backward, 3 = break
void DirectionSet(int sel, int dir);

#endif /* UTIL_MOTOR_H_ */