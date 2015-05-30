#include <unistd.h>
#include <string>
#include	"exceptions.hpp"

namespace Util
{
	std::string cwd() {
		char cwd[1024];
		if( getcwd(cwd, sizeof(cwd)) == NULL )
			throw CException("getcwd");
		return std::string(cwd);
	}
}
