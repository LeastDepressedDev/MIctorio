#pragma once

#define ING_INDET "s:"
#define RES_INDET "r:"

#include <iostream>
#include <string>
#include <list>

namespace factorio {
	inline std::list<std::string> ingr_type = {
		"item", "fluid"
	};
}

typedef struct semi_rc {
	std::string type;
	std::string id;
	int count;
};

inline semi_rc parseRecStr(std::string str) {
	semi_rc ik;
	std::string c1, c2, c3;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ':') {
			i++;
			for (i; i < str.length(); i++) {
				if (str[i] == ':') {
					i++;
					for (i; i < str.length(); i++) {
						c3 += str[i];
					}
				}
				else {
					c2 += str[i];
				}
			}
		}
		else {
			c1 += str[i];
		}
	}
	ik.id = c1;
	ik.type = c2;
	ik.count = std::stoi(c3);
	return ik;
}