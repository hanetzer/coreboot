#ifndef __TIMEBASE_H__
#define __TIMEBASE_H__

#ifndef __ASSEMBLER__

static inline unsigned long mftb(void)
{
	unsigned long tb;

	/* We use a memory clobber to avoid this being
	 * moved in the instruction stream
	 */
	asm volatile("mftb %0" : "=r"(tb) : : "memory");
	return tb;
}

#endif /* __ASSEMBLER__ */

#endif /* __TIMEBASE_H__ */
