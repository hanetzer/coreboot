/* SPDX-License-Identifier: GPL-2.0-only */

#include <memlayout.h>

#include <arch/header.ld>

// TODO: fill in these blanks for Power8.
SECTIONS
{
	DRAM_START(0x0)
	BOOTBLOCK(0x0, 64K)
	CAR_TOC(0x20000, 64K)
	ROMSTAGE(0x40000, 128K)
	STACK(0x60000, 0x40000)
	PRERAM_CBMEM_CONSOLE(0xa0000, 8K)
	FMAP_CACHE(0xa2000, 2K)
	RAMSTAGE(0xb00000, 16M)
	MEM_TOC(0x1b00000, 64K)
}
