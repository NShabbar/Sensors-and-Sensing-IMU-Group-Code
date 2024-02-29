/*
 * File:   Matrix.c
 * Author: Tristen Miller
 *
 * Created on February 27, 2024, 11:49 AM
 */
#include <math.h>
#include <stdio.h>

#include "Matrix.h"

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
 * Function to set the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @param value The value to set at the specified row and column.
 */
void setEntry(Matrix3x3 *matrix, int row, int col, float value)
{
    matrix->data[row][col] = value;
}

/**
 * Function to perform scalar multiplication on the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param scalar The scalar value to multiply the matrix by.
 */
void scalarMult(Matrix3x3 *matrix, float scalar)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            setEntry(matrix, i, j, getEntry(matrix, i, j) * scalar);
        }
    }
}

/**
 * Function to compute the dot product of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the dot product.
 */
Matrix3x3 dotProduct(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2)
{
    Matrix3x3 result = {{{0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float sum = 0.0;
            for (int k = 0; k < 3; k++) {
                sum += getEntry(matrix1, i, k) * getEntry(matrix2, k, j);
            }
            setEntry(&result, i, j, sum);
        }
    }

    return result;
}

/**
 * Function to compute the subtraction of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the subtraction.
 */
Matrix3x3 subtraction(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2)
{
    Matrix3x3 result = {{{0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float value = getEntry(matrix1, i, j) - getEntry(matrix2, i, j);
            setEntry(&result, i, j, value);
        }
    }

    return result;
}
/**
 * Function to compute the addition of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the addition.
 */
Matrix3x3 addition(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2){
        Matrix3x3 result = {{{0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float value = getEntry(matrix1, i, j) + getEntry(matrix2, i, j);
            setEntry(&result, i, j, value);
        }
    }

    return result;
}
// Function to print a matrix
void printMatrix(const Matrix3x3 *matrix) {
    printf("Matrix:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.6f  ", getEntry(matrix, i, j));
        }
        printf("\n");
    }
}
//#define MATRIX_TEST

#ifdef MATRIX_TEST
#include <assert.h>
int main() {
    // Example matrices
    Matrix3x3 matrix1 = {
        {{1.0, 2.0, 3.0},
         {4.0, 5.0, 6.0},
         {7.0, 8.0, 9.0}}
    };

    Matrix3x3 matrix2 = {
        {{9.0, 8.0, 7.0},
         {6.0, 5.0, 4.0},
         {3.0, 2.0, 1.0}}
    };

    // Test getEntry function
    assert(getEntry(&matrix1, 0, 0) == 1.0);
    assert(getEntry(&matrix1, 1, 1) == 5.0);
    assert(getEntry(&matrix1, 2, 2) == 9.0);

    // Test setEntry function
    setEntry(&matrix1, 0, 0, 10.0);
    assert(getEntry(&matrix1, 0, 0) == 10.0);

    // Test scalarMult function
    scalarMult(&matrix1, 2.0);
    assert(getEntry(&matrix1, 0, 0) == 20.0);

    // Test dotProduct function
    Matrix3x3 dotProductResult = dotProduct(&matrix1, &matrix2);
    printMatrix(&dotProductResult);
    // Assert statements for dotProductResult can be added here

    // Test subtraction function
    Matrix3x3 subtractionResult = subtraction(&matrix1, &matrix2);
    printMatrix(&subtractionResult);
    // Assert statements for subtractionResult can be added here

    printf("All tests passed!\n");

    return 0;
}
#endif

