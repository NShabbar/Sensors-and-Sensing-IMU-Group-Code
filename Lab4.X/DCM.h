/* ************************************************************************** */
/** DCM.h

  @Summary
 outlines functions for interacting with a DCM (a 3x3 matrix)

 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "Matrix.h"

#define PI (acos(-1.0))
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */



    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************
    typedef Matrix3x3 DCM;



    /**
     * Function to convert radians to degrees.
     * @param rads Value in radians.
     * @return Equivalent value in degrees.
     */
    float convertRadToDeg(float rads);
    
    float convertDegToRad(float degs);

    /**
     * Function to calculate the theta angle in radians.
     * @param matrix Pointer to the Matrix3x3 struct.
     * @return Theta angle in radians.
     */
    float getTheta(DCM *matrix);

    /**
     * Function to calculate the psi angle in radians.
     * @param matrix Pointer to the Matrix3x3 struct.
     * @return Psi angle in radians.
     */
    float getPsi(DCM *matrix);

    /**
     * Function to calculate the phi angle in radians.
     * @param matrix Pointer to the Matrix3x3 struct.
     * @return Phi angle in radians.
     */
    float getPhi(DCM *matrix);

#endif /* _EXAMPLE_FILE_NAME_H */

    /* *****************************************************************************
     End of File
     */
