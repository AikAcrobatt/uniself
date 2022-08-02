#pragma once

#pragma warning(disable: 4626; disable: 4820; disable: 5027)
#include "better_enums/enum.h"
#pragma warning(default: 4626; default: 4820; default: 5027)

//BETTER ENUMS
//a whole-declarator of better_enums
#define UNS_ENUM_DECLARATOR(ENUM_NAME, SPECIFICATION_TYPE, ...)																	            \
	BETTER_ENUM(ENUM_NAME, SPECIFICATION_TYPE, __VA_ARGS__);															                            \
	bool operator==(const ENUM_NAME& arg1, const ENUM_NAME::_enumerated& arg2) { return arg1 == static_cast<ENUM_NAME>(arg2); };			\
	bool operator==(const ENUM_NAME::_enumerated& arg1, const ENUM_NAME& arg2) { return static_cast<ENUM_NAME>(arg1) == arg2; };			\
	bool operator!=(const ENUM_NAME& arg1, const ENUM_NAME::_enumerated& arg2) { return !(arg1 == static_cast<ENUM_NAME>(arg2)); };			\
	bool operator!=(const ENUM_NAME::_enumerated& arg1, const ENUM_NAME& arg2) { return !(static_cast<ENUM_NAME>(arg1) == arg2); };		
