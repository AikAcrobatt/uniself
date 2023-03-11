
#include <exception>
#include <stdexcept>

#include "uniself/strings.hpp"

namespace uns::string {


	//CAST FUNCTIONS FOR NUMERICS TO FORMATTED STRING
	//cast of integer to string contain it's hexadecimal representation
	::std::u8string hex_cast(long long int val) {
		auto res = ::std::string(64, '\0');

		auto* begin = &(*res.begin());
		auto* end = &res.back();

		if(val >= 0) {
			auto conv = ::std::to_chars(begin, end, val, 16);
			if(conv.ec == ::std::errc()) {
				res = "0x" + res;
				return ::std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw ::std::runtime_error("An input value can't be converted to string");
		}
		else {
			auto conv = ::std::to_chars(begin, end, -val, 16);
			if(conv.ec == ::std::errc()) {
				res = "-0x" + res;
				return ::std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw ::std::runtime_error("An input value can't be converted to string");
		};
	};

	//cast of integer to string contain it's binary representation
	::std::u8string bin_cast(long long int val) {
		auto res = ::std::string(128, '\0');

		auto* begin = &(*res.begin());
		auto* end = &res.back();

		if(val >= 0) {
			auto conv = ::std::to_chars(begin, end, val, 2);
			if(conv.ec == ::std::errc()) {
				res = "0b" + res;
				return ::std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw ::std::runtime_error("An input value can't be converted to string");
		}
		else {
			auto conv = ::std::to_chars(begin, end, -val, 2);
			if(conv.ec == ::std::errc()) {
				res = "-0b" + res;
				return ::std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw ::std::runtime_error("An input value can't be converted to string");
		};
	};

};