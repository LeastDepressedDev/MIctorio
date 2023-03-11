#pragma once
#include <iostream>
#include <functional>
#include <vector>

class command {
public:
	std::string sId;
	std::string sDesc;
	std::function<void (std::vector<std::string>)> f;

	command(std::string idm, std::string desc, std::function<void (std::vector<std::string>)> realise) {
		this->sId = idm;
		this->sDesc = desc;
		this->f = realise;
	}
};