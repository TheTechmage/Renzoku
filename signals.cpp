/*
 * =====================================================================================
 *
 *       Filename:  signals.cpp
 *
 *    Description:  Catch different system signals
 *
 *        Version:  1.0
 *        Created:  05/23/2015 11:06:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "main.hpp"
#include <cstdio>
#include <csignal>

void signal_catch_int(int sig) {
	stop_program();
	fprintf(stderr, "\033[0;31m=> Caught deadly SIGINT, termination immenent!\033[0m\n");
	fflush(stderr);
	signal(sig, signal_catch_int);
}
