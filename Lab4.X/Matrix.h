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

typedef struct {
    float data[3][3];
} Matrix3x3;

/**
 * Function to get the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @return The value at the specified row and column.
 */
float getEntry(const Matrix3x3 *matrix, int row, int col);
/**
 * Function to set the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @param value The value to set at the specified row and column.
 */
void setEntry(Matrix3x3 *matrix, int row, int col, float value);

/**
 * Function to perform scalar multiplication on the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param scalar The scalar value to multiply the matrix by.
 */
void scalarMult(Matrix3x3 *matrix, float scalar);

/**
 * Function to compute the dot product of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the dot product.
 */
Matrix3x3 dotProduct(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2);

/**
 * Function to compute the subtraction of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the subtraction.
 */
Matrix3x3 subtraction(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2);



#endif	/* MATRIX_H */

