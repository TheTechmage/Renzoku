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
 *       Filename:  config.cpp
 *
 *    Description:  Configuration file retrieval
 *
 *        Version:  1.0
 *        Created:  05/23/2015 10:51:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *
 * =====================================================================================
 */
#include "config.hpp"

std::vector<std::string> Config::getProjects()
{
	std::vector<std::string> v;
	v.push_back("building");
	return v;
}
std::string Config::getBuildCommand(std::string project)
{
	return "make";
}
std::string Config::getTestCommand(std::string project)
{
	return "make test";
}
std::string Config::getRunCommand(std::string project)
{
	return "";
}
