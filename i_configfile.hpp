#pragma once
#include "configitem.hpp"
#include <string>

class iConfigFile {
	public:
		virtual ~iConfigFile() = 0;
		iConfigFile(std::string file) {mFileName = file;};
		virtual bool readConfig() = 0;

		virtual ConfigItem* getItem(string key) = 0;
	private:
		std::string mFileName;
};
