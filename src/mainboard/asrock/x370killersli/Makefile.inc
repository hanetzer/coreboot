# SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c

ramstage-y += port_descriptors.c

APCB_SOURCES = $(MAINBOARD_BLOBS_DIR)/APCB.bin
