/*
 * Copyright (C) 2014 Lucas Stach, Pengutronix
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <asm/barebox-arm-head.h>
#include <asm/errata.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <mach/generic.h>
#include <mach/imx7-regs.h>
#include <mach/imx8mq-regs.h>
#include <common.h>
#include <io.h>
#include <asm/syscounter.h>
#include <asm/system.h>

static inline void imx_cpu_timer_init(void __iomem *syscnt)
{
	set_cntfrq(syscnt_get_cntfrq(syscnt));
	syscnt_enable(syscnt);
}

#ifdef CONFIG_CPU_32
void imx5_cpu_lowlevel_init(void)
{
	arm_cpu_lowlevel_init();

	enable_arm_errata_709718_war();
	enable_arm_errata_cortexa8_enable_ibe();
}

void imx6_cpu_lowlevel_init(void)
{
	arm_cpu_lowlevel_init();

	enable_arm_errata_742230_war();
	enable_arm_errata_743622_war();
	enable_arm_errata_751472_war();
	enable_arm_errata_761320_war();
	enable_arm_errata_794072_war();
	enable_arm_errata_845369_war();
}

void imx6ul_cpu_lowlevel_init(void)
{
	cortex_a7_lowlevel_init();
	arm_cpu_lowlevel_init();
}

void imx7_cpu_lowlevel_init(void)
{
	arm_cpu_lowlevel_init();
	imx_cpu_timer_init(IOMEM(MX7_SYSCNT_CTRL_BASE_ADDR));
}

void vf610_cpu_lowlevel_init(void)
{
	arm_cpu_lowlevel_init();
}
#else
void imx8mq_cpu_lowlevel_init(void)
{
	arm_cpu_lowlevel_init();

	if (current_el() == 3)
		imx_cpu_timer_init(IOMEM(MX8MQ_SYSCNT_CTRL_BASE_ADDR));
}
#endif
