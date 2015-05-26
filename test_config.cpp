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
	CHECK(cf.getProjects().front() == "building");
	std::string proj = cf.getProjects().front();
	CHECK(cf.getBuildCommand(proj) == "make");
	CHECK(cf.getTestCommand(proj) == "make test");
	CHECK(cf.getRunCommand(proj).empty());
}

