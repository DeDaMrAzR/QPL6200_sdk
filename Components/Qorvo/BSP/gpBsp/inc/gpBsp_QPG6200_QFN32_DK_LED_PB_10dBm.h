
/*
#   Copyright (c) 2011-2016, GreenPeak Technologies
#   Copyright (c) 2017-2021, Qorvo Inc
 *
 *
 *   QPG6200 DK board
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


#ifndef _GPBSP_QPG6200_QFN32_DK_LED_PB_10DBM_H_
#define _GPBSP_QPG6200_QFN32_DK_LED_PB_10DBM_H_

/*****************************************************************************
 *                    BSP configuration interface
 *****************************************************************************/

#define GP_BSP_INTERNAL_UC
// Has the board an differential antenna?
#define GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA() 0
// Settling time for tx monitor in symbols
#define GP_BSP_TXMONITOR_SETTLING_TIME_IN_SYMBOLS() 0

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


/* White (Cool) LED - LD6 - low duty cycle for lower intensity */
#define GP_BSP_LED_WHITE_COOL_PIN 1
#define GP_BSP_LED_WHITE_COOL_LOGIC_LEVEL 1
#define GP_BSP_LED_WHITE_COOL_LED_ID 1
// HAL helpers
#define WHITE_COOL 1 // GPIO1 - LED Active when high
#define HAL_LED_WHITE_COOL_MAX_OUTPUT  0
#define HAL_LED_SET_WHITE_COOL() do{ GP_WB_WRITE_LEDS_LED1_ENABLE(1); }while(false)
#define HAL_LED_CLR_WHITE_COOL() do{ GP_WB_WRITE_LEDS_LED1_ENABLE(0); }while(false)
#define HAL_LED_TST_WHITE_COOL() GP_WB_READ_LEDS_LED1_ENABLE()
#define HAL_LED_SET_WHITE_COOL_THRESHOLD(x) GP_WB_WRITE_LEDS_LED1_THRESHOLD(x)
#define HAL_LED_TGL_WHITE_COOL() do{ if (HAL_LED_TST_WHITE_COOL()) { HAL_LED_CLR_WHITE_COOL(); } else { HAL_LED_SET_WHITE_COOL(); }; }while(false)

/* White (Warm) LED - LD5 - low duty cycle for lower intensity */
#define GP_BSP_LED_WHITE_WARM_PIN 2
#define GP_BSP_LED_WHITE_WARM_LOGIC_LEVEL 1
#define GP_BSP_LED_WHITE_WARM_LED_ID 2
// HAL helpers
#define WHITE_WARM 2 // GPIO2 - LED Active when high
#define HAL_LED_WHITE_WARM_MAX_OUTPUT  0
#define HAL_LED_SET_WHITE_WARM() do{ GP_WB_WRITE_LEDS_LED2_ENABLE(1); }while(false)
#define HAL_LED_CLR_WHITE_WARM() do{ GP_WB_WRITE_LEDS_LED2_ENABLE(0); }while(false)
#define HAL_LED_TST_WHITE_WARM() GP_WB_READ_LEDS_LED2_ENABLE()
#define HAL_LED_SET_WHITE_WARM_THRESHOLD(x) GP_WB_WRITE_LEDS_LED2_THRESHOLD(x)
#define HAL_LED_TGL_WHITE_WARM() do{ if (HAL_LED_TST_WHITE_WARM()) { HAL_LED_CLR_WHITE_WARM(); } else { HAL_LED_SET_WHITE_WARM(); }; }while(false)

/* PWM XL Red / RED led - LD1 - GPIO block driven / SSn pin for Master SPI */
#define GP_BSP_LED_RED_PIN 10
#define GP_BSP_LED_RED_LOGIC_LEVEL 1
// HAL helpers
#define RED 10 // GPIO10 - LED Active when high
#define HAL_LED_SET_RED() GP_WB_WRITE_GPIO_GPIO_10_OUTPUT_VALUE(1)
#define HAL_LED_CLR_RED() GP_WB_WRITE_GPIO_GPIO_10_OUTPUT_VALUE(0)
#define HAL_LED_TST_RED() GP_WB_READ_GPIO_GPIO_10_OUTPUT_VALUE()
#define HAL_LED_TGL_RED() do{ if (HAL_LED_TST_RED()) { HAL_LED_CLR_RED(); } else { HAL_LED_SET_RED(); }; }while(false)

/* PWM XL Green / GRN led - LD2- GPIO block driven / SPI_M_SCLK */
#define GP_BSP_LED_GRN_PIN 11
#define GP_BSP_LED_GRN_LOGIC_LEVEL 1
// HAL helpers
#define GRN 11 // GPIO11 - LED Active when high
#define HAL_LED_SET_GRN() GP_WB_WRITE_GPIO_GPIO_11_OUTPUT_VALUE(1)
#define HAL_LED_CLR_GRN() GP_WB_WRITE_GPIO_GPIO_11_OUTPUT_VALUE(0)
#define HAL_LED_TST_GRN() GP_WB_READ_GPIO_GPIO_11_OUTPUT_VALUE()
#define HAL_LED_TGL_GRN() do{ if (HAL_LED_TST_GRN()) { HAL_LED_CLR_GRN(); } else { HAL_LED_SET_GRN(); }; }while(false)

#define HAL_LED_INIT_LEDS()                         do{ \
    /*Initialize LED driver block 1 for GPIO 1*/ \
    GP_WB_WRITE_LEDS_LED1_THRESHOLD(HAL_LED_WHITE_COOL_MAX_OUTPUT); \
    GP_WB_WRITE_IOB_GPIO_1_ALTERNATE(GP_WB_ENUM_GPIO_1_ALTERNATES_LED_LED_1); \
    GP_WB_WRITE_IOB_GPIO_1_ALTERNATE_ENABLE(1); \
    HAL_LED_CLR_WHITE_COOL(); \
    /*Initialize LED driver block 2 for GPIO 2*/ \
    GP_WB_WRITE_LEDS_LED2_THRESHOLD(HAL_LED_WHITE_WARM_MAX_OUTPUT); \
    GP_WB_WRITE_IOB_GPIO_2_ALTERNATE(GP_WB_ENUM_GPIO_2_ALTERNATES_LED_LED_2); \
    GP_WB_WRITE_IOB_GPIO_2_ALTERNATE_ENABLE(1); \
    HAL_LED_CLR_WHITE_WARM(); \
    /*Put pin to output*/ \
    GP_WB_WRITE_GPIO_GPIO_10_DIRECTION(1); \
    HAL_LED_CLR_RED(); \
    /*Put pin to output*/ \
    GP_WB_WRITE_GPIO_GPIO_11_DIRECTION(1); \
    HAL_LED_CLR_GRN(); \
    /*Initialize timer for LED PWM function*/ \
    GP_WB_WRITE_LEDS_PRESCALER_DIV(7); \
    GP_WB_WRITE_LEDS_SRC_CLK_SPEED(0); \
}while(0)

#define GP_BSP_LED_GPIO_MAP                         { 0xff, 1, 2, 0xff }
#define GP_BSP_LED_ALTERNATE_MAP                    { 0, GP_WB_ENUM_GPIO_1_ALTERNATES_LED_LED_1, GP_WB_ENUM_GPIO_2_ALTERNATES_LED_LED_2, 0 }

/*****************************************************************************
 *                    GPIO - BTN
 *****************************************************************************/

/* Push-button GP PB 5 */
#define GP_BSP_BUTTON_GP_PB5_PIN  0 //GPIO0
#define GP_BSP_BUTTON_GP_PB5_LOGIC_LEVEL 0 //Active low
// HAL helpers
#define GP_PB5  0
#define HAL_BUTTON_GP_PB5_IS_PRESSED()  (!GP_WB_READ_GPIO_GPIO_0_INPUT_VALUE()) /*Button pin low = pressed*/

/* Push-button GP PB 1 */
#define GP_BSP_BUTTON_GP_PB1_PIN  3 //GPIO3
#define GP_BSP_BUTTON_GP_PB1_LOGIC_LEVEL 0 //Active low
// HAL helpers
#define GP_PB1  3
#define HAL_BUTTON_GP_PB1_IS_PRESSED()  (!GP_WB_READ_GPIO_GPIO_3_INPUT_VALUE()) /*Button pin low = pressed*/

/* Slider switch GP SW */
#define GP_BSP_BUTTON_GP_SW_PIN  4 //GPIO4
#define GP_BSP_BUTTON_GP_SW_LOGIC_LEVEL 0 //Active low
// HAL helpers
#define GP_SW  4
#define HAL_BUTTON_GP_SW_IS_PRESSED()  (!GP_WB_READ_GPIO_GPIO_4_INPUT_VALUE()) /*Button pin low = pressed*/

/* Push-button GP PB 4 */
#define GP_BSP_BUTTON_GP_PB4_PIN  21 //GPIO21
#define GP_BSP_BUTTON_GP_PB4_LOGIC_LEVEL 0 //Active low
// HAL helpers
#define GP_PB4  21
#define HAL_BUTTON_GP_PB4_IS_PRESSED()  (!GP_WB_READ_GPIO_GPIO_21_INPUT_VALUE()) /*Button pin low = pressed*/

/* Analog I/O - A1 / Push-button GP PB 3 */
#define GP_BSP_BUTTON_GP_PB3_PIN  29 //GPIO29
#define GP_BSP_BUTTON_GP_PB3_LOGIC_LEVEL 0 //Active low
// HAL helpers
#define GP_PB3  29
#define HAL_BUTTON_GP_PB3_IS_PRESSED()  (!GP_WB_READ_GPIO_GPIO_29_INPUT_VALUE()) /*Button pin low = pressed*/


#define HAL_BTN_INIT_BTNS()                         do{ \
    GP_WB_WRITE_IOB_GPIO_0_PULL_MODE(1); \
    GP_WB_WRITE_IOB_GPIO_0_PULL_ENABLE(1); \
    GP_WB_WRITE_GPIO_GPIO_0_DIRECTION(0); /*Set as input*/ \
    GP_WB_WRITE_IOB_GPIO_3_PULL_MODE(1); \
    GP_WB_WRITE_IOB_GPIO_3_PULL_ENABLE(1); \
    GP_WB_WRITE_GPIO_GPIO_3_DIRECTION(0); /*Set as input*/ \
    GP_WB_WRITE_IOB_GPIO_4_PULL_MODE(1); \
    GP_WB_WRITE_IOB_GPIO_4_PULL_ENABLE(1); \
    GP_WB_WRITE_GPIO_GPIO_4_DIRECTION(0); /*Set as input*/ \
    GP_WB_WRITE_IOB_GPIO_21_PULL_MODE(1); \
    GP_WB_WRITE_IOB_GPIO_21_PULL_ENABLE(1); \
    GP_WB_WRITE_GPIO_GPIO_21_DIRECTION(0); /*Set as input*/ \
    GP_WB_WRITE_IOB_GPIO_29_PULL_MODE(1); \
    GP_WB_WRITE_IOB_GPIO_29_PULL_ENABLE(1); \
    /*GPIO_29 is input only. Setting direction is not needed! */ \
    GP_WB_WRITE_GPIO_MAIN_CLOCK_ENABLE(1); /*Enables GPIO input clock*/ \
    GP_WB_WRITE_GPIO_INTERRUPTS_CLOCK_ENABLE(1); /*Enables GPIO interrupt input clock FIXME: only when needed?*/ \
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
 *                    MSPI
 *****************************************************************************/

#if defined(HAL_DIVERSITY_SPI)
/* White (Cool) LED - LD6 - low duty cycle for lower intensity */
// Pin 17 - GPIO 1 - LED1, MSPI_MISO
#define GP_BSP_MSPI_MISO_GPIO                       1
#define GP_BSP_MSPI_MISO_ALTERNATE                  GP_WB_ENUM_GPIO_1_ALTERNATES_SPI_M_MISO
#define GP_BSP_MSPI_MISO_INIT()                     do{ GP_WB_WRITE_IOB_GPIO_1_ALTERNATE(GP_WB_ENUM_GPIO_1_ALTERNATES_SPI_M_MISO); GP_WB_WRITE_IOB_GPIO_1_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_MSPI_MISO_DEINIT()                   GP_WB_WRITE_IOB_GPIO_1_ALTERNATE_ENABLE(0);
/* PWM XL Green / GRN led - LD2- GPIO block driven / SPI_M_SCLK */
// Pin 28 - GPIO 11 - PWMXL5, GPIO11, MSPI_SCLK
#define GP_BSP_MSPI_SCLK_GPIO                       11
#define GP_BSP_MSPI_SCLK_ALTERNATE                  GP_WB_ENUM_GPIO_11_ALTERNATES_SPI_M_SCLK
#define GP_BSP_MSPI_SCLK_INIT()                     do{ GP_WB_WRITE_IOB_GPIO_11_ALTERNATE(GP_WB_ENUM_GPIO_11_ALTERNATES_SPI_M_SCLK); GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_MSPI_SCLK_DEINIT()                   GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(0);
/* PWM XL Blue / SPI_M_MOSI */
// Pin 29 - GPIO 12 - PWMXL0, MSPI_MOSI
#define GP_BSP_MSPI_MOSI_GPIO                       12
#define GP_BSP_MSPI_MOSI_ALTERNATE                  GP_WB_ENUM_GPIO_12_ALTERNATES_SPI_M_MOSI
#define GP_BSP_MSPI_MOSI_INIT()                     do{ GP_WB_WRITE_IOB_GPIO_12_ALTERNATE(GP_WB_ENUM_GPIO_12_ALTERNATES_SPI_M_MOSI); GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_MSPI_MOSI_DEINIT()                   GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(0);
#endif


/*****************************************************************************
 *                    PWMXL
 *****************************************************************************/

/* PWM XL Red / RED led - LD1 - GPIO block driven / SSn pin for Master SPI */
// Pin 27 - GPIO 10 - PWMXL4, GPIO10
#define GP_BSP_PWMXL4_GPIO                          10
#define GP_BSP_PWMXL4_ALTERNATE                     GP_WB_ENUM_GPIO_10_ALTERNATES_PWMXL_PWMXL_4
#define GP_BSP_PWMXL4_INIT()                        do{ GP_WB_WRITE_IOB_GPIO_10_ALTERNATE(GP_WB_ENUM_GPIO_10_ALTERNATES_PWMXL_PWMXL_4); GP_WB_WRITE_IOB_GPIO_10_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_PWMXL4_DEINIT()                      GP_WB_WRITE_IOB_GPIO_10_ALTERNATE_ENABLE(0);
#define GP_BSP_PWMXL4_DRIVE                         GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL
/* PWM XL Green / GRN led - LD2- GPIO block driven / SPI_M_SCLK */
// Pin 28 - GPIO 11 - PWMXL5, GPIO11, MSPI_SCLK
#define GP_BSP_PWMXL5_GPIO                          11
#define GP_BSP_PWMXL5_ALTERNATE                     GP_WB_ENUM_GPIO_11_ALTERNATES_PWMXL_PWMXL_5
#define GP_BSP_PWMXL5_INIT()                        do{ GP_WB_WRITE_IOB_GPIO_11_ALTERNATE(GP_WB_ENUM_GPIO_11_ALTERNATES_PWMXL_PWMXL_5); GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_PWMXL5_DEINIT()                      GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(0);
#define GP_BSP_PWMXL5_DRIVE                         GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL
/* PWM XL Blue / SPI_M_MOSI */
// Pin 29 - GPIO 12 - PWMXL0, MSPI_MOSI
#define GP_BSP_PWMXL0_GPIO                          12
#define GP_BSP_PWMXL0_ALTERNATE                     GP_WB_ENUM_GPIO_12_ALTERNATES_PWMXL_PWMXL_0
#define GP_BSP_PWMXL0_INIT()                        do{ GP_WB_WRITE_IOB_GPIO_12_ALTERNATE(GP_WB_ENUM_GPIO_12_ALTERNATES_PWMXL_PWMXL_0); GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(1); }while(0)
#define GP_BSP_PWMXL0_DEINIT()                      GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(0);
#define GP_BSP_PWMXL0_DRIVE                         GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL

#define GP_BSP_PWMXL_GPIO_MAP                       { 12, 0xff, 0xff, 0xff, 10, 11, 0xff, 0xff }
#define GP_BSP_PWMXL_ALTERNATE_MAP                  { GP_WB_ENUM_GPIO_12_ALTERNATES_PWMXL_PWMXL_0, 0, 0, 0, GP_WB_ENUM_GPIO_10_ALTERNATES_PWMXL_PWMXL_4, GP_WB_ENUM_GPIO_11_ALTERNATES_PWMXL_PWMXL_5, 0, 0 }
#define GP_BSP_PWMXL_DRIVE_MAP                      { GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL, 0, 0, 0, GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL, GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL, 0, 0 }

/*****************************************************************************
 *                    ADC
 *****************************************************************************/

// Pin 11 - GPIO 28 - ANIO0
#define GP_BSP_ANIO0_GPIO                           28
#define GP_BSP_ANIO0_INIT()                         GP_WB_WRITE_IOB_GPIO_28_ALTERNATE_ENABLE(0)
#define GP_BSP_ANIO0_DEINIT()                       do{ }while(0)
// Pin 12 - GPIO 29 - GPIO29, ANIO1
#define GP_BSP_ANIO1_GPIO                           29
#define GP_BSP_ANIO1_INIT()                         GP_WB_WRITE_IOB_GPIO_29_ALTERNATE_ENABLE(0)
#define GP_BSP_ANIO1_DEINIT()                       do{ }while(0)

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
    GP_WB_WRITE_SYSRAM_RETAIN_IR(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_SPI_SL(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_QSPI_M(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_I2C_SL(0); \
    GP_WB_WRITE_SYSRAM_RETAIN_HR_ADC(0); \
}while(0)

#endif //_GPBSP_QPG6200_QFN32_DK_LED_PB_10DBM_H_
