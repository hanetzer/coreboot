#include <console/console.h>
#include <device/device.h>
#include <cbmem.h>

static void mainboard_enable(struct device *dev)
{
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
