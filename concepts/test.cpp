

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <tuple>

#include "uniself/concepts.h"


#pragma warning(push)
#pragma warning(disable: 4668; disable: 4365)

#define BOOST_TEST_MODULE uns_concepts
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

#pragma warning(pop)

template<typename elem_t, ::uns::const_iterable_collection<elem_t> collection_t>
bool checker_const_iter(collection_t&&) {
    return true;
};
template<typename elem_t, typename non_collection_t>
bool checker_const_iter(non_collection_t&&) {
    return false;
};

template<typename elem_t, ::uns::iterable_collection<elem_t> collection_t>
bool checker_iter(collection_t&&) {
    return true;
};
template<typename elem_t, typename non_collection_t>
bool checker_iter(non_collection_t&&) {
    return false;
};

template<typename elem_t>
class nonconst_vector : public ::std::vector<elem_t> {
protected:
    typename ::std::vector<elem_t>::const_iterator cbegin() const noexcept { return ::std::vector<elem_t>::cbegin(); };
    typename ::std::vector<elem_t>::const_iterator cend() const noexcept { return ::std::vector<elem_t>::cend(); };
public:
    typename ::std::vector<elem_t>::iterator begin()  noexcept { return ::std::vector<elem_t>::begin(); };
    typename ::std::vector<elem_t>::iterator end()  noexcept { return ::std::vector<elem_t>::end(); };
    ::std::size_t size() const noexcept { return ::std::vector<elem_t>::size(); };
};

template<typename elem_t>
class const_vector : public ::std::vector<elem_t> {
public:
    typename ::std::vector<elem_t>::const_iterator cbegin() const noexcept { return ::std::vector<elem_t>::cbegin(); };
    typename ::std::vector<elem_t>::const_iterator cend() const noexcept { return ::std::vector<elem_t>::cend(); };

    typename ::std::vector<elem_t>::iterator begin() noexcept { return ::std::vector<elem_t>::begin(); };
    typename ::std::vector<elem_t>::iterator end() noexcept { return ::std::vector<elem_t>::end(); };
    ::std::size_t size() const noexcept { return ::std::vector<elem_t>::size(); };
};

BOOST_AUTO_TEST_SUITE(iterable_collections)

    using types_list1 = ::boost::mpl::list<::std::vector<char>, ::std::list<char>, ::std::set<char>, ::std::string, const_vector<char>>;
    BOOST_AUTO_TEST_CASE_TEMPLATE(correct1, collection_candidate_t, types_list1) {
        BOOST_TEST(checker_const_iter<char>(collection_candidate_t{}));
    };

    using types_list2 = ::boost::mpl::list<int, double, bool, ::std::tuple<::std::vector<int>, ::std::list<bool>>, ::std::map<::std::string, char>, nonconst_vector<char>>;
    BOOST_AUTO_TEST_CASE_TEMPLATE(incorrect1, collection_candidate_t, types_list2) {
        BOOST_TEST(!checker_const_iter<char>(collection_candidate_t{}));
    };

    using types_list3 = ::boost::mpl::list<::std::vector<char>, ::std::list<char>, ::std::set<char>, ::std::string, nonconst_vector<char>, const_vector<char>>;
    BOOST_AUTO_TEST_CASE_TEMPLATE(correct2, collection_candidate_t, types_list3) {
        BOOST_TEST(checker_iter<char>(collection_candidate_t{}));
    };

    using types_list4 = ::boost::mpl::list<int, double, bool, ::std::tuple<::std::vector<int>, ::std::list<bool>>, ::std::map<::std::string, char>>;
    BOOST_AUTO_TEST_CASE_TEMPLATE(incorrect2, collection_candidate_t, types_list4) {
        BOOST_TEST(!checker_iter<char>(collection_candidate_t{}));
    };

BOOST_AUTO_TEST_SUITE_END();