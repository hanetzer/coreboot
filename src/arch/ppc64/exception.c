#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <types.h>
#include <arch/stack.h>
#include <console/console.h>
#include <processor.h>

#define REG		"%016llx"
#define REG32		"%08x"
#define REGS_PER_LINE	4

#define EXCEPTION_MAX_STR 320

void exception_entry(struct stack_frame *);

void exception_entry(struct stack_frame *stack)
{
	bool fatal = false;
	bool hv;
	uint64_t nip;
	uint64_t msr;
	char buf[EXCEPTION_MAX_STR];
	size_t l;

	switch(stack->type) {
	case 0x500:
	case 0x980:
	case 0xe00:
	case 0xe20:
	case 0xe40:
	case 0xe60:
	case 0xe80:
	case 0xea0:
	case 0xf80:
		hv = true;
		break;
	default:
		hv = false;
		break;
	}

	if (hv) {
		nip = stack->hsrr0;
		msr = stack->hsrr1;
	} else {
		nip = stack->srr0;
		msr = stack->srr1;
	}
	stack->msr = msr;
	stack->pc = nip;

	if (!(msr & MSR_RI))
		fatal = true;

	l = 0;
	switch (stack->type) {
	case 0x100:
		printk(BIOS_ERR, "***********************************************\n");
		if (fatal) {
			l += snprintf(buf + l, EXCEPTION_MAX_STR -l,
					"Fatal System Reset at "REG"   ", nip);
		} else {
			l += snprintf(buf + l, EXCEPTION_MAX_STR -l,
					"System Reset at "REG"   ", nip);
		}
		break;
	default:
		fatal = true;
		printk(BIOS_ERR, "***********************************************\n");
		l += snprintf(buf + l, EXCEPTION_MAX_STR -l,
				"Fatal Exception 0x%llx at "REG"  ", stack->type, nip);
		break;
	}
	l += snprintf(buf + l, EXCEPTION_MAX_STR - l, "  MSR"REG, msr);
	printk(BIOS_ERR, "%s\n", buf);
}
