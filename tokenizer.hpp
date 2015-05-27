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

enum TokenType {
	PROJECT,
	INT,
	FLOAT,
	OPERATOR,
	GROUPING_SYMBOL,
	STRING,
	COMMENT,
	SPACE,
	ERROR
};

class Token {
	private:
		TokenType mType;
		std::string mValue;
	public:
		Token(TokenType, std::string);
		TokenType getType() const;
		std::string getValue() const;
};

class Tokenizer {
	private:
		char getNextChar();
		unsigned int currentChar;

	public:
		Tokenizer();
		Tokenizer(std::istream&);
		Token getToken();
		std::string next();
		/*
		static const char COMMENTCHAR = '#';
		static const char INDENTATIONCHAR = '\t';
		static const char* CMDBUILD = "buildcmd";
		static const char* CMDRUN = "runcmd";
		static const char* CMDPROJECT = "project";
		const enum item_type = {
			project_name,
			build_command,
			run_command,
			file_mask,
		};
		static struct {
			item_type Type;
			char* Buffer;
		} Item;
		*/
};
