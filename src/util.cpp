#include <unistd.h>
#include <string>
#include <algorithm>
#include "exceptions.hpp"
#include "util.hpp"
#include <sys/stat.h>
#include <dirent.h>

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
		dirptr = opendir(path.c_str());
		std::vector<std::string> ret;

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
}
