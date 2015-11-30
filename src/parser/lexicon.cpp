#include "lexicon.hpp"
#include <string.h>
#include <stdio.h>

BLexicon::BLexicon() {
	memset(chars, SPECIAL, sizeof(chars));
	chars[0] = INVALID;
	for (int i = 1; i <= 31; i++) {
		chars[i] = CTRL;
	}
	chars[9] = WS;
	chars[10] = WS; // NL
	chars[13] = WS; // CR
	chars[32] = WS;

	// Special Characters
	char specials[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	for (int i = 0; i < strlen(specials); i++) {
		chars[(unsigned)specials[i]] = SPECIAL;
	}

	for (int i = 48; i < 58; i++) {
		chars[i] = NUMERIC;
	}
	for (int i = 65; i < 91; i++) {
		chars[i] = ALPHA;
	}
	chars['_'] = ALPHA;
	for (int i = 97; i < 122; i++) {
		chars[i] = ALPHA;
	}
}
BLexicon::lexicon_t BLexicon::getLexicon(char c) {
	//fprintf(stderr, "%u\n", (unsigned char)c);
	if((unsigned char)c > 255)
		return INVALID;
	return chars[(unsigned char)c];
}
BLexicon::~BLexicon() {
}
//	enum lexicon_t { CTRL, WS, ALPHA, NUMBERIC, SPECIAL, INVALID };
//	lexicon_t chars[256];
// UTF8 = just mark 128-255 as alpha
// http://zaemis.blogspot.com/2011/06/reading-unicode-utf-8-by-hand-in-c.html

UTF8Lexicon::UTF8Lexicon() :
	BLexicon()
{
	for (int i = 128; i < 256; i++) {
		chars[i] = ALPHA;
	}
}
UTF8Lexicon::~UTF8Lexicon() { }
