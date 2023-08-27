#include <arch/mmio.h>
#include <bootblock_common.h>
#include <console/console.h>
#include <device/i2c_simple.h>
#include <device/pnp_ops.h>
#include <device/smbus_host.h>
#include <smp/node.h>

#include <amdblocks/acpimmio.h>
#include <amdblocks/acpimmio_map.h>
#include <amdblocks/lpc.h>

#include <superio/nuvoton/common/nuvoton.h>
#include <superio/nuvoton/nct6779d/nct6779d.h>

#include "gpio.h"

#define GLOBAL_DEV PNP_DEV(0x2e, 0)
#define SERIAL_DEV PNP_DEV(0x2e, NCT6779D_SP1)
#define ACPI_DEV   PNP_DEV(0x2e, NCT6779D_ACPI)

void bootblock_mainboard_early_init(void)
{
	mainboard_program_early_gpios();
	lpc_enable_sio_decode(LPC_SELECT_SIO_2E2F);
	lpc_enable_decode(DECODE_ENABLE_SERIAL_PORT0 << CONFIG_UART_FOR_CONSOLE);
	nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}
