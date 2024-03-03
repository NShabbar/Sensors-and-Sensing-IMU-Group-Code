/* 
 * File:   part5.c
 * Author: Nadia
 *
 * Created on March 1, 2024, 5:20 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "BNO055.h"
#include "serial.h"
#include "Matrix.h"
#include "timers.h"
#include "RK.h"
#include "DCM.h"

/*
 * 
 */
#define dt 0.02

float gyroX;
float gyroY;
float gyroZ;

float Xtot;
float Ytot;
float Ztot;

float Xbias;
float Ybias;
float Zbias;

int prevtime;
int currtime;

float theta; //pitch
float phi; //roll
float psi; //yaw

Matrix3x3 R_0={{{1,0,0},
{0,1,0},
{0,0,1}}};

Matrix3x3 Rk_1;
float bigdata[50][3];

int maincount=0;
#ifdef PART5
int main(int argc, char** argv) {
    BOARD_Init();
    BNO055_Init();
    TIMERS_Init();
    printf("START\n");
    gyroX=BNO055_ReadGyroX();
    gyroY=BNO055_ReadGyroY();
    gyroZ=BNO055_ReadGyroZ();
    
    float x_hat = (gyroX)/131;
    float y_hat = (gyroY)/131;
    float z_hat = (gyroZ)/131;
    
    Matrix3x1 omega={{{x_hat},{y_hat},{z_hat}}};
    Rk_1=RK_Exp_Integration(R_0, omega, dt);
    while(1){
        
        currtime=TIMERS_GetMilliSeconds();
        if(currtime-prevtime>=20){
            
            prevtime=currtime;
            gyroX=BNO055_ReadGyroX();
            gyroY=BNO055_ReadGyroY();
            gyroZ=BNO055_ReadGyroZ();
            if(maincount<50){
                bigdata[maincount][0]=gyroX;
                bigdata[maincount][1]=gyroY;
                bigdata[maincount][2]=gyroZ;
            }
            else if (maincount==50){
                for(int j=0;j<50;j++){
                    Xtot+=bigdata[j][0];
                    Ytot+=bigdata[j][1];
                    Ztot+=bigdata[j][2];
                }
                Xbias=Xtot/50;
                Ybias=Ytot/50;
                Zbias=Ztot/50;
                
            }
            else 
            {
            x_hat = convertDegToRad(gyroX-Xbias)/131;
            y_hat = convertDegToRad(gyroY-Ybias)/131;
            z_hat = convertDegToRad(gyroZ-Zbias)/131;
            Matrix3x1 omega={{{x_hat},{y_hat},{z_hat}}};
            Rk_1=RK_Exp_Integration(Rk_1, omega, dt);
            
            psi=getPsi(&Rk_1);
            phi=getPhi(&Rk_1);
            theta=getTheta(&Rk_1);
                //printf("%d: %f %f %f\n",maincount,convertRadToDeg(psi), convertRadToDeg(phi), convertRadToDeg(theta));
            
            }
            maincount++;
        }
        
    }
    return (EXIT_SUCCESS);
}
#endif

