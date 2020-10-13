/* SPDX-License-Identifier: GPL-2.0-only */

/* This file contains macro definitions for memlayout.ld linker scripts. */

#ifndef __ARCH_MEMLAYOUT_H
#define __ARCH_MEMLAYOUT_H

/* TODO: Double-check that that's the correct alignment for our ABI. */
#define STACK(addr, size) REGION(stack, addr, size, 8)

#define TOC(name, addr, size) \
	SYMBOL(name, addr) \
	.got : \
	{ \
		_toc_start = . + 0x8000; \
		*(.got) \
		*(.toc) \
	} \
	SYMBOL(e##name, addr + size)

#if ENV_ROMSTAGE_OR_BEFORE
	#define CAR_TOC(addr, size) \
		TOC(car_toc, addr, size) \
		ALIAS_REGION(car_toc, toc)
	#define MEM_TOC(addr, size) \
		REGION(mem_toc, addr, size, 4K)
#else
	#define CAR_TOC(addr, size) \
		REGION(car_toc, addr, size, 4K)
	#define MEM_TOC(addr, size) \
		TOC(mem_toc, addr, size) \
		ALIAS_REGION(mem_toc, toc)
#endif

/* TODO: Need to add DMA_COHERENT region like on ARM? */

#endif /* __ARCH_MEMLAYOUT_H */
