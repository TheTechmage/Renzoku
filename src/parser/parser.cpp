#include "parser.hpp"
#include "tokenizer.hpp"
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

Parser::Parser(std::string filename) :
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
	watcher->name[name.length()] = '\0';
	// TODO(frostyfrog): Finish this
	for (size_t i = 0; i < name.length(); i++) {
		watcher->name[i] = name[i];
	}
	mTokenizer.next();
	if( mTokenizer.isWhiteSpace() )
		mTokenizer.next();
	assertExp(Tokenizer::SPECIAL, "{");
	mTokenizer.next();
	while( mTokenizer.getToken() != Tokenizer::EOFTOK &&
			mTokenizer.getToken() != Tokenizer::INVALID &&
			mTokenizer.getValue() != "}" )
	{
		if( mTokenizer.isWhiteSpace() ) {
			mTokenizer.next();
			continue;
		}
		std::string key = parseKey();

		parseWatcherOptions(watcher, "files", key);
		parseWatcherOptions(watcher, "exclude", key);
		parseWatcherOptions(watcher, "dir", key);
		if( key == "step" )
		{
			mTokenizer.next();
			if( mTokenizer.getToken() != Tokenizer::WS )
				throw std::runtime_error("Unknown exception while parsing config");
			else
				mTokenizer.next();
			if( mTokenizer.getToken() != Tokenizer::WORD &&
					mTokenizer.getToken() != Tokenizer::STRING )
				throw std::runtime_error("Unknown exception while parsing config");
			std::string name_step = mTokenizer.getValue();
			printf("\tStep: %s\n", name_step.c_str());
			mTokenizer.next();
			if( mTokenizer.isWhiteSpace() )
				mTokenizer.next();
			assertExp(Tokenizer::SPECIAL, "{");
			mTokenizer.next();
			while( mTokenizer.getValue() != "}" ) {
				mTokenizer.next();
			}
			mTokenizer.next();
		}
		mTokenizer.next();
	}
	printf("Watcher [%s] found!\n", watcher->name);
}

std::vector<std::string> Parser::parseWatcherOptions(CfgWatch* watcher,
		std::string expectKey, std::string key) {
		std::vector<std::string> values;
		if( key != expectKey )
			return values;
		parseEquals();
		values = parseValue();
		printf("K: %s V: ", key.c_str());
		for (auto value : values) {
			printf("%s, ", value.c_str());
		}
		printf("\n");
		return values;
}

const std::string& Parser::parseKey() {
	//printf("T: %d V: %s\n", mTokenizer.getToken(),
	//		mTokenizer.getValue().c_str());
	assertExp( Tokenizer::WORD );
	return mTokenizer.getValue();
}
void Parser::parseEquals() {
	bool found_equals = false;
	mTokenizer.next();
	while( mTokenizer.getToken() == Tokenizer::WS ||
			(mTokenizer.getValue() == "=" &&
			!found_equals))
	{
		if( mTokenizer.getValue() == "=" )
			found_equals = true;
		mTokenizer.next();
	}
}
const std::vector<std::string> Parser::parseValue() {
	Tokenizer::TokenType t;
	std::vector<std::string> values;
	std::string valbuf;
	while( mTokenizer.getToken() != Tokenizer::NL &&
			mTokenizer.getValue() != ";" )
	{
		t = mTokenizer.getToken();
		//printf("V: %s\n", mTokenizer.getValue().c_str());
		if( t == Tokenizer::EOFTOK ||
				t == Tokenizer::INVALID ||
				t == Tokenizer::SPECIAL && !safeSpecial())
			throw std::runtime_error("Unknown exception while parsing config");
		//printf("V: %s\n", mTokenizer.getValue().c_str());
		if ( t == Tokenizer::SPECIAL && mTokenizer.getValue() == "," ) {
			values.push_back(valbuf);
			valbuf = "";
			mTokenizer.next();
			if(mTokenizer.getToken() == Tokenizer::WS)
				mTokenizer.next();
			continue;
		}
		valbuf += mTokenizer.getValue();
		mTokenizer.next();
	}
	if( values.size() == 0 || values[values.size()-1] != valbuf )
		values.push_back(valbuf);
	//printf("V: %s\n", value.c_str());
	return values;
}

bool Parser::safeSpecial() {
	std::string v = mTokenizer.getValue();
	return ( v == "-" ||
			v == "_" ||
			v == ";" ||
			v == "," );
}

bool Parser::assertExp(const Tokenizer::TokenType& token) {
	//printf("T: %d:%d V: %s:%s\n", token, mTokenizer.getToken(), value.c_str(),
	//		mTokenizer.getValue().c_str());
	if( mTokenizer.getToken() != token )
		throw std::runtime_error("Unknown exception while parsing config");
	return true;
}

bool Parser::assertExp(const Tokenizer::TokenType& token, std::string value) {
	//printf("T: %d:%d V: %s:%s\n", token, mTokenizer.getToken(), value.c_str(),
	//		mTokenizer.getValue().c_str());
	if( mTokenizer.getToken() != token )
		throw std::runtime_error("Unknown exception while parsing config");
	if( mTokenizer.getValue() != value )
		throw std::runtime_error("Unknown exception while parsing config");
	return true;
}

void Parser::Parse() {
	BLexicon lexer;
	mTokenizer.setLexer(lexer);
	do {
		mTokenizer.next();
		if( mTokenizer.getToken() == Tokenizer::WS ||
				mTokenizer.getToken() == Tokenizer::NL ||
				mTokenizer.getToken() == Tokenizer::EOFTOK )
			continue;
		if( mTokenizer.getToken() == Tokenizer::INVALID )
			throw std::runtime_error("Unknown exception while parsing config");
		//printf("Token: %s\n", toktostr(mTokenizer.getToken()).c_str());
		std::string value = mTokenizer.getValue();
		//printf("Value: %s [%d]\n", value.c_str(), value.size());
		LOWER(value);
		if( value == "watch" ) {
			mTokenizer.next();
			if( mTokenizer.getToken() != Tokenizer::WS )
				throw std::runtime_error("Unknown exception while parsing config");
			else
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
