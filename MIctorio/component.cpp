#include "component.h"

component_t::component_t(const char* path) {
	this->path = path;
}

e_component_type component_t::ebt(std::string sType) {
	if (sType == "info") {
		return e_component_type::mod_info;
	}
}