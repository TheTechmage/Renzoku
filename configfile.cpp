#include "configfile.hpp"

ConfigFile::~ConfigFile()
{
	for (std::map<std::string, ConfigItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
	{
		if(it->second == nullptr)
			continue;
		delete it->second;
		it->second = nullptr;
	}
}

ConfigFile::ConfigFile(std::string file)
	: iConfigFile(file)
{
}

ConfigValue* ConfigFile::getItem(std::string key) {
	std::iterator it = mItems.find(key);
	if(it == mItems.end())
		return NULL;
	return it->second;
}
