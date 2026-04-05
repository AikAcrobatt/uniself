
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


template<
    ::uns::is_basic_string string_t
    , ::std::integral shift1_t
    , ::std::integral shift2_t
    , ::std::integral shift3_t
>
class SeekerMethods : public ::testing::TestWithParam<
    ::std::tuple<
        string_t
        , shift1_t
        , shift2_t
        , shift3_t
    >
> {
public:
    using string_type = string_t;
    using shift1_type = shift1_t;
    using shift2_type = shift2_t;
    using shift3_type = shift3_t;
    using elem_type = ::std::tuple<
        string_type
        , shift1_type
        , shift1_type
        , shift3_type
    >;
public:
    enum elem_part {
        sample = 0
        , shift1 = 1
        , shift2 = 2
        , shift3 = 3
    };
public:
    static ::std::string to_string(const elem_type& Elem) {
        return "IntegerCasts::Elem{ "
            + testing::PrintToString(
                ::std::get<elem_part::sample>(Elem)
            )
            + ", sh1=" + ::uns::string::cast<::std::string>(::std::get<elem_part::shift1>(Elem))
            + ", sh2=" + ::uns::string::cast<::std::string>(::std::get<elem_part::shift2>(Elem))
            + ", sh3= " + ::uns::string::cast<::std::string>(::std::get<elem_part::shift3>(Elem))
            + " }";
    };
};


class SeekerFindU32 : public ::SeekerMethods<
    ::std::u32string
    , typename ::std::iterator_traits<typename ::std::u32string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::u32string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::vector<::std::u32string>::const_iterator>::difference_type
> {
public:
    using samples_collection_type = ::std::vector<::std::u32string>;
public:
    inline static string_type target;
    inline static samples_collection_type samples;
public:
    static void SetUpTestCase() {
        target = U"0123456789ABCГEF";
        samples = {
            U"45"
            , U"ABCГ"
            , U"46Г"
        };
    };
    static elem_type make(
        shift1_type shift1
        , shift2_type shift2
        , shift3_type sample_collection_shift
    ) {
        return elem_type{
            U""
            , shift1
            , shift2
            , sample_collection_shift
        };
    };
};

template<>
::std::string testing::PrintToString(const ::SeekerFindU32::elem_type& Elem) {
    return ::SeekerFindU32::to_string(Elem);
};

TEST_P(SeekerFindU32, FindSamples) {
    using TestCaseFixture = SeekerFindU32;

    auto found_sample = samples.cend();
    const auto seeker = target.cbegin() + ::std::get<TestCaseFixture::shift1>(GetParam());

    const auto result = ::uns::string::parse::find(
        target
        , seeker
        , samples
        , found_sample
    ) - target.cbegin();

    ASSERT_EQ(result, ::std::get<TestCaseFixture::shift2>(GetParam()));

    ASSERT_EQ(
        found_sample - samples.cbegin()
        , ::std::get<TestCaseFixture::shift3>(GetParam())
    );
};

INSTANTIATE_TEST_CASE_P(SeekerMethodsTests, SeekerFindU32,
    ::testing::Values(
        ::SeekerFindU32::make(0, 4, 0)
        , ::SeekerFindU32::make(1, 4, 0)
        , ::SeekerFindU32::make(2, 4, 0)
        , ::SeekerFindU32::make(3, 4, 0)
        , ::SeekerFindU32::make(4, 4, 0)
        , ::SeekerFindU32::make(5, 10, 1)
        , ::SeekerFindU32::make(6, 10, 1)
        , ::SeekerFindU32::make(7, 10, 1)
        , ::SeekerFindU32::make(8, 10, 1)
        , ::SeekerFindU32::make(9, 10, 1)
        , ::SeekerFindU32::make(10, 10, 1)
        , ::SeekerFindU32::make(11, 16, 3)
        , ::SeekerFindU32::make(12, 16, 3)
        , ::SeekerFindU32::make(13, 16, 3)
        , ::SeekerFindU32::make(14, 16, 3)
        , ::SeekerFindU32::make(15, 16, 3)
        , ::SeekerFindU32::make(16, 16, 3)
    )
);


class SeekerFindU8 : public ::SeekerMethods<
    ::std::u8string
    , typename ::std::iterator_traits<typename ::std::u8string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::u8string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::vector<::std::u8string>::const_iterator>::difference_type
> {
public:
    using samples_collection_type = ::std::vector<::std::u8string>;
public:
    inline static string_type target;
    inline static samples_collection_type samples;
public:
    static void SetUpTestCase() {
        target = u8"\u304C\u3180абв\u0401";
        samples = {
            u8"\u3180"
            , u8"бв"
            , u8"Г"
        };
    };
    static elem_type make(
        shift1_type shift1
        , shift1_type shift2
        , shift3_type sample_collection_shift
    ) {
        return elem_type{
            u8""
            , shift1
            , shift2
            , sample_collection_shift
        };
    };
};

template<>
::std::string testing::PrintToString(const ::SeekerFindU8::elem_type& Elem) {
    return ::SeekerFindU8::to_string(Elem);
};


TEST_P(SeekerFindU8, FindSamples) {
    using TestCaseFixture = SeekerFindU8;

    auto found_sample = samples.cend();
    const auto seeker = target.cbegin() + ::std::get<TestCaseFixture::shift1>(GetParam());

    const auto result = ::uns::string::parse::find(
        target
        , seeker
        , samples
        , found_sample
    ) - target.cbegin();

    ASSERT_EQ(result, ::std::get<TestCaseFixture::shift2>(GetParam()));

    ASSERT_EQ(
        found_sample - samples.cbegin()
        , ::std::get<TestCaseFixture::shift3>(GetParam())
    );
};

INSTANTIATE_TEST_CASE_P(SeekerMethodsTests, SeekerFindU8,
    ::testing::Values(
        ::SeekerFindU8::make(0, 3, 0)
        , ::SeekerFindU8::make(1, 3, 0)
        , ::SeekerFindU8::make(2, 3, 0)
        , ::SeekerFindU8::make(3, 3, 0)
        , ::SeekerFindU8::make(4, 8, 1)
        , ::SeekerFindU8::make(5, 8, 1)
        , ::SeekerFindU8::make(6, 8, 1)
        , ::SeekerFindU8::make(7, 8, 1)
        , ::SeekerFindU8::make(8, 8, 1)
        , ::SeekerFindU8::make(9, 14, 3)
        , ::SeekerFindU8::make(10, 14, 3)
        , ::SeekerFindU8::make(11, 14, 3)
        , ::SeekerFindU8::make(12, 14, 3)
        , ::SeekerFindU8::make(13, 14, 3)
        , ::SeekerFindU8::make(14, 14, 3)
    )
);


class SeekerSetU32 : public ::SeekerMethods<
    ::std::u32string
    , typename ::std::iterator_traits<typename ::std::u32string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::u32string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::u32string::const_iterator>::difference_type
> {
public:
    using samples_collection_type = ::std::vector<::std::u32string>;
public:
    inline static samples_collection_type samples;
    inline static string_type equality = U" = ";
    inline static string_type target = U"start:some_key1" + equality + U"some_val1; некий_ключ2" + equality + U"некое_значение2; some_key3" + equality + U"some_val3; xvx";
public:
    static void SetUpTestCase() {
        samples = {
            U"some_key1"
            , U"некий_ключ2"
            , U"some_key3"
        };
    };
    static elem_type make(
        shift1_type shift1
        , shift2_type shift2
        , shift3_type sample_collection_shift
    ) {
        return elem_type{
            U""
            , shift1
            , shift2
            , sample_collection_shift
        };
    };
};

TEST_P(SeekerSetU32, RepresentativeTests) {
    const auto sample = ::std::get<::SeekerSetU32::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU32::elem_part::shift1>(GetParam());
    const auto rborder_beg = target.cbegin() + ::std::get<::SeekerSetU32::elem_part::shift2>(GetParam());
    const auto rborder_end = target.cbegin() + ::std::get<::SeekerSetU32::elem_part::shift3>(GetParam());

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (pos_of_key + sample.size() + equality.size() <= rborder_end)
        && (pos_of_key <= rborder_beg);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU32::string_type>(
            target
            , seeker
            , samples
            , false
            , -4
            , rborder_beg
            , rborder_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };

};

INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key0, SeekerSetU32,
    ::testing::Combine(
        ::testing::Values(::std::u32string{ U"some_key1" })
        , ::testing::Values(0, 3, 6)
        , ::testing::Range<
            ::SeekerSetU32::shift2_type
            , ::SeekerSetU32::shift2_type
        >(0, ::SeekerSetU32::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU32::shift3_type
            , ::SeekerSetU32::shift3_type
        >(0, ::SeekerSetU32::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key1, SeekerSetU32,
    ::testing::Combine(
        ::testing::Values(::std::u32string{ U"некий_ключ2" })
        , ::testing::Values(7, 20, 28, 29)
        , ::testing::Range<
            ::SeekerSetU32::shift2_type
            , ::SeekerSetU32::shift2_type
        >(0, ::SeekerSetU32::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU32::shift3_type
            , ::SeekerSetU32::shift3_type
        >(0, ::SeekerSetU32::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key2, SeekerSetU32,
    ::testing::Combine(
        ::testing::Values(::std::u32string{ U"some_key3" })
        , ::testing::Values(30, 40, 50, 60)
        , ::testing::Range<
            ::SeekerSetU32::shift2_type
            , ::SeekerSetU32::shift2_type
        >(0, ::SeekerSetU32::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU32::shift3_type
            , ::SeekerSetU32::shift3_type
        >(0, ::SeekerSetU32::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key3, SeekerSetU32,
    ::testing::Combine(
        ::testing::Values(::std::u32string{ U"false_key" })
        , ::testing::Values(61, 70)
        , ::testing::Range<
            ::SeekerSetU32::shift2_type
            , ::SeekerSetU32::shift2_type
        >(0, ::SeekerSetU32::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU32::shift3_type
            , ::SeekerSetU32::shift3_type
        >(0, ::SeekerSetU32::target.size(), 5)
    )
);


class SeekerSetU8 : public ::SeekerMethods<
    ::std::u8string
    , typename ::std::iterator_traits<typename ::std::u8string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::u8string::const_iterator>::difference_type
    , typename ::std::iterator_traits<typename ::std::u8string::const_iterator>::difference_type
> {
public:
    using samples_collection_type = ::std::vector<::std::u8string>;
public:
    inline static samples_collection_type samples;
    inline static string_type equality = u8" = ";
    inline static string_type target = u8"start:some_key1" + equality + u8"SOME_VAL1; некий_ключ2" + equality + u8"некое_значение2; ani_clue3" + equality + u8"ani_mean3; xvx";
public:
    static void SetUpTestCase() {
        samples = {
            u8"some_key1"
            , u8"некий_ключ2"
            , u8"ani_clue3"
        };
    };
    static elem_type make(
        shift1_type shift1
        , shift2_type shift2
        , shift3_type sample_collection_shift
    ) {
        return elem_type{
            u8""
            , shift1
            , shift2
            , sample_collection_shift
        };
    };
};

TEST_P(SeekerSetU8, SamplesExplicitRightBorders) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());
    const auto rborder_beg = target.cbegin() + (
        ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        ? ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        : ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
    );
    const auto rborder_end = target.cbegin() + (
        ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        ? ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        : ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
    );

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (pos_of_key + sample.size() + equality.size() <= rborder_end)
        && (pos_of_key <= rborder_beg);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , samples
            , false
            , -4
            , rborder_beg
            , rborder_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };

};
TEST_P(SeekerSetU8, SampleExplicitRightBorders) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());
    const auto rborder_beg = target.cbegin() + (
        ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        ? ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        : ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
    );
    const auto rborder_end = target.cbegin() + (
        ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        ? ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        : ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
    );

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (pos_of_key + sample.size() + equality.size() <= rborder_end)
        && (pos_of_key <= rborder_beg);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , sample
            , false
            , -4
            , rborder_beg
            , rborder_end
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };

};
TEST_P(SeekerSetU8, SamplesNoRightBorders) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , samples
            , false
            , -4
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };
};
TEST_P(SeekerSetU8, SampleNoRightBorders) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , sample
            , false
            , -4
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };
};
TEST_P(SeekerSetU8, SamplesRightBorders) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());
    const auto rborders = ::std::vector<::std::u8string>{
        u8" x"
        , u8"xv"
        , u8"vx"
    };
    const auto rborder_beg = ::uns::string::parse::find(target, seeker, rborders);
    const auto rborder_end = target.cend();
    ASSERT_LT(rborder_beg, rborder_end);

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (pos_of_key + sample.size() + equality.size() <= rborder_end)
        && (pos_of_key <= rborder_beg);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , samples
            , false
            , -4
            , rborders
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };

};
TEST_P(SeekerSetU8, SampleRightBorders) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());
    const auto rborders = ::std::vector<::std::u8string>{
        u8" x"
        , u8"xv"
        , u8"vx"
    };
    const auto rborder_beg = ::uns::string::parse::find(target, seeker, rborders);
    const auto rborder_end = target.cend();
    ASSERT_LT(rborder_beg, rborder_end);

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (pos_of_key + sample.size() + equality.size() <= rborder_end)
        && (pos_of_key <= rborder_beg);

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , sample
            , false
            , -4
            , rborders
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker - target.cbegin());

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker - target.cbegin());
    };

};
TEST_P(SeekerSetU8, SamplesRightBorder) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());
    const auto rborder = ::std::u8string{
        target.cbegin() + (
            ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
            : ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        )
        , target.cbegin() + (
            ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
            : ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        )
    };
    const auto rborder_beg = ::uns::string::parse::find(target, seeker, rborder);
    const auto rborder_end = (
        rborder_beg != target.cend()
        ? rborder_beg + rborder.size()
        : target.cend()
    );

    auto pos_of_key = ::uns::string::parse::find(target, target.begin(), sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (
            rborder.empty()
            || (
                !rborder.empty()
                && (
                    (rborder_end != target.cend())
                    && (pos_of_key + sample.size() + equality.size() <= rborder_end)
                )
                && (
                    (rborder_beg != target.cend())
                    && (pos_of_key <= rborder_beg)
                )
            )
        );

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , samples
            , false
            , -4
            , rborder
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker_before_seeking - target.cbegin())
        << "; rborder=" << ::testing::PrintToString(rborder);

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker_before_seeking - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker_before_seeking - target.cbegin());
    };

};
TEST_P(SeekerSetU8, SampleRightBorder) {
    const auto sample = ::std::get<::SeekerSetU8::elem_part::sample>(GetParam());
    auto seeker = target.cbegin() + ::std::get<::SeekerSetU8::elem_part::shift1>(GetParam());
    const auto rborder = ::std::u8string{
        target.cbegin() + (
            ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
            : ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
        )
        , target.cbegin() + (
            ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
            <= ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
            ? ::std::get<::SeekerSetU8::elem_part::shift3>(GetParam())
            : ::std::get<::SeekerSetU8::elem_part::shift2>(GetParam())
        )
    };
    const auto rborder_beg = ::uns::string::parse::find(target, seeker, rborder);
    const auto rborder_end = (
        rborder_beg != target.cend()
        ? rborder_beg + rborder.size()
        : target.cend()
    );

    auto pos_of_key = ::uns::string::parse::find(target, seeker, sample);

    auto seeking_result =
        pos_of_key != target.cend()
        && (seeker <= pos_of_key)
        && (
            rborder.empty()
            || (
                !rborder.empty()
                && (
                    (rborder_end != target.cend())
                    && (pos_of_key + sample.size() + equality.size() <= rborder_end)
                )
                && (
                    (rborder_beg != target.cend())
                    && (pos_of_key <= rborder_beg)
                )
            )
        );

    const auto seeker_before_seeking = seeker;
    ASSERT_EQ(
        seeking_result
        , ::uns::string::parse::seek<::SeekerSetU8::string_type>(
            target
            , seeker
            , sample
            , false
            , -4
            , rborder
        )
    ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
        << "; pos_of_key=" << (pos_of_key - target.cbegin())
        << "; sample.size()=" << sample.size()
        << "; equality.size()=" << equality.size()
        << "; seeker=" << (seeker_before_seeking - target.cbegin())
        << "; rborder=" << ::testing::PrintToString(rborder);

    if (seeking_result) {
        ASSERT_EQ(
            pos_of_key + sample.size() + equality.size()
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "; seeker=" << (seeker_before_seeking - target.cbegin());
    }
    else {
        ASSERT_EQ(
            seeker_before_seeking
            , seeker
        ) << "Target=\"" << ::testing::PrintToString(target) << "\"\n"
            << "; pos_of_key=" << (pos_of_key - target.cbegin())
            << "; sample.size()=" << sample.size()
            << "; equality.size()=" << equality.size()
            << "seeker_before_seeking=" << (seeker_before_seeking - target.cbegin())
            << "; seeker=" << (seeker_before_seeking - target.cbegin());
    };

};

INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key0, SeekerSetU8,
    ::testing::Combine(
        ::testing::Values(::std::u8string{ u8"some_key1" })
        , ::testing::Values(0, 3, 6)
        , ::testing::Range<
            ::SeekerSetU8::shift2_type
            , ::SeekerSetU8::shift2_type
        >(0, ::SeekerSetU8::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU8::shift3_type
            , ::SeekerSetU8::shift3_type
        >(0, ::SeekerSetU8::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key1, SeekerSetU8,
    ::testing::Combine(
        ::testing::Values(::std::u8string{ u8"некий_ключ2" })
        , ::testing::Values(7, 20, 28, 29)
        , ::testing::Range<
            ::SeekerSetU8::shift2_type
            , ::SeekerSetU8::shift2_type
        >(0, ::SeekerSetU8::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU8::shift3_type
            , ::SeekerSetU8::shift3_type
        >(0, ::SeekerSetU8::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key2, SeekerSetU8,
    ::testing::Combine(
        ::testing::Values(::std::u8string{ u8"ani_clue3" })
        , ::testing::Values(30, 50, 70, 80, 82)
        , ::testing::Range<
            ::SeekerSetU8::shift2_type
            , ::SeekerSetU8::shift2_type
        >(0, ::SeekerSetU8::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU8::shift3_type
            , ::SeekerSetU8::shift3_type
        >(0, ::SeekerSetU8::target.size(), 5)
    )
);
INSTANTIATE_TEST_CASE_P(SeekerMethodsTests_Key3, SeekerSetU8,
    ::testing::Combine(
        ::testing::Values(::std::u8string{ u8"false_key" })
        , ::testing::Values(83, 90)
        , ::testing::Range<
            ::SeekerSetU8::shift2_type
            , ::SeekerSetU8::shift2_type
        >(0, ::SeekerSetU8::target.size(), 5)
        , ::testing::Range<
            ::SeekerSetU8::shift3_type
            , ::SeekerSetU8::shift3_type
        >(0, ::SeekerSetU8::target.size(), 5)
    )
);

