

#include "uniself/strings.hpp"

#include "gtest/gtest.h"

namespace uns::tests::string::bwdconversions {

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
class FullCicleConversions : public ::testing::Test {};

TYPED_TEST_CASE(FullCicleConversions, ::uns::tests::string::bwdconversions::types_list);

TYPED_TEST(FullCicleConversions, BackwardConversionTest) {
    ASSERT_TRUE(
        ::uns::tests::string::bwdconversions::testphrase == ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<TypeParam>(::uns::tests::string::bwdconversions::testphrase)
        )
    );
};
