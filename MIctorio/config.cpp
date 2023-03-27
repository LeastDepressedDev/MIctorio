#include "config.h"
#include "fw_elem.h"
#include "index.h"
#include <fstream>
#include "hd.h"
#include <string>

std::map<std::string, std::string> gcfg::m_cfg = std::map<std::string, std::string>();
std::map<std::string, std::string> gcfg::def_set = { 
					  {"fstruct_name", "f:"},
					  {"rec_f",        "5"} };

bool gcfg::init() {
	std::ifstream f;
	f.open(CFG_FILE_NAME);
	if (f.is_open()) {
		f.close();
		gcfg::m_cfg = fw::read(CFG_FILE_NAME);
	}
	else {
		gcfg::def();
		return gcfg::init();
	}
	return true;
}

void gcfg::exit() {
	delete &gcfg::m_cfg;
}

void gcfg::update() {
	fw::upt(CFG_FILE_NAME, gcfg::m_cfg);
}

void gcfg::def() {
	fw::upt(CFG_FILE_NAME, gcfg::def_set);
}

void gcfg::recCall(std::string recpth) {
	if (![recpth]() {
		for (std::string ln : glob_app::recent) {
			if (ln == recpth) {
				return true;
			}
		}
		return false;
		}()) {
		if (glob_app::recent.size() == 0) {
			glob_app::recent.push_back(recpth);
		}
		else {
			for (size_t i = glob_app::recent.size() - 1; i > 0; i--) {
				glob_app::recent[i] = glob_app::recent[i - 1];
			}
			glob_app::recent[0] = recpth;
			for (int i = 0; i < glob_app::recent.size(); i++) {
				gcfg::m_cfg[std::string(RECENT) + std::to_string(i)] = glob_app::recent[i];
			}
		}
		gcfg::update();
	}
}