/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Early initialization code for PPC64.
 */

#include <processor.h>
#include <arch/asm.h>
#include <arch/asm-offsets.h>
#include <arch/stack.h>

#define GET_STACK(stack_reg,pir_reg)				\
	sldi	stack_reg,pir_reg,STACK_SHIFT;			\
	addis	stack_reg,stack_reg,CPU_STACKS_OFFSET@ha;	\
	addi	stack_reg,stack_reg,CPU_STACKS_OFFSET@l;

#define GET_CPU()
	clrrdi	r16,r1,STACK_SHIFT

#define SAVE_GPR(reg,sp)	std r##reg,STACK_GPR##reg(sp)
#define REST_GPR(reg,sp)	ld r##reg,STACK_GPR##reg(sp)

.section ".text._start", "ax"

.globl _toc_start
.global _etoc
.globl _start
_start:
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
	/* bl	relative_toc */
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

/* ENTRY(relative_toc) */
/* 	mflr	r0 */
/* 	bcl	20, 31, $+4 */
/* 0:	mflr	r11 */
/* 	ld	r2, (p_toc - 0b)(r11) */
/* 	add	r2, r2, r11 */
/* 	mtlr	r0 */
/* 	blr */
/* ENDPROC(relative_toc) */
/* .balign 8 */
/* p_toc: .8byte _toc + 0x8000 - 0b */

. = 0x100
sreset_vector:
	FIXUP_ENDIAN
	li	r3, 0
	oris	r3, r3, 0xa
	b	_start

	. = 0x180

. = 0x200
	mtsprg0	r3
	mtsprg1	r4
	mfspr	r3, SPR_SRR1
	mfcr	r4
	rldicl.	r3, r3, 48, 62
	bne	1f
	mtcr	r4
	mfspr	r3, SPR_CFAR
	li	r4, 0x200
	b _exception
1:
	cmpdi	r3, 0x1
	bne	2f
	LOAD_IMM32(r3, sreset_vector - _start)
	b	3f
2:
	LOAD_IMM32(r3, sreset_vector - _start)
3:
	LOAD_IMM64(r5, _start)
	add	r3, r5, r3
	mtctr	r3
	li	r3, 0x200
	bctr

#define EXCEPTION(nr)		\
	. = nr			;\
	mtsprg0	r3		;\
	mfspr	r3, SPR_CFAR	;\
	mtsprg1	r4		;\
	li	r4, nr		;\
	b	_exception

	EXCEPTION(0x300)
	EXCEPTION(0x380)
	EXCEPTION(0x400)
	EXCEPTION(0x480)
	EXCEPTION(0x500)
	EXCEPTION(0x600)
	EXCEPTION(0x700)
	EXCEPTION(0x800)
	EXCEPTION(0x900)
	EXCEPTION(0x980)
	EXCEPTION(0xa00)
	EXCEPTION(0xb00)
	EXCEPTION(0xc00)
	EXCEPTION(0xd00)
	EXCEPTION(0xe00)
	EXCEPTION(0xe20)
	EXCEPTION(0xe40)
	EXCEPTION(0xe60)
	EXCEPTION(0xe80)
	EXCEPTION(0xf00)
	EXCEPTION(0xf20)
	EXCEPTION(0xf40)
	EXCEPTION(0xf60)
	EXCEPTION(0xf80)
	EXCEPTION(0x1000)
	EXCEPTION(0x1100)
	EXCEPTION(0x1200)
	EXCEPTION(0x1300)
	EXCEPTION(0x1400)
	EXCEPTION(0x1500)
	EXCEPTION(0x1600)

	. = 0x1e00
_exception:
	stdu	r1, -INT_FRAMESIZE(r1)
	std	r3, STACK_CFAR(r1)
	std	r4, STACK_TYPE(r1)
	mfspr	r3, SPR_SRR0
	mfspr	r4, SPR_SRR1
	std	r3, STACK_SRR0(r1)
	std	r3, 16(r1)
	std	r4, STACK_SRR1(r1)
	mfspr	r3, SPR_DSISR
	mfspr	r4, SPR_DAR
	stw	r3, STACK_DSISR(r1)
	std	r4, STACK_DAR(r1)
	mfmsr	r3
	li	r4, MSR_RI
	std	r3,STACK_MSR(r1)
	mtmsrd	r4,1
	mfspr	r3, SPR_HSRR0
	mfspr	r4, SPR_HSRR1
	std	r3, STACK_HSRR0(r1)
	std	r4, STACK_HSRR1(r1)
	mfsprg0	r3
	mfsprg1	r4
	SAVE_GPR(0, r1)
	SAVE_GPR(1, r1)
	SAVE_GPR(2, r1)
	SAVE_GPR(3, r1)
	SAVE_GPR(4, r1)
	SAVE_GPR(5, r1)
	SAVE_GPR(6, r1)
	SAVE_GPR(7, r1)
	SAVE_GPR(8, r1)
	SAVE_GPR(9, r1)
	SAVE_GPR(10, r1)
	SAVE_GPR(11, r1)
	SAVE_GPR(12, r1)
	SAVE_GPR(13, r1)
	SAVE_GPR(14, r1)
	SAVE_GPR(15, r1)
	SAVE_GPR(16, r1)
	SAVE_GPR(17, r1)
	SAVE_GPR(18, r1)
	SAVE_GPR(19, r1)
	SAVE_GPR(20, r1)
	SAVE_GPR(21, r1)
	SAVE_GPR(22, r1)
	SAVE_GPR(23, r1)
	SAVE_GPR(24, r1)
	SAVE_GPR(25, r1)
	SAVE_GPR(26, r1)
	SAVE_GPR(27, r1)
	SAVE_GPR(28, r1)
	SAVE_GPR(29, r1)
	SAVE_GPR(30, r1)
	SAVE_GPR(31, r1)
	mfcr	r3
	mfxer	r4
	mfctr	r5
	mflr	r6
	stw	r3, STACK_CR(r1)
	stw	r4, STACK_XER(r1)
	std	r5, STACK_CTR(r1)
	std	r6, STACK_LR(r1)
	LOAD_IMM64(r3, STACK_INT_MAGIC)
	std	r3, STACK_MAGIC(r1)
	LOAD_IMM64(r4, _start)
	LOAD_IMM32(r5, _toc_start - _start)
	LOAD_IMM32(r6, exception_entry_foo - _start)
	add	r2, r4, r5
	mr	r3, r1
	add	r4, r4, r6
	mtctr	r4
	bctr
exception_entry_foo:
	bl	exception_entry
	/* Restore HSRRs in case a NMI interrupted an HSRR-live section
	 * and the NMI uses HSRRs for something. Possibly does not happen
	 * in current code, but good to be careful.
	 */
	 ld	r3, STACK_HSRR0(r1)
	 ld	r4, STACK_HSRR1(r1)
	 mtspr	SPR_HSRR0, r3
	 mtspr	SPR_HSRR1, r4
	 lwz	r3, STACK_CR(r1)
	 lwz	r4, STACK_XER(r1)
	 ld	r5, STACK_CTR(r1)
	 ld	r6, STACK_LR(r1)
	 mtcr	r3
	 mtxer	r4
	 mtctr	r5
	 mtlr	r6
	 REST_GPR(0, r1)
	 REST_GPR(2, r1)
	 REST_GPR(4, r1)
	 REST_GPR(5, r1)
	 REST_GPR(6, r1)
	 REST_GPR(7, r1)
	 REST_GPR(8, r1)
	 REST_GPR(9, r1)
	 REST_GPR(10, r1)
	 REST_GPR(11, r1)
	 REST_GPR(12, r1)
	 REST_GPR(13, r1)
	 REST_GPR(14, r1)
	 REST_GPR(15, r1)
	 REST_GPR(16, r1)
	 REST_GPR(17, r1)
	 REST_GPR(18, r1)
	 REST_GPR(19, r1)
	 REST_GPR(20, r1)
	 REST_GPR(21, r1)
	 REST_GPR(22, r1)
	 REST_GPR(23, r1)
	 REST_GPR(24, r1)
	 REST_GPR(25, r1)
	 REST_GPR(26, r1)
	 REST_GPR(27, r1)
	 REST_GPR(28, r1)
	 REST_GPR(29, r1)
	 REST_GPR(30, r1)
	 REST_GPR(31, r1)
	 li	r3, 0
	 mtmsrd	r3, 1 /* Clear MSR[RI] */
	 ld	r3, STACK_SRR0(r1)
	 mtspr	SPR_SRR0,r3
	 ld	r3, STACK_SRR1(r1)
	 mtspr	SPR_SRR1, r3
	 REST_GPR(3, r1)
	 addi	r1, r1, INT_FRAMESIZE
	 rfid
	 b .

_other_thread_spinlock:
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

call_bootblock:
	LOAD_IMM32(r2, _toc_start - _start)
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

