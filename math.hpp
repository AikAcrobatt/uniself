#pragma once

#include <concepts>
#include <limits>

#ifndef UNS_LIB_MATH
#define UNS_LIB_MATH "math.hpp"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace uns::math {

    //the most common numeric concept
    template<typename value_t>
    concept numeric = (::std::integral<value_t> || ::std::floating_point<value_t>)
        && !::std::is_same<value_t, bool>::value;


    //absolute value
    template<::uns::math::numeric value_t>
    constexpr value_t abs(value_t Arg) noexcept {
        constexpr auto zero = value_t{ 0 };
        if(Arg < zero) return zero - Arg;
        else return Arg;
    };


    namespace auxiliary {

        template<::std::floating_point value_t>
        constexpr value_t floating_point_precision() noexcept {
            if constexpr (sizeof(value_t) >= 16) {
                return 1.0e-28;
            }
            else if constexpr (sizeof(value_t) >= 8) {
                return 1.0e-14;
            }
            else if constexpr (sizeof(value_t) >= 4) {
                return 1.0e-5;
            }
            else if constexpr (sizeof(value_t) >= 2) {
                return 1.0e-3;
            }
            else {
                return 1.0e-2;
            };
        };

    };


    //correct equality checking
    template<::std::floating_point value1_t, ::std::convertible_to<value1_t> value2_t>
    constexpr bool equals(value1_t Arg1, value2_t Arg2) noexcept {
        constexpr auto minw = ::std::numeric_limits<value1_t>::min() * static_cast<value1_t>(10);

        const auto abs_arg1 = ::uns::math::abs(Arg1);
        const auto abs_arg2 = ::uns::math::abs(static_cast<value1_t>(Arg2));

        if (
            abs_arg1 > minw
            && abs_arg2 > minw
        ) {
            return ::uns::math::abs(Arg1 - static_cast<value1_t>(Arg2))
                < ::uns::math::auxiliary::floating_point_precision<value1_t>() * (
                    abs_arg1
                    + abs_arg2
                );
        }
        else {
            return abs_arg1 <= minw && abs_arg2 <= minw;
        };
    };
    template<::std::floating_point value1_t, ::std::convertible_to<value1_t> value2_t, ::std::convertible_to<value1_t> value3_t>
    constexpr bool equals(value1_t Arg1, value2_t Arg2, value3_t accuracy) noexcept {
        return ::uns::math::abs(Arg1 - static_cast<value1_t>(Arg2))
            <= ::uns::math::abs(static_cast<value1_t>(accuracy));
    };
    template<::std::integral value1_t, ::std::floating_point value2_t>
    constexpr bool equals(value1_t Arg1, value2_t Arg2) noexcept {
        return ::uns::math::equals<value2_t, value1_t>(Arg2, Arg1);
    };
    template<::std::integral value1_t, ::std::integral value2_t>
    constexpr bool equals(value1_t Arg1, value2_t Arg2) noexcept {
        return Arg2 == Arg1;
    };


    //correct >/</>=/<= checking
    template<::uns::math::numeric value1_t, ::uns::math::numeric value2_t>
    constexpr bool more(value1_t Arg1, value2_t Arg2) noexcept { return (Arg1 > Arg2) && !::uns::math::equals<value1_t>(Arg1, Arg2); };
    template<::uns::math::numeric value1_t, ::uns::math::numeric value2_t>
    constexpr bool less(value1_t Arg1, value2_t Arg2) noexcept { return (Arg1 < Arg2) && !::uns::math::equals<value1_t>(Arg1, Arg2); };
    template<::uns::math::numeric value1_t, ::uns::math::numeric value2_t>
    constexpr bool moreeq(value1_t Arg1, value2_t Arg2) noexcept { return (Arg1 > Arg2) || ::uns::math::equals<value1_t>(Arg1, Arg2); };
    template<::uns::math::numeric value1_t, ::uns::math::numeric value2_t>
    constexpr bool lesseq(value1_t Arg1, value2_t Arg2) noexcept { return (Arg1 < Arg2) || ::uns::math::equals<value1_t>(Arg1, Arg2); };


    //siqn extracting function
    template<::uns::math::numeric value_t>
    constexpr short int sign(value_t Arg)  noexcept { return (::uns::math::equals<value_t>(Arg, 0) ? 0 : (Arg > 0 ? 1 : -1)); };


    //division of two numbers representing correct zero divition
    template<::uns::math::numeric value1_t, ::uns::math::numeric value2_t>
    value1_t div(
        value1_t Numerator
        , value2_t Denominator
        , value1_t DivideToZeroValue = ::std::numeric_limits<value1_t>::max()
    ) noexcept {
        return (
            !::uns::math::equals<value2_t>(Denominator, 0)
            ? Numerator / Denominator
            : static_cast<value1_t>(::uns::math::sign(Numerator)) * DivideToZeroValue
        );
    };


    //max/min funcs
    template<::uns::math::numeric value_t>
    value_t maximal(const value_t& Arg1, const value_t& Arg2) noexcept {
        if (::uns::math::more(Arg1, Arg2)) {
            return Arg1;
        }
        else {
            return Arg2;
        };
    };
    template<::uns::math::numeric value_t, typename ... args_t>
    value_t maximal(const value_t& Arg1, const args_t& ...Args) noexcept {
        if (
            auto args_max = static_cast<value_t>(::uns::math::maximal(Args...));
            ::uns::math::more(Arg1, args_max)
        ) {
            return Arg1;
        }
        else {
            return args_max;
        };
    };

    template<::uns::math::numeric value_t>
    value_t minimal(const value_t& Arg1, const value_t& Arg2) noexcept {
        if (::uns::math::less(Arg1, Arg2)) {
            return Arg1;
        }
        else {
            return Arg2;
        };
    };
    template<::uns::math::numeric value_t, typename ... args_t>
    value_t minimal(const value_t& Arg1, const args_t& ...Args) noexcept {
        if (
            auto args_max = static_cast<value_t>(::uns::math::minimal(Args...));
            ::uns::math::less(Arg1, args_max)
        ) {
            return Arg1;
        }
        else {
            return args_max;
        };
    };


    //exponential moving average
    template<::std::floating_point value1_t, ::std::floating_point value2_t, ::std::floating_point value3_t>
    value2_t ema(value1_t CurrentValue, value2_t PreviousEma, value3_t Modifier) noexcept {
        return static_cast<value2_t>(Modifier) * static_cast<value2_t>(CurrentValue)
            + static_cast<value2_t>(static_cast<value3_t>(1) - Modifier) * PreviousEma;
    };
    template<::std::floating_point value1_t, ::std::floating_point value2_t, ::std::integral value3_t>
    value2_t ema(value1_t CurrentValue, value2_t PreviousEma, value3_t Period) noexcept  {
        return ::uns::math::ema<value2_t>(
            CurrentValue,
            PreviousEma,
            ::uns::math::div<value3_t>(2, Period + static_cast<value3_t>(1), static_cast<value3_t>(1))
        );
    };


    template<typename result_t = long double, ::uns::math::numeric value_t>
    result_t sqrt(value_t Arg) {
        return static_cast<result_t>(::sqrt(Arg));
    };

};


#endif
