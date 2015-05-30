#include <string>
#include <stdexcept>
#include <cstring>

class CException : public std::runtime_error
{
	public:
		CException(const std::string& message); // strerror(errno)
};
