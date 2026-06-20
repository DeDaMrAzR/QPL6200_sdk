/*
 * Copyright (c) 2024, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
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
 */

/** @file "gpAppUcSubsystem.h"
 *
 *
 * @defgroup INIT INIT primitives
 * This module groups the primitives for initialisation.
 *
 * @defgroup SET SET primitives
 * This module groups the primitives for setting the different calibration configs.
 *
 */

#ifndef _GPAPPUCSUBSYSTEM_H_
#define _GPAPPUCSUBSYSTEM_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct gpAppUcSubsystem_CalRamDcdc_s {
    UInt8 y2_skip_ehps;
    UInt8 y2_skip_emps;
    UInt8 y2_min_ehps;
    UInt8 y2_min_emps;
    UInt8 y1_skip;
    UInt8 y1_min;
} gpAppUcSubsystem_CalRamDcdc_t;

typedef struct gpAppUcSubsystem_CalRamXo32m_s {
    UInt8 cload_comp_0;  //   xo32m cload compensation value for the lowest temperature range (-30C and below )
    UInt8 cload_comp_1;  //   xo32m cload compensation value for temperature range (-30C to -20C)
    UInt8 cload_comp_2;  //   xo32m cload compensation value for temperature range (-20C to -10C)
    UInt8 cload_comp_3;  //   xo32m cload compensation value for temperature range (-10C to 0C)
    UInt8 cload_comp_4;  //   xo32m cload compensation value for temperature range (0C to 10C)
    UInt8 cload_comp_5;  //   xo32m cload compensation value for temperature range (10C to 20C)
    UInt8 cload_comp_6;  //   xo32m cload compensation value for temperature range (20C to 30C)
    UInt8 cload_comp_7;  //   xo32m cload compensation value for temperature range (30C to 40C)
    UInt8 cload_comp_8;  //   xo32m cload compensation value for temperature range (40C to 50C)
    UInt8 cload_comp_9;  //   xo32m cload compensation value for temperature range (50C to 60C)
    UInt8 cload_comp_10; //  xo32m cload compensation value for temperature range (60C to 70C)
    UInt8 cload_comp_11; //  xo32m cload compensation value for temperature range (70C to 80C)
    UInt8 cload_comp_12; //  xo32m cload compensation value for temperature range (80C to 90C)
    UInt8 cload_comp_13; //  xo32m cload compensation value for temperature range (90C to 100C)
    UInt8 cload_comp_14; //  xo32m cload compensation value for temperature range (100C to 110C)
    UInt8 cload_comp_15; //  xo32m cload compensation value for temperature range (110C to 120C)
    UInt8 cload_comp_16; //  xo32m cload compensation value for temperature range (120C and above)
} gpAppUcSubsystem_CalRamXo32m_t;

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// Requests
/** @ingroup SET
 *
 *  @brief Set DCDC calibration values.
 *
 *  This primitive sets the calibration values related to the DCDC operation.
 *
 *  @param pCalRamDcdcData            A pointer to a struct of type gpAppUcSubsystem_CalRamDcdc_t, where the
 *                                    calibration values of the DCDC are located.
 */
void gpAppUcSubsystem_CalibrationSetDcdcValues(gpAppUcSubsystem_CalRamDcdc_t* pCalRamDcdcData);

/** @ingroup SET
 *
 *  @brief set Xo32m calibration values.
 *
 * This primitive sets the calibration values related to the 32MHz XTAL operation.
 *
 *  @param pCalRamXo32mData           A pointer to a struct of type gpAppUcSubsystem_CalRamXo32m_t, where the
 *                                    calibration values of the Xo32m are located.
 */
void gpAppUcSubsystem_CalibrationSetXo32mValues(gpAppUcSubsystem_CalRamXo32m_t* pCalRamXo32mData);

/**
 * @brief  This primitive initializes temperature based calibration.
 *
 */
void gpAppUcSubsystem_CalibrationInit(void);

/**
 * @brief This primitive triggers calibration.
 *
 */
void gpAppUcSubsystem_TriggerCalibrations(void);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_GPAPPUCSUBSYSTEM_H_
