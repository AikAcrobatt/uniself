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

        inline char32_t digit_to_char(const uint8_t SingleDigit) {
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
        ::std::u32string integer_to_string(const in_t Base, const in_t& InitialIntegerValue) {
            auto converting_val = InitialIntegerValue;

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
    inline bool trim(::std::u32string& TrimmingString) {
        bool was_trimmed = false;

        auto new_begin = TrimmingString.cbegin();
        for (; new_begin < TrimmingString.cend(); ++new_begin) {
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

        auto new_end = TrimmingString.cend();
        for (auto new_rend = TrimmingString.crbegin(); new_rend < TrimmingString.crend(); ++new_rend) {
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
                new_end = TrimmingString.cend() - (new_rend - TrimmingString.crbegin());
                break;
            };

            was_trimmed = true;
        };

        TrimmingString = ::std::u32string{
            new_begin
            , new_end
        };

        return was_trimmed;
    };
    inline ::std::u32string trim(const ::std::u32string_view& InitialString) {
        auto trimmed_string = ::std::u32string{ InitialString };

        ::uns::string::trim(trimmed_string);

        return trimmed_string;
    };


    //STRING CAST FUNCTIONS
    // trivial ::std::u8string conversion
    template<::std::constructible_from<::std::string> out_t>
    out_t cast(const ::std::u8string_view& InitialString) {
        return ::std::string{
            reinterpret_cast<const char*>(InitialString.data())
        };
    };
    template<::std::constructible_from<::std::u8string> out_t>
    out_t cast(const ::std::string_view& InitialString) {
        return ::std::u8string{
            reinterpret_cast<const char8_t*>(InitialString.data())
        };
    };

    //convertions from ::std::u32string
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        return ::std::u32string{ InitialString };
    };
    template<::std::constructible_from<::std::u8string> out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        auto result_string = ::std::u8string();
        result_string.reserve(InitialString.size() * (sizeof(::std::u32string_view::value_type) / sizeof(::std::u8string_view::value_type)));

        for (const auto& val : InitialString) {
            if (val < 0x80) {
                result_string += static_cast<char8_t>(val);
            }
            else if (val < 0x800) {
                result_string += 0xC0 + static_cast<char8_t>(val / 0x40);
                result_string += 0x80 + static_cast<char8_t>(val % 0x40);
            }
            else if (val < 0x10000) {
                result_string += 0xE0 + static_cast<char8_t>(val / 0x1000);
                result_string += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
                result_string += 0x80 + static_cast<char8_t>(val % 0x40);
            }
            else if (val >= 0x10000 && val < 0x110000) {
                result_string += 0xF0 + static_cast<char8_t>(val / 0x40000);
                result_string += 0x80 + static_cast<char8_t>((val % 0x40000) / 0x1000);
                result_string += 0x80 + static_cast<char8_t>((val % 0x1000) / 0x40);
                result_string += 0x80 + static_cast<char8_t>(val % 0x40);
            };
        };

        result_string.shrink_to_fit();
        return result_string;
    };
    template<::std::constructible_from<::std::string> out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        return ::uns::string::cast<::std::string>(
            ::uns::string::cast<::std::u8string>(InitialString)
        );
    };
    template<::std::constructible_from<::std::u16string> out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        auto result_string = ::std::u16string();
        result_string.reserve(InitialString.size() * (sizeof(::std::u32string::value_type) / sizeof(::std::u16string::value_type)));

        for (const auto& val : InitialString) {
            if (val < 0xFFFF && !(val >= 0xD800 && val <= 0xDFFF)) {
                result_string += static_cast<char16_t>(val);
            }
            else if (val >= 0x10000 && val < 0x10FFFF) {
                result_string += 0xD800 + static_cast<char16_t>((val - 0x10000) / 0x0400);
                result_string += 0xDC00 + static_cast<char16_t>((val - 0x10000) % 0x0400);
            };
        };

        result_string.shrink_to_fit();
        return result_string;
    };
    template<::std::constructible_from<::std::wstring> out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u32string::value_type)) {
            return ::std::wstring{
                reinterpret_cast<const wchar_t*>(InitialString.data())
            };
        }
        else if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u16string::value_type)) {
            return ::std::wstring{
                reinterpret_cast<const wchar_t*>(
                    ::uns::string::cast<::std::u16string>(InitialString).c_str()
                )
            };
        }
        else {
            throw ::std::runtime_error("Size of ::std::wstring_view::value_type is neither 16 bit, nor 32 bit");
        };
    };
    //convertions result_string ::std::u32string
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::u8string_view& InitialString) {
        auto result_string = ::std::u32string();
        result_string.reserve(InitialString.size());

        char32_t symbol = 0x0;

        auto degree = 1;
        auto is_start = true;
        for (const auto& val : InitialString) {
            if ((val & 0x80) == 0) {
                if (!is_start) {
                    result_string += symbol;
                    symbol = 0;
                };
                degree = 1;
                symbol = (static_cast<char32_t>(val & 0x7F) << (--degree * 6));
            }
            else if ((val & 0xE0) == 0xC0) {
                if (!is_start) {
                    result_string += symbol;
                    symbol = 0;
                };
                degree = 2;
                symbol = (static_cast<char32_t>(val & 0x1F) << (--degree * 6));
            }
            else if ((val & 0xF0) == 0xE0) {
                if (!is_start) {
                    result_string += symbol;
                    symbol = 0;
                };
                degree = 3;
                symbol = (static_cast<char32_t>(val & 0x0F) << (--degree * 6));
            }
            else if ((val & 0xF8) == 0xF0) {
                if (!is_start) {
                    result_string += symbol;
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
            result_string += symbol;

        result_string.shrink_to_fit();
        return result_string;
    };
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::string_view& InitialString) {
        return ::uns::string::cast<::std::u32string>(
            ::uns::string::cast<::std::u8string>(InitialString)
        );
    };
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::u16string_view& InitialString) {
        auto result_string = ::std::u32string();
        result_string.reserve(InitialString.size());

        char32_t symbol = 0x0;

        auto is_start = true;
        for (const auto& val : InitialString) {
            if (val >= 0xD800 && val <= 0xDBFF) {
                if (!is_start) {
                    result_string += symbol;
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
                    result_string += symbol;
                    symbol = 0;
                };
                symbol = static_cast<char32_t>(val);
            };

            if (is_start)
                is_start = false;
        };

        if (symbol != 0)
            result_string += symbol;

        result_string.shrink_to_fit();
        return result_string;
    };
    template<::std::constructible_from<::std::u32string> out_t>
    out_t cast(const ::std::wstring_view& InitialString) {
        if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u32string::value_type)) {
            return ::std::u32string(reinterpret_cast<const char32_t*>(InitialString.data()));
        }
        else if constexpr (sizeof(::std::wstring_view::value_type) == sizeof(::std::u16string::value_type)) {
            return ::uns::string::cast<::std::u32string>(::std::u16string(reinterpret_cast<const char16_t*>(InitialString.data())));
        }
        else {
            throw ::std::runtime_error("Size of ::std::wstring_view::value_type is neither 16 bit, nor 32 bit");
        };
    };

    //conversions renum <-> ::std::u32string
    template<::std::constructible_from<::std::u32string> out_t, ::uns::is_renum in_t>
    out_t cast(const in_t& InitialRenum) {
        return InitialRenum.to_string();
    };
    template<::uns::is_renum out_t, ::std::constructible_from<::std::u32string> in_t>
    out_t cast(const in_t& InitialString) {
        return out_t::from_string(InitialString);
    };

    //boolean conversions
    template<::std::same_as<bool> out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        if (InitialString == U"true" || InitialString == U"1" || InitialString == U"True" || InitialString == U"TRUE") {
            return true;
        }
        else if (InitialString == U"false" || InitialString == U"0" || InitialString == U"False" || InitialString == U"FALSE") {
            return false;
        }
        else {
            throw ::std::runtime_error("An input string can't be converted to bool");
        };
    };
    template<::std::constructible_from<::std::u32string> out_t, ::std::same_as<bool> in_t>
    out_t cast(const in_t& InitialBooleanValue) {
        if (static_cast<bool>(InitialBooleanValue)) {
            return ::std::u32string(U"true");
        }
        else {
            return ::std::u32string(U"false");
        };
    };

    //numeric conversions
    template<typename out_t>
        requires (::std::is_integral<out_t>::value && !::std::is_same<out_t, bool>::value)
    out_t cast(const ::std::u32string_view& InitialString) {
        auto iter = InitialString.cbegin();

        if (InitialString.cend() - iter < 1) {
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
        if (InitialString.cend() - iter >= 2) {
            if (
                const auto prefix = ::std::u32string_view{ iter, iter + 2 };
                prefix == U"0x"
                || prefix == U"0X"
            ) {
                const out_t base = 0x10;

                iter += 2;
                while (iter < InitialString.cend()) {
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
                while (iter < InitialString.cend()) {
                    result *= base;
                    result += ::uns::string::auxiliary::bin_char_to_digit(*iter);

                    ++iter;
                };

                is_dec = false;
            };
        };
        if(is_dec) {
            const out_t base = 10;
            while (iter < InitialString.cend()) {
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
    out_t cast(const in_t& InitialNumericValue) {
        if constexpr (::std::is_signed<in_t>::value) {
            if (InitialNumericValue < 0) {
                return { U"-" + ::uns::string::auxiliary::integer_to_string<in_t>(10, -InitialNumericValue) };
            };
        };

        return { ::uns::string::auxiliary::integer_to_string<in_t>(10, InitialNumericValue)};
    };
    template<::std::floating_point out_t>
    out_t cast(const ::std::u32string_view& InitialString) {
        /*Currently uniself does not support hexadecimal or binary floating point strings*/
        auto narrow_string = ::uns::string::cast<::std::string>(InitialString);
        auto res = out_t(0);
        for (auto format :
            {
                ::std::chars_format::scientific,
                ::std::chars_format::general,
                ::std::chars_format::fixed
            }
        ) {
            auto conv = ::std::from_chars(
                narrow_string.c_str()
                , narrow_string.c_str() + narrow_string.size()
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
    out_t cast(const in_t& InitialNumericValue) {
        auto res = ::std::string(64, '\0');
        auto* begin = res.data();
        auto* end = res.data() + res.size();

        auto conv = ::std::to_chars(
            begin
            , end
            , InitialNumericValue
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
    out_t cast(const in_t& InitialValue) {
        return ::uns::string::cast<out_t>(
            ::uns::string::cast<::std::u32string>(InitialValue)
        );
    };


    //CAST FUNCTIONS FOR NUMERICS TO FORMATTED STRING
    template<::std::constructible_from<::std::u32string> out_t, typename in_t>
        requires (::std::is_integral<in_t>::value && !::std::is_same<in_t, bool>::value)
    out_t to_hex(const in_t& InitialNumericValue) {
        if constexpr (::std::is_signed<in_t>::value) {
            if (InitialNumericValue < 0) {
                return { U"-0x" + ::uns::string::auxiliary::integer_to_string<in_t>(16, -InitialNumericValue) };
            };
        };

        return { U"0x" + ::uns::string::auxiliary::integer_to_string<in_t>(16, InitialNumericValue) };
    };
    template<::std::constructible_from<::std::u32string> out_t, typename in_t>
        requires (::std::is_integral<in_t>::value && !::std::is_same<in_t, bool>::value)
    out_t to_bin(const in_t& InitialNumericValue) {
        if constexpr (::std::is_signed<in_t>::value) {
            if (InitialNumericValue < 0) {
                return { U"-0b" + ::uns::string::auxiliary::integer_to_string<in_t>(2, -InitialNumericValue) };
            };
        };

        return { U"0b" + ::uns::string::auxiliary::integer_to_string<in_t>(2, InitialNumericValue) };
    };


    namespace parsing {

        struct seeker_position {
            enum sample_relative {
                from_begin
                , from_end
            };
        public:
            sample_relative qualifier = sample_relative::from_begin;
            int offset = 0;
        };

        namespace auxiliary {

            //STRING-SEEKER OPERATIONS
            //positioning a Seeker in the string near some mark symbols in it
            template<::uns::is_basic_string string_t>
            bool offset(
                const string_t&                             Source          //Source string
                , typename string_t::const_iterator&        Seeker          //positioning Seeker
                , const typename string_t::const_iterator   SampleBegin     //first mark at the Source string, relatively to what the Seeker should be positioned
                , const typename string_t::const_iterator   SampleEnd       // last mark at the Source string, relatively to what the Seeker should be positioned
                , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
                , const typename string_t::const_iterator   LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for the SampleBegin
                , const typename string_t::const_iterator   LimiterEnd      //position of the last symbol of the limiter, that serves as the limit for Seeker positioning from the right
            ) {
                auto seeker_new_pos = Seeker;

                if (SampleBegin > SampleEnd) { throw ::std::runtime_error{ "Sample's end cannot be before it's begin" }; };
                if (LimiterBegin > LimiterEnd) { throw ::std::runtime_error{ "Limiter's end cannot be before it's begin" }; };

                if (SampleBegin == SampleEnd) { return false; };
                if (SampleBegin == Source.cend()) { return false; };
                if (LimiterBegin != Source.cend() && SampleBegin > LimiterBegin) { return false; };

                auto pivot_point = Source.cend();
                switch (SeekerTargetPosition.qualifier) {
                    case ::uns::string::parsing::seeker_position::from_begin: {
                        pivot_point = SampleBegin;
                        break;
                    }
                    case ::uns::string::parsing::seeker_position::from_end: {
                        pivot_point = SampleEnd;
                        break;
                    }
                    default: {
                        throw ::std::runtime_error{ "Unknown seeker_position qualifier" };
                    }
                };
                if (Source.cend() - pivot_point <= SeekerTargetPosition.offset) { return false; };
                if (pivot_point - Source.cbegin() < -SeekerTargetPosition.offset) { return false; };

                seeker_new_pos = pivot_point + SeekerTargetPosition.offset;

                if (LimiterEnd == Source.cend() || seeker_new_pos < LimiterEnd) {
                    Seeker = seeker_new_pos;
                    return true;
                }
                else {
                    return false;
                };
            };


            template<::uns::is_basic_string string_t>
            typename string_t::const_iterator find(
                const string_t&                             Source          //Source string
                , const typename ::std::iterator_traits<typename string_t::const_iterator>::difference_type& SeekerOffset //seeker's offset from Source's begin
                , const string_t&                           Sample          //a Sample to seek within the Source
            ) {
                if (
                    auto sample_offset = Source.find(Sample, SeekerOffset);
                    sample_offset != string_t::npos
                    && sample_offset >= SeekerOffset
                ) {
                    return sample_offset + Source.cbegin();
                };

                return Source.cend();
            };

        };

        //seeking for Sample(s) at given Source string (the leftmost appearance, but not lefter than the Seeker)
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        typename string_t::const_iterator find(
            const string_t&                             Source          //Source string
            , const typename string_t::const_iterator&  Seeker          //Seeker of symbol to start the search
            , const collection_t&                       Samples         //a collection of Samples wich should be found within the Source
            , typename collection_t::const_iterator&    FoundSample     //iterator of found Sample in the collection
        ) {
            FoundSample = Samples.cend();

            if (Seeker == Source.cend()) { return Source.cend(); };
            const auto seeker_pos = Seeker - Source.cbegin();
            auto found = Source.cend();

            for (auto sample = Samples.cbegin(); sample != Samples.cend(); ++sample) {
                if (sample->empty()) { continue; };
                if (
                    auto sample_seeker = ::uns::string::parsing::auxiliary::find<string_t>(
                        Source
                        , seeker_pos
                        , *sample
                    );
                    sample_seeker != Source.cend()
                    && sample_seeker < found
                ) {
                    found = sample_seeker;
                    FoundSample = sample;
                };
            };

            return found;
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        typename string_t::const_iterator find(
            const string_t&                             Source          //Source string
            , const typename string_t::const_iterator&  Seeker          //Seeker of symbol to start the search
            , const collection_t&                       Samples         //a collection of Samples wich should be found within the Source
        ) {
            auto found_sample = Samples.cend();
            return ::uns::string::parsing::find<string_t>(Source, Seeker, Samples, found_sample);
        };
        template<::uns::is_basic_string string_t>
        typename string_t::const_iterator find(
            const string_t&                             Source          //Source string
            , const typename string_t::const_iterator&  Seeker          //Seeker of symbol to start the search
            , const string_t&                           Sample          //a Sample to seek within the Source
        ) {
            if (Seeker == Source.cend() || Sample.empty()) return Source.cend();
            const auto seeker_pos = Seeker - Source.cbegin();

            return ::uns::string::parsing::auxiliary::find<string_t>(
                Source
                , seeker_pos
                , Sample
            );
        };
        /*template<::uns::is_basic_string string_t, typename unused_t>
        typename string_t::const_iterator find(
            const string_t& Source              //Source string
            , const typename string_t::const_iterator& Seeker           //Seeker of symbol to start the search
            , const string_t& Sample              //a Sample to seek within the Source
            , const unused_t& FoundSample        //an unused parameter to deliver template compatibility with a collection-of-Samples case (so it can be of any type and value)
        ) {
            return ::uns::string::parsing::find<string_t>(Source, Seeker, Sample);
        };*/


        //seeking for Sample(s) at given Source string (the leftmost appearance of one of the Sample(s) if it is,
        // but not lefter than the initial place of the Seeker) and setting a Seeker to a proper symbol of Source
        // relatively of found Sample(s) (only if at least one Sample found)
        //
        //returns true only if both: some Sample found at not lefter than Seeker pos AND Seeker was successfully placed to new pos
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const collection_t&                       Samples         //a collection of Samples wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
            , const typename string_t::const_iterator   LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
            , const typename string_t::const_iterator   LimiterEnd      //position of the last symbol of the limiter, that serves as the limit of Seeker positioning from the right
        ) noexcept {
            auto found_sample = Samples.cend();
            const auto found_sample_pos = ::uns::string::parsing::find<string_t>(Source, Seeker, Samples, found_sample);

            if (found_sample_pos == Source.cend()) {
                return false;
            };

            return ::uns::string::parsing::auxiliary::offset<string_t>(
                Source
                , Seeker
                , found_sample_pos
                , found_sample_pos + found_sample->size()
                , SeekerTargetPosition
                , LimiterBegin
                , LimiterEnd
            );
        };
        template<::uns::is_basic_string string_t>
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const string_t&                           Sample          //a collection of Samples wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
            , const typename string_t::const_iterator   LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
            , const typename string_t::const_iterator   LimiterEnd      //position of the last symbol of the limiter, that serves as the limit of Seeker positioning from the right
        ) noexcept {
            const auto found_sample_pos = ::uns::string::parsing::find<string_t>(Source, Seeker, Sample);

            if (found_sample_pos == Source.cend()) {
                return false;
            };

            return ::uns::string::parsing::auxiliary::offset<string_t>(
                Source
                , Seeker
                , found_sample_pos
                , found_sample_pos + Sample.size()
                , SeekerTargetPosition
                , LimiterBegin
                , LimiterEnd
            );
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const collection_t&                       Samples         //a collection of Samples wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
        ) noexcept {
            return ::uns::string::parsing::seek<string_t>(
                Source
                , Seeker
                , Samples
                , SeekerTargetPosition
                , Source.cend()
                , Source.cend()
            );
        };
        template<::uns::is_basic_string string_t>
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const string_t&                           Sample          //a collection of Samples wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
        ) noexcept {
            return ::uns::string::parsing::seek<string_t>(
                Source
                , Seeker
                , Sample
                , SeekerTargetPosition
                , Source.cend()
                , Source.cend()
            );
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> samples_t
            , ::uns::const_iterable_collection<string_t> limiter_t
        >
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const samples_t&                          Samples         //a collection of Samples wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
            , const limiter_t&                          Limiters        //collection of possible right borders of seeking (the mostleft will be considered an actual limiter)
        ) noexcept {
            auto actual_limiter = Limiters.cend();
            auto const limiter_beg = ::uns::string::parsing::find<string_t>(
                Source
                , Seeker
                , Limiters
                , actual_limiter
            );

            if (limiter_beg != Source.cend() && actual_limiter != Limiters.cend()) {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Samples
                    , SeekerTargetPosition
                    , limiter_beg
                    , limiter_beg + actual_limiter->size()
                );
            }
            else {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Samples
                    , SeekerTargetPosition
                );
            };
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> limiter_t
        >
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const string_t&                           Sample          //a Sample wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
            , const limiter_t&                          Limiters        //collection of possible right borders of seeking (the mostleft will be considered an actual limiter)
        ) noexcept {
            auto actual_limiter = Limiters.cend();
            const auto limiter_beg = ::uns::string::parsing::find<string_t>(
                Source
                , Seeker
                , Limiters
                , actual_limiter
            );

            if (limiter_beg != Source.cend() && actual_limiter != Limiters.cend()) {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Sample
                    , SeekerTargetPosition
                    , limiter_beg
                    , limiter_beg + actual_limiter->size()
                );
            }
            else {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Sample
                    , SeekerTargetPosition
                );
            };
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const collection_t&                       Samples         //a collection of Samples wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
            , const string_t&                           Limiter         //a substring-actual limiter of seeking
        ) noexcept {
            if (
                const auto limiter_beg = ::uns::string::parsing::find<string_t>(
                    Source
                    , Seeker
                    , Limiter
                );
                limiter_beg != Source.cend()
            ) {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Samples
                    , SeekerTargetPosition
                    , limiter_beg
                    , limiter_beg + Limiter.size()
                );
            }
            else {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Samples
                    , SeekerTargetPosition
                );
            };
        };
        template<::uns::is_basic_string string_t>
        bool seek(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , const string_t&                           Sample          //a Sample wich should be found within the Source (only not lefter than the initial place of the Seeker)
            , const ::uns::string::parsing::seeker_position SeekerTargetPosition //defines where to put seeker in case of success
            , const string_t&                           Limiter         //a substring-actual limiter of seeking
        ) noexcept {
            if (
                const auto limiter_beg = ::uns::string::parsing::find<string_t>(
                    Source
                    , Seeker
                    , Limiter
                );
                limiter_beg != Source.cend()
            ) {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Sample
                    , SeekerTargetPosition
                    , limiter_beg
                    , limiter_beg + Limiter.size()
                );
            }
            else {
                return ::uns::string::parsing::seek<string_t>(
                    Source
                    , Seeker
                    , Sample
                    , SeekerTargetPosition
                );
            };
        };


        //reading a Fragment(a substring) from the Source string from the current Seeker's pos till the Delimiter string (not including it)
        //    returns true only if the Delimiter string was found farther(!) than current Seeker AND the Seeker was successfully placed to
        //    specified position, relatively to the found Delimiter
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const collection_t&                       Delimiters      //an iterable collection of delimiting strings
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
            , const typename string_t::const_iterator   LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
            , const typename string_t::const_iterator   LimiterEnd      //position of the last symbol of the limiter
        ) noexcept {
            auto delimiter = Delimiters.cend();

            const auto delimiter_pos = ::uns::string::parsing::find<string_t>(
                Source
                , Seeker
                , Delimiters
                , delimiter
            );
            if (delimiter == Delimiters.cend()) return false;
            if (!(delimiter_pos > Seeker && delimiter_pos < Source.cend())) return false;

            Fragment = string_t{ Seeker, delimiter_pos };

            if (
                !::uns::string::parsing::auxiliary::offset<string_t>(
                    Source
                    , Seeker
                    , delimiter_pos
                    , delimiter_pos + delimiter->size()
                    , SeekerDelimiterPosition
                    , LimiterBegin
                    , LimiterEnd
                )
            ) {
                Fragment.clear();
                return false;
            }
            else {
                return true;
            };
        };
        template<::uns::is_basic_string string_t>
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //delimiting string
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
            , const typename string_t::const_iterator   LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
            , const typename string_t::const_iterator   LimiterEnd      //position of the last symbol of the limiter
        ) noexcept {
            const auto delimiter_pos = ::uns::string::parsing::find<string_t>(Source, Seeker, Delimiter);

            if (!(delimiter_pos > Seeker && delimiter_pos < Source.cend())) return false;

            Fragment = string_t{ Seeker, delimiter_pos };

            if (
                !::uns::string::parsing::auxiliary::offset<string_t>(
                    Source
                    , Seeker
                    , delimiter_pos
                    , delimiter_pos + Delimiter.size()
                    , SeekerDelimiterPosition
                    , LimiterBegin
                    , LimiterEnd
                )
            ) {
                Fragment.clear();
                return false;
            }
            else {
                return true;
            };
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const collection_t&                       Delimiters      //an iterable collection of delimiting strings
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
        ) noexcept {
            return ::uns::string::parsing::read<string_t>(
                Source
                , Seeker
                , Fragment
                , Delimiters
                , SeekerDelimiterPosition
                , Source.cend()
                , Source.cend()
            );
        };
        template<::uns::is_basic_string string_t>
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //delimiting string
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
        ) noexcept {
            return ::uns::string::parsing::read<string_t>(
                Source
                , Seeker
                , Fragment
                , Delimiter
                , SeekerDelimiterPosition
                , Source.cend()
                , Source.cend()
            );
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
            , ::uns::const_iterable_collection<string_t> limiter_t
        >
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const collection_t&                       Delimiters      //an iterable collection of delimiting strings
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
            , const limiter_t&                          Limiters        //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
        ) noexcept {
            auto limiter = Limiters.cend();

            const auto limiter_pos = ::uns::string::parsing::find<string_t>(
                Source
                , Seeker
                , Limiters
                , limiter
            );

            if (limiter != Limiters.cend()) {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiters
                    , SeekerDelimiterPosition
                    , limiter_pos
                    , limiter_pos + limiter->size()
                );
            }
            else {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiters
                    , SeekerDelimiterPosition
                );
            };
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> limiter_t
        >
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //delimiting string
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
            , const limiter_t&                          Limiters        //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
        ) noexcept {
            auto limiter = Limiters.cend();

            const auto limiter_pos = ::uns::string::parsing::find<string_t>(
                Source
                , Seeker
                , Limiters
                , limiter
            );

            if (limiter != Limiters.cend()) {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiter
                    , SeekerDelimiterPosition
                    , limiter_pos
                    , limiter_pos + limiter->size()
                );
            }
            else {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiter
                    , SeekerDelimiterPosition
                );
            };
        };
        template<
            ::uns::is_basic_string string_t
            , ::uns::const_iterable_collection<string_t> collection_t
        >
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const collection_t&                       Delimiters      //an iterable collection of delimiting strings
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
            , const string_t&                           Limiter         //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
        ) noexcept {
            const auto limiter_pos = ::uns::string::parsing::find<string_t>(Source, Seeker, Limiter);

            if (limiter_pos != Source.cend()) {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiters
                    , SeekerDelimiterPosition
                    , limiter_pos
                    , limiter_pos + Limiter.size()
                );
            }
            else {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiters
                    , SeekerDelimiterPosition
                );
            };
        };
        template<::uns::is_basic_string string_t>
        bool read(
            const string_t&                             Source          //Source string
            , typename string_t::const_iterator&        Seeker          //positioning Seeker
            , string_t&                                 Fragment        //a Fragment string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //delimiting string
            , const ::uns::string::parsing::seeker_position SeekerDelimiterPosition //defines where to put seeker in case of success
            , const string_t&                           Limiter         //position of the first symbol of the limiter, that serves as the limit for finding Delimiters (including LimiterBegin)
        ) noexcept {
            const auto limiter_pos = ::uns::string::parsing::find<string_t>(
                Source
                , Seeker
                , Limiter
            );

            if (limiter_pos != Source.cend()) {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiter
                    , SeekerDelimiterPosition
                    , limiter_pos
                    , limiter_pos + Limiter.size()
                );
            }
            else {
                return ::uns::string::parsing::read<string_t>(
                    Source
                    , Seeker
                    , Fragment
                    , Delimiter
                    , SeekerDelimiterPosition
                );
            };
        };


        //if the key was found AND seeker successfully placed to proper symbol of it in the target string,
        // reads a substring from the target string starting from the seeker till the first symbol of the delimiter(s)
        //
        //returns true only if the key was found in the target string AND the seeker was placed to the proper symbol of found
        // key successfully AND at least one of the delimiter(s) was found righter than the seeker
        template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> delimiters_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const delimiters_t&                       Delimiters      //an iterable collection of delimiting strings
            , typename string_t::const_iterator         LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for finding delimiters (including right_border_beg)
        ) noexcept {
            auto seeker = Target.cbegin();

            if (
                !::uns::string::parsing::seek<string_t>(
                    Target
                    , seeker
                    , Key
                    , SeekerKeyPosition
                    , LimiterBegin
                    , Target.cend()
                )
            ) {
                return false;
            };

            return ::uns::string::parsing::read<string_t>(
                Target
                , seeker
                , Value
                , Delimiters
                , {
                    .qualifier = ::uns::string::parsing::seeker_position::from_begin
                    , .offset = 0
                }
                , LimiterBegin
                , Target.cend()
            );
        };
        template<::uns::is_basic_string string_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //an iterable collection of delimiting strings
            , typename string_t::const_iterator         LimiterBegin    //position of the first symbol of the limiter, that serves as the limit for finding delimiters (including right_border_beg)
        ) noexcept {
            auto seeker = Target.cbegin();

            if (
                !::uns::string::parsing::seek<string_t>(
                    Target
                    , seeker
                    , Key
                    , SeekerKeyPosition
                    , LimiterBegin
                    , Target.cend()
                )
            ) {
                return false;
            };

            return ::uns::string::parsing::read<string_t>(
                Target
                , seeker
                , Value
                , Delimiter
                , {
                    .qualifier = ::uns::string::parsing::seeker_position::from_begin
                    , .offset = 0
                }
                , LimiterBegin
                , Target.cend()
            );
        };
        template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> delimiters_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const delimiters_t&                       Delimiters      //an iterable collection of delimiting strings
        ) noexcept {
            return ::uns::string::parsing::obtain<string_t>(
                Target
                , Key
                , SeekerKeyPosition
                , Value
                , Delimiters
                , Target.cend()
            );
        };
        template<::uns::is_basic_string string_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //an iterable collection of delimiting strings
        ) noexcept {
            return ::uns::string::parsing::obtain<string_t>(
                Target
                , Key
                , SeekerKeyPosition
                , Value
                , Delimiter
                , Target.cend()
            );
        };
        template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> delimiters_t, ::uns::const_iterable_collection<string_t> limiters_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const delimiters_t&                       Delimiters      //an iterable collection of delimiting strings
            , const limiters_t&                         Limiters        //position of the first symbol of the limiter, that serves as the limit for finding delimiters (including right_border_beg)
        ) noexcept {
            auto pos_limiter = ::uns::string::parsing::find<string_t>(
                Target
                , Target.cbegin()
                , Limiters
            );

            return ::uns::string::parsing::obtain<string_t>(
                Target
                , Key
                , SeekerKeyPosition
                , Value
                , Delimiters
                , pos_limiter
            );
        };
        template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> limiters_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //an iterable collection of delimiting strings
            , const limiters_t&                         Limiters        //position of the first symbol of the limiter, that serves as the limit for finding delimiters (including right_border_beg)
        ) noexcept {
            auto pos_limiter = ::uns::string::parsing::find<string_t>(
                Target
                , Target.cbegin()
                , Limiters
            );

            return ::uns::string::parsing::obtain<string_t>(
                Target
                , Key
                , SeekerKeyPosition
                , Value
                , Delimiter
                , pos_limiter
            );
        };
        template<::uns::is_basic_string string_t, ::uns::const_iterable_collection<string_t> delimiters_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const delimiters_t&                       Delimiters      //an iterable collection of delimiting strings
            , const string_t&                           Limiter         //position of the first symbol of the limiter, that serves as the limit for finding delimiters (including right_border_beg)
        ) noexcept {
            auto pos_limiter = ::uns::string::parsing::find<string_t>(
                Target
                , Target.cbegin()
                , Limiter
            );

            return ::uns::string::parsing::obtain<string_t>(
                Target
                , Key
                , SeekerKeyPosition
                , Value
                , Delimiters
                , pos_limiter
            );
        };
        template<::uns::is_basic_string string_t>
        bool obtain(
            const string_t&                             Target          //target string
            , const string_t&                           Key             //a key wich should be found within the target
            , const ::uns::string::parsing::seeker_position SeekerKeyPosition //defines where to put seeker in case of success
            , string_t&                                 Value           //a result string (in case of fail, it becomes empty)
            , const string_t&                           Delimiter       //an iterable collection of delimiting strings
            , const string_t&                           Limiter         //position of the first symbol of the limiter, that serves as the limit for finding delimiters (including right_border_beg)
        ) noexcept {
            auto pos_limiter = ::uns::string::parsing::find<string_t>(
                Target
                , Target.cbegin()
                , Limiter
            );

            return ::uns::string::parsing::obtain<string_t>(
                Target
                , Key
                , SeekerKeyPosition
                , Value
                , Delimiter
                , pos_limiter
            );
        };

    };
};

#endif
