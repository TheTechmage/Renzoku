#pragma once

class BLexicon
{
public:
	BLexicon();
	virtual ~BLexicon();
	enum lexicon_t { CTRL, WS, ALPHA, NUMERIC, SPECIAL, INVALID };
	lexicon_t getLexicon(char c);

protected:
	lexicon_t chars[256];

};

class UTF8Lexicon : public BLexicon
{
public:
	UTF8Lexicon();
	virtual ~UTF8Lexicon();
};
