#include <configvalue.hpp>
#include <string>

template <class T>
ConfigItem::ConfigItem(std::string key, T value) {
	mKey = key;
	mValue = value;
}

template <class T>
T ConfigItem::getValue() const {
	return mValue;
}

template <class T>
string ConfigItem::getKey() const {
	std::
	return mKey;
}
