#pragma once
#include <vector>
#include "command.h"

extern enum e_component_type {
	unkown, mod_info
};

class component_t {
public:
	e_component_type type = unkown;
	component_t(const char* path);
	component_t() {};
	const char* path;

	std::vector<command> acts();

	static e_component_type ebt(std::string);
};