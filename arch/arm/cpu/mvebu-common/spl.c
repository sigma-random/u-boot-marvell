/*
 * ***************************************************************************
 * Copyright (C) 2015 Marvell International Ltd.
 * ***************************************************************************
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ***************************************************************************
 */

#include <common.h>
#include <spl.h>
#include <fdtdec.h>
#include <asm/arch-mvebu/fdt.h>
#include <asm/arch-mvebu/spl.h>

#ifdef CONFIG_MVEBU_SPL_SAR_DUMP
extern void mvebu_sar_dump_reg(void);
#endif

static int setup_fdt(void)
{
#ifdef CONFIG_OF_CONTROL
#ifdef CONFIG_OF_EMBED
	/* Get a pointer to the FDT */
	gd->fdt_blob = __dtb_dt_begin;
#else
	#error "Support only embedded FDT mode in SPL"
#endif
#endif
	return 0;
}

void board_init_f(ulong silent)
{
	gd = &gdata;
	gd->baudrate = CONFIG_BAUDRATE;
#if 0
	if (silent)
		gd->flags |= GD_FLG_SILENT;
#endif

	/* Update the pointer to the FDT */
	setup_fdt();

	/* Init all drivers before UART initialization */
	early_spl_init();

	/* UART init */
	preloader_console_init();

#ifdef CONFIG_MVEBU_SPL_SAR_DUMP
	/* Sample at reset dump register */
	mvebu_sar_dump_reg();
#endif

	/* Init all relevant drivers (e.g. DDR, comphy...) */
	late_spl_init();

#ifdef CONFIG_MVEBU_SPL_MEMORY_TEST
	/* Momory test */
	if (run_memory_test())
		printf("**** DRAM test failed ****\n");
#endif

	/* Jump to U-Boot if needed */
	soc_spl_jump_uboot();
}