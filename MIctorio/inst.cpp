#include "inst.h"

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
		   })
	   }
   }
};