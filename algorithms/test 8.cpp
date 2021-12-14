// test 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"


#include <iostream>
#include <string>
#include <array>
#include "algorithms.h"


namespace unsf {
	UNS_ENUM_DECLARATOR(TEST, char,
		el0 = 0,
		el1 = 1,
		el2 = 2
	);

};

#define ENUM_NAME_TEST unsf::TEST
UNS_ENUM_STRING_CAST_DECLARATOR(ENUM_NAME_TEST);


int main()
{
	std::cout << "START" << std::endl;

	std::array<int, 10> C;
	for (int i = 0; i < 10; i++) C[i] = i;

	uns::containers::sequential::Replace(C, 7, 2);

	for (auto c : C)
		std::cout << c << std::endl;


	std::cout << "FINISH" << std::endl;
	
}
