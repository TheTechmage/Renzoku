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
#include <parser.hpp>
#include <sstream>
#include <cstring>
#include <map>
#include "catch.hpp"
#include <iostream>
#include "globals.hpp"

size_t strarrlen(char** str) {
	size_t i = 0;
	if(!str)
		return 0;
	while(str[++i]) {}
	return i;
}

TEST_CASE( "Testing Configuration Parsing", "[Config::parse]" ) {
	std::stringstream test;
	test <<
		"Watch CSS {\n"
		"	files = \"*.css\", \"*.less\"\n"
		"	exclude = \"/css/compiled_*\"\n"
		"	dir = \"src/html\"\n"
		"	step Compile {\n"
		"		command = \"lessc --clean-css=--compatibility=ie8 --advanced\\\n"
		"		less/main.less css/compiled_style.css\"\n"
		"	}\n"
		"}\n"
		"Watch \"C++ Code Base\" {\n"
		"	files = \"*.cpp\", \"*.hpp\", \"test_*.rb\", \"Makefile\"\n"
		"	exclude = \"main.*\", \"test_driver.*\"\n"
		"	step Compile {\n"
		"		command = \"make me a sandwich\"\n"
		"		enabled = true\n"
		"	}\n"
		"	step \"Build Tests\" {\n"
		"		command = \"make test\"\n"
		"		enabled = yes\n"
		"	}\n"
		"	step \"Run Ruby Tests\" {\n"
		"		command = \"ruby tests/ruby/test_driver.rb\"; enabled = false;\n"
		"	}\n"
		"	step \"Run Tests\" {\n"
		"		command = \"tests/cpp/test_framework\"\n"
		"		error_status = 5\n"
		"		ignore_status = 3-9, 27\n"
		"	}\n"
		"	step Run {\n"
		"		command = \"bin/my_program\"\n"
		"	}\n"
		"}\n"
		;

	REQUIRE(test.good());
	Parser p(test);
	REQUIRE_NOTHROW(p.Parse());
	uint counter = 0;
	std::map<std::string, const CfgWatch*> map;
	for(auto watcher : p) {
		map[std::string(watcher->name)] = watcher;
		counter++;
	}
	CHECK(counter == 2);
	const CfgWatch* css = p.getWatchers();
	CHECK(std::string(css->name) == "CSS");
	REQUIRE(2 == css->filesFilter.size());
	CHECK(std::string(css->filesFilter[0]) == "*.css");
	CHECK(std::string(css->filesFilter[1]) == "*.less");
	REQUIRE(1 == css->excludesFilter.size());
	CHECK(std::string(css->excludesFilter[0]) == "/css/compiled_*");
	REQUIRE(1 == sizeof(css->steps)/sizeof(CfgStep*));
	const CfgStep* step = &css->steps[0];
	CHECK(std::string(step->name) == "Compile");
	CHECK(std::string(step->command).compare(0, 5, "lessc") == 0);


	//ProcessManager pm(mainLogger);
	//Config cf(mainLogger, &pm, test);
	////INFO("Config file: " << test);
	//INFO("Make sure that our filters are correct.");
	//REQUIRE(cf.getWatchConfig().filters.size() == 5);
	//std::string FilterCheck = cf.getWatchConfig().filters.front();
	//CAPTURE(FilterCheck);
	//FilterCheck = cf.getWatchConfig().filters.back();
	//CAPTURE(FilterCheck);
	//CHECK(cf.getWatchConfig().filters.front() == "*.cpp");
	//CHECK(cf.getWatchConfig().filters.back() == "include/*.hxx");
	//REQUIRE(cf.getWatchConfig().include.size() == 1);
	//REQUIRE(cf.getWatchConfig().exclude.size() == 1);
	//CHECK(cf.getWatchConfig().include.front() == "Makefile");
	//CHECK(cf.getWatchConfig().exclude.front() == "src/Config.cpp");

	//REQUIRE(strarrlen(cf.getCompileConfig().command) == 1);
	//CHECK(strcmp(cf.getCompileConfig().command[0], "make") == 0);
	//CHECK(cf.getCompileConfig().enabled == true);

	//REQUIRE(strarrlen(cf.getTestConfig().command) == 2);
	//CHECK(strcmp(cf.getTestConfig().command[0], "make") == 0);
	//CHECK(strcmp(cf.getTestConfig().command[1], "test") == 0);
	//CHECK(cf.getTestConfig().enabled == false);

	//REQUIRE(strarrlen(cf.getProgramConfig().command) == 2);
	//CHECK(strcmp(cf.getProgramConfig().command[0], "./bin/program") == 0);
	//CHECK(strcmp(cf.getProgramConfig().command[1], "--help") == 0);
	//CHECK(cf.getProgramConfig().enabled == true);
	////for(std::vector<std::string>::const_iterator str = cf.sections.begin(); str != cf.sections.end(); ++str) {
	////	std::cout << std::string(*str) << std::endl;
	////	INFO( *str );
	////}
	////REQUIRE(false);
}

