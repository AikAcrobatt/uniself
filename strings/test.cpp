

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


BOOST_AUTO_TEST_SUITE(string_cast_testing_numerics)

BOOST_AUTO_TEST_SUITE(string_cast_testing_decimals)

BOOST_DATA_TEST_CASE(string_cast_stdstring_to_int_correct,
    boost::unit_test::data::make({ "0", "1", "10423", "-88", "789", "22304568 ", "-4444 ", "0" })
    ^ boost::unit_test::data::make({ 0, 1, 10423, -88, 789, 22304568, -4444, 0 }),
    in_str, out_val
) {
    BOOST_TEST(out_val == uns::string_cast<int>(static_cast<std::string>(in_str)));
};

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();