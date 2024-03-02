/*DCM.c
 * Author: Tristen Miller
 * Implements functions to get Euler angles from a DCM (3x3 Matrix) */

#include "DCM.h"
#include "Matrix.h"

//#define DCM_TEST
/**
 * Function to convert radians to degrees.
 * @param rads Value in radians.
 * @return Equivalent value in degrees.
 */
float convertRadToDeg(float rads)
{
    return rads * (180 / PI);
}

float convertDegToRad(float degs)
{
    return degs * (PI / 180);
}


/**
 * Function to calculate the theta angle in radians.
 * @param matrix Pointer to the Matrix3x3 struct.
 * @return Theta angle in radians.
 */
float getTheta(DCM *matrix)
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
float getPsi(DCM *matrix)
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
float getPhi(DCM *matrix)
{
    float theta = getTheta(matrix);
    float raw = getEntry(matrix, 1, 2);
    raw /= cos(theta);
    float rads = asin(raw);
    return rads;
}

//#define DCM_TEST
#ifdef DCM_TEST
/*
int main(void)
{
    #include "Oled.h"
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
        Matrix3x3 foo3 = {
        {
            {-0.7371, 0.3286, -0.5905},
            {0.3443, 0.9345, 0.0902},
            {0.5815, -0.1368, -0.8020}
        }
    };
        theta = getTheta(&foo3);   
       // theta = convertRadToDeg(theta);
        psi = getPsi(&foo3);
       // psi = convertRadToDeg(psi);
        phi = getPhi(&foo3);
      //  phi = convertRadToDeg(phi);
        printf("Pitch:%f  Yaw:%f  Roll:%f\n",theta,psi,phi);
        while(1);
    return 0;
}*/
#endif
