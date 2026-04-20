
#include <iostream>
#include <vector>
#include <limits>

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
