#ifndef __STACK_H__
#define __STACK_H__

#define STACK_SHIFT	15
#define STACK_SIZE	(1 << STACK_SHIFT)

#define STACK_INT_MAGIC	0xb1ab1af00ba1234ULL

#ifndef __ASSEMBLER__
#include <stdint.h>
/* This is the struct used to save GPRs, etc.. from
 * exceptions. It is not always entirely populated
 * depending on the entry type
 */
struct stack_frame {
	/* Standard 112-byte stack frame header (the minimum size required,
	 * using an 8-doubleword param save area). The callee (in C) may use
	 * lrsave; we declare these here so we don't get our own save area
	 * overwritten
	 */
	uint64_t	backchain;
	uint64_t	crsave;
	uint64_t	lrsave;
	uint64_t	compiler_dw;
	uint64_t	linker_dw;
	uint64_t	tocsave;
	uint64_t	paramsave[8];

	/* Space for stack-local vars used by asm. At present we only use
	 * one doubleword */
	uint64_t	locals[1];

	/* Interrupt entry magic value */
	uint64_t	magic;

	/* Entry type */
	uint64_t	type;

	/* GPR save area
	 *
	 * We don't necessarily save everything in here
	 */
	uint64_t	gpr[32];

	/* Other SPR saved
	 *
	 * Only for some exceptions.
	 */
	uint32_t	cr;
	uint32_t	xer;
	uint32_t	dsisr;
	uint64_t	ctr;
	uint64_t	lr;
	uint64_t	pc;
	uint64_t	msr;
	uint64_t	cfar;
	uint64_t	srr0;
	uint64_t	srr1;
	uint64_t	hsrr0;
	uint64_t	hsrr1;
	uint64_t	dar;
} __aligned(16);

#endif /* __ASSEMBLER__ */
#endif
