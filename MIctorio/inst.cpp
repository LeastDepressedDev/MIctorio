#include "inst.h"
#include "index.h"
#include "fw_elem.h"
#include "config.h"

#include "Project.h"

#include "hd.h"

std::map<e_cmd_section, std::vector<command>> CMDS = {
	{e_cmd_section::mwind, 
		{
		    command("help|h", "Help command", [](std::vector<std::string> cmd) {
			    for (std::pair<e_cmd_section, std::vector<command>> pr : CMDS) {
			 	   std::cout << gensec(pr.first) << std::endl;
			  	   for (command c : pr.second) {
					   std::cout << ":  " << GEN_SEPARATOR << c.sId << " - " << c.sDesc << std::endl;
				   }
			    }
				if (cmd.size() > 1) {
					if (cmd[1] == "all" || cmd[1] == "-a") {
						std::cout << "Elements commands: " << std::endl;
						for (std::pair<e_component_type, std::vector<command>> pr : eSet) {
							std::cout << component_t::tte(pr.first) << ":" << std::endl;
							for (command c : pr.second) {
								std::cout << ":  prj>elem" << GEN_SEPARATOR << c.sId << " - " << c.sDesc << std::endl;
							}
						}
					}
				}
		    }),
		    command("test", "Testing some shit", [](std::vector<std::string> cmd) {
				for (std::pair<std::string, std::string> pr : gcfg::m_cfg) {
					std::cout << pr.first << " - " << pr.second << std::endl;
				}
		    }),
			command("prj", "Project call method", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					if (cmd[1] == "help") {
						std::cout << "Prj command help: \n" << ">prj help - help command.\n" << ">prj new <path> - create new project.\n" << ">prj open <path> - open existing project.\n" << "[WIP]>prj recent <id> - open recent project.\n" << "[WIP]>prj recents - list of recent projects.\n" << "[WIP]>prj last - open last project.\n" << ">prj inf <path> - prints short info about project.\n";
					}
					else if (cmd[1] == "new") {
						if (cmd.size() > 2) {
							if (glob_app::cur_prj->create(cmd[2])) {
								glob_app::stage = e_cmd_section::prj;
							}
						}
						else {
							std::cout << "Path required!" << std::endl;
						}
					}
					else if (cmd[1] == "open") {
						if (cmd.size() > 2) {
							if (glob_app::cur_prj->open(cmd[2])) {
								glob_app::stage = e_cmd_section::prj;
							}
						}
						else {
							std::cout << "Path required!" << std::endl;
						}
					}
					else if (cmd[1] == "inf") {
						if (cmd.size() > 2) {
							std::map<std::string, std::string> set = fw::read((fw::correct_path(cmd[2]) + "index.prj").c_str());
							std::cout << "Project name: " << set["name"] << std::endl
								<< "Author: " << set["author"] << std::endl
								<< "Version: " << set["ver"] << std::endl
								<< "Description: " << set["descr"] << std::endl;
						}
						else {
							std::cout << "Path required!" << std::endl;
						}
					}
				}
				else {
					std::cout << "Not enough arguments for project creation. Type `>prj help` for help" << std::endl;
				}
			}),
			
				
				
				
			command("exit|ext|quit|q", "Safe exit", [](std::vector<std::string> cmd) {
				exit(0);
			})
	    }
    },
	{e_cmd_section::prj, 
		{
			command("help", "Project help command", [](std::vector<std::string> cmd) {
				std::cout << gensec(e_cmd_section::prj) << std::endl;
				for (command c : CMDS[e_cmd_section::prj]) {
					std::cout << ":  prj" << GEN_SEPARATOR << c.sId << " - " << c.sDesc << std::endl;
				}
			}),
			command("exit|ext", "Project exit command", [](std::vector<std::string> cmd) {
				delete glob_app::cur_prj;
				glob_app::cur_prj = new Project();
				glob_app::stage = e_cmd_section::mwind;
			}),
			command("info", "Shows information about opened project", [](std::vector<std::string> cmd) {
				std::cout << "Opened project's info: " << std::endl
							<< "Project name: " << glob_app::cur_prj->name << std::endl
							<< "Author: " << glob_app::cur_prj->author << std::endl
							<< "Version: " << glob_app::cur_prj->ver << std::endl
							<< "Description: " << glob_app::cur_prj->info << std::endl;
			}),
			command("list", "Shows list of project's elements", [](std::vector<std::string> cmd) {
				std::vector<component_t*> comps = glob_app::cur_prj->comp;
				printf_s("Components(%d): \n", comps.size());
				for (component_t* cmp : comps) {
					component_t vcmp = *cmp;
					std::cout << vcmp.name << " : " << component_t::tte(vcmp.type) << " : " << vcmp.path << std::endl;
				}
				std::cout << ";" << std::endl;
			}),
			command("new", "Adds new element into your project", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					for (std::pair<std::string, e_component_type> pr : nameLinker) {
						if (cmd[1] == pr.first) {
							glob_app::cur_prj->newCmp(pr.second);
							if (pr.second != e_component_type::custom) {
								glob_app::stage = e_cmd_section::elem;
							}
							return;
						}
					}
					std::string str = "only: [";
					for (std::pair<std::string, e_component_type> pr : nameLinker) {
						str += pr.first + ',';
					}
					str.pop_back();
					str += "] types is valid.";
					std::cout << str << std::endl;
				}
				else {
					std::cout << "Component type required." << std::endl;
				}
			}),
			command("goto", "Open project's element", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					switch (glob_app::cur_prj->openFG(cmd[1]))
					{
					case 1:
						glob_app::stage = elem;
						break;
					case 2:
						std::cout << "Invalid element name" << std::endl;
						break;
					case 3:
						std::cout << "You can't open custom element" << std::endl;
						break;
					default:
						break;
					}
				}
				else {
					std::cout << "Element name required." << std::endl;
				}
			}),
			command("rm|del", "Deletes element", [](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					if (!glob_app::cur_prj->rmCmp(cmd[1])) {
						std::cout << "Unable to delete this element or it's just doesn't exist." << std::endl;
					}
				}
				else {
					std::cout << "Element name required." << std::endl;
				}
			}),
			command("compile|cmp", "Compile project", [](std::vector<std::string> cmd) {
				glob_app::cur_prj->compile();
			})
		}
	},
	{e_cmd_section::elem, 
		{
			command("exit|ext", "Project exit command", [](std::vector<std::string> cmd) {
				glob_app::cur_prj->fg_c = nullptr;
				glob_app::stage = e_cmd_section::prj;
			}),
			command("sts", "Prints element's keyset", [](std::vector<std::string> cmd) {
				for (std::pair<std::string, std::string> pr : glob_app::cur_prj->fg_c->mParam) {
					printf_s("%s - %s\n", pr.first.c_str(), pr.second.c_str());
				}
			}),
			command("set", "Manually set key:value", [](std::vector<std::string> cmd) {
				if (cmd.size() > 2) {
					glob_app::cur_prj->fg_c->mParam[cmd[1]] = cmd[2];
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "Wrong format. use (set <key> <value>)" << std::endl;
				}
			}), 
			command("help", "Helping you with this shit", [](std::vector<std::string> cmd) {
				printf("Commands: \n");
				for (command c : CMDS[e_cmd_section::elem]) {
					std::cout << ":  prj" << GEN_SEPARATOR << "elem" << GEN_SEPARATOR << c.sId << " - " << c.sDesc << std::endl;
				}
				for (command c : eSet[glob_app::cur_prj->fg_c->type]) {
					std::cout << ":  prj" << GEN_SEPARATOR << component_t::tte(glob_app::cur_prj->fg_c->type) 
						<< GEN_SEPARATOR << c.sId << " - " << c.sDesc << std::endl;
				}
			}),
			command("order", "Order filter. Yep...",[](std::vector<std::string> cmd) {
				if (cmd.size() > 1) {
					glob_app::cur_prj->fg_c->mParam["order"] = cmd[1];
					glob_app::cur_prj->upt();
				}
				else {
					std::cout << "Order<Integer> required." << std::endl;
				}
			})
		} 
	},
	{e_cmd_section::general,
		{
			command("clr", "Clear console window", [](std::vector<std::string> cmd) {
				system(PROG_CLR);
			})
		}
	}
};