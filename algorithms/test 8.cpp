// test 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"


#include <iostream>
#include <string>
#include <array>
//#include "algorithms.h"

/*
#define TRY(_name)	\
	class _name##_subclass {	\
		_name##_subclass() { static_assert(typeid(_name##_subclass).name() == #(_name##_subclass)); };					\
	};						\
							\
	_name##_sumclass sub_try;\
*/

#define subclass(name) _##name##_subclass

class _t_subclass {

};

#define to_string(smth) "##_##smth##_subclass##"

int main() {
	std::cout << "START" << std::endl;
	
	//subclass(t) T;
	std::string str = to_string(t);

	std::cout << str << std::endl;


	std::cout << "FINISH" << std::endl;
}
