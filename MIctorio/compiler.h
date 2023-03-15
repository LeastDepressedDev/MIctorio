#pragma once

#define INFO_FNAME "info.json"

#include <iostream>
#include "component.h"
#include <vector>

//namespace cmp_asset {
//	extern enum type {
//
//	};
//}

class compiler {
public:
	std::vector<component_t*> comps;
	compiler(std::string prj_path, std::vector<component_t*> elems);

	void compile();

private:
	component_t* getInfo();

	void compInfo(component_t*);
	void compCust(component_t*);
	
	std::string dpath;
	std::string inpath;
	std::string outpath;
	std::string spath;

	std::string outGrapgh;
	std::string outProc;
	std::string outLoc;
};