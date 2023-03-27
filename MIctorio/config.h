#pragma once

#define CFG_FILE_NAME "index.cfg"

#include <iostream>
#include <map>

namespace gcfg {
	extern std::map<std::string, std::string> def_set;
	extern std::map<std::string, std::string> m_cfg;

	extern bool init();
	extern void update();
	extern void exit();
	extern void def();
	
	extern void recCall(std::string);
}