#include <amdblocks/lpc.h>
#include <bootblock_common.h>
#include <device/pnp_ops.h>
#include <superio/nuvoton/common/nuvoton.h>
#include <superio/nuvoton/nct6779d/nct6779d.h>

#define GLOBAL_DEV PNP_DEV(0x2e, 0)
#define SERIAL_DEV PNP_DEV(0x2e, NCT6779D_SP1)
#define ACPI_DEV   PNP_DEV(0x2e, NCT6779D_ACPI)

void bootblock_mainboard_early_init(void)
{
	/* nuvoton_pnp_enter_conf_state(GLOBAL_DEV); */
	/* pnp_write_config(GLOBAL_DEV, 0x13, 0x00); */
	/* pnp_write_config(GLOBAL_DEV, 0x14, 0x00); */
	/* pnp_write_config(GLOBAL_DEV, 0x1a, 0x38); */
	/* pnp_write_config(GLOBAL_DEV, 0x1b, 0x26); */
	/* pnp_write_config(GLOBAL_DEV, 0x2a, 0x40); */

	/* pnp_set_logical_device(ACPI_DEV); */
	/* pnp_write_config(ACPI_DEV, 0xe4, 0x10); */
	/* pnp_write_config(ACPI_DEV, 0xe6, 0x5c); */
	/* pnp_write_config(ACPI_DEV, 0xf0, 0x30); */

	/* nuvoton_pnp_exit_conf_state(GLOBAL_DEV); */

	lpc_enable_sio_decode(LPC_SELECT_SIO_2E2F);
	lpc_enable_decode(DECODE_ENABLE_SERIAL_PORT0 << CONFIG_UART_FOR_CONSOLE);
	nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}
