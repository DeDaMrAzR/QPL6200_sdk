/*
 * Copyright (c) 2024-2025, Qorvo Inc
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

/** @file "qPinCfg.h"
 *
 *  Declarations of all GPIOs on IoT Carrier Board
 */

#ifndef _QPINCFG_IOTCARRIERBOARD_QPG6200_H_
#define _QPINCFG_IOTCARRIERBOARD_QPG6200_H_

/* Buttons. */
#define SW_BUTTON_GPIO_PIN  4
#define PB1_BUTTON_GPIO_PIN 3
#define PB2_BUTTON_GPIO_PIN 5
#define PB3_BUTTON_GPIO_PIN 29
#define PB4_BUTTON_GPIO_PIN 21
#define PB5_BUTTON_GPIO_PIN 0

/* LEDs. */
#define WHITE_COOL_LED_GPIO_PIN 1
#define WHITE_WARM_LED_GPIO_PIN 2
#define RED_LED_GPIO_PIN        10
#define GREEN_LED_GPIO_PIN      11
#define BLUE_LED_GPIO_PIN       12

/* I2C */
#define I2C_SCL_GPIO_PIN 21
#define I2C_SDA_GPIO_PIN 0

/* SPI Master */
#define MSPI_MISO_GPIO_PIN 1
#define MSPI_SCLK_GPIO_PIN 11
#define MSPI_MOSI_GPIO_PIN 12

/* 8Mbit SPI Flash*/
#define FLASH_CSN_GPIO_PIN 10

/* UART pins. Used for logging by gpCom. */
#define UART1_RX_GPIO_PIN 8
#define UART1_TX_GPIO_PIN 9

/* ADC pins. */
#define ANIO0_GPIO_PIN 28
#define ANIO1_GPIO_PIN 29

/* Debug SWJDP pins */
#define DEBUG_SWJDP_SWDIO_TMS_GPIO 6
#define DEBUG_SWJDP_SWCLK_TCK_GPIO 7

/* EXT 32kHz crystal pins */
#define EXT_32KXTAL_P 25
#define EXT_32KXTAL_N 26

/* SoC specific pins */
#define SOC_10DBM_SPECIFIC 18
#define SOC_40PIN_SPECIFIC 13, 14, 15, 16, 17, 22, 23, 24, 27

// Pins unused by board will be pulled low
// Only the available pins defined by variant will be pulled low by qPinCfg
// Unavailable pins are pulled low internally and will be skipped in qPinCfg
#define BOARD_UNUSED_GPIO_PINS 19, 20, SOC_10DBM_SPECIFIC

#endif // _QPINCFG_IOTCARRIERBOARD_QPG6200_H_
