#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"

struct CfgStep {
	char* name; // Required
	char* command; // Required
	bool daemon; // Optional, do we need this? (Not recommended?)
	bool enabled; // Optional, default: true
	// on_success/on_error commands
	short* error_status; // Optional, default: NULL
	short* ignore_status; // Optional, default: NULL
	CfgStep* next; // Optional, Can be NULL
};

struct CfgWatch {
	char* name; // Required
	char** filesFilter; // Required
	char** excludesFilter; // Optional, default: NULL
	char* workingDir; // Optional, default: cwd
	CfgStep* steps; // Require at least one
	CfgWatch* next; // Optional, Can be NULL
};

class Parser {
	private:
		std::string mFilename;
		Tokenizer mTokenizer;
		CfgWatch* mWatchers;
		CfgWatch* mLastWatcher;

		void parseWatcher(CfgWatch*);
		bool assertExp(const Tokenizer::TokenType&);
		bool assertExp(const Tokenizer::TokenType&, std::string);
		const std::string& parseKey();
		void parseEquals();
		const std::vector<std::string> parseValue();
		bool safeSpecial();
		std::vector<std::string> parseWatcherOptions(CfgWatch*, std::string expectedKey, std::string key);
	public:
		virtual ~Parser();
		Parser(std::string filename);
		void Parse();
		const CfgWatch* begin() const;
		const CfgWatch* end() const;
};

/* Example Config format:
 *
 * Watch CSS {
 *     files = "*.css", "*.less"
 *     exclude = "/css/compiled_*"
 *     dir = "src/html"
 *     step Compile {
 *         command = "lessc --clean-css=\"--compatibility=ie8 --advanced\" \
 *         less/main.less css/compiled_style.css"
 *     }
 * }
 * Watch "C++ Code Base" {
 *     files = "*.cpp", "*.hpp", "test_*.rb", "Makefile"
 *     exclude = "main.*", "test_driver.*"
 *     step Compile {
 *         command = "make me a sandwich"
 *         enabled = true
 *     }
 *     step "Build Tests" {
 *         command = "make test"
 *         enabled = yes
 *     }
 *     step "Run Ruby Tests" {
 *         command = "ruby tests/ruby/test_driver.rb"; enabled = false;
 *     }
 *     step "Run Tests" {
 *         command = "tests/cpp/test_framework"
 *         error_status = 5
 *         ignore_status = 3-9, 27
 *     }
 *     step Run {
 *         command = "bin/my_program"
 *     }
 * }
 */
