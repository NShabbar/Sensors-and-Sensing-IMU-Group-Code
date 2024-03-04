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
#include "Matrix.h"
#include "DCM.h"
#include "timers.h"


#define RK_test
//#define sin_cos_test
//#define mag_test
//#define Forward_Exp_test
//#define Drift_test
#define Part5
// Function to compute the Taylor series approximation for sin(w deltaT)/w

Matrix3x1 bias = {
        {
            {0.0},
            {0.0},
            {0.0}
        }
    };

Matrix3x3 Misalignment = {
        {
            {-0.9858, -0.0095, 0.1677},
            {0.0127, 0.9913, 0.1309},
            {-0.1675, 0.1312, -0.9771}
        }
    };


float sinw_over_w(float mag_w, float time) {
    if (fabs(mag_w) < EPSILON) { // fabs takes the absolute val, 120 is the factorial of 5
        // Taylor series approximation
        return time - ((pow(time, 3) * pow((mag_w), 2)) / 6.0) + (((pow(time, 5) * pow((mag_w), 4)) / 120.0));
    } else {
        // Switch to sin function
        return sin(mag_w * time) / mag_w;
    }
}

// function to compute 1 - cos(w delta T))

float cosw_over_w_squared(float mag_w, float time) {
    if (fabs(mag_w) < EPSILON) { // fabs takes the absolute val, 120 is the factorial of 5
        // Taylor series approximation
        return (pow(time, 2) / 2) - ((pow(time, 4) * pow(mag_w, 2)) / 24.0);
    } else {
        return (1 - (cos(mag_w * time))) / (pow(mag_w, 2));
    }
}

// this function takes the w = [p, q, r] transpose, and finds the magnitutde.

float w_mag(Matrix3x1 w) {
    float p = w.data[0][0];
    float q = w.data[1][0];
    float r = w.data[2][0];
    float mag = sqrt(pow(p, 2) + pow(q, 2) + pow(r, 2));
    return mag;
}

Matrix3x3 wx(Matrix3x1 w) {
    Matrix3x3 temp;
    float p = w.data[0][0];
    float q = w.data[1][0];
    float r = w.data[2][0];
    temp.data[0][0] = 0;
    temp.data[0][1] = -r;
    temp.data[0][2] = q;
    temp.data[1][0] = r;
    temp.data[1][1] = 0;
    temp.data[1][2] = -p;
    temp.data[2][0] = -q;
    temp.data[2][1] = p;
    temp.data[2][2] = 0;
    return temp;
}

// this function performs forward integration of Rk+1 = Rk - [wx]Rk * dt

Matrix3x3 RK_Forward_Integration(Matrix3x3 RK, Matrix3x1 w, float dt) {
    Matrix3x3 cross = wx(w); // returns a 3x3 wx
    Matrix3x3 RK_Wcross = dotProduct(&cross, &RK); //[wx]Rk
    scalarMult(&RK_Wcross, dt); // [wx]Rk * dt
    Matrix3x3 RK_1 = subtraction3x3(&RK, &RK_Wcross);
    return RK_1;
}

// this function performs the matrix exponential form Rk+1 = e^(-[wx]dt) *Rk

Matrix3x3 RK_Exp_Integration(Matrix3x3 RK, Matrix3x1 w, float dt) {
    Matrix3x3 I = {
        {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0}
        }
    };
    float mag = w_mag(w); // gets magnitude
    float cos_val = cosw_over_w_squared(mag, dt);
    Matrix3x3 cross = wx(w); // returns a 3x3 wx
    Matrix3x3 wx_wx = dotProduct(&cross, &cross); // squares wx matrix
    scalarMult(&wx_wx, cos_val); // multiplies and updates the wx_wx by scalar val

    float sin_val = sinw_over_w(mag, dt);
    scalarMult(&cross, sin_val);

    Matrix3x3 exponent_p1 = subtraction3x3(&I, &cross); // first part of exponent calc.
    Matrix3x3 exponent = addition(&exponent_p1, &wx_wx); // full exponent calc

    Matrix3x3 RK_1 = dotProduct(&exponent, &RK); // the final output
    return RK_1;
}

Matrix3x3 RK_Exp_Closed_Loop(Matrix3x3 RK, Matrix3x1 bias, Matrix3x1 gyro, Matrix3x1 mags, Matrix3x1 accels, Matrix3x1 mag_inertial, Matrix3x1 accel_inertial, float dt) {
    
//    printMatrix3x1(&accels);
//    printMatrix3x1(&mags);
//    printMatrix3x1(&gyro);

    float accels_magnitude = w_mag(accels); // magnitude of accels
    float mags_magnitude = w_mag(mags); // magnitude of the mags

    if(accels_magnitude <0.0001){
        accels_magnitude=0.0001;
    }
    if(mags_magnitude <0.0001){
        mags_magnitude=0.0001;
    }
    
    Matrix3x1 acceltemp = scalarMult3x1(&accels, (1 / accels_magnitude)); // normalizes the accels
    Matrix3x1 magtemp = scalarMult3x1(&mags, (1 / mags_magnitude)); // normalizes the mags

//    printMatrix3x1(&acceltemp);
//    printMatrix3x1(&magtemp);
        
    float accels_inertial_magnitude = w_mag(accel_inertial); // magnitude of accel_inertial
    float mags_inertial_magnitude = w_mag(mag_inertial); // magnitude of the mag_inertial

    Matrix3x1 accelintertialtemp = scalarMult3x1(&accel_inertial, (1 / accels_inertial_magnitude)); // normalizes the accel inertial
    Matrix3x1 magintertialtemp = scalarMult3x1(&mag_inertial, (1 / mags_inertial_magnitude)); // normalizes the mags inertial
    
//    printMatrix3x1(&accel_inertial);
//    printMatrix3x1(&mag_inertial);
    
    
    Matrix3x1 gyroInputWithBias = subtraction3x1(&gyro, &bias);
    
//    printMatrix3x1(&gyroInputWithBias);
    
    Matrix3x1 wmeas_a_right = dotProduct3x1(&RK, &accelintertialtemp);
    Matrix3x1 wmeas_m_right = dotProduct3x1(&RK, &magintertialtemp);
    
    
    
//    printMatrix3x1(&wmeas_a_right);
//    printMatrix3x1(&wmeas_m_right);
    
    
    Matrix3x3 wmeas_a_left = wx(acceltemp);
    Matrix3x3 wmeas_m_left = wx(magtemp);
    
    
//    printMatrix(&wmeas_a_left);
//    printMatrix(&wmeas_m_left);
    
    Matrix3x1 wmeas_a = dotProduct3x1(&wmeas_a_left, &wmeas_a_right);
    Matrix3x1 wmeas_m = dotProduct3x1(&wmeas_m_left, &wmeas_m_right);

//    printMatrix3x1(&wmeas_a);
//    printMatrix3x1(&wmeas_m);
    
    Matrix3x1 wmeasxKpa = scalarMult3x1(&wmeas_a, Kp_a);
    Matrix3x1 wmeasxKpm = scalarMult3x1(&wmeas_m, Kp_m);

    
    
    
//    printMatrix3x1(&wmeasxKpa);
//    printMatrix3x1(&wmeasxKpm);
    
    
    
    Matrix3x1 gyroInputWithFeedback_right = addition3x1(&wmeasxKpa, &wmeasxKpm);

    
    
    Matrix3x1 gyroInputWithFeedback = addition3x1(&gyroInputWithBias, &gyroInputWithFeedback_right);
    
//    printMatrix3x1(&gyroInputWithFeedback);
    
    Matrix3x1 wmeasxkia = scalarMult3x1(&wmeas_a, -1 * Ki_a);
    Matrix3x1 wmeasxkim = scalarMult3x1(&wmeas_m, -1 * Ki_m);
    Matrix3x1 bdot = addition3x1(&wmeasxkia, &wmeasxkim);

    Matrix3x3 Rplus_left = RK_Exp_Integration(RK, gyroInputWithFeedback, dt);
    
//    printMatrix(&Rplus_left);
    
    Matrix3x3 RK_1 = Rplus_left;
    
    Matrix3x1 Bplus_right = scalarMult3x1(&bdot, dt);
    
    Matrix3x1 Bplus = addition3x1(&bias ,&Bplus_right);
    bias = Bplus;
//    printMatrix(&RK_1);
//    printMatrix3x1(&Bplus);
    
    return RK_1;
}
//#ifdef RK_test

int main(int argc, char** argv) {
    // Example usage
    BOARD_Init();
    TIMERS_Init();
    BNO055_Init();
    printf("START\n");
    //OledInit();
    //char msg[OLED_DRIVER_BUFFER_SIZE]; //Variable to sprintf messages to the oled
    /*


#ifdef Part5
     * */
    Matrix3x3 I = {
        {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0}
        }
    };

    
    Matrix3x3 Rk_1 = {
        {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0}
        }
    };
    
    float theta; //pitch
    float phi; //roll
    float psi; //yaw
    int prevtime = 0;
    int currtime = 0;
    printf("HERE1\n");
    int runflag=1;
    int maincount=0;
    while (1) {
        currtime = TIMERS_GetMilliSeconds();
        
        if ((currtime - prevtime >= 20) && maincount<10000) {
           
            prevtime = currtime;
            
            float accel_x = BNO055_ReadAccelX();
            float accel_y = BNO055_ReadAccelY();
            float accel_z = BNO055_ReadAccelZ();
            
            float mag_x = BNO055_ReadMagX();
            float mag_y = BNO055_ReadMagY();
            float mag_z = BNO055_ReadMagZ();

            float gyr_x = BNO055_ReadGyroX();
            float gyr_y = BNO055_ReadGyroY();
            float gyr_z = BNO055_ReadGyroZ();
            

            Matrix3x1 gyro = {
                {
                    {convertDegToRad(gyr_x)/131.0},
                    {convertDegToRad(gyr_y)/131.0},
                    {convertDegToRad(gyr_z)/131.0}
                }
            };
            Matrix3x1 mags = {
                {
                    {mag_x},
                    {mag_y},
                    {mag_z}
                }
            };

            Matrix3x1 accels = {
                {
                    {accel_x},
                    {accel_y},
                    {accel_z}
                }
            };

            Matrix3x1 mag_inertial = {
                {
                    {22770.0 / 1000.0},
                    {5329.0 / 1000.0},
                    {41510.2 / 1000.0}
                }
            };

            Matrix3x1 accel_inertial = {
                {
                    {0.0},
                    {0.0},
                    {9.81}
                }
            };
            Matrix3x1 newmags = dotProduct3x1(&Misalignment, &mags);
//            printf("HERE2\n");
            if(maincount==0){
                Rk_1 = RK_Exp_Closed_Loop(I, bias, gyro, newmags, accels, mag_inertial, accel_inertial, 0.02);
                
                
            }
            else {
                Rk_1 = RK_Exp_Closed_Loop(Rk_1, bias, gyro, newmags, accels, mag_inertial, accel_inertial, 0.02);
            }
            maincount++;
            
//            printf("HERE3\n");
//            printMatrix3x1(&gyro);
//            printMatrix3x1(&mags);
//            printMatrix3x1(&accels);
            psi = getPsi(&Rk_1);
            phi = getPhi(&Rk_1);
            theta = getTheta(&Rk_1);
            printf("%f %f %f\n", convertRadToDeg(psi), convertRadToDeg(phi), convertRadToDeg(theta));
//            printMatrix(&Rk_1);
        }
    }
    //#endif
    
//    #ifdef sin_cos_test
//    float t = 1; // test time
//    float omega_sin = 0.01; // test w
//    float result = sinw_over_w(omega_sin, t);
//
//    float omega_sin_small = 0.000001; // test w
//    float res_small = sinw_over_w(omega_sin_small, t);
//
//    float omega_cos = 0.01;
//    float res_cos = cosw(omega_cos, t);
//
//    float omega_cos_small = 0.000001;
//    float res_cos_small = cosw(omega_cos_small, t);
//    sprintf(msg, "Result: %f, %f, %f, %f\n", result, res_small, res_cos, res_cos_small);
//    OledDrawString(msg);
//    OledUpdate();
//#endif
//
//#ifdef mag_test
//    float p = 3.5;
//    float q = 4.0;
//    float r = 2.5;
//    float result = w_mag(p, q, r);
//    sprintf(msg, "Magnitude of w: %f\n", result);
//    OledDrawString(msg);
//    OledUpdate();
//#endif
//
//#ifdef Forward_Exp_test
//    Matrix3x3 I = {
//        {
//            {1.0, 0.0, 0.0},
//            {0.0, 1.0, 0.0},
//            {0.0, 0.0, 1.0}
//        }
//    };
//
//    Matrix3x1 w_test = {
//        {
//            {1.0},
//            {2.0},
//            {3.0}
//        }
//    };
//
//    Matrix3x3 result = RK_Forward_Integration(I, w_test, 2);
//    Matrix3x3 result2 = RK_Exp_Integration(I, w_test, 2);
//    printf("Forward Integration: \n");
//    printMatrix(&result);
//    printf("Exponential Integration: \n");
//    printMatrix(&result2);
//#endif
//
//#ifdef Drift_test
//    Matrix3x3 I = {
//        {
//            {1.0, 0.0, 0.0},
//            {0.0, 1.0, 0.0},
//            {0.0, 0.0, 1.0}
//        }
//    };
//
//    Matrix3x1 w_test = {
//        {
//            {1.0},
//            {2.0},
//            {3.0}
//        }
//    };
//
//    Matrix3x1 w_test_2 = {
//        {
//            {0.000001},
//            {0.000001},
//            {0.000001}
//        }
//    };
//    Matrix3x3 result = RK_Forward_Integration(I, w_test, 0.01);
//    Matrix3x3 result2 = RK_Exp_Integration(I, w_test, 0.01);
//    printf("Forward Integration Big W: \n");
//    printMatrix(&result);
//    printf("Exponential Integration Big W: \n");
//    printMatrix(&result2);
//
//    Matrix3x3 result3 = RK_Forward_Integration(I, w_test_2, 0.01);
//    Matrix3x3 result4 = RK_Exp_Integration(I, w_test_2, 0.01);
//    printf("Forward Integration Small W: \n");
//    printMatrix(&result3);
//    printf("Exponential Integration Small W: \n");
//    printMatrix(&result4);
//#endif
    return (EXIT_SUCCESS);
}
//#endif

