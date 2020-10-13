/* SPDX-License-Identifier: GPL-2.0-only */

/* Default, Big Endian, Little Endian */
OUTPUT_FORMAT("elf64-powerpc", "elf64-powerpc", "elf64-powerpcle")
/* We use ELF as output format. So that we can debug the code in some form. */
OUTPUT_ARCH(powerpc:common64)

PHDRS
{
	to_load PT_LOAD;
}

#if ENV_BOOTBLOCK
ENTRY(_start)
#else
ENTRY(stage_entry)
#endif
