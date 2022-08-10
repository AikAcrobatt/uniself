#pragma once

#include <string>
#include <charconv>
#include <type_traits>
#include <codecvt>
#include <locale>



namespace uns {

	//a universal string cast function
	template<class out_t, class in_t>
	out_t string_cast(in_t obj) noexcept {
		if constexpr(std::is_convertible<in_t, std::string_view>::value) {
			auto str = static_cast<std::string_view>(obj);

			if constexpr(std::is_convertible<std::string, out_t>::value) {
				return static_cast<std::string>(str);
			}
			else if constexpr(std::is_convertible<std::wstring, out_t>::value) {
				auto locale = std::locale();

				if(std::has_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(locale)) {
					auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(locale);

					auto res = std::wstring(str.size(), L'\0');
					auto state = std::mbstate_t();
					const char* from;
					wchar_t* to;
					facet.in(state, &str[0], &str[str.size()], from, &res[0], &res[res.size()], to);
					res.resize(to - &res[0]);

					return res;
				};
			}
			else if constexpr(std::is_integral<out_t>::value && !std::is_same<out_t, bool>::value) {
				auto pos_hex = 0;
				auto pos_bin = 0;
				auto is_hex = (pos_hex = str.substr(0, 2) == "0x") || (pos_hex = str.substr(0, 2) == "0X");
				auto is_bin = (pos_bin = str.substr(0, 2) == "0b") || (pos_bin = str.substr(0, 2) == "0B");

				if(is_hex) {
					pos_hex += 2;
					is_hex = (pos_hex < str.size() - 1);
				}
				else
					pos_hex = 0;

				if(is_bin) {
					pos_bin += 2;
					is_bin = (pos_bin < str.size() - 1);
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
						return val;
					else
						is_bin = true;
				};

				if(is_bin) {
					auto val = out_t(0);
					const char* begin = &str.data()[pos_bin];
					const char* end = &begin[str.size()];

					auto conv = std::from_chars(begin, end, val, 2);
					if(conv.ec == std::errc())
						return val;
				};

			}
			else if constexpr(std::is_floating_point<out_t>::value) {
				auto val = out_t(0);
				const char* begin = str.data();
				const char* end = &begin[str.size()];

				for(auto format : { std::chars_format::general, std::chars_format::fixed, std::chars_format::scientific }) {
					auto conv = std::from_chars(begin, end, val, format);
					if(conv.ec == std::errc())
						return val;
				};
			}
			else if constexpr(std::is_same<out_t, bool>::value) {
				if(str == "true" || str == "1" || str == "True" || str == "TRUE")
					return true;
				
				return false;
			};
		}
		else {
			/*static_assert(
				false,
				"A Uniself 'string_cast' function does not support current types"
			);*/
		};

		return out_t();
	};
	

};