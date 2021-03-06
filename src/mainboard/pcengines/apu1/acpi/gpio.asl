/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * Based on the example of Mika Westerberg: https://lwn.net/Articles/612062/
 */

Scope (\_SB.PCI0.SBUS)
{
	Device (GPIO)
	{
		Name (_HID, "PRP0001")

		Name (_CRS, ResourceTemplate () {
			/* ACPI_MMIO_BASE + gpio offset */
			Memory32Fixed(ReadWrite, 0xFED80100, 0x0000100)
		})

		Name (_DSD, Package () {
			ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package () {
				Package () {"compatible", Package () {"gpio-sb8xx"}},
			}
		})
	}
}
