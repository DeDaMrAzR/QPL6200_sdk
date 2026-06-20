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

#ifndef _Q_DRV_IOB_H_
#define _Q_DRV_IOB_H_

/**
 * @defgroup Q_DRV_IOB_H IOB driver
 * @{
 *
 * @brief This is a driver for IOB.
 *
 * @file qDrvIOB.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Definition of no alternate mode. */
#define qDrvIOB_AlternateNone 0xFF

/**
 * @brief Macro for getting the alternate mode for the specified IOB pin.
 *
 * @param[in] IOB_INDEX    IOB pin index.
 * @param[in] PERIPH       Name of the peripheral.
 * @param[in] PERIPH_INDEX Index oh the peripheral.
 * @param[in] FUNC         Alternate function name.
 */
#define Q_DRV_IOB_ALT_FUNCT_GET(IOB_INDEX, PERIPH, PERIPH_INDEX, FUNC)                                                 \
    REG_IOB_PIN_ALT_FUNC(IOB_INDEX, PERIPH, PERIPH_INDEX, FUNC)

#define Q_DRV_IOB_ALT_GET(IOB_INDEX, PERIPH, PERIPH_INDEX) REG_IOB_PIN_ALT(IOB_INDEX, PERIPH, PERIPH_INDEX)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvIOB_Drive_t */
//@{
/** @brief 2 mA. */
#define qDrvIOB_Drive2mA qRegIOB_DriveStrength2ma
/** @brief 4 mA. */
#define qDrvIOB_Drive4mA qRegIOB_DriveStrength4ma
/** @brief 8 mA. */
#define qDrvIOB_Drive8mA qRegIOB_DriveStrength8ma
/** @brief 12 mA. */
#define qDrvIOB_Drive12mA qRegIOB_DriveStrength12ma
/** @typedef qDrvIOB_Drive_t
 *  @brief IOB output drive strength.
 */
typedef UInt8 qDrvIOB_Drive_t;
//@}

/** @enum qDrvIOB_Pull_t */
//@{
/** @brief No pullup/pulldown. */
#define qDrvIOB_PullNone 0
/** @brief Internal pullup. */
#define qDrvIOB_PullUp 1
/** @brief Internal pulldown. */
#define qDrvIOB_PullDown 2
/** @typedef qDrvIOB_Pull_t
 *  @brief IOB input pull mode.
 */
typedef UInt8 qDrvIOB_Pull_t;
//@}

/** @enum qDrvIOB_SlewRate_t */
//@{
/** @brief Fast slew rate. */
#define qDrvIOB_SlewRateFast qRegIOB_SlewRateFast
/** @brief Slow slew rate. */
#define qDrvIOB_SlewRateSlow qRegIOB_SlewRateSlow
/** @typedef qDrvIOB_SlewRate_t
 *  @brief IOB output slew rate.
 */
typedef UInt8 qDrvIOB_SlewRate_t;
//@}

/** @brief IOB pin configuration structure. */
typedef struct {
    qDrvIOB_Drive_t drive;       /**< IOB output drive strength. */
    Bool inputBuffer;            /**< True if the IOB input buffer is to be enabled, false otherwise. */
    qDrvIOB_Pull_t pull;         /**< IOB input pull mode. */
    Bool schmittTrigger;         /**< True if the IOB Schmitt trigger is to be enabled, false otherwise. */
    qDrvIOB_SlewRate_t slewRate; /**< IOB output slew rate. */
} qDrvIOB_Config_t;

/** @brief IOB alternate configuration structure used for bulk setup. */
typedef struct {
    UInt8 pin;       /**< IOB pin index. */
    UInt8 alternate; /**< IOB alternate function number. */
} qDrvIOB_PinAlt_t;

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Function for initializing the IOB driver.
 *
 * Function deinitialize unavailable pins for selected package if there are any unsupported pins.
 *
 * @note Function must be called before any other IOB driver function.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE IOB driver already initialized.
 */
qResult_t qDrvIOB_Init(void);

/**
 * @brief Function for checking if the specified IOB pin is available on selected chip and package.
 *
 * @param[in] iobIndex IOB pin index.
 *
 * @return True if the pin is available, false otherwise.
 */
Bool qDrvIOB_SupportedPinCheck(UInt8 iobIndex);

/**
 * @brief Function for setting the IOB configuration for the specified pin index.
 *
 * @param[in] iobIndex IOB pin index.
 * @param[in] pConfig  Pointer to the configuration structure.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVPARAM     IOB pin index is not available on this package.
 * @retval Q_ERR_NOTSUPPORTED IOB pin index does not support the specified configuration.
 */
qResult_t qDrvIOB_ConfigSet(UInt8 iobIndex, const qDrvIOB_Config_t* const pConfig);

/**
 * @brief Function for getting the IOB configuration from the specified pin index.
 *
 * @param[in]  iobIndex IOB pin index.
 * @param[out] pConfig  Pointer to the configuration structure.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM IOB pin index is not available on this package.
 */
qResult_t qDrvIOB_ConfigGet(UInt8 iobIndex, qDrvIOB_Config_t* const pConfig);

/**
 * @brief Function for setting the IOB in the input mode.
 *
 * @param[in] iobIndex IOB pin index.
 * @param[in] pull     IOB input pull mode.
 * @param[in] schmitt  True if the IOB Schmitt trigger is to be enabled, false otherwise.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVPARAM     IOB pin index is not available on this package.
 * @retval Q_ERR_NOTSUPPORTED IOB pin index does not support the specified configuration.
 */
qResult_t qDrvIOB_ConfigInputSet(UInt8 iobIndex, qDrvIOB_Pull_t pull, Bool schmitt);

/**
 * @brief Function for setting the IOB in the output mode.
 *
 * @param[in] iobIndex IOB pin index.
 * @param[in] drive    IOB output drive strength.
 * @param[in] slewRate IOB output slew rate.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVPARAM     IOB pin index is not available on this package.
 * @retval Q_ERR_NOTSUPPORTED IOB pin index does not support the specified configuration.
 */
qResult_t qDrvIOB_ConfigOutputSet(UInt8 iobIndex, qDrvIOB_Drive_t drive, qDrvIOB_SlewRate_t slewRate);

/**
 * @brief Function for setting the IOB in the unused mode.
 *
 * Unused mode parameters:
 *     drive = qDrvIOB_Drive2mA
 *     input buffer = false
 *     pull = qDrvIOB_PullNone
 *     schmitt trigger = false
 *     slew rate = qDrvIOB_SlewRateSlow
 *
 * @param[in] iobIndex IOB pin index.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM IOB index pin is not available on this chip.
 */
qResult_t qDrvIOB_ConfigUnusedSet(UInt8 iobIndex);

/**
 * @brief Function for setting the IOB alternate function for the specified pin index.
 *
 * @param[in] iobIndex  IOB pin index.
 * @param[in] alternate IOB alternate function concatenated with the @p Q_DRV_IOB_ALT_GET() macro.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM IOB pin index is not available on this package.
 * @retval Q_ERR_INVSTATE A different alternate function is already set.
 */
qResult_t qDrvIOB_AlternateSet(UInt8 iobIndex, UInt8 alternate);

/**
 * @brief Function for getting the IOB alternate function from the specified pin index.
 *
 * @param[in]  iobIndex   IOB pin index.
 * @param[out] pAlternate Pointer to the alternate variable.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM IOB pin index is not available on this package.
 */
qResult_t qDrvIOB_AlternateGet(UInt8 iobIndex, UInt8* pAlternate);

/**
 * @brief Bulk configure the pin alternate functions.
 *        This function will not change the IOB state if at least
 *        one alternate cannot be set (pin already in use).
 *
 * @param[in] pPins Pin alternate configuration array pointer.
 * @param[in] count Configuration array length.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM At least one IOB pin index is not available on this package.
 * @retval Q_ERR_INVSTATE At least one pin alternate function is already set.
 */
qResult_t qDrvIOB_AlternateArraySet(const qDrvIOB_PinAlt_t* pPins, UInt8 count);

/**
 * @brief Bulk disable the pin alternate functions.
 *        This function will unmap the pins if the alternate function
 *        match the expected value in @p pPins. Single failure (mismatch)
 *        does not prevent changing the other pins (unlike @p qDrvIOB_AlternateArraySet).
 *
 * @param[in] pPins Pin alternate configuration array pointer.
 * @param[in] count Configuration array length.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM At least one IOB pin index is not available on this package.
 * @retval Q_ERR_INVSTATE At least one alternate function number did not
 *                        match the expected value specified in @p pPins.
 */
qResult_t qDrvIOB_AlternateArrayClear(const qDrvIOB_PinAlt_t* pPins, UInt8 count);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_IOB_H_
