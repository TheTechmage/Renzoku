#include	"exceptions.hpp"

CException::CException(const std::string& message) :
	std::runtime_error(message + ": " + strerror(errno) + " (" + std::to_string(errno) + ")")
{
}
