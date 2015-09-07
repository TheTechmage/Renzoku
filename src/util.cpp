#include <unistd.h>
#include <string>
#include <algorithm>
#include "exceptions.hpp"
#include "util.hpp"
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

namespace Util
{
	const std::string getCurrentDateTime() {
		time_t now = time(NULL);
		struct tm time_struct;
		char buf[128];
		time_struct = *localtime(&now);

		// http://en.cppreference.com/w/cpp/chrono/c/strftime contains more
		// information about the precise formats
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &time_struct);

		return buf;
	}

	const std::string cwd() {
		char cwd[1024];

		if( getcwd(cwd, sizeof(cwd)) == NULL )
			throw CException("getcwd");

		return std::string(cwd);
	}

	void lowercase(std::string& input) {
		// http://stackoverflow.com/a/313990
		std::transform(input.begin(), input.end(), input.begin(), ::tolower);
	}

	std::string lowercase_r(const std::string& input) {
		// http://stackoverflow.com/a/313990
		std::string data = input;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	DirList listDir(std::string path, FileType ft) {
		struct dirent *entry;
		struct stat st;
		DIR *dirptr;
		std::vector<std::string> ret;
		std::cout << path << '\n';
		lstat(path.c_str(), &st);
		if(!S_ISDIR(st.st_mode))
			return ret;
		dirptr = opendir(path.c_str());

		if(dirptr == nullptr)
			throw CException("opendir");

		while((entry = readdir(dirptr))) {
			lstat(entry->d_name, &st);

			if(ft == DIRECTORY && S_ISDIR(st.st_mode)) {
				std::string str(entry->d_name);
				ret.push_back(str);
			}
			else if(ft == FILETYPE && !S_ISDIR(st.st_mode)) {
				std::string str(entry->d_name);
				ret.push_back(str);
			}
			else if(ft == ALL) {
				std::string str(entry->d_name);
				ret.push_back(str);
			}
		}
		closedir(dirptr);
		return ret;
	}

	char** parseCommand(const std::string& command)
	{
		// TODO: Write string parser for parsing out the command line arguments so
		// that we can push it through the C based functions.
		size_t position = 0;
		size_t tmppos = 0;
		bool inQuote = false;
		std::vector<std::string> args;
		while(position < command.length())
		{
			switch(command[position + tmppos])
			{
				case '"':
					inQuote = !inQuote;
					break;
				case ' ':
					if(inQuote)
						break;
					args.push_back(command.substr(position, tmppos));
					position += tmppos + 1;
					tmppos = 0;
					break;
				default:
					break;
			}
			tmppos++;
			if(position + tmppos >= command.size())
			{
				args.push_back(command.substr(position));
				break;
			}
		}
		char** newCommand = new char*[args.size() + 1];
		size_t i = 0;
		for(std::string &item : args)
		{
			newCommand[i] = new char[item.length()+1];
			newCommand[i][item.length()] = 0;
			strcpy(newCommand[i++], item.c_str());
		}
		newCommand[i] = NULL;
		return newCommand;
	}
	bool strMatch(const std::string& expression, const std::string& str)
	{
		size_t lenstr, lensuffix, asterisk, i;
		//std::cout << "Comparing " << str << " with " << expression << std::endl;
		bool ends_with, starts_with;
		std::string tmpstring;

		if(!expression.length() || !str.length()) {
			//std::cout << "Couldn't find length!\n";
			return false;
		}
		lenstr = str.length();
		lensuffix = expression.length();
		if(lensuffix > lenstr) {
			//std::cout << "suffix too big?!\n";
			return 0;
		}

		asterisk = -1;
		for(i = 0; i < expression.length(); i++)
		{
			if(expression[i] == '*')
			{
				asterisk = i;
			}
		}
		if(asterisk >= 0)
			tmpstring = expression.substr(asterisk + 1);
		else
			tmpstring = expression;

		//std::cout << tmpstring << "It does start!\n";
		ends_with = str.substr(str.length() - tmpstring.length()) == tmpstring;
		if(asterisk >= 0)
		{
			tmpstring = expression.substr(0, asterisk);
			starts_with = str.substr(0, tmpstring.length()) == tmpstring;
		}
		else
			starts_with = true;
		/*if(starts_with)
			std::cout << "It does start!\n";
			if(ends_with)
			std::cout << "it does end!\n";*/
		return starts_with && ends_with;
	}
}
