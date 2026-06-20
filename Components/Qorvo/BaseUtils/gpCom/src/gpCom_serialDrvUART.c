/*
 * Copyright (c) 2017-2024, Qorvo Inc
 *
 * gpCom.c
 *
 * This file contains the implementation of the serial communication module.
 * It implements the GreenPeak serial protocol.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_COM

#include "qDrvTimerXL.h"
#include "qDrvUART.h"

#include "gpCom.h"
#include "gpCom_defs.h"

#include "gpLog.h"

#ifdef GP_DIVERSITY_FREERTOS
#include "FreeRTOS.h"
#include "stream_buffer.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#if !defined(GP_COM_ZERO_COPY_BLOCK_TRANSFERS)
#error "GP_COM_ZERO_COPY_BLOCK_TRANSFERS is required for gpCom_serialDrvUART."
#endif

#if defined(GP_DIVERSITY_FREERTOS)

#if !defined(GP_COM_DIVERSITY_NO_RX)
#define UART_TASK_NAME     "UART task"
#define UART_TASK_PRIORITY (configMAX_PRIORITIES - 2)
#define UART_STACK_SIZE    300
#endif // !defined(GP_COM_DIVERSITY_NO_RX)

#define GP_COM_UART1_TASK_NOTIFY_RX_MASK 0x01
#define GP_COM_UART2_TASK_NOTIFY_RX_MASK 0x02

#define GP_COM_UART_TASK_NOTIFY_ALL_MASK (GP_COM_UART1_TASK_NOTIFY_RX_MASK | GP_COM_UART2_TASK_NOTIFY_RX_MASK)

#endif // defined(GP_DIVERSITY_FREERTOS)


#define COM_TX_GPIO_PIN(COMPORT) Q_CONCAT(GP_BSP_UART, COMPORT, _TX_GPIO)
#define COM_RX_GPIO_PIN(COMPORT) Q_CONCAT(GP_BSP_UART, COMPORT, _RX_GPIO)

#if defined(HAL_UART_DMA_MASK) && (HAL_UART_DMA_MASK > 0)
#define GP_COM_USE_DMA 1
#else
#define GP_COM_USE_DMA 0
#endif

#if !defined(HAL_UART_RX_BUFFER_SIZE)
#define UART_RX_STORAGE_SIZE_BYTES 200
#else
#define UART_RX_STORAGE_SIZE_BYTES HAL_UART_RX_BUFFER_SIZE
#endif // !defined(HAL_UART_RX_BUFFER_SIZE)

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
static void vUartTask(void* pvParameters);
static void gpComUart_RxCbDefer(void* arg, UInt8* pBuffer, UInt16 length);
#endif // defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)

#if !defined(GP_COM_DIVERSITY_NO_RX)
static void gpComUart_RxCb1(void* arg, UInt8* pBuffer, UInt16 length);
#if(GP_COM_NUM_UART == 2)
static void gpComUart_RxCb2(void* arg, UInt8* pBuffer, UInt16 length);
#endif // (GP_COM_NUM_UART == 2)
#endif // !defined(GP_COM_DIVERSITY_NO_RX)
static void gpComUart_TxCb(void* arg, UInt8* pBuffer, UInt16 length);

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    qDrvUART_t uart;
#if GP_COM_USE_DMA
    UInt8 buffer[UART_RX_STORAGE_SIZE_BYTES];
#endif // GP_COM_USE_DMA
    qDrvUart_PinConfig_t pinConfig;
    qDrvUART_Callbacks_t cbs;
    const qDrvUART_CbRx_t cbRx;
    const gpCom_CommunicationId_t commId;
#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
    const UInt8 notifyMask;
    StreamBufferHandle_t streamBuffer;
#if configSUPPORT_STATIC_ALLOCATION
    uint8_t streamBufferStorage[UART_RX_STORAGE_SIZE_BYTES];
    StaticStreamBuffer_t streamBufferStruct;
#endif // configSUPPORT_STATIC_ALLOCATION
#endif // defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
} ComUart_Data_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static ComUart_Data_t comUartData[] = {
    {
        .uart = Q_DRV_UART_INSTANCE_DEFINE(GP_BSP_UART_COM1),
        .pinConfig = Q_DRV_UART_PIN_CONFIG(GP_BSP_UART_COM1, COM_TX_GPIO_PIN(GP_BSP_UART_COM1),
                                           COM_RX_GPIO_PIN(GP_BSP_UART_COM1)),
        .cbs = {.tx = NULL, .rx = NULL, .error = NULL},
#if !defined(GP_COM_DIVERSITY_NO_RX)
        .cbRx = gpComUart_RxCb1,
#endif
        .commId = GP_COM_COMM_ID_UART1,
#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
        .notifyMask = GP_COM_UART1_TASK_NOTIFY_RX_MASK,
#endif
    },
#if(GP_COM_NUM_UART >= 2) 
    {
        .uart = Q_DRV_UART_INSTANCE_DEFINE(GP_BSP_UART_COM2),
        .pinConfig = Q_DRV_UART_PIN_CONFIG(GP_BSP_UART_COM2, COM_TX_GPIO_PIN(GP_BSP_UART_COM2),
                                           COM_RX_GPIO_PIN(GP_BSP_UART_COM2)),
        .cbs = {.tx = NULL, .rx = NULL, .error = NULL},
#if !defined(GP_COM_DIVERSITY_NO_RX)
        .cbRx = gpComUart_RxCb2,
#endif
        .commId = GP_COM_COMM_ID_UART2,
#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
        .notifyMask = GP_COM_UART2_TASK_NOTIFY_RX_MASK,
#endif
    },
#endif
};

// Number of available UARTs.
#define COM_UART_DATA_COUNT (Q_ARRAY_SIZE(comUartData))

#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX) 
/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
static StackType_t xUartStack[UART_STACK_SIZE];

/* Structure that will hold the TCB of the task being created. */
static StaticTask_t xUartTaskBuffer;

static TaskHandle_t xUartTaskh = NULL;
#endif // defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)

#if !defined(GP_COM_DIVERSITY_NO_RX) && GP_COM_USE_DMA
static qDrvTimerXL_t rxFlushTimer;
#endif

/*****************************************************************************
 *                    Static Function
 *****************************************************************************/

static void gpComUart_TxCb(void* arg, UInt8* pBuffer, UInt16 length)
{
    ComUart_Data_t* cud = (ComUart_Data_t*)arg;

    Com_AdvanceDataProcessedPointer(cud->commId, length);
    Com_AdvanceDataReadPointer(cud->commId, length);

    UInt8 comUart = (cud->commId == GP_COM_COMM_ID_UART2 ? 1 : 0);
    ComUart_TriggerTx(comUart);
}

#if !defined(GP_COM_DIVERSITY_NO_RX)
static void gpComUart_RxCb1(void* arg, UInt8* pBuffer, UInt16 length)
{
    gpCom_ProtocolState_t* const state = &gpComUart_RxState[0];
    state->commId = GP_COM_COMM_ID_UART1;
#ifdef GP_COM_DIVERSITY_SERIAL_NO_SYN_NO_CRC
    ComNoSynNoCrcProtocol_ParseBuffer(pBuffer, length, state);
#elif defined(GP_COM_DIVERSITY_BLE_PROTOCOL)
    for(UInt16 i = 0; i < length; i++)
    {
        Com_ParseProtocol(pBuffer[i], GP_COM_COMM_ID_UART1);
    }
#else
    ComSynProtocol_ParseBuffer(pBuffer, length, state);
#endif // GP_COM_DIVERSITY_SERIAL_NO_SYN_NO_CRC
}

#if(GP_COM_NUM_UART == 2)
static void gpComUart_RxCb2(void* arg, UInt8* pBuffer, UInt16 length)
{
    gpCom_ProtocolState_t* const state = &gpComUart_RxState[1];
    state->commId = GP_COM_COMM_ID_UART2;
#ifdef GP_COM_DIVERSITY_SERIAL_NO_SYN_NO_CRC
    ComNoSynNoCrcProtocol_ParseBuffer(pBuffer, length, state);
#elif defined(GP_COM_DIVERSITY_BLE_PROTOCOL)
    for(UInt16 i = 0; i < length; i++)
    {
        Com_ParseProtocol(pBuffer[i], GP_COM_COMM_ID_UART2);
    }
#else
    ComSynProtocol_ParseBuffer(pBuffer, length, state);
#endif // GP_COM_DIVERSITY_SERIAL_NO_SYN_NO_CRC
}
#endif // (GP_COM_NUM_UART == 2)

#if GP_COM_USE_DMA
static void gpComUart_rxFlushCb(void* arg, qDrvTimerXL_Event_t event)
{
    for(UInt8 i = 0; i < COM_UART_DATA_COUNT; i++)
    {
        qDrvUART_RxFifoFlush(&comUartData[i].uart);
    }
}
#endif // GP_COM_USE_DMA
#endif // !defined(GP_COM_DIVERSITY_NO_RX)

static void gpComUart_Start(ComUart_Data_t* cud)
{
    qDrvUART_Config_t cfg = {
        .baudrate = GP_BSP_UART_COM_BAUDRATE,
        .bitlength = qRegUART_Bitlength8,
        .parity = qRegUART_ParityNone,
        .stopbits = qRegUART_StopbitsOne,
        .txDma = true, // Always use DMA for TX.
#if !defined(GP_COM_DIVERSITY_NO_RX)
        .rxDma = (GP_COM_USE_DMA),
#else
        .rxDma = false,
#endif
    };

#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
    const size_t xTriggerLevel = 1;
#if configSUPPORT_STATIC_ALLOCATION
    cud->streamBuffer = xStreamBufferCreateStatic(UART_RX_STORAGE_SIZE_BYTES, xTriggerLevel, cud->streamBufferStorage,
                                                  &cud->streamBufferStruct);
#else

    cud->streamBuffer = xStreamBufferCreate(UART_RX_STORAGE_SIZE_BYTES, xTriggerLevel);
#endif
    Q_ASSERT(cud->streamBuffer);
#endif

    qResult_t res = qDrvUART_PinConfigSet(&cud->pinConfig);
    Q_ASSERT(res == Q_OK);

#if defined(GP_COM_DIVERSITY_NO_RX)
    cud->cbs.rx = NULL;
#elif defined(GP_DIVERSITY_FREERTOS)
    cud->cbs.rx = gpComUart_RxCbDefer;
#else
    cud->cbs.rx = cud->cbRx;
#endif

    cud->cbs.tx = gpComUart_TxCb;

    res = qDrvUART_Init(&cud->uart, &cfg, &cud->cbs, cud, Q_DRV_IRQ_PRIO_DEFAULT);
    Q_ASSERT(res == Q_OK);

    res = qDrvUART_IrqEnable(&cud->uart);
    Q_ASSERT(res == Q_OK);

#if !defined(GP_COM_DIVERSITY_NO_RX)
    res = qDrvUART_RxEnable(&cud->uart);
    Q_ASSERT(res == Q_OK);
#if GP_COM_USE_DMA
    // In DMA mode, the RX buffer is used, so threshold is set to 30 bytes.
    res = qDrvUART_RxDMA(&cud->uart, cud->buffer, UART_RX_STORAGE_SIZE_BYTES, 30);
    Q_ASSERT(res == Q_OK);
#else
    // In interrupt mode, the RX buffer is not used, so threshold is set to 0 byte to pass-through each byte.
    res = qDrvUART_RxIrq(&cud->uart, NULL, 0, 0);
    Q_ASSERT(res == Q_OK);
#endif // GP_COM_USE_DMA
#endif // !defined(GP_COM_DIVERSITY_NO_RX)

#if !defined(GP_COM_DIVERSITY_NO_RX) && GP_COM_USE_DMA
    if(qDrvTimerXL_InitCheck(&rxFlushTimer) == false)
    {
        res = qDrvTimerXL_Alloc(&rxFlushTimer, qDrvTimerXL_Mode16Bit);
        Q_ASSERT(res == Q_OK);

        qDrvTimerXL_Config_t config = {
            .countMode = qDrvTimerXL_CountModePrescaled,
            .prescaler = 0,
        };

        res = qDrvTimerXL_Init(&rxFlushTimer, &config, &gpComUart_rxFlushCb, NULL, Q_DRV_IRQ_PRIO_DEFAULT);
        Q_ASSERT(res == Q_OK);

        UInt32 ticks;
        UInt8 prescaler;

        /* Get the ticks count value for 1.25 ms period. */
        res = qDrvTimerXL_UsecToTicksConvert(&rxFlushTimer, 1250, &ticks, &prescaler);
        Q_ASSERT(res == Q_OK);

        res = qDrvTimerXL_CCSet(&rxFlushTimer, 0, ticks);
        Q_ASSERT(res == Q_OK);

        /* Configure the source timer to wrap after 1.25 ms. */
        res = qDrvTimerXL_CCClrOnMatchSet(&rxFlushTimer, 0, true);
        Q_ASSERT(res == Q_OK);

        res = qDrvTimerXL_CCIrqEnable(&rxFlushTimer, 0, true);
        GP_ASSERT_SYSTEM(res == Q_OK);

        res = qDrvTimerXL_Start(&rxFlushTimer);
        Q_ASSERT(res == Q_OK);
    }
#endif // !defined(GP_COM_DIVERSITY_NO_RX)
}

#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)
void vUartTask(void* pvParameters)
{
    (void)pvParameters;

    static uint8_t rxBuf[UART_RX_STORAGE_SIZE_BYTES];

    uint32_t notificationVal;

    for(;;)
    {
        if(xTaskNotifyWait(0, GP_COM_UART_TASK_NOTIFY_ALL_MASK, &notificationVal, portMAX_DELAY) == pdTRUE)
        {
            for(UInt8 i = 0; i < COM_UART_DATA_COUNT; i++)
            {
                ComUart_Data_t* cud = &comUartData[i];

                if(notificationVal & cud->notifyMask)
                {
                    size_t xReceivedBytes = xStreamBufferReceive(cud->streamBuffer, (void*)rxBuf, sizeof(rxBuf), 0);

                    if(xReceivedBytes > 0)
                    {
                        GP_LOG_PRINTF("Uart receive: %d bytes", 0, xReceivedBytes);
                        cud->cbRx((void*)cud, rxBuf, xReceivedBytes);
                    }
                }
            }
        }
    }
}

static void gpComUart_RxCbDefer(void* arg, UInt8* pBuffer, UInt16 length)
{
    ComUart_Data_t* cud = (ComUart_Data_t*)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; /* Initialised to pdFALSE. */

    /* Attempt to send the string to the stream buffer. */
    size_t xBytesSent = xStreamBufferSendFromISR(cud->streamBuffer, (void*)pBuffer, length, &xHigherPriorityTaskWoken);

    if(xBytesSent != (size_t)length)
    {
        // uart rx buffer too small
        GP_ASSERT_SYSTEM(false);
    }
    else
    {
        if(xTaskNotifyFromISR(xUartTaskh, cud->notifyMask, eSetBits, &xHigherPriorityTaskWoken) == pdTRUE)
        {
            GP_LOG_PRINTF("Uart RX receive: %d bytes", 0, length);
        }
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif // defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


void gpComUart_Init(void)
{
    // Initialize the UART (serial port)
#if defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)

#if configSUPPORT_STATIC_ALLOCATION
    xUartTaskh = xTaskCreateStatic(vUartTask,          /* Function that implements the task. */
                                   UART_TASK_NAME,     /* Text name for the task. */
                                   UART_STACK_SIZE,    /* Number of indexes in the xStack array. */
                                   NULL,               /* Parameter passed into the task. */
                                   UART_TASK_PRIORITY, /* Priority at which the task is created. */
                                   xUartStack,         /* Array to use as the task's stack. */
                                   &xUartTaskBuffer);  /* Variable to hold the task's data structure. */
    Q_ASSERT(xUartTaskh);
#else                                                  // configSUPPORT_STATIC_ALLOCATION
    (void)xTaskCreate(vUartTask, UART_TASK_NAME, UART_STACK_SIZE, NULL, UART_TASK_PRIORITY, &xUartTaskh);
#endif                                                 // configSUPPORT_STATIC_ALLOCATION
#endif // defined(GP_DIVERSITY_FREERTOS) && !defined(GP_COM_DIVERSITY_NO_RX)

    for(UInt8 i = 0; i < COM_UART_DATA_COUNT; i++)
    {
        gpComUart_Start(&comUartData[i]);
    }
}

void gpComUart_DeInit(void)
{
    qResult_t res;
    for(UInt8 i = 0; i < COM_UART_DATA_COUNT; i++)
    {
        res = qDrvUART_Deinit(&comUartData[i].uart);
        Q_ASSERT(res == Q_OK);
    }

#if !defined(GP_COM_DIVERSITY_NO_RX) && GP_COM_USE_DMA
    res = qDrvTimerXL_Free(&rxFlushTimer);
    Q_ASSERT(res == Q_OK);
#endif
}
qResult_t qDrvUART_TxFlushPolling(qDrvUART_t* const pInstance);

qResult_t qDrvUART_TxFlush(qDrvUART_t* const pInstance);

void gpComUart_Flush(void)
{
    for(UInt8 i = 0; i < COM_UART_DATA_COUNT; i++)
    {
        ComUart_Data_t* cud = &comUartData[i];
        while(qDrvUART_TxFifoEmptyCheck(&cud->uart) == false)
        {
            ;
        }
    }
}

void ComUart_FlushRx(void)
{
    // Nothing to do here.
    // There is an automatic flush of the RX buffer with the timeout mechanism.
}

Bool ComUart_TxEnabled(UInt8 comPort)
{
    if(comPort >= COM_UART_DATA_COUNT)
    {
        return false;
    }

    // Transmitter is always enabled.
    return true;
}

void ComUart_TriggerTx(UInt8 comPort)
{
    if(comPort >= COM_UART_DATA_COUNT)
    {
        return;
    }

    ComUart_Data_t* cud = &comUartData[comPort];

    if(!qDrvUART_TxBusyCheck(&cud->uart))
    {
        if(Com_IsDataWaiting(cud->commId))
        {
            UInt16 sizePending;
            Com_CalculateSizeOfNewData(cud->commId, &sizePending);
            if(sizePending == 0)
            {
                return;
            }
            qDrvUART_TxDMA(&cud->uart, Com_GetDataReadPointer(cud->commId), sizePending);
        }
    }
}

