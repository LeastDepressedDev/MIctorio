#include <iostream>
#include <string>

#include "index.h"
#include "inst.h"
#include "config.h"

#include "hd.h"

Project* glob_app::cur_prj = new Project();
e_cmd_section glob_app::stage = e_cmd_section::mwind;

std::string path_rend() {
	std::string line = "";
	if (glob_app::cur_prj->isOpen()) {
		line += glob_app::cur_prj->name;
	}
	if (glob_app::cur_prj->fg_c != nullptr) {
		line += GEN_SEPARATOR + glob_app::cur_prj->fg_c->name;
	}
	return line + GEN_SEPARATOR;
}

void wrap () {}

int main() {
	std::cout << WELCOME_MSG << std::endl;
	gcfg::init();
	while (true)
	{
		std::cout << path_rend();
		std::string src = str_in();
		callCmd(glob_app::stage, vec_split(src, ' '));
	}
}