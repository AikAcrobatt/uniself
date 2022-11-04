#pragma once

#define UNS_HEADER_MATH

#include <concepts>
#include <limits>

namespace uns::math {

	//the most common numeric concept
	template<typename value_t>
	concept numeric = std::integral<value_t> || std::floating_point<value_t>;


	//absolute value
	template<uns::math::numeric value_t>
	constexpr value_t abs(value_t arg) noexcept {
		static const auto zero = value_t{ 0 };
		if(arg < zero) return zero - arg;
		else return arg;
	};


	//correct equality checking
	template<std::floating_point value1_t, std::convertible_to<value1_t> value2_t>
	constexpr bool equals(value1_t arg1, value2_t arg2) noexcept {
		static const auto minw = std::numeric_limits<value1_t>::min() * static_cast<value1_t>(10);
		if(uns::math::abs(arg1) > minw && uns::math::abs(static_cast<value1_t>(arg2)) > minw)
			return (uns::math::abs(arg1 - static_cast<value1_t>(arg2)) < static_cast<value1_t>(1.0e-14) * (uns::math::abs(arg1) + uns::math::abs(static_cast<value1_t>(arg2))));
		else return (uns::math::abs(arg1) <= minw && uns::math::abs(static_cast<value1_t>(arg2)) <= minw);
	};
	template<std::floating_point value1_t, std::convertible_to<value1_t> value2_t, std::convertible_to<value1_t> value3_t>
	constexpr bool equals(value1_t arg1, value2_t arg2, value3_t accuracy) noexcept {
		return (uns::math::abs(arg1 - static_cast<value1_t>(arg2)) <= uns::math::abs(static_cast<value1_t>(accuracy)));
	};
	template<std::integral value1_t, std::floating_point value2_t>
	constexpr bool equals(value1_t arg1, value2_t arg2) noexcept {
		return uns::math::equals<value2_t>(arg2, arg1);
	};
	template<std::integral value1_t, std::integral value2_t>
	constexpr bool equals(value1_t arg1, value2_t arg2) noexcept {
		return (arg2 == arg1);
	};


	//correct >/</>=/<= checking
	template<uns::math::numeric value1_t, uns::math::numeric value2_t>
	constexpr bool more(value1_t arg1, value2_t arg2) noexcept { return (arg1 > arg2) && !uns::math::equals<value1_t>(arg1, arg2); };
	template<uns::math::numeric value1_t, uns::math::numeric value2_t>
	constexpr bool less(value1_t arg1, value2_t arg2) noexcept { return (arg1 < arg2) && !uns::math::equals<value1_t>(arg1, arg2); };
	template<uns::math::numeric value1_t, uns::math::numeric value2_t>
	constexpr bool moreeq(value1_t arg1, value2_t arg2) noexcept { return (arg1 > arg2) || uns::math::equals<value1_t>(arg1, arg2); };
	template<uns::math::numeric value1_t, uns::math::numeric value2_t>
	constexpr bool lesseq(value1_t arg1, value2_t arg2) noexcept { return (arg1 < arg2) || uns::math::equals<value1_t>(arg1, arg2); };


	//siqn extracting function
	template<uns::math::numeric value_t>
	constexpr short int sign(value_t arg)  noexcept { return (uns::math::equals<value_t>(arg, 0) ? 0 : (arg > 0 ? 1 : -1)); };


	//division of two numbers representing correct zero divition
	template<uns::math::numeric value1_t, uns::math::numeric value2_t>
	value1_t div(value1_t numerator, value2_t denominator, value1_t divide_to_zero_result_value = std::numeric_limits<value1_t>::max()) noexcept {
		return (
			!uns::math::equals<value2_t>(denominator, 0)
			? numerator / denominator
			: static_cast<value1_t>(uns::math::sign(numerator)) * divide_to_zero_result_value
		);
	};


	//max/min funcs
	template<uns::math::numeric value_t>
	value_t max(const value_t& arg1, const value_t& arg2) noexcept {
		if(arg1 > arg2) return arg1;
		else return arg2;
	};
	template<uns::math::numeric value_t, typename ... args_t>
	value_t max(const value_t& arg1, const args_t& ...args) noexcept {
		if(auto arg_ = static_cast<value_t>(max(args)); arg1 > arg_) return arg1;
		else return arg_;
	};
	template<uns::math::numeric value_t>
	value_t min(const value_t& arg1, const value_t& arg2) noexcept {
		if(arg1 < arg2) return arg1;
		else return arg2;
	};
	template<uns::math::numeric value_t, typename ... args_t>
	value_t min(const value_t& arg1, const args_t& ...args) noexcept {
		if(auto arg_ = static_cast<value_t>(min(args)); arg1 < arg_) return arg1;
		else return arg_;
	};


	//exponential moving average
	template<std::floating_point value1_t, std::floating_point value2_t, std::floating_point value3_t>
	value2_t ema(value1_t current_value, value2_t previous_ema, value3_t modificator) noexcept {
		return static_cast<value2_t>(modificator) * static_cast<value2_t>(current_value)
			+ static_cast<value2_t>(static_cast<value3_t>(1) - modificator) * previous_ema;
	};
	template<std::floating_point value1_t, std::floating_point value2_t, std::integral value3_t>
	value2_t ema(value1_t current_value, value2_t previous_ema, value3_t period) noexcept  {
		return uns::math::ema<value2_t>(
			current_value,
			previous_ema,
			uns::math::div<value3_t>(2, period + static_cast<value3_t>(1), static_cast<value3_t>(1))
		);
	};


	template<typename result_t = long double, uns::math::numeric value_t>
	result_t sqrt(value_t arg) {
		return static_cast<result_t>(::sqrt(arg));
	};

};
