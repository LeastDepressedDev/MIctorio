#pragma once
#define PRJ_FNAME "index.prj"
#define SRC_DNAME "src"

#include <iostream>
#include <fstream>
#include "component.h"
#include <vector>

/*
	Project file structure:
		index.prj
		./src






*/

class Project {
public:
	//foreground component
	component_t fg_c;

	std::string name;
	std::string ver;
	std::string author;
	std::string info;
	void open(std::string);
	void create(std::string);

	void upt();
	void compile();

	Project();
	
	std::vector<component_t> comp;
private:
	std::string info_file_path;
	std::string fstr;
};