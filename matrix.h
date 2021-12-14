#pragma once

#include <vector>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "uniself/algorithms.h"
#include "uniself/mathematics.h"
#include "uniself/strings.h"


namespace ublas = boost::numeric::ublas;


namespace uns {
	namespace math {


		//перевод в строку
		//шаблон результата [m.size1(), m.size2()]<start_delimiter>m(0,0)<common_delimiter>m(0,1)<common_delimiter>...<common_delimiter>m(0,m.size2()-1)<line_delimiter>m(1,0)<common_delimiter>m(1,1)<common_delimiter>...<common_delimiter>m(1,m.size2()-1)<line_delimiter>...<line_delimiter>m(m.size1()-1,0)<common_delimiter>m(m.size1()-1,1)<common_delimiter>...<common_delimiter>m(m.size1()-1,m.size2()-1)<finish_delimiter>
		//пример по умолчанию [3,3]{{0.000000,1.000000,2.000000},{3.000000,4.000000,5.000000},{6.000000,7.000000,8.000000}}
		template<class string_t, class matrix_t>
		string_t matrix_to_string(
			const matrix_t& m,
			const string_t& start_delimiter = uns::string_cast<string_t>("{{"),
			const string_t& common_delimiter = uns::string_cast<string_t>(","),
			const string_t& line_delimiter = uns::string_cast<string_t>("},{"),
			const string_t& finish_delimiter = uns::string_cast<string_t>("}}")
		) {
			string_t res = string_t();

			res = uns::string_cast<string_t>("[") + uns::string_cast<string_t>(m.size1()) + uns::string_cast<string_t>(",") + uns::string_cast<string_t>(m.size2()) + uns::string_cast<string_t>("]");
			res += start_delimiter;
			typename matrix_t::size_type i1 = 0, i2 = 0;
			for (i1 = 0; i1 < m.size1(); i1++) {
				for (i2 = 0; i2 < m.size2(); i2++)
					res += uns::string_cast<string_t>(m(i1, i2)) + ((i2 < m.size2() - 1) ? common_delimiter : string_t());
				res += (i1 < m.size1() - 1) ? line_delimiter : string_t();
			};
			res += finish_delimiter;

			return res;
		};


		//парсер строки, содержащей матрицу в формате
		//шаблон результата [m.size1(), m.size2()]<start_delimiter>m(0,0)<common_delimiter>m(0,1)<common_delimiter>...<common_delimiter>m(0,m.size2()-1)<line_delimiter>m(1,0)<common_delimiter>m(1,1)<common_delimiter>...<common_delimiter>m(1,m.size2()-1)<line_delimiter>...<line_delimiter>m(m.size1()-1,0)<common_delimiter>m(m.size1()-1,1)<common_delimiter>...<common_delimiter>m(m.size1()-1,m.size2()-1)<finish_delimiter>
		//пример по умолчанию [3,3]{{0.000000,1.000000,2.000000},{3.000000,4.000000,5.000000},{6.000000,7.000000,8.000000}}
		template<class string_t, class number_t, class functor_t, class element_constructor_t>
		bool string_to_matrix(
			boost::numeric::ublas::matrix<number_t, functor_t, element_constructor_t>& res,
			const string_t& str,
			const string_t& start_delimiter = uns::string_cast<string_t>("{{"),
			const string_t& common_delimiter = uns::string_cast<string_t>(","),
			const string_t& line_delimiter = uns::string_cast<string_t>("},{"),
			const string_t& finish_delimiter = uns::string_cast<string_t>("}}")
		) {
			res.resize(0, 0, false);
			auto str_w = uns::string_cast<string_t>("");
			typename boost::numeric::ublas::matrix<number_t, functor_t, element_constructor_t>::size_type size1 = 0, size2 = 0;

			size_t pos = 0;
			if (!uns::string::SeekerSet<string_t>(str, pos, uns::string_cast<string_t>("["), false, -1, start_delimiter) || !uns::string::SeekerRead<string_t>(str, pos, str_w, uns::string_cast<string_t>(","), false, -1, start_delimiter)) return false;
			else size1 = uns::string_cast<boost::numeric::ublas::matrix<number_t, functor_t, element_constructor_t>::size_type>(str_w);
			if (!uns::string::SeekerRead<string_t>(str, pos, str_w, uns::string_cast<string_t>("]"), false, -1, start_delimiter)) return false;
			else size2 = uns::string_cast<boost::numeric::ublas::matrix<number_t, functor_t, element_constructor_t>::size_type>(str_w);

			res.resize(size1, size2, false);
			if (!uns::string::SeekerSet<string_t>(str, pos, start_delimiter, false, -1)) return false;
			typename boost::numeric::ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0;
			for (i1 = 0; i1 < res.size1(); i1++) {
				for (i2 = 0; i2 < res.size2(); i2++) {
					if (i2 < res.size2() - 1)
						if (!uns::string::SeekerRead<string_t>(str, pos, str_w, common_delimiter, false, -1, line_delimiter)) return false;
						else res(i1, i2) = uns::string_cast<number_t>(str_w);
					else
						if (i1 < res.size1() - 1)
							if (!uns::string::SeekerRead<string_t>(str, pos, str_w, line_delimiter, false, -1, finish_delimiter)) return false;
							else res(i1, i2) = uns::string_cast<number_t>(str_w);
						else
							if (!uns::string::SeekerRead<string_t>(str, pos, str_w, finish_delimiter, false, 0)) return false;
							else res(i1, i2) = uns::string_cast<number_t>(str_w);
				};
			};

			return true;
		};


		//Изменение размера матрицы с заполнением новых ячеек
		template<class number_t, class functor_t, class element_constructor_t>
		void Resize(ublas::matrix<number_t, functor_t, element_constructor_t>& m, size_t new_size1, size_t new_size2, number_t filler) {
			size_t size1 = m.size1();
			size_t size2 = m.size2();

			m.resize(new_size1, new_size2, true);
			for (size_t i1 = size1; i1 < m.size1(); i1++)
				for (size_t i2 = 0; i2 < size2 && i2 < m.size2(); i2++)
					m(i1, i2) = filler;
			for (size_t i1 = 0; i1 < m.size1(); i1++)
				for (size_t i2 = size2; i2 < m.size2(); i2++)
					m(i1, i2) = filler;
		};


		//проверка матриц на равенство
		template<class number_t, class functor_t, class element_constructor_t>
		bool Equals(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m1,
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m2
		) {
			if (m1.size1() != m2.size1() || m1.size2() != m2.size2()) return false;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0;
			for (i1 = 0; i1 < m1.size1(); i1++)
				for (i2 = 0; i2 < m1.size2(); i2++)
					if (!uns::math::Equals(m1(i1, i2), m2(i1, i2))) return false;
			return true;
		};


		//сложение матриц
		template<class number_t, class functor_t, class element_constructor_t>
		ublas::matrix<number_t, functor_t, element_constructor_t> Sum(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m1,
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m2
		) {
			ublas::matrix<number_t, functor_t, element_constructor_t> res(fmax(m1.size1(), m2.size1()), fmax(m1.size2(), m2.size2()));

			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type min1 = fmin(m1.size1(), m2.size1());
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type min2 = fmin(m1.size2(), m2.size2());

			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0;
			for (i1 = 0; i1 < min1; i1++) {
				for (i2 = 0; i2 < min2; i2++)
					if (!uns::math::Equals(m1(i1, i2), -m2(i1, i2))) res(i1, i2) = m1(i1, i2) + m2(i1, i2);
					else res(i1, i2) = number_t(0);
				for (i2 = min2; i2 < m1.size2(); i2++)
					res(i1, i2) = m1(i1, i2);
				for (i2 = min2; i2 < m2.size2(); i2++)
					res(i1, i2) = m2(i1, i2);
			};
			for (i1 = min1; i1 < m1.size1(); i1++) {
				for (i2 = 0; i2 < m1.size2(); i2++)
					res(i1, i2) = m1(i1, i2);
			};
			for (i1 = min1; i1 < m2.size1(); i1++) {
				for (i2 = 0; i2 < m2.size2(); i2++)
					res(i1, i2) = m2(i1, i2);
			};
			return res;
		};


		//вычитание матриц
		template<class number_t, class functor_t, class element_constructor_t>
		ublas::matrix<number_t, functor_t, element_constructor_t> Sub(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m1,
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m2
		) {
			ublas::matrix<number_t, functor_t, element_constructor_t> res(fmax(m1.size1(), m2.size1()), fmax(m1.size2(), m2.size2()));

			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type min1 = fmin(m1.size1(), m2.size1());
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type min2 = fmin(m1.size2(), m2.size2());

			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0;
			for (i1 = 0; i1 < min1; i1++) {
				for (i2 = 0; i2 < min2; i2++)
					if (!uns::math::Equals(m1(i1, i2), m2(i1, i2))) res(i1, i2) = m1(i1, i2) - m2(i1, i2);
					else res(i1, i2) = number_t(0);
				for (i2 = min2; i2 < m1.size2(); i2++)
					res(i1, i2) = m1(i1, i2);
				for (i2 = min2; i2 < m2.size2(); i2++)
					res(i1, i2) = -m2(i1, i2);
			};
			for (i1 = min1; i1 < m1.size1(); i1++) {
				for (i2 = 0; i2 < m1.size2(); i2++)
					res(i1, i2) = m1(i1, i2);
			};
			for (i1 = min1; i1 < m2.size1(); i1++) {
				for (i2 = 0; i2 < m2.size2(); i2++)
					res(i1, i2) = -m2(i1, i2);
			};
			return res;
		};


		//умножение матриц
		template<class number_t, class functor_t, class element_constructor_t>
		ublas::matrix<number_t, functor_t, element_constructor_t> Prod(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m1,
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m2
		) {
			ublas::matrix<number_t, functor_t, element_constructor_t> res(m1.size1(), m2.size2());
			res *= number_t(0);
			
			auto min = static_cast<typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type>(fmin(m1.size2(), m2.size1()));
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0, i3 = 0;
			for (i1 = 0; i1 < res.size1(); i1++)
				for (i2 = 0; i2 < res.size2(); i2++)
					for (i3 = 0; i3 < min; i3++)
						res(i1, i2) = res(i1, i2) + m1(i1, i3) * m2(i3, i2);
			return res;
		};

		//копирование строки матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		void LinesCopy(
			ublas::matrix<number_t, functor_t, element_constructor_t>& m,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type line_from,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type line_to
		) {
			if (line_from > m.size1() || line_to > m.size1()) return;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i2 = 0;
			for (i2 = 0; i2 < m.size2(); i2++)
				m(line_to, i2) = m(line_from, i2);
		};


		//перемена местами строк матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		void LinesSwap(
			ublas::matrix<number_t, functor_t, element_constructor_t>& m,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type line_from,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type line_to
		) {
			if (line_from > m.size1() || line_to > m.size1()) return;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i2 = 0;
			number_t temp = number_t(0);
			for (i2 = 0; i2 < m.size2(); i2++) {
				temp = m(line_to, i2);
				m(line_to, i2) = m(line_from, i2);
				m(line_from, i2) = temp;
			};
		};


		//копирование колонки матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		void ColsCopy(
			ublas::matrix<number_t, functor_t, element_constructor_t>& m,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type col_from,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type col_to
		) {
			if (col_from > m.size2() || col_to > m.size2()) return;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0;
			for (i1 = 0; i1 < m.size1(); i1++)
				m(i1, col_to) = m(i1, col_from);
		};


		//перемена местами колонок матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		void ColsSwap(
			ublas::matrix<number_t, functor_t, element_constructor_t>& m,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type col_from,
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type col_to
		) {
			if (col_from > m.size2() || col_to > m.size2()) return;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0;
			number_t temp = number_t(0);
			for (i1 = 0; i1 < m.size1(); i1++) {
				temp = m(i1, col_to);
				m(i1, col_to) = m(i1, col_from);
				m(i1, col_from) = temp;
			};
		};


		//след матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		number_t Trace(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m
		) {
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type min_dim = fmin(m.size1(), m.size2()), i = 0;
			number_t res = number_t(0);
			for (i = 0; i < min_dim; i++)
				res += m(i, i);
			return res;
		};


		//переворот матрицы вокруг центрального элемента
		template<class number_t, class functor_t, class element_constructor_t>
		void Overthrow(
			ublas::matrix<number_t, functor_t, element_constructor_t>& m
		) {
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type size1 = m.size1(), size2 = m.size2(), i1 = 0, i2 = 0, i1_ = 0, i2_ = 0, for_lines = m.size1() / static_cast<int>(2), for_cols = m.size2() / static_cast<int>(2);
			number_t temp = number_t(0);
			for (i1 = 0; i1 < for_lines; i1++)
				for (i2 = 0; i2 < size2; i2++) {
					i1_ = size1 - i1 - 1;
					i2_ = size2 - i2 - 1;
					temp = m(i1, i2);
					m(i1, i2) = m(i1_, i2_);
					m(i1_, i2_) = temp;
				};
			if (m.size1() % static_cast<int>(2) == 1) {
				for (i2 = 0; i2 < for_cols; i2++) {
					i2_ = size2 - i2 - 1;
					temp = m(for_lines, i2);
					m(for_lines, i2) = m(for_lines, i2_);
					m(for_lines, i2_) = temp;

				};
			};
		};

		
		//процедура диагонализации матрицы с сохранением детерминанта по методу Гаусса
		template<class number_t, class functor_t, class element_constructor_t>
		number_t Triangulate(
			ublas::matrix<number_t, functor_t, element_constructor_t>& m
		) {
			const number_t number_zero = number_t(0), number_1 = number_t(1); 
			number_t sign = number_1;
			number_t koeff = number_1;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type min_dim = fmin(m.size1(), m.size2());
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0, i3 = 0;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type line_of_nonzero = 0;

			for (i2 = 0; i2 < min_dim; i2++) {
				if (uns::math::Equals(m(i2, i2), number_zero)) {
					line_of_nonzero = 0;
					for (i1 = i2 + 1; i1 < m.size1(); i1++)
						if (!uns::math::Equals(m(i1, i2), number_zero)) {
							line_of_nonzero = i1;
							break;
						};
					if (line_of_nonzero > i2) {
						uns::math::LinesSwap(m, line_of_nonzero, i2);
						sign *= -number_1;
					};
				};
				for (i1 = i2 + 1; i1 < m.size1(); i1++)
					if (!uns::math::Equals(m(i2, i2), number_zero)) {
						if (!uns::math::Equals(m(i1, i2), number_zero)) {
							koeff = m(i1, i2) / m(i2, i2);
							for (i3 = i2; i3 < m.size2(); i3++)
								if (uns::math::Equals(m(i1, i3), koeff * m(i2, i3))) m(i1, i3) = number_zero;
								else m(i1, i3) = m(i1, i3) - koeff * m(i2, i3);
						}
						else {
							m(i1, i2) = number_zero;
						};
					}
					else {
						m(i2, i2) = number_zero;
					};
			};

			return sign;
		};


		//детерминант матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		number_t Det(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m
		) {
			if (m.size1() != m.size2()) return number_t(0);
			ublas::matrix<number_t, functor_t, element_constructor_t> res_m = m;
			number_t res = uns::math::Triangulate(res_m);
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0;
			for (i1 = 0; i1 < res_m.size1(); i1++)
				res *= res_m(i1, i1);
			return res;
		};


		//ранг матрицы
		template<class number_t, class functor_t, class element_constructor_t>
		size_t Rank(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m
		) {
			ublas::matrix<number_t, functor_t, element_constructor_t> res_m = m;
			uns::math::Triangulate(res_m);
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0;
			size_t res = 0;
			bool non_zero = false;
			const number_t number_zero = number_t(0);

			for (i1 = 0; i1 < res_m.size1(); i1++)
			{
				non_zero = false;
				for (i2 = 0; i2 < res_m.size2(); i2++)
					non_zero = non_zero || !uns::math::Equals(res_m(i1, i2), number_zero);
				res += non_zero ? 1 : 0;
			};
			return res;
		};


		//возвращает единичную матрицу
		template<class number_t, class functor_t, class element_constructor_t>
		ublas::matrix<number_t, functor_t, element_constructor_t> Make_I(
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type size1, 
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type size2
		) {
			ublas::matrix<number_t, functor_t, element_constructor_t> res(size1, size2);
			const number_t number_zero = number_t(0), number_1 = number_t(1);
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0;

			for (i1 = 0; i1 < res.size1(); i1++)
				for (i2 = 0; i2 < res.size2(); i2++)
					res(i1, i2) = (i1 == i2) ? number_1 : number_zero;

			return res;
		};
		template<class number_t, class functor_t, class element_constructor_t>
		ublas::matrix<number_t, functor_t, element_constructor_t> Make_I(const ublas::matrix<number_t, functor_t, element_constructor_t>& m) {
			return uns::math::Make_I<number_t>(m.size1(), m.size2());
		};


		//генерирует ортогональную матрицу, на основе введенной
		template<class number_t, class functor_t, class element_constructor_t>
		ublas::matrix<number_t, functor_t, element_constructor_t> Make_Orto(
			const ublas::matrix<number_t, functor_t, element_constructor_t>& m
		) {
			ublas::matrix<number_t, functor_t, element_constructor_t> res = m;
			if (m.size1() != m.size2()) return res;

			const number_t number_zero = number_t(0), number_1 = number_t(1);
			number_t norm = number_zero;
			std::vector<number_t> koeff(m.size1());
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0, i3 = 0;
			for (i3 = 1; i3 <= res.size1(); i3++) {
				//нормируем предыдущий вектор
				norm = number_zero;
				for (i2 = 0; i2 < res.size2(); i2++) {
					norm += res(i3 - 1, i2) * res(i3 - 1, i2);
				};
				norm = uns::math::Div(number_1, static_cast<number_t>(sqrt(norm)));
				for (i2 = 0; i2 < res.size2(); i2++) {
					res(i3 - 1, i2) = res(i3 - 1, i2) * norm;
				};

				if (i3 == res.size1()) break;
				std::fill(koeff.begin(), koeff.end(), number_zero);
				//считаем коэфициенты
				for (i1 = 0; i1 < i3; i1++)
					for (i2 = 0; i2 < res.size2(); i2++)
						koeff[i1] -= res(i3, i2) * res(i1, i2);
				//считаем новый вектор
				for (i1 = 0; i1 < i3; i1++)
					for (i2 = 0; i2 < res.size2(); i2++)
						res(i3, i2) = res(i3, i2) + koeff[i1] * res(i1, i2);
			};

			return res;
		};


		//вычисляет обратную матрицу методом гаусса
		template<class number_t, class functor_t, class element_constructor_t>
		bool Inverse(
			ublas::matrix<number_t, functor_t, element_constructor_t>& res, 
			const ublas::matrix<number_t, functor_t, element_constructor_t>& init
		) {
			if (init.size1() != init.size2()) return false;
			ublas::matrix<number_t, functor_t, element_constructor_t> init_copy = init;
			res = uns::math::Make_I<number_t, functor_t, element_constructor_t>(init.size1(), init.size2());

			const number_t number_zero = number_t(0), number_1 = number_t(1);
			number_t koeff = number_1;
			typename ublas::matrix<number_t, functor_t, element_constructor_t>::size_type i1 = 0, i2 = 0, i3 = 0, line_of_nonzero = 0;

			for (short int phase = 1; phase <= 2; phase++) {
				if (phase == 2) {
					uns::math::Overthrow(init_copy);
					uns::math::Overthrow(res);
				};

				for (i2 = 0; i2 < init_copy.size2(); i2++) {
					if (uns::math::Equals(init_copy(i2, i2), number_zero)) {
						line_of_nonzero = 0;
						i1 = 0;
						for (i1 = i2 + 1; i1 < init_copy.size1(); i1++)
							if (!uns::math::Equals(init_copy(i1, i2), number_zero)) {
								line_of_nonzero = i1;
								break;
							};
						if (i1 == init_copy.size1()) return false;
						if (line_of_nonzero > i2) {
							uns::math::LinesSwap(init_copy, line_of_nonzero, i2);
							uns::math::LinesSwap(res, line_of_nonzero, i2);
						};
					};
					for (i1 = i2 + 1; i1 < init_copy.size1(); i1++)
						if (!uns::math::Equals(init_copy(i1, i2), number_zero)) {
							koeff = init_copy(i1, i2) / init_copy(i2, i2);
							for (i3 = 0; i3 < init_copy.size2(); i3++) {
								if (uns::math::Equals(init_copy(i1, i3), koeff * init_copy(i2, i3))) init_copy(i1, i3) = number_zero;
								else init_copy(i1, i3) = init_copy(i1, i3) - koeff * init_copy(i2, i3);
								if (uns::math::Equals(res(i1, i3), koeff * res(i2, i3))) res(i1, i3) = number_zero;
								else res(i1, i3) = res(i1, i3) - koeff * res(i2, i3);
							};
						}
						else {
							init_copy(i1, i2) = number_zero;
						};
				};
			};

			for (i1 = 0; i1 < init_copy.size1(); i1++)
				for (i2 = 0; i2 < init_copy.size2(); i2++)
					if (!uns::math::Equals(init_copy(i1, i1), number_zero)) res(i1, i2) = res(i1, i2) / init_copy(i1, i1);
					else return false;

			uns::math::Overthrow(res);
			return true;
		};



	};

};


