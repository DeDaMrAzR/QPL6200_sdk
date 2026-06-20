/*
 * Copyright (c) 2024, Qorvo Inc
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

#ifndef _REG_QUIC_H_
#define _REG_QUIC_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief QUIC channel. */
typedef UInt8 qRegQUIC_Channel_t;

/** @brief Macro for getting the channel address. */
#define Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel) (GP_WB_QUIC_BASE_ADDRESS + ((UInt32)(channel) * sizeof(UInt32)))

/** @enum qRegQUIC_TriggerSrc_t */
//@{
#define qRegQUIC_TriggerSrcNoTrigger             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_NO_TRIGGER
#define qRegQUIC_TriggerSrcAkumaIpc0             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_0
#define qRegQUIC_TriggerSrcAkumaIpc1             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_1
#define qRegQUIC_TriggerSrcAkumaIpc2             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_2
#define qRegQUIC_TriggerSrcAkumaIpc3             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_3
#define qRegQUIC_TriggerSrcAkumaPaOn             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_PA_ON
#define qRegQUIC_TriggerSrcAkumaPhyRx            GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_PHY_RX
#define qRegQUIC_TriggerSrcAkumaPhyTx            GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_PHY_TX
#define qRegQUIC_TriggerSrcAkumaRxOn             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_RX_ON
#define qRegQUIC_TriggerSrcAkumaTxOn             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_TX_ON
#define qRegQUIC_TriggerSrcBistTsensAboveMaximum GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_BIST_TSENS_ABOVE_MAXIMUM
#define qRegQUIC_TriggerSrcBistTsensBelowMinimum GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_BIST_TSENS_BELOW_MINIMUM
#define qRegQUIC_TriggerSrcDcdcClkP1             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DCDC_CLKP1
#define qRegQUIC_TriggerSrcDcdcClkP2             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DCDC_CLKP2
#define qRegQUIC_TriggerSrcDcdcBufferComplete0   GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DMA_DCB_BUFFER_COMPLETE_0
#define qRegQUIC_TriggerSrcEsEvent0              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_0
#define qRegQUIC_TriggerSrcEsEvent1              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_1
#define qRegQUIC_TriggerSrcEsEvent2              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_2
#define qRegQUIC_TriggerSrcEsEvent3              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_3
#define qRegQUIC_TriggerSrcEsEvent4              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_4
#define qRegQUIC_TriggerSrcEsEvent5              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_5
#define qRegQUIC_TriggerSrcEsEvent6              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_6
#define qRegQUIC_TriggerSrcEsEvent7              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_7
#define qRegQUIC_TriggerSrcEsEvent8              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_8
#define qRegQUIC_TriggerSrcEsEvent9              GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_9
#define qRegQUIC_TriggerSrcEsEvent10             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_10
#define qRegQUIC_TriggerSrcEsEvent11             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_11
#define qRegQUIC_TriggerSrcEsEvent12             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_12
#define qRegQUIC_TriggerSrcEsEvent13             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_13
#define qRegQUIC_TriggerSrcEsEvent14             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_14
#define qRegQUIC_TriggerSrcEsEvent15             GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_15
#define qRegQUIC_TriggerSrcGpio0                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_0
#define qRegQUIC_TriggerSrcGpio1                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_1
#define qRegQUIC_TriggerSrcGpio2                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_2
#define qRegQUIC_TriggerSrcGpio3                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_3
#define qRegQUIC_TriggerSrcGpio4                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_4
#define qRegQUIC_TriggerSrcGpio5                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_5
#define qRegQUIC_TriggerSrcGpio6                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_6
#define qRegQUIC_TriggerSrcGpio7                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_7
#define qRegQUIC_TriggerSrcGpio8                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_8
#define qRegQUIC_TriggerSrcGpio9                 GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_9
#define qRegQUIC_TriggerSrcGpio10                GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_10
#define qRegQUIC_TriggerSrcGpio11                GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_11
#define qRegQUIC_TriggerSrcGpio12                GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_12
#define qRegQUIC_TriggerSrcGpio13                GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_13
#define qRegQUIC_TriggerSrcGpio14                GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_14
#define qRegQUIC_TriggerSrcGpio15                GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_15
#define qRegQUIC_TriggerSrcPmudLpcmpMatch        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PMUD_LPCMP_MATCH
#define qRegQUIC_TriggerSrcPwmxl0PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_0_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl0PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_0_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl0PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_0_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl1PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_1_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl1PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_1_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl1PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_1_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl2PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_2_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl2PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_2_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl2PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_2_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl3PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_3_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl3PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_3_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl3PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_3_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl4PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_4_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl4PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_4_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl4PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_4_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl5PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_5_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl5PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_5_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl5PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_5_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl6PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_6_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl6PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_6_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl6PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_6_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxl7PhaseMatch      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_7_PHASE_MATCH
#define qRegQUIC_TriggerSrcPwmxl7PhaseOff        GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_7_PHASE_OFF
#define qRegQUIC_TriggerSrcPwmxl7PhaseOn         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_7_PHASE_ON
#define qRegQUIC_TriggerSrcPwmxlTopOverflow      GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_TOP_OVERFLOW
#define qRegQUIC_TriggerSrcTimerTmr0Wrap         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR0_WRAP
#define qRegQUIC_TriggerSrcTimerTmr1Wrap         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR1_WRAP
#define qRegQUIC_TriggerSrcTimerTmr2Wrap         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR2_WRAP
#define qRegQUIC_TriggerSrcTimerTmr3Wrap         GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR3_WRAP
#define qRegQUIC_TriggerSrcTimerXL0CC0Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_0_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL0CC0Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_0_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL0CC1Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_1_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL0CC1Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_1_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL0CC2Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_2_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL0CC2Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_2_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL0CC3Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_3_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL0CC3Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_3_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL0Overflow0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_OVERFLOW_0
#define qRegQUIC_TriggerSrcTimerXL0Overflow1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_OVERFLOW_1
#define qRegQUIC_TriggerSrcTimerXL1CC0Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_0_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL1CC0Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_0_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL1CC1Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_1_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL1CC1Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_1_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL1CC2Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_2_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL1CC2Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_2_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL1CC3Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_3_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL1CC3Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_3_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL1Overflow0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_OVERFLOW_0
#define qRegQUIC_TriggerSrcTimerXL1Overflow1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_OVERFLOW_1
#define qRegQUIC_TriggerSrcTimerXL2CC0Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_0_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL2CC0Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_0_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL2CC1Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_1_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL2CC1Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_1_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL2CC2Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_2_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL2CC2Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_2_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL2CC3Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_3_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL2CC3Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_3_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL2Overflow0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_OVERFLOW_0
#define qRegQUIC_TriggerSrcTimerXL2Overflow1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_OVERFLOW_1
#define qRegQUIC_TriggerSrcTimerXL3CC0Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_0_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL3CC0Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_0_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL3CC1Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_1_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL3CC1Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_1_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL3CC2Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_2_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL3CC2Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_2_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL3CC3Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_3_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL3CC3Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_3_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL3Overflow0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_OVERFLOW_0
#define qRegQUIC_TriggerSrcTimerXL3Overflow1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_OVERFLOW_1
#define qRegQUIC_TriggerSrcTimerXL4CC0Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_0_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL4CC0Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_0_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL4CC1Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_1_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL4CC1Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_1_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL4CC2Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_2_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL4CC2Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_2_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL4CC3Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_3_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL4CC3Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_3_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL4Overflow0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_OVERFLOW_0
#define qRegQUIC_TriggerSrcTimerXL4Overflow1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_OVERFLOW_1
#define qRegQUIC_TriggerSrcTimerXL5CC0Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_0_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL5CC0Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_0_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL5CC1Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_1_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL5CC1Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_1_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL5CC2Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_2_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL5CC2Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_2_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL5CC3Match0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_3_MATCH_0
#define qRegQUIC_TriggerSrcTimerXL5CC3Match1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_3_MATCH_1
#define qRegQUIC_TriggerSrcTimerXL5Overflow0     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_OVERFLOW_0
#define qRegQUIC_TriggerSrcTimerXL5Overflow1     GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_OVERFLOW_1
/** @typedef qRegQUIC_TriggerSrc_t
 *  @brief QUIC trigger source.
 */
typedef UInt16 qRegQUIC_TriggerSrc_t;
//@}

/** @enum qRegQUIC_Edge_t */
//@{
#define qRegQUIC_EdgeRising        GP_WB_ENUM_CHANNEL_EDGE_SEL_RISING
#define qRegQUIC_EdgeFalling       GP_WB_ENUM_CHANNEL_EDGE_SEL_FALLING
#define qRegQUIC_EdgeLevel         GP_WB_ENUM_CHANNEL_EDGE_SEL_LEVEL
#define qRegQUIC_EdgeLevelInverted GP_WB_ENUM_CHANNEL_EDGE_SEL_LEVEL_INVERTED
#define qRegQUIC_EdgeRiseAndFall   GP_WB_ENUM_CHANNEL_EDGE_SEL_RISE_AND_FALL
/** @typedef qRegQUIC_Edge_t
 *  @brief QUIC edge selection.
 */
typedef UInt16 qRegQUIC_Edge_t;
//@}

/** @enum qRegQUIC_Speed_t */
//@{
#define qRegQUIC_Speed32M GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M32
#define qRegQUIC_Speed16M GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M16
#define qRegQUIC_Speed8M  GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M8
#define qRegQUIC_Speed4M  GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M4
/** @typedef qRegQUIC_Speed_t
 *  @brief QUIC speed selection.
 */
typedef UInt16 qRegQUIC_Speed_t;
//@}

/** @brief Set the trigger source to specified channel. */
static inline void qRegQUIC_TriggerSrcSet(qRegQUIC_Channel_t channel, qRegQUIC_TriggerSrc_t triggerSrc)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    GP_WB_MWRITE_U32(channelBaseAddress, GP_WB_QUIC_CHANNEL_0_TRIGGER_SRC_MASK,
                     (triggerSrc << GP_WB_QUIC_CHANNEL_0_TRIGGER_SRC_LSB));
}

/** @brief Get the trigger source from specified channel. */
static inline qRegQUIC_TriggerSrc_t qRegQUIC_TriggerSrcGet(qRegQUIC_Channel_t channel)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    return ((GP_WB_READ_U32(channelBaseAddress) & GP_WB_QUIC_CHANNEL_0_TRIGGER_SRC_MASK) >>
            GP_WB_QUIC_CHANNEL_0_TRIGGER_SRC_LSB);
}

/** @brief Set the edge to the specified channel. */
static inline void qRegQUIC_EdgeSet(qRegQUIC_Channel_t channel, qRegQUIC_Edge_t edge)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    GP_WB_MWRITE_U32(channelBaseAddress, GP_WB_QUIC_CHANNEL_0_EDGE_SEL_MASK,
                     (edge << GP_WB_QUIC_CHANNEL_0_EDGE_SEL_LSB));
}

/** @brief Get the edge from the specified channel. */
static inline qRegQUIC_Edge_t qRegQUIC_EdgeGet(qRegQUIC_Channel_t channel)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    return (
        (GP_WB_READ_U32(channelBaseAddress) & GP_WB_QUIC_CHANNEL_0_EDGE_SEL_MASK) >> GP_WB_QUIC_CHANNEL_0_EDGE_SEL_LSB);
}

/** @brief Set the source speed for the specified channel. */
static inline void qRegQUIC_SrcSpeedSet(qRegQUIC_Channel_t channel, qRegQUIC_Speed_t speed)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    GP_WB_MWRITE_U32(channelBaseAddress, GP_WB_QUIC_CHANNEL_0_SRC_SPEED_MASK,
                     (speed << GP_WB_QUIC_CHANNEL_0_SRC_SPEED_LSB));
}

/** @brief Get the source speed from the specified channel. */
static inline qRegQUIC_Speed_t qRegQUIC_SrcSpeedGet(qRegQUIC_Channel_t channel)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    return ((GP_WB_READ_U32(channelBaseAddress) & GP_WB_QUIC_CHANNEL_0_SRC_SPEED_MASK) >>
            GP_WB_QUIC_CHANNEL_0_SRC_SPEED_LSB);
}

/** @brief Set the destination speed for the specified channel. */
static inline void qRegQUIC_DstSpeedSet(qRegQUIC_Channel_t channel, qRegQUIC_Speed_t speed)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    GP_WB_MWRITE_U32(channelBaseAddress, GP_WB_QUIC_CHANNEL_0_DEST_SPEED_MASK,
                     (speed << GP_WB_QUIC_CHANNEL_0_DEST_SPEED_LSB));
}

/** @brief Get the destination speed from the specified channel. */
static inline qRegQUIC_Speed_t qRegQUIC_DstSpeedGet(qRegQUIC_Channel_t channel)
{
    UInt32 channelBaseAddress = Q_REG_QUIC_CHANNEL_ADDRESS_GET(channel);

    return ((GP_WB_READ_U32(channelBaseAddress) & GP_WB_QUIC_CHANNEL_0_DEST_SPEED_MASK) >>
            GP_WB_QUIC_CHANNEL_0_DEST_SPEED_LSB);
}

/** @brief Set the aggregation mask for the specified channel. */
static inline void qRegQUIC_AggregationMaskSet(qRegQUIC_Channel_t channel, UInt32 mask)
{
    UInt32 aggregationChannelMaskAddress = GP_WB_QUIC_CHANNEL_0_MASK_ADDRESS + (sizeof(UInt32) * channel);
    GP_WB_MWRITE_U32(aggregationChannelMaskAddress, mask, mask);
}

/** @brief Clear the aggregation mask for the specified channel. */
static inline void qRegQUIC_AggregationMaskClr(qRegQUIC_Channel_t channel, UInt32 mask)
{
    UInt32 aggregationChannelMaskAddress = GP_WB_QUIC_CHANNEL_0_MASK_ADDRESS + (sizeof(UInt32) * channel);
    GP_WB_MWRITE_U32(aggregationChannelMaskAddress, mask, 0);
}

/** @brief Get the aggregation mask from the specified channel. */
static inline UInt32 qRegQUIC_AggregationMaskGet(qRegQUIC_Channel_t channel)
{
    UInt32 aggregationChannelMaskAddress = GP_WB_QUIC_CHANNEL_0_MASK_ADDRESS + (sizeof(UInt32) * channel);
    return GP_WB_READ_U32(aggregationChannelMaskAddress);
}

#ifdef __cplusplus
}
#endif

#endif /* _REG_QUIC_H_ */
