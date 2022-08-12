

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#include "strings.h"

#define BOOST_TEST_MODULE uns::strings::test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>


BOOST_AUTO_TEST_SUITE(string_cast_testing_bool)

    BOOST_DATA_TEST_CASE(string_cast_stdstring_to_bool_correct,
        boost::unit_test::data::make({ "true", "TRUE", "1", "True", "false", "False", "FALSE", "0", "fa lse", "_false", " false ", "FalSe", "fal" })
        ^ boost::unit_test::data::make({ true, true, true, true, false, false, false, false, false, false, false, false, false }),
        in_str, out_val
    ) {
        BOOST_TEST(out_val == uns::string_cast<bool>(static_cast<const char*>(in_str)));
    };

    BOOST_DATA_TEST_CASE(string_cast_stdstring_to_bool_incorrect,
        boost::unit_test::data::make({ " true ", "TRue", " 1 " })
        ^ boost::unit_test::data::make({ true, true, true }),
        in_str, out_val
    ) {
        BOOST_TEST(out_val != uns::string_cast<bool>(static_cast<const char*>(in_str)));
    };

BOOST_AUTO_TEST_SUITE_END();


BOOST_AUTO_TEST_SUITE(string_cast_testing_integers)

    BOOST_AUTO_TEST_SUITE(string_cast_testing_decimals)

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_correct,
            boost::unit_test::data::make({ "0", "1", "10423", "-88", "789", "22304568 ", "-4444 ", "0", " 0", "1 ", "10_423", "8-8", "7J9", "2230 4568 ", "-44 44 ", " " })
            ^ boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444, 0, 0, 1, 10, 8, 7, 2230, -44, 0 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_incorrect,
            boost::unit_test::data::make({ "T1", "yhi" })
            ^ boost::unit_test::data::make({ 1, 1 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(string_cast_testing_decimals)

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_dec_correct,
            boost::unit_test::data::make({ "0", "1", "10423", "-88", "789", "22304568 ", "-4444 ", "0", " 0", "1 ", "10_423", "8-8", "7J9", "2230 4568 ", "-44 44 ", " " })
            ^ boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444, 0, 0, 1, 10, 8, 7, 2230, -44, 0 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_dec_incorrect,
            boost::unit_test::data::make({ "T1", "yhi" })
            ^ boost::unit_test::data::make({ 1, 1 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(string_cast_testing_hexadecimals)

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_hex_correct,
            boost::unit_test::data::make({ "0x3", "0X12", "0x1A", "0x-1B", "0x1C", "-0x1D ", "0x1E ", " 0x1F", "A8", "C0", "A5", "0xA1", "-FF" })
            ^ boost::unit_test::data::make({ 3,    18,     26,     -27,     28,     -29,      30,      31,     168,  192,  165,  161, -255 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_hex_incorrect,
            boost::unit_test::data::make({ "0x 3", "X12", "1A", "0xK" })
            ^ boost::unit_test::data::make({ 3,      18,     26,   27   }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(string_cast_testing_binary)

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_bin_correct,
            boost::unit_test::data::make({ "0b11", "0B10010", "0b-111", " 0b1000", " -0b1000" })
            ^ boost::unit_test::data::make({ 3, 18, -7, 8, -8 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

        BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_bin_incorrect,
            boost::unit_test::data::make({ "0b 11", "B10", "- 0b1", "0b2" })
            ^ boost::unit_test::data::make({ 3, 2, -1, 2 }),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string_cast<int>(static_cast<std::string>(in_str)));
        };

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();