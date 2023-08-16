/* SPDX-License-Identifier: GPL-2.0-only */

#include <soc/platform_descriptors.h>
#include <soc/soc_util.h>
#include <types.h>

static const fsp_dxio_descriptor pco_dxio_descriptors[] = {
	{
		/* M2_1*/
		.port_present = true,
		.engine_type = PCIE_ENGINE,
		.start_logical_lane = 4,
		.end_logical_lane = 7,
		.device_number = 0,
		.function_number = 0,
		.link_aspm = ASPM_L1,
		.link_aspm_L1_1 = true,
		.link_aspm_L1_2 = true,
		.turn_off_unused_lanes = true,
		.clk_req = CLK_REQ1,
	},
	{
		/* PCIE2_SLOT */
		.port_present = true,
		.engine_type = PCIE_ENGINE,
		.start_logical_lane = 8,
		.end_logical_lane = 15,
		.device_number = 1,
		.function_number = 7,
		.link_aspm = ASPM_L1,
		.link_aspm_L1_1 = true,
		.link_aspm_L1_2 = true,
		.turn_off_unused_lanes = true,
		.clk_req = CLK_REQ0,
	},
};

static const fsp_ddi_descriptor pco_ddi_descriptors[] = {
	{ /* DDI1 - HDMI */
		.connector_type = HDMI,
		.aux_index = AUX1,
		.hdp_index = HDP2
	}
};

void mainboard_get_dxio_ddi_descriptors(
		const fsp_dxio_descriptor **dxio_descs, size_t *dxio_num,
		const fsp_ddi_descriptor **ddi_descs, size_t *ddi_num)
{
	*dxio_descs = pco_dxio_descriptors;
	*dxio_num = ARRAY_SIZE(pco_dxio_descriptors);
	*ddi_descs = pco_ddi_descriptors;
	*ddi_num = ARRAY_SIZE(pco_ddi_descriptors);
}
