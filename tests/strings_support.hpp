#pragma once

#ifndef UNS_TESTS_STRINGS
#define UNS_TESTS_STRINGS "strings_support.hpp"

#include <string>

namespace uns::tests {

    class u8string_wrapper : public ::std::u8string {
    public:
        friend inline ::std::ostream& operator<<(::std::ostream& os, const u8string_wrapper& str) {
            return os << ::std::string{ reinterpret_cast<const char*>(str.c_str()) };
        };

        inline constexpr operator const ::std::u8string& () const& noexcept { return dynamic_cast<const ::std::u8string&>(*this); };
        inline constexpr operator ::std::u8string& () & noexcept { return dynamic_cast<::std::u8string&>(*this); };
        inline constexpr operator ::std::u8string_view() const noexcept { return dynamic_cast<const ::std::u8string&>(*this).operator ::std::u8string_view(); };
        inline constexpr operator const char8_t*() const noexcept { return c_str(); };
    };

    inline constexpr ::uns::tests::u8string_wrapper make_u8(const char8_t* cstr) noexcept {
        return ::uns::tests::u8string_wrapper{ cstr };
    };
    inline constexpr ::uns::tests::u8string_wrapper make_u8(const ::std::u8string& str) noexcept {
        return ::uns::tests::u8string_wrapper{ str };
    };
};

/*inline ::std::ostream& operator<<(::std::ostream& os, const ::uns::tests::u8string_wrapper& Str) {
    return os << ::std::string{ reinterpret_cast<const char*>(Str.c_str()) };
};*/
inline ::std::ostream& operator<<(::std::ostream& os, const ::std::u8string_view& Str) {
    return os << ::std::string{ reinterpret_cast<const char*>(Str.data()) };
};

#endif
