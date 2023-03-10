#pragma once
#include <iostream>


class Project {
public:
	std::string name;
	std::string ver;
	void open(std::string);
	Project();

	void upt();

private:
	std::string info_file_path;
};