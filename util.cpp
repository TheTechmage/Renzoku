#include <unistd.h>
#include <string>
#include "exceptions.hpp"
#include "util.hpp"
#include <sys/stat.h>
#include <dirent.h>

namespace Util
{
	std::string cwd() {
		char cwd[1024];
		if( getcwd(cwd, sizeof(cwd)) == NULL )
			throw CException("getcwd");
		return std::string(cwd);
	}
	DirList listDir(std::string path, FileType ft) {
		struct dirent *entry;
		struct stat st;
		DIR *dirptr;
		dirptr = opendir(path.c_str());
		if(dirptr == nullptr)
			throw CException("opendir");
		std::vector<std::string> ret;
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
