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

volatile sig_atomic_t gRunning = 1;

int main(int argc, char** argv) {
	struct sigaction int_catcher;
	int_catcher.sa_handler = signal_catch_int;
	//signal(SIGINT, catch_int);
	sigaction(SIGINT,&int_catcher,0);
	//watcher();
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
