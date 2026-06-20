/*
 * Copyright (c) 2024, Qorvo Inc
 *
 * !!! This is a generated file. !!!
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 *
 */


#ifndef _GPHAL_KX_RAP_TX_CONSTANTS_H_
#define _GPHAL_KX_RAP_TX_CONSTANTS_H_

#define AK_RAP_TX_PA_MODE_OFF (0)
#define AK_RAP_TX_PA_MODE_THERMO_PLUS (1)
#define AK_RAP_TX_PA_MODE_THERMO_MINUS (2)
#define AK_RAP_TX_PA_MODE_THERMO (3)

#define AK_RAP_TX_PA_MODE_THERMO_HYBRID (4)
#define AK_RAP_TX_PA_MODE_1HOT_PLUS (5)
#define AK_RAP_TX_PA_MODE_1HOT_MINUS (6)
#define AK_RAP_TX_PA_MODE_1HOT (7)

#define AK_RAP_TX_PSF_FILTER_MSK (0)
#define AK_RAP_TX_PSF_FILTER_GFSK (1)

static const UInt16 AK_RAP_TX_PSF_COEFFS_DIRAC[] = {
     0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,643
};

static const UInt16 AK_RAP_TX_PSF_COEFFS_GAUSS18[] = {
     0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,1
    ,3
    ,12
    ,35
    ,77
    ,122
    ,142
};

static const UInt16 AK_RAP_TX_PSF_COEFFS_GAUSS25[] = {
     0
    ,0
    ,0
    ,0
    ,0
    ,0
    ,2
    ,6
    ,14
    ,28
    ,50
    ,74
    ,95
    ,104
};

static const UInt16 AK_RAP_TX_PSF_COEFFS_GAUSS38ISI[] = {
     0
    ,0
    ,1
    ,2
    ,4
    ,7
    ,12
    ,19
    ,28
    ,39
    ,50
    ,59
    ,65
    ,68
};

static const UInt16 AK_RAP_TX_PSF_COEFFS_GAUSS40ISI[] = {
     0
    ,1
    ,1
    ,3
    ,5
    ,9
    ,14
    ,21
    ,29
    ,39
    ,48
    ,57
    ,62
    ,64
};

static const UInt16 AK_RAP_TX_PSF_COEFFS_GAUSS42ISI[] = {
     1
    ,1
    ,2
    ,4
    ,6
    ,10
    ,16
    ,22
    ,30
    ,39
    ,47
    ,54
    ,59
    ,61
};


#endif //_GPHAL_KX_RAP_TX_CONSTANTS_H_
