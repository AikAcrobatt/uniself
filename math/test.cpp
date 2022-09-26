
#include <iostream>
#include <limits>

#include "uniself/math.h"

#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns_math
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

#pragma warning(disable: 4514)

#undef min
auto minw = std::numeric_limits<long double>::min();
constexpr auto eq_modifier = 1.0e-14L;

BOOST_AUTO_TEST_SUITE(equals_test)

    BOOST_DATA_TEST_CASE(correct1,
        boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = -10.0L * minw, boost::unit_test::data::step = minw))
        * boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = -10.0L * minw, boost::unit_test::data::step = minw)),
        val1, val2
    ) {
        BOOST_TEST(uns::math::equals(val1, val2));
    };

    BOOST_DATA_TEST_CASE(incorrect1,
        (boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = -10.0L * minw, boost::unit_test::data::step = minw))
            * boost::unit_test::data::xrange((-10.0L - 0.01) * minw, (boost::unit_test::data::begin = -100.0L * minw, boost::unit_test::data::step = minw)))
        + (boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = -10.0L * minw, boost::unit_test::data::step = minw))
            * boost::unit_test::data::xrange(100.0L * minw, (boost::unit_test::data::begin = (10.0L + 0.01) * minw, boost::unit_test::data::step = minw))),
        val1, val2
    ) {
        BOOST_TEST(!uns::math::equals(val1, val2));

        BOOST_TEST(!uns::math::equals(val2, val1));
    };

    BOOST_DATA_TEST_CASE(correct2,
        boost::unit_test::data::xrange(1000.0L, (boost::unit_test::data::begin = 1.0L, boost::unit_test::data::step = 1.0L))
        * boost::unit_test::data::xrange(2.0L * eq_modifier, (boost::unit_test::data::begin = -2.05L * eq_modifier, boost::unit_test::data::step = 0.1 * eq_modifier)),
        val1, mult
    ) {
        auto case_flag = (mult < 2.0L * eq_modifier / (1.0L - eq_modifier)) && (mult > -2.0L * eq_modifier / (1.0L + eq_modifier));
        BOOST_TEST(
            case_flag == uns::math::equals(val1 * (1 + mult), val1)
        );
    };


BOOST_AUTO_TEST_SUITE_END();