

#include "uniself/strings.hpp"
#include "uniself/renum.hpp"

#include "gtest/gtest.h"

namespace uns::tests::string::casts {

    const auto testphrase = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    using types_list = ::testing::Types<
        ::std::u32string
        , ::std::u8string
        , ::std::string
        , ::std::u16string
        , ::std::wstring
    >;

};


template<::uns::is_basic_string string_t>
class CrossStringCasts : public ::testing::Test {};

TYPED_TEST_CASE(CrossStringCasts, ::uns::tests::string::casts::types_list);

TYPED_TEST(CrossStringCasts, BackwardIdenticalTest) {
    ASSERT_TRUE(
        ::uns::tests::string::casts::testphrase == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<TypeParam>(::uns::tests::string::casts::testphrase)
        )
    );
};


UNS_RENUM(correct, int,
    (yes,)
    , (no,)
);

class BooleanCasts : public ::testing::TestWithParam<
    ::std::tuple<::std::u32string, bool, ::correct>
> {
public:
    using elem_type = ::std::tuple<::std::u32string, bool, ::correct>;
public:
    enum {
        string = 0
        , expectation = 1
        , correctness = 2
    };
};

template<>
::std::string testing::PrintToString(const ::BooleanCasts::elem_type& Elem) {
    return "BooleanCasts::Elem{ \""
        + ::uns::string::cast<::std::string>(
            ::std::get<::BooleanCasts::string>(Elem)
        )
        + "\" ?= "
        + (
            ::std::get<::BooleanCasts::expectation>(Elem)
            ? "true"
            : "false"
        )
        + ", correct::"
        + ::uns::string::cast<::std::string>(
            ::std::get<::BooleanCasts::correctness>(Elem).to_string()
        )
        + " }";
};
template<>
::std::string testing::PrintToString(const ::std::u32string& Elem) {
    return "u32\""
        + ::uns::string::cast<::std::string>(Elem)
        + "\"";
};

class ForwardBooleanCasts : public ::BooleanCasts {};

TEST_P(ForwardBooleanCasts, CastTest) {
    auto correctness = ::std::get<::BooleanCasts::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<::BooleanCasts::string>(GetParam())
                , ::uns::string::cast<::std::u32string>(
                    ::std::get<::BooleanCasts::expectation>(GetParam())
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<::BooleanCasts::string>(GetParam())
                , ::uns::string::cast<::std::u32string>(
                    ::std::get<::BooleanCasts::expectation>(GetParam())
                )
            );

            break;
        }
        default: {
            ADD_FAILURE()
                << "Correctness value is not expected "
                << ::uns::string::cast<::std::string>(
                    correctness.to_string()
                );
        }
    };
};

INSTANTIATE_TEST_CASE_P(TypedCasts, ForwardBooleanCasts,
    ::testing::Values(
        ::BooleanCasts::elem_type{ U"true",     true,   ::correct::yes }
        , ::BooleanCasts::elem_type{ U"false",  false,  ::correct::yes }
        , ::BooleanCasts::elem_type{ U" true ", true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U"TRue",   true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U" 1 ",    true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U"Tru",    true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U"fals",   false,  ::correct::no }
        , ::BooleanCasts::elem_type{ U"FAlse",  false,  ::correct::no }
        , ::BooleanCasts::elem_type{ U" FALSE ",false,  ::correct::no }
        , ::BooleanCasts::elem_type{ U"00",     false,  ::correct::no }
    )
);


class BackwardBooleanCasts : public ::BooleanCasts {};

TEST_P(BackwardBooleanCasts, CastTest) {
    auto correctness = ::std::get<::BooleanCasts::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<::BooleanCasts::expectation>(GetParam())
                , ::uns::string::cast<bool>(
                    ::std::get<::BooleanCasts::string>(GetParam())
                )
            );

            break;
        }
        case ::correct::no: {
            #pragma warning(push)
            #pragma warning(disable: 4553)
            ASSERT_THROW(
                ::std::get<::BooleanCasts::expectation>(GetParam())
                == ::uns::string::cast<bool>(
                    ::std::get<::BooleanCasts::string>(GetParam())
                )
                , ::std::runtime_error
            );
            #pragma warning(pop)

            break;
        }
        default: {
            ADD_FAILURE()
                << "Correctness value is not expected "
                << ::uns::string::cast<::std::string>(
                    correctness.to_string()
                );
        }
    };
};

INSTANTIATE_TEST_CASE_P(TypedCasts, BackwardBooleanCasts,
    ::testing::Values(
        ::BooleanCasts::elem_type{ U"true",     true,   ::correct::yes }
        , ::BooleanCasts::elem_type{ U"TRUE",   true,   ::correct::yes }
        , ::BooleanCasts::elem_type{ U"1",      true,   ::correct::yes }
        , ::BooleanCasts::elem_type{ U"True",   true,   ::correct::yes }
        , ::BooleanCasts::elem_type{ U"false",  false,  ::correct::yes }
        , ::BooleanCasts::elem_type{ U"False",  false,  ::correct::yes }
        , ::BooleanCasts::elem_type{ U"FALSE",  false,  ::correct::yes }
        , ::BooleanCasts::elem_type{ U"0",      false,  ::correct::yes }
        , ::BooleanCasts::elem_type{ U" true ", true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U"TRue",   true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U" 1 ",    true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U"Tru",    true,   ::correct::no }
        , ::BooleanCasts::elem_type{ U"fals",   false,  ::correct::no }
        , ::BooleanCasts::elem_type{ U"FAlse",  false,  ::correct::no }
        , ::BooleanCasts::elem_type{ U" FALSE ",false,  ::correct::no }
        , ::BooleanCasts::elem_type{ U"00",     false,  ::correct::no }
    )
);
