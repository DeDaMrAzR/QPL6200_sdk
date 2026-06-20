/*
 * Copyright (c) 2023, Qorvo Inc
 *
 *   Hardware Abstraction Layer for ARM-based devices.
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
 *                    Includes Definitions
 *****************************************************************************/

#include "hal.h"
#include "hal_SEMailbox.h"
#include "esec_errors.h"
#include "esec_qorvo.h"  // uint32_t esec_nvm_write(uint32_t destination_address, uint32_t source_address, uint32_t length);
#include "esecure.h"     // void esec_enable_interrupts(void);

/*****************************************************************************
 *                    CLK frequency
 *****************************************************************************/

void hal_SEMailboxInit(void)
{
    esec_enable_interrupts();

    GP_WB_WRITE_ESEC_MB_CONFIG_RXINTEN(1);
    GP_WB_WRITE_ESEC_MB_CONFIG_TXINTEN(1);

    GP_WB_WRITE_INT_CTRL_MASK_APPUC_SECURE_ELEMENT_MAILBOX_RX_INTERRUPT(1);
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_SECURE_ELEMENT_INTERRUPT(1);

    // configure mutex to block parallel accesses to the mb.
    esec_init_mutex();
    // Communicate boot success to esecure
    esec_mark_boot_success();
}

UInt32 hal_SEMailbox_NvmWrite(UInt32 address, UInt32* data, UInt32 sizeInBytes)
{
    UInt32 status;
    status = esec_nvm_write((uint32_t)address, (uint32_t)data, (uint32_t)sizeInBytes);

    return status;
}

UInt32 hal_SEMailbox_ResetRequest(UInt32 reset_reason)
{
    UInt32 status;
    UInt8 soft_por_trigger = 0;
    switch(reset_reason)
    {
        case GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP:
            GP_WB_SET_PMUD_SOFT_POR_TO_SOFT_POR_TRIGGERS(soft_por_trigger, 1);
            break;
        case GP_WB_ENUM_POR_REASON_SOFT_POR_RRAM_BL_CRC_INVALID:
            GP_WB_SET_PMUD_SOFT_POR_INVALID_RRAM_BL_CRC_TO_SOFT_POR_TRIGGERS(soft_por_trigger, 1);
            break;
        case GP_WB_ENUM_POR_REASON_SOFT_POR_BOOTLOADER:
            GP_WB_SET_PMUD_SOFT_POR_BOOTLOADER_TO_SOFT_POR_TRIGGERS(soft_por_trigger, 1);
            break;
        case GP_WB_ENUM_POR_REASON_SOFT_POR_PRESERVE_DBG_ITF:
            GP_WB_SET_PMUD_SOFT_POR_PRESERVE_DBG_ITF_TO_SOFT_POR_TRIGGERS(soft_por_trigger, 1);
            break;
        default:
            return ESEC_INVALID_PARAMETER;
    }
    status = esec_reset_request(soft_por_trigger);

    return status;
}

UInt32 hal_SEMailbox_ResetWrite(uint8_t reset_index, uint32_t reset_value)
{
    UInt32 status;
    status = esec_standby_reset_write(reset_index, reset_value);

    return status;
}

UInt32 hal_SEMailbox_ConfigWakeupPins(UInt32 wakeup_pin_modes_0, UInt32 wakeup_pin_modes_1)
{
    UInt32 status;
    status = esec_config_gpio_wakeup(wakeup_pin_modes_0, wakeup_pin_modes_1);

    return status;
}

UInt32 hal_SEMailbox_XT32kPowerUp(bool start)
{
#ifdef GP_DIVERSITY_NO_SE
    GP_WB_WRITE_PMUD_XT32K_PUP(1);
    return ESEC_OKAY;
#else
    UInt32 status;
    status = esec_xt32k_power_up(start);

    return status;
#endif // GP_DIVERSITY_NO_SE
}

UInt32 hal_SEMailbox_SetAppucClkFreq(uint8_t clk_freq_enum)
{
    UInt32 status;
    status = esec_set_clk_freq(clk_freq_enum, core_enum_appuc);

    return status;
}

UInt32 hal_SEMailbox_SetAkumaClkFreq(uint8_t clk_freq_enum)
{
    UInt32 status;
    status = esec_set_clk_freq(clk_freq_enum, core_enum_akuma);

    return status;
}

UInt32 hal_SEMailbox_ChangeTemperature(int32_t temp_deg)
{
    GP_WB_WRITE_CAL_RAM_TEMP_DEG(temp_deg);
    return esec_change_temperature();
}

UInt32 hal_SEMailbox_ChangeBattVoltage(uint32_t vbatt_uv)
{
    GP_WB_WRITE_CAL_RAM_VBATT_UV(vbatt_uv);
    return esec_change_voltage();
}

UInt32 hal_SeMailboxAesCmacGenerate(UInt8* pMac, UInt8* pKey, UInt8 dataLength, UInt8* pData)
{
    // Currently only supporting key_size of 16 octets. Extend when needed
    struct esec_skey skey = {.storage=ESEC_STORAGE_HOST(pKey), .keyspec=16};

    return esec_cmac_generate(skey, pData, dataLength, pMac);
}

void hal_SEMailbox_MutexAcquire(void)
{
    esec_acquire_mutex();
}

void hal_SEMailbox_MutexRelease(void)
{
    esec_release_mutex();
}
