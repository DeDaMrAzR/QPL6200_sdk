/*
 * Copyright (c) 2022, Qorvo Inc
 *
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

    .syntax unified
    .thumb

    .weak hardfault_handler, memmanage_handler, busfault_handler, usagefault_handler, svcall_handler, debugmon_handler, pendsv_handler, systick_handler

    .align 1
    .text

    .section .text.unexpected_fault, "ax", %progbits
    .thumb_func
    .type   unexpected_fault, %function
hardfault_handler:
memmanage_handler:
busfault_handler:
usagefault_handler:
svcall_handler:
debugmon_handler:
pendsv_handler:
systick_handler:
unexpected_fault:
    mov         r0, sp
    push        {r4-r11}
    mov         r1, sp
    b           unexpected_fault_validsp
