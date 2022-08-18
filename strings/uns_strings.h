#pragma once

#include <string>
#include <charconv>
#include <type_traits>
#include <codecvt>
#include <locale>
#include <concepts>



namespace uns {

	class string : public std::string {
	public:

		string(const std::string_view& str) {
			*this = str;
		};
		string(const std::wstring_view& str) {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			convert(t_u32str, str);
			convert(t_u8str, t_u32str);
			convert(*this, t_u8str);
		};
		string(const std::u8string_view& str) {
			convert(*this, str);
		};
		string(const std::u16string_view& str) {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			convert(t_u32str, str);
			convert(t_u8str, t_u32str);
			convert(*this, t_u8str);
		};
		string(const std::u32string_view& str) {
			auto t_u8str = std::u8string();
			convert(t_u8str, str);
			convert(*this, t_u8str);
		};

		operator const std::string& () const& { return dynamic_cast<const std::string&>(*this); };
		operator std::string& ()& { return dynamic_cast<std::string&>(*this); };
		operator std::wstring() const {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			auto t_wstr = std::wstring();
			convert(t_u8str, *this);
			convert(t_u32str, t_u8str);
			convert(t_wstr, t_u32str);
			return t_wstr;
		};
		operator std::u8string() const {
			auto t_u8str = std::u8string();
			convert(t_u8str, *this);
			return t_u8str;
		};
		operator std::u16string() const {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			auto t_u16str = std::u16string();
			convert(t_u8str, *this);
			convert(t_u32str, t_u8str);
			convert(t_u16str, t_u32str);
			return t_u16str;
		};
		operator std::u32string() const {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			convert(t_u8str, *this);
			convert(t_u32str, t_u8str);
			return t_u32str;
		};


	protected:
		static void convert(std::u32string& to, const std::u8string_view& from) {
			auto res = std::u32string(); res.reserve(from.size());

			char32_t symbol = 0x0;

			auto degree = 1;
			auto is_start = true;
			for(const auto& val : from) {
				if((val & 0x80) == 0) {
					if(!is_start) {
						res += symbol;
						symbol = 0;
					};
					degree = 1;
					symbol = (static_cast<char32_t>(val & 0x7F) << (--degree * 6));
				}
				else if((val & 0xE0) == 0xC0) {
					if(!is_start) {
						res += symbol;
						symbol = 0;
					};
					degree = 2;
					symbol = (static_cast<char32_t>(val & 0x1F) << (--degree * 6));
				}
				else if((val & 0xF0) == 0xE0) {
					if(!is_start) {
						res += symbol;
						symbol = 0;
					};
					degree = 3;
					symbol = (static_cast<char32_t>(val & 0x0F) << (--degree * 6));
				}
				else if((val & 0xF8) == 0xF0) {
					if(!is_start) {
						res += symbol;
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
				res += symbol;

			to = std::u32string(res.begin(), res.end());
		};
		static void convert(std::u32string& to, const std::u16string_view& from) {
			auto res = std::u32string(); res.reserve(from.size());

			char32_t symbol = 0x0;

			auto is_start = true;
			for(const auto& val : from) {
				if(val >= 0xD800 && val <= 0xDBFF) {
					if(!is_start) {
						res += symbol;
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
						res += symbol;
						symbol = 0;
					};
					symbol = static_cast<char32_t>(val);
				};

				if(is_start)
					is_start = false;
			};
			if(symbol != 0)
				res += symbol;

			res.shrink_to_fit();
			to = std::u32string(res.begin(), res.end());
		};
		static void convert(std::u32string& to, const std::wstring_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				using out_t = std::u32string;
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<out_t::value_type>(liter);
				};

				to.shrink_to_fit();
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				auto t_u16str = std::u16string();
				convert(t_u16str, from);
				convert(to, t_u16str);
			};
		};
		static void convert(std::u8string& to, const std::string_view& from) {
			using out_t = std::u8string;
			to.clear();
			to.reserve(from.size());

			for(auto liter : from) {
				to += static_cast<out_t::value_type>(liter);
			};

			to.shrink_to_fit();
		};
		static void convert(std::u8string& to, const std::u32string_view& from) {
			auto res = std::u8string(); res.reserve(from.size() * (sizeof(std::u32string::value_type) / sizeof(std::u8string::value_type)));

			for(const auto& val : from) {
				if(val < 0x80) {
					res += static_cast<char8_t>(val);
				}
				else if(val < 0x800) {
					res += 0xC0 + static_cast<char8_t>(val / 0x40);
					res += 0x80 + static_cast<char8_t>(val % 0x40);
				}
				else if(val < 0x10000) {
					res += 0xE0 + static_cast<char8_t>(val / 0x1000);
					res += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
					res += 0x80 + static_cast<char8_t>(val % 0x40);
				}
				else if(val >= 0x10000 && val < 0x110000) {
					res += 0xF0 + static_cast<char8_t>(val / 0x40000);
					res += 0x80 + static_cast<char8_t>((val % 0x40000) / 0x1000);
					res += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
					res += 0x80 + static_cast<char8_t>(val % 0x40);
				};
			};

			res.shrink_to_fit();
			to = res;
		};
		static void convert(std::u16string& to, const std::wstring_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				using out_t = std::u16string;
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<out_t::value_type>(liter);
				};

				to.shrink_to_fit();
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				auto t_u32str = std::u32string();
				convert(t_u32str, from);
				convert(to, t_u32str);
			};
		};
		static void convert(std::u16string& to, const std::u32string_view& from) {
			auto res = std::u16string(); res.reserve(from.size() * (sizeof(std::u32string::value_type) / sizeof(std::u16string::value_type)));

			for(const auto& val : from) {
				if(val < 0xFFFF && !(val >= 0xD800&& val <= 0xDFFF)) {
					res += static_cast<char16_t>(val);
				}
				else if(val >= 0x10000 && val < 0x10FFFF) {
					res += 0xD800 + static_cast<char16_t>((val - 0x10000) / 0x0400);
					res += 0xDC00 + static_cast<char16_t>((val - 0x10000) % 0x0400);
				};
			};

			res.shrink_to_fit();
			to = res;
		};
		static void convert(std::string& to, const std::u8string_view& from) {
			using out_t = std::string;
			to.clear();
			to.reserve(from.size());

			for(auto liter : from) {
				to += static_cast<out_t::value_type>(liter);
			};

			to.shrink_to_fit();
		};
		static void convert(std::wstring& to, const std::u16string_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				using out_t = std::wstring;
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<out_t::value_type>(liter);
				};

				to.shrink_to_fit();
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				auto t_u32str = std::u32string();
				convert(t_u32str, from);
				convert(to, t_u32str);
			};
		};
		static void convert(std::wstring& to, const std::u32string_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				using out_t = std::wstring;
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<out_t::value_type>(liter);
				};

				to.shrink_to_fit();
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				auto t_u16str = std::u16string();
				convert(t_u16str, from);
				convert(to, t_u16str);
			};
		};



	};

	/*
	template<std::constructible_from<uns::string> out_t>
	out_t string_cast(const std::string_view& str) {
		return static_cast<uns::string>(str);
	};
	template<std::same_as<bool> out_t>
	out_t string_cast(const std::string_view& str) {
		if(str == "true" || str == "1" || str == "True" || str == "TRUE")
			return true;
		return false;
	};
	template<typename out_t>
		requires (std::is_integral<out_t>::value && !std::is_same<out_t, bool>::value)
	out_t string_cast(const std::string_view& str) {
		auto pos_hex = str.find("0x"); if(!(pos_hex >= 0 && pos_hex < str.size())) pos_hex = str.find("0X");
		auto pos_bin = str.find("0b"); if(!(pos_bin >= 0 && pos_bin < str.size())) pos_bin = str.find("0B");
		auto pos_minus = str.find("-");
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
			auto val = out_t(0);
			const char* begin = str.data();
			const char* end = &begin[str.size()];
			auto conv = std::from_chars(begin, end, val, 10);
			if(conv.ec == std::errc())
				return val;
			else
				is_hex = true;
		};
		if(is_hex) {
			auto val = out_t(0);
			const char* begin = &str.data()[pos_hex];
			const char* end = &begin[str.size()];
			auto conv = std::from_chars(begin, end, val, 16);
			if(conv.ec == std::errc())
				return val * (is_negative ? -1 : 1);
			else
				is_bin = true;
		};
		if(is_bin) {
			auto val = out_t(0);
			const char* begin = &str.data()[pos_bin];
			const char* end = &begin[str.size()];
			auto conv = std::from_chars(begin, end, val, 2);
			if(conv.ec == std::errc())
				return val * (is_negative ? -1 : 1);
		};
		return out_t(0);
	};
	template<std::floating_point out_t>
	out_t string_cast(const std::string_view& str) {
		auto val = out_t(0);
		const char* begin = str.data();
		const char* end = &begin[str.size()];
		for(auto format : { std::chars_format::scientific, std::chars_format::general, std::chars_format::fixed }) {
			auto conv = std::from_chars(begin, end, val, format);
			if(conv.ec == std::errc())
				return val;
		};
		return val;
	};
	template<std::constructible_from<uns::string> out_t, std::same_as<bool> in_t>
	out_t string_cast(const in_t& obj) {
		auto val = static_cast<bool>(obj);
		if(val)
			return uns::string("true");
		else
			return uns::string("false");
	};
	template<std::constructible_from<uns::string> out_t, typename in_t>
		requires (std::is_integral<in_t>::value && !std::is_same<in_t, bool>::value)
	out_t string_cast(const in_t& obj) {
		auto res = uns::string();
		auto res_size = static_cast<uns::string::size_type>(0);
		auto val = obj;
		if(obj < 0) {
			res_size += 1;
			val = -val;
		};
		if(obj != 0) {
			while(val > 0) {
				val /= 10;
				res_size += 1;
			};
		}
		else
			res_size += 1;
		res = uns::string("\0", res_size);
		char* begin = res.data();
		char* end = &begin[res.size()];
		auto conv = std::to_chars(begin, end, obj, 10);
		if(conv.ec == std::errc()) {
			return res;
		}
		else
			res.clear();
		return res;
	};
	template<std::constructible_from<uns::string> out_t, std::floating_point in_t>
	out_t string_cast(const in_t& obj) {
		auto str = uns::string("\0", 25);
		char* begin = str.data();
		char* end = &begin[str.size()];
			auto conv = std::to_chars(begin, end, obj, std::chars_format::general);
			if(conv.ec == std::errc()) {
				auto res = uns::string(str.data(), conv.ptr);
				return res;
			}
			else
				str.clear();
		return str;
	};
	*/



};
