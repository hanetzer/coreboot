/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright 2013-2019 IBM Corp. */

#ifndef __PPC_PPC64_ASM_H
#define __PPC_PPC64_ASM_H

#define ENDPROC(name) \
	.type name, @function; \
	END(name)

#define ENTRY_WITH_ALIGN(name, bits) \
	.section .text.name, "ax", @progbits; \
	.global name; \
	.align bits; \
	name:

#define ENTRY(name) ENTRY_WITH_ALIGN(name, 2)

#define END(name) \
	.size name, .-name

/*
 * Do NOT use the immediate load helpers with symbols
 * only with constants. Symbols will _not_ be resolved
 * by the linker since we are building -pie, and will
 * instead generate relocs of a type our little built-in
 * relocator can't handle
 */

/* Load an immediate 64-bit value into a register */
#define LOAD_IMM64(r, e)			\
	lis     r,(e)@highest;			\
	ori     r,r,(e)@higher;			\
	rldicr  r,r, 32, 31;			\
	oris    r,r, (e)@h;			\
	ori     r,r, (e)@l;

/* Load an immediate 32-bit value into a register */
#define LOAD_IMM32(r, e)			\
	lis     r,(e)@h;			\
	ori     r,r,(e)@l;

/* Load an address via the TOC */
#define LOAD_ADDR_FROM_TOC(r, e)	ld r,e@got(%r2)

/* This must preserve LR, may only clobber r11-r12, so can't use Linux kernel's
 * FIXUP_ENDIAN */
#define SWITCH_ENDIAN						   \
	.long 0xa600607d; /* mfmsr r11				*/ \
	.long 0x01006b69; /* xori r11,r11,1			*/ \
	.long 0xa64b7b7d; /* mthsrr1 r11			*/ \
	.long 0xa602687d; /* mflr r11				*/ \
	.long 0x05009f42; /* bcl 20,31,$+4			*/ \
	.long 0xa602887d; /* mflr r12				*/ \
	.long 0x14008c39; /* addi r12,r12,20			*/ \
	.long 0xa64b9a7d; /* mthsrr0 r12			*/ \
	.long 0xa603687d; /* mtlr r11				*/ \
	.long 0x2402004c  /* hrfid				*/

#define FIXUP_ENDIAN						   \
	tdi   0,0,0x48;	  /* Reverse endian of b . + 8		*/ \
	b     191f;	  /* Skip trampoline if endian is good	*/ \
	SWITCH_ENDIAN;	  /* Do the switch			*/ \
191:

#endif /* __PPC_PPC64_ASM_H */
