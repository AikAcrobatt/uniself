
#include <iostream>

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

