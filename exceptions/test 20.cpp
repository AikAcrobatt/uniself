// exceptions.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include <exception>
#include <stdexcept>

#include "exceptions.h"

UNS_EXCEPTION_DECLARE(issue, uns::exceptions::basic);

UNS_EXCEPTION_DECLARE(issue2, issue);

int main() {
	std::cout << "START" << std::endl;

	try {
		throw issue2("Try");
	}
	catch (issue2& e) {
		std::cout << "4 = " << e.what() << std::endl;
	}
	catch (issue& e) {
		std::cout << "1 = " << e.what() << std::endl;
	}
	catch (uns::exceptions::basic& e) {
		std::cout << "2 = " << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "3 = " << e.what() << std::endl;
	};

	std::cout << "FINISH" << std::endl;
};