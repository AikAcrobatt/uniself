
#include <iostream>
#include <limits>

#include "uniself/math.h"
#include "uniself/matrix.h"

#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns_math_linear
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
                    [3 x4] {
	                    {0, 1,2, 3},
	                    { 10, 11, 12, 13}, {20, 21, 22, 23}
                    }
                )^^"
        };
        auto mtx2 = uns::string::u8_cast<uns::math::matrix<int>>(str);

        for(int i1 = 0; i1 < mtx1.size1(); i1++)
            for(int i2 = 0; i2 < mtx1.size2(); i2++)
                BOOST_TEST(mtx1(i1, i2) == mtx2(i1, i2));
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(matrix_equals_test)

    BOOST_AUTO_TEST_CASE(correct1) {
        auto mtx1 = uns::math::matrix<int>{ 3, 3 };
        auto mtx2 = uns::math::matrix<int>{ 3, 3 };

        auto val = 0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val;
                val++;
            };
        };

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect1_1) {
        auto mtx1 = uns::math::matrix<int>{ 3, 3 };
        auto mtx2 = uns::math::matrix<int>{ 3, 2 };


        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect1_2) {
        auto mtx1 = uns::math::matrix<int>{ 3, 3 };
        auto mtx2 = uns::math::matrix<int>{ 3, 3 };

        auto val = 0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val + 1;
                val++;
            };
        };

        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(correct2) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<long double>{ 3, 3 };

        auto val = 0.0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val;
                val++;
            };
        };

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect2_1) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<long double>{ 3, 2 };


        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect2_2) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<long double>{ 3, 3 };

        auto val = 0.0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val + 0.001;
                val++;
            };
        };

        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(correct3) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<int>{ 3, 3 };

        auto val = 0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val;
                val++;
            };
        };

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect3_1) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<int>{ 3, 2 };


        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect3_2) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<int>{ 3, 3 };

        auto val = 0.0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = static_cast<long double>(val) + 0.001;
                mtx2(i1, i2) = val;
                val++;
            };
        };

        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(correct4) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<long double>{ 3, 3 };

        auto val = 0.0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val + 0.001;
                val++;
            };
        };

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2, 0.01)
        );
    };

    BOOST_AUTO_TEST_CASE(incorrect4_1) {
        auto mtx1 = uns::math::matrix<long double>{ 3, 3 };
        auto mtx2 = uns::math::matrix<long double>{ 3, 3 };

        auto val = 0.0;
        for(int i1 = 0; i1 < mtx2.size1(); i1++) {
            for(int i2 = 0; i2 < mtx2.size2(); i2++) {
                mtx1(i1, i2) = val;
                mtx2(i1, i2) = val + 0.002;
                val++;
            };
        };

        BOOST_TEST(
            !uns::math::equals(mtx1, mtx2, 0.001)
        );
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(matrix_basic_ops_test)

    BOOST_AUTO_TEST_CASE(matrix_make_test) {
        auto mtx1 = uns::math::matrix<int>{ 3, 4 };

        for(int i1 = 0; i1 < mtx1.size1(); i1++) {
            for(int i2 = 0; i2 < mtx1.size2(); i2++) {
                mtx1(i1, i2) = 4 * i1 + i2;
            };
        };

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 }
        });

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };
    
    BOOST_AUTO_TEST_CASE(resize_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 1, 1, 1 },
            { 1, 1, 1 },
            { 1, 1, 1 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { 1, 1, 1, 0 },
            { 1, 1, 1, 0 },
            { 1, 1, 1, 0 },
            { 0, 0, 0, 0 }
        });

        uns::math::resize(mtx1, 4, 4, 0);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(fill_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 12, 13, 14, 15 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { -123, -123, -123, -123 },
            { -123, -123, -123, -123 },
            { -123, -123, -123, -123 },
            { -123, -123, -123, -123 }
        });

        uns::math::fill(mtx1, -123);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(identity_mtxs_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 },
            { 0, 0, 0 }
        });

        auto mtx2 = uns::math::make_identity<uns::math::matrix<int>>(4, 3);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(trace_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 12, 13, 14, 15 }
        });

        auto tr = uns::math::trace(mtx1);

        BOOST_TEST(
            tr == 30
        );
    };

    BOOST_AUTO_TEST_CASE(det_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<double>>({
            { 1.0, -8.7, 3.4, 5.5 },
            { -7.0, 2.2, -1.0, 0.0 },
            { 8.8, -1.2, 5.5, 9.4 },
            { -2.2, -4.5, -0.1, -9.9 }
        });

        auto det = uns::math::det(mtx1);

        BOOST_TEST(
            uns::math::equals(det, 2685.6132)
        );
    };

    BOOST_AUTO_TEST_CASE(rank_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<double>>({
            { 1.0, -8.7, 3.4, 5.5 },
            { -7.0, 2.2, -1.0, 0.0 },
            { 8.8, -1.2, 5.5, 9.4 },
            { -2.2, -4.5, -0.1, -9.9 }
        });

        auto rank = uns::math::rank(mtx1);

        BOOST_TEST(
            rank == 4
        );
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(matrix_arithmetic_ops_test)

    BOOST_AUTO_TEST_CASE(sum_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, 1.0, 2.0, 3.0 },
            { 4.0, 5.0, 6.0, 7.0 },
            { 8.0, 9.0, 10.0, 11.0 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, -1.0, -2.0 },
            { -4.0, -5.0, -6.0 },
            { -8.0, -9.0, -10.0 }
        });

        auto mtx3 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, 0.0, 0.0, 3.0 },
            { 0.0, 0.0, 0.0, 7.0 },
            { 0.0, 0.0, 0.0, 11.0 }
        });

        BOOST_TEST(
            uns::math::equals(mtx3, uns::math::sum(mtx1, mtx2))
        );
    };

    BOOST_AUTO_TEST_CASE(sub_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, 1.0, 2.0, 3.0 },
            { 4.0, 5.0, 6.0, 7.0 },
            { 8.0, 9.0, 10.0, 11.0 },
            { 12.0, 13.0, 14.0, 15.0 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, 1.0, 2.0, 3.0 },
            { 4.0, 5.0, 6.0, 7.0 },
            { 8.0, 9.0, 10.0, 11.0 }
        });

        auto mtx3 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, 0.0, 0.0, 0.0 },
            { 0.0, 0.0, 0.0, 0.0 },
            { 0.0, 0.0, 0.0, 0.0 },
            { 12.0, 13.0, 14.0, 15.0 }
        });

        BOOST_TEST(
            uns::math::equals(mtx3, uns::math::sub(mtx1, mtx2))
        );
    };

    BOOST_AUTO_TEST_CASE(prod_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<double>>({
            { 1.0, -8.7, 3.4, 5.5 },
            { -7.0, 2.2, -1.0, 0.0 },
            { 8.8, -1.2, 5.5, 9.4 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<double>>({
            { 0.0, 1.2, 2.3, 4.4 },
            { -2.2, -4.5, -0.1, -9.9 },
            { 2.2, 3.3, 5.2, -1.9 }
        });

        auto mtx3 = uns::math::make<uns::math::matrix<double>>({
            { 26.62, 51.57, 20.85, 84.07 },
            { -7.04, -21.6, -21.52, -50.68 },
            { 14.74, 34.11, 48.96, 40.15 }
        });

        BOOST_TEST(
            uns::math::equals(mtx3, uns::math::prod(mtx1, mtx2))
        );
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(transformations_test)

    BOOST_AUTO_TEST_CASE(copy1_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 12, 13, 14, 15 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 4, 5, 6, 7 }
        });

        uns::math::copy1(mtx1, 1, 3);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(swap1_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 12, 13, 14, 15 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 12, 13, 14, 15 },
            { 8, 9, 10, 11 },
            { 4, 5, 6, 7 }
        });

        uns::math::swap1(mtx1, 1, 3);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(copy2_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 12, 13, 14, 15 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 1 },
            { 4, 5, 6, 5 },
            { 8, 9, 10, 9 },
            { 12, 13, 14, 13 }
        });

        uns::math::copy2(mtx1, 1, 3);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

    BOOST_AUTO_TEST_CASE(swap2_test) {
        auto mtx1 = uns::math::make<uns::math::matrix<int>>({
            { 0, 1, 2, 3 },
            { 4, 5, 6, 7 },
            { 8, 9, 10, 11 },
            { 12, 13, 14, 15 }
        });

        auto mtx2 = uns::math::make<uns::math::matrix<int>>({
            { 0, 3, 2, 1 },
            { 4, 7, 6, 5 },
            { 8, 11, 10, 9 },
            { 12, 15, 14, 13 }
        });

        uns::math::swap2(mtx1, 1, 3);

        BOOST_TEST(
            uns::math::equals(mtx1, mtx2)
        );
    };

BOOST_AUTO_TEST_SUITE_END();