#pragma once

#include <concepts>


#ifndef UNS_LIB_CONCEPTS
#define UNS_LIB_CONCEPTS "concepts.hpp"

namespace uns {

	template<typename collection_t, typename value_t>
	concept iterable_collection = requires (collection_t collection) {
		collection.begin();
		collection.end();
		{ *(collection.begin()) } -> ::std::convertible_to<value_t>;
		collection.begin()++;
		{ collection.size() } -> ::std::integral;
	};

	template<typename collection_t, typename value_t>
	concept const_iterable_collection =
		iterable_collection<collection_t, value_t>
		&& requires (collection_t collection) {
			collection.cbegin();
			collection.cend();
			{ *(collection.cbegin()) } -> ::std::convertible_to<value_t>;
			collection.cbegin()++;
			{ collection.size() } -> ::std::integral;
		};

	template<typename iterator_t, typename referencable_t>
	concept legacy_iterator = requires (iterator_t iter) {
		::std::convertible_to<decltype(*iter), referencable_t>;
		::std::same_as<decltype(++iter), iterator_t&>;
	};
};

#endif