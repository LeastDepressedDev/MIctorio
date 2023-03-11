#include "Project.h"

#include "fw_elem.h"
#include "config.h"
#include "index.h"
#include <map>

Project::Project() {
	this->comp = std::vector<component_t>(0);
}

void Project::create(std::string pth) {
	std::string name, athr, ver, info;
	std::cout << std::endl << "Name: ";
	std::cin >> name;
	std::cout << "Author: ";
	std::cin >> athr;
	std::cout << "Version: ";
	std::cin >> ver;
	std::cout << "Description: ";
	std::cin >> info;

	std::map<std::string, std::string> set;
	set["name"] = name;
	set["author"] = athr;
	set["ver"] = ver;
	set["descr"] = info;
	set["fstruct"] = gcfg::m_cfg["fstruct_name"];

	fw::upt((pth + std::string(PRJ_FNAME)).c_str(), set);
	
}

void Project::open(std::string pth) {
	this->info_file_path = (pth + std::string(PRJ_FNAME));
	std::map<std::string, std::string> set = fw::read(this->info_file_path.c_str());

	this->name = set["name"];
	this->author = set["author"];
	this->ver = set["ver"];
	this->info = set["descr"];
	this->fstr = set["fstruct"];

	for (std::pair<std::string, std::string> pr : set) {
		if (pr.first.rfind(this->fstr, 0) == 0) {
			std::string nm = pr.first.substr(this->fstr.length());
			
		}
	}
}