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
 *       Filename:  log.cpp
 *
 *    Description:  Log to std handles and to files
 *
 *        Version:  1.0
 *        Created:  05/24/2015 12:37:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:
 *
 * =====================================================================================
 */
#include "log.hpp"
#include <cstdarg>
#include <string>
#include <cstring>

std::string LogColors[] = {
	"\033[0m",
	"\033[1;30m",
	"\033[0;31m",
	"\033[0;32m",
	"\033[0;33m"
};
enum COLORS {
	RESET,
	GREY,
	RED,
	GREEN,
	YELLOW
};

Logger* Logger::mLogger = nullptr;
Logger* Logger::getLogger()
{
	if (mLogger == NULL)
		mLogger = new Logger;
	return mLogger;
}
Logger* Logger::removeLogger()
{
	if(!mLogger)
		return mLogger;
	mLogger->log(DEBUG, "Removing ze logger!");
	Logger* mylogger = mLogger;
	mLogger = nullptr;
	delete mylogger;
	return mLogger;
}

Logger::Logger() :
		mMethod(STDHANDLE)
{
	this->log(DEBUG, "Starting logging service for renzoku　【れんぞく】。");
}

Logger::~Logger()
{
	removeLogger();
}

void Logger::log(LogLevel level, std::string format, ...)
{
	std::string color;
	FILE* logpoint = stdout;
	std::string prefix;
	if(level < INFO)
		return;
	switch(level)
	{
		case DEBUG:
			if(color.empty())
				color = LogColors[GREY];
			prefix = " -> ";
			break;
		case INFO:
			if(color.empty())
				color = LogColors[GREY];
		case SUCCESS:
			if(color.empty())
				color = LogColors[GREEN];
			prefix = "=> ";
			break;
		case WARNING:
			if(color.empty())
				color = LogColors[YELLOW];
		case ERROR:
		case CRITICAL:
			prefix = "=> ";
			if(color.empty())
				color = LogColors[RED];
			logpoint = stderr;
			break;
	}
	va_list va;
	va_start(va, format);
	//fprintf(logpoint, "%s%s ", color.c_str(), prefix.c_str());
	format = color + prefix + format + LogColors[RESET] + '\n';
	vfprintf(logpoint, format.c_str(), va);
	//fprintf(logpoint, "%s\n", LogColors[RESET].c_str());
	va_end(va);
}

void Logger::logCError(std::string userMessage)
{
	const char* message = userMessage.c_str();
	return this->log(ERROR, "%s: %s", message, strerror(errno));
}
