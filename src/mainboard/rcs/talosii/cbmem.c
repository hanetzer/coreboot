#include <cbmem.h>

void *cbmem_top_chipset(void)
{
	/* Top of cbmem is lowst usable DRAM address below 4GiB. */
	/* For now, last 1M of 4G */
	/* void *ptr = (void *) ((1ULL << 32) - 0x100000); */
	void *ptr = (void *) (4ULL << 6);
	return ptr;
}
