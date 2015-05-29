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
#include "process.hpp"
#include "catch.hpp"
#include <sys/stat.h>
#include <cstring>

inline bool exists_test3 (const std::string& name) {
	// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

TEST_CASE( "Testing Sub-process forking", "[Process]" ) {

	SECTION("runAndWait") {
		const char* FILENAME = "/tmp/unit_test_file_0";

		REQUIRE_FALSE(exists_test3(FILENAME));
		std::string command = "touch ";
		command += FILENAME;
		Process p(command, true);
		CHECK_FALSE(exists_test3(FILENAME));
		p.runAndWait();
		REQUIRE(exists_test3(FILENAME));
		command = "rm ";
		command += FILENAME;
		Process p2(command, true);
		p2.runAndWait();
		REQUIRE_FALSE(exists_test3(FILENAME));
	}
}

