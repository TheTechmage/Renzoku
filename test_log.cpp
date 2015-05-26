#include "log.hpp"
#include <sstream>
#include "catch.hpp"

TEST_CASE( "Testing Logging Facilities", "[Logger]" ) {
	std::stringstream test;
	test <<
		"test_project {" << std::endl <<
		"test=1," << std::endl <<
		"	 meep = 39" << std::endl <<
		"}" << std::endl;

	SECTION( "make sure that we can create a logger" )
	{
		REQUIRE(Logger::getLogger());
	}
	SECTION( "attempt to log a few messages" )
	{
		CHECK( true );
		Logger::getLogger()->log(DEBUG, "testing %s", "DEBUG");
		Logger::getLogger()->log(INFO, "testing %s", "INFO");
		Logger::getLogger()->log(WARNING, "testing %s", "WARNING");
		Logger::getLogger()->log(ERROR, "testing %s", "ERROR");
		Logger::getLogger()->log(CRITICAL, "testing %s", "CRITICAL");
		Logger::getLogger()->logCError("Testing logCError");
	}
	SECTION( "make sure that our logger is a singleton" )
	{
		REQUIRE(Logger::getLogger() == Logger::getLogger());
	}
	SECTION( "delete our logger" )
	{
		CHECK_FALSE(Logger::removeLogger());
		CHECK(Logger::mLogger == nullptr);
	}
}

