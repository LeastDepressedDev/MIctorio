#include "compiler.h"

#include <direct.h>
#include "Project.h"
#include "fw_elem.h"
#include <fstream>

compiler::compiler(std::string prj_path, std::vector<component_t*> elems) {
	this->dpath = prj_path;
	this->inpath = prj_path + SRC_DNAME;
	this->spath = this->inpath + SPRITES_DNAME;
	this->comps = elems;
}

component_t* compiler::getInfo() {
	for (component_t* comp : this->comps) {
		if (comp->type == e_component_type::mod_info) {
			return comp;
		}
	}
	return nullptr;
}

void compiler::compile() {
	component_t* comp = this->getInfo();
	if (comp == nullptr) {
		printf("Info file not found. Compiling canceled!\n");
		return;
	}
	std::string nm = comp->mParam["name"];
	int c = comp->mParam["version"].rfind('.');
	for (int i = 0; i < 2 - c; i++) {
		comp->mParam["version"] += ".0";
	}
	nm += "_" + comp->mParam["version"];
	this->outpath = this->dpath + nm;

	_mkdir(this->outpath.c_str());
	this->outGrapgh = (this->outpath + "/grs");
	this->outLoc = (this->outpath + "/locale");
	this->outProc = (this->outpath + "/prototypes");
	_mkdir(outGrapgh.c_str());
	_mkdir(outLoc.c_str());
	_mkdir(outProc.c_str());

	printf("Compiling %d elements: \n", this->comps.size());
	for (component_t* cmp : this->comps) {
		printf("Compiling %s: ", cmp->name.c_str());
		if ([this, cmp]() {
			switch (cmp->type)
			{
			case e_component_type::mod_info:
				this->compInfo(cmp);
				return true;
			case e_component_type::custom:
				this->compCust(cmp);
				return true;
			default:
				return false;
			}
			}()) {
			printf("success.\n");
		}
		else {
			printf("error.\n");
		}
	}
}

bool cont(std::map<std::string, std::string> mp, std::string key) {
	for (std::pair<std::string, std::string> pr : mp) {
		if (pr.first == key) {
			return true;
		}
	}
	return false;
}

void compiler::compInfo(component_t* comp) {
	std::ofstream of;
	of.open(this->outpath + "/" + INFO_FNAME);
	std::map<std::string, std::string> copy = comp->mParam;
	of << "{\n";
	int c = 0;
	for (std::pair<std::string, std::string> pr : copy) {
		c++;
		std::string out = "    \"" + pr.first + "\": " + "\"" + pr.second + "\",";
		if (c == copy.size()) {
			out.pop_back();
		}
		of << out + "\n";
	}
	of << "}";
	of.close();
}

void compiler::compCust(component_t* comp) {
	std::ofstream of;
	of.open(this->outpath + "/" + comp->path);
	std::ifstream f;
	f.open(this->inpath + "/" + comp->path);
	char buf;
	std::string line;
	while (f.read(&buf, sizeof(buf)))
	{
		line += buf;
	}
	of << line;
	of.close();
}