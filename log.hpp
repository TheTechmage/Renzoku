/*
 * =====================================================================================
 *
 *       Filename:  log.hpp
 *
 *    Description:  Log to std handles and to files
 *
 *        Version:  1.0
 *        Created:  05/24/2015 12:37:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#pragma once
#include <string>

enum LogLevel {
	DEBUG,
	INFO,
	SUCCESS,
	WARNING,
	ERROR,
	CRITICAL
};

enum LogMethod {
	LOGFILE=1,
	STDHANDLE=2
};
inline LogMethod operator|(LogMethod a, LogMethod b)
{return static_cast<LogMethod>(static_cast<int>(a) | static_cast<int>(b));}
inline LogMethod operator&(LogMethod a, LogMethod b)
{return static_cast<LogMethod>(static_cast<int>(a) & static_cast<int>(b));}

class Logger {
	private:
		std::string mFile;
		LogMethod mMethod;
		Logger();
	public:
		static Logger* mLogger;
		static Logger* getLogger();
		static Logger* removeLogger();
		void log(LogLevel, std::string format, ...);
		void logCError(std::string message);
};
