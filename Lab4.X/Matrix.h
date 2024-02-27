/* 
 * File:   Matrix.h
 * Author: Nadia
 *
 * Created on February 27, 2024, 3:38 PM
 */

#ifndef MATRIX_H
#define	MATRIX_H

#include <stdio.h>
#include "xc.h"
#include "BOARD.h"
#include "BNO055.h"
#include <math.h>

// Function to get the value at a specific row and column in the matrix

/**
 * Function to get the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @return The value at the specified row and column.
 */
float getEntry(const Matrix3x3 *matrix, int row, int col);

/**
 * Function to convert radians to degrees.
 * @param rads Value in radians.
 * @return Equivalent value in degrees.
 */
float convertRadToDeg(float rads);

/**
 * Function to calculate the theta angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Theta angle in radians.
 */
float getTheta(Matrix3x3 *matrix);

/**
 * Function to calculate the psi angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Psi angle in radians.
 */
float getPsi(Matrix3x3 *matrix);

/**
 * Function to calculate the phi angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Phi angle in radians.
 */
float getPhi(Matrix3x3 *matrix);

#endif	/* MATRIX_H */

