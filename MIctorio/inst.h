#pragma once
#define GEN_SEPARATOR '>'

#include "command.h"
#include <vector>
#include <map>

extern enum e_cmd_section {
	mwind, prj, elem_info
};

inline std::string gensec(e_cmd_section sec) {
	switch (sec)
	{
	case e_cmd_section::mwind:
		return "Main window: ";
		break;
	case e_cmd_section::prj:
		return "Project window: ";
		break;
	case e_cmd_section::elem_info:
		return "Element info: ";
		break;
	default:
		return "";
		break;
	}
}

extern std::map<e_cmd_section, std::vector<command>> CMDS;

inline void callCmd(e_cmd_section sec, std::vector<std::string> cmd) {
	std::vector<command> cmd_vec = CMDS[sec];
	for (command c : cmd_vec) {
		if (c.sId == cmd[0]) {
			c.f(cmd);
			return;
		}
	}
	std::cout << "This command doesn't exists!" << std::endl;
}