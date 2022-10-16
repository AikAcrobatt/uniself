#pragma once

#include <concepts>

namespace uns {

	template<typename from_t, typename to_t>
	concept constructible_to = std::constructible_from<from_t, to_t>;

	template<typename collection_t, typename value_t>
	concept collection = 
		std::same_as<value_t, typename collection_t::value_type>
		&& requires (collection_t collection) {
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

	template<typename collection_t, typename target_convertion_t>
	concept collection_of_constructibles_from =
		uns::collection<collection_t, typename collection_t::value_type> && std::constructible_from<typename collection_t::value_type, target_convertion_t>;

	template<typename collection_t, typename target_convertion_t>
	concept const_collection_of_constructibles_from =
		uns::const_collection<collection_t, typename collection_t::value_type> && std::constructible_from<typename collection_t::value_type, target_convertion_t>;

	template<typename collection_t, typename target_convertion_t>
	concept collection_of_constructibles_to =
		uns::collection<collection_t, typename collection_t::value_type> && uns::constructible_to<typename collection_t::value_type, target_convertion_t>;

	template<typename collection_t, typename target_convertion_t>
	concept const_collection_of_constructibles_to =
		uns::const_collection<collection_t, typename collection_t::value_type> && uns::constructible_to<typename collection_t::value_type, target_convertion_t>;


};