## SPDX-License-Identifier: GPL-2.0-only

ppc64_flags = -I$(src)/arch/ppc64/ -Wa,-mregnames

ifeq ($(CONFIG_PPC64_LE),y)
ppc64_flags += -mlittle-endian
_ppc64_endian += -EL
else
ppc64_flags += -mbig-endian
_ppc64_endian += -EB
endif

ifneq ($(CONFIG_PPC64_ARCH),"")
ppc64_flags += -mcpu=$(CONFIG_PPC64_ARCH) -mtune=$(CONFIG_PPC64_ARCH)
endif

ppc64_asm_flags = -mregnames

COMPILER_RT_bootblock = $(shell $(GCC_bootblock) $(ppc64_flags) -print-libgcc-file-name)

COMPILER_RT_romstage = $(shell $(GCC_romstage) $(ppc64_flags) -print-libgcc-file-name)

COMPILER_RT_ramstage = $(shell $(GCC_ramstage) $(ppc64_flags) -print-libgcc-file-name)

################################################################################
## bootblock
################################################################################
ifeq ($(CONFIG_ARCH_BOOTBLOCK_PPC64),y)

bootblock-y = bootblock.S stages.c
bootblock-y += boot.c
bootblock-y += rom_media.c
bootblock-$(CONFIG_PPC64_USE_ARCH_TIMER) += arch_timer.c
bootblock-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

bootblock-generic-ccopts += $(ppc64_flags)

$(objcbfs)/bootblock.debug: $$(bootblock-objs)
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
	$(LD_bootblock) $(LDFLAGS_bootblock) $(_ppc64_endian) -o $@ -L$(obj) \
		-T $(call src-to-obj,bootblock,$(CONFIG_MEMLAYOUT_LD_FILE)) --whole-archive --start-group $(filter-out %.ld,$(bootblock-objs)) \
		$(LIBGCC_FILE_NAME_bootblock) --end-group $(COMPILER_RT_bootblock)

endif

################################################################################
## romstage
################################################################################
ifeq ($(CONFIG_ARCH_ROMSTAGE_PPC64),y)

romstage-y += boot.c
romstage-y += stages.c
romstage-y += rom_media.c
romstage-$(CONFIG_PPC64_USE_ARCH_TIMER) += arch_timer.c
romstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

romstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

# Build the romstage

$(objcbfs)/romstage.debug: $$(romstage-objs)
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
	$(LD_romstage) $(LDFLAGS_romstage) $(_ppc64_endian) -o $@ -L$(obj) -T $(call src-to-obj,romstage,$(CONFIG_MEMLAYOUT_LD_FILE)) --whole-archive --start-group $(filter-out %.ld,$(romstage-objs)) --end-group $(COMPILER_RT_romstage)

romstage-c-ccopts += $(ppc64_flags)
romstage-S-ccopts += $(ppc64_asm_flags)

endif

################################################################################
## ramstage
################################################################################
ifeq ($(CONFIG_ARCH_RAMSTAGE_PPC64),y)

ramstage-y += rom_media.c
ramstage-y += stages.c
ramstage-y += boot.c
ramstage-y += tables.c
ramstage-$(CONFIG_PPC64_USE_ARCH_TIMER) += arch_timer.c
ramstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

$(eval $(call create_class_compiler,rmodules,power8))

ramstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

ramstage-srcs += src/mainboard/$(MAINBOARDDIR)/mainboard.c

# Build the ramstage

$(objcbfs)/ramstage.debug: $$(ramstage-objs)
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(LD_ramstage) $(LDFLAGS_ramstage) $(_ppc64_endian) -o $@ -L$(obj) -T $(call src-to-obj,ramstage,$(CONFIG_MEMLAYOUT_LD_FILE)) --whole-archive --start-group $(filter-out %.ld,$(ramstage-objs)) --end-group $(COMPILER_RT_ramstage)

ramstage-c-ccopts += $(ppc64_flags)
ramstage-S-ccopts += $(ppc64_asm_flags)

endif
