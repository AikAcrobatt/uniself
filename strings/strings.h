#pragma once

#include <string>
#include <charconv>
#include <type_traits>
#include <concepts>
#include <exception>

#include "uniself/benum.h"

namespace uns {

	template<typename collection_t, typename value_t>
	concept citerable_collection = requires (collection_t collection) {
		collection.cbegin();
		collection.cend();
		{ *(collection.cbegin()) } -> std::convertible_to<value_t>;
		collection.cbegin()++;
	};

}

namespace uns::test {

	class u8string_wrapper : public std::u8string {
	public:
		template<typename ostream_t>
		friend constexpr ostream_t& operator<<(ostream_t& os, const u8string_wrapper& str) {
			if constexpr(std::is_base_of<std::basic_ostream<char>, ostream_t>::value) {
				return operator<<(os, reinterpret_cast<const char*>(str.c_str()));
			};
		};

		constexpr operator const std::u8string& () const& noexcept { return dynamic_cast<const std::u8string&>(*this); };
		constexpr operator std::u8string& () & noexcept { return dynamic_cast<std::u8string&>(*this); };
		constexpr operator std::u8string_view() const noexcept { return dynamic_cast<const std::u8string&>(*this).operator std::u8string_view(); };
	};

	constexpr uns::test::u8string_wrapper make_u8(const char8_t* cstr) noexcept {
		auto res = uns::test::u8string_wrapper(cstr);
		return uns::test::u8string_wrapper(cstr);
	};
	constexpr uns::test::u8string_wrapper make_u8(const std::u8string& str) noexcept {
		auto res = uns::test::u8string_wrapper(str);
		return static_cast<uns::test::u8string_wrapper>(str);
	};
};


namespace uns::string {

	//SPECIFIC STRING CONCEPTS
	//defines types that can be dealed with as like they are of std::basic_string type
	template<typename string_t>
	concept std_basic = std::derived_from<string_t, std::basic_string<typename string_t::value_type>>;

	//STRING CAST FUNCTIONS
	//convertions of std::u32string
	template<std::constructible_from<std::u16string> out_t>
	out_t u32_cast(const std::u32string_view& from) {
		auto to = std::u16string();
		to.reserve(from.size() * (sizeof(std::u32string::value_type) / sizeof(std::u16string::value_type)));

		for(const auto& val : from) {
			if(val < 0xFFFF && !(val >= 0xD800 && val <= 0xDFFF)) {
				to += static_cast<char16_t>(val);
			}
			else if(val >= 0x10000 && val < 0x10FFFF) {
				to += 0xD800 + static_cast<char16_t>((val - 0x10000) / 0x0400);
				to += 0xDC00 + static_cast<char16_t>((val - 0x10000) % 0x0400);
			};
		};

		to.shrink_to_fit();
		return to;
	};
	template<std::constructible_from<std::wstring> out_t>
	out_t u32_cast(const std::u32string_view& from) {
		if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
			return std::wstring(reinterpret_cast<const wchar_t*>(from.data()));
		}
		else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
			return std::wstring(reinterpret_cast<const wchar_t*>(uns::string::u32_cast<std::u16string>(from).c_str()));
		}
		else {
			throw std::runtime_error("Size of std::wstring_view::value_type is neither 16 bit, nor 32 bit");
		};
	};
	template<std::constructible_from<std::u32string> out_t>
	out_t u32_cast(const std::u16string_view& from) {
		auto to = std::u32string();
		to.reserve(from.size());

		char32_t symbol = 0x0;

		auto is_start = true;
		for(const auto& val : from) {
			if(val >= 0xD800 && val <= 0xDBFF) {
				if(!is_start) {
					to += symbol;
					symbol = 0;
				};
				symbol = (static_cast<char32_t>(val - 0xD800) << 10);
			}
			else if(val >= 0xDC00 && val <= 0xDFFF) {
				symbol += static_cast<char32_t>(val - 0xDC00);
				symbol += 0x10000;
			}
			else {
				if(!is_start) {
					to += symbol;
					symbol = 0;
				};
				symbol = static_cast<char32_t>(val);
			};

			if(is_start)
				is_start = false;
		};

		if(symbol != 0)
			to += symbol;

		to.shrink_to_fit();
		return to;
	};
	template<std::constructible_from<std::u32string> out_t>
	out_t u32_cast(const std::wstring_view& from) {
		if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
			return std::u32string(reinterpret_cast<const char32_t*>(from.data()));
		}
		else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
			return uns::string::u32_cast<std::u32string>(std::u16string(reinterpret_cast<const char16_t*>(from.data())));
		}
		else {
			throw std::runtime_error("Size of std::wstring_view::value_type is neither 16 bit, nor 32 bit");
		};
	};

	//convertions to std::u8string and its products
	template<std::constructible_from<std::u8string> out_t>
	out_t u8_cast(const std::u8string_view& from) {
		return std::u8string(from);
	};
	template<std::constructible_from<std::u8string> out_t>
	out_t u8_cast(const std::string_view& from) {
		return std::u8string(reinterpret_cast<const char8_t*>(from.data()));
	};
	template<std::constructible_from<std::u8string> out_t>
	out_t u8_cast(const std::u32string_view& from) {
		auto to = std::u8string();
		to.reserve(from.size() * (sizeof(std::u32string_view::value_type) / sizeof(std::u8string_view::value_type)));

		for(const auto& val : from) {
			if(val < 0x80) {
				to += static_cast<char8_t>(val);
			}
			else if(val < 0x800) {
				to += 0xC0 + static_cast<char8_t>(val / 0x40);
				to += 0x80 + static_cast<char8_t>(val % 0x40);
			}
			else if(val < 0x10000) {
				to += 0xE0 + static_cast<char8_t>(val / 0x1000);
				to += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
				to += 0x80 + static_cast<char8_t>(val % 0x40);
			}
			else if(val >= 0x10000 && val < 0x110000) {
				to += 0xF0 + static_cast<char8_t>(val / 0x40000);
				to += 0x80 + static_cast<char8_t>((val % 0x40000) / 0x1000);
				to += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
				to += 0x80 + static_cast<char8_t>(val % 0x40);
			};
		};

		to.shrink_to_fit();
		return to;
	};
	template<std::constructible_from<std::u8string> out_t>
	out_t u8_cast(const std::u16string_view& from) {
		return uns::string::u8_cast<std::u8string>(uns::string::u32_cast<std::u32string>(from));
	};
	template<std::constructible_from<std::u8string> out_t>
	out_t u8_cast(const std::wstring_view& from) {
		return uns::string::u8_cast<std::u8string>(uns::string::u32_cast<std::u32string>(from));
	};

	//convertions from std::u8string_view
	template<std::constructible_from<std::string> out_t>
	out_t u8_cast(const std::u8string_view& from) {
		return std::string(reinterpret_cast<const char*>(from.data()));
	};
	template<std::constructible_from<std::u32string> out_t>
	out_t u8_cast(const std::u8string_view& from) {
		auto to = std::u32string();
		to.reserve(from.size());

		char32_t symbol = 0x0;

		auto degree = 1;
		auto is_start = true;
		for(const auto& val : from) {
			if((val & 0x80) == 0) {
				if(!is_start) {
					to += symbol;
					symbol = 0;
				};
				degree = 1;
				symbol = (static_cast<char32_t>(val & 0x7F) << (--degree * 6));
			}
			else if((val & 0xE0) == 0xC0) {
				if(!is_start) {
					to += symbol;
					symbol = 0;
				};
				degree = 2;
				symbol = (static_cast<char32_t>(val & 0x1F) << (--degree * 6));
			}
			else if((val & 0xF0) == 0xE0) {
				if(!is_start) {
					to += symbol;
					symbol = 0;
				};
				degree = 3;
				symbol = (static_cast<char32_t>(val & 0x0F) << (--degree * 6));
			}
			else if((val & 0xF8) == 0xF0) {
				if(!is_start) {
					to += symbol;
					symbol = 0;
				};
				degree = 4;
				symbol = (static_cast<char32_t>(val & 0x07) << (--degree * 6));
			}
			else if((val & 0xC0) == 0x80) {
				symbol += (static_cast<char32_t>(val & 0x3F) << (--degree * 6));
			};

			if(is_start)
				is_start = false;
		};

		if(symbol != 0)
			to += symbol;

		to.shrink_to_fit();
		return to;
	};
	template<std::constructible_from<std::u16string> out_t>
	out_t u8_cast(const std::u8string_view& from) {
		return uns::string::u32_cast<std::u16string>(uns::string::u8_cast<std::u32string>(from));
	};
	template<std::constructible_from<std::wstring> out_t>
	out_t u8_cast(const std::u8string_view& from) {
		return uns::string::u32_cast<std::wstring>(uns::string::u8_cast<std::u32string>(from));
	};

	//convertions of numeric (spreadly) types with std::u8string
	template<std::same_as<bool> out_t>
	out_t u8_cast(const std::u8string_view& str) {
		if(str == u8"true" || str == u8"1" || str == u8"True" || str == u8"TRUE")
			return true;
		else if(str == u8"false" || str == u8"0" || str == u8"False" || str == u8"FALSE")
			return false;
		else
			throw std::runtime_error("An input string can't be converted to bool");
	};
	template<typename out_t>
		requires (std::is_integral<out_t>::value && !std::is_same<out_t, bool>::value)
	out_t u8_cast(const std::u8string_view& str) {
		auto pos_hex = str.find(u8"0x"); if(!(pos_hex >= 0 && pos_hex < str.size())) pos_hex = str.find(u8"0X");
		auto pos_bin = str.find(u8"0b"); if(!(pos_bin >= 0 && pos_bin < str.size())) pos_bin = str.find(u8"0B");
		auto pos_minus = str.find(u8"-");
		auto is_hex = (pos_hex >= 0 && pos_hex < str.size());
		auto is_bin = (pos_bin >= 0 && pos_bin < str.size());
		auto is_negative = (pos_minus >= 0 && pos_minus == (is_bin ? pos_bin : pos_hex) - 1 && pos_minus != std::string_view::npos);

		if(is_hex) {
			pos_hex += 2;
			is_hex = (pos_hex < str.size());
		}
		else
			pos_hex = 0;

		if(is_bin) {
			pos_bin += 2;
			is_bin = (pos_bin < str.size());
		}
		else
			pos_bin = 0;

		if(!is_hex && !is_bin) {
			auto res = out_t(0);
			const char* begin = reinterpret_cast<const char*>(str.data());
			const char* end = &begin[str.size()];

			auto conv = std::from_chars(begin, end, res, 10);
			if(conv.ec == std::errc())
				return res;
			else
				is_hex = true;
		};

		if(is_hex) {
			auto res = out_t(0);
			const char* begin = reinterpret_cast<const char*>(&str.data()[pos_hex]);
			const char* end = &begin[str.size()];

			auto conv = std::from_chars(begin, end, res, 16);
			if(conv.ec == std::errc())
				return res * (is_negative ? -1 : 1);
			else
				is_bin = true;
		};

		if(is_bin) {
			auto res = out_t(0);
			const char* begin = reinterpret_cast<const char*>(&str.data()[pos_bin]);
			const char* end = &begin[str.size()];

			auto conv = std::from_chars(begin, end, res, 2);
			if(conv.ec == std::errc())
				return res * (is_negative ? -1 : 1);
			else
				throw std::runtime_error("An input string can't be converted to integer with base neither decimal, nor hexadecimal and even binary");
		};

		return out_t(0);
	};
	template<std::floating_point out_t>
	out_t u8_cast(const std::u8string_view& str) {
		auto res = out_t(0);
		const char* begin = reinterpret_cast<const char*>(str.data());
		const char* end = &begin[str.size()];

		for(auto format :
			{ 
				std::chars_format::scientific, 
				std::chars_format::general, 
				std::chars_format::fixed 
			}
		) {
			auto conv = std::from_chars(begin, end, res, format);
			if(conv.ec == std::errc())
				return res;
		};

		throw std::runtime_error("An input string can't be converted to floating point");
	};

	template<std::constructible_from<std::u8string> out_t, std::same_as<bool> in_t>
	out_t u8_cast(const in_t& obj) {
		if(static_cast<bool>(obj))
			return std::u8string(u8"true");
		else
			return std::u8string(u8"false");
	};
	template<std::constructible_from<std::u8string> out_t, typename in_t>
		requires (std::is_integral<in_t>::value && !std::is_same<in_t, bool>::value)
	out_t u8_cast(const in_t& obj) {
		auto res = std::string(64, '\0');

		auto* begin = &(*res.begin());
		auto* end = &res.back();

		auto conv = std::to_chars(begin, end, obj, 10);
		if(conv.ec == std::errc())
			return std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
		else
			throw std::runtime_error("An input value can't be converted to string");
	};
	template<std::constructible_from<std::u8string> out_t, std::floating_point in_t>
	out_t u8_cast(const in_t& obj) {
		auto res = std::string(64, '\0');
		auto* begin = &(*res.begin());
		auto* end = &res.back();

		auto conv = std::to_chars(begin, end, obj, std::chars_format::general);
		if(conv.ec == std::errc())
			return std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
		else
			throw std::runtime_error("An input value can't be converted to string");
	};

	//convertions of benum types with std::u8string
	template<std::constructible_from<std::u8string> out_t, uns::benum in_t>
	out_t u8_cast(const in_t& obj) {
		return uns::string::u8_cast<std::u8string>(obj._to_string());
	};
	template<uns::benum out_t>
	out_t u8_cast(const std::u8string_view& obj) {
		return out_t::_from_string(uns::string::u8_cast<std::string>(obj).c_str());
	};

};


//IOSTREAMS FOR UTF-8 STRINGS
template<typename ostream_t>
ostream_t& operator<<(ostream_t& os, const std::u8string_view& str) {
	if constexpr(std::is_base_of<std::basic_ostream<char>, ostream_t>::value) {
		return operator<<(os, uns::string::u8_cast<std::string>(str));
	}
	else if constexpr(std::is_base_of<std::basic_ostream<wchar_t>, ostream_t>::value) {
		return operator<<(os, uns::string::u8_cast<std::wstring>(str));
	};
};

template<typename istream_t>
istream_t& operator>>(istream_t& is, std::u8string& str) {
	if constexpr(std::is_base_of<std::basic_ostream<char>, istream_t>::value) {
		auto val = std::string();
		auto& res = operator>>(is, val);
		str = uns::string::u8_cast<std::u8string>(val);
		return res;
	}
	else if constexpr(std::is_base_of<std::basic_ostream<wchar_t>, istream_t>::value) {
		auto val = std::wstring();
		auto& res = operator>>(is, val);
		str = uns::string::u8_cast<std::u8string>(val);
		return res;
	};
};
	

namespace uns::string {

	//CAST FUNCTIONS FOR NUMERICS TO FORMATTED STRING
	//cast of integer to string contain it's hexadecimal representation
	std::u8string hex_cast(long long int val) {
		auto res = std::string(64, '\0');

		auto* begin = &(*res.begin());
		auto* end = &res.back();

		if(val >= 0) {
			auto conv = std::to_chars(begin, end, val, 16);
			if(conv.ec == std::errc()) {
				res = "0x" + res;
				return std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw std::runtime_error("An input value can't be converted to string");
		}
		else {
			auto conv = std::to_chars(begin, end, -val, 16);
			if(conv.ec == std::errc()) {
				res = "-0x" + res;
				return std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw std::runtime_error("An input value can't be converted to string");
		};
	};

	//cast of integer to string contain it's binary representation
	std::u8string bin_cast(long long int val) {
		auto res = std::string(128, '\0');

		auto* begin = &(*res.begin());
		auto* end = &res.back();

		if(val >= 0) {
			auto conv = std::to_chars(begin, end, val, 2);
			if(conv.ec == std::errc()) {
				res = "0b" + res;
				return std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw std::runtime_error("An input value can't be converted to string");
		}
		else {
			auto conv = std::to_chars(begin, end, -val, 2);
			if(conv.ec == std::errc()) {
				res = "-0b" + res;
				return std::u8string(reinterpret_cast<const char8_t*>(res.c_str()));
			}
			else
				throw std::runtime_error("An input value can't be converted to string");
		};
	};


	//STRINGS OPERATIONS
	//positioning a seeker in the string relatively some mark symbols in it
	template<uns::string::std_basic string_t>
	bool seeker_pos(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		typename string_t::iterator													first_mark,						//first mark at the target string, relatively to what the seeker should be positioned
		typename string_t::iterator													last_mark,						// last mark at the target string, relatively to what the seeker should be positioned
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the first mark of positioning, false - if relative to the last mark
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last mark respectively (from first mark to the end of target string, from last mark - to the beginning)
		typename string_t::iterator													right_border_beg,				//position of the first symbol of the right border, that serves as the limit of seeker positioning from the right
		typename string_t::iterator													right_border_end				//position of the last symbol of the right border
	) noexcept {
		auto new_seeker_position = seeker;

		if(first_mark == target.end()) { return false; };
		if(right_border_beg != target.end() && first_mark > right_border_beg) return false;

		if(!from_begin) {
			if(last_mark == target.end() || first_mark > last_mark) last_mark = first_mark;
			if(last_mark - target.begin() < relative_position) return false;

			new_seeker_position = last_mark - relative_position;
		}
		else {
			if(target.end() - first_mark <= relative_position) return false;
			new_seeker_position = first_mark + relative_position;
		};

		if(right_border_end == target.end() || new_seeker_position <= right_border_end) {
			seeker = new_seeker_position;
			return true;
		}
		else return false;
	};
	template<uns::string::std_basic string_t>
	bool seeker_pos(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		typename string_t::iterator													first_mark,						//first mark at the target string, relatively to what the seeker should be positioned
		typename string_t::iterator													last_mark,						// last mark at the target string, relatively to what the seeker should be positioned
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the first mark of positioning, false - if relative to the last mark
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last mark respectively (from first mark to the end of target string, from last mark - to the beginning)
		typename string_t::iterator													right_border_beg				//position of the first symbol of the right border, that serves as the limit of seeker positioning from the right
	) noexcept {
		return uns::string::seeker_pos(target, seeker, first_mark, last_mark, from_begin, relative_position, right_border_beg, target.end());
	};
	template<uns::string::std_basic string_t>
	bool seeker_pos(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		typename string_t::iterator													first_mark,						//first mark at the target string, relatively to what the seeker should be positioned
		typename string_t::iterator													last_mark,						// last mark at the target string, relatively to what the seeker should be positioned
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the first mark of positioning, false - if relative to the last mark
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position				//this value indicates of how mutch symbols the seeker should be moved from first/last mark respectively (from first mark to the end of target string, from last mark - to the beginning)
	) noexcept {
		return uns::string::seeker_pos(target, seeker, first_mark, last_mark, from_begin, relative_position, target.end());
	};


	//seeking for sample(s) at given target string
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	typename string_t::iterator find(
		const string_t&																target,							//target string
		const typename string_t::iterator&											seeker,							//seeker of symbol to start the search
		const collection_t&															samples,						//a collection of samples wich should be found within the target
		typename collection_t::iterator&											found_sample					//iterator of found sample in the collection
	) {
		found_sample = samples.cend();

		if(seeker == target.end()) return target.end();
		const auto seeker_pos = seeker - target.begin();
		auto res_pos = target.end() - target.begin();

		for(const auto sample = samples.cbegin(); sample != samples.cend(); sample++) {
			if(sample->empty()) continue;
			if(
				auto sample_pos = target.find(*sample, seeker_pos);
				sample_pos >= seeker_pos
				&& sample_pos != string_t::npos
				&& sample_pos < res_pos
			) {
				res_pos = sample_pos;
				found_sample = sample;
			};
		};

		return target.begin() + res_pos;
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	typename string_t::iterator find(
		const string_t&																target,							//target string
		const typename string_t::iterator&											seeker,							//seeker of symbol to start the search
		const collection_t&															samples							//a collection of samples wich should be found within the target
	) {
		auto found_sample = samples.cend();

		return uns::string::find(target, seeker, samples, found_sample);
	};
	template<uns::string::std_basic string_t, typename unused_t>
	typename string_t::iterator find(
		const string_t&																target,							//target string
		const typename string_t::iterator&											seeker,							//seeker of symbol to start the search
		const string_t&																sample,							//a sample to seek within the target
		const unused_t&																found_sample					//an unused parameter to deliver template compatibility with a collection-of-samples case (so it can be of any type and value)
	) {
		if(seeker == target.end() || sample->empty()) return target.end();
		const auto seeker_pos = seeker - target.begin();
		auto res_pos = target.end() - target.begin();

		if(
			auto sample_pos = target.find(*sample, seeker_pos);
			sample_pos >= seeker_pos
			&& sample_pos != string_t::npos
			&& sample_pos < res_pos
		) {
			res_pos = sample_pos;
		};

		return target.begin() + res_pos;
	};
	template<uns::string::std_basic string_t, typename unused_t>
	typename string_t::iterator find(
		const string_t&																target,							//target string
		const typename string_t::iterator&											seeker,							//seeker of symbol to start the search
		const string_t&																sample							//a sample to seek within the target
	) {
		return uns::string::find(target, seeker, sample, sample);
	};


	//seeking for sample(s) at given target string and setting a seeker to a proper symbol of target relatively of found sample(s)
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const collection_t&															samples,						//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
		typename string_t::iterator													right_border_beg,				//position of the first symbol of the right border, that serves as the limit of seeker positioning from the right
		typename string_t::iterator													right_border_end				//position of the last symbol of the right border
	) {
		auto found_sample = samples.cend();
		auto found_sample_pos = uns::string::find(target, seeker, samples, found_sample);

		if(found_sample_pos == target.end()) return false;

		return uns::string::seeker_pos(target, seeker, found_sample_pos, found_sample_pos + found_sample->size(), from_begin, relative_position, right_border_beg, right_border_end);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const string_t&																sample,							//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
		typename string_t::iterator													right_border_beg,				//position of the first symbol of the right border, that serves as the limit of seeker positioning from the right
		typename string_t::iterator													right_border_end				//position of the last symbol of the right border
	) {
		auto found_sample_pos = uns::string::find(target, seeker, sample);

		if(found_sample_pos == target.end()) return false;

		return uns::string::seeker_pos(target, seeker, found_sample_pos, found_sample_pos + sample->size(), from_begin, relative_position, right_border_beg, right_border_end);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const collection_t&															samples,						//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
	) {
		auto right_border_beg = target.end();
		auto right_border_end = target.end();

		return uns::string::seeker_set(target, seeker, samples, from_begin, relative_position, right_border_beg, right_border_end);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const string_t&																sample,							//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
	) {
		auto right_border_beg = target.end();
		auto right_border_end = target.end();

		return uns::string::seeker_set(target, seeker, sample, from_begin, relative_position, right_border_beg, right_border_end);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> samples_t, uns::citerable_collection<string_t> rborder_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const samples_t&															samples,						//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
		const rborder_t&															right_borders					//collection of possible right borders of seeking (the mostleft will be considered an actual right border)
	) {
		auto actual_right_border = right_borders.cend();
		auto right_border_beg = uns::string::find(target, seeker, right_borders, actual_right_border);

		if(right_border_beg != target.end())
			return uns::string::seeker_set(target, seeker, samples, from_begin, relative_position, right_border_beg, right_border_beg + actual_right_border->size());
		else
			return uns::string::seeker_set(target, seeker, samples, from_begin, relative_position);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> rborder_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const string_t&																sample,							//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
		const rborder_t&															right_borders					//collection of possible right borders of seeking (the mostleft will be considered an actual right border)
	) {
		auto actual_right_border = right_borders.cend();
		auto right_border_beg = uns::string::find(target, seeker, right_borders, actual_right_border);

		if(right_border_beg != target.end())
			return uns::string::seeker_set(target, seeker, sample, from_begin, relative_position, right_border_beg, right_border_beg + actual_right_border->size());
		else
			return uns::string::seeker_set(target, seeker, sample, from_begin, relative_position);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const collection_t															samples,						//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
		const string_t&																right_border					//a substring-actual right border of seeking
	) {
		auto right_border_beg = uns::string::find(target, seeker, right_border);

		if(right_border_beg != target.end())
			return uns::string::seeker_set(target, seeker, samples, from_begin, relative_position, right_border_beg, right_border_beg + right_border->size());
		else
			return uns::string::seeker_set(target, seeker, samples, from_begin, relative_position);
	};
	template<uns::string::std_basic string_t, uns::citerable_collection<string_t> collection_t>
	bool seeker_set(
		const string_t&																target,							//target string
		typename string_t::iterator&												seeker,							//positioning seeker
		const string_t&																sample,							//a collection of samples wich should be found within the target
		bool																		from_begin,						//if true, this flag indicates that seekers new position must be done relative to the begin of found sample, false - to the end of found sample
		typename std::iterator_traits<typename string_t::iterator>::difference_type	relative_position,				//this value indicates of how mutch symbols the seeker should be moved from first/last symbol of found sample (if from begin than to the end of target string, if from end than to the beginning)
		const string_t&																right_border					//a substring-actual right border of seeking
	) {
		auto right_border_beg = uns::string::find(target, seeker, right_border);

		if(right_border_beg != target.end())
			return uns::string::seeker_set(target, seeker, sample, from_begin, relative_position, right_border_beg, right_border_beg + right_border->size());
		else
			return uns::string::seeker_set(target, seeker, sample, from_begin, relative_position);
	};


};