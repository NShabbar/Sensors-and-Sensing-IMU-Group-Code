/* 
 * File:   RK.c
 * Author: Nadia
 *
 * Created on February 28, 2024, 11:54 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "xc.h"
#include "BOARD.h"
#include "BNO055.h"
#include "Oled.h"
#include "RK.h"

// defines for testing
#define RK_test
#define sin_cos_test
//#define mag_test
// Function to compute the Taylor series approximation for sin(w deltaT)/w

float sinw_over_w(float w, float time) {
    if (fabs(w) < EPSILON) { // fabs takes the absolute val, 120 is the factorial of 5
        // Taylor series approximation
        return time - ((pow(time, 3) * pow((w), 2)) / 6.0) + (((pow(time, 5) * pow((w), 4)) / 120.0));
    } else {
        // Switch to sin function
        return sin(w * time) / w;
    }
}

// function to compute 1 - cos(w delta T))
float cosw(float w, float time) {
    return 1 - cos(w * time);
}

float w_mag(float p, float q, float r){
    float mag = sqrt(pow(p, 2) + pow( q, 2) + pow( r, 2));
    return mag;
}

#ifdef RK_test
int main(int argc, char** argv) {
    // Example usage
    BOARD_Init();
    OledInit();
    char msg[OLED_DRIVER_BUFFER_SIZE]; //Variable to sprintf messages to the oled
#ifdef sin_cos_test
    float t = 1; // test time
    float omega_sin = 0.01; // test w
    float result = sinw_over_w(omega_sin, t);
    
    float omega_sin_small = 0.000001; // test w
    float res_small = sinw_over_w(omega_sin_small, t);
    
    float omega_cos = 0.01;
    float res_cos = cosw(omega_cos, t);
    
    float omega_cos_small = 0.000001;
    float res_cos_small = cosw(omega_cos_small, t);
    sprintf(msg, "Result: %f, %f, %f, %f\n", result, res_small, res_cos, res_cos_small);
    OledDrawString(msg);
    OledUpdate();
#endif

#ifdef mag_test
    float p = 3.5;
    float q = 4.0;
    float r = 2.5;
    float result = w_mag(p, q, r);
    sprintf(msg, "Magnitude of w: %f\n", result);
    OledDrawString(msg);
    OledUpdate();
#endif
    return (EXIT_SUCCESS);
}
#endif
