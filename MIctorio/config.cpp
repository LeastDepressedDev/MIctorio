#include "config.h"
#include "fw_elem.h"
#include <fstream>

#define CFG_FILE_NAME "index.cfg"

bool gcfg::init() {
	gcfg::m_cfg = fw::read(CFG_FILE_NAME);
}

void gcfg::exit() {
	delete &gcfg::m_cfg;
}

void gcfg::update() {
	std::ofstream of;
	of.open(CFG_FILE_NAME);
	std::string lines;
	for (std::pair<std::string, std::string> pr : gcfg::m_cfg) {
		lines += pr.first + "@" + pr.second + ";\n";
	}

	of.write(lines.c_str(), sizeof(lines));
}