#pragma once

#ifndef UNS_LIB_STRINGS
#define UNS_LIB_STRINGS

#include <string>
#include <charconv>
#include <limits>
#include <type_traits>
#include <concepts>
#include <exception>
#include <stdexcept>

#include "uniself/renum.hpp"
#include "uniself/concepts.hpp"

namespace uns::string {

    //conversions from non-::std::u32string to non-::std::u32string
    namespace auxiliary {

        template<typename testing_t>
        concept non_constructible_from_u32string = !::std::constructible_from<testing_t, ::std::u32string>;

        inline char32_t digit_to_char(uint8_t SingleDigit) {
            if (SingleDigit >= 0 && SingleDigit < 10) {
                return U'0' + SingleDigit;
            }
            else if (SingleDigit >= 0x0a && SingleDigit < 0x10) {
                return U'a' + SingleDigit - 0x0a;
            }
            else {
                throw ::std::runtime_error{
                    ::std::string{ "Numeric value \'" }
                    + static_cast<char>(SingleDigit)
                    + "\' cannot be interpreted as a single digit of any base"
                };
            };
        };
        inline uint8_t dec_char_to_digit(const char32_t DecimalChar) {
            if (DecimalChar >= U'0' && DecimalChar <= U'9') {
                return DecimalChar - U'0';
            }
            else {
                throw ::std::runtime_error{ "A char32 cannot be interpreted as decimal digit" };
            };
        };
        inline uint8_t hex_char_to_digit(const char32_t HexadecimalChar) {
            if (HexadecimalChar >= U'0' && HexadecimalChar <= U'9') {
                return HexadecimalChar - U'0';
            }
            else if (HexadecimalChar >= U'a' && HexadecimalChar <= U'f') {
                return HexadecimalChar - U'a' + 10;
            }
            else if (HexadecimalChar >= U'A' && HexadecimalChar <= U'F') {
                return HexadecimalChar - U'A' + 10;
            }
            else {
                throw ::std::runtime_error{ "A char32 cannot be interpreted as hexadecimal digit" };
            };
        };
        inline uint8_t bin_char_to_digit(const char32_t BinaryChar) {
            if (BinaryChar >= U'0' && BinaryChar <= U'1') {
                return BinaryChar - U'0';
            }
            else {
                throw ::std::runtime_error{ "A char32 cannot be interpreted as binary digit" };
            };
        };

        template<typename in_t>
            requires (::std::is_integral<in_t>::value && !::std::is_same<in_t, bool>::value)
        ::std::u32string integer_to_string(const in_t Base, const in_t& Obj) {
            auto converting_val = Obj;

            constexpr ::std::size_t reversed_result_size = sizeof(in_t) * 8;
            char32_t reversed_result[reversed_result_size];
            int digits_counter = 0;
            for (auto& single_char : reversed_result) {
                single_char = ::uns::string::auxiliary::digit_to_char(converting_val % Base);
                converting_val /= Base;
                digits_counter++;

                if (converting_val == 0) {
                    break;
                };
            };

            auto result = ::std::u32string{};
            result.reserve(digits_counter);
            digits_counter -= 1;
            for (; digits_counter >= 0; --digits_counter) {
                result += reversed_result[digits_counter];
            };

            return result;
        }
    };


    //TRIM
    inline bool trim(::std::u32string& Str) {
        bool was_trimmed = false;

        auto new_begin = Str.cbegin();
        for (; new_begin < Str.cend(); ++new_begin) {
            if (
                *new_begin != U' '
                && *new_begin != U'\n'
                && *new_begin != U'\t'
                && *new_begin != U'\v'
                && *new_begin != U'\b'
                && *new_begin != U'\r'
                && *new_begin != U'\f'
                && *new_begin != U'\a'
            ) {
                break;
            };

            was_trimmed = true;
        };

        auto new_end = Str.cend();
        for (auto new_rend = Str.crbegin(); new_rend < Str.crend(); ++new_rend) {
            if (
                *new_rend != U' '
                && *new_rend != U'\n'
                && *new_rend != U'\t'
                && *new_rend != U'\v'
                && *new_rend != U'\b'
                && *new_rend != U'\r'
                && *new_rend != U'\f'
                && *new_rend != U'\a'
            ) {
                new_end = Str.cend() - (new_rend - Str.crbegin());
                break;
            };

            was_trimmed = true;
        };

        Str = ::std::u32string{
            new_begin
            , new_end
        };

        return was_trimmed;
    };
    inline ::std::u32string trim(const ::std::u32string_view& StrView) {
        auto result = ::std::u32string{ StrView };

        ::uns::string::trim(result);

        return result;
    };


    //STRING CAST FUNCTIONS
    // trivial ::std::u8string conversion
    template<::std::constructible_from<::std::string> out_t>
    out_t cast(const ::std::u8string_view& from) {
        return ::std::string{
            reinterpret_cast<const char*>(from.data())
        };
    };
    template<::std::constructible_from<::std::u8string> out_t>
    out_t cast(const ::std::string_view& from) {
        return ::std::u8string{
            reinterpret_cast<const char8_t*>(from.data())
        };
    };

    //convertions from ::std::u32string
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::u32string_view& from) {
        return ::std::u32string{ from };
    };
    template<::std::constructible_from<::std::u8string> out_t>
    out_t cast(const ::std::u32string_view& from) {
        auto to = ::std::u8string();
        to.reserve(from.size() * (sizeof(::std::u32string_view::value_type) / sizeof(::std::u8string_view::value_type)));

        for (const auto& val : from) {
            if (val < 0x80) {
                to += static_cast<char8_t>(val);
            }
            else if (val < 0x800) {
                to += 0xC0 + static_cast<char8_t>(val / 0x40);
                to += 0x80 + static_cast<char8_t>(val % 0x40);
            }
            else if (val < 0x10000) {
                to += 0xE0 + static_cast<char8_t>(val / 0x1000);
                to += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
                to += 0x80 + static_cast<char8_t>(val % 0x40);
            }
            else if (val >= 0x10000 && val < 0x110000) {
                to += 0xF0 + static_cast<char8_t>(val / 0x40000);
                to += 0x80 + static_cast<char8_t>((val % 0x40000) / 0x1000);
                to += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
                to += 0x80 + static_cast<char8_t>(val % 0x40);
            };
        };

        to.shrink_to_fit();
        return to;
    };
    template<::std::constructible_from<::std::string> out_t>
    out_t cast(const ::std::u32string_view& from) {
        return ::uns::string::cast<::std::string>(
            ::uns::string::cast<::std::u8string>(from)
        );
    };
    template<::std::constructible_from<::std::u16string> out_t>
    out_t cast(const ::std::u32string_view& from) {
        auto to = ::std::u16string();
        to.reserve(from.size() * (sizeof(::std::u32string::value_type) / sizeof(::std::u16string::value_type)));

        for (const auto& val : from) {
            if (val < 0xFFFF && !(val >= 0xD800 && val <= 0xDFFF)) {
                to += static_cast<char16_t>(val);
            }
            else if (val >= 0x10000 && val < 0x10FFFF) {
                to += 0xD800 + static_cast<char16_t>((val - 0x10000) / 0x0400);
                to += 0xDC00 + static_cast<char16_t>((val - 0x10000) % 0x0400);
            };
        };

        to.shrink_to_fit();
        return to;
    };
    template<::std::constructible_from<::std::wstring> out_t>
    out_t cast(const ::std::u32string_view& from) {
        if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u32string::value_type)) {
            return ::std::wstring{
                reinterpret_cast<const wchar_t*>(from.data())
            };
        }
        else if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u16string::value_type)) {
            return ::std::wstring{
                reinterpret_cast<const wchar_t*>(
                    ::uns::string::cast<::std::u16string>(from).c_str()
                )
            };
        }
        else {
            throw ::std::runtime_error("Size of ::std::wstring_view::value_type is neither 16 bit, nor 32 bit");
        };
    };
    //convertions to ::std::u32string
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::u8string_view& from) {
        auto to = ::std::u32string();
        to.reserve(from.size());

        char32_t symbol = 0x0;

        auto degree = 1;
        auto is_start = true;
        for (const auto& val : from) {
            if ((val & 0x80) == 0) {
                if (!is_start) {
                    to += symbol;
                    symbol = 0;
                };
                degree = 1;
                symbol = (static_cast<char32_t>(val & 0x7F) << (--degree * 6));
            }
            else if ((val & 0xE0) == 0xC0) {
                if (!is_start) {
                    to += symbol;
                    symbol = 0;
                };
                degree = 2;
                symbol = (static_cast<char32_t>(val & 0x1F) << (--degree * 6));
            }
            else if ((val & 0xF0) == 0xE0) {
                if (!is_start) {
                    to += symbol;
                    symbol = 0;
                };
                degree = 3;
                symbol = (static_cast<char32_t>(val & 0x0F) << (--degree * 6));
            }
            else if ((val & 0xF8) == 0xF0) {
                if (!is_start) {
                    to += symbol;
                    symbol = 0;
                };
                degree = 4;
                symbol = (static_cast<char32_t>(val & 0x07) << (--degree * 6));
            }
            else if ((val & 0xC0) == 0x80) {
                symbol += (static_cast<char32_t>(val & 0x3F) << (--degree * 6));
            };

            if (is_start)
                is_start = false;
        };

        if (symbol != 0)
            to += symbol;

        to.shrink_to_fit();
        return to;
    };
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::string_view& from) {
        return ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<::std::u8string>(from)
        );
    };
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::u16string_view& from) {
        auto to = ::std::u32string();
        to.reserve(from.size());

        char32_t symbol = 0x0;

        auto is_start = true;
        for (const auto& val : from) {
            if (val >= 0xD800 && val <= 0xDBFF) {
                if (!is_start) {
                    to += symbol;
                    symbol = 0;
                };
                symbol = (static_cast<char32_t>(val - 0xD800) << 10);
            }
            else if (val >= 0xDC00 && val <= 0xDFFF) {
                symbol += static_cast<char32_t>(val - 0xDC00);
                symbol += 0x10000;
            }
            else {
                if (!is_start) {
                    to += symbol;
                    symbol = 0;
                };
                symbol = static_cast<char32_t>(val);
            };

            if (is_start)
                is_start = false;
        };

        if (symbol != 0)
            to += symbol;

        to.shrink_to_fit();
        return to;
    };
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::wstring_view& from) {
        if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u32string::value_type)) {
            return ::std::u32string(reinterpret_cast<const char32_t*>(from.data()));
        }
        else if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u16string::value_type)) {
            return ::uns::string::cast<::std::u32string>(::std::u16string(reinterpret_cast<const char16_t*>(from.data())));
        }
        else {
            throw ::std::runtime_error("Size of ::std::wstring_view::value_type is neither 16 bit, nor 32 bit");
        };
    };

    //conversions renum <-> ::std::u32string
    template<::std::constructible_from<::std::u32string> out_t, ::uns::is_renum in_t>
    out_t cast(const in_t& from) {
        return from.to_string();
    };
    template<::uns::is_renum out_t, ::std::constructible_from<::std::u32string> in_t>
    out_t cast(const in_t& from) {
        return out_t::from_string(from);
    };

    //boolean conversions
    template<::std::same_as<bool> out_t>
    out_t cast(const ::std::u32string_view& str) {
        if (str == U"true" || str == U"1" || str == U"True" || str == U"TRUE") {
            return true;
        }
        else if (str == U"false" || str == U"0" || str == U"False" || str == U"FALSE") {
            return false;
        }
        else {
            throw ::std::runtime_error("An input string can't be converted to bool");
        };
    };
    template<::std::constructible_from<::std::u32string> out_t, ::std::same_as<bool> in_t>
    out_t cast(const in_t& Obj) {
        if (static_cast<bool>(Obj)) {
            return ::std::u32string(U"true");
        }
        else {
            return ::std::u32string(U"false");
        };
    };

    //numeric conversions
    template<typename out_t>
        requires (::std::is_integral<out_t>::value && !::std::is_same<out_t, bool>::value)
    out_t cast(const ::std::u32string_view& str) {
        auto iter = str.cbegin();

        if (str.cend() - iter < 1) {
            throw ::std::runtime_error{ "An input string cannot be converted to numeric type" };
        };

        out_t sign = 1;
        if (*iter == U'-') {
            if constexpr (!::std::is_signed<out_t>::value) {
                throw ::std::runtime_error{ "An input string cannot be converted to unsigned integer type" };
            };

            sign *= -1;
            ++iter;
        }
        else if (*iter == U'+') {
            ++iter;
        };

        out_t result = 0;
        bool is_dec = true;
        if (str.cend() - iter >= 2) {
            if (
                const auto prefix = ::std::u32string_view{ iter, iter + 2 };
                prefix == U"0x"
                || prefix == U"0X"
            ) {
                const out_t base = 0x10;

                iter += 2;
                while (iter < str.cend()) {
                    result *= base;
                    result += ::uns::string::auxiliary::hex_char_to_digit(*iter);

                    ++iter;
                };

                is_dec = false;
            }
            else if (
                prefix == U"0b"
                || prefix == U"0B"
            ) {
                const out_t base = 0b10;

                iter += 2;
                while (iter < str.cend()) {
                    result *= base;
                    result += ::uns::string::auxiliary::bin_char_to_digit(*iter);

                    ++iter;
                };

                is_dec = false;
            };
        };
        if(is_dec) {
            const out_t base = 10;
            while (iter < str.cend()) {
                result *= base;
                result += ::uns::string::auxiliary::dec_char_to_digit(*iter);

                ++iter;
            };
        };

        result *= sign;

        return result;
    };
    template<::std::constructible_from<::std::u32string> out_t, typename in_t>
        requires (::std::is_integral<in_t>::value && !::std::is_same<in_t, bool>::value)
    out_t cast(const in_t& Obj) {
        if constexpr (::std::is_signed<in_t>::value) {
            if (Obj < 0) {
                return { U"-" + ::uns::string::auxiliary::integer_to_string<in_t>(10, -Obj) };
            };
        };

        return { ::uns::string::auxiliary::integer_to_string<in_t>(10, Obj)};
    };
    template<::std::floating_point out_t>
    out_t cast(const ::std::u32string_view& Str) {
        /*Currently uniself does not support of hexadecimal or binary floating point strings*/
        auto one_byte_string = ::uns::string::cast<::std::string>(Str);
        auto res = out_t(0);
        for (auto format :
            {
                ::std::chars_format::scientific,
                ::std::chars_format::general,
                ::std::chars_format::fixed
            }
        ) {
            auto conv = ::std::from_chars(
                one_byte_string.c_str()
                , one_byte_string.c_str() + one_byte_string.size()
                , res
                , format
            );
            if (conv.ec == ::std::errc()) {
                return res;
            };
        };

        throw ::std::runtime_error("An input string can't be converted to floating point");
    };
    template<::std::constructible_from<::std::u32string> out_t, ::std::floating_point in_t>
    out_t cast(const in_t& Obj) {
        auto res = ::std::string(64, '\0');
        auto* begin = res.data();
        auto* end = res.data() + res.size();

        auto conv = ::std::to_chars(
            begin
            , end
            , Obj
            , ::std::chars_format::general
        );
        if (conv.ec == ::std::errc()) {
            return ::uns::string::cast<::std::u32string>(res);
        }
        else {
            throw ::std::runtime_error("An input value can't be converted to string");
        };
    };

    template<
        ::uns::string::auxiliary::non_constructible_from_u32string out_t
        , ::uns::string::auxiliary::non_constructible_from_u32string in_t
    >
    out_t cast(const in_t& from) {
        return ::uns::string::cast<out_t>(
            ::uns::string::cast<::std::u32string>(from)
        );
    };


    //CAST FUNCTIONS FOR NUMERICS TO FORMATTED STRING
    template<::std::constructible_from<::std::u32string> out_t, typename in_t>
        requires (::std::is_integral<in_t>::value && !::std::is_same<in_t, bool>::value)
    out_t to_hex(const in_t& Obj) {
        if constexpr (::std::is_signed<in_t>::value) {
            if (Obj < 0) {
                return { U"-0x" + ::uns::string::auxiliary::integer_to_string<in_t>(16, -Obj) };
            };
        };

        return { U"0x" + ::uns::string::auxiliary::integer_to_string<in_t>(16, Obj) };
    };
    template<::std::constructible_from<::std::u32string> out_t, typename in_t>
        requires (::std::is_integral<in_t>::value && !::std::is_same<in_t, bool>::value)
    out_t to_bin(const in_t& Obj) {
        if constexpr (::std::is_signed<in_t>::value) {
            if (Obj < 0) {
                return { U"-0b" + ::uns::string::auxiliary::integer_to_string<in_t>(2, -Obj) };
            };
        };

        return { U"0b" + ::uns::string::auxiliary::integer_to_string<in_t>(2, Obj) };
    };


    //STRING-SEEKER OPERATIONS
    //positioning a seeker in the string relatively some mark symbols in it
    template<::uns::is_basic_string string_t>
    bool seeker_pos(
        const string_t&                         target              //target string
        , typename string_t::const_iterator&    seeker              //positioning seeker
        , typename string_t::const_iterator     first_mark          //first mark at the target string, relatively to what the seeker should be positioned
        , typename string_t::const_iterator     last_mark           // last mark at the target string, relatively to what the seeker should be positioned
        , bool                                  from_begin          //if true, this flag indicates that seekers new position must be done relative to the first mark of positioning, false - if relative to the last mark
        , typename ::std::iterator_traits<typename string_t::const_iterator>::difference_type relative_position //this value indicates of how mutch symbols the seeker should be moved from first/last mark respectively (from first mark to the end of target string, from last mark - to the beginning)
        , typename string_t::const_iterator     right_border_beg    //position of the first symbol of the right border, that serves as the limit for the first_mark
        , typename string_t::const_iterator     right_border_end    //position of the last symbol of the right border, that serves as the limit for seeker positioning from the right
    ) noexcept {
        auto new_seeker_position = seeker;

        if (first_mark == target.cend()) { return false; };
        if (right_border_beg != target.cend() && first_mark > right_border_beg) return false;

        if (!from_begin) {
            if (last_mark == target.cend() || first_mark > last_mark) last_mark = first_mark;
            if (last_mark - target.cbegin() < relative_position) return false;

            new_seeker_position = last_mark - relative_position;
        }
        else {
            if (target.cend() - first_mark <= relative_position) return false;
            new_seeker_position = first_mark + relative_position;
        };

        if (right_border_end == target.cend() || new_seeker_position <= right_border_end) {
            seeker = new_seeker_position;
            return true;
        }
        else return false;
    };
    template<::uns::is_basic_string string_t>
    bool seeker_pos(
        const string_t&                         target              //target string
        , typename string_t::const_iterator&    seeker              //positioning seeker
        , typename string_t::const_iterator     first_mark          //first mark at the target string, relatively to what the seeker should be positioned
        , typename string_t::const_iterator     last_mark           // last mark at the target string, relatively to what the seeker should be positioned
        , bool                                  from_begin          //if true, this flag indicates that seekers new position must be done relative to the first mark of positioning, false - if relative to the last mark
        , typename ::std::iterator_traits<typename string_t::const_iterator>::difference_type relative_position //this value indicates of how mutch symbols the seeker should be moved from first/last mark respectively (from first mark to the end of target string, from last mark - to the beginning)
        , typename string_t::const_iterator     right_border_beg    //position of the first symbol of the right border, that serves as the limit for the first_mark
    ) noexcept {
        return ::uns::string::seeker_pos<string_t>(target, seeker, first_mark, last_mark, from_begin, relative_position, right_border_beg, target.cend());
    };
    template<::uns::is_basic_string string_t>
    bool seeker_pos(
        const string_t&                         target              //target string
        , typename string_t::const_iterator&    seeker              //positioning seeker
        , typename string_t::const_iterator     first_mark          //first mark at the target string, relatively to what the seeker should be positioned
        , typename string_t::const_iterator     last_mark           // last mark at the target string, relatively to what the seeker should be positioned
        , bool                                  from_begin          //if true, this flag indicates that seekers new position must be done relative to the first mark of positioning, false - if relative to the last mark
        , typename ::std::iterator_traits<typename string_t::const_iterator>::difference_type relative_position //this value indicates of how mutch symbols the seeker should be moved from first/last mark respectively (from first mark to the end of target string, from last mark - to the beginning)
    ) noexcept {
        return ::uns::string::seeker_pos(target, seeker, first_mark, last_mark, from_begin, relative_position, target.cend());
    };


    //seeking for sample(s) at given target string (the leftmost appearance, but not lefter than the seeker)
    template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> collection_t>
    typename string_t::const_iterator find(
        const string_t&                         target              //target string
        , const typename string_t::const_iterator& seeker           //seeker of symbol to start the search
        , const collection_t&                   samples             //a collection of samples wich should be found within the target
        , typename collection_t::const_iterator& found_sample       //iterator of found sample in the collection
    ) noexcept {
        found_sample = samples.cend();

        if (seeker == target.cend()) return target.cend();
        const auto seeker_pos = seeker - target.cbegin();
        auto res_pos = target.cend() - target.cbegin();

        for (auto sample = samples.cbegin(); sample != samples.cend(); ++sample) {
            if (sample->empty()) continue;
            if (
                auto sample_pos = target.find(*sample, seeker_pos);
                static_cast<decltype(seeker_pos)>(sample_pos) >= seeker_pos
                && sample_pos != string_t::npos
                && static_cast<decltype(res_pos)>(sample_pos) < res_pos
            ) {
                res_pos = static_cast<decltype(res_pos)>(sample_pos);
                found_sample = sample;
            };
        };

        return target.cbegin() + res_pos;
    };
    template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> collection_t>
    typename string_t::const_iterator find(
        const string_t&                         target              //target string
        , const typename string_t::const_iterator& seeker           //seeker of symbol to start the search
        , const collection_t&                   samples             //a collection of samples wich should be found within the target
    ) noexcept {
        auto found_sample = samples.cend();

        return ::uns::string::find<string_t>(target, seeker, samples, found_sample);
    };
    template<::uns::is_basic_string string_t>
    typename string_t::const_iterator find(
        const string_t&                         target              //target string
        , const typename string_t::const_iterator& seeker           //seeker of symbol to start the search
        , const string_t&                       sample              //a sample to seek within the target
    ) noexcept {
        if (seeker == target.cend() || sample.empty()) return target.cend();
        const auto seeker_pos = seeker - target.cbegin();
        auto res_pos = target.cend() - target.cbegin();

        if (!sample.empty()) {
            if (
                auto sample_pos = target.find(sample, seeker_pos);
                static_cast<decltype(seeker_pos)>(sample_pos) >= seeker_pos
                && sample_pos != string_t::npos
                && static_cast<decltype(res_pos)>(sample_pos) < res_pos
            ) {
                res_pos = static_cast<decltype(res_pos)>(sample_pos);
            };
        };

        return target.cbegin() + res_pos;
    };
    template<::uns::is_basic_string string_t, typename unused_t>
    typename string_t::const_iterator find(
        const string_t&                         target              //target string
        , const typename string_t::const_iterator& seeker           //seeker of symbol to start the search
        , const string_t&                       sample              //a sample to seek within the target
        , const unused_t&                       found_sample        //an unused parameter to deliver template compatibility with a collection-of-samples case (so it can be of any type and value)
    ) noexcept {
        return ::uns::string::find<string_t>(target, seeker, sample);
    };


};

#endif
