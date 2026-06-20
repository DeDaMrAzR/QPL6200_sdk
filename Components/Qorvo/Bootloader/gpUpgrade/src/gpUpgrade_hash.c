/*
 *   Copyright (c) 2021, Qorvo Inc
 *
 *   Upgrade functionality
 *   Implementation of gpUpgrade hash actions
 *
 *   This software is owned by Qorvo Inc
 *   and protected under applicable copyright laws.
 *   It is delivered under the terms of the license
 *   and is intended and supplied for use solely and
 *   exclusively with products manufactured by
 *   Qorvo Inc.
 *
 *
 *   THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 *   CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 *   IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 *   LIMITED TO, IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *   QORVO INC. SHALL NOT, IN ANY
 *   CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 *   INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 *   FOR ANY REASON WHATSOEVER.
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_UPGRADE

#include "gpHal.h"
#include "gpUtils.h"
#include "gpUpgrade.h"
#include "gpUpgrade_flash.h"

#include "gpUpgrade_defs.h"


#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "mm.h"
#include "hal_SEMailbox.h"
#define GP_MM_RAM_LINEAR_SIZE GP_MM_AKUMA_RAM_LINEAR_CODE_SIZE
#define GP_MM_FLASH_START     GP_MM_CMA_NVM_LINEAR_START + GP_HAL_NVM_APPUC_OFFSET
#define GP_MM_FLASH_ALT_START GP_MM_CMA_NVM_LINEAR_START + GP_HAL_NVM_APPUC_OFFSET
#endif
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/** @brief This function calculates a partial CRC over an address range in internal or external flash
*
*   @param crcVal     The pointer to the return value
*   @param address    The address on which to start the calculation
*   @param totalSize  Size over which to calculate
*/
void gpUpgrade_HashPartialCrc(UInt32 * crcVal, UInt32 address, UInt32 totalSize)
{
    UInt16 imgBlockLen = 0;
    UInt32 sz = 0;
    UInt32 imgAddr = address;
    UInt8 imgData[IMG_MAX_BLOCK_LEN];

    do
    {
        imgBlockLen = min(IMG_MAX_BLOCK_LEN, (totalSize - sz));
        if(gpUpgrade_StatusSuccess == gpUpgrade_FlashLoad(imgAddr, imgBlockLen, imgData))
        {
            gpUtils_CalculatePartialCrc32(crcVal, imgData, imgBlockLen);
        }
        else
        {
            *crcVal = 0xFFFFFFFF;
            break;
        }
        sz += imgBlockLen;
        imgAddr += imgBlockLen;

    }while(sz < totalSize);
}

#if defined(GP_DIVERSITY_APPUC_FW_HEADER)
#else
/** @brief This function calculates a CRC over an entire image
*
*   @param swUpgTab             The software upgrade table with offset, size and flags
*/
UInt32 gpUpgrade_HashImageCrc(gpUpgrade_Table_t swUpgTab)
{
    UInt32 crcVal = 0;

    crcVal ^= GP_UTILS_CRC32_FINAL_XOR_VALUE;

#if           \
    defined(GP_DIVERSITY_GPHAL_XP4002)
    if ( swUpgTab.flagStatus & GP_UPGRADE_FLAG_JUMPTABLE_UPGRADE )
    {
        gpUpgrade_HashPartialCrc(&crcVal,
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
                                    swUpgTab.memStartAddr - GP_UPGRADE_APP_JUMP_TABLE_SIZE,
#else
                                    GP_UPGRADE_APP_JUMP_TABLE_ADDR(swUpgTab.memStartAddr),
#endif
                                    GP_UPGRADE_APP_JUMP_TABLE_SIZE);
    }
#endif //GP_DIVERSITY_GPHAL_K8C || GP_DIVERSITY_GPHAL_K8D || GP_DIVERSITY_GPHAL_K8E

    gpUpgrade_HashPartialCrc(&crcVal, swUpgTab.memStartAddr, swUpgTab.memSz);

    crcVal ^= GP_UTILS_CRC32_FINAL_XOR_VALUE;
    return crcVal;
}
#endif
