#include "parser.hpp"
#include <algorithm>
#include <stdexcept>

#define LOWER(x) std::transform(x.begin(), x.end(), x.begin(),\
		::tolower);

CfgWatch* create_CfgWatch() {
	CfgWatch* watcher = new CfgWatch;
	watcher->name = NULL;
	watcher->filesFilter = NULL;
	watcher->excludesFilter = NULL;
	watcher->workingDir = NULL;
	watcher->steps = NULL;
	watcher->next = NULL;
	return watcher;
}

Parser::Parser(std::string& filename) :
	mFilename(filename),
	mTokenizer(mFilename),
	mWatchers(NULL),
	mLastWatcher(NULL)
{
}

Parser::~Parser() {
}

void Parser::parseWatcher(CfgWatch* watcher) {
	std::string name = mTokenizer.getValue();
	watcher->name = new char[name.length()+1];
	// TODO(frostyfrog): Finish this
}
void Parser::Parse() {
	BLexicon lexer;
	mTokenizer.setLexer(lexer);
	do {
		mTokenizer.next();
		if( mTokenizer.getToken() != Tokenizer::WS ||
				mTokenizer.getToken() == Tokenizer::NL ||
				mTokenizer.getToken() == Tokenizer::EOFTOK )
			continue;
		if( mTokenizer.getToken() == Tokenizer::INVALID )
			throw std::runtime_error("Unknown exception while parsing config");
		std::string value = mTokenizer.getValue();
		LOWER(value);
		if( value == "watch" ) {
			mTokenizer.next();
			if( mTokenizer.getToken() != Tokenizer::WORD &&
					mTokenizer.getToken() != Tokenizer::STRING )
				throw std::runtime_error("Unknown exception while parsing config");
			CfgWatch* watcher;
			if( mLastWatcher == NULL ) {
				mWatchers = create_CfgWatch();
				mLastWatcher = mWatchers;
			} else {
				watcher = create_CfgWatch();
				mLastWatcher = watcher;
				mLastWatcher->next = watcher;
			}
			watcher = mLastWatcher;
			this->parseWatcher(watcher);
		}
	} while( mTokenizer.getToken() != Tokenizer::EOFTOK &&
			mTokenizer.getToken() != Tokenizer::INVALID );
}

const CfgWatch* Parser::begin() const {
}

const CfgWatch* Parser::end() const {
}
