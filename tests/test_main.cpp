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
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <sstream>
#include <string>
#include <ctime>
#include "log.hpp"
iLogger* mainLogger;
//#include <valgrind/memcheck.h>

int main( int argc, char* const argv[] )
{
	time_t t = time(0);
	struct tm * now = localtime( & t );
	std::stringstream filename;
	filename << "/tmp/renzoku_test" <<
		'-' << (now->tm_year + 1900) <<
		'-' << (now->tm_mon + 1) <<
		'-' << now->tm_mday <<
		/*'-' << now->tm_hour <<
		'-' << now->tm_min <<
		'-' << now->tm_sec <<
		*/".log";
	mainLogger = new FileLogger(filename.str());
  Catch::Session session;

  // writing to session.configData() here sets defaults
  // this is the preferred way to set them

  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) // Indicates a command line error
    return returnCode;

  // writing to session.configData() or session.Config() here
  // overrides command line args
  // only do this if you know you need to

  returnCode = session.run();
  delete mainLogger;
	//VALGRIND_DO_LEAK_CHECK;
}
