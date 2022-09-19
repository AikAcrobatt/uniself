#pragma once

#include <concepts>

namespace uns {

	template<typename collection_t, typename value_t>
	concept iterable_collection = requires (collection_t collection) {
		collection.begin();
		collection.end();
		{ *(collection.begin()) } -> std::convertible_to<value_t>;
		collection.begin()++;
	};

	template<typename collection_t, typename value_t>
	concept const_iterable_collection =
		iterable_collection<collection_t, value_t>
		&& requires (collection_t collection) {
		collection.cbegin();
		collection.cend();
		{ *(collection.cbegin()) } -> std::convertible_to<value_t>;
		collection.cbegin()++;
	};

};