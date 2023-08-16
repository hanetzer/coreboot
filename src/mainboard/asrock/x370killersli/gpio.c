/* SPDX-License-Identifier: GPL-2.0-only */

#include <gpio.h>
#include "gpio.h"

static const struct soc_amd_gpio gpio_set_stage_ram[] = {
	/* PWR_BTN_L */
	PAD_NF(GPIO_0, PWR_BTN_L, PULL_NONE),
	/* SYS_RESET_L */
	PAD_NF(GPIO_1, SYS_RESET_L, PULL_NONE),
	/* I2C3_SCL */
	PAD_NF(GPIO_19, I2C3_SCL, PULL_NONE),
	/* I2C3_SDA */
	PAD_NF(GPIO_20, I2C3_SDA, PULL_NONE),
	/* LPC */
	PAD_NF(GPIO_21, LPC_PD_L, PULL_NONE),
	PAD_NF(GPIO_22, LPC_PME_L, PULL_NONE),
	PAD_NF(GPIO_32, LPC_RST_L, PULL_DOWN),
	PAD_NF(GPIO_74, LPCCLK0, PULL_NONE),
	PAD_NF(GPIO_75, LPCCLK1, PULL_NONE),
	PAD_NF(GPIO_86, LPC_SMI_L, PULL_NONE),
	PAD_NF(GPIO_88, LPC_CLKRUN_L, PULL_NONE),
	PAD_NF(GPIO_104, LAD0, PULL_NONE),
	PAD_NF(GPIO_105, LAD1, PULL_NONE),
	PAD_NF(GPIO_106, LAD2, PULL_NONE),
	PAD_NF(GPIO_107, LAD3, PULL_NONE),
};

void mainboard_program_gpios(void)
{
	gpio_configure_pads(gpio_set_stage_ram, ARRAY_SIZE(gpio_set_stage_ram));
}
