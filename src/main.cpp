/*
 * Renzoku - Re-build, re-test, and re-run a program whenever the code changes
 * Copyright (C) 2015  Colton Wolkins
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main Driver
 *
 *        Version:  1.0
 *        Created:  05/23/2015 11:05:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:
 *
 * =====================================================================================
 */
#include <cstdlib>
#include "main.hpp"
#include "signals.hpp"
#include "config.hpp"
#include "watcher.hpp"

volatile sig_atomic_t gRunning = 1;

int main(int argc, char** argv) {
	struct sigaction int_catcher;
	int_catcher.sa_handler = signal_catch_int;
	//signal(SIGINT, catch_int);
	sigaction(SIGINT,&int_catcher,0);
	//watcher();
	Config config;
	printf("%s\n", config.getCompileConfig().command[0]);
	Watcher w("./", config, true);
	while(gRunning)
		w.listen();
	return EXIT_SUCCESS;
}

sig_atomic_t program_is_running()
{
	return gRunning;
}

void stop_program()
{
	gRunning = 0;
}
