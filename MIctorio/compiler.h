#pragma once

#define INFO_FNAME "info.json"

#include <iostream>
#include "component.h"
#include <map>
#include <vector>

//namespace cmp_asset {
//	extern enum type {
//
//	};
//}

class compiler {
public:
	static std::map<e_component_type, std::string> links;
	static std::map<e_component_type, std::string> secNm;

	std::vector<component_t*> comps;
	compiler(std::string prj_path, std::vector<component_t*> elems);

	void compile();

private:
	component_t* getInfo();

	std::map<e_component_type, std::vector<std::string>> pred;
	std::vector<std::string> tec;

	void prepMap();

	void dataLua(std::vector<std::string>);
	void addLocale();

	void pushAll();
	void push(e_component_type, std::vector<std::string>);

	void compInfo(component_t*);
	void compCust(component_t*);
	void compItem(component_t*);

	std::string mod_name;
	
	std::string dpath;
	std::string inpath;
	std::string outpath;
	std::string spath;

	std::string outGrapgh;
	std::string outProc;
	std::string outLoc;
};