#include <iostream>
#include <string>
#include <vector>

#include "index.h"
#include "inst.h"
#include "config.h"

std::string path_rend() {
	std::string line = "";
	if (PRJ.cur_prj != nullptr) {
		line += PRJ.cur_prj->name + ">";
	}
	return line + ">";
}

void wrap () {

}

int main() {
	gcfg::init();
	while (true)
	{
		
		std::string str;
		std::cin >> str;
		std::vector<std::string> cmd(0);
		std::string buf = "";
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == ' ') {
				cmd.push_back(buf);
				buf = "";
			}
			else {
				buf += str[i];
			}
		}
		cmd.push_back(buf);
		callCmd(cmd);
	}
}