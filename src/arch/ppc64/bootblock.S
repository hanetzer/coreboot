/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Early initialization code for PPC64.
 */

#include <arch/asm.h>

ENTRY(_start)
	FIXUP_ENDIAN
	/* Set thread priority high. */
	or	2,2,2
	/* Clear MSR[TA] (bit 1) */
	mfmsr	r20
	/* Clear bit 1 - result [1-63,0] */
	rldicl	r20, r20, 1, 1
	/* Rotate right 1 - result [0,63] */
	rotrdi	r20, r20, 1
	/* Set up srr0/srr1 to enable new MSR */
	mtmsr	r20
	/* Want to default NAP value */
	lis	r20, 0x31
	/* Value is 0x310001 */
	ori	r20, r20, 1
	/* Set actual psscr */
	mtspr	855, r20

_start_postmsr:
	bl	relative_toc
	/* Determine if this is the first thread */
	li	r4, 2
	/* Read spinlock value */
	lis	r20, kernel_other_thread_spinlock@ha
	ori	r20, r20, kernel_other_thread_spinlock@l
	lwsync
1:
	ldarx	r3, 0, r20
	cmpwi	r3, 0
	bnel	cr0, _other_thread_spinlock
	stdcx.	r4, 0, r20
	bne	1b
	isync
	b	call_bootblock
END(_start)

ENTRY(relative_toc)
	mflr	r0
	bcl	20, 31, $+4
0:	mflr	r11
	ld	r2, (p_toc - 0b)(r11)
	add	r2, r2, r11
	mtlr	r0
	blr
ENDPROC(relative_toc)
.balign 8
p_toc: .8byte _toc + 0x8000 - 0b

ENTRY(_other_thread_spinlock)
	/* Read spinlock value */
	lis	r2, kernel_other_thread_spinlock@ha
	ori	r2, r2, kernel_other_thread_spinlock@l
1:
	ld	r3, 0(r2)
	/* Loop until value is 1 */
	cmpwi	cr0, r3, 1
	beq	_other_thread_spinlock_complete
	/* Lower thread priority */
	or	1,1,1
	b	1b
_other_thread_spinlock_complete:
	/* Raise thread priority */
	or	2,2,2
	isync
ENDPROC(_other_thread_spinlock)

call_bootblock:
	/* LOAD_REG_IMMEDIATE(r0, 0xdeadbeefdeadbeef) */
	li	r3, 0
	lis	r1, _estack@h
	ori	r1, r1, _estack@l
	std	r3, 0(r1)
	std	r3, 8(r1)
	std	r3, 16(r1)
	/* lis	r3, _estack@ha */
	/* ori	r3, r3, _estack@l */
/* init_stack_loop: */
	/* std	r0, 0(r1) */
	/* addi	r1, r1, 8 */
	/* cmpd	cr0, r1, r3 */
	/* bne	init_stack_loop */

	bl	main
	nop
	b	.


	/* .section ".id", "a", %progbits */

	/* .section ".id", "a", @progbits */

	/* .globl __id_start */
/* __id_start: */
/* ver: */
	/* .asciz "4" //COREBOOT_VERSION */
/* vendor: */
	/* .asciz "qemu" //CONFIG_MAINBOARD_VENDOR */
/* part: */
	/* .asciz "1" //CONFIG_MAINBOARD_PART_NUMBER */
	/* /1* Reverse offset to the vendor id *1/ */
/* .long __id_end + CONFIG_ID_SECTION_OFFSET - ver */
	/* /1* Reverse offset to the vendor id *1/ */
/* .long __id_end + CONFIG_ID_SECTION_OFFSET - vendor */
	/* /1* Reverse offset to the part number *1/ */
/* .long __id_end + CONFIG_ID_SECTION_OFFSET - part */
	/* /1* of this romimage *1/ */
/* .long CONFIG_ROM_SIZE */
	/* .globl __id_end */

/* __id_end: */
/* .previous */

.section .data

.global kernel_other_thread_spinlock
kernel_other_thread_spinlock:
	.space 8

