/*
 * File:   Main.c
 * Author: Tristen Miller
 *
 * Created on February 27, 2024, 11:49 AM
 */
#include <math.h>
#include <stdio.h>

#include "xc.h"
#include "BOARD.h"
#include "BNO055.h"
#include "Oled.h"

// defines for code testing
//#define Matrix_Test

#define PI (acos(-1.0))


//Struct definition for a 3x3 matrix

typedef struct {
    float data[3][3];
} Matrix3x3;



// Function to get the value at a specific row and column in the matrix

/**
 * Function to get the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @return The value at the specified row and column.
 */
float getEntry(const Matrix3x3 *matrix, int row, int col) {
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
float getTheta(Matrix3x3 *matrix) {
    float raw = getEntry(matrix, 0, 2);
    float rads = -asin(raw);
    return rads;
}

/**
 * Function to calculate the psi angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Psi angle in radians.
 */
float getPsi(Matrix3x3 *matrix) {
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
float getPhi(Matrix3x3 *matrix) {
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 1, 2);
    raw /= cos(theta);
    float rads = asin(raw);
    return rads;
}

#ifdef Matrix_Test
int main(void) {
    BOARD_Init();
    OledInit();
    char msg[OLED_DRIVER_BUFFER_SIZE]; //Variable to sprintf messages to the oled
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
    sprintf(msg, "Theta:%f\nPsi  :%f\nPhi  :%f\n", theta, psi, phi); // Expected values: Theta:5.727654  Psi:33.544071  Phi:-11.460480
    OledDrawString(msg);
    OledUpdate();
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
    sprintf(msg, "Theta:%f\nPsi  :%f\nPhi  :%f\n", theta, psi, phi); //Theta:-5.727654  Psi:-33.542721  Phi:-11.460480
    OledDrawString(msg);
    OledUpdate();
    return 0;
}
#endif

