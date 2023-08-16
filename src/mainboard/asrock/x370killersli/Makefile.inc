# SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c
bootblock-y += early_gpio.c

romstage-y += port_descriptors.c

ramstage-y += gpio.c
ramstage-y += port_descriptors.c

APCB_SOURCES = src/mainboard/$(MAINBOARDDIR)/bios_17_10_1f.apcb.bin
