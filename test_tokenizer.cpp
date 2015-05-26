#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "tokenizer.hpp"
#include <sstream>
#include "catch.hpp"

TEST_CASE( "Parsing Tokens", "[Tokenizer::parse]" ) {
	Tokenizer tok;
	std::stringstream test;
	test <<
		"test_project {" << std::endl <<
		"test=1," << std::endl <<
		"	 meep = 39" << std::endl <<
		"}" << std::endl;

	tok = Tokenizer(test);
	CHECK(tok.next() == "test_project");
	Token token = tok.getToken();
	CHECK_FALSE(token.getType() == ERROR);
	CHECK(token.getType() == PROJECT);
	CHECK(token.getValue() == "test_project");
}
