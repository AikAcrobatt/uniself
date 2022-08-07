

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
        l1 = 1
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
    auto l1 = static_cast<abyss::layers>(abyss::layers::l1);
    BOOST_TEST(l1 == abyss::layers::l1);
};


BOOST_DATA_TEST_CASE(benum_equality_test1,
    boost::unit_test::data::make({ static_cast<abyss::layers>(abyss::layers::l0), static_cast<abyss::layers>(abyss::layers::l1) }) ^ boost::unit_test::data::make({ abyss::layers::l0, abyss::layers::l1 }),
    true_benum_obj, benum_id
) {
    BOOST_TEST(true_benum_obj == benum_id);
};