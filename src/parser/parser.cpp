#include "parser.hpp"
#include "tokenizer.hpp"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>

#define LOWER(x) std::transform(x.begin(), x.end(), x.begin(),\
		::tolower);

CfgStep* create_CfgStep() {
	CfgStep* step = new CfgStep;
	step->name = NULL;
	step->command = NULL;
	step->daemon = false;
	step->enabled = true;
	step->error_status = NULL;
	step->ignore_status = NULL;
	step->next = NULL;
	return step;
}

CfgWatch* create_CfgWatch() {
	CfgWatch* watcher = new CfgWatch;
	watcher->name = NULL;
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

template <class T>
bool str_to_type(T& t, const std::string& s) {
	std::istringstream iss(s);
	return !(iss >> t).fail();
}

Parser::Parser(std::istream& file) :
	mFilename("<BUFFER>"),
	mTokenizer(mFilename, file),
	mWatchers(NULL),
	mLastWatcher(NULL)
{
}

Parser::~Parser() {
	// TODO(frostyfrog): Cleanup
	if( mWatchers != NULL ) {
		CfgWatch *watcher = mWatchers;
		CfgWatch *tmpwatcher = watcher->next;
		CfgStep *step = NULL;
		CfgStep *tmpstep = NULL;
		while( watcher ) {
			step = watcher->steps;
			tmpwatcher = watcher->next;
			delete [] watcher->name;
			while( step ) {
				tmpstep = step->next;
				delete [] step->name;
				delete [] step->command;
				delete step;
				step = tmpstep;
			}
			delete watcher;
			watcher = tmpwatcher;
		}
	}
}

char* Parser::mStrToChar(const std::string& str) {
	char* retval = new char[str.length()+1];
	retval[str.length()] = '\0';

	// Convert string to char array
	for (size_t i = 0; i < str.length(); i++) {
		retval[i] = str[i];
	}
	return retval;
}

void Parser::parseWatcher(CfgWatch* watcher) {
	// Initialize local variables
	std::string name = mTokenizer.getValue();
	watcher->name = mStrToChar(name);

	// TODO(frostyfrog): Finish this

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
		if( mIsKey("files", key) )
			watcher->filesFilter = parseWatcherOptions(watcher, key);
		if( mIsKey("exclude", key) )
			watcher->excludesFilter = parseWatcherOptions(watcher, key);
		if ( mIsKey("dir", key) )
			watcher->workingDir = mStrToChar(parseWatcherOptions(watcher, key).at(0)); // Can throw std::out_of_range

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
			CfgStep* step = create_CfgStep();
			step->name = mStrToChar(name_step);

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
					//printf("\t");
					if (mIsKey("enabled", key))
					step->enabled = parseBooleanOption(watcher, key);
					if (mIsKey("command", key))
					{
						step->command = parseWatcherOption(watcher, key);
					}
					// XXX: For now, we skip these two
					if (mIsKey("error_status", key))
					parseWatcherOptions(watcher, key);
					// parseStatusCodes (parse options retval)
					// mTokenizer.invokeError(msg);
					if (mIsKey("ignore_status", key))
					parseWatcherOptions(watcher, key);
				}
				mTokenizer.next();
			}

			// Skip the }
			mTokenizer.next();
			if (!watcher->steps) {
				watcher->steps = step;
			} else {
				CfgStep* tmpstep = watcher->steps;
				while(tmpstep->next) {
					tmpstep = tmpstep->next;
				}
				tmpstep->next = step;
			}
		}

		// TODO(frostyfrog): Figure out why this was here again...
		mTokenizer.next();
	}
}

// Determine if we have the right key
bool Parser::mIsKey(const std::string& expectKey, const std::string& key) {
	//mTokenizer.next();
	//printf("(%s != %s) == %d\n", key.c_str(), expectKey.c_str(), ( key != expectKey ));
	return ( key == expectKey );
}

// Parse out boolean values
bool Parser::parseBooleanOption(CfgWatch* watcher, std::string key) {

	// Get rid of the equals
	parseEquals();

	// Parse out the value
	const char value = parseBoolean();
	bool retval;
	retval = value == 'y';

	// Debug: Print out all values
	printf("\t\t%s: %c", key.c_str(), value);
	printf("\n");

	// Return the values
	return retval;
}

char* Parser::parseWatcherOption(CfgWatch* watcher, std::string key) {

	// Get rid of the equals
	parseEquals();

	// Parse out the value
	char* values = mStrToChar(parseString());

	// Debug: Print out all values
	printf("\t\t%s: %s", key.c_str(), values);
	printf("\n");

	// Return the values
	return values;
}

// Parse just the options (key/value)
const std::vector<std::string> Parser::parseWatcherOptions(CfgWatch* watcher,
		std::string key) {

	// Setup our vector
	std::vector<std::string> values;

	// Get rid of the equals
	parseEquals();

	// Parse out the value
	values = parseValue();

	// Debug: Print out all values
	printf("\t\t%s: ", key.c_str());
	for (auto value : values) {
		printf("[%s]", value.c_str());
	}
	printf("\n");

	// Return the values
	return values;
}

// Parse the string list of values and convert them into short numbers
short* Parser::parseStatusCodes(std::vector<std::string>& strs) {
	size_t pos = 0;
	short tmp;
	short* retval = NULL;
	std::vector<short> builder;
	for (auto s : strs) {
		pos = s.find("-");
		if( pos != (size_t)-1 ) {
			// TODO: Parse and count all numbers in range
		} else {
			if( str_to_type( tmp, s ) ) {
				builder.push_back(tmp);
			} else {
				std::string msg = "Unable to find number in '" + s + "'";
				mTokenizer.invokeError(msg);
			}
		}
	}
	if( builder.size() > 0 ) {
		pos = 0;
		retval = new short[builder.size() + 1];
		for (auto i : builder) {
			retval[pos++] = i;
		}
	}
	return retval;
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
		if( t == Tokenizer::EOFTOK ||
				t == Tokenizer::INVALID ||
				t == Tokenizer::SPECIAL && !safeSpecial())
			mTokenizer.invokeError("[e1] Unexpected character");


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

const std::string Parser::parseString() {
	// Initialize the variables
	std::string retval;

	// Loop until the end of the value
	if( mTokenizer.getToken() == Tokenizer::WS )
		mTokenizer.next();
	if( mTokenizer.getToken() != Tokenizer::STRING )
		mTokenizer.invokeError("[e2] Unexpected character");
	//printf("\tT: %d V: %s\n", mTokenizer.getToken(),
	//		mTokenizer.getValue().c_str());
	retval = mTokenizer.getValue();
	mTokenizer.next();
	if( mTokenizer.getToken() == Tokenizer::EOFTOK ||
			mTokenizer.getToken() == Tokenizer::INVALID ||
			mTokenizer.getToken() == Tokenizer::SPECIAL && !safeSpecial())
		mTokenizer.invokeError("[e3] Unexpected character");
	return retval;
}

const char Parser::parseBoolean() {
	// Initialize the variables
	char retval;

	// Loop until the end of the value
	if( mTokenizer.getToken() == Tokenizer::WS )
		mTokenizer.next();
	if( mTokenizer.getToken() != Tokenizer::BOOLEAN )
		mTokenizer.invokeError("[e4] Unexpected character");
	retval = mTokenizer.getValue().c_str()[0];
	mTokenizer.next();
	if( mTokenizer.getValue() != ";" && (
			mTokenizer.getToken() == Tokenizer::EOFTOK ||
			mTokenizer.getToken() == Tokenizer::INVALID ||
			mTokenizer.getToken() == Tokenizer::SPECIAL && !safeSpecial()))
		mTokenizer.invokeError("[e5] Unexpected character");
	return retval;
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
	if( mTokenizer.getToken() != token ) {
		std::string token_str = toktostr(token);
		std::string token_str2 = toktostr(mTokenizer.getToken());
		std::string msg = "Expected " + token_str + ", got " + token_str2;
		mTokenizer.invokeError(msg);
		//throw std::runtime_error("Unknown exception while parsing config");
	}
	return true;
}

// Assert that we have a valid token, and not what we aren't expecting
bool Parser::assertExp(const Tokenizer::TokenType& token, std::string value) {
	//printf("T: %d:%d V: %s:%s\n", token, mTokenizer.getToken(), value.c_str(),
	//		mTokenizer.getValue().c_str());
	if( mTokenizer.getToken() != token ) {
		std::string token_str = toktostr(token);
		std::string token_str2 = toktostr(mTokenizer.getToken());
		std::string msg = "Expected " + token_str + ", got " + token_str2;
		mTokenizer.invokeError(msg);
		//throw std::runtime_error("Unknown exception while parsing config");
	}
	if( mTokenizer.getValue() != value ) {
		std::string msg = "Expected \"" + value + "\", got " +
			mTokenizer.getValue();
		mTokenizer.invokeError(msg);
		throw std::runtime_error("Unknown exception while parsing config");
	}
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
				mLastWatcher->next = watcher;
				mLastWatcher = watcher;
			}
			watcher = mLastWatcher;

			// Parse the configuration file
			this->parseWatcher(watcher);
		}
	} while( mTokenizer.getToken() != Tokenizer::EOFTOK &&
			mTokenizer.getToken() != Tokenizer::INVALID );
}

CfgWatchIter Parser::begin() const {
	return CfgWatchIter(mWatchers);
}

CfgWatchIter Parser::end() const {
	return CfgWatchIter();
}
