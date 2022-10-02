
#include <iostream>
#include <chrono>

#include "uniself/time.h"
/*
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

BOOST_AUTO_TEST_SUITE(abs_test)

    BOOST_DATA_TEST_CASE(correct,
        boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw * 0.5))
        + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)),
        val
    ) {
        BOOST_TEST(val == uns::math::abs(val));

        BOOST_TEST(val == uns::math::abs(-val));
    };

BOOST_AUTO_TEST_SUITE_END();*/

int main() {
    std::cout << "START\n";

    auto time_point = std::chrono::utc_clock::now();

    auto dateinfo = uns::time::date(time_point);

    std::cout << "FINISH\n";
};