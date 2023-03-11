#include "inst.h"
#include "config.h"

std::map<e_cmd_section, std::vector<command>> CMDS = {
	{e_cmd_section::mwind,
		{
		    command("help", "Help command", [](std::vector<std::string> cmd) {
			    for (std::pair<e_cmd_section, std::vector<command>> pr : CMDS) {
			 	   std::cout << gensec(pr.first) << std::endl;
			  	   for (command c : pr.second) {
					   std::cout << ":  " << GEN_SEPARATOR << c.sId << " - " << c.sDesc << std::endl;
				   }
			    }
		    }),
		    command("test", "Testing some shit", [](std::vector<std::string> cmd) {
				for (std::pair<std::string, std::string> pr : gcfg::m_cfg) {
					std::cout << pr.first << " - " << pr.second << std::endl;
				}
		    })
	    }
    }
};