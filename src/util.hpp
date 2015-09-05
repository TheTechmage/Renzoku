
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
	DirList listDir(std::string path, FileType ft);
	char** parseCommand(const std::string& command);
	bool strMatch(const std::string& expression, const std::string& str);
}

