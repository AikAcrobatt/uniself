
#include <iostream>

#include "uniself/math.h"

#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns_math
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

#pragma warning(disable: 4514)



BOOST_AUTO_TEST_SUITE(equals_test)

    BOOST_DATA_TEST_CASE(correct1,
        boost::unit_test::data::xrange(0.1L, (boost::unit_test::data::begin = -0.1L, boost::unit_test::data::step = 0.001L))
        * boost::unit_test::data::xrange(0.1L, (boost::unit_test::data::begin = -0.1L, boost::unit_test::data::step = 0.001L)),
        val1, val2
    ) {
        BOOST_TEST(uns::math::equals(val1, val2) == (uns::math::abs(val1 - val2) < 0.000000001));
    };

    BOOST_DATA_TEST_CASE(correct2,
        boost::unit_test::data::xrange(3.1L, (boost::unit_test::data::begin = 2.9L, boost::unit_test::data::step = 0.001L))
        * boost::unit_test::data::xrange(10, (boost::unit_test::data::begin = -10, boost::unit_test::data::step = 1)),
        val1, val2
    ) {
        BOOST_TEST(uns::math::equals(val1, val2) == (uns::math::abs(val1 - static_cast<long double>(val2)) < 0.000000001));
    };

BOOST_AUTO_TEST_SUITE_END();