
#include <iostream>
#include <limits>

#include "uniself/math.h"
#include "uniself/matrix.h"

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

BOOST_AUTO_TEST_SUITE_END();

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

BOOST_AUTO_TEST_SUITE(more_test)

    BOOST_DATA_TEST_CASE(correct,
        (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)))
        * (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L))),
        val1, val2
    ) {
        auto correct_result = !uns::math::equals(val1, val2) && val1 > val2;
        BOOST_TEST(correct_result == uns::math::more(val1, val2));
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(less_test)

    BOOST_DATA_TEST_CASE(correct,
        (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)))
        * (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L))),
        val1, val2
    ) {
        auto correct_result = !uns::math::equals(val1, val2) && val1 < val2;
        BOOST_TEST(correct_result == uns::math::less(val1, val2));
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(moreeq_test)

    BOOST_DATA_TEST_CASE(correct,
        (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)))
        * (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L))),
        val1, val2
    ) {
        auto correct_result = (!uns::math::equals(val1, val2) && val1 > val2) || uns::math::equals(val1, val2);
        BOOST_TEST(correct_result == uns::math::moreeq(val1, val2));
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(lesseq_test)

    BOOST_DATA_TEST_CASE(correct,
        (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)))
        * (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L))),
        val1, val2
    ) {
        auto correct_result = (!uns::math::equals(val1, val2) && val1 < val2) || uns::math::equals(val1, val2);
        BOOST_TEST(correct_result == uns::math::lesseq(val1, val2));
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(sign_test)

    BOOST_DATA_TEST_CASE(correct,
        boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
        + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
        + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)),
        val
    ) {
        auto correct_result = (uns::math::equals(val, 0.0L) ? 0 : (uns::math::more(val, 0.0L) ? 1 : -1));
        BOOST_TEST(correct_result == uns::math::sign(val));
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(div_test)

    BOOST_DATA_TEST_CASE(correct1,
        (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L)))
        * (boost::unit_test::data::xrange(-0.1L, (boost::unit_test::data::begin = -10.0L, boost::unit_test::data::step = 0.12399L))
            + boost::unit_test::data::xrange(10.0L * minw, (boost::unit_test::data::begin = 0.0L, boost::unit_test::data::step = minw))
            + boost::unit_test::data::xrange(10.0L, (boost::unit_test::data::begin = 11.0L * minw, boost::unit_test::data::step = 0.12399L))),
        val1, val2
    ) {
        auto divide_to_zero_result_value = 777.777L;
        auto correct_result = static_cast<long double>(uns::math::equals(val2, 0.0L) ? uns::math::sign(val1) * divide_to_zero_result_value : val1 / val2);
        auto div_res = correct_result;
        auto res = true;
        BOOST_TEST(res = uns::math::equals(correct_result, div_res = uns::math::div(val1, val2, divide_to_zero_result_value)));

        if(!res) {
            std::cout << "correct_result = " << correct_result << "\n";
            std::cout << "div_res = " << div_res << "\n";
        };
    };

    BOOST_DATA_TEST_CASE(correct2,
        boost::unit_test::data::xrange(100, (boost::unit_test::data::begin = -100, boost::unit_test::data::step = 1))
        * boost::unit_test::data::xrange(100, (boost::unit_test::data::begin = -100, boost::unit_test::data::step = 1)),
        val1, val2
    ) {
        auto divide_to_zero_result_value = 777;
        auto correct_result = static_cast<int>(val2 == 0 ? uns::math::sign(val1) * divide_to_zero_result_value : val1 / val2);
        auto div_res = correct_result;
        auto res = true;
        BOOST_TEST(res = uns::math::equals(correct_result, div_res = uns::math::div(val1, val2, divide_to_zero_result_value)));

        if(!res) {
            std::cout << "correct_result = " << correct_result << "\n";
            std::cout << "div_res = " << div_res << "\n";
        };
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(matrix_str_test)

    BOOST_AUTO_TEST_CASE(correct1) {
        
        auto mtx1 = uns::math::matrix<long double, ublas::row_major, std::vector<long double>>(3, 4);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                mtx1(i1, i2) = i1 * 10 + i2;

        auto str = uns::string::u8_cast<std::u8string>(mtx1);
        auto mtx2 = uns::string::u8_cast<uns::math::matrix<long double, ublas::row_major, std::vector<long double>>>(str);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                BOOST_TEST(mtx1(i1, i2) == mtx2(i1, i2));
    };

BOOST_AUTO_TEST_SUITE_END();