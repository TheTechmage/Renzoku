
#include <string>
#include <vector>

namespace Util
{
	typedef std::vector<std::string> DirList;
	enum FileType {
		ALL,
		DIRECTORY,
		FILETYPE
	};
	const std::string cwd();
	const std::string getCurrentDateTime();
	void lowercase(std::string&);
	std::string lowercase_r(const std::string&);
	bool ends_with(const char* str, const char* suffix);
	DirList listDir(const std::string& path, FileType ft);
}

