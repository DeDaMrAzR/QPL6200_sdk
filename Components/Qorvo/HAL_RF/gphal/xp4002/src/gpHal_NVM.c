/*
 * Copyright (c) 2022, Qorvo Inc
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
 *                    Preprocessor Includes
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "global.h"
#include "gpHal_kx_NVM.h"
#include "gpHal_kx_mm.h"
#include "gpHal_kx_defs.h"
#include "gpHal_kx_Rap_nvm.h"
#include "gpHal_kx_memcpy.h"
#include "gpLog.h"
#include "gpUtils.h"
#include "gpAssert.h"
#include "gpHal_ROM.h"
#if defined(SEUC_TARGET_FIRMWARE)
#include "q_nvm.h"
#endif
#include "qRegInfoPage.h"


/*****************************************************************************
 *                    Preprocessor Macros
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Implementations
 *****************************************************************************/

static Bool is_region_inside_nvm_linear_or_infopage(UInt32 address, UInt32 length)
{
    if(gpUtils_is_region_a_within_region_b((UInt32)address, length, GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_LINEAR_START),
                                            GP_MM_CMA_NVM_LINEAR_SIZE - Q_MM_OTP_SIZE) ^
       gpUtils_is_region_a_within_region_b(
           (UInt32)address, length, GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_INFO_USER_START), GP_MM_CMA_NVM_INFO_USER_SIZE))
    {
        return true;
    }

    return false;
}

/*****************************************************************************
 *                    Public Function Implementations
 *****************************************************************************/




gpHal_NVMError_t gpHal_NVMRead(NvmPtr address, UInt32 length, UInt8* pData)
{
    if(is_region_inside_nvm_linear_or_infopage((UInt32)address, length))
    {
        hal_memcpy_wb2ptr(pData, address, length, 0);
        return gpHal_NVMError_Success;
    }

    return gpHal_NVMError_OutOfRange;
}

gpHal_NVMError_t gpHal_NVMWrite(NvmPtr address, UInt32 length, const UInt8* pData)
{
    gpHal_NVMError_t writeResult = gpHal_NVMWriteNoVerify(address, length, pData);

    if(gpHal_NVMError_Success != writeResult)
    {
        return writeResult;
    }

    if(hal_memcmp_ptr2wb(address, pData, length, 0))
    {
        return gpHal_NVMError_VerifyFailure;
    }

    return gpHal_NVMError_Success;
}

gpHal_NVMError_t gpHal_NVMWriteNoVerify(NvmPtr address, UInt32 length, const UInt8* pData)
{
    NvmPtr relative_address = address - GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_START);
    NvmPtr aligned_address = relative_address - (relative_address % NVM_WORD_SIZE_BYTES);
    const UInt8* source_address = pData;

    GP_LOG_PRINTF("gpHal_NVMWriteNoVerify(%08x, %d, %08x)", 0, (void*)address, length, (void*)pData);
    GP_LOG_PRINTF("relative = 0x%08x, aligned = 0x%08x", 0, (void*)relative_address, (void*)aligned_address);

    if(is_region_inside_nvm_linear_or_infopage((UInt32)address, length))
    {

        /* Only allow source data from APPUC RAM, AKUMA RAM and SE RAM */
        if(!gpUtils_is_region_a_within_region_b((UInt32)source_address, length, GP_MM_ADDR_FROM_SYS(GP_MM_SYSRAM_START),
                                                GP_MM_SYSRAM_SIZE) &&
           !gpUtils_is_region_a_within_region_b((UInt32)source_address, length,
                                                GP_MM_ADDR_FROM_SYS(GP_MM_APPUC_RAM_START), GP_MM_APPUC_RAM_SIZE) &&
           !gpUtils_is_region_a_within_region_b((UInt32)source_address, length, GP_MM_ADDR_FROM_SYS(GP_MM_AKUMA_RAM_LINEAR_START), GP_MM_AKUMA_RAM_LINEAR_SIZE)
        )
        {
            return gpHal_NVMError_OutOfRange;
        }

#if defined(SEUC_TARGET_FIRMWARE)
        uint8_t prepare_write_state = q_nvm_prepare_write();
#endif

        if(aligned_address != relative_address)
        {
            UInt8 buffer[NVM_WORD_SIZE_BYTES];
            UInt32 copy_length = NVM_WORD_SIZE_BYTES - (relative_address % NVM_WORD_SIZE_BYTES);

            GP_LOG_PRINTF("Start unaligned: length = %d, copy_length = %d", 0, length, copy_length);
            if(length < copy_length)
            {
                copy_length = length;
            }

            hal_memcpy_wb2ptr(buffer, GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_START) + aligned_address, sizeof(buffer), 0);
            MEMCPY(buffer + (relative_address % NVM_WORD_SIZE_BYTES), source_address, copy_length);

            gpHal_ROM_NVM_write(aligned_address, 1, buffer);

            length -= copy_length;
            relative_address += copy_length;
            source_address += copy_length;
        }

        if(length > NVM_WORD_SIZE_BYTES)
        {
            GP_LOG_PRINTF("Aligned rel = 0x%08x", 0, (void*)relative_address);

            UInt16 num_units = (UInt16)((length) / NVM_WORD_SIZE_BYTES);

            gpHal_ROM_NVM_write(relative_address, num_units, source_address);

            length = length % NVM_WORD_SIZE_BYTES;
            relative_address += (num_units * NVM_WORD_SIZE_BYTES);
            source_address += (num_units * NVM_WORD_SIZE_BYTES);
        }

        if(length)
        {
            UInt8 buffer[NVM_WORD_SIZE_BYTES];

            GP_LOG_PRINTF("End unaligned", 0);

            hal_memcpy_wb2ptr(buffer, GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_START) + relative_address, sizeof(buffer), 0);
            MEMCPY(buffer, source_address, length);

            gpHal_ROM_NVM_write(relative_address, 1, buffer);
        }

    #if defined(SEUC_TARGET_FIRMWARE)
        q_nvm_finalize_write(prepare_write_state);
    #endif

        /* invalidate cache */
        GP_WB_WRITE_CMA_NVM_PORT_BUFFER_INVALIDATE(GP_WB_CMA_NVM_PORT_BUFFER_INVALIDATE_MASK);

        return gpHal_NVMError_Success;
    }

    return gpHal_NVMError_OutOfRange;
}

gpHal_NVMError_t gpHal_NVMBlankCheck(NvmPtr address, UInt32 length)
{
    /* only allow actual NVM */
    if(is_region_inside_nvm_linear_or_infopage((UInt32)address, length))
    {

        while(length--)
        {
            if(GP_WB_READ_U8(address++))
            {
                return gpHal_NVMError_BlankFailure;
            }
        }

        return gpHal_NVMError_Success;
    }

    return gpHal_NVMError_OutOfRange;
}

gpHal_NVMError_t gpHal_NVMEraseApplicationArea(void)
{
    UInt32 appuc_nvm_offset;
    appuc_nvm_offset = gpHal_InfoPageGetSizeSeucFwArea();
    return gpHal_NVMErase(GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_LINEAR_START + appuc_nvm_offset), (GP_MM_CMA_NVM_LINEAR_SIZE - appuc_nvm_offset - Q_MM_OTP_SIZE));
}
gpHal_NVMError_t gpHal_NVMEraseNoVerify(NvmPtr addr, UInt32 len)
{
    UInt32 x = 0;
    uint8_t all_zeroes[NVM_WORD_SIZE_BYTES];
    memset(all_zeroes, 0x00, NVM_WORD_SIZE_BYTES);

    if(is_region_inside_nvm_linear_or_infopage((UInt32)addr, len))
    {

        if(len % NVM_WORD_SIZE_BYTES != 0)
        {
            return gpHal_NVMError_Alignment;
        }

        for(x = 0; x < (len / NVM_WORD_SIZE_BYTES); x++)
        {
            gpHal_NVMError_t result = gpHal_NVMWriteNoVerify(addr + (NVM_WORD_SIZE_BYTES * x), NVM_WORD_SIZE_BYTES, (const UInt8*)all_zeroes);
            GP_ASSERT_DEV_EXT(result == gpHal_NVMError_Success);
        }

        return gpHal_NVMError_Success;
    }

    return gpHal_NVMError_OutOfRange;
}

gpHal_NVMError_t gpHal_NVMErase(NvmPtr addr, UInt32 len)
{
    gpHal_NVMError_t result;
    result = gpHal_NVMEraseNoVerify(addr, len);
    if(result != gpHal_NVMError_Success)
    {
        return result;
    }

    result = gpHal_NVMBlankCheck(addr, len);
    if(result != gpHal_NVMError_Success)
    {
        return result;
    }

    return gpHal_NVMError_Success;
}
