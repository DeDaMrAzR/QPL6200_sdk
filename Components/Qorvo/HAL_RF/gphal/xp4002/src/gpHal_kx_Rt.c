/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
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

/*****************************************************************************
 *                    Include Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpHal_kx_Rt.h"
#include "gpLog.h"
#include "gpAssert.h"
#include "gp_global.h"
#include "gpHal_kx_Ipc.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_HAL_RT_ADDRESS_INVALID           0

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
 *                    Static Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
void gpHal_CheckPbmConflicts(UInt32 fr_ptr)
{
    UInt8 i;
    UInt8 nr_entries;
    UInt32 fr_list_ptr = FROM_RT_ADDR(GP_WB_READ_RT_FORMAT_R_PTRS_BASE_PTR());

    nr_entries = GP_WB_READ_RT_FORMAT_R_PTRS_NR();

    if(gpHal_GetCurrentRxDataPtr() == GP_WB_READ_FORMAT_R_FRAME_PTR(fr_ptr))
    {
        GP_LOG_SYSTEM_PRINTF("[ERROR] Format ptr to free is already in use!",0);
        gpLog_Flush();
        GP_ASSERT_SYSTEM(false);
    }

    for (i = 0; i < nr_entries; i++)
    {
        UInt16 list_ptr = GP_WB_READ_U16(fr_list_ptr + 2 * i);

        if (list_ptr != 0)
        {
            if(list_ptr == ak_rap_rt_to_ptr(fr_ptr))
            {
                GP_LOG_SYSTEM_PRINTF("[ERROR] Format ptr to free is already in queue!",0);
                gpLog_Flush();
                GP_ASSERT_SYSTEM(false);
            }
        }
    }

}
#endif //GP_DIVERSITY_DEVELOPMENT

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

UIntPtr gpHal_GetCurrentRxDataPtr(void)
{
    UIntPtr dmaPtr;

    dmaPtr = GP_WB_DMAS_SCB_SRC_ADDR_0_ADDRESS + GP_WB_READ_RT_RX_DMA_CHANNEL()*(GP_WB_DMAS_SCB_SRC_ADDR_1_ADDRESS-GP_WB_DMAS_SCB_SRC_ADDR_0_ADDRESS);

    return GP_WB_READ_DMA_SCB_DEST_ADDR(dmaPtr);
}

UInt16 gpHal_RtAddressToOffset(UInt32 address)
{
    if(address == GP_HAL_RT_ADDRESS_INVALID)
    {
        return GP_HAL_RT_ADDRESS_INVALID;
    }

    if(RANGE_CHECK(address, GP_MM_AKUMA_RAM_LINEAR_START, GP_MM_AKUMA_RAM_LINEAR_END))
    {
        return address - GP_MM_AKUMA_RAM_LINEAR_START;
    }
    else if(RANGE_CHECK(address, GP_MM_SYSRAM_START, GP_MM_SYSRAM_END))
    {
        return address - GP_MM_SYSRAM_START;
    }

    return GP_HAL_RT_ADDRESS_INVALID;
}

UInt32 gpHal_RtRciPop(UInt32 queue_ptr)
{
    UInt32 tail_ptr;
    UInt32 prev_ptr;

    tail_ptr = queue_ptr;
    prev_ptr = 0;
    {
        UInt16 next_rt_ptr;
        next_rt_ptr = GP_WB_READ_U16(queue_ptr); // read RT head address
        // find the tail
        UInt8 dbg_count = 0;
        while (next_rt_ptr && 0xFFFF != next_rt_ptr)
        {
            GP_ASSERT_SYSTEM(dbg_count++ < GP_HAL_NR_OF_RCI_ENTRIES);

            prev_ptr = tail_ptr;
            tail_ptr = FROM_RT_ADDR(next_rt_ptr);
            next_rt_ptr = GP_WB_READ_RT_RCI_ENTRY_NEXT_PTR(tail_ptr);
        }
    }

    if (tail_ptr == queue_ptr)
    {
        // So we get an interrupt but no item is queued. How is this case even possible?
        // why don't we clear the interrupt here?
        return 0xFFFFFFFF;
    }
    else
    {
        UInt32 format_ptr = GP_WB_READ_RT_RCI_ENTRY_FORMAT_PTR(tail_ptr);

        if (prev_ptr == queue_ptr /* only one entry in the queue*/)
        {
            /* We don't clear the queue_ptr because the RT could be reading/writing concurrently
            * Instead we always keep at least one "empty" RCI_ENTRY in the list.
            * We also set the tail value to 0xFFFF so RT cannot claim this entry after
            * RCI_ENTRY_FORMAT_PTR is set to 0 (below) and create a circular linked list. */
            GP_WB_WRITE_RT_RCI_ENTRY_NEXT_PTR(tail_ptr, 0xFFFF);
            GP_WB_WRITE_AK_IPC_CLR_RT2X_RCI_INTERRUPTS((1<<((queue_ptr-GP_WB_RT_RCI_DATA_IND_HEAD_PTR_ADDRESS)/2)));
        }
        else
        {
            // Since this is not the only entry RT can reclaim this one
            GP_WB_WRITE_RT_RCI_ENTRY_NEXT_PTR(tail_ptr, 0);
            GP_WB_WRITE_RT_RCI_ENTRY_NEXT_PTR(prev_ptr, 0xFFFF);
        }

        if (format_ptr)
        {
            /* We clear the RT_RCI_ENTRY_FORMAT_PTR so that future pops will not
            *  report this element a second time. This also enables RT to claim
            *  the entry again. */
            GP_WB_WRITE_RT_RCI_ENTRY_FORMAT_PTR(tail_ptr, 0);
            UInt32 prt = FROM_RT_ADDR(format_ptr);
            return prt;
        }
        else if (prev_ptr != queue_ptr)
        {
            // This is the empty tail entry. We do a recursive call so that we
            // return the format_ptr from the next entry (will be non-zero).
            return gpHal_RtRciPop(queue_ptr);
        }
        else
        {
            // This means there was a new interrupt while no entry was added
            // to the queue and only the empty entry is here.
            // This should not happen.
            GP_ASSERT_DEV_EXT(false);
            return 0xFFFFFFFF;
        }
    }
    return 0xFFFFFFFF;
}

Bool gpHal_RtAddFrPtr(UInt32 fr_ptr)
{
    UInt8 nr_entries;
    UInt8 i;
    UInt32 fr_list_ptr;
    UInt16 tmp_val;

    nr_entries = GP_WB_READ_RT_FORMAT_R_PTRS_NR();
    fr_list_ptr = GP_WB_READ_RT_FORMAT_R_PTRS_BASE_PTR();
    fr_list_ptr = FROM_RT_ADDR(fr_list_ptr);

#ifdef GP_DIVERSITY_DEVELOPMENT
    // Perform some checks on the format_ptr to return (check if it not already returned, or currently in use by the radio)
    gpHal_CheckPbmConflicts(fr_ptr);
#endif //GP_DIVERSITY_DEVELOPMENT

    for (i=0; i<nr_entries; i++)
    {
        tmp_val = GP_WB_READ_U16(fr_list_ptr);
        if (tmp_val == 0)
        {
            GP_WB_WRITE_U16(fr_list_ptr, ak_rap_rt_to_ptr(fr_ptr));
            tmp_val = GP_WB_READ_AK_IPC_UNMASKED_RT2X_RX_PTR_LIST_EMPTY_INTERRUPT();
            if (tmp_val == 1)
            {
                gpHal_Result_t result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_RENEW_ACTIVE_FORMAT_R_PTR, 0, NULL);
                GP_ASSERT_SYSTEM(gpHal_ResultSuccess == result);
                GP_WB_AK_IPC_CLR_RT2X_RX_PTR_LIST_EMPTY_INTERRUPT();
            }
            return true;
        }
        fr_list_ptr = fr_list_ptr+2;
    }
    return false;
}
