#pragma once

#include <cmath>
#include <limits>

#ifndef UNS_EQL_ACCURACY
#define UNS_EQL_ACCURACY 0.00000000001
#endif

#ifndef UNS_NUMBER_MIN_MODIFICATOR
#define UNS_NUMBER_MIN_MODIFICATOR 10
#endif

namespace uns::math {

	//получение абсолютного значения
	template<typename number_t>
	number_t Abs(number_t arg) noexcept {
		if (arg < 0) return 0 - arg;
		else return arg;
	};


	//проверка на равенство с заданной точностью
	template<typename number_t>
	bool Equals(number_t arg1, number_t arg2) noexcept {
		static const number_t minw = std::numeric_limits<number_t>::min() * UNS_NUMBER_MIN_MODIFICATOR;
		if (uns::math::Abs(arg1) > minw && uns::math::Abs(arg2) > minw)
			return(uns::math::Abs(arg1 - arg2) <= UNS_EQL_ACCURACY * (uns::math::Abs(arg1) + uns::math::Abs(arg2)));
		else return(uns::math::Abs(arg1) <= minw && uns::math::Abs(arg2) <= minw);
	};
	template<typename number1_t, typename number2_t>
	bool Equals(number1_t arg1, number2_t arg2) noexcept {
		return uns::math::Equals(static_cast<long double>(arg1), static_cast<long double>(arg2));
	};
	template<typename number_t>
	bool Equals(number_t arg1, number_t arg2, number_t accuracy) noexcept {
		return(uns::math::Abs(arg1 - arg2) <= uns::math::Abs(accuracy));
	};


	//проверка на >
	template<typename number_t>
	bool More(number_t arg1, number_t arg2) noexcept { return(!uns::math::Equals(arg1, arg2) && (arg1 > arg2)); };
	template<typename number1_t, typename number2_t>
	bool More(number1_t arg1, number2_t arg2) noexcept { return(!uns::math::Equals(arg1, arg2) && (arg1 > arg2)); };


	//проверка на <
	template<typename number_t>
	bool Less(number_t arg1, number_t arg2) noexcept { return(!uns::math::Equals(arg1, arg2) && (arg1 < arg2)); };
	template<typename number1_t, typename number2_t>
	bool Less(number1_t arg1, number2_t arg2) noexcept { return(!uns::math::Equals(arg1, arg2) && (arg1 < arg2)); };


	//проверка на >=
	template<typename number_t>
	bool MoreEq(number_t arg1, number_t arg2) noexcept { return(uns::math::Equals(arg1, arg2) || (arg1 > arg2)); };
	template<typename number1_t, typename number2_t>
	bool MoreEq(number1_t arg1, number2_t arg2) noexcept { return(uns::math::Equals(arg1, arg2) || (arg1 > arg2)); };


	//проверка на <=
	template<typename number_t>
	bool LessEq(number_t arg1, number_t arg2) noexcept { return(uns::math::Equals(arg1, arg2) || (arg1 < arg2)); };
	template<typename number1_t, typename number2_t>
	bool LessEq(number1_t arg1, number2_t arg2) noexcept { return(uns::math::Equals(arg1, arg2) || (arg1 < arg2)); };


	//функция знака числа
	template<typename number_t>
	short int Sign(number_t arg)  noexcept { return uns::math::Equals(arg, 0) ? 0 : (arg > 0 ? 1 : -1); };


	//безопасное деление двух чисел number_t аналог numerator/denominator
	template<typename number_t>
	number_t Div(number_t numerator, number_t denominator, number_t divide_to_zero_result_value = std::numeric_limits<number_t>::max()) noexcept { return (!uns::math::Equals(denominator, 0) ? numerator / denominator : uns::math::Sign(numerator) * divide_to_zero_result_value); };
	template<typename number1_t, typename number2_t>
	number2_t Div(number1_t numerator, number2_t denominator, number2_t divide_to_zero_result_value = std::numeric_limits<number2_t>::max()) noexcept { return uns::math::Div(static_cast<number2_t>(numerator), denominator, divide_to_zero_result_value); };


	//экспоненциальное скользящее среднее
	template<typename number_t>
	number_t EMA(number_t current_value, number_t previous_EMA, number_t modificator) noexcept { return modificator * current_value + (1.0F - modificator) * previous_EMA; };
	template<typename number_t>
	number_t EMA(number_t current_value, number_t previous_EMA, size_t period) noexcept { return uns::math::EMA(current_value, previous_EMA, static_cast<number_t>((period + 1 > 0) ? 2.0F / static_cast<number_t>(period + 1) : 1.0F)); };


	//комплексное споряжение
	template<typename number_t>
	number_t CmpxConjugate(number_t number) noexcept { return number; };



}

#undef UNS_EQL_ACCURACY

#undef UNS_NUMBER_MIN_MODIFICATOR
