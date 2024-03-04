/*
 * File:   Matrix.c
 * Author: Tristen Miller
 *
 * Created on February 27, 2024, 11:49 AM
 */
#include <math.h>
#include <stdio.h>

#include "Matrix.h"
//#define MATRIX_TEST
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

float getEntry3x1(const Matrix3x1 *matrix, int row) {
    return matrix->data[row][0];
}

float getEntry1x3(const Matrix1x3 *matrix, int col) {
    return matrix->data[0][col];
}

/**
 * Function to set the value at a specific row and column in the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param row Row index (0-based) of the element.
 * @param col Column index (0-based) of the element.
 * @param value The value to set at the specified row and column.
 */
void setEntry(Matrix3x3 *matrix, int row, int col, float value) {
    matrix->data[row][col] = value;
}

void setEntry3x1(Matrix3x1 *matrix, int row, float value) {
    matrix->data[row][0] = value;
}

void setEntry1x3(Matrix1x3 *matrix, int col, float value) {
    matrix->data[0][col] = value;
}

/**
 * Function to transpose a 1x3 to a 3x1
 * */
Matrix3x1 transpose1x3to3x1(Matrix1x3 *matrix) {
    Matrix3x1 transposed;
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            transposed.data[j][i] = matrix->data[i][j];
        }
    }
    return transposed;
}

Matrix1x3 transpose3x1to1x3(Matrix3x1 *matrix) {
    Matrix1x3 transposed;
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            transposed.data[i][j] = matrix->data[j][i];
        }
    }
    return transposed;
}

/**
 * Function to perform scalar multiplication on the matrix.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @param scalar The scalar value to multiply the matrix by.
 */
void scalarMult(Matrix3x3 *matrix, float scalar) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            setEntry(matrix, i, j, getEntry(matrix, i, j) * scalar);
        }
    }
}

Matrix3x1 scalarMult3x1(Matrix3x1 *matrix, float scalar) {
    Matrix3x1 result = {{
        {0},{0},{0}}};
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            setEntry3x1(&result, j, getEntry3x1(matrix, j) * scalar);
        }
    }
    return result;
   
}

void scalarMult1x3(Matrix1x3 *matrix, float scalar) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 1; j++) {
            setEntry1x3(matrix, j, getEntry1x3(matrix, j) * scalar);
        }
    }
}
/**
 * Function to compute the dot product of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the dot product.
 */
Matrix3x3 dotProduct(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2) {
    Matrix3x3 result = {
        {
            {0}}}; // Initialize result matrix with zeros

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

Matrix3x1 dotProduct3x1(const Matrix3x3 *matrix1, const Matrix3x1 *matrix2) {
    Matrix3x1 result = {{
        
        {0},{0},{0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 3; i++) {
        float sum = 0.0;
        for (int j = 0; j < 3; j++) {
                sum += getEntry(matrix1, i, j) * getEntry3x1(matrix2, j);
                //printf("%d %d: %f\n", i, j, getEntry(matrix1, i, j) * getEntry3x1(matrix2, j));
        }
        
        setEntry3x1(&result, i, sum);
    }

    return result;
}
/**
 * Function to compute the subtraction of two matrices.
 * @param matrix1 Pointer to the first Matrix3x3 struct.
 * @param matrix2 Pointer to the second Matrix3x3 struct.
 * @return The resulting Matrix3x3 object representing the subtraction Matrix1 - Matrix2.
 */
Matrix3x3 subtraction3x3(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2) {
    Matrix3x3 result = {
        {
            {0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float value = getEntry(matrix1, i, j) - getEntry(matrix2, i, j);
            setEntry(&result, i, j, value);
        }
    }

    return result;
}

Matrix3x1 subtraction3x1(const Matrix3x1 *matrix1, const Matrix3x1 *matrix2) {
    Matrix3x1 result = {
        {
            {0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            float value = getEntry3x1(matrix1, j) - getEntry3x1(matrix2, j);
            setEntry3x1(&result, j, value);
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
Matrix3x3 addition(const Matrix3x3 *matrix1, const Matrix3x3 *matrix2) {
    Matrix3x3 result = {
        {
            {0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float value = getEntry(matrix1, i, j) + getEntry(matrix2, i, j);
            setEntry(&result, i, j, value);
        }
    }

    return result;
}

Matrix3x1 addition3x1(const Matrix3x1 *matrix1, const Matrix3x1 *matrix2) {
    Matrix3x1 result = {
        {
            {0}}}; // Initialize result matrix with zeros

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            float value = getEntry3x1(matrix1, j) + getEntry3x1(matrix2, j);
            setEntry3x1(&result, j, value);
        }
    }

    return result;
}
// Function to print a matrix

void printMatrix(const Matrix3x3 *matrix) {
    printf("[");
    for (int i = 0; i < 3; i++) {
        printf("[");
        for (int j = 0; j < 3; j++) {
            printf("%.6f", getEntry(matrix, i, j));
            if (j < 2) {
                printf(",");
            }
        }
        printf("]");
        if (i < 2) {
            printf(";\n");
        }
    }
    printf("]\n");
}

void printMatrix3x1(const Matrix3x1 *matrix) {
    printf("[");
    for (int i = 0; i < 3; i++) {
        printf("[");
        for (int j = 0; j < 1; j++) {
            printf("%.6f", getEntry3x1(matrix, i));
            if (j < 2) {
                printf(",");
            }
        }
        printf("]");
        if (i < 2) {
            printf(";\n");
        }
    }
    printf("]\n");
}

//#define MATRIX_TEST

#ifdef MATRIX_TEST
#include <assert.h>
#include "BOARD.h"
/*
int main() {
    BOARD_Init();
    // Example matrices
    Matrix3x3 matrix1 = {
        {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0}}
    };

    Matrix3x3 matrix2 = {
        {
            {9.0, 8.0, 7.0},
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
    while (1);
    return 0;
*/
#endif

