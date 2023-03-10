#pragma once
#include <iostream>
#include <map>

namespace fw {
	extern std::map<std::string, std::string> read(const char* path);
}