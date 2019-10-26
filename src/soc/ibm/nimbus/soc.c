/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/device.h>
#include <console/console.h>

static void soc_init(struct device *dev)
{
	printk(BIOS_INFO, "src/soc/ibm/nimbus/soc.c:soc_init");
}

static struct device_operations soc_ops = {
	.init = soc_init,
};

static void enable_soc_dev(struct device *dev)
{
	dev->ops = &soc_ops;
}

struct chip_operations soc_ibm_nimbus_ops = {
	CHIP_NAME("SOC IBM Nimbus")
		.enable_dev = enable_soc_dev,
};
