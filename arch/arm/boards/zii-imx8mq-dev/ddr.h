/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2018 Zodiac Inflight Innovation
 * Author: Andrey Smirnov <andrew.smirnov@gmail.com>
 *
 * Varios wrappers and macros needed to intgrate code generated by
 * i.MX8M DDR Tool into rest of Barebox
 */
#include <common.h>
#include <io.h>
#include <mach/imx8-ddrc.h>

/*
 * Code generated by i.MX8 M DDR Tool doesn't have any prefixes in the
 * global identifiers below, so in order to avoid symbol name
 * collisions with other boards we re-name them via a #define
 */
#define ddr_init	zii_imx8mq_rdu3_ddr_init
#define ddr_cfg_phy	zii_imx8mq_rdu3_ddr_cfg_phy

void zii_imx8mq_rdu3_ddr_init(void);
void zii_imx8mq_rdu3_ddr_cfg_phy(void);

#define FW_1D_IMAGE	lpddr4_pmu_train_1d_imem_bin, \
			lpddr4_pmu_train_1d_dmem_bin
#define FW_2D_IMAGE	lpddr4_pmu_train_2d_imem_bin, \
			lpddr4_pmu_train_2d_dmem_bin


