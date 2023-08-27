#include <bootblock_common.h>
#include <console/console.h>
#include <device/i2c_simple.h>

#include <soc/platform_descriptors.h>

void mb_pre_fspm(void)
{
	uint8_t i2c_dat[] = {0x52, 0x02, 0x00, 0x00};
	int ret;

	printk(BIOS_INFO, "before i2c_write_raw\n");
	ret = i2c_write_raw(3, 0x6a, i2c_dat, sizeof(i2c_dat));
	printk(BIOS_WARNING, "i2c_write_raw ret=%d\n", ret);
}
