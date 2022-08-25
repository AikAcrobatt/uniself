

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#include "strings.h"


#define BOOST_TEST_MODULE uns::strings::test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE(string_cast_testing)

    BOOST_AUTO_TEST_SUITE(u8_strings)

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make({ u8"ABCD efgh !@#% АБВГдеёжзик_" }),
            the_val
        ) {
            //direct testing results are very significant while testing at file ostreams
            BOOST_TEST(the_val == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::string>(the_val)));
            BOOST_TEST(the_val == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::wstring>(the_val)));
            BOOST_TEST(the_val == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::u16string>(the_val)));
            BOOST_TEST(the_val == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::u32string>(the_val)));

        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(bools)

        BOOST_DATA_TEST_CASE(stdstring_to_bool_correct,
            boost::unit_test::data::make({ u8"true", u8"TRUE", u8"1", u8"True", u8"false", u8"False", u8"FALSE", u8"0", u8"fa lse", u8"_false", u8" false ", u8"FalSe", u8"fal" })
            ^ boost::unit_test::data::make({ true, true, true, true, false, false, false, false, false, false, false, false, false }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string::u8_cast<bool>(in_str));
        };

        BOOST_DATA_TEST_CASE(stdstring_to_bool_incorrect,
            boost::unit_test::data::make({ u8" true ", u8"TRue", u8" 1 " })
            ^ boost::unit_test::data::make({ true, true, true }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string::u8_cast<bool>(in_str));
        };

        BOOST_DATA_TEST_CASE(bool_to_stdstring_correct,
            boost::unit_test::data::make({ true, false })
            ^ boost::unit_test::data::make({ u8"true", u8"false" }),
            in_val, out_str
        ) {
            BOOST_TEST(out_str == uns::string::u8_cast<std::u8string>(in_val));
        };

        BOOST_DATA_TEST_CASE(forward_identical_cast,
            boost::unit_test::data::make({ true, false }),
            the_val
        ) {
            BOOST_TEST(the_val == uns::string::u8_cast<bool>(uns::string::u8_cast<std::u8string>(the_val)));
        };

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make({ u8"true", u8"false"}),
            the_str
        ) {
            BOOST_TEST(the_str == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<bool>(the_str)));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(integers)

        BOOST_AUTO_TEST_SUITE(decimals)

            BOOST_DATA_TEST_CASE(stdstring_to_dec_correct,
                boost::unit_test::data::make({ u8"0", u8"1", u8"10423", u8"-88", u8"789", u8"22304568 ", u8"-4444 ", u8"0", u8" 0", u8"1 ", u8"10_423", u8"8-8", u8"7J9", u8"2230 4568 ", u8"-44 44 ", u8" " })
                ^ boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444, 0, 0, 1, 10, 8, 7, 2230, -44, 0 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(in_str));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_dec_incorrect,
                boost::unit_test::data::make({ u8"T1", u8"yhi" })
                ^ boost::unit_test::data::make({ 1, 1 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<int>(in_str));
            };

            BOOST_DATA_TEST_CASE(dec_to_stdstring_correct,
                boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444 })
                ^ boost::unit_test::data::make({ u8"0", u8"1", u8"10423", u8"-88", u8"789", u8"22304568", u8"-4444" }),
                in_val, out_str
            ) {
                auto returned = uns::string::u8_cast<std::u8string>(in_val);
                BOOST_TEST(
                    out_str == returned,
                    std::u8string(u8"returned = ") + returned
                );
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make({ 0LL, 1LL, 10423LL, -88LL, 789LL, 22304568LL, -4444LL }),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string::u8_cast<long long int>(uns::string::u8_cast<std::u8string>(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make({ u8"0", u8"1", u8"10423", u8"-88", u8"789", u8"22304568", u8"-4444" }),
                the_str
            ) {
                BOOST_TEST(the_str == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<long long int>(the_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

        BOOST_AUTO_TEST_SUITE(hexadecimals)

            BOOST_DATA_TEST_CASE(stdstring_to_hex_correct,
                boost::unit_test::data::make({ u8"0x3", u8"0X12", u8"0x1A", u8"0x-1B", u8"0x1C", u8"-0x1D ", u8"0x1E ", u8" 0x1F", u8"A8", u8"C0", u8"A5", u8"0xA1", u8"-FF" })
                ^ boost::unit_test::data::make({ 3,     18,       26,       -27,       28,       -29,        30,        31,        168,    192,    165,    161,      -255 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(in_str));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_hex_incorrect,
                boost::unit_test::data::make({ u8"0x 3", u8"X12", u8"1A", u8"0xK" })
                ^ boost::unit_test::data::make({ 3,      18,      26,     27   }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<int>(in_str));
            };

        BOOST_AUTO_TEST_SUITE_END();

        BOOST_AUTO_TEST_SUITE(binaries)

            BOOST_DATA_TEST_CASE(stdstring_to_bin_correct,
                boost::unit_test::data::make({ u8"0b11", u8"0B10010", u8"0b-111", u8" 0b1000", u8" -0b1000" })
                ^ boost::unit_test::data::make({ 3,      18,          -7,         8,           -8 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(in_str));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_bin_incorrect,
                boost::unit_test::data::make({ u8"0b 11", u8"B10", u8"- 0b1", u8"0b2" })
                ^ boost::unit_test::data::make({ 3,       2,       -1,        2 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<int>(in_str));
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(floating_points)

        BOOST_AUTO_TEST_SUITE(doubles)

            BOOST_DATA_TEST_CASE(stdstring_to_dbl_correct,
                boost::unit_test::data::make({ u8"0.0", u8"1.012", u8"10.423", u8"-8.8", u8"7.8900e-10", u8"10.01234567890123456", u8".7", u8"452", u8"1744." })
                ^ boost::unit_test::data::make({ 0.0,   1.012,     10.423,     -8.8,     789.0e-12,      10.012345678901234,       0.7,    452.0,   1744.0 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<double>(in_str));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_dbl_incorrect,
                boost::unit_test::data::make({ u8"1. 012", u8"- 8.8" })
                ^ boost::unit_test::data::make({ 1.012,    -8.8 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<double>(in_str));
            };
            
            BOOST_DATA_TEST_CASE(dbl_to_stdstring_correct,
                boost::unit_test::data::make({   0.0,   1.012,     10.423,     -8.8,     789.0e-12,    10.012345678901234,     0.7,     452.0,   1744.0 })
                ^ boost::unit_test::data::make({ u8"0", u8"1.012", u8"10.423", u8"-8.8", u8"7.89e-10", u8"10.012345678901234", u8"0.7", u8"452", u8"1744" }),
                in_val, out_str
            ) {
                auto returned = uns::string::u8_cast<std::u8string>(in_val);
                BOOST_TEST(
                    out_str == returned,
                    std::u8string(u8"returned = ") + returned
                );
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make({ 0.0, 1.012, 10.423, -8.8, 789.0e-12, 10.012345678901234, 0.7, 452.0, 1744.0 }),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string::u8_cast<double>(uns::string::u8_cast<std::u8string>(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make({ u8"0", u8"1.012", u8"10.423", u8"-8.8", u8"7.89e-10", u8"10.012345678901234", u8"0.7", u8"452", u8"1744" }),
                the_str
            ) {
                BOOST_TEST(the_str == uns::string::u8_cast<std::u8string>(uns::string::u8_cast<double>(the_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

