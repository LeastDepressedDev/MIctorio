#include <iostream>
#include <string>
#include <vector>

#include "index.h"
#include "config.h"

#define WELCOME_MSG "######################################\n#       Mod Interface For Factorio   #\n#    by Sirtage(qigan)               #\n#          Made in Russia            #\n######################################\n Type >help to see for commands."

Project* glob_app::cur_prj = nullptr;
e_cmd_section glob_app::stage = e_cmd_section::mwind;

std::string path_rend() {
	std::string line = "";
	if (glob_app::cur_prj != nullptr) {
		line += glob_app::cur_prj->name;
	}
	return line + GEN_SEPARATOR;
}

void wrap () {

}

std::vector<std::string> vec_split(std::string str, char sep) {
	std::vector<std::string> vec(0);
	std::string buf = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == sep) {
			vec.push_back(buf);
			buf = "";
		}
		else {
			buf += str[i];
		}
	}
	vec.push_back(buf);
	return vec;
}

std::string in() {
	std::string buf = "";
	char a = '0';
	while (a != '\n')
	{
		scanf_s("%c", &a);
		buf += a;
	}
	return buf.substr(1, buf.length() - 2);
}

int main() {
	std::cout << WELCOME_MSG << std::endl;
	gcfg::init();
	while (true)
	{
		std::cout << path_rend();
		std::string src = in();
		callCmd(glob_app::stage, vec_split(src, ' '));
	}
}