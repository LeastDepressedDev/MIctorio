#include "fw_elem.h"
#include <fstream>

std::map<std::string, std::string> fw::read(const char* path) {
	std::ifstream f;
	f.open(path);
	if (!f.is_open()) {
		std::ofstream of;
		of.open(path);
		of.close();
	}
	f.open(path);
	std::string line = "";
	char* c_buf = new char[256];
	while (f.read(c_buf, sizeof(c_buf)))
	{
		line += c_buf;
	}

	std::map<std::string, std::string> set = std::map<std::string, std::string>();
	std::string s1 = "", s2 = "";
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == '\n') {
			continue;
		}
		else if (line[i] == '@') {
			for (i; i < line.length(); i++) {
				if (line[i] == '\n') {
					continue;
				}
				else if (line[i] == ';') {
					set[s1] = s2;
					s1 = ""; s2 = "";
					break;
				}
				else {
					s2 += line[i];
				}
			}
		}
		else {
			s1 += line[i];
		}
	}

	return set;
}

void fw::upt(const char* path, std::map<std::string, std::string> set) {
	std::ofstream of;
	of.open(path);
	std::string lines;
	for (std::pair<std::string, std::string> pr : set) {
		lines += pr.first + "@" + pr.second + ";\n";
	}

	of.write(lines.c_str(), sizeof(lines));
	of.close();
}