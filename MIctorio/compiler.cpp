#include "compiler.h"

#include <direct.h>
#include "Project.h"

compiler::compiler(std::string prj_path, std::vector<component_t*> elems) {
	this->inpath = prj_path + SRC_DNAME;
	this->spath = this->inpath + SPRITES_DNAME;
	this->outpath = prj_path + OUT_DNAME;
}

void compiler::compile() {
	_mkdir(this->outpath.c_str());
	this->outGrapgh = (this->outpath + "/grs");
	this->outLoc = (this->outpath + "/locale");
	this->outProc = (this->outpath + "/prototypes");
	_mkdir(outGrapgh.c_str());
	_mkdir(outLoc.c_str());
	_mkdir(outProc.c_str());

	for (component_t* cmp : this->comps) {
		switch (cmp->type)
		{
		case e_component_type::mod_info:

			break;
		default:
			break;
		}
	}
}

void compiler::compInfo(component_t* comp) {
	
}