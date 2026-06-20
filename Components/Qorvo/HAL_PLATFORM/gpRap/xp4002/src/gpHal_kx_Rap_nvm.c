/*
 * Copyright (c) 2024, Qorvo Inc
 *
 * !!! This is a generated file. !!!
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
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

#include "gpHal.h"
#if defined(GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM)
#include "hal.h"
#endif //defined(GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM)

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

//wrappers/stubs for RAP code
#define abs(a)              (((a) < 0) ? -(a) : (a))

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/*
 * The actual RAP methods.
 */


void rap_nvm_enable_mri(void)
{
    GP_WB_WRITE_CMA_NVM_CTRL_ACCESS_ENABLE(0);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_CMA_NVM_CTRL_ACCESS_BLOCKED() != 1), 100);

    GP_WB_WRITE_CMA_MRI_NVM_OVERRULE_MODE(1);
}

void rap_nvm_disable_mri(void)
{
    rap_nvm_wait_for_idle();

    GP_WB_WRITE_CMA_MRI_NVM_OVERRULE_MODE(0);

    GP_WB_WRITE_CMA_NVM_CTRL_ACCESS_ENABLE(1);
}

void rap_nvm_por(UInt16 por_duration_us)
{
    UInt8 nvm_mri_enabled;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    GP_WB_WRITE_CMA_MRI_NVM_PORN(0);

    rap_h_wait_us(por_duration_us);

    GP_WB_WRITE_CMA_MRI_NVM_PORN(1);
}

void rap_nvm_recall(void)
{
    UInt8 nvm_mri_enabled;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(0);

    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_RECALL);
    GP_WB_CMA_MRI_NVM_CMD_TRIGGER();

    rap_nvm_wait_for_idle();
}

void rap_nvm_write_config(UInt32 xaddr)
{
    UInt8 nvm_mri_enabled;
    UInt32 addr;

    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    addr = xaddr * (1<<(5));
    GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(addr);
    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_WRITE_CONFIG);
    GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
    rap_h_wait_us(1);
    rap_nvm_wait_for_idle();
}

void rap_nvm_read_config(UInt32 xaddr)
{
    UInt8 nvm_mri_enabled;
    UInt32 addr;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    addr = xaddr * (1<<(5));
    GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(addr);
    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_READ_CONFIG);
    GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
    rap_h_wait_us(1);
    rap_nvm_wait_for_idle();

}

void rap_nvm_enable_testmode(void)
{
    GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_0(1);
    GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_1(0);
    rap_nvm_write_config(GP_WB_ENUM_NVM_TMC_REGISTER_TEST);
}

void rap_nvm_disable_testmode(void)
{
    GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_0(0);
    GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_1(0);
    rap_nvm_write_config(GP_WB_ENUM_NVM_TMC_REGISTER_TEST);
}

void rap_nvm_wait_for_idle(void)
{
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_CMA_MRI_NVM_ACCESS_BUSY() != 0), 5000);
}

void rap_nvm_write(UInt32 addr, Bool set_addr, Bool auto_increment_enable, Bool write_enable)
{
    UInt8 nvm_mri_enabled;
    UInt32 phys_addr;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    if (set_addr)
    {
        phys_addr = addr / (1<<(4));
        rap_h_assert_sys(addr == phys_addr *(1<<(4)));

        GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(phys_addr);
    }

    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_LOAD);
    if (auto_increment_enable & ~write_enable)
    {
        GP_WB_WRITE_CMA_MRI_NVM_CMD_TRIGGER_CTRL(5);
    }
    else
    {
        GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
    }

    if (write_enable)
    {
        GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_WRITE);
        if (auto_increment_enable)
        {
            GP_WB_WRITE_CMA_MRI_NVM_CMD_TRIGGER_CTRL(5);
        }
        else
        {
            GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
        }
        rap_h_wait_us(1);
        rap_nvm_wait_for_idle();
    }
}


void rap_nvm_write_eotp(UInt32 addr)
{
    UInt8 nvm_mri_enabled;
    UInt32 phys_addr;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    phys_addr = addr / (1<<(4));
    rap_h_assert_sys(addr == phys_addr *(1<<(4)));
    GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(phys_addr);

    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_WRITE_EOTP);
    GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
    rap_h_wait_us(1);
    rap_nvm_wait_for_idle();
}

void rap_nvm_destroy_eotp(UInt32 addr)
{
    UInt8 nvm_mri_enabled;
    UInt32 phys_addr;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    phys_addr = addr / (1<<(4));
    rap_h_assert_sys(addr == phys_addr *(1<<(4)));
    GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(phys_addr);

    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_DESTROY_EOTP);
    GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
    rap_h_wait_us(1);
    rap_nvm_wait_for_idle();
}

void rap_nvm_read(UInt32 addr, Bool set_addr, Bool auto_increment_enable)
{
    UInt8 nvm_mri_enabled;
    UInt32 phys_addr;
    nvm_mri_enabled = GP_WB_READ_CMA_MRI_NVM_OVERRULE_MODE();
    rap_h_assert_sys(nvm_mri_enabled == 1);

    if (set_addr)
    {
        phys_addr = addr / (1<<(4));
        rap_h_assert_sys(addr == phys_addr *(1<<(4)));

        GP_WB_WRITE_CMA_MRI_NVM_ADDRESS(phys_addr);
    }

    GP_WB_WRITE_CMA_MRI_NVM_CMD(GP_WB_ENUM_NVM_CMD_READ);
    if (auto_increment_enable)
    {
        GP_WB_WRITE_CMA_MRI_NVM_CMD_TRIGGER_CTRL(5);
    }
    else
    {
        GP_WB_CMA_MRI_NVM_CMD_TRIGGER();
    }
    rap_nvm_wait_for_idle();
}

Bool rap_nvm_check_tmc_register_access(UInt32 addr, Bool testmode_enabled)
{
    UInt32 backup_w0;
    UInt32 backup_w1;
    UInt32 data_wr_w0 = 0;
    UInt32 data_wr_w1 = 0;
    UInt32 data_rd_w0;
    UInt32 data_rd_w1;

    rap_nvm_read_config(addr);
    backup_w0 = GP_WB_READ_CMA_MRI_NVM_READ_DATA_0();
    backup_w1 = GP_WB_READ_CMA_MRI_NVM_READ_DATA_1();


    if (backup_w0 == data_wr_w0)
    {
        data_wr_w0 = 1;
    }
    if (backup_w1 == data_wr_w1)
    {
        data_wr_w1 = 1;
    }

    GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_0(data_wr_w0);
    GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_1(data_wr_w1);
    rap_nvm_write_config(addr);

    rap_nvm_read_config(addr);
    data_rd_w0 = GP_WB_READ_CMA_MRI_NVM_READ_DATA_0();
    data_rd_w1 = GP_WB_READ_CMA_MRI_NVM_READ_DATA_1();

    if (testmode_enabled)
    {
        GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_0(backup_w0);
        GP_WB_WRITE_CMA_MRI_NVM_WRITE_DATA_1(backup_w1);
        rap_nvm_write_config(addr);
        return (data_rd_w0 == data_wr_w0) && (data_rd_w1 == data_wr_w1);
    }
    else
    {
        return (data_rd_w0 == backup_w0) && (data_rd_w1 == backup_w1);
    }

}

void rap_nvm_init(void)
{
    UInt8 nvm_init_done_by_hw;

    nvm_init_done_by_hw = GP_WB_READ_CMA_NVM_CTRL_HW_INIT_MODE();

    if (nvm_init_done_by_hw == 0)
    {
        GP_WB_WRITE_CMA_NVM_CTRL_ACCESS_ENABLE(0);
        GP_WB_WRITE_CMA_MRI_NVM_PORN(0);
        GP_WB_WRITE_CMA_MRI_NVM_PD(1);
        GP_WB_WRITE_CMA_MRI_NVM_PMUD_ENABLE_VDD(0);
        GP_WB_WRITE_CMA_MRI_NVM_PMUD_POC_IO_SW(1);


        GP_WB_WRITE_CMA_MRI_NVM_OVERRULE_MODE(1);
    }

    GP_WB_WRITE_CMA_NVM_CTRL_INIT_ENABLE(1);

    if (nvm_init_done_by_hw == 0)
    {

        GP_WB_WRITE_CMA_MRI_NVM_PMUD_ENABLE_VDD(1);

        rap_h_wait_us(1);
        GP_WB_WRITE_CMA_MRI_NVM_PMUD_POC_IO_SW(0);

        GP_WB_WRITE_CMA_MRI_NVM_PORN(1);
        rap_h_wait_us(1);

        GP_WB_WRITE_CMA_MRI_NVM_PD(0);
        rap_h_wait_us(1);

        rap_nvm_wait_for_idle();

        GP_WB_CMA_MRI_NVM_CLR_INTERRUPT();
        rap_nvm_disable_mri();

        GP_WB_WRITE_CMA_NVM_CTRL_ACCESS_ENABLE(1);
    }

}

