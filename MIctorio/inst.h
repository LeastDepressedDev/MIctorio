#pragma once
#define GEN_SEPARATOR '>'

#include "command.h"
#include "component.h"
#include "str_proc.h"
#include <vector>
#include <map>

extern enum e_cmd_section {
	mwind, prj, elem, general
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
	case e_cmd_section::elem:
		return "General element commands: ";
		break;
	case e_cmd_section::general:
		return "General commands(Usable in every section): ";
		break;
	default:
		return "";
		break;
	}
}

extern std::map<e_cmd_section, std::vector<command>> CMDS;

inline void callCmd(e_cmd_section sec, std::vector<std::string> cmd) {
	for (command c : CMDS[e_cmd_section::general]) {
		std::vector<std::string> als = vec_split(c.sId, '|');
		for (std::string a : als) {
			if (a == cmd[0]) {
				c.f(cmd);
				return;
			}
		}
	}
	std::vector<command> cmd_vec;
	if (sec == e_cmd_section::elem) {
		//if ()
		for (command c : CMDS[sec]) {
			std::vector<std::string> als = vec_split(c.sId, '|');
			for (std::string a : als) {
				if (a == cmd[0]) {
					c.f(cmd);
					return;
				}
			}
		}
		cmd_vec = component_t::cmdRs();
	}
	else {
		cmd_vec = CMDS[sec];
	}
	for (command c : cmd_vec) {
		std::vector<std::string> als = vec_split(c.sId, '|');
		for (std::string a : als) {
			if (a == cmd[0]) {
				c.f(cmd);
				return;
			}
		}
	}
	std::cout << "This command doesn't exists!" << std::endl;
}