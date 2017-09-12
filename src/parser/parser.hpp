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
	std::vector<std::string> filesFilter; // Required
	std::vector<std::string> excludesFilter; // Optional, default: NULL
	char* workingDir; // Optional, default: cwd
	CfgStep* steps; // Require at least one
	CfgWatch* next; // Optional, Can be NULL
};
class CfgWatchIter {
	private:
		CfgWatch* node;
	public:
		inline CfgWatchIter(CfgWatch* node=NULL) { this->node = node; }
		inline CfgWatchIter& operator++() { node = node->next; return *this; }
		inline const CfgWatch* operator*() { return node; }
		inline bool operator==(const CfgWatchIter& x) const {
			return node == x.node;
		}
		inline bool operator!=(const CfgWatchIter& x) const {
			return node != x.node;
		}
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
		const char parseBoolean();
		const std::string parseString();
		bool safeSpecial();
		short* parseStatusCodes(std::vector<std::string>&);
		std::vector<std::string> parseWatcherOptions(CfgWatch*, std::string key);
		std::string parseWatcherOption(CfgWatch*, std::string key);
		bool parseBooleanOption(CfgWatch*, std::string key);
		bool mIsKey(const std::string& expectKey, const std::string& key);
		char* mStrToChar(const std::string&);
	public:
		virtual ~Parser();
		Parser(std::string filename);
		Parser(std::istream& file);
		void Parse();
		CfgWatchIter begin() const;
		CfgWatchIter end() const;
		const CfgWatch* getWatchers() const { return mWatchers; }
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
