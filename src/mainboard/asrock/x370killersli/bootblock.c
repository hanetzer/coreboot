/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/io.h>
#include <arch/mmio.h>
#include <bootblock_common.h>
#include <console/console.h>
#include <device/i2c_simple.h>
#include <device/pnp_ops.h>
#include <device/smbus_host.h>
#include <smp/node.h>

#include <amdblocks/acpimmio.h>
#include <amdblocks/acpimmio_map.h>
#include <amdblocks/lpc.h>
#include <amdblocks/smn.h>

#include <superio/nuvoton/common/nuvoton.h>
#include <superio/nuvoton/nct6779d/nct6779d.h>

#include "gpio.h"

#define GLOBAL_DEV PNP_DEV(0x2e, 0)
#define SERIAL_DEV PNP_DEV(0x2e, NCT6779D_SP1)
#define ACPI_DEV   PNP_DEV(0x2e, NCT6779D_ACPI)

void bootblock_mainboard_early_init(void)
{
	mainboard_program_early_gpios();
	lpc_enable_sio_decode(LPC_SELECT_SIO_2E2F);
	/* lpc_set_wideio_range(0x2e & ~0xf, 16); */
	lpc_enable_decode(DECODE_ENABLE_SERIAL_PORT0 << CONFIG_UART_FOR_CONSOLE);

	/* Fan Control */
	nuvoton_pnp_enter_conf_state(GLOBAL_DEV);
	pnp_write_config(GLOBAL_DEV, 0x1a, 0x38);
	pnp_write_config(GLOBAL_DEV, 0x1b, 0x26);

	/* Power RAM in S3 */
	pnp_set_logical_device(ACPI_DEV);
	pnp_write_config(ACPI_DEV, 0xe4, 0x10);

	nuvoton_pnp_exit_conf_state(GLOBAL_DEV);

	nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}

void bootblock_mainboard_init(void)
{
	uint8_t nct3933_15_dat1[] = {0x01, 0x00};
	uint8_t nct3933_15_dat2[] = {0x02, 0x00};
	uint8_t nct3933_15_dat3[] = {0x03, 0x05};
	uint8_t nct3933_13_dat1[] = {0x03, 0x8a};
	uint8_t nct3933_13_dat2[] = {0x02, 0x03};
	uint8_t nct3933_13_dat3[] = {0x01, 0x80};
	uint8_t rgb_dat1[] = {0x52, 0x02, 0x00, 0x00};
	uint8_t rgb_dat2[] = {0x31, 0x01, 0x00};
	int ret;

	ret = i2c_write_raw(3, 0x15, nct3933_15_dat1, sizeof(nct3933_15_dat1));
	printk(BIOS_INFO, "i2c_write_raw nct3933_15_dat1 ret=%d\n", ret);
	ret = i2c_write_raw(3, 0x15, nct3933_15_dat2, sizeof(nct3933_15_dat2));
	printk(BIOS_INFO, "i2c_write_raw nct3933_15_dat2 ret=%d\n", ret);
	ret = i2c_write_raw(3, 0x15, nct3933_15_dat3, sizeof(nct3933_15_dat3));
	printk(BIOS_INFO, "i2c_write_raw nct3933_15_dat3 ret=%d\n", ret);

	ret = i2c_write_raw(3, 0x13, nct3933_13_dat1, sizeof(nct3933_13_dat1));
	printk(BIOS_INFO, "i2c_write_raw nct3933_13_dat1 ret=%d\n", ret);
	ret = i2c_write_raw(3, 0x13, nct3933_13_dat2, sizeof(nct3933_13_dat2));
	printk(BIOS_INFO, "i2c_write_raw nct3933_13_dat2 ret=%d\n", ret);
	ret = i2c_write_raw(3, 0x13, nct3933_13_dat3, sizeof(nct3933_13_dat3));
	printk(BIOS_INFO, "i2c_write_raw nct3933_13_dat3 ret=%d\n", ret);

	/* Prevent the RGB Controller from turning the board off */
	ret = i2c_write_raw(3, 0x6a, rgb_dat1, sizeof(rgb_dat1));
	printk(BIOS_INFO, "i2c_write_raw rgb_dat1 ret=%d\n", ret);
	ret = i2c_write_raw(3, 0x6a, rgb_dat2, sizeof(rgb_dat2));
	printk(BIOS_INFO, "i2c_write_raw rgb_dat2 ret=%d\n", ret);

	/* ret = smn_read32(0x10134a00); */
	printk(BIOS_INFO, "disabling ACP 1022:15e2\n");
	smn_write32(0x10134a00, 0x000015e2);
	printk(BIOS_INFO, "disabling Sensor Fusion Hub 1022:15e4\n");
	smn_write32(0x10134e00, 0x000015e4);
}
