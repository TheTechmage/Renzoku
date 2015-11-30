/*
 * Renzoku - Re-build, re-test, and re-run a program whenever the code changes
 * Copyright (C) 2015  Colton Wolkins
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
/*
 * =====================================================================================
 *
 *       Filename:  tokenizer.cpp
 *
 *    Description:  Token Parser for config files
 *
 *        Version:  1.0
 *        Created:  05/22/2015 01:37:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:
 *
 * =====================================================================================
 */
#include "tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <cerrno>

#define THROWPARSE(MESSAGE) throw ParserException(MESSAGE, mFilename, \
		mCurrentLine, mCurrentColumn)

Tokenizer::Tokenizer(std::string filename) :
		mCurrentChar(0),
		mCurrentLine(0),
		mCurrentColumn(0)
{
	std::ifstream* file = new std::ifstream();
	//file->exceptions( std::ifstream::failbit | std::ifstream::badbit );
	mFilename = filename;
	file->open(filename.c_str(), std::fstream::in);
	/* This code section is not needed due to the fact that we turned exceptions
	 * on for the file stream.
	if(!file.good()) {
		//fprintf(stderr, "Unable to open %s: %s\n", filename.c_str(), perror(errno));
		std::string error = "Unable to open config file '" + filename + "'";
		perror(error.c_str());
		return;
	}// */

	// Code for dumping file
	//char c = file.get();
	//while (file.good()) {
	//	std::cout << c;
	//	c = file.get();
	//}
	mFile = file;
}
const Tokenizer::TokenType Tokenizer::getToken() const
{
	return mToken;
}
char Tokenizer::nextChar()
{
		mCurrentChar++;
		mCurrentColumn++;
		char c = mFile->get();
		if( c == '\n' || c == '\r' ) {
			mCurrentLine++;
			mCurrentColumn = 0;
		}
		return c;
}
void Tokenizer::next()
{
	char c;
	BLexicon::lexicon_t lexicon;
	BLexicon::lexicon_t oldlexicon;
	mValue = "";
	mToken = INVALID;

	c = mFile->peek();
	if( c == EOF ) {// EOF reached
		mToken = EOFTOK;
		return;
	}
	lexicon = mLexer->getLexicon(c);
	if( lexicon == BLexicon::WS && c == ' ' ) {
		do {
			nextChar(); // Eat the space
			c = mFile->peek();
		} while( c == ' ' );
		mToken = WS;
	}
	else if( lexicon == BLexicon::WS ) {
		do {
			nextChar(); // Eat the space
			c = mFile->peek();
		} while( c != EOF && mLexer->getLexicon(c) == BLexicon::WS );
		mToken = NL;
	}
	else if( lexicon == BLexicon::ALPHA || lexicon == BLexicon::NUMERIC ) {
		oldlexicon = lexicon;
		do {
			mValue += nextChar();
			c = mFile->peek();
			lexicon = mLexer->getLexicon(c);
		} while( c != EOF && lexicon == oldlexicon );
		mToken = WORD;
	}
	else if( lexicon == BLexicon::SPECIAL ) {
		if( c == '"' || c == '\'' ) {
			//mFile->exceptions( std::ifstream::failbit & std::ifstream::badbit );
			mToken = STRING;
			char quotemarker = nextChar();
			do {
				mValue += nextChar();
				c = mFile->peek();
			} while( mFile->good() && c != EOF && c != quotemarker );
			if( c == quotemarker && mFile->good())
				nextChar();
			if( !mFile->good() )
				THROWPARSE("Failed to find end of string");
			//mFile->exceptions( std::ifstream::failbit | std::ifstream::badbit );
		} else {
			mValue = nextChar();
			mToken = SPECIAL;
		}
	}
	else {
		return;
	}
}

void Tokenizer::setLexer(BLexicon& lexer) {
	mLexer = &lexer;
}

const std::string& Tokenizer::getValue() const
{
	return mValue;
}
