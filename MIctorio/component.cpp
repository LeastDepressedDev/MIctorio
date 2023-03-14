#include "component.h"

#include "index.h"

std::map<e_component_type, std::vector<command>> eSet = {
	{e_component_type::mod_info,
		{
			command("title", "title", [](std::vector<std::string> cmd) {
				
			})
		}
	}
};

std::map<std::string, e_component_type> nameLinker = {
	{"info", e_component_type::mod_info},
	{"cust", e_component_type::custom}
};

component_t::component_t(std::string path, std::string str) {
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