#pragma once
#include <iostream>
#include <vector>

extern std::vector<std::string> vec_split(std::string, char sep);
extern std::string str_in();
extern std::string str_in(std::string);

template<typename _Type>
inline bool vec_cont(std::vector<_Type> vec, std::vector<std::string> args) {
	for (_Type c : vec) {
		for (_Type t : args) {
			if (c == t) {
				return true;
			}
		}
	}
	return false;
}