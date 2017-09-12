#include <cstdio>
#include <string.h>
#include "lexicon.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"

int main(int argc, char *argv[])
{
	char str[] = "Rawr! I'm a monster!\nれんぞく。　日本語。\n１人二人2人\n";
	printf("Original String: <<EOF\n%s\nEOF\n", str);
	int i = 0;
	printf("Lexed String: <<EOF\n");
	BLexicon lexer;
	for(i = 0; i < strlen(str); i++) {
		BLexicon::lexicon_t lex = lexer.getLexicon(str[i]);
		switch (lex) {
			case BLexicon::ALPHA:
				printf("%c[ALPHA] ", str[i]);
				break;
			case BLexicon::WS:
				printf("%c[WS] ", str[i]);
				break;
			case BLexicon::NUMERIC:
				printf("%c[NUMERIC] ", str[i]);
				break;
			default:
				printf("%c", str[i]);
		}
	}
	printf("\nEOF\n");
	printf("\n%d - %d\n", i - 7, 10);
	// UTF8 = just mark 128-255 as alpha
	Tokenizer t("test.conf");
	t.setLexer(lexer);
	do {
		t.next();
		if( t.getToken() != Tokenizer::WS && t.getToken() != Tokenizer::NL )
			printf("T:%s | V:%s\n", toktostr(t.getToken()).c_str(), t.getValue().c_str());
	} while( t.getToken() != Tokenizer::EOFTOK && t.getToken() != Tokenizer::INVALID );
	printf("\n");
	Parser p("test.conf");
	p.Parse();
	printf("\nEOF\n");
	printf("\nFrom main.c:\n");
	//const CfgWatch* watchers = p.getWatchers();
	//CfgWatchIter iter;
	CfgStep* step;
	for (auto i : p) {
		printf("[%s]\n", (*i).name);
		step = i->steps;
		//printf("%d\n", step->next);
		while(step) {
			printf("\t%s\n", step->name);
			printf("\t\t%s\n", step->command);
			printf("\t\t%s\n", step->enabled ? "true" : "false");
			step = step->next;
		}
	}
	return 0;
}
