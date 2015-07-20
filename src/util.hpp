
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
	DirList listDir(std::string path, FileType ft);
}

