/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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
#ifdef GP_DIVERSITY_GPHAL_XP4002

/* The RAP code generates the sw-defined registers also with the AK prefix */
/* In order to keep the generated code as-is, we add the translation macros here */
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#undef GP_WB_READ_AK_RT_RCI_ENTRY_NEXT_PTR
#undef GP_WB_READ_AK_RT_RCI_ENTRY_FORMAT_PTR
#undef GP_WB_WRITE_AK_RT_RCI_ENTRY_NEXT_PTR
#undef GP_WB_WRITE_AK_RT_RCI_ENTRY_FORMAT_PTR
#undef GP_WB_READ_AK_RT_FORMAT_R_PTRS_NR
#undef GP_WB_READ_AK_RT_FORMAT_R_PTRS_BASE_PTR


#define GP_WB_READ_AK_RT_RCI_ENTRY_NEXT_PTR(val)                GP_WB_READ_RT_RCI_ENTRY_NEXT_PTR(val)
#define GP_WB_READ_AK_RT_RCI_ENTRY_FORMAT_PTR(val)              GP_WB_READ_RT_RCI_ENTRY_FORMAT_PTR(val)
#define GP_WB_WRITE_AK_RT_RCI_ENTRY_NEXT_PTR(ptr, val)          GP_WB_WRITE_RT_RCI_ENTRY_NEXT_PTR(ptr, val)
#define GP_WB_WRITE_AK_RT_RCI_ENTRY_FORMAT_PTR(ptr, val)        GP_WB_WRITE_RT_RCI_ENTRY_FORMAT_PTR(ptr, val)
#define GP_WB_READ_AK_RT_FORMAT_R_PTRS_NR(val)                  GP_WB_READ_RT_FORMAT_R_PTRS_NR(val)
#define GP_WB_READ_AK_RT_FORMAT_R_PTRS_BASE_PTR(val)            GP_WB_READ_RT_FORMAT_R_PTRS_BASE_PTR(val)

/* this "undef" needs to remain untill halprop(_basic).h is cleaned up */
#undef GP_WB_AK_RT_RCI_DATA_IND_HEAD_PTR_ADDRESS
#define GP_WB_AK_RT_RCI_DATA_IND_HEAD_PTR_ADDRESS             GP_WB_RT_RCI_DATA_IND_HEAD_PTR_ADDRESS

#endif //GP_DIVERSITY_GPHAL_XP4002
