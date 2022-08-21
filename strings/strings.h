#pragma once

#include <string>
#include <charconv>
#include <type_traits>
#include <concepts>



namespace uns {

	//UNIFIED STRING CLASS DECLARATION
	//unified string view
	class string_view : public std::string_view {
	protected:
	public:
		using base_type = std::string_view;

		constexpr string_view() noexcept : base_type() {};
		constexpr string_view(const std::string_view& view) noexcept : base_type(view) {};
		template<typename iter_t, typename end_t> constexpr string_view(iter_t first, end_t last) : base_type(first, last) {};
		constexpr string_view(const uns::string_view& view) noexcept : base_type(static_cast<const base_type&>(view)) {};
		constexpr string_view(uns::string_view&& view) noexcept : base_type(std::move(static_cast<base_type&&>(view))) {};
		constexpr uns::string_view& operator=(const uns::string_view& view) noexcept {
			if(this == &view) return *this;
			base_type::operator=(static_cast<const base_type&>(view));
			return *this;
		};
		constexpr uns::string_view& operator=(uns::string_view&& view) noexcept {
			if(this == &view) return *this;
			base_type::operator=(std::move(static_cast<base_type&&>(view)));
			return *this;
		};
		~string_view() = default;

		constexpr operator const std::string_view& () const& noexcept { return dynamic_cast<const base_type&>(*this); };
		constexpr operator std::string_view& () & noexcept { return dynamic_cast<base_type&>(*this); };
	};


	//unified string class
	class string : public std::string {
	public:
		using base_type = std::string;

		constexpr string() noexcept : base_type() {};
		constexpr string(const uns::string_view& str) noexcept : base_type(static_cast<std::string_view>(str)) {};
		constexpr string(const std::string_view& str) noexcept : base_type(str) {};
		constexpr string(const std::wstring_view& str) noexcept : base_type() {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			convert(t_u32str, str);
			convert(t_u8str, t_u32str);
			convert(*this, t_u8str);
		};
		constexpr string(const std::u8string_view& str) noexcept : base_type() {
			convert(*this, str);
		};
		constexpr string(const std::u16string_view& str) noexcept : base_type() {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			convert(t_u32str, str);
			convert(t_u8str, t_u32str);
			convert(*this, t_u8str);
		};
		constexpr string(const std::u32string_view& str) noexcept : base_type() {
			auto t_u8str = std::u8string();
			convert(t_u8str, str);
			convert(*this, t_u8str);
		};
		constexpr string(const uns::string& str) noexcept : base_type(static_cast<const base_type&>(str)) {};
		constexpr string(uns::string&& str) noexcept : base_type(std::move(static_cast<base_type&&>(str))) {};
		constexpr uns::string& operator=(const uns::string& str) noexcept {
			if(this == &str) return *this;
			base_type::operator=(static_cast<const base_type&>(str));
			return *this;
		};
		constexpr uns::string& operator=(uns::string&& str) noexcept {
			if(this == &str) return *this;
			base_type::operator=(std::move(static_cast<base_type&&>(str)));
			return *this;
		};
		template<typename basic_string_view_like_t>
			requires std::constructible_from<uns::string, basic_string_view_like_t>
		constexpr uns::string& operator=(basic_string_view_like_t&& str) {
			return *this = static_cast<uns::string>(std::forward<basic_string_view_like_t>(str));
		};
		template<typename valid_char_t, typename valid_char_traits_t = std::char_traits<valid_char_t>, typename valid_char_alloc_t = std::allocator<valid_char_t>>
			requires std::constructible_from<uns::string, std::basic_string<valid_char_t, valid_char_traits_t, valid_char_alloc_t>>
		constexpr uns::string& operator=(const valid_char_t* c_str) {
			return *this = static_cast<uns::string>(std::basic_string<valid_char_t, valid_char_traits_t, valid_char_alloc_t>(c_str));
		};
		template<typename valid_char_t, typename valid_char_traits_t = std::char_traits<valid_char_t>, typename valid_char_alloc_t = std::allocator<valid_char_t>>
			requires std::constructible_from<uns::string, std::basic_string<valid_char_t, valid_char_traits_t, valid_char_alloc_t>>
		constexpr uns::string& operator=(std::initializer_list<valid_char_t> char_list) {
			return *this = std::basic_string<valid_char_t, valid_char_traits_t, valid_char_alloc_t>(char_list.begin(), char_list.end());
		};
		~string() = default;

		constexpr operator const std::string& () const& noexcept { return dynamic_cast<const base_type&>(*this); };
		constexpr operator std::string& () & noexcept { return dynamic_cast<base_type&>(*this); };
		constexpr operator std::wstring() const {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			auto t_wstr = std::wstring();
			convert(t_u8str, *this);
			convert(t_u32str, t_u8str);
			convert(t_wstr, t_u32str);
			return t_wstr;
		};
		constexpr operator std::u8string() const {
			auto t_u8str = std::u8string();
			convert(t_u8str, *this);
			return t_u8str;
		};
		constexpr operator std::u16string() const {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			auto t_u16str = std::u16string();
			convert(t_u8str, *this);
			convert(t_u32str, t_u8str);
			convert(t_u16str, t_u32str);
			return t_u16str;
		};
		constexpr operator std::u32string() const {
			auto t_u32str = std::u32string();
			auto t_u8str = std::u8string();
			convert(t_u8str, *this);
			convert(t_u32str, t_u8str);
			return t_u32str;
		};
		constexpr operator const uns::string_view& () const& noexcept { return dynamic_cast<const base_type&>(*this).operator std::string_view(); };

		constexpr uns::string& operator +=(const uns::string_view& str) { static_cast<std::string&>(*this).operator+=(static_cast<const uns::string&>(str)); return *this; };
		constexpr uns::string& operator +=(const char8_t* str) { static_cast<std::string&>(*this).operator+=(static_cast<uns::string>(str)); return *this; };
	protected:
		static constexpr void convert(std::u32string& to, const std::u8string_view& from) {
			to.clear();
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
		};
		static constexpr void convert(std::u32string& to, const std::u16string_view& from) {
			to.clear();
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
		};
		static constexpr void convert(std::u32string& to, const std::wstring_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<std::u32string::value_type>(liter);
				};
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				auto t_u16str = std::u16string();
				convert(t_u16str, from);
				convert(to, t_u16str);
			};
		};
		static constexpr void convert(std::u8string& to, const std::string_view& from) {
			to.clear();
			to.reserve(from.size());

			for(auto liter : from) {
				to += static_cast<std::u8string::value_type>(liter);
			};
		};
		static constexpr void convert(std::u8string& to, const std::u32string_view& from) {
			to.clear();
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
		};
		static constexpr void convert(std::u16string& to, const std::wstring_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<std::wstring::value_type>(liter);
				};
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				auto t_u32str = std::u32string();
				convert(t_u32str, from);
				convert(to, t_u32str);
			};
		};
		static constexpr void convert(std::u16string& to, const std::u32string_view& from) {
			to.clear();
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
		};
		static constexpr void convert(std::string& to, const std::u8string_view& from) {
			to.clear();
			to.reserve(from.size());

			for(auto liter : from) {
				to += static_cast<std::string::value_type>(liter);
			};
		};
		static constexpr void convert(std::wstring& to, const std::u16string_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<std::wstring::value_type>(liter);
				};
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				auto t_u32str = std::u32string();
				convert(t_u32str, from);
				convert(to, t_u32str);
			};
		};
		static constexpr void convert(std::wstring& to, const std::u32string_view& from) {
			if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u32string::value_type)) {
				to.clear();
				to.reserve(from.size());

				for(auto liter : from) {
					to += static_cast<std::wstring::value_type>(liter);
				};
			}
			else if constexpr(sizeof(std::wstring_view::value_type) == sizeof(std::u16string::value_type)) {
				auto t_u16str = std::u16string();
				convert(t_u16str, from);
				convert(to, t_u16str);
			};
		};

	};

	//streams io-ops for unified strings
	template<typename ostream_t>
	auto& operator<<(ostream_t& os, const uns::string& str) {
		if constexpr(std::is_base_of<std::basic_ostream<char>, ostream_t >::value) {
			return operator<<(os, static_cast<std::string>(str));
		}
		else if constexpr(std::is_base_of<std::basic_ostream<wchar_t>, ostream_t>::value) {
			return operator<<(os, static_cast<std::wstring>(str));
		}
		else {
			return ostream_t();
		};
	};

	template<typename istream_t>
	auto& operator>>(istream_t& is, uns::string& str) {
		if constexpr(std::is_base_of<std::basic_ostream<char>, istream_t >::value) {
			return operator>>(is, static_cast<std::string>(str));
		}
		else if constexpr(std::is_base_of<std::basic_ostream<wchar_t>, istream_t>::value) {
			return operator>>(is, static_cast<std::wstring>(str));
		}
		else {
			return istream_t();
		};
	};

	//unified strings comparisons
	constexpr bool operator==(const uns::string& lhs, const char8_t* rhs) {
		return static_cast<const std::string&>(lhs) == static_cast<const std::string&>(static_cast<uns::string>(rhs));
	};
	constexpr auto operator<=>(const uns::string& lhs, const char8_t* rhs) {
		return static_cast<const std::string&>(lhs) <=> static_cast<const std::string&>(static_cast<uns::string>(lhs));
	};

	//concatenation op for unified strings
	uns::string operator+(const uns::string& lhs, const uns::string& rhs) {
		return static_cast<uns::string>(static_cast<const std::string&>(lhs) + static_cast<const std::string&>(rhs));
	};
	uns::string operator+(const uns::string& lhs, const char8_t* rhs) {
		return lhs + static_cast<uns::string>(rhs);
	};
	uns::string operator+(const char8_t* lhs, const uns::string& rhs) {
		return static_cast<uns::string>(lhs) + rhs;
	};
	uns::string operator+(uns::string&& lhs, uns::string&& rhs) {
		return static_cast<uns::string>(static_cast<std::string&&>(lhs) + static_cast<std::string&&>(rhs));
	};
	uns::string operator+(uns::string&& lhs, const uns::string& rhs) {
		return static_cast<uns::string>(static_cast<std::string&&>(lhs) + static_cast<const std::string&>(rhs));
	};
	uns::string operator+(uns::string&& lhs, const char8_t* rhs) {
		return lhs + static_cast<uns::string>(rhs);
	};
	uns::string operator+(const uns::string& lhs, uns::string&& rhs) {
		return static_cast<uns::string>(static_cast<const std::string&>(lhs) + static_cast<std::string&&>(rhs));
	};
	uns::string operator+(const char8_t* lhs, uns::string&& rhs) {
		return static_cast<uns::string>(lhs) + rhs;
	};


	//STRING_CAST FUNCTION DECLARATION
	template<std::same_as<bool> out_t>
	out_t string_cast(const uns::string_view& str) {
		return (str == u8"true" || str == u8"1" || str == u8"True" || str == u8"TRUE");
	};
	template<typename out_t>
		requires (std::is_integral<out_t>::value && !std::is_same<out_t, bool>::value)
	out_t string_cast(const uns::string_view& str) {
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

		for(auto format :
			{ 
				std::chars_format::scientific, 
				std::chars_format::general, 
				std::chars_format::fixed 
			}
		) {
			auto conv = std::from_chars(begin, end, val, format);
			if(conv.ec == std::errc())
				return val;
		};

		return val;
	};

	template<std::convertible_to<uns::string> out_t, std::same_as<bool> in_t>
	out_t string_cast(const in_t& obj) {
		auto val = static_cast<bool>(obj);

		if(val)
			return std::string("true");
		else
			return std::string("false");
	};
	template<std::convertible_to<uns::string> out_t, typename in_t>
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
	template<std::convertible_to<uns::string> out_t, std::floating_point in_t>
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