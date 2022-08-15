#pragma once

#include <string>
#include <charconv>
#include <type_traits>
#include <codecvt>
#include <locale>
#include <concepts>



namespace uns {

	template<std::constructible_from<std::u32string> out_t>
	out_t string_cast(const std::u8string_view& str) {
		auto res = std::u32string();

		return res;
	};
	template<std::constructible_from<std::u8string> out_t>
	out_t string_cast(const std::u32string_view& str) {
		auto res = std::u8string(); 

		return res;
	};

	template<std::constructible_from<std::string> out_t>
	out_t string_cast(const std::u8string_view& str) {
		auto res = std::string(); res.reserve(str.size());

		for(const auto& sym : str)
			res += static_cast<std::string::value_type>(sym);

		return res;
	};
	template<std::constructible_from<std::u8string> out_t>
	out_t string_cast(const std::string_view& str) {
		auto res = std::u8string(); res.reserve(str.size());

		for(const auto& sym : str)
			res += static_cast<std::u8string::value_type>(sym);

		return res;
	};

	template<std::constructible_from<std::wstring> out_t>
	out_t string_cast(const std::u16string_view& str) {
		if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u16string::value_type)) {
			auto res = std::wstring(); res.reserve(str.size());

			for(const auto& sym : str)
				res += static_cast<std::wstring::value_type>(sym);

			return res;
		}
		else if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u32string::value_type)) {
			auto val = uns::string_cast<std::u32string>(str);
			auto res = std::wstring(); res.reserve(val.size());

			for(const auto& sym : val)
				res += static_cast<std::wstring::value_type>(sym);

			return res;
		}
		else {
			return out_t();
		};
	};
	template<std::constructible_from<std::u16string> out_t>
	out_t string_cast(const std::wstring_view& str) {
		if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u16string::value_type)) {
			auto res = std::u16string(); res.reserve(str.size());

			for(const auto& sym : str)
				res += static_cast<std::u16string::value_type>(sym);

			return res;
		}
		else if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u32string::value_type)) {
			auto res = std::u32string(); res.reserve(str.size());

			for(const auto& sym : str)
				res += static_cast<std::u32string::value_type>(sym);

			return uns::string_cast<std::u16string>(res);
		}
		else {
			return out_t();
		};
	};
	template<std::constructible_from<std::wstring> out_t>
	out_t string_cast(const std::u32string_view& str) {
		if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u16string::value_type)) {
			auto val = uns::string_cast<std::u16string>(str);
			auto res = std::wstring(); res.reserve(val.size());

			for(const auto& sym : val)
				res += static_cast<std::wstring::value_type>(sym);

			return res;
		}
		else if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u32string::value_type)) {
			auto res = std::wstring(); res.reserve(str.size());

			for(const auto& sym : str)
				res += static_cast<std::wstring::value_type>(sym);

			return res;
		}
		else {
			return out_t();
		};
	};
	template<std::constructible_from<std::u32string> out_t>
	out_t string_cast(const std::wstring_view& str) {
		if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u16string::value_type)) {
			auto res = std::u16string(); res.reserve(str.size());

			for(const auto& sym : str)
				res += static_cast<std::u16string::value_type>(sym);

			return uns::string_cast<std::u32string>(res);
		}
		else if constexpr(sizeof(std::wstring::value_type) == sizeof(std::u32string::value_type)) {
			auto res = std::u32string(); res.reserve(str.size());

			for(const auto& sym : str)
				res += static_cast<std::u32string::value_type>(sym);

			return res;
		}
		else {
			return out_t();
		};
	};

	template<std::constructible_from<std::string> out_t>
	out_t string_cast(const std::string_view& str) {
		return static_cast<std::string>(str);
	};
	template<std::constructible_from<std::wstring> out_t>
	out_t string_cast(const std::string_view& str) {
		return uns::string_cast<std::wstring>(uns::string_cast<std::u16string>(uns::string_cast<std::u8string>(str)));
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

	template<std::constructible_from<std::string> out_t, std::same_as<bool> in_t>
	out_t string_cast(const in_t& obj) {
		auto val = static_cast<bool>(obj);

		if(val)
			return std::string("true");
		else
			return std::string("false");
	};
	template<std::constructible_from<std::string> out_t, typename in_t>
		requires (std::is_integral<in_t>::value && !std::is_same<in_t, bool>::value)
	out_t string_cast(const in_t& obj) {
		auto res = std::string();
		auto res_size = static_cast<std::string::size_type>(0);
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
		res = std::string("\0", res_size);

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
	template<std::constructible_from<std::string> out_t, std::floating_point in_t>
	out_t string_cast(const in_t& obj) {
		auto str = std::string("\0", 25);
		char* begin = str.data();
		char* end = &begin[str.size()];

			auto conv = std::to_chars(begin, end, obj, std::chars_format::general);
			if(conv.ec == std::errc()) {
				auto res = std::string(str.data(), conv.ptr);
				return res;
			}
			else
				str.clear();

		return str;
	};


	
	

};