#pragma once
#include <iostream>
#include <map>

namespace gcfg {
	extern std::map<std::string, std::string> m_cfg;

	extern bool init();
	extern void update();
	extern void exit();
}