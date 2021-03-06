/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _TEGRA210_SECURE_BOOT_H_
#define _TEGRA210_SECURE_BOOT_H_

struct tegra_secure_boot {
	u32 sb_csr;		/* offset 0x00 */
	u32 sb_pirom_start;	/* offset 0x04 */
	u32 sb_pfcfg;		/* offset 0x08 */
	u32 sb_secure_spare_0;	/* offset 0x0c */
	u32 sb_secure_spare_1;	/* offset 0x10 */
	u32 sb_secure_spare_2;	/* offset 0x14 */
	u32 sb_secure_spare_3;	/* offset 0x18 */
	u32 sb_secure_spare_4;	/* offset 0x1c */
	u32 sb_secure_spare_5;	/* offset 0x20 */
	u32 sb_secure_spare_6;	/* offset 0x24 */
	u32 sb_secure_spare_7;	/* offset 0x28 */
	u32 rsvd;		/* offset 0x2c */
	u32 sb_aa64_reset_low;	/* offset 0x30 */
	u32 sb_aa64_reset_high;	/* offset 0x3c */
};

#endif	/* _TEGRA210_SECURE_BOOT_H_ */
