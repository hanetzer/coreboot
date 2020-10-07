/* SPDX-License-Identifier: GPL-2.0-only */

#include <boot_device.h>
#include <stdint.h>

/*
 * 0x60300f4000000
 * 0x60300fc000000
 */
/* This assumes that the CBFS resides at 0x0, which is true for the default
 * configuration. */
/* #define rom_base ((void *) (uintptr_t)(0x6030000000000ULL-CONFIG_ROM_SIZE)) */
#define rom_base ((void *) (uintptr_t)(0x60300fc000000ULL))
static const struct mem_region_device boot_dev =
	MEM_REGION_DEV_RO_INIT(rom_base, CONFIG_ROM_SIZE);

const struct region_device *boot_device_ro(void)
{
	return &boot_dev.rdev;
}
