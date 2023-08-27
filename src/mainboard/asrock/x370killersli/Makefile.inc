# SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c
bootblock-y += early_gpio.c

romstage-y += port_descriptors.c
romstage-y += romstage.c

ramstage-y += gpio.c
ramstage-y += port_descriptors.c

APCB_SOURCES = $(MAINBOARD_BLOBS_DIR)/APCB.bin
