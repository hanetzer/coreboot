ifeq ($(CONFIG_SOC_IBM_NIMBUS),y)

ramstage-y += soc.c

CPPFLAGS_common += -I$(src)/soc/ibm/common/include
CPPFLAGS_common += -I$(src)/soc/ibm/power9/include
CPPFLAGS_common += -I$(src)/soc/ibm/nimbus/include

endif # CONFIG_SOC_IBM_NIMBUS
