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
    return rads;
}

float getPsi(Matrix3x3 *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 0, 1);
    printf("Raw psi = %f\n", raw);
    raw /= cos(theta);
    printf("Psi after division by cos(theta): %f\n", raw);
    float rads = asin(raw);
    return (rads);
}

float getPhi(Matrix3x3 *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 1, 2);
    raw /= cos(theta);
    float rads = asin(raw);
    return (rads);
}

int main(void)
{
    BOARD_Init();
    Matrix3x3 foo = {
        {
            {0.8293, 0.5498, -0.0998},
            {-0.5581, 0.8059, -0.1977},
            {-0.0282, 0.2197, 0.9752}
        }
    };
    float theta = getTheta(&foo);
    theta = convertRadToDeg(theta);
    float psi = getPsi(&foo);
    psi = convertRadToDeg(psi);
    float phi = getPhi(&foo);
    phi = convertRadToDeg(phi);
    printf("Theta:%f  Psi:%f  Phi:%f\n", theta, psi, phi); // Expected values: Theta:5.727654  Psi:33.544071  Phi:-11.460480
    Matrix3x3 foo2 = {
        {
            {0.8293, -0.5498, 0.0998},
            {0.5581, 0.8059, -0.1977},
            {0.0282, 0.2197, 0.9752}
        }
    };
    theta = getTheta(&foo2);
    theta = convertRadToDeg(theta);
    psi = getPsi(&foo2);
    psi = convertRadToDeg(psi);
    phi = getPhi(&foo2);
    phi = convertRadToDeg(phi);
    printf("Theta:%f  Psi:%f  Phi:%f\n", theta, psi, phi); //Theta:-5.727654  Psi:-33.542721  Phi:-11.460480
    return 0;
}

