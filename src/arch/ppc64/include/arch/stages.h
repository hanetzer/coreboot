/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __ARCH_STAGES_H
#define __ARCH_STAGES_H

#include <stdint.h>
#include <main_decl.h>

void stage_entry(uintptr_t stage_arg) __attribute__((section(".text.stage_entry")));

void platform_romstage_main(void);
void platform_romstage_postram(void);
#endif
