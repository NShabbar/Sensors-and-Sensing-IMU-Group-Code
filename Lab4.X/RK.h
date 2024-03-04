/* 
 * File:   RK.h
 * Author: Nadia
 *
 * Created on February 28, 2024, 11:55 AM
 */

#ifndef RK_H
#define	RK_H

#include "Matrix.h"

#define EPSILON 0.0001 // Threshold for switching to sin function from Taylor Expansion
#define Kp_a 10
#define Kp_m 10
#define Ki_a Kp_a/10
#define Ki_m Kp_m/10

//typedef struct {
//    float data[3][1];
//} Matrix3x1;

float sinw_over_w(float w, float time);

// function to compute 1 - cos(w delta T))
float cosw(float w, float time);

// function to compute 1 - cos(w delta T))
float cosw(float w, float time);

// this function takes the w = [p, q, r] transpose, and finds the magnitutde.
float w_mag(Matrix3x1 w);

// this function performs forward integration of Rk+1 = Rk - [wx]Rk * dt
Matrix3x3 RK_Forward_Integration(Matrix3x3 RK, Matrix3x1 w, float dt);

// this function performs the matrix exponential form Rk+1 = e^(-[wx]dt) *Rk
Matrix3x3 RK_Exp_Integration(Matrix3x3 RK, Matrix3x1 w, float dt);

Matrix3x3 RK_Exp_Closed_Loop(Matrix3x3 RK, Matrix3x1 bias, Matrix3x1 gyro, Matrix3x1 mags, Matrix3x1 accels, Matrix3x1 mag_intertial, Matrix3x1 accel_intertial, float dt);
#endif	/* RK_H */

