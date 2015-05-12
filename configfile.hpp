#pragma once
#include <map>
#include <string>
#include "i_configfile.hpp"
#include "configitem.hpp"

class ConfigFile : public iConfigFile {
	public:
		~ConfigFile();
		ConfigFile(std::string);
	private:
		std::map<std::string, ConfigItem*> mItems;
}
