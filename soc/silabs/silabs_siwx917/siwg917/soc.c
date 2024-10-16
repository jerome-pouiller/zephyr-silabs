/*
 * Copyright (c) 2023 Antmicro
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sw_isr_table.h>
#include "rsi_rom_clks.h"
#include "rsi_rom_ulpss_clk.h"

int silabs_siwx917_init(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	/* FIXME: do not hardcode UART instances */
#if DT_NODE_HAS_STATUS(DT_NODELABEL(ulpuart0), okay)
	RSI_ULPSS_UlpUartClkConfig(ULPCLK, ENABLE_STATIC_CLK, 0, ULP_UART_ULP_32MHZ_RC_CLK, 1);
#endif
#if DT_NODE_HAS_STATUS(DT_NODELABEL(uart0), okay)
	RSI_CLK_UsartClkConfig(M4CLK, ENABLE_STATIC_CLK, 0, USART1, 0, 1);
#endif

	return 0;
}
SYS_INIT(silabs_siwx917_init, PRE_KERNEL_1, 0);

// Co-processor will use value stored in IVT to store its stack. See also
// linker.ld.
// FIXME: We can't use Z_ISR_DECLARE() to declare this entry
// FIXME: Allow to configure size of buffer
uint8_t __attribute__((aligned(4))) siwx917_coprocessor_stack[10 * 1024];
static Z_DECL_ALIGN(struct _isr_list) Z_GENERIC_SECTION(.intList) __used __isr_siwx917_coprocessor_stack_irq = {
	30, ISR_FLAG_DIRECT, siwx917_coprocessor_stack + sizeof(siwx917_coprocessor_stack), 0
};

/* SiWx917's bootloader requires IRQn 32 to hold payload's entry point address. */
extern void z_arm_reset(void);
Z_ISR_DECLARE(32, ISR_FLAG_DIRECT, z_arm_reset, 0);
