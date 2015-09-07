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
#include "log.hpp"
#include <sstream>
#include <ctime>
#include "catch.hpp"


TEST_CASE( "Testing Logging Facilities", "[Logger]" ) {
	time_t t = time(0);
	struct tm * now = localtime( & t );
	std::stringstream filename;
	filename << "/tmp/renzoku_log_test" <<
			'-' << (now->tm_year + 1900) <<
			'-' << (now->tm_mon + 1) <<
			'-' << now->tm_mday <<
			/*'-' << now->tm_hour <<
			'-' << now->tm_min <<
			'-' << now->tm_sec <<
			*/".log";
	FileLogger log(filename.str());
	iLogger* logptr = &log;
	//iLogger* logptr = new StdoutLogger();

	SECTION( "make sure that we can create a logger" )
	{
		REQUIRE(log.is_good());
	}
	SECTION( "attempt to log a few messages" )
	{
		CHECK( true );
		LOG(logptr, DEBUG, "testing %s", "DEBUG");
		LOG(logptr, INFO, "testing %s", "INFO");
		LOG(logptr, WARNING, "testing %s", "WARNING");
		LOG(logptr, ERROR, "testing %s", "ERROR");
		LOG(logptr, CRITICAL, "testing %s", "CRITICAL");
		LOG_ERROR(logptr, "Testing logCError");
	}

	//delete logptr;
}

