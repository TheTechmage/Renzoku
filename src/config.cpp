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
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <stdexcept>
#include "util.hpp"
#include "log.hpp"

Config::Config(iLogger *logger, ProcessManager* pm) :
	mWatch(),
	mCompile(),
	mTest(),
	mProgram(),
	procman(pm),
	logger(logger)
{
	std::ifstream file;
	this->findConfig(file);
	this->parseConfig(file);
}

Config::Config(iLogger *logger, ProcessManager* pm, std::istream& file) :
	mWatch(),
	mCompile(),
	mTest(),
	mProgram(),
	procman(pm),
	logger(logger)
{
	this->parseConfig(file);
}

Config::~Config()
{
	size_t i;
	if(mCompile.command != NULL) {
		for(i = 0; mCompile.command[i]; i++)
			delete [] mCompile.command[i];
		delete [] mCompile.command;
	}
	if(mTest.command != NULL) {
		for(i = 0; mTest.command[i]; i++)
			delete [] mTest.command[i];
		delete [] mTest.command;
	}
	if(mProgram.command != NULL) {
		for(i = 0; mProgram.command[i]; i++)
			delete [] mProgram.command[i];
		delete [] mProgram.command;
	}
}

void Config::findConfig(std::ifstream& is)
{
	// TODO: file discovery like git finds .git files (including mount point)
	// http://stackoverflow.com/questions/2337139/where-is-a-file-mounted
	is.open(CONFIG_NAME);
	if(!is)
		throw std::runtime_error("Could not open file");
}

void Config::parseConfig(std::istream& file)
{
	int err;
	size_t file_length;
	char* file_contents = NULL;
	file.seekg(0, file.end);
	file_length = file.tellg();
	file.seekg(0, file.beg);
	file_contents = new char[file_length];
	if(file_contents == NULL)
		throw std::runtime_error("Failed to read config [Allocation error]");
	file.read(file_contents, file_length);
	err = ( (size_t)file.gcount() != file_length );
	if(err)
		throw std::runtime_error("Failed to read config [Failed to read all "
				"characters]");
	config.readString(file_contents);
	libconfig::Setting &root = config.getRoot();

	assert(config.exists("watch"));
	assert(config.exists("program"));
	for (auto it = root.begin(); it != root.end(); ++it) {
		assert(it->isGroup());
		//Logger::getLogger()->LOG(logger, DEBUG, "%s - %s\n", it->first.as<std::string>().c_str(), it->second.as<std::string>().c_str());
		std::string key = Util::lowercase_r(it->getName());
		if(key == "watch")
		{
			this->parseWatcher(*it);
		}
		else if(key == "program")
		{
			this->parseCommand(*it, mProgram);
			if(procman->getProgram()) {
				throw std::runtime_error("Program already set!");
			}
			if(mProgram.command && mProgram.enabled)
				procman->setProgram(new Process(logger, mProgram.command, true));
		}
		else if(key == "test")
		{
			this->parseCommand(*it, mTest);
			if(mTest.command && mTest.enabled)
				procman->addProcess(new Process(logger, mTest.command, true));
		}
		else if(key == "compile")
		{
			this->parseCommand(*it, mCompile);
			if(procman->getBuildStep()) {
				throw std::runtime_error("BuildStage already set!");
			}
			if(mCompile.command && mCompile.enabled)
				procman->setBuilder(new Process(logger, mCompile.command, true));
		}
		else
		{
			std::cerr << "Found an error! -" << key << '\n';
			throw std::runtime_error("Found a config error!");
		}
	}
}

void Config::parseCommand(const libconfig::Setting& node, iCommandConfig& config)
{
	//std::cerr << "Under active development!" << '\n';
	//return;
	printf("%d %d %s\n", node.getLength(), node.isArray(), node.getName());
	if(node.getLength() > 0 && node.isGroup())
	{
#if (((LIBCONFIGXX_VER_MAJOR == 1) && (LIBCONFIGXX_VER_MINOR >= 5)) \
		|| (LIBCONFIGXX_VER_MAJOR > 1))
		for (auto iter = node.begin(); iter != node.end(); ++iter) {
			std::string key = iter->getName();
			const libconfig::Setting &value = *iter;
			Util::lowercase(key);
			//std::cout << key << std::endl;
			if(key == "command")
			{
				if(value.isScalar())
				{
					LOG(logger, DEBUG, "Proc: %s", (const char*)value);
					config.command = Util::parseCommand(value);
#if 0
					// TODO: We need to take quotes from the user and send them all as
					// a single argument instead of multiple arguments.
					std::string cnfval = value.as<std::string>();
					char* cmd = new char[cnfval.size() + 1];
					cmd[cnfval.size()] = 0;
					memcpy(cmd, cnfval.c_str(), cnfval.size());
					size_t size = 1;
					for(size_t i=0; i<cnfval.size(); i++)
						if(cmd[i] == ' ')
							size++;
					config.command = new char*[size+1];
					config.command[size] = 0;
					config.command[0] = cmd;
					size_t csize = 0;
					for(size_t i=0; i<cnfval.size(); i++)
					{
						if(cmd[i] == ' ')
						{
							cmd[i] = 0;
							if(csize < size && (i + 1) < cnfval.size())
							{
								config.command[++csize] = cmd+(unsigned int)i+1;
							}
						}
					}
#endif
				}
			}
			else if( key == "enabled" ) {
				config.enabled = value;
			}
		}
#endif
	}
	//for(size_t i=0;config.command[i]; i++)
	//{
	//	std::cout << "==" << config.command[i] << '\n';
	//}
}

void Config::restartProcesses()
{
	procman->restartAll();
}

void Config::parseWatcher(const libconfig::Setting& node)
{
	size_t asterisk_count;
	if(node.getLength() > 0 && node.isGroup())
	{
#if (((LIBCONFIGXX_VER_MAJOR == 1) && (LIBCONFIGXX_VER_MINOR >= 5)) \
		|| (LIBCONFIGXX_VER_MAJOR > 1))
		for (auto iter = node.begin(); iter != node.end(); ++iter) {
			std::string key = iter->getName();
			const libconfig::Setting &value = *iter;
			Util::lowercase(key);
			if(key == "filter")
			{
				if(!value.isArray())
					std::cerr << "ERROR!\n" << key;
				for(auto filter_iter=value.begin();
						filter_iter != value.end();
						++filter_iter)
				{
					asterisk_count = 0;
					std::string val = *filter_iter;
					for(size_t i = 0; i < val.length(); i++)
						if(val[i] == '*')
							asterisk_count++;
					LOG(logger, DEBUG, "Filter: %s", val.c_str());
					if(asterisk_count > 1)
						throw std::runtime_error("Could not open file");
					mWatch.filters.push_back(val);
				}
			}
			else if(key == "include")
			{
				if(!value.isArray() && !value.isScalar())
					std::cerr << "ERROR!\n";
				if(value.isArray())
				{
					for(auto filter_iter=value.begin();
							filter_iter!=value.end();
							++filter_iter)
					{
						LOG(logger, DEBUG, "Include: %s", (const char*)*filter_iter);
						mWatch.include.push_back(*filter_iter);
					}
				}
				else if(value.isScalar())
				{
					LOG(logger, DEBUG, "Include: %s", (const char*)value);
					mWatch.include.push_back(value);
				}
			}
			else if(key == "exclude")
			{
				if(!value.isArray() && !value.isScalar())
					std::cerr << "ERROR!\n";
				if(value.isArray())
				{
					for(auto filter_iter=value.begin();
							filter_iter!=value.end();
							++filter_iter)
					{
						LOG(logger, DEBUG, "Exclude: %s", (const char*)*filter_iter);
						mWatch.exclude.push_back(*filter_iter);
					}
				}
				else if(value.isScalar())
				{
					LOG(logger, DEBUG, "Exclude: %s", (const char*)value);
					mWatch.exclude.push_back(value);
				}
			}
			else
				LOG(logger, DEBUG, "Value: %s\n", (const char*)value);
		}
#endif
	}
}

