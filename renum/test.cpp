
#include <iostream>
#include <tuple>

#include "gtest/gtest.h"

#include "uniself/renum.hpp"
#include "uniself/strings.hpp"

namespace abyss {

    UNS_RENUM(layers, int,
        (l0, = 0)
        , (l1, = 1)
        , (l2, = 2)
        , (l3, = 3)
        , (l4, = 4)
        , (l5, = 5)
        , (l6, = 6)
        , (l7, = 7)
        , (l8, = 8)
        , (l9, = 9)
    );

};

template<typename renum_candidate_t>
class RenumTyped : public ::testing::Test {};

using types_list = ::testing::Types<::abyss::layers, void, int, char, ::std::string>;
TYPED_TEST_CASE(RenumTyped, types_list);

TYPED_TEST(RenumTyped, TraitsTest) {
    if (typeid(TypeParam) == typeid(::abyss::layers)) {
        ASSERT_TRUE(::uns::renum_traits<TypeParam>::value);
    }
    else {
        ASSERT_FALSE(::uns::renum_traits<TypeParam>::value);
    };
};


class ComparationChecks : public ::testing::TestWithParam<
        ::std::tuple<::abyss::layers::integral_type, ::abyss::layers::integral_type>
    > {};

TEST_P(ComparationChecks, EqualityCheckRenumEnum) {
    const ::abyss::layers renum_obj = ::abyss::layers::enum_type{ ::std::get<0>(GetParam()) };
    const auto renum_enum = ::abyss::layers::enum_type{ ::std::get<1>(GetParam()) };
    if (::std::get<0>(GetParam()) == ::std::get<1>(GetParam())) {
        EXPECT_TRUE(renum_obj == renum_enum);
        EXPECT_TRUE(renum_enum == renum_obj);
    }
    else {
        EXPECT_FALSE(renum_obj == renum_enum);
        EXPECT_FALSE(renum_enum == renum_obj);
    };
};

TEST_P(ComparationChecks, UnEqualityCheckRenumEnum) {
    const ::abyss::layers renum_obj = ::abyss::layers::enum_type{ ::std::get<0>(GetParam()) };
    const auto renum_enum = ::abyss::layers::enum_type{ ::std::get<1>(GetParam()) };
    if (::std::get<0>(GetParam()) == ::std::get<1>(GetParam())) {
        EXPECT_FALSE(renum_obj != renum_enum);
        EXPECT_FALSE(renum_enum != renum_obj);
    }
    else {
        EXPECT_TRUE(renum_obj != renum_enum);
        EXPECT_TRUE(renum_enum != renum_obj);
    };
};

TEST_P(ComparationChecks, EqualityCheckRenumRenum) {
    const ::abyss::layers renum_obj_1 = ::abyss::layers::enum_type{ ::std::get<0>(GetParam()) };
    const ::abyss::layers renum_obj_2 = ::abyss::layers::enum_type{ ::std::get<1>(GetParam()) };
    if (::std::get<0>(GetParam()) == ::std::get<1>(GetParam())) {
        EXPECT_TRUE(renum_obj_1 == renum_obj_2);
    }
    else {
        EXPECT_FALSE(renum_obj_1 == renum_obj_2);
    };
};

TEST_P(ComparationChecks, UnEqualityCheckRenumRenum) {
    const ::abyss::layers renum_obj_1 = ::abyss::layers::enum_type{ ::std::get<0>(GetParam()) };
    const ::abyss::layers renum_obj_2 = ::abyss::layers::enum_type{ ::std::get<1>(GetParam()) };
    if (::std::get<0>(GetParam()) == ::std::get<1>(GetParam())) {
        EXPECT_FALSE(renum_obj_1 != renum_obj_2);
    }
    else {
        EXPECT_TRUE(renum_obj_1 != renum_obj_2);
    };
};

INSTANTIATE_TEST_CASE_P(RenumGeneral, ComparationChecks,
    ::testing::Combine(
        ::testing::Range<::abyss::layers::integral_type>(0, ::abyss::layers::size()),
        ::testing::Range<::abyss::layers::integral_type>(0, ::abyss::layers::size())
    )
);


UNS_RENUM(renum_test, int,
    (t1, = 0)
    , (t2, = 10)
    , (t3,)
);

class RenumStrings : public ::testing::TestWithParam<
    ::std::tuple<::renum_test, ::std::u32string>
> {
public:
    using elem_type = ::std::tuple<::renum_test, ::std::u32string>;
public:
    enum {
        renum = 0
        , string = 1
    };
};


template<>
::std::string testing::PrintToString(const ::RenumStrings::elem_type& Elem) {
    return "RenumStrings::Elem{ "
        + ::uns::string::u8_cast<::std::string>(
            ::uns::string::u8_cast<::std::u8string>(
                static_cast<::renum_test::integral_type>(::std::get<::RenumStrings::renum>(Elem))
            )
        )
        + ", "
        + ::uns::string::u8_cast<::std::string>(
            ::uns::string::u8_cast<::std::u8string>(
                ::std::get<::RenumStrings::string>(Elem)
            )
        )
        + " }";
};

class RenumConvertStringsCorrect : public RenumStrings {};

TEST_P(RenumConvertStringsCorrect, RenumFromStrings) {
    ASSERT_TRUE(
        ::std::get<::RenumStrings::renum>(GetParam())
            == ::renum_test::from_string(::std::get<::RenumStrings::string>(GetParam()))
    );
};

TEST_P(RenumConvertStringsCorrect, RenumToStrings) {
    ASSERT_TRUE(
        ::std::get<::RenumStrings::renum>(GetParam()).to_string()
            == ::std::get<::RenumStrings::string>(GetParam())
    );
};

INSTANTIATE_TEST_CASE_P(RenumGeneral, RenumConvertStringsCorrect,
    ::testing::Values(
        ::RenumStrings::elem_type{ ::renum_test::t1, U"t1" }
        , ::RenumStrings::elem_type{ ::renum_test::t2, U"t2" }
        , ::RenumStrings::elem_type{ ::renum_test::t3, U"t3" }
    )
);


class RenumConvertStringsInCorrect : public RenumStrings {};

TEST_P(RenumConvertStringsInCorrect, RenumFromStrings) {
    ASSERT_THROW(
        ::std::get<::RenumStrings::renum>(GetParam())
            == ::renum_test::from_string(::std::get<::RenumStrings::string>(GetParam()))
        , ::std::runtime_error
    );
};

TEST_P(RenumConvertStringsInCorrect, RenumToStrings) {
    ASSERT_FALSE(
        ::std::get<::RenumStrings::renum>(GetParam()).to_string()
            == ::std::get<::RenumStrings::string>(GetParam())
    );
};

INSTANTIATE_TEST_CASE_P(RenumGeneral, RenumConvertStringsInCorrect,
    ::testing::Values(
        ::RenumStrings::elem_type{ ::renum_test::t1, U" t1" }
        , ::RenumStrings::elem_type{ ::renum_test::t2, U"t 2" }
        , ::RenumStrings::elem_type{ ::renum_test::t3, U"t4" }
        , ::RenumStrings::elem_type{ ::renum_test::t3, U"t3 " }
    )
);


UNS_RENUM(unconditioned, int,
    (T1, = 0)
    , (_,)
    , ( ABRACKAM_LINCOLN_WAS_THE_MOST_FAMOUS_PRESIDENT_OF_THE_USA, = 111)
    , (hg1jkh081974, )
    , (Ky______________________________________lksugbh, = 7890)
    , (                HJJH,= 11111)
    , (UwAuWa,=141315)
    , (slhdbc                    , )
    , (a, )
    , (b, )
);

template<>
::std::string testing::PrintToString(const ::unconditioned& Elem) {
    return "unconditioned::" + ::uns::string::u8_cast<::std::string>(
        ::uns::string::u8_cast<::std::u8string>(Elem.to_string())
    );
};

template<typename initial_t>
class RenumIdenticalCast : public ::testing::TestWithParam<initial_t> {};

class RenumIdenticalCastRenum : public ::RenumIdenticalCast<::unconditioned> {};

TEST_P(RenumIdenticalCastRenum, FromRenumToRenum) {
    ASSERT_TRUE(
        GetParam() == ::unconditioned::from_string(GetParam().to_string())
    );
};

INSTANTIATE_TEST_CASE_P(RenumGeneral, RenumIdenticalCastRenum,
    ::testing::Values(
        ::unconditioned::_
        , ::unconditioned::a
        , ::unconditioned::ABRACKAM_LINCOLN_WAS_THE_MOST_FAMOUS_PRESIDENT_OF_THE_USA
        , ::unconditioned::b
        , ::unconditioned::hg1jkh081974
        , ::unconditioned::HJJH
        , ::unconditioned::Ky______________________________________lksugbh
        , ::unconditioned::slhdbc
        , ::unconditioned::T1
        , ::unconditioned::UwAuWa
    )
);


class RenumIdenticalCastString : public ::RenumIdenticalCast<::std::u32string> {};

template<>
::std::string testing::PrintToString(const ::std::u32string& Elem) {
    return "u8\"" + ::uns::string::u8_cast<::std::string>(
        ::uns::string::u8_cast<::std::u8string>(Elem)
    ) + "\"";
};

TEST_P(RenumIdenticalCastString, FromStringToString) {
    ASSERT_TRUE(
        GetParam() == ::unconditioned::from_string(GetParam()).to_string()
    );
};

INSTANTIATE_TEST_CASE_P(RenumGeneral, RenumIdenticalCastString,
    ::testing::Values(
        U"_"
        , U"a"
        , U"ABRACKAM_LINCOLN_WAS_THE_MOST_FAMOUS_PRESIDENT_OF_THE_USA"
        , U"b"
        , U"hg1jkh081974"
        , U"HJJH"
        , U"Ky______________________________________lksugbh"
        , U"slhdbc"
        , U"T1"
        , U"UwAuWa"
    )
);
