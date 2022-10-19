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

