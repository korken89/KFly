#ifndef _KALMAN_H
#define _KALMAN_H

#include "configKFly.h"

#define DT 			1.0f/UPDATE_RATE 
// Q diagonal 3x3 with these elements on diagonal
#define Q1 			5.0f		// How well do the model represent the changes of Angle
#define Q2 			100.0f		// How well do the model represent the changes of Angular Velocity
#define Q3 			0.01f		// How well do the model represent the changes of Gyro Bias (changes very slowly)
// R diagonal 2x2 with these elements on diagonal
#define R1 			1000.0f		// How much do we trust the measurement of Angle
#define R2 			1000.0f		// How much do we trust the measurement of Angular Velocity

struct _kalman_data
{
	float x1, x2, x3;
	float p11, p12, p13, p21, p22, p23, p31, p32, p33;
};
typedef struct _kalman_data kalman_data;

void InitKalman(kalman_data *data);
void UpdKalman(kalman_data *data, float z1, float z2);

#endif
