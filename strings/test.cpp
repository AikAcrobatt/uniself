

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#include "strings.h"

/*
#define BOOST_TEST_MODULE uns::strings::test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE(string_cast_testing)

    BOOST_AUTO_TEST_SUITE(u8_strings)

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make({ "ABCD efgh !@#% АБВГдеёжзик_" }),
            the_val
        ) {
            //direct testing results are very significant while testing at file ostreams
            BOOST_TEST(the_val == uns::string_cast<std::string>(uns::string_cast<std::u8string>(the_val)));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(bools)

        BOOST_DATA_TEST_CASE(stdstring_to_bool_correct,
            boost::unit_test::data::make({ "true", "TRUE", "1", "True", "false", "False", "FALSE", "0", "fa lse", "_false", " false ", "FalSe", "fal" })
            ^ boost::unit_test::data::make({ true, true, true, true, false, false, false, false, false, false, false, false, false }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string_cast<bool>(static_cast<const char*>(in_str)));
        };

        BOOST_DATA_TEST_CASE(stdstring_to_bool_incorrect,
            boost::unit_test::data::make({ " true ", "TRue", " 1 " })
            ^ boost::unit_test::data::make({ true, true, true }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string_cast<bool>(static_cast<const char*>(in_str)));
        };

        BOOST_DATA_TEST_CASE(bool_to_stdstring_correct,
            boost::unit_test::data::make({ true, false })
            ^ boost::unit_test::data::make({ "true", "false" }),
            in_val, out_str
        ) {
            BOOST_TEST(out_str == uns::string_cast<std::string>(in_val));
        };

        BOOST_DATA_TEST_CASE(forward_identical_cast,
            boost::unit_test::data::make({ true, false }),
            the_val
        ) {
            BOOST_TEST(the_val == uns::string_cast<bool>(uns::string_cast<std::string>(the_val)));
        };

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make({ "true", "false"}),
            the_str
        ) {
            BOOST_TEST(the_str == uns::string_cast<std::string>(uns::string_cast<bool>(the_str)));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(integers)

        BOOST_AUTO_TEST_SUITE(decimals)

            BOOST_DATA_TEST_CASE(stdstring_to_dec_correct,
                boost::unit_test::data::make({ "0", "1", "10423", "-88", "789", "22304568 ", "-4444 ", "0", " 0", "1 ", "10_423", "8-8", "7J9", "2230 4568 ", "-44 44 ", " " })
                ^ boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444, 0, 0, 1, 10, 8, 7, 2230, -44, 0 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_dec_incorrect,
                boost::unit_test::data::make({ "T1", "yhi" })
                ^ boost::unit_test::data::make({ 1, 1 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(dec_to_stdstring_correct,
                boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444 })
                ^ boost::unit_test::data::make({ "0", "1", "10423", "-88", "789", "22304568", "-4444" }),
                in_val, out_str
            ) {
                auto returned = uns::string_cast<std::string>(in_val);
                BOOST_TEST(
                    out_str == returned,
                    std::string("returned = ") + returned
                );
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make({ 0LL, 1LL, 10423LL, -88LL, 789LL, 22304568LL, -4444LL }),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string_cast<long long int>(uns::string_cast<std::string>(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make({ "0", "1", "10423", "-88", "789", "22304568", "-4444" }),
                the_str
            ) {
                BOOST_TEST(the_str == uns::string_cast<std::string>(uns::string_cast<long long int>(the_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

        BOOST_AUTO_TEST_SUITE(hexadecimals)

            BOOST_DATA_TEST_CASE(stdstring_to_hex_correct,
                boost::unit_test::data::make({ "0x3", "0X12", "0x1A", "0x-1B", "0x1C", "-0x1D ", "0x1E ", " 0x1F", "A8", "C0", "A5", "0xA1", "-FF" })
                ^ boost::unit_test::data::make({ 3,    18,     26,     -27,     28,     -29,      30,      31,     168,  192,  165,  161, -255 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_hex_incorrect,
                boost::unit_test::data::make({ "0x 3", "X12", "1A", "0xK" })
                ^ boost::unit_test::data::make({ 3,      18,     26,   27   }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

        BOOST_AUTO_TEST_SUITE(binaries)

            BOOST_DATA_TEST_CASE(stdstring_to_bin_correct,
                boost::unit_test::data::make({ "0b11", "0B10010", "0b-111", " 0b1000", " -0b1000" })
                ^ boost::unit_test::data::make({ 3, 18, -7, 8, -8 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_bin_incorrect,
                boost::unit_test::data::make({ "0b 11", "B10", "- 0b1", "0b2" })
                ^ boost::unit_test::data::make({ 3, 2, -1, 2 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(floating_points)

        BOOST_AUTO_TEST_SUITE(doubles)

            BOOST_DATA_TEST_CASE(stdstring_to_dbl_correct,
                boost::unit_test::data::make({ "0.0", "1.012", "10.423", "-8.8", "7.8900e-10", "10.01234567890123456", ".7", "452", "1744." })
                ^ boost::unit_test::data::make({ 0.0, 1.012, 10.423, -8.8, 789.0e-12, 10.012345678901234, 0.7, 452.0, 1744.0 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string_cast<double>(static_cast<std::string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_dbl_incorrect,
                boost::unit_test::data::make({ "1. 012", "- 8.8" })
                ^ boost::unit_test::data::make({ 1.012, -8.8 }),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string_cast<double>(static_cast<std::string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(dbl_to_stdstring_correct,
                boost::unit_test::data::make({ 0.0, 1.012, 10.423, -8.8, 789.0e-12, 10.012345678901234, 0.7, 452.0, 1744.0 })
                ^ boost::unit_test::data::make({ "0", "1.012", "10.423", "-8.8", "7.89e-10", "10.012345678901234", "0.7", "452", "1744" }),
                in_val, out_str
            ) {
                auto returned = uns::string_cast<std::string>(in_val);
                BOOST_TEST(
                    out_str == returned,
                    std::string("returned = ") + returned
                );
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make({ 0.0, 1.012, 10.423, -8.8, 789.0e-12, 10.012345678901234, 0.7, 452.0, 1744.0 }),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string_cast<double>(uns::string_cast<std::string>(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make({ "0", "1.012", "10.423", "-8.8", "7.89e-10", "10.012345678901234", "0.7", "452", "1744" }),
                the_str
            ) {
                BOOST_TEST(the_str == uns::string_cast<std::string>(uns::string_cast<double>(the_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();*/

#include <locale>
#include "uns_strings.h"


int main() {
    std::cout << "START" << "\n";

    auto str = uns::string(u8"Это некий текст на русском Яя проба яя! я");

    std::locale::global(std::locale(".utf-8"));
    std::wcout << static_cast<std::wstring>(str) << "\n";

    std::cout << "FINISH" << "\n";
};