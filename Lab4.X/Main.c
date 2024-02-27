/*
 * File:   Main.c
 * Author: Tristen
 *
 * Created on February 27, 2024, 11:49 AM
 */

#include <stdio.h>
#include "xc.h"
#include "BOARD.h"
#include "BNO055.h"
#include <math.h>

#define PI (acos(-1.0))
// Struct definition for a 3x3 matrix

typedef struct {
    float data[3][3];
} Matrix3x3;

// Function to get the value at a specific row and column in the matrix

float getEntry(const Matrix3x3 *matrix, int row, int col)
{
    return matrix->data[row][col];
}

float convertRadToDeg(float rads)
{
    return rads * (180 / PI);
}

float getTheta(Matrix3x3 *matrix)
{
    float raw = getEntry(matrix, 0, 2);
    float rads = -asin(raw);
    return convertRadToDeg(rads);
}

float getPsi(Matrix3x3 *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 0, 0);
    raw /= cos(theta);
    float rads = acos(raw);
    return convertRadToDeg(rads);
}

float getPhi(Matrix3x3 *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 2, 2);
    raw /= cos(theta);
    float rads = acos(raw);
    return convertRadToDeg(rads);
}

int main(void)
{
    BOARD_Init();
    Matrix3x3 foo = {
        {
            {0.8293, 0.5498, -0.0998},
            {-0.5581, 0.8059, -0.1977},
            {-0.0282, 0.2197, 0.9752}}
    } ;
    float theta = getTheta(&foo);
    float psi = getPsi(&foo);
    float phi = getPhi(&foo);
    printf("Theta:%f  Psi:%f  Phi:%f\n", theta, psi, phi);
    return 0;
}
