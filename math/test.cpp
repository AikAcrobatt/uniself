
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>

#include "uniself/renum.hpp"
#include "uniself/math.hpp"

#include "gtest/gtest.h"


UNS_RENUM(correct, int,
    (yes, )
    , (no, )
);

namespace uns::tests::math {

    template<typename maybe_numeric_t>
    constexpr bool is_this_type_numeric() noexcept {
        return false;
    };
    template<>
    constexpr bool is_this_type_numeric<short int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<unsigned short int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<unsigned int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<long int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<unsigned long int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<long long int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<unsigned long long int>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<int8_t>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<uint8_t>() noexcept {
        return true;
    };
    //template<>
    //constexpr bool is_this_type_numeric<int16_t>() noexcept {
    //    return true;
    //};
    //template<>
    //constexpr bool is_this_type_numeric<uint16_t>() noexcept {
    //    return true;
    //};
    //template<>
    //constexpr bool is_this_type_numeric<int32_t>() noexcept {
    //    return true;
    //};
    //template<>
    //constexpr bool is_this_type_numeric<uint32_t>() noexcept {
    //    return true;
    //};
    //template<>
    //constexpr bool is_this_type_numeric<int64_t>() noexcept {
    //    return true;
    //};
    //template<>
    //constexpr bool is_this_type_numeric<uint64_t>() noexcept {
    //    return true;
    //};
    template<>
    constexpr bool is_this_type_numeric<float>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<double>() noexcept {
        return true;
    };
    template<>
    constexpr bool is_this_type_numeric<long double>() noexcept {
        return true;
    };

    using types_list = ::testing::Types<
        short int
        , unsigned short int
        , int
        , unsigned int
        , long int
        , unsigned long int
        , long long int
        , unsigned long long int
        , int8_t
        , uint8_t
        , int16_t
        , uint16_t
        , int32_t
        , uint32_t
        , int64_t
        , uint64_t
        , float
        , double
        , long double
        , bool
        , char*
        , void*
        , ::std::string
        , void(*)(int)
    >;

    template<::uns::math::numeric is_numeric_t>
    bool numeric_concept_apply() {
        return true;
    };
    template<typename isnot_numeric_t>
    bool numeric_concept_apply() {
        return false;
    };
};

template<typename maybe_numeric_t>
class NumericConcept : public ::testing::Test {};

TYPED_TEST_CASE(NumericConcept, ::uns::tests::math::types_list);
TYPED_TEST(NumericConcept, Test) {
    ASSERT_EQ(
        ::uns::tests::math::is_this_type_numeric<TypeParam>()
        , ::uns::tests::math::numeric_concept_apply<TypeParam>()
    );
};


template<::std::floating_point numeric_t>
class AbsTestsFlt : public ::testing::TestWithParam<numeric_t> {
public:
    static ::std::vector<numeric_t> generate_tests() {
        constexpr auto minw = ::std::numeric_limits<numeric_t>::min();
        constexpr auto t10 = numeric_t{ 10 };
        constexpr auto step = numeric_t{ 0.12399 };

        auto result = ::std::vector<numeric_t>{};

        numeric_t current_value = 0;
        for (; current_value < t10 * minw; current_value += minw) {
            result.emplace_back(current_value);
        };
        for (; current_value < t10; current_value += step) {
            result.emplace_back(current_value);
        };

        return result;
    };
};

using AbsTests_Flt = ::AbsTestsFlt<float>;
TEST_P(AbsTests_Flt, Do) {
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(GetParam())
    );
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(-GetParam())
    );
};
INSTANTIATE_TEST_CASE_P(AbsTesting, AbsTests_Flt,
    ::testing::ValuesIn(
        ::AbsTests_Flt::generate_tests()
    )
);
using AbsTests_Dbl = ::AbsTestsFlt<double>;
TEST_P(AbsTests_Dbl, Do) {
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(GetParam())
    );
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(-GetParam())
    );
};
INSTANTIATE_TEST_CASE_P(AbsTesting, AbsTests_Dbl,
    ::testing::ValuesIn(
        ::AbsTests_Dbl::generate_tests()
    )
);
using AbsTests_LDbl = ::AbsTestsFlt<long double>;
TEST_P(AbsTests_LDbl, Do) {
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(GetParam())
    );
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(-GetParam())
    );
};
INSTANTIATE_TEST_CASE_P(AbsTesting, AbsTests_LDbl,
    ::testing::ValuesIn(
        ::AbsTests_LDbl::generate_tests()
    )
);

template<::std::integral numeric_t>
class AbsTestsInt : public ::testing::TestWithParam<numeric_t> {
public:
    static ::std::vector<numeric_t> generate_tests() {
        constexpr auto maxw = ::std::numeric_limits<numeric_t>::max() >> 10;

        auto result = ::std::vector<numeric_t>{};

        for (numeric_t current_value = 1; current_value < maxw; current_value *= 2) {
            result.emplace_back(current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 3) {
            result.emplace_back(current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 5) {
            result.emplace_back(current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 7) {
            result.emplace_back(current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 11) {
            result.emplace_back(current_value);
        };

        return result;
    };
};

using AbsTests_Int = ::AbsTestsInt<int>;
TEST_P(AbsTests_Int, Do) {
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(GetParam())
    );
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(-GetParam())
    );
};
INSTANTIATE_TEST_CASE_P(AbsTesting, AbsTests_Int,
    ::testing::ValuesIn(
        ::AbsTests_Int::generate_tests()
    )
);
using AbsTests_LLInt = ::AbsTestsInt<long long int>;
TEST_P(AbsTests_LLInt, Do) {
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(GetParam())
    );
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(-GetParam())
    );
};
INSTANTIATE_TEST_CASE_P(AbsTesting, AbsTests_LLInt,
    ::testing::ValuesIn(
        ::AbsTests_LLInt::generate_tests()
    )
);
using AbsTests_ULLInt = ::AbsTestsInt<unsigned long long int>;
TEST_P(AbsTests_ULLInt, Do) {
    ASSERT_FLOAT_EQ(
        GetParam()
        , ::uns::math::abs(GetParam())
    );
};
INSTANTIATE_TEST_CASE_P(AbsTesting, AbsTests_ULLInt,
    ::testing::ValuesIn(
        ::AbsTests_ULLInt::generate_tests()
    )
);


template<::std::floating_point numeric_t>
class EqualsTestsFlt : public ::testing::TestWithParam<::std::tuple<numeric_t, numeric_t>> {
private:
    using test = ::testing::TestWithParam<::std::tuple<numeric_t, numeric_t>>;
public:
    using numeric_type = numeric_t;
public:
    numeric_type get_arg1() const {
        return ::std::get<0>(test::GetParam());
    };
    numeric_type get_arg2() const {
        return ::std::get<1>(test::GetParam());
    };
    bool are_equal() const {
        constexpr auto minw = ::std::numeric_limits<numeric_type>::min() * static_cast<numeric_type>(10);

        const auto arg1 = ::std::get<0>(test::GetParam());
        const auto arg2 = ::std::get<1>(test::GetParam());

        const auto abs_arg1 = ::uns::math::abs(arg1);
        const auto abs_arg2 = ::uns::math::abs(arg2);

        bool equals_result = false;
        if (
            abs_arg1 > minw
            && abs_arg2 > minw
        ) {
            return 
                ::uns::math::abs(arg1 - arg2)
                < numeric_type{ 1.0e-14 } * (
                    abs_arg1
                    + abs_arg2
                );
        }
        else {
            return abs_arg1 <= minw && abs_arg2 <= minw;
        };
    };
public:
    static ::std::vector<numeric_t> generate_tests() {
        constexpr auto minw = ::std::numeric_limits<numeric_t>::min();
        constexpr auto t10 = numeric_t{ 10 };
        constexpr auto step = numeric_t{ 0.12399 };

        auto result = ::std::vector<numeric_t>{};

        numeric_t current_value = 0;
        for (; current_value < t10 * minw; current_value += minw) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };
        for (; current_value < t10; current_value += step) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };

        return result;
    };
};

using EqualsTests_Flt = ::EqualsTestsFlt<float>;
TEST_P(EqualsTests_Flt, Do) {
    ASSERT_EQ(
        are_equal()
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_Flt,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_Flt::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_Flt::generate_tests()
        )
    )
);
using EqualsTests_Dbl = ::EqualsTestsFlt<double>;
TEST_P(EqualsTests_Dbl, Do) {
    ASSERT_EQ(
        are_equal()
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_Dbl,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_Dbl::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_Dbl::generate_tests()
        )
    )
);
using EqualsTests_LDbl = ::EqualsTestsFlt<long double>;
TEST_P(EqualsTests_LDbl, Do) {
    ASSERT_EQ(
        are_equal()
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_LDbl,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_LDbl::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_LDbl::generate_tests()
        )
    )
);

template<::std::integral numeric_t>
class EqualsTestsInt : public ::testing::TestWithParam<::std::tuple<numeric_t, numeric_t>> {
private:
    using test = ::testing::TestWithParam<::std::tuple<numeric_t, numeric_t>>;
public:
    using numeric_type = numeric_t;
public:
    numeric_type get_arg1() const {
        return ::std::get<0>(test::GetParam());
    };
    numeric_type get_arg2() const {
        return ::std::get<1>(test::GetParam());
    };
public:
    static ::std::vector<numeric_t> generate_tests() {
        constexpr auto maxw = ::std::numeric_limits<numeric_t>::max() >> 10;

        auto result = ::std::vector<numeric_t>{};

        for (numeric_t current_value = 1; current_value < maxw; current_value *= 2) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 3) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 5) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 7) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };
        for (numeric_t current_value = 1; current_value < maxw; current_value *= 11) {
            result.emplace_back(current_value);
            result.emplace_back(-current_value);
        };

        return result;
    };
};

using EqualsTests_SInt = ::EqualsTestsInt<short int>;
TEST_P(EqualsTests_SInt, Do) {
    ASSERT_EQ(
        (get_arg1() == get_arg2())
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_SInt,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_SInt::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_SInt::generate_tests()
        )
    )
);
using EqualsTests_Int = ::EqualsTestsInt<int>;
TEST_P(EqualsTests_Int, Do) {
    ASSERT_EQ(
        (get_arg1() == get_arg2())
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_Int,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_Int::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_Int::generate_tests()
        )
    )
);
using EqualsTests_LInt = ::EqualsTestsInt<long int>;
TEST_P(EqualsTests_LInt, Do) {
    ASSERT_EQ(
        (get_arg1() == get_arg2())
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_LInt,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_LInt::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_LInt::generate_tests()
        )
    )
);
using EqualsTests_LLInt = ::EqualsTestsInt<long long int>;
TEST_P(EqualsTests_LLInt, Do) {
    ASSERT_EQ(
        (get_arg1() == get_arg2())
        , ::uns::math::equals(get_arg1(), get_arg2())
    );
};
INSTANTIATE_TEST_CASE_P(EqualsTesting, EqualsTests_LLInt,
    ::testing::Combine(
        ::testing::ValuesIn(
            ::EqualsTests_LLInt::generate_tests()
        )
        , ::testing::ValuesIn(
            ::EqualsTests_LLInt::generate_tests()
        )
    )
);


template<typename numeric_t>
class DivTests : public ::testing::TestWithParam<::std::tuple<bool, numeric_t, numeric_t>> {
private:
    using test = ::testing::TestWithParam<::std::tuple<bool, numeric_t, numeric_t>>;
public:
    using numeric_type = numeric_t;
public:
    numeric_type get_numerator() const {
        return get_sign() * ::std::get<1>(test::GetParam());
    };
    numeric_type get_denominator() const {
        return ::std::get<2>(test::GetParam());
    };
    numeric_type get_sign() const {
        return (
            ::std::get<0>(test::GetParam())
            ? 1
            : -1
        );
    };
    numeric_type div_result() const {
        return (
            !::uns::math::equals(get_denominator(), 0)
            ? get_numerator() / get_denominator()
            : get_sign() * ::std::numeric_limits<numeric_type>::max()
        );
    };
public:
    static ::std::vector<::std::tuple<bool, numeric_t, numeric_t>> generate_tests() {
        constexpr bool signs[2] = { true, false };

        auto result = ::std::vector<::std::tuple<bool, numeric_t, numeric_t>>{};

        constexpr auto max = static_cast<numeric_type>(100);
        for (auto sign : signs) {
            constexpr auto numerator_step = static_cast<numeric_type>(1.517);
            for (numeric_type numerator = 0; numerator < max; numerator += numerator_step) {
                constexpr auto denominator_step = static_cast<numeric_type>(2.809);
                for (numeric_type denominator = 0; denominator < max; denominator += denominator_step) {
                    result.emplace_back(
                        sign
                        , numerator
                        , denominator
                    );
                    result.emplace_back(
                        sign
                        , numerator
                        , -denominator
                    );
                };
            };
        };

        return result;
    };
};

using DivTests_Flt = ::DivTests<float>;
TEST_P(DivTests_Flt, Do) {
    ASSERT_FLOAT_EQ(
        div_result()
        , ::uns::math::div(get_numerator(), get_denominator())
    );
};
INSTANTIATE_TEST_CASE_P(DivTesting, DivTests_Flt,
    ::testing::ValuesIn(
        ::DivTests_Flt::generate_tests()
    )
);
using DivTests_LDbl = ::DivTests<long double>;
TEST_P(DivTests_LDbl, Do) {
    ASSERT_FLOAT_EQ(
        div_result()
        , ::uns::math::div(get_numerator(), get_denominator())
    );
};
INSTANTIATE_TEST_CASE_P(DivTesting, DivTests_LDbl,
    ::testing::ValuesIn(
        ::DivTests_LDbl::generate_tests()
    )
);
using DivTests_Int = ::DivTests<int>;
TEST_P(DivTests_Int, Do) {
    ASSERT_FLOAT_EQ(
        div_result()
        , ::uns::math::div(get_numerator(), get_denominator())
    );
};
INSTANTIATE_TEST_CASE_P(DivTesting, DivTests_Int,
    ::testing::ValuesIn(
        ::DivTests_Int::generate_tests()
    )
);
using DivTests_LInt = ::DivTests<long int>;
TEST_P(DivTests_LInt, Do) {
    ASSERT_FLOAT_EQ(
        div_result()
        , ::uns::math::div(get_numerator(), get_denominator())
    );
};
INSTANTIATE_TEST_CASE_P(DivTesting, DivTests_LInt,
    ::testing::ValuesIn(
        ::DivTests_LInt::generate_tests()
    )
);
