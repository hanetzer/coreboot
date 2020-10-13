/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Early initialization code for PPC64.
 */

#include <processor.h>
#include <arch/asm.h>
#include <arch/asm-offsets.h>
#include <arch/stack.h>

.section ".text._start", "ax"

.globl _start
_start:
	FIXUP_ENDIAN
	b	boot_entry
	/* This is our boot semaphore used for CPUs to sync, it has to be
	 * at an easy to locate address (without relocation) since we
	 * need to get at it very early, before we apply our relocs
	 */
	. = 0xf0
boot_sem:
	.long	0
boot_flag:
	.long	0

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

boot_entry:
	/* Set thread priority high. */
	smt_medium
	/* Clear MSR[TA] (bit 1) */
	#if CONFIG(PPC64_LE)
	LOAD_IMM64(r3, (MSR_HV | MSR_SF | MSR_LE))
	#else
	LOAD_IMM64(r3, (MSR_HV | MSR_SF))
	#endif
	mtmsrd	r3, 0
	/* Want to default NAP value */
	LOAD_IMM64(r3, 0x31000);
	/* Set actual psscr */
	mtspr	SPR_PSSCR, r3

	/* Get ourselves a TOC */
	LOAD_IMM64(r2, _toc_start)

	/* Check PVR and set some CR bits */
	mfspr	r28, SPR_PVR
	li	r26, 3 /* Default to SMT4 */
	srdi	r3, r28, 16
	cmpwi	cr0, r3, PVR_TYPE_P8
	beq	2f
	cmpwi	cr0, r3, PVR_TYPE_P8E
	beq	2f
	cmpwi	cr0, r3, PVR_TYPE_P8NVL
	beq	2f
	cmpwi	cr0, r3, PVR_TYPE_P9
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P9P
	beq	3f
	attn		/* Unsupported CPU type... what do we do ? */
	b	.	/* loop here, just in case attn is disabled */

	/* Check for fused core and set flag */
3:
	li	r3, 0x1e0
	mtspr	SPR_SPRC, r3
	mfspr	r3, SPR_SPRD
	andi.	r25, r3, 1
	beq	1f

	/* P8 or P9 fused -> 8 threads */
2:	li	r26, 7

	/* If fused, t1 is primary chiplet and must init shared sprs */
1:	andi.	r3, r25, 1
	beq	not_fused

	mfspr	r31, SPR_PIR
	andi.	r3, r31, 1
	bnel	init_shared_sprs

not_fused:
	/* Check our PIR, avoid threads */
	mfspr	r31, SPR_PIR
	and.	r0, r31, r26
	bne	secondary_wait

	/* Initialize per-core SPRs */
	bl	init_shared_sprs

	/* Pick a boot CPU, cpu index in r31 */
	LOAD_IMM32(r3, boot_sem)
1:	lwarx	r4, 0, r3
	addi	r0, r4, 1
	stwcx.	r0, 0, r3
	bne	1b
	isync
	cmpwi	cr0, r4, 0
	bne	secondary_wait

	/* Make sure we are in SMT medium */
	smt_medium

	/* Initialize thread SPRs */
	bl init_replicated_sprs

	/* Get ready for C code: get a stack */
	GET_STACK(r1, r31)

	/* Clear up initial frame
	 * Zero back chain indicates stack entry from boot,
	 * non-zero indicates entry from OS (see backtrace code).
	 */
	li	r3, 0
	std	r3, 0(r1)
	std	r3, 8(r1)
	std	r3, 16(r1)

	/* Get our per-cpu pointer into r16 */
	GET_CPU()

	/* Jump to C */
	bl	main
	nop
	b	.

	/* Secondary CPUs wait here r31 is PIR */
secondary_wait:
	/* The primary might be in the middle of relocating us,
	 * so first we spin on the boot_flag
	 */
	LOAD_IMM32(r3, boot_flag)
1:	smt_lowest
	lwz	r0, 0(r3)
	cmpdi	r0, 0
	beq	1b

	/* Init some registers */
	bl init_replicated_sprs

	/* Now wait for cpu_secondary_start to be set */
	/* LOAD_ADDR_FROM_TOC(r3, cpu_se */

.global init_shared_sprs
init_shared_sprs:
	li	r0, 0
	mtspr	SPR_AMOR, r0

	mfspr	r3, SPR_PVR
	srdi	r3, r3, 16
	cmpwi	cr0, r3, PVR_TYPE_P8E
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P8
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P8NVL
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P9
	beq	4f
	cmpwi	cr0, r3, PVR_TYPE_P9P
	beq	4f
	/* Unsupported CPU type... what do we do ? */
	b	9f

3:	/* P8E/P8 */
	mtspr	SPR_SDR1, r0
	/* TSCR: Recommended value by HW folks */
	LOAD_IMM32(r3, 0x8acc6880)
	mtspr	SPR_TSCR, r3

	/* HID0: Clear bit 13 (enable core recovery)
	 *       Set/clear bit 19 (HILE) depending on coreboot endian
	 */
	mfspr	r3, SPR_HID0
	li	r0, 1
	sldi	r4, r0, (63-13)
	andc	r3, r3, r4
	sldi	r4, r0, (63-19)
	#if CONFIG(PPC64_LE)
	or	r3, r3, r4
	#else
	andc	r3, r3, r4
	#endif
	sync
	mtspr SPR_HID0, r3
	mfspr	r3, SPR_HID0
	mfspr	r3, SPR_HID0
	mfspr	r3, SPR_HID0
	mfspr	r3, SPR_HID0
	mfspr	r3, SPR_HID0
	mfspr	r3, SPR_HID0
	isync
	/* HMEER: Enable HMIs for core recovery and TOD errors. */
	mfspr	r3, SPR_HMEER
	or	r3, r3, r0
	sync
	mtspr	SPR_HMEER, r3
	isync
	/* RPR (per-cpu LPAR but lets treat it as replicated for now) */
	LOAD_IMM64(r3, 0x00000103070F1F3F)
	mtspr	SPR_RPR, r3
	b	9f

4:	/* P9 */
	/* TSCR: Recommended value by HW folks */
	LOAD_IMM32(r3, 0x80287880)
	mtspr	SPR_TSCR, r3
	/* HID0: Clear bit 5 (enable core recovery)
	 *       Set/clear bit 4 (HILE) depending on coreboot endian
	 *       Set bit 8 (radix)
	 */
	mfspr	r3, SPR_HID0
	li	r0, 1
	sldi	r4, r0, (63-4)
	#if CONFIG(PPC64_LE)
	or	r3, r3, r4
	#else
	andc	r3, r3, r4
	#endif
	sldi	r4, r0, (63-5)
	andc	r3, r3, r4
	sldi	r4, r0, (63-8)
	sync
	mtspr	SPR_HID0, r3
	isync
	/* HMEER: Enable HMIs for core recovery and TOD errors. */
	LOAD_IMM64(r0, SPR_HMEER_HMI_ENABLE_MASK)
	mfspr	r3, SPR_HMEER
	or	r3, r3, r0
	sync
	mtspr	SPR_HMEER, r3
	isync

	LOAD_IMM32(r3, 0x00000103070F1F3F)
	mtspr	SPR_RPR, r3
9:	blr

.global init_replicated_sprs
init_replicated_sprs:
	mfspr	r3, SPR_PVR
	srdi	r3, r3, 16
	cmpwi	cr0, r3, PVR_TYPE_P8E
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P8
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P8NVL
	beq	3f
	cmpwi	cr0, r3, PVR_TYPE_P9
	beq	4f
	cmpwi	cr0, r3, PVR_TYPE_P9P
	beq	4f
	/* Unsupported CPU type... what do we do ? */
	b	9f

3:	/* P8, P8E */
	/* LPCR: sane value */
	LOAD_IMM64(r3, 0x0040000000000000)
	mtspr	SPR_LPCR, r3
	sync
	isync
	LOAD_IMM64(r3, 0x0)
	mtspr	SPR_DSCR, r3
	b	9f

4:	/* P9 */
	/* LPCR: sane value */
	LOAD_IMM64(r3, 0x0040000000000000)
	mtspr	SPR_LPCR, r3
	sync
	isync
	/* DSCR: Stride-N Stream Enable */
	LOAD_IMM64(r3, 0x0000000000000010)
	mtspr	SPR_DSCR, r3

9: blr

