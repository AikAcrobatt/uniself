#pragma once

#define UNS_HEADER_BENUM "benum.h"

#include <concepts>
#include <type_traits>

#define BETTER_ENUMS_CONSTEXPR_TO_STRING
#include "better_enums/enum.h"


#ifndef UNS_HEADER_STRINGS
#include "uniself/strings.h"
#endif

//BENUM DECLARATOR
#define UNS_BENUM_DECLARATOR(BENUM_NAME, SPECIFICATION_TYPE, ...)																	        \
	BETTER_ENUM(BENUM_NAME, SPECIFICATION_TYPE, __VA_ARGS__);															                    \
	bool operator==(const BENUM_NAME& arg1, const BENUM_NAME::_enumerated& arg2) { return arg1 == static_cast<BENUM_NAME>(arg2); };			\
	bool operator==(const BENUM_NAME::_enumerated& arg1, const BENUM_NAME& arg2) { return static_cast<BENUM_NAME>(arg1) == arg2; };			\
	bool operator!=(const BENUM_NAME& arg1, const BENUM_NAME::_enumerated& arg2) { return !(arg1 == static_cast<BENUM_NAME>(arg2)); };		\
	bool operator!=(const BENUM_NAME::_enumerated& arg1, const BENUM_NAME& arg2) { return !(static_cast<BENUM_NAME>(arg1) == arg2); };		


namespace uns {

	//BENUM CONCEPT
	template<typename benum_t>
	concept is_benum = requires(benum_t enum_obj) {
		benum_t::_values();
	}
	&& requires(benum_t enum_obj) {
		benum_t::_values().begin();
	}
	&& requires(benum_t enum_obj) {
		benum_t::_values().end();
	}
	&& requires(benum_t enum_obj) {
		benum_t::_values().size();
	}
	&& requires(benum_t enum_obj) {
		benum_t::_enumerated;
	}
	&& requires(benum_t enum_obj) {
		{ enum_obj } -> ::std::convertible_to<typename benum_t::_enumerated>;
	}
	&& requires(benum_t enum_obj) {
		benum_t::_value_iterator;
	}
	&& requires(benum_t enum_obj) {
		enum_obj._to_string();
	}
	&& requires(benum_t enum_obj) {
		benum_t::_from_string("...");
	};


	//BENUM TYPE_TRAITS
	template<typename benum_t>
	class benum_traits : public ::std::integral_constant<bool, ::uns::is_benum<benum_t>> {};
};


namespace uns::string {

	//convertions of benum types with std::u8string
	template<::std::constructible_from<::std::u8string> out_t, ::uns::is_benum in_t>
	out_t u8_cast(const in_t& obj) {
		return ::uns::string::u8_cast<::std::u8string>(obj._to_string());
	};
	template<::uns::is_benum out_t>
	out_t u8_cast(const ::std::u8string_view& obj) {
		return out_t::_from_string(::uns::string::u8_cast<::std::string>(obj).c_str());
	};
};