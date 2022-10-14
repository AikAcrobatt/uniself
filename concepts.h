#pragma once

#include <concepts>

namespace uns {

	template<typename collection_t, typename value_t>
	concept collection = requires (collection_t collection) {
		collection.begin();
		collection.end();
		{ *(collection.begin()) } -> std::convertible_to<value_t>;
		collection.begin()++;
		{ collection.size() } -> std::integral;
	};

	template<typename collection_t, typename value_t>
	concept const_collection =
		collection<collection_t, value_t>
		&& requires (collection_t collection) {
			collection.cbegin();
			collection.cend();
			{ *(collection.cbegin()) } -> std::convertible_to<value_t>;
			collection.cbegin()++;
			{ collection.size() } -> std::integral;
		};

	template<typename collection_t, typename target_convertion_t>
	concept collection_of_convertibles_to =
		uns::collection<collection_t, typename collection_t::value_type> && std::convertible_to<typename collection_t::value_type, target_convertion_t>;

	template<typename collection_t, typename target_convertion_t>
	concept const_collection_of_convertibles_to =
		uns::const_collection<collection_t, typename collection_t::value_type> && std::convertible_to<typename collection_t::value_type, target_convertion_t>;

};