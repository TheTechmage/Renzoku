#include <fstream>
#include <stdio.h>
#include <libconfig.h++>
#include <iostream>

using namespace libconfig;

int main(int argc, char *argv[])
{
	printf("test\n");
	/*std::ifstream infile("test.conf");
	// http://stackoverflow.com/questions/7868936/read-file-line-by-line
	std::string line;
	while(std::getline(infile, line))
		printf("%s\n", line.c_str());*/
	// http://stackoverflow.com/questions/4823177/reading-a-file-character-by-character-in-c
	FILE* file = fopen("src/config/test.conf", "r");
	char* code;
	size_t n = 0;
	int c;
	if (file == NULL)
		return 1;
	while((c = fgetc(file)) != EOF)
		printf("%c", (char)c);
	fclose(file);
	Config cfg;
	try {
		cfg.readFile("src/config/test.conf");
	} catch(const FileIOException &fioex) {
		std::cerr << "I/O error while reading file." << std::endl;
		return(EXIT_FAILURE);
	} catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return(EXIT_FAILURE);
	}
	if (cfg.exists("watcher") && cfg.lookup("watcher").isGroup()) {
		printf("Watcher Exists! ^.^\n");
		const Setting &watcher = cfg.lookup("watcher");
		for (auto it = watcher.begin(); it != watcher.end(); ++it) {
			printf("%s\n", it->getName());

		}
	}
	return 0;
}
