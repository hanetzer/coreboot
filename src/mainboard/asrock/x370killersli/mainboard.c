/* SPDX-License-Identifier: GPL-2.0-only */

#include <commonlib/helpers.h>
#include <console/console.h>
#include <device/device.h>
#include <types.h>

#include <FspsUpd.h>
#include <amdblocks/amd_pci_util.h>
#include <soc/acpi.h>
#include <soc/amd/picasso/chip.h>
#include <soc/cpu.h>
#include <soc/pci_devs.h>
#include <soc/southbridge.h>

#include "gpio.h"

/* TODO: recheck IRQ tables */

/* The IRQ mapping in fch_irq_map ends up getting written to the indirect address space that is
   accessed via I/O ports 0xc00/0xc01. */
static const struct fch_irq_routing fch_irq_map[] = {
	/* intr_index   pic_irq_num     apic_irq_num */
	{ PIRQ_A,	0x06,		0x10 },
	{ PIRQ_B,	0x0d,		0x11 },
	{ PIRQ_C,	0x0e,		0x12 },
	{ PIRQ_D,	0x0f,		0x13 },
	{ PIRQ_E,	0x0f,		0x14 },
	{ PIRQ_F,	0x0e,		0x15 },
	{ PIRQ_G,	0x0d,		0x16 },
	{ PIRQ_H,	0x06,		0x17 },

	{ PIRQ_SCI,	ACPI_SCI_IRQ,	ACPI_SCI_IRQ },
	{ PIRQ_GPIO,	0x07,		0x07 },
	{ PIRQ_I2C2,	0x06,		0x06 },
	{ PIRQ_I2C3,	0x0e,		0x0e },
	{ PIRQ_UART0,	0x04,		0x04 },
	{ PIRQ_UART1,	0x03,		0x03 },

	/* The MISC registers are not interrupt numbers */
	{ PIRQ_MISC,	0xfa,		0x00 },
	{ PIRQ_MISC0,	0x91,		0x00 },
	{ PIRQ_MISC1,	0x00,		0x00 },
	{ PIRQ_MISC2,	0x00,		0x00 },
};

const struct fch_irq_routing *mb_get_fch_irq_mapping(size_t *length)
{
	*length = ARRAY_SIZE(fch_irq_map);
	return fch_irq_map;
}

static void mainboard_init(void *chip_info)
{
	mainboard_program_gpios();
}

struct chip_operations mainboard_ops = {
	.init = mainboard_init,
};
