/* SPDX-License-Identifier: GPL-2.0-only */

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

#endif /* __PPC_PPC64_ASM_H */
