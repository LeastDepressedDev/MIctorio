#pragma once

#define COMPONENT_FILE_EXTENSION "cmp"

#include <vector>
#include <iostream>
#include "command.h"
#include "DataRaw.h"
#include <map>

extern std::map<e_component_type, std::vector<command>> eSet;

extern std::map<std::string, e_component_type> nameLinker;

class component_t {
public:
	e_component_type type = unkown;
	component_t(std::string path, std::string name);
	component_t() {};
	std::string path;
	std::string name;

	std::map<std::string, std::string> mParam = std::map<std::string, std::string>();

	static std::vector<command> cmdRs();

	static e_component_type ebt(std::string);
	static std::string tte(e_component_type);
};