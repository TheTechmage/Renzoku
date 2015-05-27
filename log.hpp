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
 *       Filename:  log.hpp
 *
 *    Description:  Log to std handles and to files
 *
 *        Version:  1.0
 *        Created:  05/24/2015 12:37:54 AM
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

enum LogLevel {
	DEBUG,
	INFO,
	SUCCESS,
	WARNING,
	ERROR,
	CRITICAL
};

enum LogMethod {
	LOGFILE=1,
	STDHANDLE=2
};
inline LogMethod operator|(LogMethod a, LogMethod b)
{return static_cast<LogMethod>(static_cast<int>(a) | static_cast<int>(b));}
inline LogMethod operator&(LogMethod a, LogMethod b)
{return static_cast<LogMethod>(static_cast<int>(a) & static_cast<int>(b));}

class Logger {
	private:
		std::string mFile;
		LogMethod mMethod;
		Logger();
	public:
		static Logger* mLogger;
		static Logger* getLogger();
		static Logger* removeLogger();
		void log(LogLevel, std::string format, ...);
		void logCError(std::string message);
};
