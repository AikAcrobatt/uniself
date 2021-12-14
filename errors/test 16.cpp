// errors.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include "uniself/algorithms.h"

#include "errors.h"

int main()
{
    std::cout << "START" << std::endl;

    std::cout << std::boolalpha << uns::errors::Settle(L"ErrorLog.txt") << std::endl;







    uns::errors::Fix("Test 16.cpp", uns::string_cast<std::string>(__FILE__) + "_" + uns::string_cast<std::string>(__LINE__), "testing");

    std::cout << "FINISH" << std::endl;
}