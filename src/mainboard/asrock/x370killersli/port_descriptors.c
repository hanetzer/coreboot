/* SPDX-License-Identifier: GPL-2.0-only */

#include <soc/platform_descriptors.h>
#include <soc/soc_util.h>
#include <types.h>

static const fsp_dxio_descriptor pco_dxio_descriptors[] = {
	{
		/* PCIE2_SLOT */
		.port_present = true,
		.engine_type = PCIE_ENGINE,
		.start_logical_lane = 8,
		.end_logical_lane = 15,
		.device_number = 1,
		.function_number = 1,
		.link_aspm = ASPM_L1,
		.link_aspm_L1_1 = true,
		.link_aspm_L1_2 = true,
		.turn_off_unused_lanes = true,
		.clk_req = CLK_REQGFX,
	},
	{
		/* M2_1? */
		.port_present = true,
		.engine_type = PCIE_ENGINE,
		.start_logical_lane = 4,
		.end_logical_lane = 7,
		.device_number = 1,
		.function_number = 6,
		.link_aspm = ASPM_L1,
		.link_aspm_L1_1 = true,
		.link_aspm_L1_2 = true,
		.turn_off_unused_lanes = true,
		.clk_req = CLK_REQ4,
	},
	{
		/* X370? */
		.port_present = false,
		.engine_type = UNUSED_ENGINE,
		.start_logical_lane = 0,
		.end_logical_lane = 3,
		.device_number = 1,
		.function_number = 2,
		.link_aspm = ASPM_L1,
		.link_aspm_L1_1 = true,
		.link_aspm_L1_2 = true,
		.turn_off_unused_lanes = true,
		.clk_req = CLK_REQ2,
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
