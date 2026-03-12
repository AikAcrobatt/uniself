
#include <iostream>

//enum contents support

#define UNS_RENUM_OBTAIN_FIRST(first, ...) first
#define UNS_RENUM_MAKE_EQUALITY(arg1, arg2) arg1 arg2

#define UNS_RENUM_EXPAND(x) x
#define UNS_RENUM_FOR_FIRST(macro, ...) \
    UNS_RENUM_EXPAND(macro)UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST(__VA_ARGS__))
#define UNS_RENUM_GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define UNS_RENUM_FOR_EACH(macro, delimiter, ...) \
    UNS_RENUM_EXPAND(UNS_RENUM_GET_MACRO(__VA_ARGS__, UNS_RENUM_FOR_4, UNS_RENUM_FOR_3, UNS_RENUM_FOR_2, UNS_RENUM_FOR_1)(macro, delimiter, __VA_ARGS__))

#define UNS_RENUM_FOR_1(m, d, x) m(x)
#define UNS_RENUM_FOR_2(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_1(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_3(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_2(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_4(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_3(m, d, __VA_ARGS__))

#define UNS_RENUM_COUNTER(...)\
    UNS_RENUM_EXPAND(UNS_RENUM_GET_MACRO(__VA_ARGS__, 4, 3, 2, 1))

//renum-specific macros

#define UNS_U8INSTRUCTION(arg) u8#arg
#define UNS_U8LITERAL(arg) UNS_U8INSTRUCTION(##arg)

#define UNS_RENUM_DELIMITER_NONE()
#define UNS_RENUM_DELIMITER_COMMA() ,

#define UNS_RENUM_SEGMENT(renum_pair) UNS_RENUM_EXPAND(UNS_RENUM_MAKE_EQUALITY)renum_pair
#define UNS_RENUM_VALUE(renum_pair) UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST)renum_pair

#define UNS_RENUM_TOSTRING_SEGMENT_INNER(arg)\
    case arg: { return UNS_U8LITERAL(arg); }
#define UNS_RENUM_TOSTRING_SEGMENT(renum_pair)\
    UNS_RENUM_TOSTRING_SEGMENT_INNER(UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST)renum_pair)

#define UNS_RENUM_FROMSTRING_SEGMENT_INNER(arg)\
    if (Str == UNS_U8LITERAL(arg)) { return arg; } else
#define UNS_RENUM_FROMSTRING_SEGMENT(renum_pair)\
    UNS_RENUM_FROMSTRING_SEGMENT_INNER(UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST)renum_pair)

#include <vector>

#define UNS_RENUM(renum_name, underlying_type, ...)\
class renum_name {\
public:\
    using integral_type = underlying_type;\
public:\
    enum enum_type : integral_type {\
        UNS_RENUM_FOR_EACH(\
            UNS_RENUM_SEGMENT,\
            UNS_RENUM_DELIMITER_COMMA,\
            __VA_ARGS__\
        )\
    };\
    inline static constexpr ::std::size_t s_size = UNS_RENUM_COUNTER(\
        __VA_ARGS__\
    );\
protected:\
    enum_type m_value = UNS_RENUM_FOR_FIRST(\
        UNS_RENUM_OBTAIN_FIRST,\
        __VA_ARGS__\
    );\
public:\
    inline constexpr renum_name() noexcept {};\
    inline constexpr renum_name(enum_type EnumVal) noexcept : m_value{ EnumVal } {};\
    inline constexpr renum_name(const renum_name& Obj) noexcept : m_value{ Obj.m_value } {};\
    inline constexpr renum_name& operator=(const renum_name& Obj) noexcept {\
        if (this == &Obj) return *this;\
        m_value = Obj.m_value;\
        return *this;\
    };\
    inline constexpr renum_name(renum_name&& Obj) noexcept : m_value{ ::std::move(Obj.m_value) } {};\
    inline constexpr renum_name& operator=(renum_name&& Obj) noexcept {\
        if (this == &Obj) return *this;\
        m_value = ::std::move(Obj.m_value);\
        return *this;\
    };\
    inline constexpr ~renum_name() noexcept {};\
public:\
    inline constexpr bool operator==(const renum_name& arg) const noexcept { return m_value == arg.m_value; };\
    inline constexpr bool operator!=(const renum_name& arg) const noexcept { return !(m_value == arg.m_value); };\
public:\
    inline explicit constexpr operator enum_type() const noexcept { return m_value; };\
    inline explicit constexpr operator integral_type() const noexcept { return m_value; };\
public:\
    inline constexpr static ::std::size_t size() noexcept { return s_size; };\
    inline constexpr static ::std::vector<renum_name> values() {/*TODO*/\
        return {\
            UNS_RENUM_FOR_EACH(\
                UNS_RENUM_VALUE,\
                UNS_RENUM_DELIMITER_COMMA,\
                __VA_ARGS__\
            )\
        };\
    };\
public:\
    inline constexpr ::std::u8string to_string() const {\
        switch (m_value) {\
            UNS_RENUM_FOR_EACH(\
                UNS_RENUM_TOSTRING_SEGMENT,\
                UNS_RENUM_DELIMITER_NONE,\
                __VA_ARGS__\
            )\
            default: {\
                throw ::std::runtime_error{ "TODO" };\
            }\
        };\
    };\
    inline constexpr static renum_name from_string(const ::std::u8string_view& Str) {\
        UNS_RENUM_FOR_EACH(\
            UNS_RENUM_FROMSTRING_SEGMENT,\
            UNS_RENUM_DELIMITER_NONE,\
            __VA_ARGS__\
        )\
        {\
            throw ::std::runtime_error{ "TODO" };\
        };\
    };\
};\

UNS_RENUM(some, long int,
    (bcu, = -1229),
    (__, ),
    (abc, ),
    (engcef, )
);

int main() {
    ::std::cout << "START\n";

    some eval;

    auto strs = ::std::vector<::std::u8string>{};
    for (auto renum_val : some::values()) {
        strs.emplace_back(renum_val.to_string());
    };

    ::std::cout << "FINISH\n";
};
