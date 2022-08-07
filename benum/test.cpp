

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns::benum::test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

#pragma warning(disable: 4514)
#include "benum.h"

namespace abyss {

    UNS_BENUM_DECLARATOR(layers, int,
        l0 = 0,
        l1 = 1,
        l2 = 2,
        l3 = 3,
        l4 = 4,
        l5 = 5,
        l6 = 6,
        l7 = 7,
        l8 = 8,
        l9 = 9
    );

};
#pragma warning(default: 4514)

using types_list = boost::mpl::list<abyss::layers, void, int, char, std::string>;
BOOST_AUTO_TEST_CASE_TEMPLATE(is_benum_test, benum_candidate_t, types_list) {
    if(typeid(benum_candidate_t) == typeid(abyss::layers)) {
        BOOST_TEST(uns::is_benum<benum_candidate_t>::value);
    }
    else {
        BOOST_TEST(!uns::is_benum<benum_candidate_t>::value);
    };
};


BOOST_DATA_TEST_CASE(benum_equality_operators_test,
    boost::unit_test::data::make({
        static_cast<abyss::layers>(abyss::layers::l0),
        static_cast<abyss::layers>(abyss::layers::l1),
        static_cast<abyss::layers>(abyss::layers::l2),
        static_cast<abyss::layers>(abyss::layers::l3),
        static_cast<abyss::layers>(abyss::layers::l4),
        static_cast<abyss::layers>(abyss::layers::l5),
        static_cast<abyss::layers>(abyss::layers::l6),
        static_cast<abyss::layers>(abyss::layers::l7),
        static_cast<abyss::layers>(abyss::layers::l8),
        static_cast<abyss::layers>(abyss::layers::l9)
    })
    * boost::unit_test::data::make({
        abyss::layers::l0,
        abyss::layers::l1,
        abyss::layers::l2,
        abyss::layers::l3,
        abyss::layers::l4,
        abyss::layers::l5,
        abyss::layers::l6,
        abyss::layers::l7,
        abyss::layers::l8,
        abyss::layers::l9
    }),
    true_benum_obj, benum_id
) {
    if(true_benum_obj == static_cast<abyss::layers>(benum_id)) {
        BOOST_TEST(true_benum_obj == benum_id);
        BOOST_TEST(benum_id == true_benum_obj);
    }
    else {
        BOOST_TEST(true_benum_obj != benum_id);
        BOOST_TEST(benum_id != true_benum_obj);
    };
};