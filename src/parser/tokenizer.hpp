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
 *       Filename:  tokenizer.hpp
 *
 *    Description:  Token Parser header file
 *
 *        Version:  1.0
 *        Created:  05/22/2015 01:38:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#pragma once

#include <string>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <lexicon.hpp>
#include <stdexcept>

class ParserException : public std::runtime_error {
	private:
		std::string buildMessage( const std::string& what,
				const std::string& filename,
				unsigned long linenum,
				unsigned long column,
				const std::string& line = "") {
			std::string message = "";
			message = what;
			message += ". File ";
			message += filename;
			message += ". Line ";
			message += std::to_string(linenum);
			message += " character ";
			message += std::to_string(column);
			message += ".";
			if(!line.empty()) {
				std::string line_replacement = line;
				std::replace( line_replacement.begin(), line_replacement.end(), '\t', ' ');
				message += "\n\r";
				message += line_replacement;
				message += "\n\r";
				message.insert( message.size(), column - 1, ' ');
				message += "\e[0;31m^\e[0m Parse error starts from this position";
			}
			return message;
		}
	public:
		explicit ParserException( const std::string& what, const std::string&
				filename, size_t linenum, size_t column, const std::string& line ) :
			std::runtime_error(buildMessage(what, filename, linenum, column, line))
		{
		}
};

class Tokenizer {
	public:
		enum TokenType {
			INVALID=0,
			WORD,
			BOOLEAN,
			NUMBER,
			LIST,
			STRING,
			WS,
			NL,
			EOFTOK,
			SPECIAL
		};

	private:
		size_t mCurrentChar;
		size_t mCurrentLine;
		size_t mCurrentColumn;
		std::string mValue;
		TokenType	mToken;
		BLexicon* mLexer;
		std::string mFilename;
		std::ifstream* mFile;
		char nextChar();

	public:
		Tokenizer(std::string filename);
		virtual ~Tokenizer();
		void setLexer(BLexicon&);
		void next();
		const TokenType getToken() const;
		const std::string& getValue() const;
		const std::string getCurrentLine(size_t&, size_t&);
		const size_t getLinePosition() const { return mCurrentLine; }
		const size_t getColumnPosition() const { return mCurrentColumn; }
		const size_t getFilePosition() const { return mCurrentChar; }
};

class DummyTokenizer : public Tokenizer {
	public:
		DummyTokenizer(std::string str);
};
