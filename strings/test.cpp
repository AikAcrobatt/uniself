

#include "uniself/strings.hpp"
#include "uniself/renum.hpp"

#include "gtest/gtest.h"

template<>
::std::string testing::PrintToString(const ::std::u32string& Elem) {
    return "u32\""
        + ::uns::string::cast<::std::string>(Elem)
        + "\"";
};
template<>
::std::string testing::PrintToString(const ::std::u8string& Elem) {
    return "u8\""
        + ::uns::string::cast<::std::string>(Elem)
        + "\"";
};
template<>
::std::string testing::PrintToString(const ::std::u16string& Elem) {
    return "u16\""
        + ::uns::string::cast<::std::string>(Elem)
        + "\"";
};
template<>
::std::string testing::PrintToString(const ::std::wstring& Elem) {
    return "L\""
        + ::uns::string::cast<::std::string>(Elem)
        + "\"";
};


class StringCasts : public ::testing::Test {};

TEST(StringCasts, U32U32Test) {
    const auto from = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U32U8Test) {
    const auto from = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U32U16Test) {
    const auto from = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U32WTest) {
    const auto from = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};

TEST(StringCasts, U8U32Test) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U82U8Test) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U8U16Test) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U8WTest) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};

TEST(StringCasts, U16U32Test) {
    const auto from = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U162U8Test) {
    const auto from = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U16U16Test) {
    const auto from = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U16WTest) {
    const auto from = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};

TEST(StringCasts, WU32Test) {
    const auto from = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, W2U8Test) {
    const auto from = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, WU16Test) {
    const auto from = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, WWTest) {
    const auto from = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};


namespace uns::tests::string::casts {

    const auto testphrase = ::std::u32string{ U"AB\u304FCD e\u3051fgh !@\u30F1#% \u304CАБ\u3058ВГ\u3081д\u3062еёжзик_" };

    using types_list = ::testing::Types<
        ::std::u32string
        , ::std::u8string
        , ::std::string
        , ::std::u16string
        , ::std::wstring
    >;

};

template<::uns::is_basic_string string_t>
class ForwardCrossStringCasts : public ::testing::Test {};

TYPED_TEST_CASE(ForwardCrossStringCasts, ::uns::tests::string::casts::types_list);

TYPED_TEST(ForwardCrossStringCasts, IdenticalTest) {
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
            bool comparison_result = false;

            #pragma warning(push)
            #pragma warning(disable: 4553)
            ASSERT_THROW(
                comparison_result = (
                    ::std::get<::BooleanCasts::expectation>(GetParam())
                    == ::uns::string::cast<bool>(
                        ::std::get<::BooleanCasts::string>(GetParam())
                    )
                )
                , ::std::runtime_error
            );
            #pragma warning(pop)

            ASSERT_FALSE(comparison_result);

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
