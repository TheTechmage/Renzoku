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
#include "util.hpp"
#include "catch.hpp"
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>

TEST_CASE( "Testing Utility Functions", "[Util]" ) {

	SECTION("cwd") {
		std::string dir;
		dir = Util::cwd();
		REQUIRE_FALSE(dir == "/tmp");
		REQUIRE(chdir("/tmp/") == 0);
		REQUIRE(Util::cwd() == "/tmp");
		chdir(dir.c_str());
	}
	SECTION("listDir") {
		Util::DirList dir = Util::listDir(Util::cwd(), Util::FILETYPE);
		//for(std::string item : dir)
		//	std::cout << "Follow this command: " << item << std::endl;
		REQUIRE_FALSE(dir.empty());
		REQUIRE_FALSE(std::find(dir.begin(), dir.end(), "Makefile") == dir.end());
	}
}

