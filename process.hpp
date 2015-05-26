/*
 * =====================================================================================
 *
 *       Filename:  process.hpp
 *
 *    Description:  Create and manage subprocesses
 *
 *        Version:  1.0
 *        Created:  05/24/2015 12:23:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#pragma once
#include <string>
#include <unistd.h>

class Process
{
	private:
		pid_t mProc;
		bool mSearchInPath;
		const char** mCommand;
	public:
		~Process();
		Process(std::string command, bool in_path);
		bool run();
		bool runAndWait();
		bool kill();
		int status();
};
