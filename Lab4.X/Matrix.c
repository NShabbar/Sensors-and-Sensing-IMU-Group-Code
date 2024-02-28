/*
 * File:   Main.c
 * Author: Tristen Miller
 *
 * Created on February 27, 2024, 11:49 AM
 */

#include <stdio.h>
#include "xc.h"
#include "BOARD.h"
#include "BNO055.h"
#include <math.h>

// defines for code testing
//#define Matrix_Test
#define Sin_Taylor_Test

#define PI (acos(-1.0))
// Struct definition for a 3x3 matrix
#define EPSILON 0.0001 // Threshold for switching to sin function from Taylor Expansion

typedef struct {
    float data[3][3];
} Matrix3x3;

// Function to compute the Taylor series approximation for sin(x?T)/x
float sinx_over_x(float w, float time) {
    if (fabs(w) < EPSILON) { // fabs takes the absolute val, 120 is the factorial of 5
        // Taylor series approximation
        return 1.0 - (pow((w*time), 2) / 6.0) + (pow((w*time), 4) / 120.0);
    } else {
        // Switch to sin function
        return sin(w*time) / w;
    }
}

float cosx_over_x_squared(float x) {
    if (fabs(x) < EPSILON) { // fabs takes the absolute val, 120 is the factorial of 5
        // Taylor series approximation
        return (pow(x, 2) / 6.0) + (pow(x, 4) / 120.0);
    } else {
        // Switch to sin function
        return 1 - cos(w*time);
    }
}

// Function to get the value at a specific row and column in the matrix

/**
 * Function to get the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @return The value at the specified row and column.
 */
float getEntry(const Matrix3x3 *matrix, int row, int col)
{
    return matrix->data[row][col];
}

/**
 * Function to convert radians to degrees.
 * @param rads Value in radians.
 * @return Equivalent value in degrees.
 */
float convertRadToDeg(float rads)
{ 
    return rads * (180 / PI);
}

/**
 * Function to calculate the theta angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Theta angle in radians.
 */
float getTheta(Matrix3x3 *matrix)
{
    float raw = getEntry(matrix, 0, 2);
    float rads = -asin(raw);
    return rads;
}

/**
 * Function to calculate the psi angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Psi angle in radians.
 */
float getPsi(Matrix3x3 *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 0, 1);
    raw /= cos(theta);
    float rads = asin(raw);
    return rads;
}

/**
 * Function to calculate the phi angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Phi angle in radians.
 */
float getPhi(Matrix3x3 *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 1, 2);
    raw /= cos(theta);
    float rads = asin(raw);
    return rads;
}

int main(void)
{
#ifdef Matrix_Test
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
#endif
    
#ifdef Sin_Taylor_Test
    // Example usage
    float omega_delta_t = 0.01; // Example value of ??t
    float result = sinx_over_x(omega_delta_t);
    printf("Result: %f\n", result);
#endif
    return 0;
}

