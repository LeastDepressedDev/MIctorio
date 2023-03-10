#include <iostream>
#include <string>
#include <vector>

#include "inst.h"

int main() {
	while (true)
	{
		printf("%c", '.');
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