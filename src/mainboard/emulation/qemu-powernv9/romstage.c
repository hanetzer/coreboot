/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/stages.h>
#include <console/console.h>
#include <program_loading.h>

void platform_romstage_main(void)
{
	console_init();
	run_ramstage();
}
