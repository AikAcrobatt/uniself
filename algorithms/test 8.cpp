// test 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"


#include <iostream>
#include <string>
#include <array>
#include "algorithms.h"


namespace unsf {
	UNS_ENUM_DECLARATOR(test, int,
		el0 = 0,
		el1 = 1,
		el2 = 2
	);

};

#define ENUM_NAME_TEST unsf::test
UNS_ENUM_STRING_CAST_DECLARATOR(ENUM_NAME_TEST);


int main()
{
	std::cout << "START" << std::endl;

	unsf::test v = unsf::test::el1;

	std::cout << uns::string_cast<std::string>(v) << std::endl;


	std::cout << "FINISH" << std::endl;
	
}
