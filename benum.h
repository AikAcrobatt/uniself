#pragma once

#define UNS_HEADER_BENUM "benum.h"

#include <type_traits>
#pragma warning(disable: 4626; disable: 4820; disable: 5027)
#include "better_enums/enum.h"
#pragma warning(default: 4626; default: 4820; default: 5027)


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
	concept benum = requires(benum_t enum_obj) {
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
		{ enum_obj } -> std::convertible_to<typename benum_t::_enumerated>;
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
	class is_benum : public std::integral_constant<bool, uns::benum<benum_t>> {};
};