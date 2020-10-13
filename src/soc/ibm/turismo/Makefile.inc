ifeq ($(CONFIG_SOC_IBM_TURISMO),y)

ramstage-y += soc.c

CPPFLAGS_common += -I$(src)/soc/ibm/common/include
CPPFLAGS_common += -I$(src)/soc/ibm/power8/include
CPPFLAGS_common += -I$(src)/soc/ibm/turismo/include

endif # CONFIG_SOC_IBM_TURISMO
