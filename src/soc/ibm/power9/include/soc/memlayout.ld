/*
 * This file is part of the coreboot project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <memlayout.h>
#include <arch/header.ld>

SECTIONS
{
	DRAM_START(0x0)
	BOOTBLOCK(0x0, 64K)
	TOC(0x20000, 64K)
	ROMSTAGE(0x40000, 128K)
	STACK(0x60000, 16K)
	PRERAM_CBMEM_CONSOLE(0xa0000, 8K)
	FMAP_CACHE(0xa2000, 2K)
	RAMSTAGE(0xb00000, 16M)
}
