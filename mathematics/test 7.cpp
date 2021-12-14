// test 7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <iomanip>

#define UNS_EQL_ACCURACY 0.0000001
#include "mathematics.h"


int main()
{
    std::cout << "START" << std::endl;
    
    double d = 1.7F;

    std::cout << std::boolalpha << uns::math::Div(-d,0.0F) << std::endl;

    std::cout << "FINISH" << std::endl;
}