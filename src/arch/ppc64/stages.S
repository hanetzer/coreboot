/* #include <cbmem.h> */
/* #include <arch/stages.h> */
#include <rules.h>
#include <arch/asm.h>
#include <arch/stack.h>
#include <processor.h>

ENTRY(stage_entry)
	FIXUP_ENDIAN
	LOAD_IMM64(r2, _toc_start)
	#if !ENV_ROMSTAGE_OR_BEFORE
	LOAD_ADDR_FROM_TOC(r0, _cbmem_top_ptr)
	std	r3,0(r0)
	#endif

	/* Get ready for C code, get a stack */
	mfspr	r31, SPR_PIR
	GET_STACK(r1,r31)

	/* Clear up initial frame.
	 * Zero back chain indicates stack entry from boot,
	 * non-zero indicates entry from OS (see backtrace code)
	 */
	li	r3,0
	std	r3,0(r1)
	std	r3,8(r1)
	std	r3,16(r1)

	/* Jump to C */
	bl	main
	b	.
ENDPROC(stage_entry)
