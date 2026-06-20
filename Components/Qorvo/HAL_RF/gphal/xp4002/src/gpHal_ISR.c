/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gphal_ISR.c
 *
 *  The file contains the interrupt handling code
 *
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
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpPd.h"
#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Pbm.h"
#include "gpHal_kx_Fll.h"
#include "gpHal_Statistics.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"          //Containing all uC dependent implementations
#include "gpHal_reg.h"
#include "gpAssert.h"

#ifdef GP_COMP_TXMONITOR
#include "gpTxMonitor.h"
#endif //GP_COMP_TXMONITOR


#include "gpSched.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#if defined(GP_COMP_GPHAL_ES_ABS_EVENT) && (GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS > 0)
static gpHal_AbsoluteEventCallback_t        gpHal_AbsoluteEventCallback[GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS] = { NULL, };
#endif

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  ISR handlers (polled)
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
//  ES INTERRUPT HANDLER
//-------------------------------------------------------------------------------------------------------
#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)

void gpHal_ESInterrupt(void)
{
#if defined(GP_COMP_GPHAL_ES_ABS_EVENT) && (GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS > 0)
    {
        UInt16 maskedEventInterrupts      = 0;
        UInt16 maskedEventInterruptsCheck = 0;
        UInt8  interruptIndex             = 0;

        //Look for Absolute interrupts active
        maskedEventInterrupts = GP_WB_READ_ES_UNMASKED_EVENT_INTERRUPTS();

        GP_ASSERT_SYSTEM(maskedEventInterrupts != 0);
        //Minimum one absolute event should be triggered - relative + external events are no longer a source of the ES interrupt
        if (maskedEventInterrupts == 0)
        {
            return;
        }

        for (interruptIndex = 0; interruptIndex <  GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS; interruptIndex++)
        {
            UInt8 eventResult;

            maskedEventInterruptsCheck = ((UInt16) 1 << interruptIndex);

            if ((maskedEventInterrupts & maskedEventInterruptsCheck) == 0)
            {
                continue;
            }

            eventResult = GP_WB_READ_ABS_EV_RESULT(GPHAL_ES_EVENT_NR_TO_START_OFFSET(interruptIndex));

            if((eventResult == GPHAL_ENUM_EVENT_RESULT_EXECUTED_TOO_LATE) || (eventResult == GPHAL_ENUM_EVENT_RESULT_MISSED_TOO_LATE))
            {
                // If an event was too late, update the symbol startup time
                gpHal_cbEventTooLate();
            }

            if (gpHal_AbsoluteEventCallback[interruptIndex])
            {
                gpHal_AbsoluteEventCallback[interruptIndex]();
            }
            // Clear interrupt - calculation cost is larger then a direct 16-bit access
            GP_WB_WRITE_ES_CLR_EVENT_INTERRUPTS(maskedEventInterruptsCheck);

            // Stop handling event interrupts to give possible PIO interrupt priority
            break;
        }
    }
#else
    //All the interrupts should be serviced by now.
    //If no absolute events are used, the ES interrupt should be handled by relative+external code.
    GP_ASSERT_SYSTEM(false);
#endif //GP_COMP_GPHAL_ES_ABS_EVENT
}

#endif //defined(GP_COMP_GPHAL_ES_ABS_EVENT)

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

//ES register callbacks
#if defined(GP_COMP_GPHAL_ES_ABS_EVENT) && (GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS > 0)
void gpHal_RegisterAbsoluteEventCallback(gpHal_AbsoluteEventCallback_t callback, UInt8 eventNbr)
{
    GP_ASSERT_DEV_EXT(eventNbr < GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS);
    gpHal_AbsoluteEventCallback[eventNbr]=callback;
}

#endif //GP_COMP_GPHAL_ES_ABS_EVENT

//-------------------------------------------------------------------------------------------------------
//  ISR handlers (called directly from ISR)
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
//  RCI INTERRUPT HANDLERS
//-------------------------------------------------------------------------------------------------------
