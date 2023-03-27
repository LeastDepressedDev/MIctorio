#include "component.h"

#include "index.h"
#include "DataRaw.h"
#include "recipe.h"
#include <string>

std::map<e_component_type, std::vector<command>> eSet = {
	{e_component_type::mod_info,
		{
			command("title", "Sets title of your mod", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					std::string line;
					for (int i = 1; i < cmd.size(); i++) {
						line += cmd[i] + " ";
					}
					line.pop_back();
					glob_app::cur_prj->fg_c->mParam["title"] = line;
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			})
		}
	},
	{e_component_type::c_item, 
		{
			command("title", "Sets default localisation name", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					std::string line;
					for (int i = 1; i < cmd.size(); i++) {
						line += cmd[i] + " ";
					}
					line.pop_back();
					glob_app::cur_prj->fg_c->mParam["title"] = line;
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			}),
			command("icon", "sets title of your mod", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					glob_app::cur_prj->fg_c->mParam["icon"] = cmd[1];
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Path> required." << std::endl;
				}
			}),
			command("icsz", "Sets icon size", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					glob_app::cur_prj->fg_c->mParam["icon_size"] = cmd[1];
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Integer> required." << std::endl;
				}
			}),
			command("stack", "Sets stack limit", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					glob_app::cur_prj->fg_c->mParam["stack_size"] = cmd[1];
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			}),
			command("subgroup", "Sets subgroup(Required to render in the right recipe grid!)", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					if ([cmd]() {
						for (std::string st : factorio::actual::item_subgroup) {
							if (st == cmd[1]) {
								return true;
							}
						}
						return false;
					}()) {
						glob_app::cur_prj->fg_c->mParam["subgroup"] = cmd[1];
						glob_app::cur_prj->upt();
					}
					else {
						std::cout << "Wrong subgroup name!" << std::endl;
						std::cout << "Allowed subgroups: " << std::endl;
						std::string to_out;
						for (int i = 0; i < factorio::actual::item_subgroup.size(); i++) {
							for (int j = 0; j < 5; j++) {
								to_out += factorio::actual::item_subgroup[i] + ", ";
								i++;
							}
							to_out += "\n";
						}
						if (to_out[to_out.length() - 1] == '\n') {
							to_out.pop_back();
						}
						to_out.pop_back(); to_out.pop_back();
						std::cout << to_out << std::endl;
					}
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			})
		}
	},
	{e_component_type::c_recipe,
		{
			command("see", "Shows current recipe state", [](std::vector<std::string> cmd) {
				std::vector<semi_rc> ingr(std::stoi(glob_app::cur_prj->fg_c->mParam["icount"])), 
					resu(std::stoi(glob_app::cur_prj->fg_c->mParam["rcount"]));
				for (std::pair<std::string, std::string> pr : glob_app::cur_prj->fg_c->mParam) {
					if (pr.first._Starts_with(ING_INDET)) {
						int icd = std::stoi(pr.first.substr(std::string(ING_INDET).length()));
						ingr[icd] = parseRecStr(pr.second);
					}
					else if (pr.first._Starts_with(RES_INDET)) {
						int icd = std::stoi(pr.first.substr(std::string(RES_INDET).length()));
						resu[icd] = parseRecStr(pr.second);
					}
				}

				std::cout << "From:" << std::endl;
				for (semi_rc rc : ingr) {
					std::cout << rc.type << " - " << rc.id << " - " << rc.count << ";" << std::endl;
				}
				std::cout << "To:" << std::endl;
				for (semi_rc rc : resu) {
					std::cout << rc.type << " - " << rc.id << " - " << rc.count << ";" << std::endl;
				}
			}),
			command("add", "Adding component /add <mode> <type> <id> <count>", [](std::vector<std::string> cmd) {
				if (cmd.size() > 4) {
					if (cmd[1] == "i") {
						if ([cmd]() {
							for (std::string tp : factorio::ingr_type) {
								if (tp == cmd[2]) {
									return false;
								}
							}
							return true;
							}()) {
							std::cout << "Incorrect recipe type. >dlt to help " << std::endl;
							return;
						}

						if ([cmd]() {
							if (cmd[2] == "item") {
								for (std::string tp : factorio::actual::items) {
									if (tp == cmd[3]) {
										return false;
									}
								}
							}
							else if (cmd[2] == "fluid") {
								for (std::string tp : factorio::actual::fluids) {
									if (tp == cmd[3]) {
										return false;
									}
								}
							}
							return true;
							}()) {
							std::cout << "Incorrect " << cmd[2] << " name. >dlt to help " << std::endl;
							return;
						}

						std::string addr = cmd[3] + ":" + cmd[2] + ":" + cmd[4];
						int nid = std::stoi(glob_app::cur_prj->fg_c->mParam["icount"]);
						glob_app::cur_prj->fg_c->mParam[std::string(ING_INDET) + std::to_string(nid)] = addr;
						glob_app::cur_prj->fg_c->mParam["icount"] = std::to_string(nid + 1);
						glob_app::cur_prj->upt();
					}
					else if (cmd[1] == "r") {
						if ([cmd]() {
							for (std::string tp : factorio::ingr_type) {
								if (tp == cmd[2]) {
									return false;
								}
							}
							return true;
							}()) {
							std::cout << "Incorrect recipe type. Allowed type: " << std::endl;
							for (std::string tp : factorio::ingr_type) {
								std::cout << tp << std::endl;
							}
							return;
						}

						if ([cmd]() {
							if (cmd[2] == "item") {
								for (std::string tp : factorio::actual::items) {
									if (tp == cmd[3]) {
										return false;
									}
								}
							}
							else if (cmd[2] == "fluid") {
								for (std::string tp : factorio::actual::fluids) {
									if (tp == cmd[3]) {
										return false;
									}
								}
							}
							return true;
							}()) {
							std::cout << "Incorrect " << cmd[2] << " name. >dlt to help " << std::endl;
							return;
						}

						std::string addr = cmd[3] + ":" + cmd[2] + ":" + cmd[4];
						int nid = std::stoi(glob_app::cur_prj->fg_c->mParam["rcount"]);
						glob_app::cur_prj->fg_c->mParam[std::string(RES_INDET) + std::to_string(nid)] = addr;
						glob_app::cur_prj->fg_c->mParam["rcount"] = std::to_string(nid + 1);
						glob_app::cur_prj->upt();
					}
					else {
						std::cout << "Wrong action!" << std::endl;
					}
				}
				else {
					std::cout << "4 Arguments required: <act> <type> <id> <count>" << std::endl;
				}
			}),
			command("rm", "Removing component /rm <mode> <id>", [](std::vector<std::string> cmd) {
				if (cmd.size() > 2) {
					if (cmd[1] == "i") {
						int ln = std::stoi(glob_app::cur_prj->fg_c->mParam["icount"]);
						int st = std::stoi(cmd[2]);
						if (st < ln) {
							std::string igt(ING_INDET);
							glob_app::cur_prj->fg_c->mParam.erase(igt + cmd[2]);
							std::map<std::string, std::string> clone = glob_app::cur_prj->fg_c->mParam;
							for (std::pair<std::string, std::string> pr : clone) {
								if (pr.first._Starts_with(igt)) {
									int icd = std::stoi(pr.first.substr(igt.length()));
									if (icd > st) {
										icd--;
										glob_app::cur_prj->fg_c->mParam[igt + std::to_string(icd)] = pr.second;
									}
								}
							}
							glob_app::cur_prj->fg_c->mParam.erase(igt + std::to_string(ln-1));
							glob_app::cur_prj->fg_c->mParam["icount"] = std::to_string(ln-1);
						}
						else {
							std::cout << "Please, enter valid id." << std::endl;
						}
					}
					else if (cmd[1] == "r") {
						int ln = std::stoi(glob_app::cur_prj->fg_c->mParam["rcount"]);
						int st = std::stoi(cmd[2]);
						if (st < ln) {
							std::string igt(RES_INDET);
							glob_app::cur_prj->fg_c->mParam.erase(igt + cmd[2]);
							std::map<std::string, std::string> clone = glob_app::cur_prj->fg_c->mParam;
							for (std::pair<std::string, std::string> pr : clone) {
								if (pr.first._Starts_with(igt)) {
									int icd = std::stoi(pr.first.substr(igt.length()));
									if (icd > st) {
										icd--;
										glob_app::cur_prj->fg_c->mParam[igt + std::to_string(icd)] = pr.second;
									}
								}
							}
							glob_app::cur_prj->fg_c->mParam.erase(igt + std::to_string(ln - 1));
							glob_app::cur_prj->fg_c->mParam["rcount"] = std::to_string(ln - 1);
						}
						else {
							std::cout << "Please, enter valid id." << std::endl;
						}
					}
					else {
						std::cout << "Wrong action!" << std::endl;
					}
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "2 Arguments required: <mode> <id>" << std::endl;
				}
			}),
			command("speed", "Set's time, required to craft an item(<Double> value); value > => requiring more time", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					glob_app::cur_prj->fg_c->mParam["energy_required"] = cmd[1];
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			}),
			command("subgroup", "Sets subgroup(Required to render in the right recipe grid!)", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					if ([cmd]() {
						for (std::string st : factorio::actual::item_subgroup) {
							if (st == cmd[1]) {
								return true;
							}
						}
						return false;
					}()) {
						glob_app::cur_prj->fg_c->mParam["subgroup"] = cmd[1];
						glob_app::cur_prj->upt();
					}
					else {
						std::cout << "Wrong subgroup name!" << std::endl;
						std::cout << "Allowed subgroups: " << std::endl;
						std::string to_out;
						for (int i = 0; i < factorio::actual::item_subgroup.size(); i++) {
							for (int j = 0; j < 5; j++) {
								to_out += factorio::actual::item_subgroup[i] + ", ";
								i++;
							}
							to_out += "\n";
						}
						if (to_out[to_out.length() - 1] == '\n') {
							to_out.pop_back();
						}
						to_out.pop_back(); to_out.pop_back();
						std::cout << to_out << std::endl;
					}
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			}),
			command("title", "Sets default localisation name", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					std::string line;
					for (int i = 1; i < cmd.size(); i++) {
						line += cmd[i] + " ";
					}
					line.pop_back();
					glob_app::cur_prj->fg_c->mParam["title"] = line;
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "<Title> required." << std::endl;
				}
			})
		}
	}
};

std::map<std::string, e_component_type> nameLinker = {
	{"info", e_component_type::mod_info},
	{"cust", e_component_type::custom},
	{"item", e_component_type::c_item},
	{"recipe", e_component_type::c_recipe},
	{"virtual", e_component_type::virt}
};

component_t::component_t(std::string path, std::string str) {
	this->mParam = std::map<std::string, std::string>();
	this->path = path;
	this->name = str;
}

e_component_type component_t::ebt(std::string sType) {
	return nameLinker[sType];
}

std::string component_t::tte(e_component_type e) {
	for (std::pair<std::string, e_component_type> pr : nameLinker) {
		if (pr.second == e) {
			return pr.first;
		}
	}
	return NULL;
}

std::vector<command> component_t::cmdRs() {
	return eSet[glob_app::cur_prj->fg_c->type];
}