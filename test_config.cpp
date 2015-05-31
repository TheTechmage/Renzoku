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
#include "catch.hpp"

TEST_CASE( "Testing Configuration Parsing", "[Config::parse]" ) {
	std::stringstream test;
	test <<
		"test_project {" << std::endl <<
		"test=1," << std::endl <<
		"	 meep = 39" << std::endl <<
		"}" << std::endl;

	Config cf;
	INFO("Config file: " << test);
	INFO("Make sure that our first project is our dummy project");
	CHECK(cf.getProjects().front() == "building");
	std::string proj = cf.getProjects().front();
	CAPTURE(proj);
	CHECK(cf.getBuildCommand(proj) == "make");
	CHECK(cf.getTestCommand(proj) == "make test");
	CHECK(cf.getRunCommand(proj).empty());
}

