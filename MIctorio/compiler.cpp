#include "compiler.h"

#include "hd.h"

#include <direct.h>
#include <regex>
#include <fstream>
#include "Project.h"
#include <string>
#include "fw_elem.h"

#include <filesystem>

#include "recipe.h"

std::map<e_component_type, std::string> compiler::links = {
	{e_component_type::c_item, "items.lua"},
	{e_component_type::c_recipe, "recipes.lua"},
	{e_component_type::wit, "wit.lua"}
};

std::map<e_component_type, std::string> compiler::secNm = {
	{e_component_type::c_item, "item-name"},
	{e_component_type::c_recipe, "recipe-name"},
	{e_component_type::wit, "unsorted"},
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
	if (data.size() < 1) return;
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

void compiler::assetDeal(std::string pthAdr) {
	for (const auto& fl : std::filesystem::directory_iterator(this->inpath + "/" + SPRITES_DNAME + ((pthAdr.length() > 0) ? "/" : "") + pthAdr)) {
		std::string fname = fl.path().filename().string();
		if (fl.is_directory()) {
			assetDeal(pthAdr + "/" + fname);
		}
		else {
			std::cout << fname << "..";
			std::ofstream of;
			std::ifstream f;
			f.open(this->inpath + "/" + SPRITES_DNAME + ((pthAdr.length() > 0) ? "/" : "") + pthAdr + "/" + fname, std::ios::binary);
			std::string outPth = this->outGrapgh + ((pthAdr.length() > 0) ? "/" : "") + pthAdr + "/" + fname;
			fw::buildPth(outPth);
			of.open(outPth, std::ios::binary);
			char c;
			while (f.read(&c, sizeof(c))) {
				of << c;
			}
			f.close();
			of.close();
			std::cout << "transfered." << std::endl;
		}
	}
}

void compiler::addLocale() {
	std::map<e_component_type, std::string> locn;
	printf("Finding item names... ");
	int i = 0;
	for (component_t* cmp : this->comps) {
		if ([cmp](){
			switch (cmp->type)
			{
			case e_component_type::custom:
			case e_component_type::unkown:
			case e_component_type::mod_info:
			case e_component_type::virt:
				return false;
			default:
				return true;
			}
			}()) {
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
	this->pred[e_component_type::c_recipe] = std::vector<std::string>(0);

	this->pred[e_component_type::wit] = std::vector<std::string>(0);
}

std::string reb(std::string str) {
	std::string ln;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '/') {
			ln += "\\\\";
		}
		else {
			ln += str[i];
		}
	}
	return ln;
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

	std::filesystem::directory_entry dir(this->outpath);
	if (dir.exists()) {
		std::string cmd = std::string(PROG_RMDIR) + " /s \"" + reb(dir.path().string()) + "\"";
		system(cmd.c_str());
	}

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
	this->cob = 0;
	for (component_t* cmp : this->comps) {
		printf("Compiling %s: ", cmp->name.c_str());
		if (comph(cmp)) {
			printf("success.\n");
			this->cob++;
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
	printf("Transfering assets... \n");
	this->assetDeal("");
	printf("Done.\n");
}

bool compiler::comph(component_t* comp) {
	component_t* cmp = new component_t(*comp);
	switch (cmp->type)
	{
	case e_component_type::mod_info:
		this->compInfo(cmp);
		delete cmp;
		return true;
	case e_component_type::custom:
		this->compCust(cmp);
		delete cmp;
		return true;
	case e_component_type::c_item:
		this->compItem(cmp);
		delete cmp;
		return true;
	case e_component_type::c_recipe:
		this->compRecipe(cmp);
		delete cmp;
		return true;
	case e_component_type::virt:
		this->cob--;
		delete cmp;
		return true;
	case e_component_type::wit:
		this->compWit(cmp);
		delete cmp;
		return true;
	case e_component_type::hpar:
		this->compHpar(cmp);
		delete cmp;
		return true;
	default:
		return false;
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

bool checkNumb(std::string str) {
	//return std::regex_match(str, std::regex("(\\d)\\w+")) || std::regex_match(str, std::regex("(\\d)"));
	return std::regex_match(str, std::regex("^[0-9]*\\.*[0-9]*$"));
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
	//std::string s = comp->path.substr(0, comp->path.size() - 4) + ".lua";
	fw::buildPth(this->outpath + "/" + comp->path);
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
	tec.push_back(comp->path);
}

void compiler::compWit(component_t* comp) {
	std::string lines = "{";
	for (std::pair<std::string, std::string> arg : comp->mParam) {
		lines += "\n	" + arg.first;
		lines += ((checkNumb(arg.second)) || (arg.second[0] == '{')) ? (" = " + arg.second + ",") : (" = \"" + arg.second + "\",");
	}
	lines.pop_back();
	lines += "\n}";
	this->pred[e_component_type::wit].push_back(lines);
}

std::string subl(std::string str) {
	size_t sz = std::string(SPRITES_DNAME).length();
	int x = str.find(SPRITES_DNAME, sz);

	return str.substr(x + sz);
}

void compiler::compItem(component_t* comp) {
	std::string lines = "{\n";
	std::string icon = "__" + this->mod_name + "__/grs" + subl(comp->mParam["icon"]);
	lines += "	icon = \"" + icon + "\",";
	for (std::pair<std::string, std::string> pr : comp->mParam) {
		if (pr.first == "icon") continue;
		lines += "\n	" + pr.first;
		lines += ((checkNumb(pr.second)) || (pr.second[0] == '{')) ? (" = " + pr.second + ",") : (" = \"" + pr.second + "\",");
	}
	lines.pop_back();
	lines += "\n}";
	this->pred[e_component_type::c_item].push_back(lines);
}

void compiler::compRecipe(component_t* comp) {
	std::vector<semi_rc> irg(std::stoi(comp->mParam["icount"])), rrg(std::stoi(comp->mParam["rcount"]));
	for (std::pair<std::string, std::string> pr : comp->mParam) {
		if (pr.first._Starts_with(ING_INDET)) {
			int icd = std::stoi(pr.first.substr(std::string(ING_INDET).length()));
			irg[icd] = parseRecStr(pr.second);
		}
		else if (pr.first._Starts_with(RES_INDET)) {
			int icd = std::stoi(pr.first.substr(std::string(RES_INDET).length()));
			rrg[icd] = parseRecStr(pr.second);
		}
	}

	std::string line = "{\n";
	line += "	name = \"" + comp->mParam["name"] + "\",\n";
	line += "	type = \"" + comp->mParam["type"] + "\",\n";
	line += "	category = \"" + comp->mParam["category"] + "\",\n";
	line += "	subgroup = \"" + comp->mParam["subgroup"] + "\",\n";
	line += "	energy_required = " + comp->mParam["energy_required"] + ",\n";
	//std::string ingr_str;
	line += "	ingredients = {";
	for (semi_rc rc : irg) {
		line += "{type=\"" + rc.type + "\", name=\"" + rc.id + "\", amount=" + std::to_string(rc.count) + "},";
	}
	line.pop_back();
	line += "},\n";
	if (rrg.size() > 1) {
		line += "	results = {";
		for (semi_rc rc : rrg) {
			line += "{type=\"" + rc.type + "\", name=\"" + rc.id + "\", amount=" + std::to_string(rc.count) + "},";
		}
		line.pop_back();
		line += "},\n";
	}
	else {
		line += "	result = \"" + rrg[0].id + "\",\n";
		line += "	result_count = " + std::to_string(rrg[0].count) + ",\n";
	}

	line += "	enabled = " + comp->mParam["enabled"] + "\n}\n";
	this->pred[e_component_type::c_recipe].push_back(line);
}

void compiler::compHpar(component_t* comp) {
	comp->type = component_t::ebt(comp->mParam["pclass"]);
	comp->mParam.erase("pclass");
	this->comph(comp);
}