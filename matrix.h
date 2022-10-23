#pragma once

#include <concepts>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "uniself/math.h"
#include "uniself/strings.h"


namespace ublas = boost::numeric::ublas;

namespace uns::math {

	template<typename number_t, typename functor_t = ublas::row_major, typename element_constructor_t = std::vector<number_t>>
	using matrix = ublas::matrix<number_t, functor_t, element_constructor_t>;

	template<typename matrix_t>
	concept ublas_matrix = std::derived_from<matrix_t, ublas::matrix<typename matrix_t::value_type, ublas::row_major, typename matrix_t::array_type>>
		|| std::derived_from<matrix_t, ublas::matrix<typename matrix_t::value_type, ublas::column_major, typename matrix_t::array_type>>;

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

		auto is_upmost = true;
		auto is_leftmost = true;
		for(int i1 = 0; i1 < mtx.size1(); i1++) {
			if(!is_upmost)
				res += u8",\n";
			else
				is_upmost = false;

			res += u8"\t{";

			is_leftmost = true;
			for(int i2 = 0; i2 < mtx.size2(); i2++) {
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
		static const auto mul = std::u8string{ u8" x " };
		static const auto bracket_cl = std::u8string{ u8"]" };
		static const auto space = std::u8string{ u8" " };
		static const auto comma = std::u8string{ u8"," };
		static const auto comma_space = std::u8string{ u8", " };
		static const auto brace_op = std::u8string{ u8"{" };
		static const auto brace_cl = std::u8string{ u8"}" };

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

		for(int i1 = 0; i1 < res.size1(); i1++) {
			if(!uns::string::seeker_set(str, seeker, brace_op, false, -1)) throw std::runtime_error{ "Matrix string format violation: a newline '{' not found" };

			for(int i2 = 0; i2 < res.size2(); i2++) {
				if(i2 < res.size2() - 1) {
					if(!uns::string::seeker_read(str, seeker, read_str, comma_space, false, -1, brace_cl)) throw std::runtime_error{ "Matrix string format violation: cant read till ',' of dim #2" };
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

namespace uns::math::linear {

	//matrix resize with filling
	template<uns::math::ublas_matrix matrix_t>
	void resize(
		matrix_t& m, 
		typename matrix_t::size_type new_size1, 
		typename matrix_t::size_type new_size2, 
		typename matrix_t::value_type filler
	) {
		auto size1 = m.size1();
		auto size2 = m.size2();

		m.resize(new_size1, new_size2, true);
		for(auto i1 = size1; i1 < m.size1(); i1++) {
			for(auto i2 = 0; i2 < size2 && i2 < m.size2(); i2++) {
				m(i1, i2) = filler;
			};
		};
		for(auto i1 = 0; i1 < m.size1(); i1++) {
			for(auto i2 = size2; i2 < m.size2(); i2++) {
				m(i1, i2) = filler;
			};
		};
	};


	//fills the matrix with specified value
	template<uns::math::ublas_matrix matrix_t>
	void fill(
		matrix_t& m,
		typename matrix_t::value_type filler
	) {
		auto i1 = typename matrix_t::size_type{ 0 };
		auto i2 = typename matrix_t::size_type{ 0 };
		for(i1 = 0; i1 < m.size1(); i1++) {
			for(i2 = 0; i2 < m.size2(); i2++) {
				m(i1, i2) = filler;
			};
		};
	};


	//addition of matrices fo different sizes
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


	//trace of matrix
	template<uns::math::ublas_matrix matrix_t>
	typename matrix_t::value_type trace(
		const matrix_t& m
	) {
		auto res = typename matrix_t::value_type{ 0 };

		auto min_dim = uns::math::min(m.size1(), m.size2());
		auto i = min_dim;

		for(i = 0; i < min_dim; i++) {
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
		return uns::math::linear::make_identity<matrix_t>(m.size1(), m.size2());
	};
};