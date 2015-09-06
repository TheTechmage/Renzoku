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

Config::Config() :
	mWatch(),
	mCompile(),
	mTest(),
	mProgram()
{
	std::ifstream file = this->findConfig();
	this->parseConfig(file);
}

Config::Config(std::istream& file) :
	mWatch(),
	mCompile(),
	mTest(),
	mProgram()
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
	for(Process* proc : processes)
	{
		if(proc)
			delete proc;
	}
}

std::ifstream Config::findConfig()
{
	// TODO: file discovery like git finds .git files (including mount point)
	// http://stackoverflow.com/questions/2337139/where-is-a-file-mounted
	std::ifstream is(CONFIG_NAME);
	if(!is)
		throw std::runtime_error("Could not open file");
	return is;
}

void Config::parseConfig(std::istream& file)
{
	config = YAML::Load(file);
	assert(config);
	assert(config.Type() == YAML::NodeType::Map);
	assert(config.IsMap());
	assert(config["watch"]);
	assert(config["program"]);
	// Compiler
	// Tester
	for (YAML::const_iterator it=config.begin();it!=config.end();++it) {
		assert(it->second.Type() == YAML::NodeType::Map);
		//Logger::getLogger()->log(DEBUG, "%s - %s\n", it->first.as<std::string>().c_str(), it->second.as<std::string>().c_str());
		std::string key = Util::lowercase_r(it->first.as<std::string>());
		if(key == "watch")
		{
			this->parseWatcher(it->second);
		}
		else if(key == "program")
		{
			this->parseCommand(it->second, mProgram);
			if(mProgram.command && mProgram.enabled)
				processes.push_back(new Process(mProgram.command, true));
		}
		else if(key == "test")
		{
			this->parseCommand(it->second, mTest);
			if(mTest.command && mTest.enabled)
				processes.push_back(new Process(mTest.command, true));
		}
		else if(key == "compile")
		{
			this->parseCommand(it->second, mCompile);
			if(mCompile.command && mCompile.enabled)
				processes.push_back(new Process(mCompile.command, true));
		}
		else
		{
			std::cerr << "Found an error! -" << key << '\n';
			throw std::runtime_error("Found a config error!");
		}
	}
}

void Config::parseCommand(const YAML::Node& node, iCommandConfig& config)
{
	//std::cerr << "Under active development!" << '\n';
	//return;
	if(node.size() >= 1 && node.IsMap())
		for (YAML::const_iterator iter=node.begin();iter!=node.end();++iter) {
			std::string key = iter->first.as<std::string>();
			YAML::Node value = iter->second;
			Util::lowercase(key);
			//std::cout << key << std::endl;
			if(key == "command")
			{
				if(value.IsScalar())
				{
					Logger::getLogger()->log(DEBUG, "Proc: %s", value.as<std::string>().c_str());
					config.command = Util::parseCommand(value.as<std::string>());
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
				config.enabled = value.as<bool>();
			}
		}
	//for(size_t i=0;config.command[i]; i++)
	//{
	//	std::cout << "==" << config.command[i] << '\n';
	//}
}

void Config::restartProcesses()
{
	for(Process* proc : processes)
	{
		if(proc->kill())
		{
			if(proc != processes.back())
			{
				if(proc->runAndWait())
				{
					Logger::getLogger()->log(SUCCESS, "Successfully ran command");
				}
				else
				{
					Logger::getLogger()->log(ERROR, "A command failed to run :(");
				}
			}
			else
			{
				proc->run();
			}
		}
	}
}

void Config::parseWatcher(const YAML::Node& node)
{
	size_t asterisk_count;
	if(node.size() >= 1 && node.IsMap())
		for (YAML::const_iterator iter=node.begin();iter!=node.end();++iter) {
			std::string key = iter->first.as<std::string>();
			YAML::Node value = iter->second;
			Util::lowercase(key);
			if(key == "filter")
			{
				if(!value.IsSequence())
					std::cerr << "ERROR!\n";
				for(YAML::const_iterator filter_iter=value.begin();
						filter_iter!=value.end();
						++filter_iter)
				{
					asterisk_count = 0;
					std::string val = filter_iter->as<std::string>();
					for(size_t i = 0; i < val.length(); i++)
						if(val[i] == '*')
							asterisk_count++;
					Logger::getLogger()->log(DEBUG, "Filter: %s", val.c_str());
					if(asterisk_count > 1)
						throw std::runtime_error("Could not open file");
					mWatch.filters.push_back(val);
				}
			}
			else if(key == "include")
			{
				if(!value.IsSequence() && !value.IsScalar())
					std::cerr << "ERROR!\n";
				if(value.IsSequence())
				{
					for(YAML::const_iterator filter_iter=value.begin();
							filter_iter!=value.end();
							++filter_iter)
					{
						Logger::getLogger()->log(DEBUG, "Include: %s", filter_iter->as<std::string>().c_str());
						mWatch.include.push_back(filter_iter->as<std::string>());
					}
				}
				else if(value.IsScalar())
				{
					Logger::getLogger()->log(DEBUG, "Include: %s", value.as<std::string>().c_str());
					mWatch.include.push_back(value.as<std::string>());
				}
			}
			else if(key == "exclude")
			{
				if(!value.IsSequence() && !value.IsScalar())
					std::cerr << "ERROR!\n";
				if(value.IsSequence())
				{
					for(YAML::const_iterator filter_iter=value.begin();
							filter_iter!=value.end();
							++filter_iter)
					{
						Logger::getLogger()->log(DEBUG, "Exclude: %s", filter_iter->as<std::string>().c_str());
						mWatch.exclude.push_back(filter_iter->as<std::string>());
					}
				}
				else if(value.IsScalar())
				{
					Logger::getLogger()->log(DEBUG, "Exclude: %s", value.as<std::string>().c_str());
					mWatch.exclude.push_back(value.as<std::string>());
				}
			}
			else
				Logger::getLogger()->log(DEBUG, "Value: %s\n", value.as<std::string>().c_str());
		}
}

