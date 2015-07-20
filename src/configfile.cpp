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
#include "configfile.hpp"

ConfigFile::~ConfigFile()
{
	for (std::map<std::string, ConfigItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
	{
		if(it->second == nullptr)
			continue;
		delete it->second;
		it->second = nullptr;
	}
}

ConfigFile::ConfigFile(std::string file)
	: iConfigFile(file)
{
}

ConfigValue* ConfigFile::getItem(std::string key) {
	std::iterator it = mItems.find(key);
	if(it == mItems.end())
		return NULL;
	return it->second;
}
