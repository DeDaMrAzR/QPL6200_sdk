/*
 * Copyright (c) 2020, Qorvo Inc
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

/** @file "qvCHIP_KVS.h"
 *
 *  CHIP wrapper KVS API
 *
 *  Declarations of the KVS specific public functions and enumerations of qvCHIP.
 */

#ifndef _QVCHIP_KVS_H_
#define _QVCHIP_KVS_H_

#ifdef GP_DIVERSITY_GPHAL_XP4002
#define QVCHIP_SECURE_STORAGE
#endif

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include <stdint.h>
#include "qvCHIP.h"

#ifdef QVCHIP_SECURE_STORAGE
#include "esecure.h"
#endif // QVCHIP_SECURE_STORAGE

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/
#define qvCHIP_sensitive   true
#define qvCHIP_insensitive false

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#ifdef QVCHIP_SECURE_STORAGE
#define QV_CRYPTO_META_DATA_SIZE (ESEC_STOR_PROT_SIZE + ESEC_STOR_AUTH_SIZE)
#else
#define QV_CRYPTO_META_DATA_SIZE 0
#endif // QVCHIP_SECURE_STORAGE

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    NVM API
 *****************************************************************************/

/** @brief Initialization of NVM size and setting parameters.
 *  Added to work with fixed NVM library.
 */
void qvCHIP_NvmSetVariableSettings(void);

/** @brief Initialization of KVS variables (mutex)
 *  @return                 QV_STATUS_NO_ERROR - No error
 *                          QV_STATUS_NVM_ERROR - Cannot create mutex
 */
qvStatus_t qvCHIP_KvsInit(void);

/** @brief Function for KVS data consistency check.
 *  @return QV_STATUS_NO_ERROR - No error
 */
qvStatus_t qvCHIP_KvsConsistencyCheck(void);

/** @brief Store data to NVM for a given key.
 *
 *  @param key              String Identifier for NVM area to backup.
 *  @param value            Pointer to data to store.
 *  @param valueSize        Length of data to store. Cannot exceed maximum length for the key.
 *  @param isSensitive      Does @value need to be encrypted before committing it to storage.
 *  @return                 QV_STATUS_NO_ERROR - No error
 *                          QV_STATUS_INVALID_ARGUMENT - one of the arguments is not valid
 *                          QV_STATUS_INVALID_DATA - the data could not be stored
 */
qvStatus_t qvCHIP_KvsPut(const char* key, const void* value, size_t valueSize, bool isSensitive);

/** @brief Read back data from NVM for a given key.
 *
 *  @param key              Identifier for NVM area to restore.
 *  @param value            Pointer to the RAM location to restore data to.
 *  @param valueSize        Maximum length of data to restore.
 *  @param readBytesSize    Actual length of data that was restored. Cannot exceed maximum length
                            for the key.
 *  @param offsetBytes      Offset into the memory area. Currently not implemented
 *  @param isSensitive      Does @value need to be decrypted before returning it.
 *  @return                 QV_STATUS_NO_ERROR - no error
 *                          QV_STATUS_INVALID_ARGUMENT - one of the arguments is not valid
 *                          QV_STATUS_BUFFER_TOO_SMALL - the provided return data buffer is too small
 *                                                       for the data stored under the specified tag
 *                          QV_STATUS_INVALID_DATA - the data retrieved is not valid
 */
qvStatus_t qvCHIP_KvsGet(const char* key, void* value, size_t valueSize, size_t* readBytesSize, size_t offsetBytes,
                         bool isSensitive);

/** @brief Remove data from NVM for a given key.
 *
 *  @param key              Identifier for NVM data to remove.
 *  @return                 QV_STATUS_NO_ERROR - no error
 *                          QV_STATUS_INVALID_ARGUMENT - pointer to key is not valid
 *                          QV_STATUS_INVALID_DATA - the data could not be deleted
 */
qvStatus_t qvCHIP_KvsDelete(const char* key);

/** @brief Delete the entire KVS partition.
 *
 *  @return                 QV_STATUS_NO_ERROR - no error
 *                          QV_STATUS_INVALID_DATA - the partition could not be deleted
 */
qvStatus_t qvCHIP_KvsErasePartition(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_QVCHIP_KVS_H_
