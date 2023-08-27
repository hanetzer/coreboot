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
};

void mainboard_program_gpios(void)
{
	gpio_configure_pads(gpio_set_stage_ram, ARRAY_SIZE(gpio_set_stage_ram));
}
