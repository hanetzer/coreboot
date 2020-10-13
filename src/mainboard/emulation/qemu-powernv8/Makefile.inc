## SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c
bootblock-y += uart.c
bootblock-y += timer.c

romstage-y += cbmem.c
romstage-y += romstage.c
romstage-y += uart.c

ramstage-y += timer.c
ramstage-y += uart.c
