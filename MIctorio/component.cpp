#include "component.h"

#include "index.h"

std::map<e_component_type, std::vector<command>> eSet = {
	{e_component_type::mod_info,
		{
			command("title", "sets title of your mod", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					std::string line;
					for (int i = 1; i < cmd.size(); i++) {
						line += cmd[i] + " ";
					}
					glob_app::cur_prj->fg_c->mParam["title"] = line;
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			})
		}
	}
};

std::map<std::string, e_component_type> nameLinker = {
	{"info", e_component_type::mod_info},
	{"cust", e_component_type::custom}
};

component_t::component_t(std::string path, std::string str) {
	this->mParam = std::map<std::string, std::string>();
	this->path = path;
	this->name = str;
}

e_component_type component_t::ebt(std::string sType) {
	return nameLinker[sType];
}

std::string component_t::tte(e_component_type e) {
	for (std::pair<std::string, e_component_type> pr : nameLinker) {
		if (pr.second == e) {
			return pr.first;
		}
	}
	return NULL;
}

std::vector<command> component_t::cmdRs() {
	return eSet[glob_app::cur_prj->fg_c->type];
}