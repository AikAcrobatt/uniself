
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <tuple>

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


class TrimTests : public ::testing::TestWithParam<
    ::std::tuple<::std::u32string, ::std::u32string, ::std::u32string>
> {
public:
    using elem_type = ::std::tuple<::std::u32string, ::std::u32string, ::std::u32string>;
    using string_type = ::std::u32string;
public:
    enum elem_part {
        prefix = 0
        , body = 1
        , postfix = 2
    };
public:
    static ::std::string to_string(const elem_type& Elem) {
        return testing::PrintToString(
                ::std::get<elem_part::prefix>(Elem)
            )
            + testing::PrintToString(
                ::std::get<elem_part::body>(Elem)
            )
            + testing::PrintToString(
                ::std::get<elem_part::postfix>(Elem)
            );
    };
};

template<>
::std::string testing::PrintToString(const ::TrimTests::elem_type& Elem) {
    return ::TrimTests::to_string(Elem);
};

TEST_P(TrimTests, ArgumentTrim) {
    auto origin = ::std::get<::TrimTests::elem_part::prefix>(GetParam())
        + ::std::get<::TrimTests::elem_part::body>(GetParam())
        + ::std::get<::TrimTests::elem_part::postfix>(GetParam());
    const auto expectation = ::std::get<::TrimTests::elem_part::body>(GetParam());

    ASSERT_NO_THROW(::uns::string::trim(origin));
    ASSERT_EQ(
        origin
        , expectation
    );
};
TEST_P(TrimTests, TrimmedResult) {
    const auto origin = ::std::get<::TrimTests::elem_part::prefix>(GetParam())
        + ::std::get<::TrimTests::elem_part::body>(GetParam())
        + ::std::get<::TrimTests::elem_part::postfix>(GetParam());
    const auto expectation = ::std::get<::TrimTests::elem_part::body>(GetParam());

    ASSERT_EQ(
        ::uns::string::trim(origin)
        , expectation
    );
};
INSTANTIATE_TEST_CASE_P(Trim, TrimTests,
    ::testing::Combine(
        ::testing::Values(
            ::std::u32string{ U"" }
            , ::std::u32string{ U" " }
            , ::std::u32string{ U"  " }
            , ::std::u32string{ U"   " }
            , ::std::u32string{ U"   \n" }
            , ::std::u32string{ U"   \n\t" }
            , ::std::u32string{ U"   \n\t\v" }
            , ::std::u32string{ U"   \n\t\v\a" }
            , ::std::u32string{ U"   \n\t\v\a\b" }
            , ::std::u32string{ U"   \n\t\v\a\b\r" }
            , ::std::u32string{ U"   \n\t\v\a\b\r\f" }
            , ::std::u32string{ U"   \n\t\v\a\b\r\f   " }
            , ::std::u32string{ U"           \n\t\v\a\b\r\f               " }
            , ::std::u32string{ U"           \n\t\v\a\b\r\f        \n\t\v\a\b\r\f       " }
        )
        , ::testing::Values(
            ::std::u32string{ U"A" }
            , ::std::u32string{ U"ABCD" }
            , ::std::u32string{ U"ABCD   efgh" }
            , ::std::u32string{ U"ABCD   efgh  \n\t\v\a\b\r\f  !@#%" }
            , ::std::u32string{ U"\u304C\u304C\u304C\u304C\u304C" }
            , ::std::u32string{ U"!@#% \u304CА \n\t\v\a\b\r\f  Б" }
            , ::std::u32string{ U"ЫЫЫ                         ЙЙЙ" }
            , ::std::u32string{ U"ABCD   efgh  \n\t\v\a\b\r\f  !@#% \u304CА \n\t\v\a\b\r\f  БВГдеёжзик_" }
        )
        , ::testing::Values(
            ::std::u32string{ U"" }
            , ::std::u32string{ U" " }
            , ::std::u32string{ U"  " }
            , ::std::u32string{ U"   " }
            , ::std::u32string{ U"   \n" }
            , ::std::u32string{ U"   \n\t" }
            , ::std::u32string{ U"   \n\t\v" }
            , ::std::u32string{ U"   \n\t\v\a" }
            , ::std::u32string{ U"   \n\t\v\a\b" }
            , ::std::u32string{ U"   \n\t\v\a\b\r" }
            , ::std::u32string{ U"   \n\t\v\a\b\r\f" }
            , ::std::u32string{ U"   \n\t\v\a\b\r\f   " }
            , ::std::u32string{ U"           \n\t\v\a\b\r\f               " }
            , ::std::u32string{ U"           \n\t\v\a\b\r\f        \n\t\v\a\b\r\f       " }
        )
    )
);


class StringCasts : public ::testing::Test {};

TEST(StringCasts, U32U32Test) {
    const auto from = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U32U8Test) {
    const auto from = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U32U16Test) {
    const auto from = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U32WTest) {
    const auto from = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};

TEST(StringCasts, U8U32Test) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U8U8Test) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U8U16Test) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U8WTest) {
    const auto from = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};

TEST(StringCasts, U16U32Test) {
    const auto from = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U16U8Test) {
    const auto from = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U16U16Test) {
    const auto from = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, U16WTest) {
    const auto from = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};

TEST(StringCasts, WU32Test) {
    const auto from = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u32string{ U"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, WU8Test) {
    const auto from = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u8string{ u8"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, WU16Test) {
    const auto from = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::u16string{ u"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

    ASSERT_EQ(
        ::uns::string::cast<decltype(to)>(from)
        , to
    );
};
TEST(StringCasts, WWTest) {
    const auto from = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };
    const auto to = ::std::wstring{ L"ABCD efgh !@\uD7FF\uE000#% \u304CАБВГдеёжзик_" };

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
class CrossStringCastsForward : public ::testing::Test {};

TYPED_TEST_CASE(CrossStringCastsForward, ::uns::tests::string::casts::types_list);
TYPED_TEST(CrossStringCastsForward, IdenticalTest) {
    ASSERT_TRUE(
        ::uns::tests::string::casts::testphrase == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<TypeParam>(::uns::tests::string::casts::testphrase)
        )
    );
};

namespace uns::testing {

    class letter_casts : public ::testing::TestWithParam<char32_t> {
    public:
        using elem_type = char32_t;
        using string_type = ::std::u32string;
    public:
        static string_type compose(const elem_type& Elem) {
            return string_type{ Elem };
        };
        static ::std::string to_string(const elem_type& Elem) {
            return ::testing::PrintToString(compose(Elem));
        };
    };

};


template<>
::std::string testing::PrintToString(const ::uns::testing::letter_casts::elem_type& Elem) {
    return ::uns::testing::letter_casts::to_string(Elem);
};

using NarrowLetterCasts = ::uns::testing::letter_casts;
TEST_P(NarrowLetterCasts, U8Tests) {
    ASSERT_TRUE(
        compose(GetParam()) == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<::std::u8string>(compose(GetParam()))
        )
    );
};
TEST_P(NarrowLetterCasts, STests) {
    ASSERT_TRUE(
        compose(GetParam()) == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<::std::string>(compose(GetParam()))
        )
    );
};
INSTANTIATE_TEST_CASE_P(LetterCasts, NarrowLetterCasts,
    ::testing::Range<char32_t>(0x0001, 0xFFFF)
);

using WideLetterCasts = ::uns::testing::letter_casts;
TEST_P(WideLetterCasts, U16Tests) {
    ASSERT_TRUE(
        compose(GetParam()) == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<::std::u16string>(compose(GetParam()))
        )
    );
};
TEST_P(WideLetterCasts, WTests) {
    ASSERT_TRUE(
        compose(GetParam()) == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<::std::wstring>(compose(GetParam()))
        )
    );
};
INSTANTIATE_TEST_CASE_P(LowerLetterCasts, WideLetterCasts,
    ::testing::Range<char32_t>(0x0001, 0xD7FF)
);
INSTANTIATE_TEST_CASE_P(UpperLetterCasts, WideLetterCasts,
    ::testing::Range<char32_t>(0xE000, 0xFFFF)
);


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


template<typename numeric_t, ::uns::is_basic_string string_t>
class NumericCasts : public ::testing::TestWithParam<
    ::std::tuple<string_t, numeric_t, ::correct, string_t>
> {
public:
    using elem_type = ::std::tuple<string_t, numeric_t, ::correct, string_t>;
    using string_type = string_t;
    using numeric_type = numeric_t;
public:
    enum elem_part {
        string = 0
        , expectation = 1
        , correctness = 2
        , string_equ = 3
    };
public:
    static elem_type make(
        string_type Str
        , numeric_type Num
        , ::correct Cor
    ) {
        return { Str, Num, Cor, Str };
    };
public:
    static ::std::string to_string(const elem_type& Elem) {
        auto ss = ::std::stringstream{};
        ss << ::std::get<elem_part::expectation>(Elem);

        auto expected = ::std::string{};
        ss >> expected;

        return "IntegerCasts::Elem{ "
            + testing::PrintToString(
                ::std::get<elem_part::string>(Elem)
            )
            + " ?= "
            + expected
            + ", correct::"
            + ::uns::string::cast<::std::string>(
                ::std::get<elem_part::correctness>(Elem).to_string()
            )
            + " }";
    };
};


template<typename numeric_t, ::uns::is_basic_string string_t>
class IntegerCasts: public NumericCasts<numeric_t, string_t> {};

class UnsignedIntegerCastsU32 : public ::IntegerCasts<unsigned long long int, ::std::u32string> {};
class UnsignedIntegerCastsS : public ::IntegerCasts<unsigned long long int, ::std::string> {};
class UnsignedIntegerCastsU8 : public ::IntegerCasts<unsigned long long int, ::std::u8string> {};
class UnsignedIntegerCastsU16 : public ::IntegerCasts<unsigned long long int, ::std::u16string> {};
class UnsignedIntegerCastsW : public ::IntegerCasts<unsigned long long int, ::std::wstring> {};

template<>
::std::string testing::PrintToString(const ::UnsignedIntegerCastsU32::elem_type& Elem) {
    return ::UnsignedIntegerCastsU32::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::UnsignedIntegerCastsS ::elem_type& Elem) {
    return ::UnsignedIntegerCastsS::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::UnsignedIntegerCastsU8::elem_type& Elem) {
    return ::UnsignedIntegerCastsU8::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::UnsignedIntegerCastsU16::elem_type& Elem) {
    return ::UnsignedIntegerCastsU16::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::UnsignedIntegerCastsW::elem_type& Elem) {
    return ::UnsignedIntegerCastsW::to_string(Elem);
};

TEST_P(UnsignedIntegerCastsU32, ForwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(UnsignedIntegerCastsU32, BackwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, UnsignedIntegerCastsU32,
    ::testing::Values(
        UnsignedIntegerCastsU32::make( U"0",          0,      ::correct::yes )
        , UnsignedIntegerCastsU32::make( U"1",         1,      ::correct::yes )
        , UnsignedIntegerCastsU32::make( U"10423",     10423,  ::correct::yes )
        , UnsignedIntegerCastsU32::make( U"789",       789,    ::correct::yes )
        , UnsignedIntegerCastsU32::make( U"22304568",  22304568, ::correct::yes )
        , UnsignedIntegerCastsU32::make( U"22304568 ", 22304568, ::correct::no )
        , UnsignedIntegerCastsU32::make( U" 0",        0,      ::correct::no )
        , UnsignedIntegerCastsU32::make( U"1 ",        1,      ::correct::no )
        , UnsignedIntegerCastsU32::make( U"10_423",    10423,  ::correct::no )
        , UnsignedIntegerCastsU32::make( U"8-8",       88,     ::correct::no )
        , UnsignedIntegerCastsU32::make( U"7J9",       7,      ::correct::no )
        , UnsignedIntegerCastsU32::make( U"2230 4568 ",2230,   ::correct::no )
        , UnsignedIntegerCastsU32::make( U"-56",       -56,    ::correct::no )
        , UnsignedIntegerCastsU32::make( U"",          0,      ::correct::no )
        , UnsignedIntegerCastsU32::elem_type{ U"0000", 0,      ::correct::yes, U"0" }
        , UnsignedIntegerCastsU32::elem_type{ U"0000001", 1,   ::correct::yes, U"1" }
    )
);

TEST_P(UnsignedIntegerCastsS, ForwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(UnsignedIntegerCastsS, BackwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, UnsignedIntegerCastsS,
    ::testing::Values(
        UnsignedIntegerCastsS::make( "0",           0,      ::correct::yes )
        , UnsignedIntegerCastsS::make( "1",         1,      ::correct::yes )
        , UnsignedIntegerCastsS::make( "10423",     10423,  ::correct::yes )
        , UnsignedIntegerCastsS::make( "789",       789,    ::correct::yes )
        , UnsignedIntegerCastsS::make( "22304568",  22304568, ::correct::yes )
        , UnsignedIntegerCastsS::make( "22304568 ", 22304568, ::correct::no )
        , UnsignedIntegerCastsS::make( " 0",        0,      ::correct::no )
        , UnsignedIntegerCastsS::make( "1 ",        1,      ::correct::no )
        , UnsignedIntegerCastsS::make( "10_423",    10423,  ::correct::no )
        , UnsignedIntegerCastsS::make( "8-8",       88,     ::correct::no )
        , UnsignedIntegerCastsS::make( "7J9",       7,      ::correct::no )
        , UnsignedIntegerCastsS::make( "2230 4568 ",2230,   ::correct::no )
        , UnsignedIntegerCastsS::make( "-56",       -56,    ::correct::no )
        , UnsignedIntegerCastsS::make( "",          0,      ::correct::no)
        , UnsignedIntegerCastsS::elem_type{ "0000", 0,      ::correct::yes, "0" }
        , UnsignedIntegerCastsS::elem_type{ "0000001", 1,   ::correct::yes, "1" }
    )
);

TEST_P(UnsignedIntegerCastsU8, ForwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(UnsignedIntegerCastsU8, BackwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, UnsignedIntegerCastsU8,
    ::testing::Values(
        UnsignedIntegerCastsU8::make( u8"0",           0,      ::correct::yes )
        , UnsignedIntegerCastsU8::make( u8"1",         1,      ::correct::yes )
        , UnsignedIntegerCastsU8::make( u8"10423",     10423,  ::correct::yes )
        , UnsignedIntegerCastsU8::make( u8"789",       789,    ::correct::yes )
        , UnsignedIntegerCastsU8::make( u8"22304568",  22304568, ::correct::yes )
        , UnsignedIntegerCastsU8::make( u8"22304568 ", 22304568, ::correct::no )
        , UnsignedIntegerCastsU8::make( u8" 0",        0,      ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"1 ",        1,      ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"10_423",    10423,  ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"8-8",       88,     ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"7J9",       7,      ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"2230 4568 ",2230,   ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"-56",       -56,    ::correct::no )
        , UnsignedIntegerCastsU8::make( u8"",          0,      ::correct::no )
        , UnsignedIntegerCastsU8::elem_type{ u8"0000", 0,      ::correct::yes, u8"0" }
        , UnsignedIntegerCastsU8::elem_type{ u8"0000001", 1,   ::correct::yes, u8"1" }
    )
);

TEST_P(UnsignedIntegerCastsU16, ForwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(UnsignedIntegerCastsU16, BackwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, UnsignedIntegerCastsU16,
    ::testing::Values(
        UnsignedIntegerCastsU16::make( u"0",           0,      ::correct::yes )
        , UnsignedIntegerCastsU16::make( u"1",         1,      ::correct::yes )
        , UnsignedIntegerCastsU16::make( u"10423",     10423,  ::correct::yes )
        , UnsignedIntegerCastsU16::make( u"789",       789,    ::correct::yes )
        , UnsignedIntegerCastsU16::make( u"22304568",  22304568, ::correct::yes )
        , UnsignedIntegerCastsU16::make( u"22304568 ", 22304568, ::correct::no )
        , UnsignedIntegerCastsU16::make( u" 0",        0,      ::correct::no )
        , UnsignedIntegerCastsU16::make( u"1 ",        1,      ::correct::no )
        , UnsignedIntegerCastsU16::make( u"10_423",    10423,  ::correct::no )
        , UnsignedIntegerCastsU16::make( u"8-8",       88,     ::correct::no )
        , UnsignedIntegerCastsU16::make( u"7J9",       7,      ::correct::no )
        , UnsignedIntegerCastsU16::make( u"2230 4568 ",2230,   ::correct::no )
        , UnsignedIntegerCastsU16::make( u"-56",       -56,    ::correct::no )
        , UnsignedIntegerCastsU16::make( u"",          0,      ::correct::no )
        , UnsignedIntegerCastsU16::elem_type{ u"0000", 0,      ::correct::yes, u"0" }
        , UnsignedIntegerCastsU16::elem_type{ u"0000001", 1,   ::correct::yes, u"1" }
    )
);

TEST_P(UnsignedIntegerCastsW, ForwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(UnsignedIntegerCastsW, BackwardCastTest) {
    using TestCaseFixture = UnsignedIntegerCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, UnsignedIntegerCastsW,
    ::testing::Values(
        UnsignedIntegerCastsW::make( L"0",           0,      ::correct::yes )
        , UnsignedIntegerCastsW::make( L"1",         1,      ::correct::yes )
        , UnsignedIntegerCastsW::make( L"10423",     10423,  ::correct::yes )
        , UnsignedIntegerCastsW::make( L"789",       789,    ::correct::yes )
        , UnsignedIntegerCastsW::make( L"22304568",  22304568, ::correct::yes )
        , UnsignedIntegerCastsW::make( L"22304568 ", 22304568, ::correct::no )
        , UnsignedIntegerCastsW::make( L" 0",        0,      ::correct::no )
        , UnsignedIntegerCastsW::make( L"1 ",        1,      ::correct::no )
        , UnsignedIntegerCastsW::make( L"10_423",    10423,  ::correct::no )
        , UnsignedIntegerCastsW::make( L"8-8",       88,     ::correct::no )
        , UnsignedIntegerCastsW::make( L"7J9",       7,      ::correct::no )
        , UnsignedIntegerCastsW::make( L"2230 4568 ",2230,   ::correct::no )
        , UnsignedIntegerCastsW::make( L"-56",       -56,    ::correct::no )
        , UnsignedIntegerCastsW::make( L"",          0,      ::correct::no )
        , UnsignedIntegerCastsW::elem_type{ L"0000", 0,      ::correct::yes, L"0" }
        , UnsignedIntegerCastsW::elem_type{ L"0000001", 1,   ::correct::yes, L"1" }
    )
);

class SignedIntegerCastsU32 : public ::IntegerCasts<long long int, ::std::u32string> {};
class SignedIntegerCastsS : public ::IntegerCasts<long long int, ::std::string> {};
class SignedIntegerCastsU8 : public ::IntegerCasts<long long int, ::std::u8string> {};
class SignedIntegerCastsU16 : public ::IntegerCasts<long long int, ::std::u16string> {};
class SignedIntegerCastsW : public ::IntegerCasts<long long int, ::std::wstring> {};

template<>
::std::string testing::PrintToString(const ::SignedIntegerCastsU32::elem_type& Elem) {
    return ::SignedIntegerCastsU32::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SignedIntegerCastsS::elem_type& Elem) {
    return ::SignedIntegerCastsS::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SignedIntegerCastsU8::elem_type& Elem) {
    return ::SignedIntegerCastsU8::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SignedIntegerCastsU16::elem_type& Elem) {
    return ::SignedIntegerCastsU16::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SignedIntegerCastsW::elem_type& Elem) {
    return ::SignedIntegerCastsW::to_string(Elem);
};

TEST_P(SignedIntegerCastsU32, ForwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SignedIntegerCastsU32, BackwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SignedIntegerCastsU32,
    ::testing::Values(
        SignedIntegerCastsU32::make( U"0",           0,      ::correct::yes )
        , SignedIntegerCastsU32::elem_type(U"-0",    0,      ::correct::yes, U"0")
        , SignedIntegerCastsU32::make( U"1",         1,      ::correct::yes )
        , SignedIntegerCastsU32::make( U"-1",        -1,     ::correct::yes )
        , SignedIntegerCastsU32::make( U"10423",     10423,  ::correct::yes )
        , SignedIntegerCastsU32::make( U"-88",       -88,    ::correct::yes )
        , SignedIntegerCastsU32::make( U"- 88",      -88,    ::correct::no )
        , SignedIntegerCastsU32::make( U"789",       789,    ::correct::yes )
        , SignedIntegerCastsU32::make( U"22304568",  22304568, ::correct::yes )
        , SignedIntegerCastsU32::make( U"22304568 ", 22304568, ::correct::no )
        , SignedIntegerCastsU32::make( U"-4444",     -4444,  ::correct::yes )
        , SignedIntegerCastsU32::make( U"-4444 ",    -4444,  ::correct::no )
        , SignedIntegerCastsU32::make(U"--4444", -4444, ::correct::no)
        , SignedIntegerCastsU32::make(U"+-4444", -4444, ::correct::no)
        , SignedIntegerCastsU32::make(U"-+4444", -4444, ::correct::no)
        , SignedIntegerCastsU32::make(U"++4444", -4444, ::correct::no)
        , SignedIntegerCastsU32::make( U" 0",        0,      ::correct::no )
        , SignedIntegerCastsU32::make( U"1 ",        1,      ::correct::no )
        , SignedIntegerCastsU32::make( U"10_423",    10423,  ::correct::no )
        , SignedIntegerCastsU32::make( U"8-8",       88,     ::correct::no )
        , SignedIntegerCastsU32::make( U"7J9",       7,      ::correct::no )
        , SignedIntegerCastsU32::make( U"2230 4568 ",2230,   ::correct::no )
        , SignedIntegerCastsU32::make( U"-44 44 ",   -44,    ::correct::no )
        , SignedIntegerCastsU32::make( U"",          0,      ::correct::no )
        , SignedIntegerCastsU32::elem_type{ U"0000", 0,      ::correct::yes, U"0" }
        , SignedIntegerCastsU32::elem_type{ U"0000001", 1,   ::correct::yes, U"1" }
        , SignedIntegerCastsU32::elem_type{ U"-00088", -88,   ::correct::yes, U"-88" }
    )
);

TEST_P(SignedIntegerCastsS, ForwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SignedIntegerCastsS, BackwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SignedIntegerCastsS,
    ::testing::Values(
        SignedIntegerCastsS::make( "0",           0,      ::correct::yes )
        , SignedIntegerCastsS::elem_type( "-0",   0,      ::correct::yes, "0")
        , SignedIntegerCastsS::make( "1",         1,      ::correct::yes )
        , SignedIntegerCastsS::make( "-1",        -1,     ::correct::yes)
        , SignedIntegerCastsS::make( "10423",     10423,  ::correct::yes )
        , SignedIntegerCastsS::make( "-88",       -88,    ::correct::yes )
        , SignedIntegerCastsS::make( "- 88",      -88,    ::correct::no )
        , SignedIntegerCastsS::make( "789",       789,    ::correct::yes )
        , SignedIntegerCastsS::make( "22304568",  22304568, ::correct::yes )
        , SignedIntegerCastsS::make( "22304568 ", 22304568, ::correct::no )
        , SignedIntegerCastsS::make( "-4444",     -4444,  ::correct::yes )
        , SignedIntegerCastsS::make( "-4444 ",    -4444,  ::correct::no )
        , SignedIntegerCastsS::make("--4444", -4444, ::correct::no)
        , SignedIntegerCastsS::make("+-4444", -4444, ::correct::no)
        , SignedIntegerCastsS::make("-+4444", -4444, ::correct::no)
        , SignedIntegerCastsS::make("++4444", -4444, ::correct::no)
        , SignedIntegerCastsS::make( " 0",        0,      ::correct::no )
        , SignedIntegerCastsS::make( "1 ",        1,      ::correct::no )
        , SignedIntegerCastsS::make( "10_423",    10423,  ::correct::no )
        , SignedIntegerCastsS::make( "8-8",       88,     ::correct::no )
        , SignedIntegerCastsS::make( "7J9",       7,      ::correct::no )
        , SignedIntegerCastsS::make( "2230 4568 ",2230,   ::correct::no )
        , SignedIntegerCastsS::make( "-44 44 ",   -44,    ::correct::no )
        , SignedIntegerCastsS::make( "",          0,      ::correct::no )
        , SignedIntegerCastsS::elem_type{ "0000", 0,      ::correct::yes, "0" }
        , SignedIntegerCastsS::elem_type{ "0000001", 1,   ::correct::yes, "1" }
        , SignedIntegerCastsS::elem_type{ "-00088", -88,   ::correct::yes, "-88" }
    )
);

TEST_P(SignedIntegerCastsU8, ForwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SignedIntegerCastsU8, BackwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SignedIntegerCastsU8,
    ::testing::Values(
        SignedIntegerCastsU8::make( u8"0",           0,      ::correct::yes )
        , SignedIntegerCastsU8::elem_type(u8"-0",     0,      ::correct::yes, u8"0")
        , SignedIntegerCastsU8::make( u8"1",         1,      ::correct::yes )
        , SignedIntegerCastsU8::make( u8"-1",        -1,     ::correct::yes)
        , SignedIntegerCastsU8::make( u8"10423",     10423,  ::correct::yes )
        , SignedIntegerCastsU8::make( u8"-88",       -88,    ::correct::yes )
        , SignedIntegerCastsU8::make( u8"- 88",      -88,    ::correct::no )
        , SignedIntegerCastsU8::make( u8"789",       789,    ::correct::yes )
        , SignedIntegerCastsU8::make( u8"22304568",  22304568, ::correct::yes )
        , SignedIntegerCastsU8::make( u8"22304568 ", 22304568, ::correct::no )
        , SignedIntegerCastsU8::make( u8"-4444",     -4444,  ::correct::yes )
        , SignedIntegerCastsU8::make( u8"-4444 ",    -4444,  ::correct::no )
        , SignedIntegerCastsU8::make(u8"--4444", -4444, ::correct::no)
        , SignedIntegerCastsU8::make(u8"+-4444", -4444, ::correct::no)
        , SignedIntegerCastsU8::make(u8"-+4444", -4444, ::correct::no)
        , SignedIntegerCastsU8::make(u8"++4444", -4444, ::correct::no)
        , SignedIntegerCastsU8::make( u8" 0",        0,      ::correct::no )
        , SignedIntegerCastsU8::make( u8"1 ",        1,      ::correct::no )
        , SignedIntegerCastsU8::make( u8"10_423",    10423,  ::correct::no )
        , SignedIntegerCastsU8::make( u8"8-8",       88,     ::correct::no )
        , SignedIntegerCastsU8::make( u8"7J9",       7,      ::correct::no )
        , SignedIntegerCastsU8::make( u8"2230 4568 ",2230,   ::correct::no )
        , SignedIntegerCastsU8::make( u8"-44 44 ",   -44,    ::correct::no )
        , SignedIntegerCastsU8::make( u8"",          0,      ::correct::no )
        , SignedIntegerCastsU8::elem_type{ u8"0000", 0,      ::correct::yes, u8"0" }
        , SignedIntegerCastsU8::elem_type{ u8"0000001", 1,   ::correct::yes, u8"1" }
        , SignedIntegerCastsU8::elem_type{ u8"-00088", -88,   ::correct::yes, u8"-88" }
    )
);

TEST_P(SignedIntegerCastsU16, ForwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SignedIntegerCastsU16, BackwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SignedIntegerCastsU16,
    ::testing::Values(
        SignedIntegerCastsU16::make( u"0",           0,      ::correct::yes )
        , SignedIntegerCastsU16::elem_type(u"-0",    0,      ::correct::yes, u"0")
        , SignedIntegerCastsU16::make( u"1",         1,      ::correct::yes )
        , SignedIntegerCastsU16::make( u"-1",        -1,     ::correct::yes)
        , SignedIntegerCastsU16::make( u"10423",     10423,  ::correct::yes )
        , SignedIntegerCastsU16::make( u"-88",       -88,    ::correct::yes )
        , SignedIntegerCastsU16::make( u"- 88",      -88,    ::correct::no )
        , SignedIntegerCastsU16::make( u"789",       789,    ::correct::yes )
        , SignedIntegerCastsU16::make( u"22304568",  22304568, ::correct::yes )
        , SignedIntegerCastsU16::make( u"22304568 ", 22304568, ::correct::no )
        , SignedIntegerCastsU16::make( u"-4444",     -4444,  ::correct::yes )
        , SignedIntegerCastsU16::make( u"-4444 ",    -4444,  ::correct::no )
        , SignedIntegerCastsU16::make( u"--4444",    -4444,  ::correct::no )
        , SignedIntegerCastsU16::make( u"+-4444",    -4444,  ::correct::no )
        , SignedIntegerCastsU16::make( u"-+4444",    -4444,  ::correct::no )
        , SignedIntegerCastsU16::make( u"++4444",    -4444,  ::correct::no )
        , SignedIntegerCastsU16::make( u" 0",        0,      ::correct::no )
        , SignedIntegerCastsU16::make( u"1 ",        1,      ::correct::no )
        , SignedIntegerCastsU16::make( u"10_423",    10423,  ::correct::no )
        , SignedIntegerCastsU16::make( u"8-8",       88,     ::correct::no )
        , SignedIntegerCastsU16::make( u"7J9",       7,      ::correct::no )
        , SignedIntegerCastsU16::make( u"2230 4568 ",2230,   ::correct::no )
        , SignedIntegerCastsU16::make( u"-44 44 ",   -44,    ::correct::no )
        , SignedIntegerCastsU16::make( u"",          0,      ::correct::no )
        , SignedIntegerCastsU16::elem_type{ u"0000", 0,      ::correct::yes, u"0" }
        , SignedIntegerCastsU16::elem_type{ u"0000001", 1,   ::correct::yes, u"1" }
        , SignedIntegerCastsU16::elem_type{ u"-00088", -88,   ::correct::yes, u"-88" }
    )
);

TEST_P(SignedIntegerCastsW, ForwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SignedIntegerCastsW, BackwardCastTest) {
    using TestCaseFixture = SignedIntegerCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SignedIntegerCastsW,
    ::testing::Values(
        SignedIntegerCastsW::make( L"0",           0,      ::correct::yes )
        , SignedIntegerCastsW::elem_type(L"-0",    0,      ::correct::yes, L"0")
        , SignedIntegerCastsW::make( L"1",         1,      ::correct::yes )
        , SignedIntegerCastsW::make( L"-1",        -1,     ::correct::yes)
        , SignedIntegerCastsW::make( L"10423",     10423,  ::correct::yes )
        , SignedIntegerCastsW::make( L"-88",       -88,    ::correct::yes )
        , SignedIntegerCastsW::make( L"- 88",      -88,    ::correct::no )
        , SignedIntegerCastsW::make( L"789",       789,    ::correct::yes )
        , SignedIntegerCastsW::make( L"22304568",  22304568, ::correct::yes )
        , SignedIntegerCastsW::make( L"22304568 ", 22304568, ::correct::no )
        , SignedIntegerCastsW::make( L"-4444",     -4444,  ::correct::yes )
        , SignedIntegerCastsW::make( L"-4444 ",    -4444,  ::correct::no )
        , SignedIntegerCastsW::make( L"--4444",    -4444,  ::correct::no )
        , SignedIntegerCastsW::make( L"+-4444",    -4444,  ::correct::no )
        , SignedIntegerCastsW::make( L"-+4444",    -4444,  ::correct::no )
        , SignedIntegerCastsW::make( L"++4444",    -4444,  ::correct::no )
        , SignedIntegerCastsW::make( L" 0",        0,      ::correct::no )
        , SignedIntegerCastsW::make( L"1 ",        1,      ::correct::no )
        , SignedIntegerCastsW::make( L"10_423",    10423,  ::correct::no )
        , SignedIntegerCastsW::make( L"8-8",       88,     ::correct::no )
        , SignedIntegerCastsW::make( L"7J9",       7,      ::correct::no )
        , SignedIntegerCastsW::make( L"2230 4568 ",2230,   ::correct::no )
        , SignedIntegerCastsW::make( L"-44 44 ",   -44,    ::correct::no )
        , SignedIntegerCastsW::make( L"",          0,      ::correct::no )
        , SignedIntegerCastsW::elem_type{ L"0000", 0,      ::correct::yes, L"0" }
        , SignedIntegerCastsW::elem_type{ L"0000001", 1,   ::correct::yes, L"1" }
        , SignedIntegerCastsW::elem_type{ L"-00088", -88,   ::correct::yes, L"-88" }
    )
);


class HexUnsignedIntegerCastsU32 : public ::IntegerCasts<unsigned long long int, ::std::u32string> {};

TEST_P(HexUnsignedIntegerCastsU32, ForwardCastTest) {
    using TestCaseFixture = HexUnsignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::to_hex<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::to_hex<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::to_hex<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(HexUnsignedIntegerCastsU32, BackwardCastTest) {
    using TestCaseFixture = HexUnsignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, HexUnsignedIntegerCastsU32,
    ::testing::Values(
        HexUnsignedIntegerCastsU32::make(U"0x0", 0, ::correct::yes)
        , HexUnsignedIntegerCastsU32::make(U"0x1", 1, ::correct::yes)
        , HexUnsignedIntegerCastsU32::elem_type(U"0X10423", 0x10423, ::correct::yes, U"0x10423")
        , HexUnsignedIntegerCastsU32::elem_type(U"+0x7a8b8", 0x7a8b8, ::correct::yes, U"0x7a8b8")
        , HexUnsignedIntegerCastsU32::make(U"0x22304568", 0x22304568, ::correct::yes)
        , HexUnsignedIntegerCastsU32::elem_type(U"0X22304568", 0x22304568, ::correct::yes, U"0x22304568")
        , HexUnsignedIntegerCastsU32::elem_type(U"0x01234789abcdef", 0x01234789abcdef, ::correct::yes, U"0x1234789abcdef")
        , HexUnsignedIntegerCastsU32::elem_type(U"+0X01234789abcdef", 0x01234789abcdef, ::correct::yes, U"0x1234789abcdef")
        , HexUnsignedIntegerCastsU32::make(U"0x22304568 ", 22304568, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U" 0x0", 0, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x1 ", 1, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x10_423", 0x10423, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x8-8", 0x88, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x7j9", 0x7d9, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x2230 4568 ", 0x22304568, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x-56", -0x56, ::correct::no)
        , HexUnsignedIntegerCastsU32::elem_type{ U"0x0000", 0x0,      ::correct::yes, U"0x0" }
        , HexUnsignedIntegerCastsU32::elem_type{ U"+0x0000001", 0x1,   ::correct::yes, U"0x1" }
        , HexUnsignedIntegerCastsU32::make(U"x10423", 0x10423, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"++0x10423", 0x10423, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"00x7a8b9", 0x7a8b9, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"0x 114", 0x114, ::correct::no)
        , HexUnsignedIntegerCastsU32::make(U"", 0, ::correct::no)
        , HexUnsignedIntegerCastsU32::elem_type(U"0X10A23", 0x10a23, ::correct::yes, U"0x10a23")
        , HexUnsignedIntegerCastsU32::elem_type(U"0x7a8B8", 0x7a8b8, ::correct::yes, U"0x7a8b8")
        , HexUnsignedIntegerCastsU32::elem_type(U"0x2aC304Ef68", 0x2ac304ef68, ::correct::yes, U"0x2ac304ef68")
        , HexUnsignedIntegerCastsU32::elem_type(U"0X2Ac304eF68", 0x2ac304ef68, ::correct::yes, U"0x2ac304ef68")
        , HexUnsignedIntegerCastsU32::elem_type(U"+0x0123456789ABCDEF", 0x0123456789abcdef, ::correct::yes, U"0x123456789abcdef")
        , HexUnsignedIntegerCastsU32::elem_type(U"0X0123456789ABCDEF", 0x0123456789abcdef, ::correct::yes, U"0x123456789abcdef")
    )
);


class HexSignedIntegerCastsU32 : public ::IntegerCasts<long long int, ::std::u32string> {};

TEST_P(HexSignedIntegerCastsU32, ForwardCastTest) {
    using TestCaseFixture = HexSignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::to_hex<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::to_hex<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::to_hex<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(HexSignedIntegerCastsU32, BackwardCastTest) {
    using TestCaseFixture = HexSignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, HexSignedIntegerCastsU32,
    ::testing::Values(
        HexSignedIntegerCastsU32::make(U"0x0", 0, ::correct::yes)
        , HexSignedIntegerCastsU32::elem_type(U"-0x0", -0, ::correct::yes, U"0x0")
        , HexSignedIntegerCastsU32::make(U"-0x1", -1, ::correct::yes)
        , HexSignedIntegerCastsU32::elem_type(U"0X10423", 0x10423, ::correct::yes, U"0x10423")
        , HexSignedIntegerCastsU32::make(U"-0x7a8b9", -0x7a8b9, ::correct::yes)
        , HexSignedIntegerCastsU32::make(U"0x22304568", 0x22304568, ::correct::yes)
        , HexSignedIntegerCastsU32::elem_type(U"-0X22304568", -0x22304568, ::correct::yes, U"-0x22304568")
        , HexSignedIntegerCastsU32::elem_type(U"-0x01234567abcdef", -0x1234567abcdef, ::correct::yes, U"-0x1234567abcdef")
        , HexSignedIntegerCastsU32::elem_type(U"+0X01234567abcdef", 0x1234567abcdef, ::correct::yes, U"0x1234567abcdef")
        , HexSignedIntegerCastsU32::make(U"-0x22304568 ", -22304568, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"- 0x0", -0, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"-0x1 ", -1, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"0x10_423", 0x10423, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"0x8-8", 0x88, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"0x7j9", 0x7d9, ::correct::no)
        , HexSignedIntegerCastsU32::make(U" -0x7d9", -0x7d9, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"0x2230 4568 ", 0x22304568, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"0x-56", -0x56, ::correct::no)
        , HexSignedIntegerCastsU32::elem_type{ U"-0x0000", -0x0,      ::correct::yes, U"0x0" }
        , HexSignedIntegerCastsU32::elem_type{ U"0x0000001", 0x1,   ::correct::yes, U"0x1" }
        , HexSignedIntegerCastsU32::make(U"++0x10423", 0x10423, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"--0X10423", 0x10423, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"x10423", 0x10423, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"X10423", 0x10423, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"00x7a8b9", 0x7a8b9, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"0x 114", 0x114, ::correct::no)
        , HexSignedIntegerCastsU32::make(U"", 0, ::correct::no)
        , HexSignedIntegerCastsU32::elem_type(U"0X10A23", 0x10a23, ::correct::yes, U"0x10a23")
        , HexSignedIntegerCastsU32::elem_type(U"+0x7a8B8", 0x7a8b8, ::correct::yes, U"0x7a8b8")
        , HexSignedIntegerCastsU32::elem_type(U"-0x2aC304Ef68", -0x2ac304ef68, ::correct::yes, U"-0x2ac304ef68")
        , HexSignedIntegerCastsU32::elem_type(U"0X2Ac304eF68", 0x2ac304ef68, ::correct::yes, U"0x2ac304ef68")
        , HexSignedIntegerCastsU32::elem_type(U"0x0123456789ABCDEF", 0x0123456789abcdef, ::correct::yes, U"0x123456789abcdef")
        , HexSignedIntegerCastsU32::elem_type(U"+0X0123456789ABCDEF", 0x0123456789abcdef, ::correct::yes, U"0x123456789abcdef")
    )
);


class BinUnsignedIntegerCastsU32 : public ::IntegerCasts<unsigned long long int, ::std::u32string> {};

TEST_P(BinUnsignedIntegerCastsU32, ForwardCastTest) {
    using TestCaseFixture = BinUnsignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::to_bin<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::to_bin<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::to_bin<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(BinUnsignedIntegerCastsU32, BackwardCastTest) {
    using TestCaseFixture = BinUnsignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, BinUnsignedIntegerCastsU32,
    ::testing::Values(
        BinUnsignedIntegerCastsU32::make(U"0b0", 0, ::correct::yes)
        , BinUnsignedIntegerCastsU32::make(U"0b1", 1, ::correct::yes)
        , BinUnsignedIntegerCastsU32::elem_type(U"0B10110", 0b10110, ::correct::yes, U"0b10110")
        , BinUnsignedIntegerCastsU32::make(U"0b1000101", 0b1000101, ::correct::yes)
        , BinUnsignedIntegerCastsU32::elem_type(U"+0b11111101110110", 0b11111101110110, ::correct::yes, U"0b11111101110110")
        , BinUnsignedIntegerCastsU32::elem_type(U"0B11111101110110", 0b11111101110110, ::correct::yes, U"0b11111101110110")
        , BinUnsignedIntegerCastsU32::elem_type(U"0b000111111011101101000010", 0b111111011101101000010, ::correct::yes, U"0b111111011101101000010")
        , BinUnsignedIntegerCastsU32::elem_type(U"+0B000111111011101101000010", 0b111111011101101000010, ::correct::yes, U"0b111111011101101000010")
        , BinUnsignedIntegerCastsU32::make(U"0b1000101 ", 0b1000101, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U" 0b0", 0, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b1 ", 1, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"++0b1", 1, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b10_111", 0b10111, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b0-1", 0b01, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b1j1", 0b101, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"++0b101", 0b101, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b1110 1010 ", 0b11101010, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b-11", -0b11, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"", 0, ::correct::no)
        , BinUnsignedIntegerCastsU32::elem_type{ U"0b0000", 0b0,      ::correct::yes, U"0b0" }
        , BinUnsignedIntegerCastsU32::elem_type{ U"+0b0000001", 0b1,   ::correct::yes, U"0b1" }
        , BinUnsignedIntegerCastsU32::make(U"b10101", 0b10101, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"00b10010", 0b10010, ::correct::no)
        , BinUnsignedIntegerCastsU32::make(U"0b 110", 0b110, ::correct::no)
    )
);


class BinSignedIntegerCastsU32 : public ::IntegerCasts<long long int, ::std::u32string> {};

TEST_P(BinSignedIntegerCastsU32, ForwardCastTest) {
    using TestCaseFixture = BinSignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::to_bin<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::to_bin<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::to_bin<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(BinSignedIntegerCastsU32, BackwardCastTest) {
    using TestCaseFixture = BinSignedIntegerCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, BinSignedIntegerCastsU32,
    ::testing::Values(
        BinSignedIntegerCastsU32::make(U"0b0", 0, ::correct::yes)
        , BinSignedIntegerCastsU32::make(U"-0b1", -1, ::correct::yes)
        , BinSignedIntegerCastsU32::elem_type(U"+0B10110", 0b10110, ::correct::yes, U"0b10110")
        , BinSignedIntegerCastsU32::make(U"0b1000101", 0b1000101, ::correct::yes)
        , BinSignedIntegerCastsU32::make(U"-0b11111101110110", -0b11111101110110, ::correct::yes)
        , BinSignedIntegerCastsU32::elem_type(U"0B11111101110110", 0b11111101110110, ::correct::yes, U"0b11111101110110")
        , BinSignedIntegerCastsU32::elem_type(U"+0b000111111011101101000010", 0b111111011101101000010, ::correct::yes, U"0b111111011101101000010")
        , BinSignedIntegerCastsU32::elem_type(U"-0B000111111011101101000010", -0b111111011101101000010, ::correct::yes, U"-0b111111011101101000010")
        , BinSignedIntegerCastsU32::make(U"0b1000101 ", 0b1000101, ::correct::no)
        , BinSignedIntegerCastsU32::make(U" 0b0", 0, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b1 ", 1, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"++0b1", 1, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"--0b1", 1, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b10_111", 0b10111, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b0-1", 0b01, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b1j1", 0b101, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b1110 1010 ", 0b11101010, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b-11", -0b11, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"", 0, ::correct::no)
        , BinSignedIntegerCastsU32::elem_type{ U"0b0000", 0b0,      ::correct::yes, U"0b0" }
        , BinSignedIntegerCastsU32::elem_type{ U"-0b0000001", -0b1,   ::correct::yes, U"-0b1" }
        , BinSignedIntegerCastsU32::make(U"b10101", 0b10101, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"00b10010", 0b10010, ::correct::no)
        , BinSignedIntegerCastsU32::make(U"0b 110", 0b110, ::correct::no)
    )
);


template<typename numeric_t, ::uns::is_basic_string string_t>
class FloatingPointCasts : public NumericCasts<numeric_t, string_t> {};

class SinglePrecisionFloatingPointCastsU32 : public FloatingPointCasts<float, ::std::u32string> {};
class SinglePrecisionFloatingPointCastsS : public FloatingPointCasts<float, ::std::string> {};
class SinglePrecisionFloatingPointCastsU8 : public FloatingPointCasts<float, ::std::u8string> {};
class SinglePrecisionFloatingPointCastsU16 : public FloatingPointCasts<float, ::std::u16string> {};
class SinglePrecisionFloatingPointCastsW : public FloatingPointCasts<float, ::std::wstring> {};

template<>
::std::string testing::PrintToString(const ::SinglePrecisionFloatingPointCastsU32::elem_type& Elem) {
    return ::SinglePrecisionFloatingPointCastsU32::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SinglePrecisionFloatingPointCastsS::elem_type& Elem) {
    return ::SinglePrecisionFloatingPointCastsS::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SinglePrecisionFloatingPointCastsU8::elem_type& Elem) {
    return ::SinglePrecisionFloatingPointCastsU8::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SinglePrecisionFloatingPointCastsU16::elem_type& Elem) {
    return ::SinglePrecisionFloatingPointCastsU16::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::SinglePrecisionFloatingPointCastsW::elem_type& Elem) {
    return ::SinglePrecisionFloatingPointCastsW::to_string(Elem);
};

TEST_P(SinglePrecisionFloatingPointCastsU32, ForwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SinglePrecisionFloatingPointCastsU32, BackwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SinglePrecisionFloatingPointCastsU32,
    ::testing::Values(
        SinglePrecisionFloatingPointCastsU32::make(U"3.14159", 3.14159f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::make(U"2.71828", 2.71828f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::make(U"6.62607e-34", 6.62607e-34f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::make(U"-6.02214e+23", -6.02214e23f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::elem_type{ U"0.0", 0.0f, ::correct::yes, U"0"}
        , SinglePrecisionFloatingPointCastsU32::make(U"1.012", 1.012f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::make(U"10.423", 10.423f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::make(U"-8.8", -8.8f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::elem_type{ U"789.0e-12", 789.0e-12f, ::correct::yes, U"7.89e-10" }
        , SinglePrecisionFloatingPointCastsU32::make(U"-10.01234", -10.01234f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::make(U"0.7", 0.7f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU32::elem_type{ U"452.0", 452.0f, ::correct::yes, U"452" }
        , SinglePrecisionFloatingPointCastsU32::elem_type{ U"-1744.0", -1744.0f, ::correct::yes, U"-1744" }
        , SinglePrecisionFloatingPointCastsU32::elem_type{ U"12.0", 12.0f, ::correct::yes, U"12" }
    )
);

TEST_P(SinglePrecisionFloatingPointCastsS, ForwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SinglePrecisionFloatingPointCastsS, BackwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SinglePrecisionFloatingPointCastsS,
    ::testing::Values(
        SinglePrecisionFloatingPointCastsS::make("3.14159", 3.14159f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::make("2.71828", 2.71828f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::make("6.62607e-34", 6.62607e-34f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::make("-6.02214e+23", -6.02214e23f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::elem_type{ "0.0", 0.0f, ::correct::yes, "0" }
        , SinglePrecisionFloatingPointCastsS::make("1.012", 1.012f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::make("10.423", 10.423f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::make("-8.8", -8.8f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::elem_type{ "789.0e-12", 789.0e-12f, ::correct::yes, "7.89e-10" }
        , SinglePrecisionFloatingPointCastsS::make("-10.01234", -10.01234f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::make("0.7", 0.7f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsS::elem_type{ "452.0", 452.0f, ::correct::yes, "452" }
        , SinglePrecisionFloatingPointCastsS::elem_type{ "-1744.0", -1744.0f, ::correct::yes, "-1744" }
        , SinglePrecisionFloatingPointCastsS::elem_type{ "12.0", 12.0f, ::correct::yes, "12" }
    )
);

TEST_P(SinglePrecisionFloatingPointCastsU8, ForwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                    )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                    )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SinglePrecisionFloatingPointCastsU8, BackwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SinglePrecisionFloatingPointCastsU8,
    ::testing::Values(
        SinglePrecisionFloatingPointCastsU8::make(u8"3.14159", 3.14159f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::make(u8"2.71828", 2.71828f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::make(u8"6.62607e-34", 6.62607e-34f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::make(u8"-6.02214e+23", -6.02214e23f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::elem_type{ u8"0.0", 0.0f, ::correct::yes, u8"0" }
        , SinglePrecisionFloatingPointCastsU8::make(u8"1.012", 1.012f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::make(u8"10.423", 10.423f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::make(u8"-8.8", -8.8f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::elem_type{ u8"789.0e-12", 789.0e-12f, ::correct::yes, u8"7.89e-10" }
        , SinglePrecisionFloatingPointCastsU8::make(u8"-10.01234", -10.01234f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::make(u8"0.7", 0.7f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU8::elem_type{ u8"452.0", 452.0f, ::correct::yes, u8"452" }
        , SinglePrecisionFloatingPointCastsU8::elem_type{ u8"-1744.0", -1744.0f, ::correct::yes, u8"-1744" }
        , SinglePrecisionFloatingPointCastsU8::elem_type{ u8"12.0", 12.0f, ::correct::yes, u8"12" }
    )
);

TEST_P(SinglePrecisionFloatingPointCastsU16, ForwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SinglePrecisionFloatingPointCastsU16, BackwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SinglePrecisionFloatingPointCastsU16,
    ::testing::Values(
        SinglePrecisionFloatingPointCastsU16::make(u"3.14159", 3.14159f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::make(u"2.71828", 2.71828f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::make(u"6.62607e-34", 6.62607e-34f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::make(u"-6.02214e+23", -6.02214e23f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::elem_type{ u"0.0", 0.0f, ::correct::yes, u"0" }
        , SinglePrecisionFloatingPointCastsU16::make(u"1.012", 1.012f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::make(u"10.423", 10.423f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::make(u"-8.8", -8.8f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::elem_type{ u"789.0e-12", 789.0e-12f, ::correct::yes, u"7.89e-10" }
        , SinglePrecisionFloatingPointCastsU16::make(u"-10.01234", -10.01234f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::make(u"0.7", 0.7f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsU16::elem_type{ u"452.0", 452.0f, ::correct::yes, u"452" }
        , SinglePrecisionFloatingPointCastsU16::elem_type{ u"-1744.0", -1744.0f, ::correct::yes, u"-1744" }
        , SinglePrecisionFloatingPointCastsU16::elem_type{ u"12.0", 12.0f, ::correct::yes, u"12" }
    )
);

TEST_P(SinglePrecisionFloatingPointCastsW, ForwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(SinglePrecisionFloatingPointCastsW, BackwardCastTest) {
    using TestCaseFixture = SinglePrecisionFloatingPointCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, SinglePrecisionFloatingPointCastsW,
    ::testing::Values(
        SinglePrecisionFloatingPointCastsW::make(L"3.14159", 3.14159f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::make(L"2.71828", 2.71828f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::make(L"6.62607e-34", 6.62607e-34f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::make(L"-6.02214e+23", -6.02214e23f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::elem_type{ L"0.0", 0.0f, ::correct::yes, L"0" }
        , SinglePrecisionFloatingPointCastsW::make(L"1.012", 1.012f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::make(L"10.423", 10.423f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::make(L"-8.8", -8.8f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::elem_type{ L"789.0e-12", 789.0e-12f, ::correct::yes, L"7.89e-10" }
        , SinglePrecisionFloatingPointCastsW::make(L"-10.01234", -10.01234f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::make(L"0.7", 0.7f, ::correct::yes)
        , SinglePrecisionFloatingPointCastsW::elem_type{ L"452.0", 452.0f, ::correct::yes, L"452" }
        , SinglePrecisionFloatingPointCastsW::elem_type{ L"-1744.0", -1744.0f, ::correct::yes, L"-1744" }
        , SinglePrecisionFloatingPointCastsW::elem_type{ L"12.0", 12.0f, ::correct::yes, L"12" }
    )
);

class DoublePrecisionFloatingPointCastsU32 : public FloatingPointCasts<long double, ::std::u32string> {};
class DoublePrecisionFloatingPointCastsS : public FloatingPointCasts<long double, ::std::string> {};
class DoublePrecisionFloatingPointCastsU8 : public FloatingPointCasts<long double, ::std::u8string> {};
class DoublePrecisionFloatingPointCastsU16 : public FloatingPointCasts<long double, ::std::u16string> {};
class DoublePrecisionFloatingPointCastsW : public FloatingPointCasts<long double, ::std::wstring> {};

template<>
::std::string testing::PrintToString(const ::DoublePrecisionFloatingPointCastsU32::elem_type& Elem) {
    return ::DoublePrecisionFloatingPointCastsU32::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::DoublePrecisionFloatingPointCastsS::elem_type& Elem) {
    return ::DoublePrecisionFloatingPointCastsS::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::DoublePrecisionFloatingPointCastsU8::elem_type& Elem) {
    return ::DoublePrecisionFloatingPointCastsU8::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::DoublePrecisionFloatingPointCastsU16::elem_type& Elem) {
    return ::DoublePrecisionFloatingPointCastsU16::to_string(Elem);
};
template<>
::std::string testing::PrintToString(const ::DoublePrecisionFloatingPointCastsW::elem_type& Elem) {
    return ::DoublePrecisionFloatingPointCastsW::to_string(Elem);
};

TEST_P(DoublePrecisionFloatingPointCastsU32, ForwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(DoublePrecisionFloatingPointCastsU32, BackwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsU32;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, DoublePrecisionFloatingPointCastsU32,
    ::testing::Values(
        DoublePrecisionFloatingPointCastsU32::make(U"3.14159", 3.14159, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::make(U"2.71828", 2.71828, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::make(U"6.62607e-34", 6.62607e-34, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::make(U"-6.02214e+23", -6.02214e23, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"0.0", 0.0, ::correct::yes, U"0" }
        , DoublePrecisionFloatingPointCastsU32::make(U"1.012", 1.012, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::make(U"10.423", 10.423, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::make(U"-8.8", -8.8, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"789.0e-12", 789.0e-12, ::correct::yes, U"7.89e-10" }
        , DoublePrecisionFloatingPointCastsU32::make(U"-10.01234", -10.01234, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::make(U"0.7", 0.7, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"452.0", 452.0, ::correct::yes, U"452" }
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"-1744.0", -1744.0, ::correct::yes, U"-1744" }
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"12.0", 12.0, ::correct::yes, U"12" }
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"611540.626075e-304", 611540.626075e-304, ::correct::yes, U"6.11540626075e-299"}
        , DoublePrecisionFloatingPointCastsU32::elem_type{ U"-0000.02214e+23", -0.02214e23, ::correct::yes, U"-2.214e+21" }
    )
);

TEST_P(DoublePrecisionFloatingPointCastsS, ForwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(DoublePrecisionFloatingPointCastsS, BackwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsS;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, DoublePrecisionFloatingPointCastsS,
    ::testing::Values(
        DoublePrecisionFloatingPointCastsS::make("3.14159", 3.14159, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::make("2.71828", 2.71828, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::make("6.62607e-34", 6.62607e-34, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::make("-6.02214e+23", -6.02214e23, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::elem_type{ "0.0", 0.0, ::correct::yes, "0" }
        , DoublePrecisionFloatingPointCastsS::make("1.012", 1.012, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::make("10.423", 10.423, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::make("-8.8", -8.8, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::elem_type{ "789.0e-12", 789.0e-12, ::correct::yes, "7.89e-10" }
        , DoublePrecisionFloatingPointCastsS::make("-10.01234", -10.01234, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::make("0.7", 0.7, ::correct::yes)
        , DoublePrecisionFloatingPointCastsS::elem_type{ "452.0", 452.0, ::correct::yes, "452" }
        , DoublePrecisionFloatingPointCastsS::elem_type{ "-1744.0", -1744.0, ::correct::yes, "-1744" }
        , DoublePrecisionFloatingPointCastsS::elem_type{ "12.0", 12.0, ::correct::yes, "12" }
        , DoublePrecisionFloatingPointCastsS::elem_type{ "611540.626075e-304", 611540.626075e-304, ::correct::yes, "6.11540626075e-299" }
        , DoublePrecisionFloatingPointCastsS::elem_type{ "-0000.02214e+23", -0.02214e23, ::correct::yes, "-2.214e+21" }
    )
);

TEST_P(DoublePrecisionFloatingPointCastsU8, ForwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(DoublePrecisionFloatingPointCastsU8, BackwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsU8;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, DoublePrecisionFloatingPointCastsU8,
    ::testing::Values(
        DoublePrecisionFloatingPointCastsU8::make(u8"3.14159", 3.14159, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::make(u8"2.71828", 2.71828, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::make(u8"6.62607e-34", 6.62607e-34, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::make(u8"-6.02214e+23", -6.02214e23, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"0.0", 0.0, ::correct::yes, u8"0" }
        , DoublePrecisionFloatingPointCastsU8::make(u8"1.012", 1.012, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::make(u8"10.423", 10.423, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::make(u8"-8.8", -8.8, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"789.0e-12", 789.0e-12, ::correct::yes, u8"7.89e-10" }
        , DoublePrecisionFloatingPointCastsU8::make(u8"-10.01234", -10.01234, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::make(u8"0.7", 0.7, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"452.0", 452.0, ::correct::yes, u8"452" }
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"-1744.0", -1744.0, ::correct::yes, u8"-1744" }
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"12.0", 12.0, ::correct::yes, u8"12" }
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"611540.626075e-304", 611540.626075e-304, ::correct::yes, u8"6.11540626075e-299" }
        , DoublePrecisionFloatingPointCastsU8::elem_type{ u8"-0000.02214e+23", -0.02214e23, ::correct::yes, u8"-2.214e+21" }
    )
);

TEST_P(DoublePrecisionFloatingPointCastsU16, ForwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(DoublePrecisionFloatingPointCastsU16, BackwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsU16;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, DoublePrecisionFloatingPointCastsU16,
    ::testing::Values(
        DoublePrecisionFloatingPointCastsU16::make(u"3.14159", 3.14159, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::make(u"2.71828", 2.71828, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::make(u"6.62607e-34", 6.62607e-34, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::make(u"-6.02214e+23", -6.02214e23, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"0.0", 0.0, ::correct::yes, u"0" }
        , DoublePrecisionFloatingPointCastsU16::make(u"1.012", 1.012, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::make(u"10.423", 10.423, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::make(u"-8.8", -8.8, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"789.0e-12", 789.0e-12, ::correct::yes, u"7.89e-10" }
        , DoublePrecisionFloatingPointCastsU16::make(u"-10.01234", -10.01234, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::make(u"0.7", 0.7, ::correct::yes)
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"452.0", 452.0, ::correct::yes, u"452" }
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"-1744.0", -1744.0, ::correct::yes, u"-1744" }
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"12.0", 12.0, ::correct::yes, u"12" }
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"611540.626075e-304", 611540.626075e-304, ::correct::yes, u"6.11540626075e-299" }
        , DoublePrecisionFloatingPointCastsU16::elem_type{ u"-0000.02214e+23", -0.02214e23, ::correct::yes, u"-2.214e+21" }
    )
);

TEST_P(DoublePrecisionFloatingPointCastsW, ForwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            const TestCaseFixture::string_type string_variants[] = {
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::std::get<TestCaseFixture::string_equ>(GetParam())
            };

            ASSERT_TRUE(
                (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[0]
                )
                || (
                    ::uns::string::cast<TestCaseFixture::string_type>(
                        ::std::get<TestCaseFixture::expectation>(GetParam())
                    )
                    == string_variants[1]
                )
            );

            break;
        }
        case ::correct::no: {
            ASSERT_NE(
                ::std::get<TestCaseFixture::string>(GetParam())
                , ::uns::string::cast<TestCaseFixture::string_type>(
                    ::std::get<TestCaseFixture::expectation>(GetParam())
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
TEST_P(DoublePrecisionFloatingPointCastsW, BackwardCastTest) {
    using TestCaseFixture = DoublePrecisionFloatingPointCastsW;

    auto correctness = ::std::get<TestCaseFixture::correctness>(GetParam());
    switch (correctness) {
        case ::correct::yes: {
            ASSERT_DOUBLE_EQ(
                ::std::get<TestCaseFixture::expectation>(GetParam())
                , ::uns::string::cast<TestCaseFixture::numeric_type>(
                    ::std::get<TestCaseFixture::string>(GetParam())
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
                    ::std::get<TestCaseFixture::expectation>(GetParam())
                    == ::uns::string::cast<TestCaseFixture::numeric_type>(
                        ::std::get<TestCaseFixture::string>(GetParam())
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
INSTANTIATE_TEST_CASE_P(NumericTests, DoublePrecisionFloatingPointCastsW,
    ::testing::Values(
        DoublePrecisionFloatingPointCastsW::make(L"3.14159", 3.14159, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::make(L"2.71828", 2.71828, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::make(L"6.62607e-34", 6.62607e-34, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::make(L"-6.02214e+23", -6.02214e23, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"0.0", 0.0, ::correct::yes, L"0" }
        , DoublePrecisionFloatingPointCastsW::make(L"1.012", 1.012, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::make(L"10.423", 10.423, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::make(L"-8.8", -8.8, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"789.0e-12", 789.0e-12, ::correct::yes, L"7.89e-10" }
        , DoublePrecisionFloatingPointCastsW::make(L"-10.01234", -10.01234, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::make(L"0.7", 0.7, ::correct::yes)
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"452.0", 452.0, ::correct::yes, L"452" }
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"-1744.0", -1744.0, ::correct::yes, L"-1744" }
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"12.0", 12.0, ::correct::yes, L"12" }
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"611540.626075e-304", 611540.626075e-304, ::correct::yes, L"6.11540626075e-299" }
        , DoublePrecisionFloatingPointCastsW::elem_type{ L"-0000.02214e+23", -0.02214e23, ::correct::yes, L"-2.214e+21" }
    )
);

/*
class ReadU8 : public ::SeekU8 {};

TEST_P(ReadU8, DelimitersExplicitLimiters) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    const auto limiter_beg = ::ReadU8::target.cbegin() + (
        ::std::get<::ReadU8::elem_part::shift2>(GetParam())
        <= ::std::get<::ReadU8::elem_part::shift3>(GetParam())
        ? ::std::get<::ReadU8::elem_part::shift2>(GetParam())
        : ::std::get<::ReadU8::elem_part::shift3>(GetParam())
    );
    const auto limiter_end = ::ReadU8::target.cbegin() + (
        ::std::get<::ReadU8::elem_part::shift2>(GetParam())
        <= ::std::get<::ReadU8::elem_part::shift3>(GetParam())
        ? ::std::get<::ReadU8::elem_part::shift3>(GetParam())
        : ::std::get<::ReadU8::elem_part::shift2>(GetParam())
    );
    const auto delimiters = ::std::vector<::ReadU8::string_type>{
        ::ReadU8::delimiter
        , ::ReadU8::finish
    };

    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiters);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters)
        && (pos_of_delimiters <= limiter_beg)
        && (pos_of_delimiters < limiter_end);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiters
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
        << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    };
};
TEST_P(ReadU8, DelimiterExplicitLimiters) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    const auto limiter_beg = ::ReadU8::target.cbegin() + (
        ::std::get<::ReadU8::elem_part::shift2>(GetParam())
        <= ::std::get<::ReadU8::elem_part::shift3>(GetParam())
        ? ::std::get<::ReadU8::elem_part::shift2>(GetParam())
        : ::std::get<::ReadU8::elem_part::shift3>(GetParam())
    );
    const auto limiter_end = ::ReadU8::target.cbegin() + (
        ::std::get<::ReadU8::elem_part::shift2>(GetParam())
        <= ::std::get<::ReadU8::elem_part::shift3>(GetParam())
        ? ::std::get<::ReadU8::elem_part::shift3>(GetParam())
        : ::std::get<::ReadU8::elem_part::shift2>(GetParam())
    );
    const auto delimiter = ::ReadU8::delimiter;

    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiter);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters)
        && (pos_of_delimiters <= limiter_beg)
        && (pos_of_delimiters < limiter_end);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiter
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
        << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    };
};
TEST_P(ReadU8, DelimitersNoLimiters) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    const auto delimiters = ::std::vector<::ReadU8::string_type>{
        ::ReadU8::delimiter
        , ::ReadU8::finish
    };

    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiters);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiters
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin());
    };
};
TEST_P(ReadU8, DelimiterNoLimiters) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    const auto delimiters = ::ReadU8::delimiter;

    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiter);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiter
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin());
    };
};
TEST_P(ReadU8, DelimitersLimiters) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    const auto limiters = ::std::vector<::std::u8string>{
        u8" x"
        , u8"xv"
        , u8"vx"
    };
    const auto limiter_beg = ::uns::string::parsing::find(target, seeker, limiters);
    const auto limiter_end = target.cend();
    ASSERT_LT(limiter_beg, limiter_end);

    const auto delimiters = ::std::vector<::ReadU8::string_type>{
        ::ReadU8::delimiter
        , ::ReadU8::finish
    };
    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiters);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters)
        && (pos_of_delimiters <= limiter_beg)
        && (pos_of_delimiters < limiter_end);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiters
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
            , limiters
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
        << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    };
};
TEST_P(ReadU8, DelimiterLimiters) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    const auto limiters = ::std::vector<::std::u8string>{
        u8" x"
        , u8"xv"
        , u8"vx"
    };
    const auto limiter_beg = ::uns::string::parsing::find(target, seeker, limiters);
    const auto limiter_end = target.cend();
    ASSERT_LT(limiter_beg, limiter_end);

    const auto delimiters = ::ReadU8::delimiter;
    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiters);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters)
        && (pos_of_delimiters <= limiter_beg)
        && (pos_of_delimiters < limiter_end);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiter
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
            , limiters
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
        << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(::std::u8string{ limiter_beg, limiter_end });
    };
};
TEST_P(ReadU8, DelimitersLimiter) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    auto limiter = ::std::u8string{
        target.cbegin() + (
            ::std::get<::SeekU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekU8::elem_part::shift2>(GetParam())
            : ::std::get<::SeekU8::elem_part::shift3>(GetParam())
        )
        , target.cbegin() + (
            ::std::get<::SeekU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekU8::elem_part::shift3>(GetParam())
            : ::std::get<::SeekU8::elem_part::shift2>(GetParam())
        )
    };
    const auto limiter_beg = ::uns::string::parsing::find(target, seeker, limiter);
    const auto limiter_end = (
        limiter_beg != target.cend()
        ? limiter_beg + limiter.size()
        : target.cend()
    );
    if (
        limiter_beg == limiter_end
        && limiter_end == target.cend()
    ) {
        limiter.clear();
    };

    const auto delimiters = ::std::vector<::ReadU8::string_type>{
        ::ReadU8::delimiter
        , ::ReadU8::finish
    };
    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiters);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters)
        && (pos_of_delimiters <= limiter_beg)
        && (pos_of_delimiters < limiter_end);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiters
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
        << "; limiter=" << ::testing::PrintToString(limiter);

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(limiter);
    };
};
TEST_P(ReadU8, DelimiterLimiter) {
    const auto value = ::std::get<::ReadU8::elem_part::sample>(GetParam());
    auto seeker = ::ReadU8::target.cbegin() + ::std::get<::ReadU8::elem_part::shift1>(GetParam());
    auto limiter = ::std::u8string{
        target.cbegin() + (
            ::std::get<::SeekU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekU8::elem_part::shift2>(GetParam())
            : ::std::get<::SeekU8::elem_part::shift3>(GetParam())
        )
        , target.cbegin() + (
            ::std::get<::SeekU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekU8::elem_part::shift3>(GetParam())
            : ::std::get<::SeekU8::elem_part::shift2>(GetParam())
        )
    };
    const auto limiter_beg = ::uns::string::parsing::find(target, seeker, limiter);
    const auto limiter_end = (
        limiter_beg != target.cend()
        ? limiter_beg + limiter.size()
        : target.cend()
    );
    if (
        limiter_beg == limiter_end
        && limiter_end == target.cend()
    ) {
        limiter.clear();
    };

    const auto delimiter = ::ReadU8::delimiter;
    const auto pos_of_delimiters = ::uns::string::parsing::find(::ReadU8::target, seeker, delimiter);

    const auto seeking_result =
        pos_of_delimiters != ::ReadU8::target.cend()
        && (seeker < pos_of_delimiters)
        && (pos_of_delimiters <= limiter_beg)
        && (pos_of_delimiters < limiter_end);

    ::ReadU8::string_type fragment = u8"";
    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parsing::read<::ReadU8::string_type>(
            ::ReadU8::target
            , seeker
            , fragment
            , delimiter
            , {
                .qualifier = ::uns::string::parsing::seeker_position::from_end
                , .offset = 1
            }
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
        << "; value=" << ::testing::PrintToString(value)
        << "; value.size()=" << value.size()
        << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
        << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
        << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
        << "; limiter=" << ::testing::PrintToString(limiter);

    if (seeking_result) {
        ASSERT_EQ(
            fragment
            , value
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(::ReadU8::target) << "\"\n"
            << "; value=" << ::testing::PrintToString(value)
            << "; value.size()=" << value.size()
            << "; pos_of_delimiters=" << (pos_of_delimiters - ::ReadU8::target.cbegin())
            << "seeker_before_seeking=" << (seeker_before_seeking - ::ReadU8::target.cbegin())
            << "; seeker=" << (seeker - ::ReadU8::target.cbegin())
            << "; limiter=" << ::testing::PrintToString(limiter);
    };
};

INSTANTIATE_TEST_CASE_P(ParsingMethodsTests_Val0, ReadU8,
    ::testing::Combine(
        ::testing::Values(::ReadU8::values[0])
        , ::testing::Values(::ReadU8::start.size() + ::ReadU8::samples[0].size() + ::ReadU8::equality.size())
        , ::testing::Range<
            ::ReadU8::shift2_type
            , ::ReadU8::shift2_type
        >(0, ::ReadU8::target.size(), 5)
        , ::testing::Range<
            ::ReadU8::shift3_type
            , ::ReadU8::shift3_type
        >(0, ::ReadU8::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(ParsingMethodsTests_Val1, ReadU8,
    ::testing::Combine(
        ::testing::Values(::ReadU8::values[1])
        , ::testing::Values(
            ::ReadU8::start.size()
            + ::ReadU8::samples[0].size() + ::ReadU8::equality.size() + ::ReadU8::values[0].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[1].size() + ::ReadU8::equality.size()
        )
        , ::testing::Range<
            ::ReadU8::shift2_type
            , ::ReadU8::shift2_type
        >(0, ::ReadU8::target.size(), 5)
        , ::testing::Range<
            ::ReadU8::shift3_type
            , ::ReadU8::shift3_type
        >(0, ::ReadU8::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(ParsingMethodsTests_Val2, ReadU8,
    ::testing::Combine(
        ::testing::Values(::ReadU8::values[2])
        , ::testing::Values(
            ::ReadU8::start.size()
            + ::ReadU8::samples[0].size() + ::ReadU8::equality.size() + ::ReadU8::values[0].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[1].size() + ::ReadU8::equality.size() + ::ReadU8::values[1].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[2].size() + ::ReadU8::equality.size()
        )
        , ::testing::Range<
            ::ReadU8::shift2_type
            , ::ReadU8::shift2_type
        >(0, ::ReadU8::target.size(), 5)
        , ::testing::Range<
            ::ReadU8::shift3_type
            , ::ReadU8::shift3_type
        >(0, ::ReadU8::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(ParsingMethodsTests_ValNone, ReadU8,
    ::testing::Combine(
        ::testing::Values(::ReadU8::string_type{})
        , ::testing::Values(
            ::ReadU8::start.size()
            + ::ReadU8::samples[0].size() + ::ReadU8::equality.size() + ::ReadU8::values[0].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[1].size() + ::ReadU8::equality.size() + ::ReadU8::values[1].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[2].size() + ::ReadU8::equality.size() + ::ReadU8::values[2].size()
            , ::ReadU8::start.size()
            + ::ReadU8::samples[0].size() + ::ReadU8::equality.size() + ::ReadU8::values[0].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[1].size() + ::ReadU8::equality.size() + ::ReadU8::values[1].size() + ::ReadU8::delimiter.size()
            + ::ReadU8::samples[2].size() + ::ReadU8::equality.size() + ::ReadU8::values[2].size() + ::ReadU8::delimiter.size()
        )
        , ::testing::Range<
            ::ReadU8::shift2_type
            , ::ReadU8::shift2_type
        >(0, ::ReadU8::target.size(), 5)
        , ::testing::Range<
            ::ReadU8::shift3_type
            , ::ReadU8::shift3_type
        >(0, ::ReadU8::target.size(), 5)
    )
);
*/


template<::uns::is_basic_string string_t>
class Parsing {
public:
    using string_type = string_t;
    using samples_collection_type = ::std::vector<string_type>;
public:
    inline static string_type start = ::uns::string::cast<string_type>(U"start:");
    inline static samples_collection_type keys = {
        //::uns::string::cast<string_type>(U"some_key1")
        ::uns::string::cast<string_type>(U"нiкий_ключ2")
        //, ::uns::string::cast<string_type>(U"ani_clue3")
        , ::uns::string::cast<string_type>(U"нек\u53A3е_\u4FB1\u50244")
        , ::uns::string::cast<string_type>(U"\u4F51\u3086\u304Anu_clue5")
    };
    inline static samples_collection_type values = {
        //::uns::string::cast<string_type>(U"нек\u53E9е_зна4ение1")
        ::uns::string::cast<string_type>(U"\u4F65\u3099\u305Bnо\u4FA2\u50382")
        //, ::uns::string::cast<string_type>(U"ani_mean3")
        , ::uns::string::cast<string_type>(U"\u4FD3\u1089\u354B\u2FA1\u5043")
        , ::uns::string::cast<string_type>(U"\u4FF7\u355AЫ\u5051\u4FC5\u356Cж\u5066")
    };
    inline static string_type equality = ::uns::string::cast<string_type>(U"=");
    inline static samples_collection_type delimiters = {
        //::uns::string::cast<string_type>(U";")
        ::uns::string::cast<string_type>(U"; ")
        //, ::uns::string::cast<string_type>(U"?")
        , ::uns::string::cast<string_type>(U"&&&")
        , ::uns::string::cast<string_type>(U".")
    };
    inline static string_type finish = ::uns::string::cast<string_type>(U"xvx");
    inline static string_type target = start
        + keys[0] + equality + values[0] + delimiters[0]
        + keys[1] + equality + values[1] + delimiters[1]
        + keys[2] + equality + values[2] + delimiters[2]
        //+ keys[3] + equality + values[3] + delimiters[3]
        //+ keys[4] + equality + values[4] + delimiters[4]
        + finish;
public:
    static int seeker_to_num(const typename string_type::const_iterator& Seeker) {
        return Seeker - target.cbegin();
    };
    static int key_iter_to_num(const typename decltype(keys)::const_iterator& KeyIter) {
        return KeyIter - keys.cbegin();
    };
    static int value_iter_to_num(const typename decltype(values)::const_iterator& ValueIter) {
        return ValueIter - values.cbegin();
    };
public:
    static string_type get_target() { return target; };
};

namespace param_set {

    template<::uns::is_basic_string string_t>
    struct ParsingFind {
        ::std::size_t seeker_init;
        ::std::size_t seeker_result;
        string_t sample;
        ::std::size_t sample_iter;
    public:
        ::std::string to_string() const {
            return "{ seeker_init=" + ::testing::PrintToString(seeker_init)
                + ", seeker_result=" + ::testing::PrintToString(seeker_result)
                + ", sample=" + ::testing::PrintToString(sample)
                + ", sample_iter=" + ::testing::PrintToString(sample_iter) + " }";
        };
    };

};


template<::uns::is_basic_string string_t>
class ParsingFind :
    public ::Parsing<string_t>
    , public ::testing::TestWithParam<::param_set::ParsingFind<string_t>>
{
private:
    using base = ::Parsing<string_t>;
    using test = ::testing::TestWithParam<::param_set::ParsingFind<string_t>>;
public:
    using string_type = string_t;
public:
    typename base::string_type::const_iterator get_seeker_init() {
        return base::target.cbegin() + test::GetParam().seeker_init;
    };
    typename base::string_type::const_iterator get_seeker_result() {
        return base::target.cbegin() + test::GetParam().seeker_result;
    };
    typename decltype(base::keys)::const_iterator default_sample_iter() {
        return base::keys.cend();
    };
};


template<::uns::is_basic_string string_t>
class ParsingFind_Samples : public ::ParsingFind<string_t> {
private:
    using base = ::ParsingFind<string_t>;
public:
    typename decltype(base::keys)::const_iterator expected_sample_iter() {
        return base::keys.cbegin() + base::GetParam().sample_iter;
    };
public:
    static ::std::vector<::param_set::ParsingFind<string_t>> generate_tests() {
        using single_test = ::param_set::ParsingFind<string_t>;

        auto result = ::std::vector<::param_set::ParsingFind<string_t>>{};

        ::std::size_t seeker = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            for (; seeker <= precursor && seeker < base::target.size(); ++seeker) {
                result.emplace_back(
                    single_test{
                        .seeker_init = seeker
                        , .seeker_result = precursor
                        , .sample = base::keys[idx]
                        , .sample_iter = idx
                    }
                );
            };
            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };
        for (; seeker < base::target.size(); ++seeker) {
            result.emplace_back(
                single_test{
                    .seeker_init = seeker
                    , .seeker_result = base::target.size()
                    , .sample = ::uns::string::cast<string_t>("fake_key")
                    , .sample_iter = base::keys.size()
                }
            );
        };

        return result;
    };
};

using ParsingFind_SamplesU32 = ::ParsingFind_Samples<::std::u32string>;
template<>
::std::string testing::PrintToString(const ::param_set::ParsingFind<::std::u32string>& Params) {
    return Params.to_string();
};

TEST_P(ParsingFind_SamplesU32, Do) {
    auto found_sample = default_sample_iter();
    const auto seeker = get_seeker_init();

    const auto result = ::uns::string::parsing::find(
        target
        , seeker
        , keys
        , found_sample
    );

    ASSERT_EQ(
        seeker_to_num(result)
        , seeker_to_num(get_seeker_result())
    ) << ::testing::PrintToString(get_target());

    ASSERT_EQ(
        key_iter_to_num(found_sample)
        , key_iter_to_num(expected_sample_iter())
    ) << ::testing::PrintToString(get_target());
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingFind_SamplesU32,
    ::testing::ValuesIn(
        ::ParsingFind_SamplesU32::generate_tests()
    )
);

using ParsingFind_SamplesU8 = ::ParsingFind_Samples<::std::u8string>;
template<>
::std::string testing::PrintToString(const ::param_set::ParsingFind<::std::u8string>& Params) {
    return Params.to_string();
};

TEST_P(ParsingFind_SamplesU8, Do) {
    auto found_sample = default_sample_iter();
    const auto seeker = get_seeker_init();

    const auto result = ::uns::string::parsing::find(
        target
        , seeker
        , keys
        , found_sample
    );

    ASSERT_EQ(
        seeker_to_num(result)
        , seeker_to_num(get_seeker_result())
    ) << ::testing::PrintToString(get_target());

    ASSERT_EQ(
        key_iter_to_num(found_sample)
        , key_iter_to_num(expected_sample_iter())
    ) << ::testing::PrintToString(get_target());
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingFind_SamplesU8,
    ::testing::ValuesIn(
        ::ParsingFind_SamplesU8::generate_tests()
    )
);

using ParsingFind_SamplesU16 = ::ParsingFind_Samples<::std::u16string>;
template<>
::std::string testing::PrintToString(const ::param_set::ParsingFind<::std::u16string>& Params) {
    return Params.to_string();
};

TEST_P(ParsingFind_SamplesU16, Do) {
    auto found_sample = default_sample_iter();
    const auto seeker = get_seeker_init();

    const auto result = ::uns::string::parsing::find(
        target
        , seeker
        , keys
        , found_sample
    );

    ASSERT_EQ(
        seeker_to_num(result)
        , seeker_to_num(get_seeker_result())
    ) << ::testing::PrintToString(get_target());

    ASSERT_EQ(
        key_iter_to_num(found_sample)
        , key_iter_to_num(expected_sample_iter())
    ) << ::testing::PrintToString(get_target());
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingFind_SamplesU16,
    ::testing::ValuesIn(
        ::ParsingFind_SamplesU16::generate_tests()
    )
);


template<::uns::is_basic_string string_t>
class ParsingFind_Sample : public ::ParsingFind<string_t> {
private:
    using base = ::ParsingFind<string_t>;
public:
    string_t expected_sample() {
        return base::GetParam().sample;
    };
public:
    static ::std::vector<::param_set::ParsingFind<string_t>> generate_tests() {
        using single_test = ::param_set::ParsingFind<string_t>;

        auto result = ::std::vector<::param_set::ParsingFind<string_t>>{};

        ::std::size_t seeker = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            seeker = 0;
            for (; seeker <= precursor && seeker < base::target.size(); ++seeker) {
                result.emplace_back(
                    single_test{
                        .seeker_init = seeker
                        , .seeker_result = precursor
                        , .sample = base::keys[idx]
                        , .sample_iter = idx
                    }
                );
            };
            for (; seeker < base::target.size(); ++seeker) {
                result.emplace_back(
                    single_test{
                        .seeker_init = seeker
                        , .seeker_result = base::target.size()
                        , .sample = base::keys[idx]
                        , .sample_iter = idx
                    }
                );
            };
            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };
        seeker = 0;
        for (; seeker < base::target.size(); ++seeker) {
            result.emplace_back(
                single_test{
                    .seeker_init = seeker
                    , .seeker_result = base::target.size()
                    , .sample = ::uns::string::cast<string_t>("fake_key")
                    , .sample_iter = base::keys.size()
                }
            );
        };

        return result;
    };
};

using ParsingFind_SampleU32 = ::ParsingFind_Sample<::std::u32string>;

TEST_P(ParsingFind_SampleU32, Do) {
    const auto seeker = get_seeker_init();

    const auto result = ::uns::string::parsing::find(
        target
        , seeker
        , expected_sample()
    );

    ASSERT_EQ(
        seeker_to_num(result)
        , seeker_to_num(get_seeker_result())
    ) << ::testing::PrintToString(get_target());
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingFind_SampleU32,
    ::testing::ValuesIn(
        ::ParsingFind_SampleU32::generate_tests()
    )
);

using ParsingFind_SampleU8 = ::ParsingFind_Sample<::std::u8string>;

TEST_P(ParsingFind_SampleU8, Do) {
    const auto seeker = get_seeker_init();

    const auto result = ::uns::string::parsing::find(
        target
        , seeker
        , expected_sample()
    );

    ASSERT_EQ(
        seeker_to_num(result)
        , seeker_to_num(get_seeker_result())
    ) << ::testing::PrintToString(get_target());
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingFind_SampleU8,
    ::testing::ValuesIn(
        ::ParsingFind_SampleU8::generate_tests()
    )
);

using ParsingFind_SampleU16 = ::ParsingFind_Sample<::std::u16string>;

TEST_P(ParsingFind_SampleU16, Do) {
    const auto seeker = get_seeker_init();

    const auto result = ::uns::string::parsing::find(
        target
        , seeker
        , expected_sample()
    );

    ASSERT_EQ(
        seeker_to_num(result)
        , seeker_to_num(get_seeker_result())
    ) << ::testing::PrintToString(get_target());
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingFind_SampleU16,
    ::testing::ValuesIn(
        ::ParsingFind_SampleU16::generate_tests()
    )
);


namespace param_set {

    struct ParsingSeek {
        ::std::size_t seeker_init = 0;
        ::std::size_t seeker_expected = 0;
        ::std::size_t sample_idx = 0;
        ::uns::string::parsing::seeker_position seeker_pos_in_sample;
        ::std::size_t limiter_begin = 0;
        ::std::size_t limiter_length = 0;
    private:
        static ::std::string to_string(const ::uns::string::parsing::seeker_position& SeekerPosInSample) {
            return "{ qualifier=" + ::std::string{
                    SeekerPosInSample.qualifier == ::uns::string::parsing::seeker_position::from_begin
                    ? "from_begin"
                    : "from_end"
                }
                + ", offset=" + ::testing::PrintToString(SeekerPosInSample.offset) + " }";
        };
    public:
        ::std::string to_string() const {
            return "{ seeker_init=" + ::testing::PrintToString(seeker_init)
                + ", seeker_expected=" + ::testing::PrintToString(seeker_expected)
                + ", sample_idx=" + ::testing::PrintToString(sample_idx)
                + ", seeker_pos_in_sample=" + to_string(seeker_pos_in_sample)
                + ", limiter_begin=" + ::testing::PrintToString(limiter_begin)
                + ", limiter_length=" + ::testing::PrintToString(limiter_length) + " }";
        };
    };

};

template<>
::std::string testing::PrintToString(const ::param_set::ParsingSeek& Params) {
    return Params.to_string();
};

template<::uns::is_basic_string string_t>
class ParsingSeek :
    public ::Parsing<string_t>
    , public ::testing::TestWithParam<::param_set::ParsingSeek>
{
private:
    using base = ::ParsingSeek<string_t>;
    using test = ::testing::TestWithParam<::param_set::ParsingSeek>;
public:
    using string_type = string_t;
public:
    typename string_type::const_iterator get_seeker_init() const {
        return base::target.cbegin() + GetParam().seeker_init;
    };
    typename string_type::const_iterator get_seeker_expected() const  {
        return base::target.cbegin() + GetParam().seeker_expected;
    };
    typename string_type get_sample() const {
        return *(base::keys.cbegin() + GetParam().sample_idx);
    };
    ::uns::string::parsing::seeker_position get_seeker_pos_in_sample() const  {
        return GetParam().seeker_pos_in_sample;
    };
    const ::std::tuple<
        string_type
        , typename string_type::const_iterator
        , typename string_type::const_iterator
    > get_limiter() const {
        auto limiter_beg = base::target.cend();
        auto limiter_end = base::target.cend();

        if (GetParam().limiter_begin < base::target.size()) {
            limiter_beg = base::target.cbegin() + GetParam().limiter_begin;
        };
        if (GetParam().limiter_begin + GetParam().limiter_length < base::target.size()) {
            limiter_end = base::target.cbegin() + GetParam().limiter_begin + GetParam().limiter_length;
        };

        return {
            string_type{ limiter_beg, limiter_end }
            , limiter_beg
            , limiter_end
        };
    };
};

template<::uns::is_basic_string string_t>
class ParsingSeek_SamplesExplicitLimiters : public ::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        single_test.limiter_begin = 0;
                        const ::std::size_t lengths[6] = {
                            0
                            , 3
                            , 4
                            , 5
                            , 10
                            , base::target.size() - single_test.limiter_begin
                        };
                        for (; single_test.limiter_begin < base::target.size(); single_test.limiter_begin += 5) {
                            single_test.limiter_length = 0;

                            for (auto length : lengths) {
                                if (single_test.limiter_begin + length <= base::target.size()) {
                                    single_test.limiter_length = length;

                                    result.push_back(single_test);
                                };
                            };
                        };
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SamplesExplicitLimiters_U32 = ::ParsingSeek_SamplesExplicitLimiters<::std::u32string>;
TEST_P(ParsingSeek_SamplesExplicitLimiters_U32, Do) {
    using fixture = ParsingSeek_SamplesExplicitLimiters_U32;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);
    ASSERT_NE(samples_iter, samples.cend());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (sample_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesExplicitLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesExplicitLimiters_U32::generate_tests()
    )
);
using ParsingSeek_SamplesExplicitLimiters_U16 = ::ParsingSeek_SamplesExplicitLimiters<::std::u16string>;
TEST_P(ParsingSeek_SamplesExplicitLimiters_U16, Do) {
    using fixture = ParsingSeek_SamplesExplicitLimiters_U16;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);
    ASSERT_NE(samples_iter, samples.cend());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (sample_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesExplicitLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesExplicitLimiters_U16::generate_tests()
    )
);
using ParsingSeek_SamplesExplicitLimiters_U8 = ::ParsingSeek_SamplesExplicitLimiters<::std::u8string>;
TEST_P(ParsingSeek_SamplesExplicitLimiters_U8, Do) {
    using fixture = ParsingSeek_SamplesExplicitLimiters_U8;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);
    ASSERT_NE(samples_iter, samples.cend());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (sample_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesExplicitLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesExplicitLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingSeek_SampleExplicitLimiters : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        single_test.limiter_begin = 0;
                        const ::std::size_t lengths[6] = {
                            0
                            , 3
                            , 4
                            , 5
                            , 10
                            , base::target.size() - single_test.limiter_begin
                        };
                        for (auto length : lengths) {
                            if (single_test.limiter_begin + length <= base::target.size()) {
                                single_test.limiter_length = length;

                                result.push_back(single_test);
                            };
                        };
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SampleExplicitLimiters_U32 = ::ParsingSeek_SampleExplicitLimiters<::std::u32string>;
TEST_P(ParsingSeek_SampleExplicitLimiters_U32, Do) {
    using fixture = ParsingSeek_SampleExplicitLimiters_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (sample_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleExplicitLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleExplicitLimiters_U32::generate_tests()
    )
);
using ParsingSeek_SampleExplicitLimiters_U16 = ::ParsingSeek_SampleExplicitLimiters<::std::u16string>;
TEST_P(ParsingSeek_SampleExplicitLimiters_U16, Do) {
    using fixture = ParsingSeek_SampleExplicitLimiters_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (sample_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleExplicitLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleExplicitLimiters_U16::generate_tests()
    )
);
using ParsingSeek_SampleExplicitLimiters_U8 = ::ParsingSeek_SampleExplicitLimiters<::std::u8string>;
TEST_P(ParsingSeek_SampleExplicitLimiters_U8, Do) {
    using fixture = ParsingSeek_SampleExplicitLimiters_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (sample_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleExplicitLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleExplicitLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingSeek_SamplesNoLimiters : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        single_test.limiter_begin = precursor + base::keys[idx].size() + base::equality.size() + base::values[idx].size();
                        single_test.limiter_length = base::delimiters[idx].size();

                        result.push_back(single_test);
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SamplesNoLimiters_U32 = ::ParsingSeek_SamplesNoLimiters<::std::u32string>;
TEST_P(ParsingSeek_SamplesNoLimiters_U32, Do) {
    using fixture = ParsingSeek_SamplesNoLimiters_U32;
    const auto& samples = keys;
    auto seeker = get_seeker_init();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples);

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesNoLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesNoLimiters_U32::generate_tests()
    )
);
using ParsingSeek_SamplesNoLimiters_U16 = ::ParsingSeek_SamplesNoLimiters<::std::u16string>;
TEST_P(ParsingSeek_SamplesNoLimiters_U16, Do) {
    using fixture = ParsingSeek_SamplesNoLimiters_U16;
    const auto& samples = keys;
    auto seeker = get_seeker_init();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples);

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesNoLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesNoLimiters_U16::generate_tests()
    )
);
using ParsingSeek_SamplesNoLimiters_U8 = ::ParsingSeek_SamplesNoLimiters<::std::u8string>;
TEST_P(ParsingSeek_SamplesNoLimiters_U8, Do) {
    using fixture = ParsingSeek_SamplesNoLimiters_U8;
    const auto& samples = keys;
    auto seeker = get_seeker_init();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples);

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesNoLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesNoLimiters_U8::generate_tests()
    )
);


template<::uns::is_basic_string string_t>
class ParsingSeek_SampleNoLimiters : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        single_test.limiter_begin = precursor + base::keys[idx].size() + base::equality.size() + base::values[idx].size();
                        single_test.limiter_length = base::delimiters[idx].size();

                        result.push_back(single_test);
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SampleNoLimiters_U32 = ::ParsingSeek_SampleNoLimiters<::std::u32string>;
TEST_P(ParsingSeek_SampleNoLimiters_U32, Do) {
    using fixture = ParsingSeek_SampleNoLimiters_U32;
    auto seeker = get_seeker_init();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleNoLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleNoLimiters_U32::generate_tests()
    )
);
using ParsingSeek_SampleNoLimiters_U16 = ::ParsingSeek_SampleNoLimiters<::std::u16string>;
TEST_P(ParsingSeek_SampleNoLimiters_U16, Do) {
    using fixture = ParsingSeek_SampleNoLimiters_U16;
    auto seeker = get_seeker_init();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleNoLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleNoLimiters_U16::generate_tests()
    )
);
using ParsingSeek_SampleNoLimiters_U8 = ::ParsingSeek_SampleNoLimiters<::std::u8string>;
TEST_P(ParsingSeek_SampleNoLimiters_U8, Do) {
    using fixture = ParsingSeek_SampleNoLimiters_U8;
    auto seeker = get_seeker_init();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleNoLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleNoLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingSeek_SamplesLimiters : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        if (idx > 0 && single_test.seeker_init <= precursor - base::delimiters[idx - 1].size()) {
                            single_test.limiter_begin = precursor - base::delimiters[idx - 1].size();
                            single_test.limiter_length = base::delimiters[idx - 1].size();
                        }
                        else {
                            single_test.limiter_begin = precursor + base::keys[idx].size() + base::equality.size() + base::values[idx].size();
                            single_test.limiter_length = base::delimiters[idx].size();
                        };

                        result.push_back(single_test);
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SamplesLimiters_U32 = ::ParsingSeek_SamplesLimiters<::std::u32string>;
TEST_P(ParsingSeek_SamplesLimiters_U32, Do) {
    using fixture = ParsingSeek_SamplesLimiters_U32;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);
    ASSERT_NE(samples_iter, samples.cend());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (get_seeker_expected() < limiter_end)
        && (sample_pos <= limiter_beg);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , delimiters
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesLimiters_U32::generate_tests()
    )
);
using ParsingSeek_SamplesLimiters_U16 = ::ParsingSeek_SamplesLimiters<::std::u16string>;
TEST_P(ParsingSeek_SamplesLimiters_U16, Do) {
    using fixture = ParsingSeek_SamplesLimiters_U16;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);
    ASSERT_NE(samples_iter, samples.cend());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (get_seeker_expected() < limiter_end)
        && (sample_pos <= limiter_beg);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , delimiters
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesLimiters_U16::generate_tests()
    )
);
using ParsingSeek_SamplesLimiters_U8 = ::ParsingSeek_SamplesLimiters<::std::u8string>;
TEST_P(ParsingSeek_SamplesLimiters_U8, Do) {
    using fixture = ParsingSeek_SamplesLimiters_U8;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);
    ASSERT_NE(samples_iter, samples.cend());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (get_seeker_expected() < limiter_end)
        && (sample_pos <= limiter_beg);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , delimiters
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingSeek_SampleLimiters : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        if (idx > 0 && single_test.seeker_init <= precursor - base::delimiters[idx - 1].size()) {
                            single_test.limiter_begin = precursor - base::delimiters[idx - 1].size();
                            single_test.limiter_length = base::delimiters[idx - 1].size();
                        }
                        else {
                            single_test.limiter_begin = precursor + base::keys[idx].size() + base::equality.size() + base::values[idx].size();
                            single_test.limiter_length = base::delimiters[idx].size();
                        };

                        result.push_back(single_test);
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SampleLimiters_U32 = ::ParsingSeek_SampleLimiters<::std::u32string>;
TEST_P(ParsingSeek_SampleLimiters_U32, Do) {
    using fixture = ParsingSeek_SamplesLimiters_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (get_seeker_expected() < limiter_end)
        && (sample_pos <= limiter_beg);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , delimiters
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleLimiters_U32::generate_tests()
    )
);
using ParsingSeek_SampleLimiters_U16 = ::ParsingSeek_SampleLimiters<::std::u16string>;
TEST_P(ParsingSeek_SampleLimiters_U16, Do) {
    using fixture = ParsingSeek_SampleLimiters_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (get_seeker_expected() < limiter_end)
        && (sample_pos <= limiter_beg);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , delimiters
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleLimiters_U16::generate_tests()
    )
);
using ParsingSeek_SampleLimiters_U8 = ::ParsingSeek_SampleLimiters<::std::u8string>;
TEST_P(ParsingSeek_SampleLimiters_U8, Do) {
    using fixture = ParsingSeek_SampleLimiters_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && (seeker <= sample_pos)
        && (get_seeker_expected() < limiter_end)
        && (sample_pos <= limiter_beg);

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , delimiters
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingSeek_SamplesLimiter : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        single_test.limiter_begin = 0;
                        for (; single_test.limiter_begin < base::target.size(); single_test.limiter_begin += 5) {
                            const ::std::size_t lengths[6] = {
                                0
                                , 3
                                , 4
                                , 5
                                , 10
                                , base::target.size() - single_test.limiter_begin
                            };
                            for (auto length : lengths) {
                                if (single_test.limiter_begin + length <= base::target.size()) {
                                    single_test.limiter_length = length;

                                    result.push_back(single_test);
                                };
                            };
                        };
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SamplesLimiter_U32 = ::ParsingSeek_SamplesLimiter<::std::u32string>;
TEST_P(ParsingSeek_SamplesLimiter_U32, Do) {
    using fixture = ParsingSeek_SamplesLimiter_U32;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);

    const auto expected_result =
        sample_pos != target.cend()
        && seeker <= sample_pos
        && (
            (
                limiter_beg < limiter_end
                && seeker <= limiter_beg
                && sample_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesLimiter_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesLimiter_U32::generate_tests()
    )
);
using ParsingSeek_SamplesLimiter_U16 = ::ParsingSeek_SamplesLimiter<::std::u16string>;
TEST_P(ParsingSeek_SamplesLimiter_U16, Do) {
    using fixture = ParsingSeek_SamplesLimiter_U16;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);

    const auto expected_result =
        sample_pos != target.cend()
        && seeker <= sample_pos
        && (
            (
                limiter_beg < limiter_end
                && seeker <= limiter_beg
                && sample_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesLimiter_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesLimiter_U16::generate_tests()
    )
);
using ParsingSeek_SamplesLimiter_U8 = ::ParsingSeek_SamplesLimiter<::std::u8string>;
TEST_P(ParsingSeek_SamplesLimiter_U8, Do) {
    using fixture = ParsingSeek_SamplesLimiter_U8;
    const auto& samples = keys;
    auto samples_iter = samples.cend();
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, samples, samples_iter);

    const auto expected_result =
        sample_pos != target.cend()
        && seeker <= sample_pos
        && (
            (
                limiter_beg < limiter_end
                && seeker <= limiter_beg
                && sample_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , samples
            , get_seeker_pos_in_sample()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SamplesLimiter_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SamplesLimiter_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingSeek_SampleLimiter : public::ParsingSeek<string_t> {
private:
    using base = ::ParsingSeek<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingSeek> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto single_test = ::param_set::ParsingSeek{};

        auto result = ::std::vector<::param_set::ParsingSeek>{};

        single_test.seeker_pos_in_sample.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_sample.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.sample_idx = idx;

            single_test.seeker_init = precursor - 5;
            for (; single_test.seeker_init <= precursor && single_test.seeker_init < base::target.size(); ++single_test.seeker_init) {
                for (auto seeker_result_position_variant : seeker_result_position_variants) {
                    single_test.seeker_pos_in_sample.qualifier = seeker_result_position_variant;
                    single_test.seeker_pos_in_sample.offset = -seeker_extremal_offset;

                    for (; single_test.seeker_pos_in_sample.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_sample.offset) {
                        single_test.seeker_expected = static_cast<::std::size_t>(
                            static_cast<int>(precursor)
                            + single_test.seeker_pos_in_sample.offset
                            );
                        if (single_test.seeker_pos_in_sample.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                            single_test.seeker_expected += base::keys[idx].size();
                        };

                        single_test.limiter_begin = 0;
                        for (; single_test.limiter_begin < base::target.size(); single_test.limiter_begin += 5) {
                            const ::std::size_t lengths[6] = {
                                0
                                , 3
                                , 4
                                , 5
                                , 10
                                , base::target.size() - single_test.limiter_begin
                            };
                            for (auto length : lengths) {
                                if (single_test.limiter_begin + length <= base::target.size()) {
                                    single_test.limiter_length = length;

                                    result.push_back(single_test);
                                };
                            };
                        };
                    };
                };
            };

            precursor += base::keys[idx].size() + base::equality.size() + base::values[idx].size() + base::delimiters[idx].size();
        };

        return result;
    };
};

using ParsingSeek_SampleLimiter_U32 = ::ParsingSeek_SampleLimiter<::std::u32string>;
TEST_P(ParsingSeek_SampleLimiter_U32, Do) {
    using fixture = ParsingSeek_SampleLimiter_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && seeker <= sample_pos
        && (
            (
                limiter_beg < limiter_end
                && seeker <= limiter_beg
                && sample_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleLimiter_U32,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleLimiter_U32::generate_tests()
    )
);
using ParsingSeek_SampleLimiter_U16 = ::ParsingSeek_SampleLimiter<::std::u16string>;
TEST_P(ParsingSeek_SampleLimiter_U16, Do) {
    using fixture = ParsingSeek_SampleLimiter_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && seeker <= sample_pos
        && (
            (
                limiter_beg < limiter_end
                && seeker <= limiter_beg
                && sample_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleLimiter_U16,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleLimiter_U16::generate_tests()
    )
);
using ParsingSeek_SampleLimiter_U8 = ::ParsingSeek_SampleLimiter<::std::u8string>;
TEST_P(ParsingSeek_SampleLimiter_U8, Do) {
    using fixture = ParsingSeek_SampleLimiter_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto sample_pos = ::uns::string::parsing::find(target, seeker, get_sample());

    const auto expected_result =
        sample_pos != target.cend()
        && seeker <= sample_pos
        && (
            (
                limiter_beg < limiter_end
                && seeker <= limiter_beg
                && sample_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::seek<fixture::string_type>(
            target
            , seeker
            , get_sample()
            , get_seeker_pos_in_sample()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            get_seeker_expected()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingSeek_SampleLimiter_U8,
    ::testing::ValuesIn(
        ::ParsingSeek_SampleLimiter_U8::generate_tests()
    )
);


namespace param_set {

    struct ParsingRead {
        ::std::size_t seeker_init = 0;
        ::std::size_t seeker_expected = 0;
        ::std::size_t fragment_idx = 0;
        ::std::size_t delimiter_idx = 0;
        ::uns::string::parsing::seeker_position seeker_pos_in_delimiter;
        ::std::size_t limiter_begin = 0;
        ::std::size_t limiter_length = 0;
    private:
        static ::std::string to_string(const ::uns::string::parsing::seeker_position& SeekerPosInDelimiter) {
            return "{ qualifier=" + ::std::string{
                    SeekerPosInDelimiter.qualifier == ::uns::string::parsing::seeker_position::from_begin
                    ? "from_begin"
                    : "from_end"
            }
            + ", offset=" + ::testing::PrintToString(SeekerPosInDelimiter.offset) + " }";
        };
    public:
        ::std::string to_string() const {
            return "{ seeker_init=" + ::testing::PrintToString(seeker_init)
                + ", seeker_expected=" + ::testing::PrintToString(seeker_expected)
                + ", fragment_idx=" + ::testing::PrintToString(fragment_idx)
                + ", delimiter_idx=" + ::testing::PrintToString(delimiter_idx)
                + ", seeker_pos_in_sample=" + to_string(seeker_pos_in_delimiter)
                + ", limiter_begin=" + ::testing::PrintToString(limiter_begin)
                + ", limiter_length=" + ::testing::PrintToString(limiter_length) + " }";
        };
    };

};

template<>
::std::string testing::PrintToString(const ::param_set::ParsingRead& Params) {
    return Params.to_string();
};

template<::uns::is_basic_string string_t>
class ParsingRead :
    public ::Parsing<string_t>
    , public ::testing::TestWithParam<::param_set::ParsingRead>
{
private:
    using base = ::ParsingRead<string_t>;
    using test = ::testing::TestWithParam<::param_set::ParsingRead>;
public:
    using string_type = string_t;
public:
    typename string_type::const_iterator get_seeker_init() const {
        return base::target.cbegin() + GetParam().seeker_init;
    };
    typename string_type::const_iterator get_seeker_expected() const {
        return base::target.cbegin() + GetParam().seeker_expected;
    };
    typename string_type get_fragment() const {
        return *(base::values.cbegin() + GetParam().fragment_idx);
    };
    typename string_type get_delimiter() const {
        return *(base::delimiters.cbegin() + GetParam().delimiter_idx);
    };
    ::uns::string::parsing::seeker_position get_seeker_pos_in_delimiter() const {
        return GetParam().seeker_pos_in_delimiter;
    };
    const ::std::tuple<
        string_type
        , typename string_type::const_iterator
        , typename string_type::const_iterator
    > get_limiter() const {
        auto limiter_beg = base::target.cend();
        auto limiter_end = base::target.cend();

        if (GetParam().limiter_begin < base::target.size()) {
            limiter_beg = base::target.cbegin() + GetParam().limiter_begin;
        };
        if (GetParam().limiter_begin + GetParam().limiter_length < base::target.size()) {
            limiter_end = base::target.cbegin() + GetParam().limiter_begin + GetParam().limiter_length;
        };

        return {
            string_type{ limiter_beg, limiter_end }
            , limiter_beg
            , limiter_end
        };
    };
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests_common() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 3;

        auto result = ::std::vector<::param_set::ParsingRead>{};
        auto single_test = ::param_set::ParsingRead{};

        single_test.seeker_pos_in_delimiter.qualifier = ::uns::string::parsing::seeker_position::from_begin;
        single_test.seeker_pos_in_delimiter.offset = 0;
        single_test.seeker_init = 0;
        auto precursor = base::start.size() + base::keys[0].size() + base::equality.size();
        for (::std::size_t idx = 0; idx < base::keys.size() && idx < base::values.size() && idx < base::delimiters.size(); ++idx) {
            single_test.fragment_idx = idx;
            single_test.delimiter_idx = idx;

            single_test.seeker_init = precursor;
            precursor += base::values[idx].size();
            for (auto seeker_result_position_variant : seeker_result_position_variants) {
                single_test.seeker_pos_in_delimiter.qualifier = seeker_result_position_variant;
                single_test.seeker_pos_in_delimiter.offset = -seeker_extremal_offset;

                for (; single_test.seeker_pos_in_delimiter.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_delimiter.offset) {
                    single_test.seeker_expected = static_cast<::std::size_t>(
                        static_cast<int>(precursor)
                        + single_test.seeker_pos_in_delimiter.offset
                    );
                    if (single_test.seeker_pos_in_delimiter.qualifier == ::uns::string::parsing::seeker_position::from_end) {
                        single_test.seeker_expected += base::delimiters[idx].size();
                    };

                    single_test.limiter_begin = 0;
                    const ::std::size_t lengths[6] = {
                        0
                        , 3
                        , 4
                        , 5
                        , 10
                        , base::target.size() - single_test.limiter_begin
                    };
                    for (; single_test.limiter_begin < base::target.size(); single_test.limiter_begin += 5) {
                        single_test.limiter_length = 0;

                        for (auto length : lengths) {
                            if (single_test.limiter_begin + length <= base::target.size()) {
                                single_test.limiter_length = length;

                                result.push_back(single_test);
                            };
                        };
                    };
                };
            };

            precursor += base::delimiters[idx].size();
            if (
                auto key_idx = idx + 1;
                key_idx < base::keys.size()
            ) {
                precursor += base::keys[key_idx].size() + base::equality.size();
            };
        };

        return result;
    };
};

template<::uns::is_basic_string string_t>
class ParsingRead_DelimitersExplicitLimiters : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimitersExplicitLimiters_U32 = ::ParsingRead_DelimitersExplicitLimiters<::std::u32string>;
TEST_P(ParsingRead_DelimitersExplicitLimiters_U32, Do) {
    using fixture = ParsingRead_DelimitersExplicitLimiters_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersExplicitLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersExplicitLimiters_U32::generate_tests()
    )
);
using ParsingRead_DelimitersExplicitLimiters_U16 = ::ParsingRead_DelimitersExplicitLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimitersExplicitLimiters_U16, Do) {
    using fixture = ParsingRead_DelimitersExplicitLimiters_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersExplicitLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersExplicitLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimitersExplicitLimiters_U8 = ::ParsingRead_DelimitersExplicitLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimitersExplicitLimiters_U8, Do) {
    using fixture = ParsingRead_DelimitersExplicitLimiters_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersExplicitLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersExplicitLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimiterExplicitLimiters : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimiterExplicitLimiters_U32 = ::ParsingRead_DelimiterExplicitLimiters<::std::u32string>;
TEST_P(ParsingRead_DelimiterExplicitLimiters_U32, Do) {
    using fixture = ParsingRead_DelimiterExplicitLimiters_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterExplicitLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterExplicitLimiters_U32::generate_tests()
    )
);
using ParsingRead_DelimiterExplicitLimiters_U16 = ::ParsingRead_DelimiterExplicitLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimiterExplicitLimiters_U16, Do) {
    using fixture = ParsingRead_DelimiterExplicitLimiters_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterExplicitLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterExplicitLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimiterExplicitLimiters_U8 = ::ParsingRead_DelimiterExplicitLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimiterExplicitLimiters_U8, Do) {
    using fixture = ParsingRead_DelimiterExplicitLimiters_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterExplicitLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersExplicitLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimitersNoLimiters : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimitersNoLimiters_U32 = ::ParsingRead_DelimitersNoLimiters<::std::u32string>;
TEST_P(ParsingRead_DelimitersNoLimiters_U32, Do) {
    using fixture = ParsingRead_DelimitersNoLimiters_U32;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersNoLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersNoLimiters_U32::generate_tests()
    )
);
using ParsingRead_DelimitersNoLimiters_U16 = ::ParsingRead_DelimitersNoLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimitersNoLimiters_U16, Do) {
    using fixture = ParsingRead_DelimitersNoLimiters_U16;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersNoLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersNoLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimitersNoLimiters_U8 = ::ParsingRead_DelimitersNoLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimitersNoLimiters_U8, Do) {
    using fixture = ParsingRead_DelimitersNoLimiters_U8;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersNoLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersNoLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimiterNoLimiters : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimiterNoLimiters_U32 = ::ParsingRead_DelimiterNoLimiters<::std::u32string>;
TEST_P(ParsingRead_DelimiterNoLimiters_U32, Do) {
    using fixture = ParsingRead_DelimiterNoLimiters_U32;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterNoLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterNoLimiters_U32::generate_tests()
    )
);
using ParsingRead_DelimiterNoLimiters_U16 = ::ParsingRead_DelimiterNoLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimiterNoLimiters_U16, Do) {
    using fixture = ParsingRead_DelimiterNoLimiters_U16;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterNoLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterNoLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimiterNoLimiters_U8 = ::ParsingRead_DelimiterNoLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimiterNoLimiters_U8, Do) {
    using fixture = ParsingRead_DelimiterNoLimiters_U8;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterNoLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterNoLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimitersLimiters : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    ::std::vector<string_type> get_limiters() {
        return {
            base::delimiters[1]
            , base::delimiters[2]
            , base::finish
        };
    };
    typename string_type::const_iterator get_seeker_limit(
        const typename string_type::const_iterator& Seeker
    ) {
        if (
            auto limiter_end = ::uns::string::parsing::find<string_type>(
                base::target
                , base::target.cbegin()
                , base::delimiters[1]
            )
            + base::delimiters[1].size();
            Seeker < limiter_end
        ) {
            return limiter_end;
        }
        else {
            return ::uns::string::parsing::find<string_type>(
                base::target
                , base::target.cbegin()
                , base::delimiters[2]
            )
            + base::delimiters[2].size();
        };
    };
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimitersLimiters_U32 = ::ParsingRead_DelimitersLimiters<::std::u32string>;
TEST_P(ParsingRead_DelimitersLimiters_U32, Do) {
    using fixture = ParsingRead_DelimitersLimiters_U32;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (get_seeker_expected() < get_seeker_limit(seeker));

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , get_limiters()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersLimiters_U32::generate_tests()
    )
);
using ParsingRead_DelimitersLimiters_U16 = ::ParsingRead_DelimitersLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimitersLimiters_U16, Do) {
    using fixture = ParsingRead_DelimitersLimiters_U16;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (get_seeker_expected() < get_seeker_limit(seeker));

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , get_limiters()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimitersLimiters_U8 = ::ParsingRead_DelimitersLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimitersLimiters_U8, Do) {
    using fixture = ParsingRead_DelimitersLimiters_U8;
    auto seeker = get_seeker_init();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (get_seeker_expected() < get_seeker_limit(seeker));

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , get_limiters()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimiterLimiters : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    ::std::vector<string_type> get_limiters() {
        return {
            base::delimiters[1]
            , base::delimiters[2]
            , base::finish
        };
    };
    typename string_type::const_iterator get_seeker_limit(
        const typename string_type::const_iterator& Seeker
    ) {
        if (
            auto limiter_end = ::uns::string::parsing::find<string_type>(
                base::target
                , base::target.cbegin()
                , base::delimiters[1]
            )
            + base::delimiters[1].size();
            Seeker < limiter_end
        ) {
            return limiter_end;
        }
        else {
            return ::uns::string::parsing::find<string_type>(
                base::target
                , base::target.cbegin()
                , base::delimiters[2]
            )
                + base::delimiters[2].size();
        };
    };
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimiterLimiters_U32 = ::ParsingRead_DelimiterLimiters<::std::u32string>;
TEST_P(ParsingRead_DelimiterLimiters_U32, Do) {
    using fixture = ParsingRead_DelimiterLimiters_U32;
    auto seeker = get_seeker_init();

    const auto delimiter_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiter_pos != target.cend()
        && (seeker < delimiter_pos)
        && (get_seeker_expected() < get_seeker_limit(seeker));

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , get_limiters()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterLimiters_U32::generate_tests()
    )
);
using ParsingRead_DelimiterLimiters_U16 = ::ParsingRead_DelimiterLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimiterLimiters_U16, Do) {
    using fixture = ParsingRead_DelimiterLimiters_U16;
    auto seeker = get_seeker_init();

    const auto delimiter_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiter_pos != target.cend()
        && (seeker < delimiter_pos)
        && (get_seeker_expected() < get_seeker_limit(seeker));

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , get_limiters()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimiterLimiters_U8 = ::ParsingRead_DelimiterLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimiterLimiters_U8, Do) {
    using fixture = ParsingRead_DelimiterLimiters_U8;
    auto seeker = get_seeker_init();

    const auto delimiter_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiter_pos != target.cend()
        && (seeker < delimiter_pos)
        && (get_seeker_expected() < get_seeker_limit(seeker));

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , get_limiters()
        )
    ) << "Target=\"" << ::testing::PrintToString(target);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterLimiters_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimitersLimiter : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimitersLimiter_U32 = ::ParsingRead_DelimitersLimiter<::std::u32string>;
TEST_P(ParsingRead_DelimitersLimiter_U32, Do) {
    using fixture = ParsingRead_DelimitersLimiter_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && seeker < delimiters_pos
        && (
            (
                limiter_beg < limiter_end
                && delimiters_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersLimiter_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersLimiter_U32::generate_tests()
    )
);
using ParsingRead_DelimitersLimiter_U16 = ::ParsingRead_DelimitersLimiter<::std::u16string>;
TEST_P(ParsingRead_DelimitersLimiter_U16, Do) {
    using fixture = ParsingRead_DelimitersLimiter_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && seeker < delimiters_pos
        && (
            (
                limiter_beg < limiter_end
                && delimiters_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                    )
                && get_seeker_expected() < target.cend()
            )
        );

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersLimiter_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersLimiter_U16::generate_tests()
    )
);
using ParsingRead_DelimitersLimiter_U8 = ::ParsingRead_DelimitersLimiter<::std::u8string>;
TEST_P(ParsingRead_DelimitersLimiter_U8, Do) {
    using fixture = ParsingRead_DelimitersLimiter_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && seeker < delimiters_pos
        && (
            (
                limiter_beg < limiter_end
                && delimiters_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersLimiter_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersLimiter_U8::generate_tests()
    )
);

template<::uns::is_basic_string string_t>
class ParsingRead_DelimiterLimiter : public::ParsingRead<string_t> {
private:
    using base = ::ParsingRead<string_t>;
public:
    using string_type = string_t;
public:
    static ::std::vector<::param_set::ParsingRead> generate_tests() {
        return base::generate_tests_common();
    };
};

using ParsingRead_DelimiterLimiter_U32 = ::ParsingRead_DelimiterLimiter<::std::u32string>;
TEST_P(ParsingRead_DelimiterLimiter_U32, Do) {
    using fixture = ParsingRead_DelimiterLimiter_U32;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && seeker < delimiters_pos
        && (
            (
                limiter_beg < limiter_end
                && delimiters_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterLimiter_U32,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterLimiter_U32::generate_tests()
    )
);
using ParsingRead_DelimiterLimiter_U16 = ::ParsingRead_DelimiterLimiter<::std::u16string>;
TEST_P(ParsingRead_DelimiterLimiter_U16, Do) {
    using fixture = ParsingRead_DelimiterLimiter_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && seeker < delimiters_pos
        && (
            (
                limiter_beg < limiter_end
                && delimiters_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterLimiter_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterLimiter_U16::generate_tests()
    )
);
using ParsingRead_DelimiterLimiter_U8 = ::ParsingRead_DelimiterLimiter<::std::u8string>;
TEST_P(ParsingRead_DelimiterLimiter_U8, Do) {
    using fixture = ParsingRead_DelimiterLimiter_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, get_delimiter());

    const auto expected_result =
        delimiters_pos != target.cend()
        && seeker < delimiters_pos
        && (
            (
                limiter_beg < limiter_end
                && delimiters_pos <= limiter_beg
                && get_seeker_expected() < limiter_end
            )
            || (
                (
                    limiter_beg == limiter_end
                    || seeker > limiter_beg
                )
                && get_seeker_expected() < target.cend()
            )
        );

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , get_delimiter()
            , get_seeker_pos_in_delimiter()
            , limiter
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimiterLimiter_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimiterLimiter_U8::generate_tests()
    )
);



namespace param_set {

    struct ParsingObtain {
        ::std::size_t key_idx = 0;
        ::uns::string::parsing::seeker_position seeker_pos_in_delimiter;
        ::std::size_t limiter_begin = 0;
        ::std::size_t limiter_length = 0;
    private:
        static ::std::string to_string(const ::uns::string::parsing::seeker_position& SeekerPosInDelimiter) {
            return "{ qualifier=" + ::std::string{
                    SeekerPosInDelimiter.qualifier == ::uns::string::parsing::seeker_position::from_begin
                    ? "from_begin"
                    : "from_end"
            }
            + ", offset=" + ::testing::PrintToString(SeekerPosInDelimiter.offset) + " }";
        };
    public:
        ::std::string to_string() const {
            return "{ key_idx=" + ::testing::PrintToString(key_idx)
                + ", seeker_pos_in_sample=" + to_string(seeker_pos_in_delimiter)
                + ", limiter_begin=" + ::testing::PrintToString(limiter_begin)
                + ", limiter_length=" + ::testing::PrintToString(limiter_length) + " }";
        };
    };

};

template<>
::std::string testing::PrintToString(const ::param_set::ParsingObtain& Params) {
    return Params.to_string();
};

template<::uns::is_basic_string string_t>
class ParsingObtain :
    public ::Parsing<string_t>
    , public ::testing::TestWithParam<::param_set::ParsingObtain>
{
private:
    using base = ::ParsingObtain<string_t>;
    using test = ::testing::TestWithParam<::param_set::ParsingObtain>;
public:
    using string_type = string_t;
private:
    inline static ::std::vector<::std::size_t> s_key_poss;
public:
    static void SetUpTestCase() {
        if (!s_key_poss.empty()) return;
        for (const auto& key : base::keys) {
            s_key_poss.push_back(base::target.find(key));
        };
    };
public:
    string_type get_key() const {
        return *(base::keys.cbegin() + GetParam().key_idx);
    };
    typename string_type::const_iterator get_value_pos() const {
        if (GetParam().seeker_pos_in_delimiter.qualifier == ::uns::string::parsing::seeker_position::from_begin) {
            if (
                static_cast<long int>(s_key_poss.at(GetParam().key_idx))
                + GetParam().seeker_pos_in_delimiter.offset < 0
            ) {
                return base::target.cend();
            }
            else if (
                static_cast<long int>(s_key_poss.at(GetParam().key_idx))
                + GetParam().seeker_pos_in_delimiter.offset >= static_cast<long int>(base::target.size())
            ) {
                return base::target.cend();
            }
            else {
                return base::target.cbegin()
                    + static_cast<::std::size_t>(
                        static_cast<long int>(s_key_poss.at(GetParam().key_idx))
                        + GetParam().seeker_pos_in_delimiter.offset
                    );
            };
        }
        else {
            if (
                static_cast<long int>(s_key_poss.at(GetParam().key_idx))
                + static_cast<long int>(base::keys[GetParam().key_idx].size())
                + GetParam().seeker_pos_in_delimiter.offset < 0
            ) {
                return base::target.cend();
            }
            else if (
                static_cast<long int>(s_key_poss.at(GetParam().key_idx))
                + static_cast<long int>(base::keys[GetParam().key_idx].size())
                + GetParam().seeker_pos_in_delimiter.offset >= static_cast<long int>(base::target.size())
            ) {
                return base::target.cend();
            }
            else {
                return base::target.cbegin()
                    + static_cast<::std::size_t>(
                        static_cast<long int>(s_key_poss.at(GetParam().key_idx))
                        + static_cast<long int>(base::keys[GetParam().key_idx].size())
                        + GetParam().seeker_pos_in_delimiter.offset
                    );
            };
        };
    };
    string_type get_value(const typename string_type::const_iterator& DelimiterPos) const {
        if (
            get_value_pos() < DelimiterPos
        ) {
            return string_type{
                get_value_pos()
                , DelimiterPos
            };
        }
        else {
            return {};
        };
    };
    typename string_type get_delimiter() const {
        return *(base::delimiters.cbegin() + GetParam().key_idx);
    };
    ::uns::string::parsing::seeker_position get_seeker_pos_in_delimiter() const {
        return GetParam().seeker_pos_in_delimiter;
    };
    const ::std::tuple<
        string_type
        , typename string_type::const_iterator
        , typename string_type::const_iterator
    > get_limiter() const {
        auto limiter_beg = base::target.cend();
        auto limiter_end = base::target.cend();

        if (GetParam().limiter_begin < base::target.size()) {
            limiter_beg = base::target.cbegin() + GetParam().limiter_begin;
        };
        if (GetParam().limiter_begin + GetParam().limiter_length < base::target.size()) {
            limiter_end = base::target.cbegin() + GetParam().limiter_begin + GetParam().limiter_length;
        };

        return {
            string_type{ limiter_beg, limiter_end }
            , limiter_beg
            , limiter_end
        };
    };
public:
    static ::std::vector<::param_set::ParsingObtain> generate_tests() {
        const ::uns::string::parsing::seeker_position::sample_relative seeker_result_position_variants[2] = {
            ::uns::string::parsing::seeker_position::from_begin
            , ::uns::string::parsing::seeker_position::from_end
        };
        const int seeker_extremal_offset = 20;

        auto result = ::std::vector<::param_set::ParsingObtain>{};
        auto single_test = ::param_set::ParsingObtain{};

        single_test.key_idx = 0;
        for (; single_test.key_idx < base::keys.size(); single_test.key_idx++) {
            for (auto seeker_result_position_variant : seeker_result_position_variants) {
                single_test.seeker_pos_in_delimiter.qualifier = seeker_result_position_variant;
                single_test.seeker_pos_in_delimiter.offset = -seeker_extremal_offset;

                for (; single_test.seeker_pos_in_delimiter.offset < seeker_extremal_offset; ++single_test.seeker_pos_in_delimiter.offset) {
                    single_test.limiter_begin = 0;
                    for (; single_test.limiter_begin < base::target.size(); single_test.limiter_begin += 5) {
                        single_test.limiter_length = 0;

                        const ::std::size_t lengths[6] = {
                            0
                            , 3
                            , 4
                            , 5
                            , 10
                            , base::target.size() - single_test.limiter_begin
                        };
                        for (auto length : lengths) {
                            if (single_test.limiter_begin + length <= base::target.size()) {
                                single_test.limiter_length = length;

                                result.push_back(single_test);
                            };
                        };
                    };
                };
            };
        };

        return result;
    };
};


using ParsingObtain_DelimitersExplicitLimiters_U32 = ::ParsingObtain<::std::u32string>;
TEST_P(ParsingObtain_DelimitersExplicitLimiters_U32, Do) {
    using fixture = ParsingObtain_DelimitersExplicitLimiters_U32;
    const auto key = get_key();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto key_pos = ::uns::string::parsing::find(target, target.cbegin(), key);
    const auto delimiters_pos = ::uns::string::parsing::find(target, get_value_pos(), delimiters);

    const auto expected_result =
        key_pos <= limiter_beg
        && delimiters_pos != target.cend()
        && delimiters_pos <= limiter_beg
        && get_value_pos() < limiter_beg
        && get_value_pos() < delimiters_pos;

    auto value = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::obtain<fixture::string_type>(
            target
            , key
            , get_seeker_pos_in_delimiter()
            , value
            , delimiters
            , limiter_beg
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            value
            , get_value(delimiters_pos)
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(value.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingObtain_DelimitersExplicitLimiters_U32,
    ::testing::ValuesIn(
        ::ParsingObtain_DelimitersExplicitLimiters_U32::generate_tests()
    )
);
using ParsingObtain_DelimitersExplicitLimiters_U16 = ::ParsingObtain<::std::u32string>;
TEST_P(ParsingObtain_DelimitersExplicitLimiters_U16, Do) {
    using fixture = ParsingObtain_DelimitersExplicitLimiters_U16;
    const auto key = get_key();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto key_pos = ::uns::string::parsing::find(target, target.cbegin(), key);
    const auto delimiters_pos = ::uns::string::parsing::find(target, get_value_pos(), delimiters);

    const auto expected_result =
        key_pos <= limiter_beg
        && delimiters_pos != target.cend()
        && delimiters_pos <= limiter_beg
        && get_value_pos() < limiter_beg
        && get_value_pos() < delimiters_pos;

    auto value = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::obtain<fixture::string_type>(
            target
            , key
            , get_seeker_pos_in_delimiter()
            , value
            , delimiters
            , limiter_beg
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            value
            , get_value(delimiters_pos)
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(value.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingObtain_DelimitersExplicitLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingObtain_DelimitersExplicitLimiters_U16::generate_tests()
    )
);
using ParsingObtain_DelimitersExplicitLimiters_U8 = ::ParsingObtain<::std::u32string>;
TEST_P(ParsingObtain_DelimitersExplicitLimiters_U8, Do) {
    using fixture = ParsingObtain_DelimitersExplicitLimiters_U8;
    const auto key = get_key();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto key_pos = ::uns::string::parsing::find(target, target.cbegin(), key);
    const auto delimiters_pos = ::uns::string::parsing::find(target, get_value_pos(), delimiters);

    const auto expected_result =
        key_pos <= limiter_beg
        && delimiters_pos != target.cend()
        && delimiters_pos <= limiter_beg
        && get_value_pos() < limiter_beg
        && get_value_pos() < delimiters_pos;

    auto value = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::obtain<fixture::string_type>(
            target
            , key
            , get_seeker_pos_in_delimiter()
            , value
            , delimiters
            , limiter_beg
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            value
            , get_value(delimiters_pos)
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(value.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingObtain_DelimitersExplicitLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingObtain_DelimitersExplicitLimiters_U8::generate_tests()
    )
);
/*using ParsingRead_DelimitersExplicitLimiters_U16 = ::ParsingRead_DelimitersExplicitLimiters<::std::u16string>;
TEST_P(ParsingRead_DelimitersExplicitLimiters_U16, Do) {
    using fixture = ParsingRead_DelimitersExplicitLimiters_U16;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersExplicitLimiters_U16,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersExplicitLimiters_U16::generate_tests()
    )
);
using ParsingRead_DelimitersExplicitLimiters_U8 = ::ParsingRead_DelimitersExplicitLimiters<::std::u8string>;
TEST_P(ParsingRead_DelimitersExplicitLimiters_U8, Do) {
    using fixture = ParsingRead_DelimitersExplicitLimiters_U8;
    auto seeker = get_seeker_init();
    const auto [limiter, limiter_beg, limiter_end] = get_limiter();

    const auto delimiters_pos = ::uns::string::parsing::find(target, seeker, delimiters);

    const auto expected_result =
        delimiters_pos != target.cend()
        && (seeker < delimiters_pos)
        && (delimiters_pos <= limiter_beg)
        && (get_seeker_expected() < limiter_end);

    auto fragment = fixture::string_type{};
    ASSERT_EQ(
        expected_result
        , ::uns::string::parsing::read<fixture::string_type>(
            target
            , seeker
            , fragment
            , delimiters
            , get_seeker_pos_in_delimiter()
            , limiter_beg
            , limiter_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target)
        << ", limiter=\"" << ::testing::PrintToString(limiter);

    if (expected_result) {
        ASSERT_EQ(
            fragment
            , get_fragment()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            seeker
            , get_seeker_expected()
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    }
    else {
        ASSERT_TRUE(fragment.empty()) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);

        ASSERT_EQ(
            get_seeker_init()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target)
            << ", limiter=\"" << ::testing::PrintToString(limiter);
    };
};
INSTANTIATE_TEST_CASE_P(ParsingMethods, ParsingRead_DelimitersExplicitLimiters_U8,
    ::testing::ValuesIn(
        ::ParsingRead_DelimitersExplicitLimiters_U8::generate_tests()
    )
);*/

