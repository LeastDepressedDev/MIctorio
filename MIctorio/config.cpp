#include "config.h"
#include "fw_elem.h"
#include <fstream>

std::map<std::string, std::string> gcfg::m_cfg = std::map<std::string, std::string>();
std::map<std::string, std::string> gcfg::def_set = std::map<std::string, std::string>();

bool gcfg::init() {
	gcfg::def_set = { {"fstruct_name", "f:"},
				      {"rec_f",        "5"} };
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
	fw::upt(CFG_FILE_NAME, { {} });
}