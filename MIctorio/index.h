#pragma once
#include <iostream>
#include "inst.h"
#include <vector>
#include "Project.h"

namespace glob_app {
	extern Project* cur_prj;
	extern e_cmd_section stage;

	extern std::vector<std::string> recent;

	inline struct {
		std::string title = "MIctorio";
		std::string ver = "0.1a";
		std::string author = "Sirtage(aka qigan)";
	} INC;
}