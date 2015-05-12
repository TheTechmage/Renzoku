#pragma once
#include <string>

template <class T>
class ConfigItem {
	public:
		~ConfigItem();
		ConfigItem(std::string, T);
		std::string getKey() const;
		T getValue() const;
	private:
		ConfigItem* mParent;
		std::string mKey;
		T mValue;
};
