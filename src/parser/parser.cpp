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
	// TODO(frostyfrog): Cleanup
}

void Parser::parseWatcher(CfgWatch* watcher) {
	// Initialize local variables
	std::string name = mTokenizer.getValue();
	watcher->name = new char[name.length()+1];
	watcher->name[name.length()] = '\0';

	// TODO(frostyfrog): Finish this

	// Convert string to char array
	for (size_t i = 0; i < name.length(); i++) {
		watcher->name[i] = name[i];
	}

	// Debug: Print out the watcher's name
	printf("Watcher [%s] found!\n", watcher->name);

	// Move off the watch name token
	mTokenizer.next();

	// Eat whitespace
	if( mTokenizer.isWhiteSpace() )
		mTokenizer.next();

	// Parse the section opening
	assertExp(Tokenizer::SPECIAL, "{");
	mTokenizer.next();

	// Loop through internal values until we hit an invalid token or }
	while( mTokenizer.getToken() != Tokenizer::EOFTOK &&
			mTokenizer.getToken() != Tokenizer::INVALID &&
			mTokenizer.getValue() != "}" )
	{

		// Skip whitespace
		if( mTokenizer.isWhiteSpace() ) {
			mTokenizer.next();
			continue;
		}

		// Parse out the key in the key value pairs
		std::string key = parseKey();
		//LOWER(key) // Should we allow this?

		// Parse each valid section
		parseWatcherOptions(watcher, "files", key);
		parseWatcherOptions(watcher, "exclude", key);
		parseWatcherOptions(watcher, "dir", key);

		// We are now parsing a step
		if( key == "step" )
		{
			// Skip the whitespace and move on to the next token
			mTokenizer.next();
			if( mTokenizer.getToken() != Tokenizer::WS )
				throw std::runtime_error("Unknown exception while parsing config");
			else
				mTokenizer.next();

			// Make sure that the token is a word
			if( mTokenizer.getToken() != Tokenizer::WORD &&
					mTokenizer.getToken() != Tokenizer::STRING )
				throw std::runtime_error("Unknown exception while parsing config");

			// Get the name of the current step
			std::string name_step = mTokenizer.getValue();
			printf("\tStep: %s\n", name_step.c_str());

			// Skip whitespace
			mTokenizer.next();
			if( mTokenizer.isWhiteSpace() )
				mTokenizer.next();

			// Find the { and skip it
			assertExp(Tokenizer::SPECIAL, "{");
			mTokenizer.next();

			// TODO(frostyfrog): Parse the insides of the step
			while( mTokenizer.getValue() != "}" ) {
				if( mTokenizer.getToken() == Tokenizer::WORD ) {
					key = parseKey();
					// Parse each valid section
					printf("\t");
					parseWatcherOptions(watcher, "enabled", key);
					parseWatcherOptions(watcher, "command", key);
					parseWatcherOptions(watcher, "error_status", key);
					parseWatcherOptions(watcher, "ignore_status", key);
				}
				mTokenizer.next();
			}

			// Skip the }
			mTokenizer.next();
		}

		// TODO(frostyfrog): Figure out why this was here again...
		mTokenizer.next();
	}
}

// Parse just the options (key/value)
std::vector<std::string> Parser::parseWatcherOptions(CfgWatch* watcher,
		std::string expectKey, std::string key) {

	// Setup our vector
	std::vector<std::string> values;

	// Return an empty vector if this is the wrong code block
	if( key != expectKey )
		return values;

	// Get rid of the equals
	parseEquals();

	// Parse out the value
	values = parseValue();

	// Debug: Print out all values
	printf("\t%s: ", key.c_str());
	for (auto value : values) {
		printf("[%s]", value.c_str());
	}
	printf("\n");

	// Return the values
	return values;
}

// Get the key for the key/value pair
const std::string& Parser::parseKey() {
	//printf("T: %d V: %s\n", mTokenizer.getToken(),
	//		mTokenizer.getValue().c_str());
	assertExp( Tokenizer::WORD );
	return mTokenizer.getValue();
}

// Skip the equals sign
void Parser::parseEquals() {
	bool found_equals = false;
	mTokenizer.next();

	// While we hit whitespace AND only encounter a single equals sign
	while( mTokenizer.getToken() == Tokenizer::WS ||
			(mTokenizer.getValue() == "=" &&
			!found_equals))
	{
		// We saw an equals sign, bail on next equals
		if( mTokenizer.getValue() == "=" )
			found_equals = true;
		mTokenizer.next();
	}
}

const std::vector<std::string> Parser::parseValue() {
	// Initialize the variables
	Tokenizer::TokenType t;
	std::vector<std::string> values;
	std::string valbuf;

	// Loop until the end of the value
	while( mTokenizer.getToken() != Tokenizer::NL &&
			mTokenizer.getValue() != ";" )
	{
		// Cache the token and check it's validity
		t = mTokenizer.getToken();
		//printf("V: %s\n", mTokenizer.getValue().c_str());
		if( t == Tokenizer::EOFTOK ||
				t == Tokenizer::INVALID ||
				t == Tokenizer::SPECIAL && !safeSpecial())
			throw std::runtime_error("Unknown exception while parsing config");


		//printf("V: %s\n", mTokenizer.getValue().c_str());
		// If we have a list, this should be handled by the tokenizer, not parser
		// TODO(frostyfrog): Implement this in the tokenizer
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

// Check that the special token should be considered "safe"
bool Parser::safeSpecial() {
	std::string v = mTokenizer.getValue();
	return ( v == "-" ||
			v == "_" ||
			v == ";" ||
			v == "," );
}

// Assert that we have a valid token, and not what we aren't expecting
bool Parser::assertExp(const Tokenizer::TokenType& token) {
	//printf("T: %d:%d V: %s:%s\n", token, mTokenizer.getToken(), value.c_str(),
	//		mTokenizer.getValue().c_str());
	if( mTokenizer.getToken() != token )
		throw std::runtime_error("Unknown exception while parsing config");
	return true;
}

// Assert that we have a valid token, and not what we aren't expecting
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
	// setup the lexer
	BLexicon lexer;
	mTokenizer.setLexer(lexer);

	// Loop until we hit the end of the file
	do {

		// Grab the next tokens
		mTokenizer.next();

		// Skip whitespace
		if( mTokenizer.getToken() == Tokenizer::WS ||
				mTokenizer.getToken() == Tokenizer::NL ||
				mTokenizer.getToken() == Tokenizer::EOFTOK )
			continue;

		// Error on a bad token
		if( mTokenizer.getToken() == Tokenizer::INVALID )
			throw std::runtime_error("Unknown exception while parsing config");

		// Get the current value
		//printf("Token: %s\n", toktostr(mTokenizer.getToken()).c_str());
		std::string value = mTokenizer.getValue();
		//printf("Value: %s [%d]\n", value.c_str(), value.size());

		// Lowercase the value
		LOWER(value);

		// Make sure we hit a watch block (only current block)
		// TODO(frostyfrog): Implement a config block (websocket reload?)
		if( value == "watch" ) {

			// Skip over the watch token
			mTokenizer.next();

			// Skip whitespace, error if no whitespace
			if( mTokenizer.getToken() != Tokenizer::WS )
				throw std::runtime_error("Unknown exception while parsing config");
			else
				mTokenizer.next();

			// Make sure that we can grab a word
			if( mTokenizer.getToken() != Tokenizer::WORD &&
					mTokenizer.getToken() != Tokenizer::STRING )
				throw std::runtime_error("Unknown exception while parsing config");

			// Create a watcher pointer and set the last one to the current one
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

			// Parse the configuration file
			this->parseWatcher(watcher);
		}
	} while( mTokenizer.getToken() != Tokenizer::EOFTOK &&
			mTokenizer.getToken() != Tokenizer::INVALID );
}

const CfgWatch* Parser::begin() const {
}

const CfgWatch* Parser::end() const {
}
