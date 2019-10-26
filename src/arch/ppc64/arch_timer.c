/* SPDX-License-Identifier: GPL-2.0-only */

#include <timer.h>
#include <arch/timebase.h>

void timer_monotonic_get(struct mono_time *mt)
{
	// TODO: see src/kernel/timemgr.C
	// looks right for now.
	uint64_t value = mftb();
	uint64_t freq = 512000000ULL;
	long usecs = (value * 1000000) / freq;
	mono_time_set_usecs(mt, usecs);
}
