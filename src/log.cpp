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

std::string iLogger::createCErrorMessage(std::string userMessage)
{
	const char* message = userMessage.c_str();
	return this->createMessage(ERROR, "%s: %s", message, ::strerror(errno));
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
