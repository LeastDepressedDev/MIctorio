#include <iostream>
#include <Windows.h>
#include <string>

#include "index.h"
#include "inst.h"
#include "config.h"
#include "str_proc.h"
#include <fcntl.h>
#include <io.h>

#include "hd.h"

Project* glob_app::cur_prj = new Project();
e_cmd_section glob_app::stage = e_cmd_section::mwind;
std::vector<std::string> glob_app::recent = std::vector<std::string>(0);

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

void wrap () {
	size_t rec = std::stoi(gcfg::m_cfg["rec_f"]);
	for (size_t i = 0; i < rec; i++) {
		std::string lk = gcfg::m_cfg[std::string(RECENT) + std::to_string(i)];
		if (lk.size() > 0) {
			glob_app::recent.push_back(lk);
		}
	}
}

int classic() {
	gcfg::init();
	wrap();
	system(PROG_CLR);
	std::cout << WELCOME_MSG << std::endl;
	while (true)
	{
		std::cout << path_rend();
		std::string src = str_in();
		callCmd(glob_app::stage, vec_split(src, ' '));
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdln, int cmdOf) {
	int sz;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &sz);

	std::vector<std::string> args(0);

	for (int i = 0; i < sz; i++) {
		args.push_back(std::string((const char*) argv[i]));
	}

	std::vector<std::string> gs = { "-c", "-classic" };
	if (true/*vec_cont<std::string>(args, gs)*/) {
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		for (std::string s : args) std::cout << s << std::endl;
		return classic();
	}
	else {
		//there will be gui
	}
}

//int main() {
//	return classic();
//}