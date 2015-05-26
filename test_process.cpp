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
		Process p(std::string("touch ") + FILENAME, true);
		CHECK_FALSE(exists_test3(FILENAME));
		p.runAndWait();
		REQUIRE(exists_test3(FILENAME));
		Process p2(std::string("rm ") + FILENAME, true);
		p2.runAndWait();
		REQUIRE_FALSE(exists_test3(FILENAME));
	}
}

