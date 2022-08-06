

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns::algorithms::test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#pragma warning(disable: 4514)
#include "algorithms.h"

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


template<uns::is_benum benum_t>
class benum_dataset_generator {
public:
    enum {
        arity = 1
    };

    boost::unit_test::data::size_t size() const { return benum_t::_values().size(); };

    benum_t::_value_iterator begin() { return benum_t::_values().begin(); };
    benum_t::_value_iterator end() { return benum_t::_values().begin(); };
};


BOOST_AUTO_TEST_CASE(benum_equality_test0) {
    auto l3 = static_cast<abyss::layers>(abyss::layers::l3);
    BOOST_TEST(l3 == abyss::layers::l3);
};


BOOST_DATA_TEST_CASE(benum_equality_test1,
    benum_dataset_generator<abyss::layers>() ^ boost::unit_test::data::make({ abyss::layers::l0, abyss::layers::l1, abyss::layers::l2, abyss::layers::l3, abyss::layers::l4, abyss::layers::l5, abyss::layers::l6, abyss::layers::l7, abyss::layers::l8, abyss::layers::l9 }),
    //boost::unit_test::data::make({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) ^ boost::unit_test::data::make({ abyss::layers::l0, abyss::layers::l1, abyss::layers::l2, abyss::layers::l3, abyss::layers::l4, abyss::layers::l5, abyss::layers::l6, abyss::layers::l7, abyss::layers::l8, abyss::layers::l9 }),
    //benum_dataset_generator<abyss::layers>(),
    true_benum_obj, benum_id
) {
    BOOST_TEST(true_benum_obj == benum_id);
};