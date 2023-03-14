#pragma once
#define PRJ_FNAME "index.prj"
#define SRC_DNAME "src"
#define OUT_DNAME "out"
#define SPRITES_DNAME "spts"

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
	component_t* fg_c = nullptr;

	std::string name;
	std::string ver;
	std::string author;
	std::string info;
	bool open(std::string);
	bool create(std::string);

	void upt();
	void compile();

	Project();
	
	std::vector<component_t*> comp;

	void newCmp(e_component_type);
	void addCmp(std::pair<std::string, std::string>);
	void addCmp(component_t*);

	void openFG(component_t*);
	uint8_t openFG(std::string);
	bool isOpen();
private:
	bool opened = false;
	std::string projectPath;
	std::string info_file_path;
	std::string fstr;
};