/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
 *
 */

#ifdef CONFIG_DEBUG_INITCALLS
#define DEBUG
#endif

/**
 * @file
 * @brief Main entry into the C part of barebox
 */
#include <common.h>
#include <shell.h>
#include <init.h>
#include <command.h>
#include <malloc.h>
#include <debug_ll.h>
#include <fs.h>
#include <errno.h>
#include <linux/stat.h>
#include <envfs.h>
#include <asm/sections.h>
#include <uncompress.h>
#include <globalvar.h>
#include <console_countdown.h>
#include <environment.h>
#include <linux/ctype.h>

extern initcall_t __barebox_initcalls_start[], __barebox_early_initcalls_end[],
		  __barebox_initcalls_end[];

extern exitcall_t __barebox_exitcalls_start[], __barebox_exitcalls_end[];


#if defined CONFIG_FS_RAMFS && defined CONFIG_FS_DEVFS
static int mount_root(void)
{
	mount("none", "ramfs", "/", NULL);
	mkdir("/dev", 0);
	mkdir("/tmp", 0);
	mount("none", "devfs", "/dev", NULL);

	if (IS_ENABLED(CONFIG_FS_EFIVARFS)) {
		mkdir("/efivars", 0);
		mount("none", "efivarfs", "/efivars", NULL);
	}

	if (IS_ENABLED(CONFIG_FS_PSTORE)) {
		mkdir("/pstore", 0);
		mount("none", "pstore", "/pstore", NULL);
	}

	return 0;
}
fs_initcall(mount_root);
#endif

#ifdef CONFIG_ENV_HANDLING
static int check_overlap(const char *path)
{
	struct cdev *cenv, *cdisk, *cpart;
	const char *name;

	name = devpath_to_name(path);

	if (name == path)
		/*
		 * no /dev/ in front, so *path is some file. No need to
		 * check further.
		 */
		return 0;

	cenv = cdev_by_name(name);
	if (!cenv)
		return -EINVAL;

	if (cenv->mtd)
		return 0;

	cdisk = cenv->master;

	if (!cdisk)
		return 0;

	list_for_each_entry(cpart, &cdisk->partitions, partition_entry) {
		if (cpart == cenv)
			continue;

		if (lregion_overlap(cpart->offset, cpart->size,
				    cenv->offset, cenv->size))
			goto conflict;
	}

	return 0;

conflict:
	pr_err("Environment partition (0x%08llx-0x%08llx) "
		"overlaps with partition %s (0x%08llx-0x%08llx), not using it\n",
		cenv->offset, cenv->offset + cenv->size - 1,
		cpart->name,
		cpart->offset, cpart->offset + cpart->size - 1);

	return -EINVAL;
}

static int load_environment(void)
{
	const char *default_environment_path;
	int ret;

	default_environment_path = default_environment_path_get();

	if (IS_ENABLED(CONFIG_DEFAULT_ENVIRONMENT))
		defaultenv_load("/env", 0);

	ret = check_overlap(default_environment_path);
	if (ret)
		default_environment_path_set(NULL);
	else
		envfs_load(default_environment_path, "/env", 0);

	nvvar_load();

	return 0;
}
environment_initcall(load_environment);
#endif

static int global_autoboot_abort_key;
static const char * const global_autoboot_abort_keys[] = {
	"any",
	"ctrl-c",
};
static int global_autoboot_timeout = 3;
static char *global_boot_default;
static char *global_editcmd;
static char *global_linux_bootargs_base;
static char *global_linux_bootargs_console;
static char *global_linux_bootargs_dyn_ip;
static char *global_linux_bootargs_dyn_root;
static char *global_user;

static bool test_abort(void)
{
	bool do_abort = false;
	int c, ret;
	char key;

	while (tstc()) {
		c = getchar();
		if (tolower(c) == 'q' || c == 3)
			do_abort = true;
	}

	if (!do_abort)
		return false;

	printf("Abort init sequence? (y/n)\n"
	       "Will continue with init sequence in:");

	ret = console_countdown(5, CONSOLE_COUNTDOWN_EXTERN, "yYnN", &key);
	if (!ret)
		return false;

	if (tolower(key) == 'y')
		return true;

	return false;
}

static int run_init(void)
{
	DIR *dir;
	struct dirent *d;
	const char *initfile = "/env/bin/init";
	const char *initdir = "/env/init";
	const char *menufile = "/env/menu/mainmenu";
	struct stat s;
	unsigned flags = CONSOLE_COUNTDOWN_EXTERN;
	unsigned char outkey;
	int ret;
	bool menu_exists;
	bool env_bin_init_exists;
	char *abortkeys = NULL;

	setenv("PATH", "/env/bin");

	/* Run legacy /env/bin/init if it exists */
	env_bin_init_exists = stat(initfile, &s) == 0;
	if (env_bin_init_exists) {
		pr_info("running %s...\n", initfile);
		run_command(initfile);
		return 0;
	}

	global_editcmd = xstrdup("sedit");
	global_user = xstrdup("none");
	globalvar_add_simple_string("user", &global_user);
	global_boot_default = xstrdup("net");

	globalvar_add_simple_enum("autoboot_abort_key",
				  &global_autoboot_abort_key,
                                  global_autoboot_abort_keys,
				  ARRAY_SIZE(global_autoboot_abort_keys));
	globalvar_add_simple_int("autoboot_timeout",
				 &global_autoboot_timeout, "%u");
	globalvar_add_simple_string("boot.default", &global_boot_default);
	globalvar_add_simple_string("editcmd", &global_editcmd);
	globalvar_add_simple_string("linux.bootargs.base",
				    &global_linux_bootargs_base);
	globalvar_add_simple_string("linux.bootargs.console",
				    &global_linux_bootargs_console);
	globalvar_add_simple_string("linux.bootargs.dyn.ip",
				    &global_linux_bootargs_dyn_ip);
	globalvar_add_simple_string("linux.bootargs.dyn.root",
				    &global_linux_bootargs_dyn_root);

	/* Unblank console cursor */
	printf("\e[?25h");

	if (test_abort()) {
		pr_info("Init sequence aborted\n");
		return -EINTR;
	}

	/* Run scripts in /env/init/ */
	dir = opendir(initdir);
	if (dir) {
		char *scr;

		while ((d = readdir(dir))) {
			if (*d->d_name == '.')
				continue;

			pr_debug("Executing '%s/%s'...\n", initdir, d->d_name);
			scr = basprintf("source %s/%s", initdir, d->d_name);
			run_command(scr);
			free(scr);
		}

		closedir(dir);
	}

	menu_exists = stat(menufile, &s) == 0;

	if (menu_exists) {
		printf("\nHit m for menu or %s to stop autoboot: ",
		       global_autoboot_abort_keys[global_autoboot_abort_key]);
		abortkeys = "m";
	} else {
		printf("\nHit %s to stop autoboot: ",
		       global_autoboot_abort_keys[global_autoboot_abort_key]);
	}

	switch (global_autoboot_abort_key) {
	case 0:
		flags |= CONSOLE_COUNTDOWN_ANYKEY;
		break;
	case 1:
		flags |= CONSOLE_COUNTDOWN_CTRLC;
		break;
	default:
		break;
	}

	ret = console_countdown(global_autoboot_timeout, flags, abortkeys,
				&outkey);

	if (ret == 0)
		run_command("boot");

	console_ctrlc_allow();

	if (menu_exists) {
		if (outkey == 'm')
			run_command(menufile);

		printf("Enter 'exit' to get back to the menu\n");
		run_shell();
		run_command(menufile);
	}

	return 0;
}

int (*barebox_main)(void);

void __noreturn start_barebox(void)
{
	initcall_t *initcall;
	int result;

	if (!IS_ENABLED(CONFIG_SHELL_NONE) && IS_ENABLED(CONFIG_COMMAND_SUPPORT))
		barebox_main = run_init;

	for (initcall = __barebox_initcalls_start;
			initcall < __barebox_initcalls_end; initcall++) {
		pr_debug("initcall-> %pS\n", *initcall);
		result = (*initcall)();
		if (result)
			pr_err("initcall %pS failed: %s\n", *initcall,
					strerror(-result));
	}

	pr_debug("initcalls done\n");

	if (barebox_main)
		barebox_main();

	if (IS_ENABLED(CONFIG_SHELL_NONE)) {
		pr_err("Nothing left to do\n");
		hang();
	} else {
		while (1)
			run_shell();
	}
}

void __noreturn hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}

/* Everything needed to cleanly shutdown barebox.
 * Should be called before starting an OS to get
 * the devices into a clean state
 */
void shutdown_barebox(void)
{
	exitcall_t *exitcall;

	for (exitcall = __barebox_exitcalls_start;
			exitcall < __barebox_exitcalls_end; exitcall++) {
		pr_debug("exitcall-> %pS\n", *exitcall);
		(*exitcall)();
	}
}
