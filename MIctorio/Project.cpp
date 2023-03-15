#include "Project.h"

#include "fw_elem.h"
#include "config.h"
#include "str_proc.h"
#include "compiler.h"
#include <direct.h>

Project::Project() {
	this->comp = std::vector<component_t*>(0);
}

bool Project::create(std::string pth) {
	pth = fw::correct_path(pth);
	std::string name, athr, ver, info;
	std::cout << std::endl << "Name: ";
	name = str_in();
	std::cout << "Author: ";
	athr = str_in();
	std::cout << "Version: ";
	ver = str_in();
	std::cout << "Description: ";
	info = str_in();

	std::map<std::string, std::string> set;
	set["name"] = name;
	set["author"] = athr;
	set["ver"] = ver;
	set["descr"] = info;
	set["fstruct"] = gcfg::m_cfg["fstruct_name"];

	fw::upt((pth + std::string(PRJ_FNAME)).c_str(), set);

	_mkdir((pth + SRC_DNAME).c_str());
	_mkdir((pth + SRC_DNAME + "/" + SPRITES_DNAME).c_str());
	return this->open(pth);
}

bool Project::open(std::string pth) {
	pth = fw::correct_path(pth);
	this->projectPath = pth;
	this->info_file_path = (pth + std::string(PRJ_FNAME));
	std::map<std::string, std::string> set = fw::read(this->info_file_path.c_str());

	this->name = set["name"];
	this->author = set["author"];
	this->ver = set["ver"];
	this->info = set["descr"];
	this->fstr = set["fstruct"];

	for (std::pair<std::string, std::string> pr : set) {
		if (pr.first.rfind(this->fstr, 0) == 0) {
			addCmp(pr);
		}
	}
	this->opened = true;
	return true;
}

bool Project::isOpen() {
	return this->opened;
}

void Project::upt() {
	std::map<std::string, std::string> set = fw::read(this->info_file_path.c_str());
	for (component_t* cmp : this->comp) {
		std::string cl1 = this->fstr + cmp->name, cl2 = component_t::tte(cmp->type) + ":" + cmp->path;
		for (std::pair<std::string, std::string> pr : set) {
			if (!(pr.first == cl1 && pr.second == cl2)) {
				set[cl1] = cl2;
			}
		}
		if (cmp->type != e_component_type::custom) {
			fw::upt((this->projectPath + SRC_DNAME + "/" + cmp->path).c_str(), cmp->mParam);
		}
		else {
			const char* pth = (this->projectPath + cmp->path).c_str();
			std::ifstream f;
			f.open(pth);
			if (!f.is_open()) {
				std::ofstream of;
				of.open(pth);
				of.close();
			}
		}
	}
	fw::upt(this->info_file_path.c_str(), set);
}

void Project::removeComp(size_t i) {
	std::remove((this->projectPath + this->comp[i]->path).c_str());
	std::map<std::string, std::string> set = fw::read(this->info_file_path.c_str());
	set.erase(this->fstr + this->comp[i]->name);
	fw::upt(this->info_file_path.c_str(), set);
	this->comp.erase(comp.begin() + i);
	this->upt();
}

bool Project::rmCmp(std::string name) {
	for (size_t i = 0; i < this->comp.size(); i++) {
		if (this->comp[i]->name == name) {
			this->removeComp(i);
			return true;
		}
	}
	return false;
}

bool Project::rmCmpByPath(std::string pth) {
	for (size_t i = 0; i < this->comp.size(); i++) {
		if (this->comp[i]->path == pth) {
			this->removeComp(i);
			return true;
		}
	}
	return false;
}

void Project::addCmp(std::pair<std::string, std::string> pr) {
	std::string nm = pr.first.substr(this->fstr.length());
	std::string type = "", file = "";
	for (int i = 0; i < pr.second.length(); i++) {
		if (pr.second[i] != ':') {
			type += pr.second[i];
		}
		else {
			i++;
			for (i; i < pr.second.length(); i++) {
				file += pr.second[i];
			}
		}
	}
	component_t* comp = new component_t(file, nm);
	comp->type = component_t::ebt(type);
	comp->mParam = fw::read((this->projectPath + SRC_DNAME + "/" + comp->path).c_str());
	this->addCmp(comp);
}

void Project::addCmp(component_t* cmp) {
	this->comp.push_back(cmp);
}

void Project::openFG(component_t* cmp) {
	this->fg_c = cmp;
}

uint8_t Project::openFG(std::string str) {
	for (component_t* c : this->comp) {
		if (str == c->name) {
			if (c->type == e_component_type::custom) {
				return 2;
			}
			else {
				this->openFG(c); 
				return 1;
			}
		}
	}
	return 2;
}

void Project::newCmp(e_component_type ec) {
	std::string name, path;
	std::cout << "Name: ";
	name = str_in();
	std::cout << "Path(relative): ";
	path = (str_in() + '.' + COMPONENT_FILE_EXTENSION);
	
	component_t* cmp = new component_t(path, name);
	cmp->type = ec;
	cmp->mParam = genDef(cmp->type);

	this->addCmp(cmp);
	this->upt();

	if (ec != e_component_type::custom) {
		this->openFG(cmp);
	}
	else {
		std::ifstream f;
		f.open(cmp->path);
		if (!f.is_open()) {
			std::ofstream of;
			of.open(cmp->path);
			of.close();
		}
	}
}

std::map<std::string, std::string> Project::genDef(e_component_type type) {
	std::map<std::string, std::string> smp;
	if (type == e_component_type::mod_info) {
		smp["name"] = this->name;
		smp["version"] = this->ver;
		smp["title"] = this->name;
		smp["author"] = this->author;
		smp["factorio_version"] = "1.1";
	}
	return smp;
}

void Project::compile() {
	compiler cmp(this->projectPath, this->comp);
	cmp.compile();
}