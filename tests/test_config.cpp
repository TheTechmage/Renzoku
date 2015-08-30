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
#include "config.hpp"
#include <sstream>
#include <cstring>
#include "catch.hpp"
#include <iostream>

size_t strarrlen(char** str) {
	size_t i = 0;
	while(str[++i]) {}
	return i;
}

TEST_CASE( "Testing Configuration Parsing", "[Config::parse]" ) {
	std::stringstream test;
	test <<
		"watch:\n"
		"  Filter: ['*.cpp', '*.hpp', '*.c', '*.h', 'include/*.hxx']\n"
		"  Include: Makefile\n"
		"  Exclude: src/Config.cpp\n"
		"compile:\n"
		"  Command: make\n"
		"  Enabled: yes\n"
		"Test:\n"
		"  command: make test\n"
		"  IgnoreStatus: [2,3,4]\n"
		"  Enabled: no\n"
		//"Coverage:\n"
		"program:\n"
		"  command: ./bin/program -h\n"
		"  Enabled: yes\n"
		;

	Config cf(test);
	//INFO("Config file: " << test);
	INFO("Make sure that our filters are correct.");
	REQUIRE(cf.getWatchConfig().filters.size() == 5);
	std::string FilterCheck = cf.getWatchConfig().filters.front();
	CAPTURE(FilterCheck);
	FilterCheck = cf.getWatchConfig().filters.back();
	CAPTURE(FilterCheck);
	CHECK(cf.getWatchConfig().filters.front() == "*.cpp");
	CHECK(cf.getWatchConfig().filters.back() == "include/*.hxx");
	REQUIRE(cf.getWatchConfig().include.size() == 1);
	REQUIRE(cf.getWatchConfig().exclude.size() == 1);
	CHECK(cf.getWatchConfig().include.front() == "Makefile");
	CHECK(cf.getWatchConfig().exclude.front() == "src/Config.cpp");

	REQUIRE(strarrlen(cf.getCompileConfig().command) == 1);
	CHECK(strcmp(cf.getCompileConfig().command[0], "make") == 0);
	CHECK(cf.getCompileConfig().enabled == true);

	REQUIRE(strarrlen(cf.getTestConfig().command) == 2);
	CHECK(strcmp(cf.getTestConfig().command[0], "make") == 0);
	CHECK(strcmp(cf.getTestConfig().command[1], "test") == 0);
	CHECK(cf.getTestConfig().enabled == false);

	REQUIRE(strarrlen(cf.getProgramConfig().command) == 2);
	CHECK(strcmp(cf.getProgramConfig().command[0], "./bin/program") == 0);
	CHECK(strcmp(cf.getProgramConfig().command[1], "-h") == 0);
	CHECK(cf.getProgramConfig().enabled == true);
	//for(std::vector<std::string>::const_iterator str = cf.sections.begin(); str != cf.sections.end(); ++str) {
	//	std::cout << std::string(*str) << std::endl;
	//	INFO( *str );
	//}
	//REQUIRE(false);
}

