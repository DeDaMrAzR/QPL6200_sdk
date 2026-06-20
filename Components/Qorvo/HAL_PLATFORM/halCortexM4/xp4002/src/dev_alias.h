#ifndef _DEV_ALIAS_H_
#define _DEV_ALIAS_H_

#if defined(__GNUC__)
#define WEAK_ALIAS(aliasSymbol) __attribute__((weak, alias(aliasSymbol)))
#elif defined(__IAR_SYSTEMS_ICC__)
#define WEAK_ALIAS(aliasSymbol)
#endif

INTERRUPT_H void akuma_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void asp_0_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void asp_1_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void bbpll_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void bist_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void dcdc_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void debug_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void dma_dcb_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void dma_scb_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void es_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void gp_adc_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void gpio_0_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void gpio_1_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void hr_adc_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void i2c_m_0_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void i2c_m_1_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void i2c_sl_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void i2s_0_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void i2s_1_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void ipc_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void ir_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void keypad_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void pmud_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void pwm_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void pwmxl_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void qspi_m_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void rtz_0_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void rtz_1_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void secure_element_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void spi_m_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void spi_sl_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void ssp_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void standby_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timer_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timerxl_0_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timerxl_1_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timerxl_2_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timerxl_3_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timerxl_4_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void timerxl_5_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void uart_0_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void uart_1_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void uart_2_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void usb_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void watchdog_handler_impl(void) WEAK_ALIAS("unexpected_int");

INTERRUPT_H void pendsv_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void systick_handler_impl(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void nmi_handler(void) WEAK_ALIAS("unexpected_int");
INTERRUPT_H void forced_wakeup_handler(void) WEAK_ALIAS("unexpected_int");
#endif // _DEV_ALIAS_H
