
#include <iostream>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include "boost/property_tree/json_parser.hpp"

#include "uniself/neuronetworks.h"
/*
#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns_neuronetworks
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

#pragma warning(disable: 4514)

BOOST_AUTO_TEST_SUITE(neuronetworks_str_test)

    BOOST_AUTO_TEST_CASE(correct1) {

        BOOST_TEST(true);
    };

BOOST_AUTO_TEST_SUITE_END();
*/


int main() {
    std::cout << "START\n";

    auto repr = boost::property_tree::ptree{};
    repr.put("type", "Line.Perc");
    repr.put("R", 1.2056);
    repr.put("C", -0.0701);

    repr.put("links.total", 3);

    repr.put("links._0.layer", -1);
    repr.put("links._0.index", 0);
    repr.put("links._0.weight", 8.047);

    repr.put("links._1.layer", -1);
    repr.put("links._1.index", 1);
    repr.put("links._1.weight", 5.16);

    repr.put("links._2.layer", -1);
    repr.put("links._2.index", 2);
    repr.put("links._2.weight", -0.01);

    repr.put("params.total", 3);

    repr.put("params._0", 5.0);
    repr.put("params._1", 6.0);

    auto file = std::fstream("G:/Visual Studio/uniself/neuronetworks/sequential_neuron.json", std::ios::out);

    boost::property_tree::write_json(file, repr);

    std::cout << "FINISH\n";
};