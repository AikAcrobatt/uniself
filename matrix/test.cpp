
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


BOOST_AUTO_TEST_SUITE(matrix_str_test)

    BOOST_AUTO_TEST_CASE(correct1) {

        auto mtx1 = uns::math::matrix<int>(3, 4);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                mtx1(i1, i2) = i1 * 10 + i2;

        auto str = uns::string::u8_cast<std::u8string>(mtx1);
        auto mtx2 = uns::string::u8_cast<uns::math::matrix<int>>(str);

        BOOST_TEST(mtx1.size1() == mtx2.size1());
        BOOST_TEST(mtx1.size2() == mtx2.size2());

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                BOOST_TEST(mtx1(i1, i2) == mtx2(i1, i2));
    };

    BOOST_AUTO_TEST_CASE(correct2) {

        auto mtx1 = uns::math::matrix<double>(3, 4);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                mtx1(i1, i2) = i1 * 10 + i2;

        auto str = uns::string::u8_cast<std::u8string>(mtx1);
        auto mtx2 = uns::string::u8_cast<uns::math::matrix<double>>(str);

        BOOST_TEST(mtx1.size1() == mtx2.size1());
        BOOST_TEST(mtx1.size2() == mtx2.size2());

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                BOOST_TEST(uns::math::equals(mtx1(i1, i2), mtx2(i1, i2)));
    };

    BOOST_AUTO_TEST_CASE(correct3) {
        auto mtx1 = uns::math::matrix<int>(3, 4);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                mtx1(i1, i2) = i1 * 10 + i2;

        auto str = std::u8string{
            u8R"^^(
                    [3 x 4] {
	                    {0, 1, 2, 3},
	                    { 10, 11, 12, 13},
	                    {20, 21, 22, 23}
                    }
                )^^"
        };
        auto mtx2 = uns::string::u8_cast<uns::math::matrix<int>>(str);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                BOOST_TEST(mtx1(i1, i2) == mtx2(i1, i2));
    };

BOOST_AUTO_TEST_SUITE_END();