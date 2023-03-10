#pragma once
#include "command.h"
#include <vector>

const std::vector<command> CMDS = {
	command("help", "Help command", [](std::vector<std::string> cmd) {
		for (command c : CMDS) {
			std::cout << '.' << c.sId << " - " << c.sDesc << std::endl;
		}
	})
};

inline void callCmd(std::vector<std::string> cmd) {
	for (command c : CMDS) {
		if (c.sId == cmd[0]) {
			c.f(cmd);
		}
	}
}