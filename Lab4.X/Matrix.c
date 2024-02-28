/*
 * File:   Main.c
 * Author: Tristen Miller
 *
 * Created on February 27, 2024, 11:49 AM
 */
#include <math.h>
#include <stdio.h>

#include "Matrix.h"
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





