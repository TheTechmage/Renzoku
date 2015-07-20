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
#include <vector>
#include <string>


class iConfigSection {
	public:
		// virtual void registerSection() = 0; // This will be handled by a macro
		virtual void readSection() = 0;
		virtual void writeSection() = 0;
};

class Config {
	/* Config file format:
	 * project XXXXXX
	 * \tbuildcmd XXXX XXXX
	 * \truncmd XXXX
	 * #vim: set tw=8 sw=8 ai noet:
	 */
	private:
	public:
		std::vector<std::string> sections = {
			"test",
#define CONFIG_SECTION(X) #X,
#include "config_sections.hpp"
#undef CONFIG_SECTION
		};
		std::vector<std::string> getProjects();
		std::string getBuildCommand(std::string project);
		std::string getTestCommand(std::string project);
		std::string getRunCommand(std::string project);
};
