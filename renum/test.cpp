
#include <iostream>

#include "uniself/renum.hpp"
#include "uniself/tests/strings_support.hpp"

#pragma warning(push)
#pragma warning(    \
    disable: 4266;  \
    disable: 4365;  \
    disable: 4371;  \
    disable: 4514;  \
    disable: 4548;  \
    disable: 4625;  \
    disable: 4626;  \
    disable: 4668;  \
    disable: 4820;  \
    disable: 4866;  \
    disable: 5026;  \
    disable: 5027;  \
    disable: 5039;  \
    disable: 5045;  \
    disable: 5262;  \
    disable: 5264;  \
    disable: 6011;  \
    disable: 6031;  \
    disable: 6387;  \
    disable: 6269;  \
    disable: 6335;  \
    disable: 26439; \
    disable: 26451; \
    disable: 26495; \
    disable: 26827  \
)
#define BOOST_TEST_MODULE uns_renum_test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>
#pragma warning(pop)

namespace abyss {

    UNS_RENUM(layers, int,
        (l0, = 0),
        (l1, = 1),
        (l2, = 2),
        (l3, = 3),
        (l4, = 4),
        (l5, = 5),
        (l6, = 6),
        (l7, = 7),
        (l8, = 8),
        (l9, = 9)
    );

};

UNS_RENUM(renum_test, int,
    (t1, = 0),
    (t2, = 10),
    (t3,)
);


BOOST_AUTO_TEST_SUITE(renums_typization_checks)

    using types_list = ::boost::mpl::list<::abyss::layers, void, int, char, ::std::string>;
    BOOST_AUTO_TEST_CASE_TEMPLATE(renum_traits_test, renum_candidate_t, types_list) {
        if (typeid(renum_candidate_t) == typeid(::abyss::layers)) {
            BOOST_TEST(::uns::renum_traits<renum_candidate_t>::value);
        }
        else {
            BOOST_TEST(!::uns::renum_traits<renum_candidate_t>::value);
        };
    };

    BOOST_DATA_TEST_CASE(renum_equality_operators_test,
        ::boost::unit_test::data::make({
            static_cast<::abyss::layers>(::abyss::layers::l0),
            static_cast<::abyss::layers>(::abyss::layers::l1),
            static_cast<::abyss::layers>(::abyss::layers::l2),
            static_cast<::abyss::layers>(::abyss::layers::l3),
            static_cast<::abyss::layers>(::abyss::layers::l4),
            static_cast<::abyss::layers>(::abyss::layers::l5),
            static_cast<::abyss::layers>(::abyss::layers::l6),
            static_cast<::abyss::layers>(::abyss::layers::l7),
            static_cast<::abyss::layers>(::abyss::layers::l8),
            static_cast<::abyss::layers>(::abyss::layers::l9)
        })
        * ::boost::unit_test::data::make({
            ::abyss::layers::l0,
            ::abyss::layers::l1,
            ::abyss::layers::l2,
            ::abyss::layers::l3,
            ::abyss::layers::l4,
            ::abyss::layers::l5,
            ::abyss::layers::l6,
            ::abyss::layers::l7,
            ::abyss::layers::l8,
            ::abyss::layers::l9
        }),
        true_renum_obj, renum_id
    ) {
        if (true_renum_obj == static_cast<::abyss::layers>(renum_id)) {
            BOOST_TEST(true_renum_obj == renum_id);
            BOOST_TEST(renum_id == true_renum_obj);
        }
        else {
            BOOST_TEST(true_renum_obj != renum_id);
            BOOST_TEST(renum_id != true_renum_obj);
        };
    };

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(renums_and_std_u8strings)

    BOOST_DATA_TEST_CASE(std_u8string_to_renum_correct,
        ::boost::unit_test::data::make(
            {
                ::uns::tests::make_u8(u8"t1"),
                ::uns::tests::make_u8(u8"t2"),
                ::uns::tests::make_u8(u8"t3")
            }
        )
        ^ ::boost::unit_test::data::make(
            {
                static_cast<renum_test>(renum_test::t1),
                static_cast<renum_test>(renum_test::t2),
                static_cast<renum_test>(renum_test::t3)
            }
        ),
        in_str, out_val
    ) {
        BOOST_TEST(out_val == renum_test::from_string(in_str));
    };

    BOOST_DATA_TEST_CASE(std_u8string_to_renum_incorrect,
        ::boost::unit_test::data::make(
            {
                ::uns::tests::make_u8(u8" t1"),
                ::uns::tests::make_u8(u8"t 2"),
                ::uns::tests::make_u8(u8"t3 ")
            }
        )
        ^ ::boost::unit_test::data::make(
            {
                static_cast<renum_test>(renum_test::t1),
                static_cast<renum_test>(renum_test::t2),
                static_cast<renum_test>(renum_test::t3)
            }
        ),
        in_str, out_val
    ) {
        BOOST_CHECK_THROW(out_val == renum_test::from_string(in_str), ::std::runtime_error);
    };

    BOOST_DATA_TEST_CASE(renum_to_std_u8string_correct,
        ::boost::unit_test::data::make(
            {
                ::uns::tests::make_u8(u8"t1"),
                ::uns::tests::make_u8(u8"t2"),
                ::uns::tests::make_u8(u8"t3")
            }
        )
        ^ ::boost::unit_test::data::make(
            {
                static_cast<renum_test>(renum_test::t1),
                static_cast<renum_test>(renum_test::t2),
                static_cast<renum_test>(renum_test::t3)
            }
        ),
        out_str, in_val
    ) {
        const ::std::u8string out_str_u8 = out_str;
        BOOST_TEST(out_str_u8 == in_val.to_string());
    };

    BOOST_DATA_TEST_CASE(forward_identical_cast,
        ::boost::unit_test::data::make(
            {
                static_cast<renum_test>(renum_test::t1),
                static_cast<renum_test>(renum_test::t2),
                static_cast<renum_test>(renum_test::t3)
            }
        ),
        the_val
    ) {
        BOOST_TEST(
            the_val == renum_test::from_string(
                the_val.to_string()
            )
        );
    };

    /*BOOST_DATA_TEST_CASE(backward_identical_cast,
        ::boost::unit_test::data::make(
            {
                ::uns::tests::make_u8(u8"t1"),
                ::uns::tests::make_u8(u8"t2"),
                ::uns::tests::make_u8(u8"t3")
            }
        ),
        the_str
    ) {
        BOOST_TEST(
            the_str == renum_test::from_string(the_str).to_string()
        );
    };*/

BOOST_AUTO_TEST_SUITE_END();
