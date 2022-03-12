// test 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <iomanip>

#include "time.h"



int main() {
	std::cout << "START" << std::endl;

	uns::timer t;

	auto check = t.Count<std::chrono::seconds>();
	while (check < 0.0011001) {
		check = t.Count<std::chrono::seconds>();
	};

	std::cout << check << std::endl;

	std::cout << "FINISH" << std::endl;
}