#pragma once

#define UNS_HEADER_MATRIX

#include <concepts>
#include <initializer_list>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#ifndef UNS_HEADER_MATH
#include "uniself/math.h"
#endif
#ifndef UNS_HEADER_STRINGS
#include "uniself/strings.h"
#endif


namespace ublas = boost::numeric::ublas;

namespace uns::math {

	template<typename number_t, typename functor_t = ublas::row_major, typename element_constructor_t = std::vector<number_t>>
	using matrix = ublas::matrix<number_t, functor_t, element_constructor_t>;

	template<typename matrix_t>
	concept ublas_matrix = std::derived_from<matrix_t, ublas::matrix<typename matrix_t::value_type, ublas::row_major, typename matrix_t::array_type>>
		|| std::derived_from<matrix_t, ublas::matrix<typename matrix_t::value_type, ublas::column_major, typename matrix_t::array_type>>;

	template<uns::math::ublas_matrix matrix_t>
	matrix_t make(
		std::initializer_list<std::initializer_list<typename matrix_t::value_type>> list_of_lists
	) {
		auto res = matrix_t{ 0, 0 };

		auto size1 = list_of_lists.size();
		if(size1 == 0) return res;

		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };
		auto list_ptr = list_of_lists.begin();
		auto elem_ptr = list_ptr->begin();
		auto end1_found = false;
		auto end2_found = false;

		auto size2 = list_ptr->size();
		if(size2 == 0) return res;

		res.resize(size1, size2, false);

		for(i1 = 0; i1 < res.size1(); i1++) {
			end2_found = false;
			
			for(i2 = 0; i2 < res.size2(); i2++) {
				end1_found = (end1_found || list_ptr == list_of_lists.end());
				end2_found = (end1_found || end2_found || (!end1_found && elem_ptr == list_ptr->end()));

				if(end1_found || end2_found) {
					res(i1, i2) = static_cast<typename matrix_t::value_type>(0);
				}
				else {
					res(i1, i2) = static_cast<typename matrix_t::value_type>(*elem_ptr);
				};

				if(!end2_found) elem_ptr++;
			};

			if(!end1_found) {
				list_ptr++;
				elem_ptr = list_ptr->begin();
			};
		};

		return res;
	};

};


namespace uns::string {

	//casts to & from u8strings for matrices
	template<std::convertible_to<std::u8string> out_t,uns::math::ublas_matrix matrix_t>
	out_t u8_cast(const matrix_t& mtx) {
		auto res = static_cast<std::u8string>(u8"[");
		res += uns::string::u8_cast<std::u8string>(mtx.size1());
		res += u8" x ";
		res += uns::string::u8_cast<std::u8string>(mtx.size2());
		res += u8"] {\n";

		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };

		auto is_upmost = true;
		auto is_leftmost = true;
		for(i1 = 0; i1 < mtx.size1(); i1++) {
			if(!is_upmost)
				res += u8",\n";
			else
				is_upmost = false;

			res += u8"\t{";

			is_leftmost = true;
			for(i2 = 0; i2 < mtx.size2(); i2++) {
				if(!is_leftmost)
					res += u8", ";
				else
					is_leftmost = false;

				res += uns::string::u8_cast<std::u8string>(mtx(i1, i2));
			};

			res += u8"}";
		};

		res += u8"\n}";

		return res;
	};
	template<uns::math::ublas_matrix matrix_t>
	matrix_t u8_cast(const std::u8string& str) {
		static const auto bracket_op = std::u8string{ u8"[" };
		static const auto mul = std::u8string{ u8"x" };
		static const auto bracket_cl = std::u8string{ u8"]" };
		static const auto space = std::u8string{ u8" " };
		static const auto comma = std::u8string{ u8"," };
		static const auto brace_op = std::u8string{ u8"{" };
		static const auto brace_cl = std::u8string{ u8"}" };

		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };

		auto res = matrix_t{};
		auto read_str = std::u8string{};
		auto seeker = str.begin();

		if(!uns::string::seeker_set(str, seeker, bracket_op, false, -1)) throw std::runtime_error{"Matrix string format violation: a '[' not found"};
		if(!uns::string::seeker_read(str, seeker, read_str, mul, false, -1)) throw std::runtime_error{ "Matrix string format violation: cant read till 'x'" };
		auto size1 = uns::string::u8_cast<typename matrix_t::size_type>(read_str);
		if(!uns::string::seeker_read(str, seeker, read_str, bracket_cl, false, -1)) throw std::runtime_error{ "Matrix string format violation: cant read till ']'" };
		auto size2 = uns::string::u8_cast<typename matrix_t::size_type>(read_str);
		res.resize(size1, size2);

		if(!uns::string::seeker_set(str, seeker, brace_op, false, -1)) throw std::runtime_error{ "Matrix string format violation: a '{' not found" };

		for(i1 = 0; i1 < res.size1(); i1++) {
			if(!uns::string::seeker_set(str, seeker, brace_op, false, -1)) throw std::runtime_error{ "Matrix string format violation: a newline '{' not found" };

			for(i2 = 0; i2 < res.size2(); i2++) {
				if(i2 < res.size2() - 1) {
					if(!uns::string::seeker_read(str, seeker, read_str, comma, false, -1, brace_cl)) throw std::runtime_error{ "Matrix string format violation: cant read till ',' of dim #2" };
					else res(i1, i2) = uns::string::u8_cast<typename matrix_t::value_type>(read_str);
				}
				else {
					if(!uns::string::seeker_read(str, seeker, read_str, brace_cl, false, -1)) throw std::runtime_error{ "Matrix string format violation: cant read till  '}' of dim #2" };
					else res(i1, i2) = uns::string::u8_cast<typename matrix_t::value_type>(read_str);
				};

			};
			if(i1 < res.size1() - 1) {
				if(!uns::string::seeker_set(str, seeker, comma, false, -1, brace_cl)) throw std::runtime_error{ "Matrix string format violation: a ',' of dim #1 not found" };
			}
			else {
				if(!uns::string::seeker_set(str, seeker, brace_cl, false, -1)) throw std::runtime_error{ "Matrix string format violation: a '}' of dim #1 not found" };
			};
		};

		return res;
	};

};


namespace uns::math {

	template<uns::math::ublas_matrix matrix1_t, uns::math::ublas_matrix matrix2_t>
	constexpr bool equals(matrix1_t mtx1, matrix2_t mtx2) noexcept {
		if(mtx1.size1() != mtx2.size1() || mtx1.size2() != mtx2.size2()) return false;

		auto i1 = typename matrix1_t::size_type{ 0 };
		auto i2 = typename matrix1_t::size_type{ 0 };

		for(i1 = 0; i1 < mtx2.size1(); i1++) {
			for(i2 = 0; i2 < mtx2.size2(); i2++) {
				if(!uns::math::equals<typename matrix1_t::value_type, typename matrix2_t::value_type>(mtx1(i1, i2), mtx2(i1, i2))) return false;
			};
		};

		return true;
	};
	template<uns::math::ublas_matrix matrix1_t, uns::math::ublas_matrix matrix2_t, std::convertible_to<typename matrix1_t::value_type> accuracy_t>
	constexpr bool equals(matrix1_t mtx1, matrix2_t mtx2, const accuracy_t accuracy) noexcept {
		if(mtx1.size1() != mtx2.size1() || mtx1.size2() != mtx2.size2()) return false;

		auto i1 = typename matrix1_t::size_type{ 0 };
		auto i2 = typename matrix1_t::size_type{ 0 };

		for(i1 = 0; i1 < mtx2.size1(); i1++) {
			for(i2 = 0; i2 < mtx2.size2(); i2++) {
				if(!uns::math::equals<typename matrix1_t::value_type, typename matrix2_t::value_type, accuracy_t>(mtx1(i1, i2), mtx2(i1, i2), accuracy)) return false;
			};
		};

		return true;
	};

};


namespace uns::math {

	//matrix resize with filling
	template<uns::math::ublas_matrix matrix_t>
	void resize(
		matrix_t& m, 
		const typename matrix_t::size_type new_size1, 
		const typename matrix_t::size_type new_size2,
		const typename matrix_t::value_type filler
	) {
		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };

		auto size1 = m.size1();
		auto size2 = m.size2();

		m.resize(new_size1, new_size2, true);
		for(i1 = size1; i1 < m.size1(); i1++) {
			for(i2 = 0; i2 < size2 && i2 < m.size2(); i2++) {
				m(i1, i2) = filler;
			};
		};
		for(i1 = 0; i1 < m.size1(); i1++) {
			for(i2 = size2; i2 < m.size2(); i2++) {
				m(i1, i2) = filler;
			};
		};
	};


	//fills the matrix with specified value
	template<uns::math::ublas_matrix matrix_t>
	void fill(
		matrix_t& m,
		const typename matrix_t::value_type filler
	) {
		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };
		for(i1 = 0; i1 < m.size1(); i1++) {
			for(i2 = 0; i2 < m.size2(); i2++) {
				m(i1, i2) = filler;
			};
		};
	};


	//addition of matrices of different sizes
	template<uns::math::ublas_matrix matrix_t>
	matrix_t sum(
		const matrix_t& m1,
		const matrix_t& m2
	) {
		matrix_t res(uns::math::max(m1.size1(), m2.size1()), uns::math::max(m1.size2(), m2.size2()));

		auto min1 = uns::math::min(m1.size1(), m2.size1());
		auto min2 = uns::math::min(m1.size2(), m2.size2());

		auto i1 = min1;
		auto i2 = min2;
		for(i1 = 0; i1 < min1; i1++) {
			for(i2 = 0; i2 < min2; i2++) {
				if(!uns::math::equals(m1(i1, i2), -m2(i1, i2))) res(i1, i2) = m1(i1, i2) + m2(i1, i2);
				else res(i1, i2) = typename matrix_t::value_type{ 0 };
			};
			for(i2 = min2; i2 < m1.size2(); i2++) {
				res(i1, i2) = m1(i1, i2);
			};
			for(i2 = min2; i2 < m2.size2(); i2++) {
				res(i1, i2) = m2(i1, i2);
			};
		};
		for(i1 = min1; i1 < m1.size1(); i1++) {
			for(i2 = 0; i2 < m1.size2(); i2++) {
				res(i1, i2) = m1(i1, i2);
			};
		};
		for(i1 = min1; i1 < m2.size1(); i1++) {
			for(i2 = 0; i2 < m2.size2(); i2++) {
				res(i1, i2) = m2(i1, i2);
			};
		};

		return res;
	};


	//subtraction of matrices of different sizes
	template<uns::math::ublas_matrix matrix_t>
	matrix_t sub(
		const matrix_t& m1,
		const matrix_t& m2
	) {
		matrix_t res(uns::math::max(m1.size1(), m2.size1()), uns::math::max(m1.size2(), m2.size2()));

		auto min1 = uns::math::min(m1.size1(), m2.size1());
		auto min2 = uns::math::min(m1.size2(), m2.size2());

		auto i1 = min1;
		auto i2 = min2;
		for(i1 = 0; i1 < min1; i1++) {
			for(i2 = 0; i2 < min2; i2++) {
				if(!uns::math::equals(m1(i1, i2), m2(i1, i2))) res(i1, i2) = m1(i1, i2) - m2(i1, i2);
				else res(i1, i2) = typename matrix_t::value_type{ 0 };
			};
			for(i2 = min2; i2 < m1.size2(); i2++) {
				res(i1, i2) = m1(i1, i2);
			};
			for(i2 = min2; i2 < m2.size2(); i2++) {
				res(i1, i2) = -m2(i1, i2);
			};
		};
		for(i1 = min1; i1 < m1.size1(); i1++) {
			for(i2 = 0; i2 < m1.size2(); i2++) {
				res(i1, i2) = m1(i1, i2);
			};
		};
		for(i1 = min1; i1 < m2.size1(); i1++) {
			for(i2 = 0; i2 < m2.size2(); i2++) {
				res(i1, i2) = -m2(i1, i2);
			};
		};

		return res;
	};


	//production of matrices of different sizes
	template<uns::math::ublas_matrix matrix_t>
	matrix_t prod(
		const matrix_t& m1,
		const matrix_t& m2
	) {
		auto res = matrix_t{ m1.size1(), m2.size2() };
		res *= typename matrix_t::value_type{ 0 };

		auto min = uns::math::min(m1.size2(), m2.size1());
		auto i1 = min;
		auto i2 = min; 
		auto i3 = min;
		for(i1 = 0; i1 < res.size1(); i1++) {
			for(i2 = 0; i2 < res.size2(); i2++) {
				for(i3 = 0; i3 < min; i3++) {
					res(i1, i2) += m1(i1, i3) * m2(i3, i2);
				};
			};
		};

		return res;
	};


	//trace of matrix
	template<uns::math::ublas_matrix matrix_t>
	typename matrix_t::value_type trace(
		const matrix_t& m
	) {
		auto res = typename matrix_t::value_type{ 0 };

		auto min_size = uns::math::min(m.size1(), m.size2());
		auto i = min_size;

		for(i = 0; i < min_size; i++) {
			res += m(i, i);
		};

		return res;
	};


	//returns the identical matrix
	template<uns::math::ublas_matrix matrix_t>
	matrix_t make_identity(
		typename matrix_t::size_type size1,
		typename matrix_t::size_type size2
	) {
		matrix_t res(size1, size2);

		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };

		for(i1 = 0; i1 < res.size1(); i1++)
			for(i2 = 0; i2 < res.size2(); i2++)
				res(i1, i2) = ((i1 == i2) ? typename matrix_t::value_type{ 1 } : typename matrix_t::value_type{ 0 });

		return res;
	};
	template<uns::math::ublas_matrix matrix_t>
	matrix_t make_identity(const matrix_t& m) {
		return uns::math::make_identity<matrix_t>(m.size1(), m.size2());
	};


	//generates an ortogonal matrix based on input
	template<uns::math::ublas_matrix matrix_t>
	matrix_t make_orto(
		const matrix_t& m
	) {
		using result_t = matrix_t;

		if(m.size1() != m.size2()) return uns::math::make_identity<result_t>(m.size1(), m.size2());

		auto i1 = static_cast<typename result_t::size_type>(0);
		auto i2 = static_cast<typename result_t::size_type>(0);
		auto i3 = static_cast<typename result_t::size_type>(0);

		auto res = m;

		const auto _0 = static_cast<typename result_t::value_type>(0);
		const auto _1 = static_cast<typename result_t::value_type>(1);
		auto norm = _0;
		auto koeff = std::vector<typename result_t::value_type>{}; koeff.resize(m.size1());

		for(i3 = 1; i3 <= res.size1(); i3++) {
			//normalizing previous vector
			norm = _0;
			for(i2 = 0; i2 < res.size2(); i2++) {
				norm += res(i3 - 1, i2) * res(i3 - 1, i2);
			};
			norm = uns::math::div(_1, static_cast<typename result_t::value_type>(uns::math::sqrt(norm)));
			for(i2 = 0; i2 < res.size2(); i2++) {
				res(i3 - 1, i2) = res(i3 - 1, i2) * norm;
			};

			if(i3 == res.size1()) break;
			for(auto& el : koeff) {
				el = _0;
			};
			
			//counting koefficients
			for(i1 = 0; i1 < i3; i1++) {
				for(i2 = 0; i2 < res.size2(); i2++) {
					koeff[i1] -= res(i3, i2) * res(i1, i2);
				};
			};
			//counting new vector
			for(i1 = 0; i1 < i3; i1++) {
				for(i2 = 0; i2 < res.size2(); i2++) {
					res(i3, i2) = res(i3, i2) + koeff[i1] * res(i1, i2);
				};
			};
		};

		return res;
	};


	//copying of dim1 units within the same matrix
	template<uns::math::ublas_matrix matrix_t>
	matrix_t trans(
		matrix_t& m
	) {
		auto res = matrix_t{ m.size2(), m.size1() };

		auto i1 = static_cast<typename matrix_t::size_type>(0);
		auto i2 = static_cast<typename matrix_t::size_type>(0);

		for(i1 = 0; i1 < m.size1(); i1++) {
			for(i2 = 0; i2 < m.size2(); i2++) {
				res(i2, i1) = m(i1, i2);
			};
		};

		return res;
	};


	//copying of dim1 units within the same matrix
	template<uns::math::ublas_matrix matrix_t>
	void copy1(
		matrix_t& m,
		typename matrix_t::size_type from1,
		typename matrix_t::size_type to1
	) {
		if(from1 > m.size1() || to1 > m.size1()) return;
		auto i2 = typename matrix_t::size_type{ 0 };
		for(i2 = 0; i2 < m.size2(); i2++)
			m(to1, i2) = m(from1, i2);
	};


	//swap of dim1 units within the same matrix
	template<uns::math::ublas_matrix matrix_t>
	void swap1(
		matrix_t& m,
		typename matrix_t::size_type from1,
		typename matrix_t::size_type to1
	) {
		if(from1 > m.size1() || to1 > m.size1()) return;
		auto i2 = typename matrix_t::size_type{ 0 };
		auto temp = typename matrix_t::value_type{ 0 };
		for(i2 = 0; i2 < m.size2(); i2++) {
			temp = m(to1, i2);
			m(to1, i2) = m(from1, i2);
			m(from1, i2) = temp;
		};
	};


	//copying of dim2 units within the same matrix
	template<uns::math::ublas_matrix matrix_t>
	void copy2(
		matrix_t& m,
		typename matrix_t::size_type from2,
		typename matrix_t::size_type to2
	) {
		if(from2 > m.size2() || to2 > m.size2()) return;
		auto i1 = typename matrix_t::size_type{ 0 };
		for(i1 = 0; i1 < m.size1(); i1++)
			m(i1, to2) = m(i1, from2);
	};


	//swap of dim2 units within the same matrix
	template<uns::math::ublas_matrix matrix_t>
	void swap2(
		matrix_t& m,
		typename matrix_t::size_type from2,
		typename matrix_t::size_type to2
	) {
		if(from2 > m.size2() || to2 > m.size2()) return;
		auto i1 = typename matrix_t::size_type{ 0 };
		auto temp = typename matrix_t::value_type{ 0 };
		for(i1 = 0; i1 < m.size1(); i1++) {
			temp = m(i1, to2);
			m(i1, to2) = m(i1, from2);
			m(i1, from2) = temp;
		};
	};


	//matrix upper triangulation, based on Haussian algorithm
	template<uns::math::ublas_matrix matrix_t>
	typename matrix_t::value_type triangulate(
		matrix_t& m
	) {
		const auto _0 = static_cast<typename matrix_t::value_type>(0);
		const auto _1 = static_cast<typename matrix_t::value_type>(1);
		auto sign = _1;
		auto multiplier = _1;
		auto min_size = uns::math::min<typename matrix_t::size_type>(m.size1(), m.size2());
		auto i1 = static_cast<typename matrix_t::size_type>(0);
		auto i2 = static_cast<typename matrix_t::size_type>(0);
		auto i3 = static_cast<typename matrix_t::size_type>(0);
		auto line_of_nonzero = static_cast<typename matrix_t::size_type>(0);

		for(i2 = 0; i2 < min_size; i2++) {
			if(uns::math::equals<typename matrix_t::value_type>(m(i2, i2), _0)) {
				line_of_nonzero = 0;
				for(i1 = i2 + 1; i1 < m.size1(); i1++) {
					if(!uns::math::equals<typename matrix_t::value_type>(m(i1, i2), _0)) {
						line_of_nonzero = i1;
						break;
					};
				};
				if(line_of_nonzero > i2) {
					uns::math::swap1(m, line_of_nonzero, i2);
					sign *= -_1;
				};
			};
			for(i1 = i2 + 1; i1 < m.size1(); i1++) {
				if(!uns::math::equals<typename matrix_t::value_type>(m(i2, i2), _0)) {
					if(!uns::math::equals<typename matrix_t::value_type>(m(i1, i2), _0)) {
						multiplier = static_cast<typename matrix_t::value_type>(m(i1, i2)) / static_cast<typename matrix_t::value_type>(m(i2, i2));

						for(i3 = i2; i3 < m.size2(); i3++) {
							if(uns::math::equals<typename matrix_t::value_type>(m(i1, i3), multiplier * m(i2, i3))) {
								m(i1, i3) = _0;
							}
							else {
								m(i1, i3) = m(i1, i3) - multiplier * m(i2, i3);
							};
						};
					}
					else {
						m(i1, i2) = _0;
					};
				}
				else {
					m(i2, i2) = _0;
				};
			};
		};

		return sign;
	};


	//determinant of matrix
	template<uns::math::ublas_matrix matrix_t>
	typename matrix_t::value_type det(
		matrix_t m
	) {
		auto res = static_cast<typename matrix_t::value_type>(0);

		if(m.size1() != m.size2()) return res;
		else res = static_cast<typename matrix_t::value_type>(1);

		uns::math::triangulate(m);

		typename matrix_t::size_type i1 = 0;
		for(i1 = 0; i1 < m.size1(); i1++) {
			res *= m(i1, i1);
		};

		return res;
	};

	
	//rank of matrix
	template<uns::math::ublas_matrix matrix_t>
	typename matrix_t::size_type rank(
		matrix_t m
	) {
		uns::math::triangulate(m);

		const auto _0 = static_cast<typename matrix_t::value_type>(0);

		auto i1 = static_cast<typename matrix_t::size_type>(0);
		auto i2 = static_cast<typename matrix_t::size_type>(0);

		auto res = static_cast<typename matrix_t::size_type>(0);

		for(i1 = 0; i1 < m.size1(); i1++) {
			for(i2 = 0; i2 < m.size2(); i2++) {
				if(!uns::math::equals(m(i1, i2), _0)) {
					res++;
					break;
				};
			};
		};

		return res;
	};

	
	//matrix upper triangulation, based on Haussian algorithm
	template<uns::math::ublas_matrix matrix_t>
	void overthrow(
		matrix_t& m
	) {
		const auto size1 = m.size1();
		const auto size2 = m.size2();
		auto i1 = static_cast<typename matrix_t::size_type>(0);
		auto i2 = static_cast<typename matrix_t::size_type>(0);
		auto i1_ = static_cast<typename matrix_t::size_type>(0);
		auto i2_ = static_cast<typename matrix_t::size_type>(0);
		auto for_lines = m.size1() / static_cast<typename matrix_t::size_type>(2);
		auto for_cols = m.size2() / static_cast<typename matrix_t::size_type>(2);
		auto temp = static_cast<typename matrix_t::value_type>(0);

		for(i1 = 0; i1 < for_lines; i1++) {
			for(i2 = 0; i2 < size2; i2++) {
				i1_ = size1 - i1 - 1;
				i2_ = size2 - i2 - 1;
				temp = m(i1, i2);
				m(i1, i2) = m(i1_, i2_);
				m(i1_, i2_) = temp;
			};
		};

		if(m.size1() % static_cast<int>(2) == 1) {
			for(i2 = 0; i2 < for_cols; i2++) {
				i2_ = size2 - i2 - 1;
				temp = m(for_lines, i2);
				m(for_lines, i2) = m(for_lines, i2_);
				m(for_lines, i2_) = temp;
			};
		};
	};


	//generates a fully-inverted matrix
	template<uns::math::ublas_matrix matrix_t>
	bool inverse(
		const matrix_t& init,
		matrix_t& res
	) {
		if(init.size1() != init.size2()) return false;

		auto i1 = static_cast<typename matrix_t::size_type>(0);
		auto i2 = static_cast<typename matrix_t::size_type>(0);
		auto i3 = static_cast<typename matrix_t::size_type>(0);
		auto line_of_nonzero = static_cast<typename matrix_t::size_type>(0);

		const auto _0 = static_cast<typename matrix_t::value_type>(0);
		const auto _1 = static_cast<typename matrix_t::value_type>(1);
		auto koeff = _1;

		auto init_copy = init;
		res = uns::math::make_identity(init);

		for(short int phase = 1; phase <= 2; phase++) {
			if(phase == 2) {
				uns::math::overthrow(init_copy);
				uns::math::overthrow(res);
			};

			for(i2 = 0; i2 < init_copy.size2(); i2++) {
				if(uns::math::equals(init_copy(i2, i2), _0)) {
					line_of_nonzero = 0;
					i1 = 0;
					for(i1 = i2 + 1; i1 < init_copy.size1(); i1++) {
						if(!uns::math::equals(init_copy(i1, i2), _0)) {
							line_of_nonzero = i1;
							break;
						};
					};
					if(i1 == init_copy.size1()) {
						return false;
					};
					if(line_of_nonzero > i2) {
						uns::math::swap1(init_copy, line_of_nonzero, i2);
						uns::math::swap1(res, line_of_nonzero, i2);
					};
				};
				for(i1 = i2 + 1; i1 < init_copy.size1(); i1++) {
					if(!uns::math::equals(init_copy(i1, i2), _0)) {
						koeff = init_copy(i1, i2) / init_copy(i2, i2);
						for(i3 = 0; i3 < init_copy.size2(); i3++) {
							if(uns::math::equals(init_copy(i1, i3), koeff * init_copy(i2, i3))) {
								init_copy(i1, i3) = _0;
							}
							else {
								init_copy(i1, i3) = init_copy(i1, i3) - koeff * init_copy(i2, i3);
							};

							if(uns::math::equals(res(i1, i3), koeff * res(i2, i3))) {
								res(i1, i3) = _0;
							}
							else {
								res(i1, i3) = res(i1, i3) - koeff * res(i2, i3);
							};
						};
					}
					else {
						init_copy(i1, i2) = _0;
					};
				};
			};
		};

		for(i1 = 0; i1 < init_copy.size1(); i1++) {
			for(i2 = 0; i2 < init_copy.size2(); i2++) {
				if(!uns::math::equals(init_copy(i1, i1), _0)) {
					res(i1, i2) = res(i1, i2) / init_copy(i1, i1);
				}
				else {
					return false;
				};
			};
		};

		uns::math::overthrow(res);
		return true;
	};


};