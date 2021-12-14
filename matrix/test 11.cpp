// test 11.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>

//#include "uniself/strings.h"
#include "uniself/matrix/matrix.h"

/*namespace uns {
	template<class string_t, class number_r, class functor_t, class element_constructor_t>
	string_t unified_cast(const boost::numeric::ublas::matrix<number_r, functor_t, element_constructor_t>& m) {
		string_t res = string_t();

		return res;
	};
};*/

/*template<class number_t, class functor_t, class element_constructor_t>
void Fill(boost::numeric::ublas::matrix<number_t, functor_t, element_constructor_t>& m) {
	for (unsigned i1 = 0; i1 < m.size1(); i1++)
		for (unsigned i2 = 0; i2 < m.size2(); i2++)
			m(i1, i2) = uns::unified_cast<number_t>(m.size2() * i1 + i2);
};*/
template<class number_t>
void Fill(boost::numeric::ublas::matrix<number_t>& m) {
	typename boost::numeric::ublas::matrix<number_t>::size_type i1 = 0, i2 = 0;
	number_t arg1 = 0;
	number_t arg2 = 0;
	for (i1 = 0; i1 < m.size1(); i1++)
		for (i2 = 0; i2 < m.size2(); i2++) {
			arg1 = static_cast<number_t>(i1);
			arg2 = static_cast<number_t>(i2);
			m(i1, i2) = 2.5 * arg1 * arg1 - 7 * arg1 - 1.3 * arg2 * arg2 + 2.11 * arg1 + 0.1 * arg1 * arg2 - 2.3;
		};
};

int main() {
	std::cout << "START" << std::endl;


	/*boost::numeric::ublas::matrix<double> m1(3, 3), m2(3, 3), m3(3, 3);
	Fill(m1);
	Fill(m2); //m2 *= 0.5;
	Fill(m3); //m3 *= 0.5;

	auto str1 = uns::math::matrix_to_string<std::wstring>(m1, L"\n{", L",", L"\n", L"}\n");
	std::wcout << str1 << std::endl;

	auto res = uns::math::Inverse<double>(m2, m1);

	auto str2 = uns::math::matrix_to_string<std::wstring>(m2, L"\n{", L",", L"\n", L"}\n");
	std::wcout << str2 << std::endl;

	m3 = uns::math::Prod(m1, m2);

	std::wcout << std::boolalpha << res << "\n" << uns::math::matrix_to_string<std::wstring>(m3, L"\n{", L",", L"\n", L"}\n") << std::endl;*/

	ublas::matrix<float> m(2, 2);
	std::string str = "\n";
	str += "[3,3]\n";
	str += "{{1.2, -3, 4.0},\n";
	str += "{0.0, 1, -0.1},\n";
	str += "{-5.55, 10, 4000.0}}\n";

	std::cout << std::boolalpha << uns::math::string_to_matrix<std::string>(m, str, "{{", ",", "},\n{", "}}") << std::endl;

	//m.resize(4, 4, true);
	uns::math::Resize(m, 2U, 4U, 0.0F);

	auto str0 = uns::math::matrix_to_string<std::string>(m, "\n{{", ",", "},\n{", "}}\n");

	std::cout << str0 << std::endl;


	std::cout << "FINISH" << std::endl;
}