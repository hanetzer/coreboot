/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <cpu/cpu.h>
#include <device/device.h>

static void soc_init(struct device *dev)
{
}

static struct device_operations soc_ops = {
	.init = soc_init,
};

static void enable_soc_dev(struct device *dev)
{
	dev->ops = &soc_ops;
}

struct chip_operations soc_ibm_turismo_ops = {
	CHIP_NAME("SOC IBM Turismo")
		.enable_dev = enable_soc_dev,
};
