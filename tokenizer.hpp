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
