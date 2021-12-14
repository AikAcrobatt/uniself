#pragma once

#include <vector>
#include <string>

#include "better_enums/enum.h"

namespace uns {

	//ФУНКЦИЯ УНИВЕРСАЛЬНОГО СТРОКОВОГО КАСТА
	template<class out_t, class in_t>
	out_t string_cast(const in_t& obj) {
		static_assert(false, "A Uniself string_cast function does not support current types");
		return out_t();
	};
	template<class out_t, class in_t>
	out_t string_cast(const in_t* obj) {
		static_assert(false, "A Uniself string_cast function does not support current types");
		return out_t();
	};
	//преобразование строк к строкам
	template<>
	std::string string_cast<std::string, std::string>(const std::string& obj) {
		return obj;
	};
	template<>
	std::wstring string_cast<std::wstring, std::string>(const std::string& obj) {
		return std::wstring(obj.cbegin(), obj.cend());
	};
	template<>
	std::string string_cast<std::string, std::wstring>(const std::wstring& obj) {
		return std::string(obj.cbegin(), obj.cend());
	};
	template<>
	std::wstring string_cast<std::wstring, std::wstring>(const std::wstring& obj) {
		return obj;
	};
	template<>
	std::string string_cast<std::string, char>(const char* obj) {
		return std::string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, char>(const char* obj) {
		return uns::string_cast<std::wstring>(uns::string_cast<std::string>(obj));
	};
	template<>
	std::wstring string_cast<std::wstring, wchar_t>(const wchar_t* obj) {
		return std::wstring(obj);
	};
	template<>
	std::string string_cast<std::string, wchar_t>(const wchar_t* obj) {
		return uns::string_cast<std::string>(uns::string_cast<std::wstring>(obj));
	};
	//преобразование строк к простым типам
	template<>
	int string_cast<int, std::string>(const std::string& obj) {
		return std::stoi(obj);
	};
	template<>
	int string_cast<int, std::wstring>(const std::wstring& obj) {
		return std::stoi(obj);
	};
	template<>
	int string_cast<int, char>(const char* obj) {
		return std::stoi(std::string(obj));
	};
	template<>
	int string_cast<int, wchar_t>(const wchar_t* obj) {
		return std::stoi(std::wstring(obj));
	};
	template<>
	long string_cast<long, std::string>(const std::string& obj) {
		return std::stol(obj);
	};
	template<>
	long string_cast<long, std::wstring>(const std::wstring& obj) {
		return std::stol(obj);
	};
	template<>
	long string_cast<long, char>(const char* obj) {
		return std::stol(std::string(obj));
	};
	template<>
	long string_cast<long, wchar_t>(const wchar_t* obj) {
		return std::stol(std::wstring(obj));
	};
	template<>
	long long string_cast<long long, std::string>(const std::string& obj) {
		return std::stoll(obj);
	};
	template<>
	long long string_cast<long long, std::wstring>(const std::wstring& obj) {
		return std::stoll(obj);
	};
	template<>
	long long string_cast<long long, char>(const char* obj) {
		return std::stoll(std::string(obj));
	};
	template<>
	long long string_cast<long long, wchar_t>(const wchar_t* obj) {
		return std::stoll(std::wstring(obj));
	};
	template<>
	unsigned int string_cast<unsigned int, std::string>(const std::string& obj) {
		return static_cast<unsigned int>(std::stoull(obj));
	};
	template<>
	unsigned int string_cast<unsigned int, std::wstring>(const std::wstring& obj) {
		return static_cast<unsigned int>(std::stoull(obj));
	};
	template<>
	unsigned int string_cast<unsigned int, char>(const char* obj) {
		return static_cast<unsigned int>(std::stoull(std::string(obj)));
	};
	template<>
	unsigned int string_cast<unsigned int, wchar_t>(const wchar_t* obj) {
		return static_cast<unsigned int>(std::stoull(std::wstring(obj)));
	};
	template<>
	unsigned long string_cast<unsigned long, std::string>(const std::string& obj) {
		return std::stoul(obj);
	};
	template<>
	unsigned long string_cast<unsigned long, std::wstring>(const std::wstring& obj) {
		return std::stoul(obj);
	};
	template<>
	unsigned long string_cast<unsigned long, char>(const char* obj) {
		return std::stoul(std::string(obj));
	};
	template<>
	unsigned long string_cast<unsigned long, wchar_t>(const wchar_t* obj) {
		return std::stoul(std::wstring(obj));
	};
	template<>
	unsigned long long string_cast<unsigned long long, std::string>(const std::string& obj) {
		return std::stoull(obj);
	};
	template<>
	unsigned long long string_cast<unsigned long long, std::wstring>(const std::wstring& obj) {
		return std::stoull(obj);
	};
	template<>
	unsigned long long string_cast<unsigned long long, char>(const char* obj) {
		return std::stoull(std::string(obj));
	};
	template<>
	unsigned long long string_cast<unsigned long long, wchar_t>(const wchar_t* obj) {
		return std::stoull(std::wstring(obj));
	};
	template<>
	float string_cast<float, std::string>(const std::string& obj) {
		return std::stof(obj);
	};
	template<>
	float string_cast<float, std::wstring>(const std::wstring& obj) {
		return std::stof(obj);
	};
	template<>
	float string_cast<float, char>(const char* obj) {
		return std::stof(std::string(obj));
	};
	template<>
	float string_cast<float, wchar_t>(const wchar_t* obj) {
		return std::stof(std::wstring(obj));
	};
	template<>
	double string_cast<double, std::string>(const std::string& obj) {
		return std::stod(obj);
	};
	template<>
	double string_cast<double, std::wstring>(const std::wstring& obj) {
		return std::stod(obj);
	};
	template<>
	double string_cast<double, char>(const char* obj) {
		return std::stod(std::string(obj));
	};
	template<>
	double string_cast<double, wchar_t>(const wchar_t* obj) {
		return std::stod(std::wstring(obj));
	};
	template<>
	long double string_cast<long double, std::string>(const std::string& obj) {
		return std::stold(obj);
	};
	template<>
	long double string_cast<long double, std::wstring>(const std::wstring& obj) {
		return std::stold(obj);
	};
	template<>
	long double string_cast<long double, char>(const char* obj) {
		return std::stold(std::string(obj));
	};
	template<>
	long double string_cast<long double, wchar_t>(const wchar_t* obj) {
		return std::stold(std::wstring(obj));
	};
	template<>
	bool string_cast<bool, std::string>(const std::string& obj) {
		return (obj == "true");
	};
	template<>
	bool string_cast<bool, std::wstring>(const std::wstring& obj) {
		return (obj == L"true");
	};
	template<>
	bool string_cast<bool, char>(const char* obj) {
		return (obj == "true");
	};
	template<>
	bool string_cast<bool, wchar_t>(const wchar_t* obj) {
		return (obj == L"true");
	};
	//преобразование простых типов к строкам
	template<>
	std::string string_cast<std::string, int>(const int& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, int>(const int& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, long>(const long& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, long>(const long& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, long long>(const long long& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, long long>(const long long& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, unsigned int>(const unsigned int& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, unsigned int>(const unsigned int& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, unsigned long>(const unsigned long& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, unsigned long>(const unsigned long& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, unsigned long long>(const unsigned long long& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, unsigned long long>(const unsigned long long& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, float>(const float& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, float>(const float& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, double>(const double& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, double>(const double& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, long double>(const long double& obj) {
		return std::to_string(obj);
	};
	template<>
	std::wstring string_cast<std::wstring, long double>(const long double& obj) {
		return std::to_wstring(obj);
	};
	template<>
	std::string string_cast<std::string, bool>(const bool& obj) {
		return obj ? "true" : "false";
	};
	template<>
	std::wstring string_cast<std::wstring, bool>(const bool& obj) {
		return obj ? L"true" : L"false";
	};

};


//РАБОТА С ПЕРЕЧИСЛЕНИЯМИ
//макрос-декларатор для better_enum-перечислений
#define UNS_ENUM_DECLARATOR(ENUM_NAME, SPECIFICATION_TYPE, ...)																	\
	BETTER_ENUM(ENUM_NAME, SPECIFICATION_TYPE, __VA_ARGS__)																		\

	//макрос-декларатор специализаций функции string_cast для better_enum-перечислений
	//!!!!!!!!!!!!!use it global-scope only!!!!!!!!!!!!
#define UNS_ENUM_STRING_CAST_DECLARATOR(ENUM_NAME)																				\
	template<>																													\
	ENUM_NAME uns::string_cast<ENUM_NAME, std::string>(const std::string& obj) {												\
		return ENUM_NAME::_from_string(obj.c_str());																			\
	};																															\
	template<>																													\
	ENUM_NAME uns::string_cast<ENUM_NAME, std::wstring>(const std::wstring& obj) {												\
		return ENUM_NAME::_from_string(uns::string_cast<std::string>(obj).c_str());												\
	};																															\
	template<>																													\
	ENUM_NAME uns::string_cast<ENUM_NAME, char>(const char* obj) {																\
		return ENUM_NAME::_from_string(obj);																					\
	};																															\
	template<>																													\
	ENUM_NAME uns::string_cast<ENUM_NAME, wchar_t>(const wchar_t* obj) {														\
		return ENUM_NAME::_from_string(uns::string_cast<std::string>(obj).c_str());												\
	};																															\
	template<>																													\
	std::string uns::string_cast<std::string, ENUM_NAME::_enumerated>(const ENUM_NAME::_enumerated& obj) {						\
		return ENUM_NAME(obj)._to_string();																						\
	};																															\
	template<>																													\
	std::wstring uns::string_cast<std::wstring, ENUM_NAME::_enumerated>(const ENUM_NAME::_enumerated& obj) {					\
		return uns::string_cast<std::wstring>(ENUM_NAME(obj)._to_string());														\
	};																															\
	template<>																													\
	std::string uns::string_cast<std::string, ENUM_NAME>(const ENUM_NAME& obj) {												\
		return obj._to_string();																								\
	};																															\
	template<>																													\
	std::wstring uns::string_cast<std::wstring, ENUM_NAME>(const ENUM_NAME& obj) {												\
		return uns::string_cast<std::wstring>(obj._to_string());																\
	};																															\


//АЛГОРИТМЫ РАБОТЫ С КОНТЕНЕРАМИ
namespace uns::containers::sequential {

	//операции над векторами
	//операция сдвига всех элементов в контейнере
		//(пока поддерживаются только vector и array)
	template<class container_t>
	void Move(container_t& container, int move_to_right) {
		int size = static_cast<int>(container.size());
		int seeker = 0;
		if (move_to_right > 0) {
			for (seeker = size - 1; seeker >= move_to_right; seeker--) {
				if (seeker >= 0 && seeker < size && seeker - move_to_right >= 0 && seeker - move_to_right < size)
					container[seeker] = container[seeker - move_to_right];
			};
		}
		else if (move_to_right < 0) {
			for (seeker = 0; seeker < size + move_to_right; seeker++) {
				if (seeker >= 0 && seeker < size && seeker - move_to_right >= 0 && seeker - move_to_right < size)
					container[seeker] = container[seeker - move_to_right];
			};
		};
	};
	template<class container_t, class content_t>
	void Move(container_t& container, int move_to_right, const content_t& filler_elementh) {
		uns::containers::sequential::Move(container, move_to_right);
		int size = static_cast<int>(container.size());
		int seeker = 0;
		if (move_to_right > 0) {
			for (seeker = move_to_right - 1; seeker >= 0; seeker--) {
				if (seeker >= 0 && seeker < size)
					container[seeker] = filler_elementh;
			};
		}
		else if (move_to_right < 0) {
			for (seeker = size + move_to_right; seeker < size; seeker++) {
				if (seeker >= 0 && seeker < size)
					container[seeker] = filler_elementh;
			};
		};
	};


	//операция перемещения элемента в контейнере без изменения размера контейнера и без потери данных
		//(пока поддерживаются только vector и array)
	template<class container_t>
	void Replace(container_t& container, typename container_t::size_type num_of_el, typename container_t::size_type new_place) {
		if (num_of_el >= container.size()) return;
		if (new_place >= container.size()) return;

		typename container_t::value_type el = container[num_of_el];
		typename container_t::size_type seeker = 0;
		for (seeker = num_of_el; seeker < new_place; seeker++)
			container[seeker] = container[seeker + 1];
		for (seeker = num_of_el; seeker > new_place; seeker--)
			container[seeker] = container[seeker - 1];
		container[new_place] = el;
	};


};