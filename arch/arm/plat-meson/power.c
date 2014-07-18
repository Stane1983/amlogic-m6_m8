/* 
 *Copyright (c) AMLOGIC CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * Created by Frank Chen
 */

#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/efi.h>
#include <linux/dmi.h>
#include <linux/sched.h>
#include <linux/tboot.h>
#include <linux/delay.h>

/*
 * These are system power hooks to implement power down policy
 * pls add rule and policy notes 
 *
 * pm_power_off_prepare will be called before system actually power off
 * pm_power_off will be called after system power off
 *
 * 
 * now the policy is:
 *     1 poweroff, reboot system go into uboot, and shutdown, this is typical requirement for tablet production
 *
 *
 */

void meson_power_off_prepare(void)
{
	printk("meson prepare power off \n");
}

void meson_power_off(void)
{
        printk("meson power off \n");
	kernel_restart("charging_reboot");
}

void meson_power_idle(void)
{
	printk("meson power idle\n");
}

static int __init meson_reboot_setup(void)
{
        pm_power_off_prepare = meson_power_off_prepare;
        pm_power_off = meson_power_off;
//	pm_idle = meson_power_idle;
        return 0;
}

arch_initcall(meson_reboot_setup);

