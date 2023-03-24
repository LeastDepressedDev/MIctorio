#include <vector>
#include <iostream>

template<typename _Type>
class data_container : public std::vector<_Type> {
public:
	using std::vector<_Type>::vector;
	//using std::vector<_Type>::push_back;
	std::string name; 

	data_container sname(std::string name) {
		this->name = name;
		return *this;
	}
};