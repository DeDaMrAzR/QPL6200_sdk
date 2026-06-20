/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
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


/** @file "gpHal_kx_NVM.h"
 *
 *  gpHal non-volatile memory abstraction API.
 *
 *  Implements a byte-buffer based API to read/write from/to NVM, abstracting away any
 *  alignment requirements or underlying word-sizes.
 *
 *  Specifically targeting MRAM and RRAM type of memories that do not have page or
 *  sector erase requirements.
 */

#ifndef _GPHAL_KX_NVM_H_
#define _GPHAL_KX_NVM_H_

#include "global.h"
#include "gpHal_ROM.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum gpHal_NVMError_t */
/** @brief Error values returned by gpHal_NVM API calls. */
//@{
/** @brief Operation completed without error. */
#define gpHal_NVMError_Success 0
/** @brief Requested address range is out of NVM range. */
#define gpHal_NVMError_OutOfRange 3
/** @brief Non-blank content found. */
#define gpHal_NVMError_BlankFailure 5
/** @brief Failure in verification after write. */
#define gpHal_NVMError_VerifyFailure 6
/** @brief Address range is not correctly aligned */
#define gpHal_NVMError_Alignment 13
/** @typedef gpHal_NVMError_t
    @brief Type for error values returned by gpHal_NVM API calls.
*/
typedef UInt8 gpHal_NVMError_t;
//@}

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif


/**
 *  Read from NVM.
 *
 *  @param address  Absolute address of data within Kx memory map (not relative to start of NVM).
 *  @param length   Number of bytes to read.
 *  @param pData    Pointer to buffer of "length" bytes where read data will be stored.
 */
gpHal_NVMError_t gpHal_NVMRead(NvmPtr address, UInt32 length, UInt8* pData);

/**
 *  Write data to non-volatile memory.
 * *
 *  @param address  Absolute address of data within Kx memory map (not relative to start of NVM).
 *  @param length   Number of bytes to write.
 *  @param pData    Pointer to input data buffer containing `length` bytes to write.
 *                  Buffer must be located in RAM. Writing directly from one part of NVM to another is not supported.
 *  @return gpHal_NVMError_Success if write succesful;
 *          gpHal_NVMError_VerifyFailure if write failed.
 */
gpHal_NVMError_t gpHal_NVMWrite(NvmPtr address, UInt32 length, const UInt8* pData);

/**
 *  Write data to non-volatile memory and do not verify written data.
 *  See @a gpHal_NVMWrite for a description of parameters.
 */
gpHal_NVMError_t gpHal_NVMWriteNoVerify(NvmPtr address, UInt32 length, const UInt8* pData);

/**
 *  Check that a specified area of non-volatile memory is blank (all zero bytes).
 *
 *  @param address    Absolute address of NVM data within Kx memory map (not relative to start of NVM).
 *  @param length     Number of bytes to check.
 *  @return gpHal_NVMError_Success if specified area is blank;
 *          gpHal_NVMError_BlankFailure if specified area contains non-zero data.
 */
gpHal_NVMError_t gpHal_NVMBlankCheck(NvmPtr address, UInt32 length);

/**
 *  Erase all APP NVM
 *  note: erased value is 0
 *
 *  @return gpHal_NVMError_Success if NVM is fully erased or error code from gpHal_NVMErase result
 */
gpHal_NVMError_t gpHal_NVMEraseApplicationArea(void);

/**
 *  Erase NVM and verify selected region is erased
 *  note: erased value is 0
 *
 *  @param  addr        start address to erase as viewed from the main system bus
 *  @param  len         number of bytes to erase - needs to be multiples of NVM_WORD_SIZE_BYTES
 *  @return gpHal_NVMError_Success if NVM is erased
 *          gpHal_NVMError_Alignment if len is not multiple of NVM_WORD_SIZE_BYTES
 *          other potential error from gpHal_NVMWrite
 *          gpHal_NVMError_BlankFailure if verification fails (area not erased).
 */
gpHal_NVMError_t gpHal_NVMErase(NvmPtr addr, UInt32 len);

/**
 *  Erase NVM without verifying selected region is erased
 *  note: erased value is 0
 *
 *  @param  addr        start address to erase as viewed from the main system bus
 *  @param  len         number of bytes to erase - needs to be multiples of NVM_WORD_SIZE_BYTES
 *  @return gpHal_NVMError_Success if NVM is erased
 *          gpHal_NVMError_Alignment if len is not multiple of NVM_WORD_SIZE_BYTES
 *          other potential error from gpHal_NVMWrite
 */
gpHal_NVMError_t gpHal_NVMEraseNoVerify(NvmPtr addr, UInt32 len);

#ifdef __cplusplus
}
#endif
#endif //_GPHAL_KX_NVM_H_
