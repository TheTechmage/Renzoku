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
 *       Filename:  config.hpp
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
#pragma once
#include <vector>
#include <string>
#include <istream>
#include <yaml-cpp/yaml.h>
#include "process.hpp"
#include "log.hpp"

#define CONFIG_NAME "renzoku.conf"

struct iConfig {
	virtual ~iConfig() {}
};

struct WatchConfig : public iConfig {
	std::vector<std::string> filters;
	std::vector<std::string> include;
	std::vector<std::string> exclude;
};
struct iCommandConfig : public iConfig {
	bool enabled = true;
	char** command = NULL;
};
struct CompileConfig : public iCommandConfig {
};
struct TestConfig : public iCommandConfig {
	std::vector<short> ignore_states; // TODO: Populate this
};
struct ProgramConfig : public iCommandConfig {
};


class Config {
	/* Config file format:
	 * project XXXXXX
	 * \tbuildcmd XXXX XXXX
	 * \truncmd XXXX
	 * #vim: set tw=8 sw=8 ai noet:
	 */
	private:
		YAML::Node config;
#define CONFIG_SECTION(X) X##Config m##X;
#include "config_sections.hpp"
#undef CONFIG_SECTION
		std::vector<Process*> processes;
		iLogger* logger;
		void parseWatcher(const YAML::Node&);
		void parseCommand(const YAML::Node&, iCommandConfig&);
		void parseConfig(std::istream&);
		std::ifstream findConfig();
	public:
		std::vector<iConfig> sections = {
#define CONFIG_SECTION(X) m##X,
#include "config_sections.hpp"
#undef CONFIG_SECTION
		};
		Config(iLogger*);
		Config(iLogger*, std::istream&);
		~Config();
		inline WatchConfig getWatchConfig() { return mWatch; };
		inline CompileConfig getCompileConfig() { return mCompile; };
		inline TestConfig getTestConfig() { return mTest; };
		inline ProgramConfig getProgramConfig() { return mProgram; };
		inline std::vector<Process*> getProcesses() const { return processes; };
		void restartProcesses();
};
