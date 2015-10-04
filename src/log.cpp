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
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cstdarg>
#include <exception>


iLogger::iLogger() :
	colors(false)
{
}

iLogger::iLogger(bool colors) :
	colors(colors)
{
}

void iLogger::startup()
{
	//this->log(DEBUG, "Starting logging service for renzoku　【れんぞく】。");
}

void iLogger::shutdown()
{
	//this->log(DEBUG, "Removing ze logger!");
}

std::string iLogger::createMessage(LogLevel level, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::string retval = this->createMessage(level, format, args);
	va_end(args);
	return retval;
}

std::string iLogger::createMessage(LogLevel level, const char* format, va_list
		arguments)
{
	va_list var_copy;
	va_copy(var_copy, arguments);
	std::string color;
	std::string prefix;
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
			break;
	}
	std::stringstream final_format;
	if(colors)
	{
		final_format <<
			color <<
			prefix <<
			format <<
			LogColors[RESET] <<
			std::endl;
	} else {
		time_t t = time(0);
		struct tm * now = localtime( & t );
		final_format << "[" <<
			(now->tm_year + 1900) << '-' <<
			(now->tm_mon + 1) << '-' <<
			now->tm_mday << ' ' <<
			now->tm_hour << ':' <<
			now->tm_min << ':' <<
			now->tm_sec << "] " <<
			prefix <<
			format <<
			std::endl;
	}
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wformat-security"
	size_t string_size = vsnprintf(nullptr, 0, final_format.str().c_str(),
			var_copy) + 1;
	char* buffer = new char[ string_size + 10 ];
	buffer[0] = 0;
	//char* f = new char[final_format.str().length()+1];
	//strcpy(f, final_format.str().c_str());
	vsprintf( buffer, final_format.str().c_str(), arguments );
//#pragma GCC diagnostic pop
	std::string ret = std::string( buffer, string_size - 1 );
	delete [] buffer;
	return ret;
	// TODO: For windows, see note here:
	// http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
}

std::string iLogger::createCErrorMessage(std::string userMessage)
{
	const char* message = userMessage.c_str();
	return this->createMessage(ERROR, "%s: %s (%d)", message, ::strerror(errno),
			errno);
}

StdoutLogger::StdoutLogger() :
	iLogger(true)
{
	this->startup();
}

StdoutLogger::~StdoutLogger()
{
	this->shutdown();
}

void StdoutLogger::print(LogLevel lvl, std::string message)
{
	if(lvl < WARNING)
		std::cout << message;
	else
		std::cerr << message;
}

void StdoutLogger::printError(std::string message)
{
	std::cout << message;
}

FileLogger::FileLogger(std::string filepath) :
	iLogger(false)
{
	mFile.open(filepath, std::ofstream::out | std::ofstream::app);
	if(!mFile.good())
		throw std::runtime_error("Failed to open log file.");
}

FileLogger::~FileLogger()
{
	if(mFile)
		mFile.close();
}

void FileLogger::print(LogLevel level, std::string message)
{
	mFile << message;
}

void FileLogger::printError(std::string message)
{
	mFile << message;
}

void LogMe(iLogger* logger, LogLevel level, std::string format, ...)
{
	va_list args;
	va_start(args, format);
	logger->print(level, logger->createMessage(level, format.c_str(), args));
	va_end(args);
}
