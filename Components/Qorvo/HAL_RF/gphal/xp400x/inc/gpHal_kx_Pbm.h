/*
 * Copyright (c) 2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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

#ifndef _GPHAL_KX_PBM_H_
#define _GPHAL_KX_PBM_H_

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/**  The size of an Enhanced ACK frame PDU with an encryption header in octets.
 *         As specified in IEEE802.15.4-2015 paragraph 7.3.3 Acknowledgment frame format.
 *         The current max value is the value with:
 *         Extended Dest addressing, PanID compression, Aux header,
 *         One Thread CSL IE, VS IE and MIC.
 *         fctrl | len | pan | dst | sec | csl ies | vs ie | MIC | fcs |
 *           2      1     2     8     6      2+4      2+4     4     2    = 37
 */
// The size and amount of the pbms that are reserved for special use (empty PDU, ACK PBM) multiple of 4.
#define GP_HAL_RESERVED_PBM_SIZE 40

#ifndef GP_HAL_MAX_NR_OF_RESERVED_PBMS
#define GP_HAL_MAX_NR_OF_RESERVED_PBMS 3
#endif

#define GPHAL_PBM_15_4_MAX_SIZE         128
// The maximum size we allow for the BLE PBMs. Must be aligned to at least four bytes.
#define GPHAL_PBM_BLE_SIZE_MAX          268

// The number of bytes in the (BLE) PBM that cannot be used, because they are reserved for the HW/RT system
// We need to allocate three bytes for the CRC (this is only needed in the RX path)
#define GP_HAL_PBM_BLE_NR_RESERVED_BYTES    3

#ifndef GP_HAL_PBM_TYPE1_SIZE
#define GP_HAL_PBM_TYPE1_SIZE       GPHAL_PBM_15_4_MAX_SIZE
#endif //GP_HAL_PBM_TYPE1_SIZE

#ifndef GP_HAL_PBM_TYPE2_SIZE
#define GP_HAL_PBM_TYPE2_SIZE       GPHAL_PBM_BLE_SIZE_MAX
#endif //GP_HAL_PBM_TYPE2_SIZE

#ifndef GP_HAL_PBM_TYPE1_AMOUNT
#define GP_HAL_PBM_TYPE1_AMOUNT     8
#endif //GP_HAL_PBM_TYPE1_AMOUNT

#ifndef GP_HAL_PBM_TYPE2_AMOUNT
#define GP_HAL_PBM_TYPE2_AMOUNT 21
#endif //GP_HAL_PBM_TYPE1_AMOUNT

// The max size of a PBM currently maps onto the size of the type 2 pbms
#define GP_HAL_PBM_MAX_SIZE     (GP_HAL_PBM_TYPE2_SIZE)

// By default, half of the max size PBMs are used for RX
#ifndef GP_HAL_NR_OF_RX_PBMS
#define GP_HAL_NR_OF_RX_PBMS    (GP_HAL_PBM_TYPE2_AMOUNT >> 1)
#endif

// Used when a format_r or format_t ptr is not valid
#define GP_HAL_FORMAT_PTR_INVALID       (0x0000)

#define GP_MM_RAM_PBM_OPTS_SIZE                     (0x44) // Size aligned to nearest 32-bit - max size is format_R 32 bytes
#define GP_MM_RAM_PBM_OPTS_NR_OF                    (32)   // This can be increased/decreased

#define GP_HAL_NR_OF_RCI_QUEUES ((GP_WB_RT_FORMAT_R_PTRS_BASE_PTR_ADDRESS - GP_WB_RT_RCI_DATA_IND_HEAD_PTR_ADDRESS) / GP_WB_RT_RCI_BLE_DATA_CNF_HEAD_PTR_LEN)

// The number of pbms the user has specified (or the defaults)
#define GP_HAL_NR_OF_PBMS_SPECIFIED_BY_USER (GP_HAL_PBM_TYPE1_AMOUNT + GP_HAL_PBM_TYPE2_AMOUNT)

#define GP_HAL_NR_OF_RCI_ENTRIES (GP_HAL_NR_OF_PBMS_SPECIFIED_BY_USER + \
                                  GP_HAL_MAX_NR_OF_RESERVED_PBMS + \
                                  GP_HAL_NR_OF_RCI_QUEUES /* because we also need one empty RCI entry per queue (see gpHal_RtRciPop) */)

#define GPHAL_MM_PBM_NR_OF             (GP_MM_RAM_PBM_OPTS_NR_OF - 1 - 1 - 1) /*ACK buffer always claimed + extra buffer for Ack Rx + channel change PBM */

/*****************************************************************************
 *                    Function Prototypes
 *****************************************************************************/

// Get the base pointer of options buffer for a pbm entry
UInt32 gpHal_getOptsBase(UInt8 pbmEntry);

// Get the base pointer of data buffer for a pbm entry
UInt32 gpHal_getDataBase(UInt8 pbmEntry);

// Check if this pbm is in use
Bool gpHal_isPbmAllocated(UInt8 pbmEntry);

// Get pointer to options buffer for the pbm reserved for ACK
UInt32 gpHal_getAckOptsBase(void);

// retrieve the pbm entry given the base pointer of options buffer
UInt8 gpHal_findPbmHandle(UInt32 optsbase);

UInt16 gpHal_PbmToFormatPtr(UInt8 pbmEntry);

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

// Ack pbm address
#define GPHAL_ACK_PBM_ADDR                          gpHal_getAckOptsBase()

#define GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmEntry)    gpHal_getOptsBase(pbmEntry)
#define GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(pbmEntry)   gpHal_getDataBase(pbmEntry)
#define GP_HAL_PBM_ENTRY2ADDR(entry)                GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(entry)
#define GP_HAL_CHECK_PBM_VALID(entry)               ((entry) < GP_MM_RAM_PBM_OPTS_NR_OF)
#define GP_HAL_IS_PBM_ALLOCATED(entry)              (gpHal_isPbmAllocated(entry))
#define GP_HAL_PBM_OFFSET_VALID(offset)             ((UInt16)(offset) < GP_HAL_PBM_MAX_SIZE)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#endif //_GPHAL_KX_PBM_H_
