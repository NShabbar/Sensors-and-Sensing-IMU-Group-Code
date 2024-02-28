/* 
 * File:   RK.h
 * Author: Nadia
 *
 * Created on February 28, 2024, 11:55 AM
 */

#ifndef RK_H
#define	RK_H

#define EPSILON 0.0001 // Threshold for switching to sin function from Taylor Expansion
#include "Matrix.h"

typedef struct {
    float data[3];
} Matrix1x3;

float sinw_over_w(float w, float time);

// function to compute 1 - cos(w delta T))
float cosw(float w, float time);

// function to compute 1 - cos(w delta T))
float cosw(float w, float time);

// this function takes the w = [p, q, r] transpose, and finds the magnitutde.
float w_mag(float p, float q, float r);

// this function performs forward integration of Rk+1 = Rk - [wx]Rk * dt
Matrix3x3 RK_Forward_Integration(Matrix3x3 RK, Matrix1x3 w, float dt);

// this function performs the matrix exponential form Rk+1 = e^(-[wx]dt) *Rk
Matrix3x3 RK_Exp_Integration(Matrix3x3 RK, Matrix1x3 w, float dt);
#endif	/* RK_H */

