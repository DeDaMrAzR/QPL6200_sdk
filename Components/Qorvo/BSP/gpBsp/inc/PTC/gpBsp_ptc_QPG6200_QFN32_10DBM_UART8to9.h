
/*
#   Copyright (c) 2011-2016, GreenPeak Technologies
#   Copyright (c) 2017-2021, Qorvo Inc
 *
 *
 *   QPG6200 PTC 10DBM Single Ended UART 8 to 9
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


#ifndef _GPBSP_PTC_QPG6200_QFN32_10DBM_UART8TO9_H_
#define _GPBSP_PTC_QPG6200_QFN32_10DBM_UART8TO9_H_

/*****************************************************************************
 *                    BSP configuration interface
 *****************************************************************************/

#define GP_BSP_INTERNAL_UC
// Has the board an differential antenna?
#define GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA() 0

/*****************************************************************************
 *                    Debug
 *****************************************************************************/

//SWJDP interface will be disabled at startup - overlapping pins:
// Pin 22 - GPIO 6 - SWDIO_TMS
// Pin 23 - GPIO 7 - SWCLK_TCK

#define GP_BSP_DEBUG_SWJDP_AVAILABLE

//SIF mapping used in debug mode:
// Pin 22 - GPIO 6 - SWDIO_TMS
#define GP_BSP_DEBUG_SWJDP_SWDIO_TMS_GPIO           6
#define GP_BSP_DEBUG_SWJDP_SWDIO_TMS_ALTERNATE      GP_WB_ENUM_GPIO_6_ALTERNATES_CSSYS_SWDIO_TMS
#define GP_BSP_DEBUG_SWJDP_SWDIO_TMS_INIT()         do{ GP_WB_WRITE_IOB_GPIO_6_ALTERNATE(GP_WB_ENUM_GPIO_6_ALTERNATES_CSSYS_SWDIO_TMS); GP_WB_WRITE_IOB_GPIO_6_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_DEBUG_SWJDP_SWDIO_TMS_DEINIT()       GP_WB_WRITE_IOB_GPIO_6_ALTERNATE_ENABLE(0);
// Pin 23 - GPIO 7 - SWCLK_TCK
#define GP_BSP_DEBUG_SWJDP_SWCLK_TCK_GPIO           7
#define GP_BSP_DEBUG_SWJDP_SWCLK_TCK_ALTERNATE      GP_WB_ENUM_GPIO_7_ALTERNATES_CSSYS_SWCLK_TCK
#define GP_BSP_DEBUG_SWJDP_SWCLK_TCK_INIT()         do{ GP_WB_WRITE_IOB_GPIO_7_ALTERNATE(GP_WB_ENUM_GPIO_7_ALTERNATES_CSSYS_SWCLK_TCK); GP_WB_WRITE_IOB_GPIO_7_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_DEBUG_SWJDP_SWCLK_TCK_DEINIT()       GP_WB_WRITE_IOB_GPIO_7_ALTERNATE_ENABLE(0);

/*****************************************************************************
 *                    GPIO - LED
 *****************************************************************************/

//No Leds on this board
#define GP_DIVERSITY_NO_LED

#define HAL_LED_INIT_LEDS()                         do{ \
}while(0)

#define GP_BSP_LED_GPIO_MAP                         { 0xff, 0xff, 0xff, 0xff }
#define GP_BSP_LED_ALTERNATE_MAP                    { 0, 0, 0, 0 }

/*****************************************************************************
 *                    GPIO - BTN - No buttons used
 *****************************************************************************/

#define HAL_BTN_INIT_BTNS()                         do{ \
}while(0)

/*****************************************************************************
 *                    GPIO - ALTERNATIVE - not used
 *****************************************************************************/


/*****************************************************************************
 *                    UART
 *****************************************************************************/

#define GP_BSP_UART0_RX_DEFINED()                   (0)
#define GP_BSP_UART0_RX_GPIO_CFG()                  do { } while(0)
#define GP_BSP_UART0_RX_ENABLE(en)                  do { } while(0)
#define GP_BSP_UART0_RX_ENABLED()                   (0)

#define GP_BSP_UART0_TX_DEFINED()                   (0)
#define GP_BSP_UART0_TX_GPIO_CFG()                  do { } while(0)
#define GP_BSP_UART0_TX_ENABLE(en)                  do { } while(0)
#define GP_BSP_UART0_TX_ENABLED()                   (0)

#define GP_BSP_UART0_CTS_DEFINED()                  (0)
#define GP_BSP_UART0_CTS_GPIO_CFG()                 do { } while(0)
#define GP_BSP_UART0_CTS_ENABLE(en)                 do { } while(0)
#define GP_BSP_UART0_CTS_ENABLED()                  (0)

// Pin 24 - GPIO 8 - UART1_RX
#define GP_BSP_UART1_RX_GPIO                        8
#define GP_BSP_UART1_RX_ALTERNATE                   GP_WB_ENUM_GPIO_8_ALTERNATES_UART_1_RX
#define GP_BSP_UART1_RX_INIT()                      do{ GP_WB_WRITE_IOB_GPIO_8_ALTERNATE(GP_WB_ENUM_GPIO_8_ALTERNATES_UART_1_RX); GP_WB_WRITE_IOB_GPIO_8_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_UART1_RX_DEINIT()                    GP_WB_WRITE_IOB_GPIO_8_ALTERNATE_ENABLE(0);
#define GP_BSP_UART1_RX_DEFINED()                   (1)
#define GP_BSP_UART1_RX_GPIO_CFG()                  do{ \
    GP_WB_WRITE_IOB_GPIO_8_PULL_MODE(1); \
    GP_WB_WRITE_IOB_GPIO_8_PULL_ENABLE(1); \
}while(0)
#define GP_BSP_UART1_RX_ENABLE(en)                  do{ if (en) { GP_WB_WRITE_IOB_GPIO_8_ALTERNATE(GP_WB_ENUM_GPIO_8_ALTERNATES_UART_1_RX); } GP_WB_WRITE_IOB_GPIO_8_ALTERNATE_ENABLE((en)); }while(0)
#define GP_BSP_UART1_RX_ENABLED()                   GP_WB_READ_IOB_GPIO_8_ALTERNATE_ENABLE()

// Pin 26 - GPIO 9 - UART1_TX
#define GP_BSP_UART1_TX_GPIO                        9
#define GP_BSP_UART1_TX_ALTERNATE                   GP_WB_ENUM_GPIO_9_ALTERNATES_UART_1_TX
#define GP_BSP_UART1_TX_INIT()                      do{ GP_WB_WRITE_IOB_GPIO_9_ALTERNATE(GP_WB_ENUM_GPIO_9_ALTERNATES_UART_1_TX); GP_WB_WRITE_IOB_GPIO_9_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_UART1_TX_DEINIT()                    GP_WB_WRITE_IOB_GPIO_9_ALTERNATE_ENABLE(0);
#define GP_BSP_UART1_TX_DEFINED()                   (1)
#define GP_BSP_UART1_TX_GPIO_CFG()                  do{ \
    GP_WB_WRITE_IOB_GPIO_9_PULL_MODE(0); \
    GP_WB_WRITE_IOB_GPIO_9_PULL_ENABLE(0); \
}while(0)
#define GP_BSP_UART1_TX_ENABLE(en)                  do{ if (en) { GP_WB_WRITE_IOB_GPIO_9_ALTERNATE(GP_WB_ENUM_GPIO_9_ALTERNATES_UART_1_TX); } GP_WB_WRITE_IOB_GPIO_9_ALTERNATE_ENABLE((en)); }while(0)
#define GP_BSP_UART1_TX_ENABLED()                   GP_WB_READ_IOB_GPIO_9_ALTERNATE_ENABLE()

#define GP_BSP_UART1_CTS_DEFINED()                  (0)
#define GP_BSP_UART1_CTS_GPIO_CFG()                 do { } while(0)
#define GP_BSP_UART1_CTS_ENABLE(en)                 do { } while(0)
#define GP_BSP_UART1_CTS_ENABLED()                  (0)

#define GP_BSP_UART2_RX_DEFINED()                   (0)
#define GP_BSP_UART2_RX_GPIO_CFG()                  do { } while(0)
#define GP_BSP_UART2_RX_ENABLE(en)                  do { } while(0)
#define GP_BSP_UART2_RX_ENABLED()                   (0)

#define GP_BSP_UART2_TX_DEFINED()                   (0)
#define GP_BSP_UART2_TX_GPIO_CFG()                  do { } while(0)
#define GP_BSP_UART2_TX_ENABLE(en)                  do { } while(0)
#define GP_BSP_UART2_TX_ENABLED()                   (0)

#define GP_BSP_UART2_CTS_DEFINED()                  (0)
#define GP_BSP_UART2_CTS_GPIO_CFG()                 do { } while(0)
#define GP_BSP_UART2_CTS_ENABLE(en)                 do { } while(0)
#define GP_BSP_UART2_CTS_ENABLED()                  (0)

#define GP_BSP_UART_TX_GPIO_MAP                     { 0xff, 9, 0xff }
#define GP_BSP_UART_TX_ALTERNATE_MAP                { 0, GP_WB_ENUM_GPIO_9_ALTERNATES_UART_1_TX, 0 }
#define GP_BSP_UART_RX_GPIO_MAP                     { 0xff, 8, 0xff }
#define GP_BSP_UART_RX_ALTERNATE_MAP                { 0, GP_WB_ENUM_GPIO_8_ALTERNATES_UART_1_RX, 0 }
#define GP_BSP_UART_CTS_GPIO_MAP                    { 0xff, 0xff, 0xff }
#define GP_BSP_UART_CTS_ALTERNATE_MAP               { 0, 0, 0 }

#define GP_BSP_UART_COM1                            1

#ifndef GP_BSP_UART_COM_BAUDRATE
#define GP_BSP_UART_COM_BAUDRATE                    115200
#endif

#ifndef GP_BSP_UART_SCOM_BAUDRATE
#define GP_BSP_UART_SCOM_BAUDRATE                   115200
#endif


/*****************************************************************************
 *                    ADC
 *****************************************************************************/


#define GP_BSP_ADC_GPIO_MAP                         { 28, 29, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }

/*****************************************************************************
 *                    GPIO - Unused
 *****************************************************************************/

/* Pull down unused pins */
#define GP_BSP_UNUSED_INIT()                        do{ \
}while(0)

/*****************************************************************************
 *                    IO Pending
 *****************************************************************************/

#define HAL_BSP_IO_ACTIVITY_PENDING()               (false)

/*****************************************************************************
 *                    Generic
 *****************************************************************************/


/* Enable one of the defines below to enable the correct antenna tuning parameters and TX power table*/
/* #define GP_BSP_ANTENNATUNECONFIG_10DBM_DIFFERENTIAL */
 #define GP_BSP_ANTENNATUNECONFIG_10DBM_SINGLE_ENDED
/* #define GP_BSP_ANTENNATUNECONFIG_20DBM_DIFFERENTIAL */
/* #define GP_BSP_ANTENNATUNECONFIG_20DBM_SINGLE_ENDED */

#include "gpBsp_xp4002_antenna_tune_parameters.h"

#define GP_BSP_GENERIC_INIT()                       do{ \
    /*Disable bus keeper/PU/PD on UART1_RX*/ \
    GP_WB_WRITE_IOB_GPIO_8_PULL_MODE(0); \
    GP_WB_WRITE_IOB_GPIO_8_PULL_ENABLE(0); \
    /*Disable bus keeper/PU/PD on UART1_TX*/ \
    GP_WB_WRITE_IOB_GPIO_9_PULL_MODE(0); \
    GP_WB_WRITE_IOB_GPIO_9_PULL_ENABLE(0); \
    GP_BSP_ANTENNATUNECONFIG_INIT(); \
}while(0)

#define GP_BSP_SLEEPMODENOTRETAINEDLIST_INIT()      do{ \
    GP_WB_WRITE_SYSRAM_RETAIN_UART_0(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_UART_2(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_ASP_0(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_ASP_1(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_RTZ_0(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_RTZ_1(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_I2S_0(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_I2S_1(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_I2C_M_0(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_I2C_M_1(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_KEYPAD(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_SPI_M(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_IR(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_LEDS(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_SPI_SL(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_PWMXLS(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_QSPI_M(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_I2C_SL(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_HR_ADC(0); \
}while(0)

#endif //_GPBSP_PTC_QPG6200_QFN32_10DBM_UART8TO9_H_
