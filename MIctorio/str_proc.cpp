#include "str_proc.h"

std::vector<std::string> vec_split(std::string str, char sep) {
	std::vector<std::string> vec(0);
	std::string buf = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == sep) {
			vec.push_back(buf);
			buf = "";
		}
		else {
			buf += str[i];
		}
	}
	vec.push_back(buf);
	return vec;
}

std::string str_in(std::string inMsg) {
	std::cout << inMsg;
	return str_in();
}

std::string str_in() {
	std::string buf = "";
	char a = '0';
	while (a != '\n')
	{
		scanf_s("%c", &a);
		buf += a;
	}
	return buf.substr(1, buf.length() - 2);
}

//template<typename _Type>
//bool vec_cont(std::vector<_Type> vec, std::vector<_Type> args) 