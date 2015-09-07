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
#include "log.hpp"
#include <string>
#include <unistd.h>

class Process
{
	private:
		pid_t mProc;
		bool mSearchInPath;
		bool mGeneratedCommand;
		char** mCommand;
		iLogger* logger;
	public:
		~Process();
		Process(iLogger*, const std::string &command, bool in_path);
		Process(iLogger*, char** command, bool in_path);
		bool run();
		bool runAndWait();
		bool kill();
		int status() const;
};
