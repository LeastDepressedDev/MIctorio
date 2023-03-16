#include "compiler.h"

#include "hd.h"

#include <direct.h>
#include "Project.h"
#include "fw_elem.h"
#include <fstream>

std::map<e_component_type, std::string> compiler::links = {
	{e_component_type::c_item, "items.lua"}
};

std::map<e_component_type, std::string> compiler::secNm = {
	{e_component_type::c_item, "item-name"}
};

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

void compiler::dataLua(std::vector<std::string> regs) {
	std::ofstream of;
	of.open(this->outpath + "/data.lua");
	for (std::string reg : regs) {
		of << "require(\"__" + this->mod_name + "__/" + reg + "\")\n";
	}
	of.close();
}

void compiler::push(e_component_type type, std::vector<std::string> data) {
	std::string line = "data:extend({";
	for (std::string comp : data) {
		line += "\n" + comp + ",";
	}
	line.pop_back();
	line += "})";
	std::ofstream of;
	std::string add = compiler::links[type];
	of.open(this->outProc + "/" + add);
	of << line;
	of.close();
	tec.push_back("prototypes/" + add);
}

void compiler::pushAll() {
	for (std::pair<e_component_type, std::vector<std::string>> pr : this->pred) {
		this->push(pr.first, pr.second);
	}
}

void compiler::addLocale() {
	std::map<e_component_type, std::string> locn;
	printf("Finding item names... ");
	int i = 0;
	for (component_t* cmp : this->comps) {
		if (!(cmp->type == e_component_type::mod_info || cmp->type == e_component_type::custom)) {
			locn[cmp->type] += cmp->mParam["name"] + "=" + cmp->mParam["title"] + "\n";
			i++;
		}
	}
	printf("Done.\n%d names found.\n Inserting... ", i);
	std::ofstream of;
	std::string pth = this->outLoc + "/" + "en" + "/" + this->mod_name + ".cfg";
	fw::buildPth(pth);
	of.open(pth);
	for (std::pair<e_component_type, std::string> pr : locn) {
		of << "[" + secNm[pr.first] + "]\n" + pr.second + "\n\n";
	}
	of.close();
	printf("Done.\n");
}

void compiler::prepMap() {
	this->pred[e_component_type::c_item] = std::vector<std::string>(0);
}

void compiler::compile() {
	system(PROG_CLR);

	printf("Compiling...\n");
	printf("Preparing output folder...\n");
	component_t* comp = this->getInfo();
	tec = std::vector<std::string>(0);
	if (comp == nullptr) {
		printf("Info file not found. Compiling canceled!\n");
		return;
	}
	this->mod_name = comp->mParam["name"];
	std::string nm = this->mod_name;
	int c = comp->mParam["version"].rfind('.');
	for (int i = 0; i < 2 - c; i++) {
		comp->mParam["version"] += ".0";
	}
	nm += "_" + comp->mParam["version"];
	this->outpath = this->dpath + nm;

	_rmdir(this->outpath.c_str());
	_mkdir(this->outpath.c_str());
	this->outGrapgh = (this->outpath + "/grs");
	this->outLoc = (this->outpath + "/locale");
	this->outProc = (this->outpath + "/prototypes");
	_mkdir(outGrapgh.c_str());
	_mkdir(outLoc.c_str());
	_mkdir(outProc.c_str());
	printf("Done.\n");

	printf("Preparing sets...\n");
	prepMap();
	printf("Done.\n");

	printf("Compiling %d elements: \n", this->comps.size());
	int cob = 0;
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
			case e_component_type::c_item:
				this->compItem(cmp);
				return true;
			default:
				return false;
			}
			}()) {
			printf("success.\n");
			cob++;
		}
		else {
			printf("error.\n");
		}
	}

	printf("Creating %d files... \n", this->pred.size());
	this->pushAll();
	printf("Building data.lua for %d objects... \n", this->pred.size());
	this->dataLua(tec);
	printf("Localising %d objects... \n", cob);
	this->addLocale();
	printf("Localising done.\n");
}

bool cont(std::map<std::string, std::string> mp, std::string key) {
	for (std::pair<std::string, std::string> pr : mp) {
		if (pr.first == key) {
			return true;
		}
	}
	return false;
}

bool checkNumb(std::string str) {
	for (int i = 0; i < str.length(); i++) {
		switch (str[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
			return false;
		}
	}
	return true;
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
	std::string s = comp->path.substr(0, comp->path.size() - 4) + ".lua";
	fw::buildPth(this->outpath + "/" + s);
	of.open(this->outpath + "/" + s);
	std::ifstream f;
	f.open(this->inpath + "/" + SRC_DNAME + "/" + comp->path);
	char buf;
	std::string line;
	while (f.read(&buf, sizeof(buf)))
	{
		line += buf;
	}
	of << line;
	of.close();
	tec.push_back(s);
}

void compiler::compItem(component_t* comp) {
	std::string lines = "{";
	for (std::pair<std::string, std::string> pr : comp->mParam) {
		lines += "\n	" + pr.first;
		lines += ((checkNumb(pr.second)) ? (" = " + pr.second + ",") : (" = \"" + pr.second + "\","));
		/*if (checkNumb(pr.second)) {
			lines += "= " + pr.second + ",";
		}
		else {
			"= \"" + pr.second + "\",";
		}*/
	}
	lines.pop_back();
	lines += "\n}\n";
	this->pred[e_component_type::c_item].push_back(lines);
}