#pragma once

#include <iostream>
#include <limits>
#include <concepts>

#include "uniself/math.hpp"

#ifndef UNS_LIB_DATETIME
#define UNS_LIB_DATETIME

	/*
namespace uns {


	template<int16_t bin_exp>
	class fixed_point {
	public:
		using base_tipe = int16_t;
		using binary_exponent_type = int16_t;
		using floating_type = float;
	public:
		static constexpr binary_exponent_type binary_exp;
		static constexpr binary_exponent_type binary_digits;
	protected:
		base_tipe m_val = 0;
	public:
		fixed_point() noexcept = default;
		fixed_point(const ::uns::fixed_point<bin_exp>& obj) noexcept :
			m_val(obj.m_val)
		{};
		::uns::fixed_point<bin_exp>& operator=(const ::uns::fixed_point<bin_exp>& obj) noexcept {
			if(this == &obj) return *this;

			m_val = obj.m_val;

			return *this;
		};
		fixed_point(::uns::fixed_point<bin_exp>&& obj) noexcept :
			m_val(obj.m_val)
		{};
		::uns::fixed_point<bin_exp>&& operator=(::uns::fixed_point<bin_exp>&& obj) noexcept {
			if(this == &obj) return *this;

			::std::swap(m_val, obj.m_val);

			return *this;
		};
		~fixed_point() noexcept = default;
	public:
		static ::uns::fixed_point<bin_exp> make(base_tipe base) noexcept {
			auto res = ::uns::fixed_point<bin_exp>{};

			res.m_val = base;

			return res;
		};
	};
	template<int16_t bin_exp> static constexpr typename ::uns::fixed_point<bin_exp>::binary_exponent_type binary_exp = bin_exp;
	template<int16_t bin_exp> static constexpr typename ::uns::fixed_point<bin_exp>::binary_exponent_type binary_digits = sizeof(typename ::uns::fixed_point<bin_exp>::base_tipe) * 8;
	
};
*/

namespace uns::fixed_point {

	template<int8_t unit_value, ::std::signed_integral base_type_t>
		requires (!::std::is_same<int64_t, base_type_t>::value)//TODO instead of int64_t it is supposed to be a signed integer type with maximal size supported with current platform
	class q1 {
	public:
		using base_type = base_type_t;
		using represent_type = float;
		using unit_value_type = int8_t;
	protected:
		using widest_type = int64_t;//TODO instead of int64_t it is supposed to be a signed integer type with maximal size supported with current platform
	protected:
		base_type m_frac = 0;
	protected:
		q1(base_type fraction) noexcept :
			m_frac(fraction)
		{};
	public:


		constexpr static unit_value_type value_bdigits() noexcept {
			return (sizeof(base_type) * 8) - 1;
		};
	public:
		template<unit_value_type arg_unit_value>
			requires (
				static_cast<widest_type>(unit_value) - static_cast<widest_type>(arg_unit_value)
					>= static_cast<widest_type>(::std::numeric_limits<unit_value_type>::min())
			)
			&& (
				static_cast<widest_type>(arg_unit_value) - static_cast<widest_type>(unit_value)
					>= static_cast<widest_type>(::std::numeric_limits<unit_value_type>::min())
			)
		::uns::fixed_point::q1<::uns::math::maximal<unit_value_type>(unit_value, arg_unit_value), base_type> accurate_add(
			const ::uns::fixed_point::q1<arg_unit_value, base_type>& arg
		) const noexcept {
			if constexpr(static_cast<widest_type>(unit_value) - static_cast<widest_type>(arg_unit_value) > static_cast<widest_type>(value_bdigits())) {
				return *this;
			}
			else if constexpr((static_cast<widest_type>(arg_unit_value) - static_cast<widest_type>(unit_value)) > static_cast<widest_type>(value_bdigits())) {
				return arg;
			}
			else if constexpr(static_cast<widest_type>(unit_value) >= static_cast<widest_type>(arg_unit_value)) {
				return { m_frac + (arg.m_frac >> (static_cast<widest_type>(arg_unit_value) + static_cast<widest_type>(value_bdigits()) - static_cast<widest_type>(unit_value))) };
			}
			else if constexpr(static_cast<widest_type>(arg_unit_value) > static_cast<widest_type>(unit_value)) {
				return { arg.m_frac + (m_frac >> (static_cast<widest_type>(unit_value) + static_cast<widest_type>(value_bdigits()) - static_cast<widest_type>(arg_unit_value))) };
			};
		};
	};
};

#endif
