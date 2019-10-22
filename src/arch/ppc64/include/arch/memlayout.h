/* SPDX-License-Identifier: GPL-2.0-only */

/* This file contains macro definitions for memlayout.ld linker scripts. */

#ifndef __ARCH_MEMLAYOUT_H
#define __ARCH_MEMLAYOUT_H

/* TODO: Double-check that that's the correct alignment for our ABI. */
#define STACK(addr, size) REGION(stack, addr, size, 8)

#define TOC(addr, size) \
	SYMBOL(toc, addr) \
	.got : \
	{ \
		_toc_start = . + 0x8000; \
		*(.got) \
		*(.toc) \
	} \
	SYMBOL(etoc, addr + size)

/* TODO: Need to add DMA_COHERENT region like on ARM? */

#endif /* __ARCH_MEMLAYOUT_H */
