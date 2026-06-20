/*
 * Copyright (c) 2023, Qorvo Inc
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

#include "qRegHwInfo.h"
#include "qRegInternal.h"
#include "qDrvIOB.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief Initialization flag. */
static Bool initialized = false;

/** @brief Input mask. */
static UInt32 inputMask = 0;

/** @brief Output mask. */
static UInt32 outputMask = 0;

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvIOB_Init(void)
{
    if(initialized)
    {
        /* IOB driver already initialized. */
        return Q_ERR_INVSTATE;
    }

    /* Get all supported IOB pins for this SoC. */
    UInt32 allPinMask = qRegHwInfo_InputMaskGet(qRegHwInfo_PackageIDDefault);

    /* Get the package information form hardware info. */
    qRegHwInfo_Package_t package = qRegHwInfo_PackageGet();

    /* Get all supported input pins for the selected package. */
    inputMask = qRegHwInfo_InputMaskGet(package);

    /* Get all supported output pins for the selected package. */
    outputMask = qRegHwInfo_OutputMaskGet(package);

    /* Configure pull and input buffer for all unsupported pins. */
    UInt32 mask = allPinMask & (~(inputMask | outputMask));
    qRegIOB_InputBufferMaskEnableSet(mask, false);
    qRegIOB_PullMaskEnableSet(mask, true);
    qRegIOB_PullMaskModeSet(mask, qRegIOB_PullDown);

    initialized = true;

    return Q_OK;
}

Bool qDrvIOB_SupportedPinCheck(UInt8 iobIndex)
{
    return ((inputMask | outputMask) & (1UL << (UInt32)iobIndex)) ? true : false;
}

qResult_t qDrvIOB_ConfigSet(UInt8 iobIndex, const qDrvIOB_Config_t* const pConfig)
{
    Q_ASSERT(pConfig);

    if(!qDrvIOB_SupportedPinCheck(iobIndex))
    {
        /* Selected IOB pin index is not available on this package. */
        return Q_ERR_INVPARAM;
    }

    if(!BIT_TST((pConfig->inputBuffer ? inputMask : outputMask), iobIndex))
    {
        return Q_ERR_NOTSUPPORTED;
    }

    if(pConfig->schmittTrigger && !BIT_TST(Q_REG_IOB_SCHMITT_MASK, iobIndex))
    {
        return Q_ERR_NOTSUPPORTED;
    }

    switch(pConfig->pull)
    {
        case qDrvIOB_PullUp:
            if(!BIT_TST(Q_REG_IOB_PULL_MASK, iobIndex))
            {
                return Q_ERR_NOTSUPPORTED;
            }
            qRegIOB_PullModeSet(iobIndex, qRegIOB_PullUp);
            qRegIOB_PullEnableSet(iobIndex, true);
            break;
        case qDrvIOB_PullDown:
            if(!BIT_TST(Q_REG_IOB_PULL_MASK, iobIndex))
            {
                return Q_ERR_NOTSUPPORTED;
            }
            qRegIOB_PullModeSet(iobIndex, qRegIOB_PullDown);
            qRegIOB_PullEnableSet(iobIndex, true);
            break;
        default:
            qRegIOB_PullEnableSet(iobIndex, false);
            break;
    }

    qRegIOB_InputBufferEnableSet(iobIndex, pConfig->inputBuffer);
    qRegIOB_DriveStrengthSet(iobIndex, pConfig->drive);
    qRegIOB_SchmittTriggerEnableSet(iobIndex, pConfig->schmittTrigger);
    qRegIOB_SlewRateSet(iobIndex, pConfig->slewRate);

    return Q_OK;
}

qResult_t qDrvIOB_ConfigGet(UInt8 iobIndex, qDrvIOB_Config_t* const pConfig)
{
    Q_ASSERT(pConfig);

    if(!qDrvIOB_SupportedPinCheck(iobIndex))
    {
        /* Selected IOB index pin is not available on this package. */
        return Q_ERR_INVPARAM;
    }

    pConfig->drive = qRegIOB_DriveStrengthGet(iobIndex);

    pConfig->inputBuffer = qRegIOB_InputBufferEnableGet(iobIndex);

    qRegIOB_Pull_t pull = qRegIOB_PullModeGet(iobIndex);
    Bool pullEnable = qRegIOB_PullEnableGet(iobIndex);

    if(pullEnable == false)
    {
        pConfig->pull = qDrvIOB_PullNone;
    }
    else
    {
        if(pull == qRegIOB_PullUp)
        {
            pConfig->pull = qDrvIOB_PullUp;
        }

        if(pull == qRegIOB_PullDown)
        {
            pConfig->pull = qDrvIOB_PullDown;
        }
    }

    pConfig->schmittTrigger = qRegIOB_SchmittTriggerEnableGet(iobIndex);

    pConfig->slewRate = qRegIOB_SlewRateGet(iobIndex);

    return Q_OK;
}

qResult_t qDrvIOB_ConfigInputSet(UInt8 iobIndex, qDrvIOB_Pull_t pull, Bool schmitt)
{
    qDrvIOB_Config_t iobConfig = {
        .drive = qDrvIOB_Drive2mA,
        .inputBuffer = true,
        .pull = pull,
        .schmittTrigger = schmitt,
        .slewRate = qDrvIOB_SlewRateSlow,
    };

    return qDrvIOB_ConfigSet(iobIndex, &iobConfig);
}

qResult_t qDrvIOB_ConfigOutputSet(UInt8 iobIndex, qDrvIOB_Drive_t drive, qDrvIOB_SlewRate_t slewRate)
{
    qDrvIOB_Config_t iobConfig = {
        .drive = drive,
        .inputBuffer = false,
        .pull = qDrvIOB_PullNone,
        .schmittTrigger = false,
        .slewRate = slewRate,
    };

    return qDrvIOB_ConfigSet(iobIndex, &iobConfig);
}

qResult_t qDrvIOB_ConfigUnusedSet(UInt8 iobIndex)
{
    /* Unused config can be set to all pins. */
    if(!((inputMask | outputMask) & (1UL << (UInt32)iobIndex)))
    {
        /* Selected IOB index pin is not available on this chip. */
        return Q_ERR_INVPARAM;
    }

    qRegIOB_DriveStrengthSet(iobIndex, qRegIOB_DriveStrength2ma);

    if(BIT_TST(inputMask, iobIndex))
    {
        qRegIOB_InputBufferEnableSet(iobIndex, false);
    }

    qRegIOB_PullModeSet(iobIndex, qRegIOB_PullDown);
    qRegIOB_PullEnableSet(iobIndex, true);

    if(BIT_TST(Q_REG_IOB_SCHMITT_MASK, iobIndex))
    {
        qRegIOB_SchmittTriggerEnableSet(iobIndex, false);
    }

    qRegIOB_SlewRateSet(iobIndex, qDrvIOB_SlewRateSlow);

    return Q_OK;
}

qResult_t qDrvIOB_AlternateSet(UInt8 iobIndex, UInt8 alternate)
{
    if(!qDrvIOB_SupportedPinCheck(iobIndex))
    {
        /* Selected IOB index pin is not available on this package. */
        return Q_ERR_INVPARAM;
    }

    if(qRegIOB_AlternateEnableGet(iobIndex) && (alternate != qDrvIOB_AlternateNone) &&
       (alternate != qRegIOB_AlternateGet(iobIndex)))
    {
        /* Debug pins are excluded from this check. */
        if(!((1UL << iobIndex) & qRegHwInfo_DebugPinMaskGet()))
        {
            return Q_ERR_INVSTATE;
        }
    }

    qRegIOB_AlternateEnableSet(iobIndex, false);

    if(alternate != qDrvIOB_AlternateNone)
    {
        /* The approach where the alternate function is explicitly disabled and enabled
           when changing the function on the pins allows for glitches function switchover. */
        qRegIOB_AlternateSet(iobIndex, alternate);
        qRegIOB_AlternateEnableSet(iobIndex, true);
    }

    return Q_OK;
}

qResult_t qDrvIOB_AlternateGet(UInt8 iobIndex, UInt8* pAlternate)
{
    Q_ASSERT(pAlternate);

    if(!qDrvIOB_SupportedPinCheck(iobIndex))
    {
        /* Selected IOB index pin is not available on this package. */
        return Q_ERR_INVPARAM;
    }

    Bool enable = qRegIOB_AlternateEnableGet(iobIndex);

    if(enable == true)
    {
        *pAlternate = qRegIOB_AlternateGet(iobIndex);
    }
    else
    {
        *pAlternate = qDrvIOB_AlternateNone;
    }

    return Q_OK;
}

qResult_t qDrvIOB_AlternateArraySet(const qDrvIOB_PinAlt_t* pPins, UInt8 count)
{
    Q_ASSERT(pPins);

    for(UInt8 n = 0; n < count; ++n)
    {
        if(pPins[n].pin == Q_NOPIN)
        {
            continue;
        }

        if(!qDrvIOB_SupportedPinCheck(pPins[n].pin))
        {
            /* Selected IOB index pin is not available on this package. */
            return Q_ERR_INVPARAM;
        }

        if((qRegIOB_AlternateEnableGet(pPins[n].pin) && (pPins[n].alternate != qDrvIOB_AlternateNone) &&
            pPins[n].alternate != qRegIOB_AlternateGet(pPins[n].pin)))
        {
            /* Debug pins are excluded from this check. */
            if(!((1UL << pPins[n].pin) & qRegHwInfo_DebugPinMaskGet()))
            {
                return Q_ERR_INVSTATE;
            }
        }
    }

    for(UInt8 n = 0; n < count; ++n)
    {
        if(pPins[n].pin == Q_NOPIN)
        {
            continue;
        }

        qRegIOB_AlternateEnableSet(pPins[n].pin, false);
        if(pPins[n].alternate != qDrvIOB_AlternateNone)
        {
            qRegIOB_AlternateSet(pPins[n].pin, pPins[n].alternate);
            qRegIOB_AlternateEnableSet(pPins[n].pin, true);
        }
    }

    return Q_OK;
}

qResult_t qDrvIOB_AlternateArrayClear(const qDrvIOB_PinAlt_t* pPins, UInt8 count)
{
    Q_ASSERT(pPins);

    qResult_t res = Q_OK;

    for(UInt8 n = 0; n < count; ++n)
    {
        if(pPins[n].pin == Q_NOPIN)
        {
            continue;
        }

        if(!qDrvIOB_SupportedPinCheck(pPins[n].pin))
        {
            /* Selected IOB index pin is not available on this package. */
            return Q_ERR_INVPARAM;
        }

        if(!qRegIOB_AlternateEnableGet(pPins[n].pin) || (pPins[n].alternate != qRegIOB_AlternateGet(pPins[n].pin)))
        {
            return Q_ERR_INVSTATE;
        }

        qRegIOB_AlternateEnableSet(pPins[n].pin, false);
    }

    return res;
}
