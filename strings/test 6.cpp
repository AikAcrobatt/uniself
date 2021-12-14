// test 6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <tuple>

#include "strings.h"



int main()
{
	std::cout << "START" << std::endl;
	setlocale(LC_ALL, "Russian");

	/*std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	size_t s = 0;

	std::vector<std::string> samples = { "F","4","A","K","Y" };
	std::vector<std::string> rbs = { "Z", "B", "?", "9" };
	typename std::string::size_type seeker = 0;
	
	std::string fragment = "";
	auto res = false;

	res = uns::string::ReadBySample<std::string>(str, "0123", false, -1, fragment, "4", "Z");
	std::cout << std::boolalpha << res << " " << seeker << " " << fragment << std::endl;*/

	/*std::wstring str = L"_a";
	std::wstring delimiter = L"_";
	std::wstring res1, res2;

	std::tie(res1, res2) = uns::string::Split<std::wstring>(str, delimiter);

	std::wcout << "res1=" << res1 << ";;" << std::endl;
	std::wcout << "delimiter=" << delimiter << ";;" << std::endl;
	std::wcout << "res2=" << res2 << ";;" << std::endl;*/

	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	size_t s = std::string::npos;

	std::cout << std::boolalpha << (str.find("__",0) == std::string::npos) << std::endl;

	std::cout << "FINISH" << std::endl;
}