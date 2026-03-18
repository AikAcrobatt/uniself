#pragma once

#ifndef UNS_LIB_CONCEPTS
#define UNS_LIB_CONCEPTS

#include <concepts>
#include <string>

namespace uns {

    //defines types that can be dealed with as like they are of ::std::basic_string type
    template<typename string_t>
    concept is_basic_string = ::std::derived_from<string_t, ::std::basic_string<typename string_t::value_type>>;


    template<typename iterator_t, typename referensable_t>
    concept legacy_iterator = requires (iterator_t iter) {
        ::std::convertible_to<decltype(*iter), referensable_t>;
        ::std::same_as<decltype(++iter), iterator_t&>;
    };


    template<typename collection_t, typename value_t>
    concept iterable_collection = requires (collection_t& collection) {
        collection.begin();
        collection.end();
        ++collection.begin();
        { collection.size() } -> ::std::integral;
    }
    && ::uns::legacy_iterator<typename collection_t::iterator, value_t>
    && ::std::same_as<typename collection_t::value_type, value_t>;


    template<typename collection_t, typename value_t>
    concept const_iterable_collection =
        iterable_collection<collection_t, value_t>
        && requires (const collection_t& collection) {
            collection.cbegin();
            collection.cend();
            ++collection.cbegin();
        }
        && ::uns::legacy_iterator<typename collection_t::const_iterator, value_t>;

};

#endif