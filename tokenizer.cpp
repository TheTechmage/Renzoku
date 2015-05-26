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

Token::Token(TokenType type, std::string value) :
	mValue(value),
	mType(type)
{
}

TokenType Token::getType() const
{
	return mType;
}

std::string Token::getValue() const
{
	return mValue;
}

Tokenizer::Tokenizer()
{
}
Tokenizer::Tokenizer(std::istream& file)
{
}
Token Tokenizer::getToken()
{
	return Token(ERROR, "testing!");
}
std::string Tokenizer::next()
{
	return "RAWR";
}
