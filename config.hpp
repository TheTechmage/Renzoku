/*
 * =====================================================================================
 *
 *       Filename:  config.hpp
 *
 *    Description:  Configuration file retrieval
 *
 *        Version:  1.0
 *        Created:  05/23/2015 10:51:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *
 * =====================================================================================
 */
#include <vector>
#include <string>

class Config {
	/* Config file format:
	 * project XXXXXX
	 * \tbuildcmd XXXX XXXX
	 * \truncmd XXXX
	 * #vim: set tw=8 sw=8 ai noet:
	 */
	public:
		std::vector<std::string> getProjects();
		std::string getBuildCommand(std::string project);
		std::string getTestCommand(std::string project);
		std::string getRunCommand(std::string project);
};
