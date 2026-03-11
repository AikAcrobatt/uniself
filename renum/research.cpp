
#include <iostream>

using underline_type = int;

//renum-specific macros

#define U8INSTRUCTION(arg) u8#arg
#define U8LIT(arg) U8INSTRUCTION(##arg)

#define RENUM_OBTAIN_FIRST(first, ...) first
#define RENUM_SEGMENT(apl, renum_val) apl##renum_val,

#define RENUM_TOSTRING_SEGMENT_INNER(arg)\
    case arg: { return U8LIT(arg); }
#define RENUM_TOSTRING_SEGMENT(apl, renum_val)\
    RENUM_TOSTRING_SEGMENT_INNER(RENUM_EXPAND(apl##renum_val))

#define RENUM_FROMSTRING_SEGMENT_INNER(arg)\
    if (Str == U8LIT(arg)) { return arg; } else
#define RENUM_FROMSTRING_SEGMENT(apl, renum_val)\
    RENUM_FROMSTRING_SEGMENT_INNER(RENUM_EXPAND(apl##renum_val))

//enum contents support

#define RENUM_EXPAND(x) x
#define RENUM_GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define RENUM_FOR_EACH(apl, macro, ...) \
    RENUM_EXPAND(RENUM_GET_MACRO(__VA_ARGS__, RENUM_FOR_4, RENUM_FOR_3, RENUM_FOR_2, RENUM_FOR_1)(apl, macro, __VA_ARGS__))

#define RENUM_COMPOUND(m, x) m##x
#define RENUM_FOR_1(a, m, x) m(a, x)
#define RENUM_FOR_2(a, m, x, ...) m(a, x) RENUM_EXPAND(RENUM_FOR_1(a, m, __VA_ARGS__))
#define RENUM_FOR_3(a, m, x, ...) m(a, x) RENUM_EXPAND(RENUM_FOR_2(a, m, __VA_ARGS__))
#define RENUM_FOR_4(a, m, x, ...) m(a, x) RENUM_EXPAND(RENUM_FOR_3(a, m, __VA_ARGS__))

#define RENUM_COUNTER(...)\
    RENUM_EXPAND(RENUM_GET_MACRO(__VA_ARGS__, 4, 3, 2, 1))

//enum value definition support

#define RENUM_PICK_FIRST(arg1, ...) arg1
#define RENUM_MAKE_EQUALITY(arg1, arg2) arg1 arg2

#include <vector>

#define UNS_RENUM(renum_name, underlying_type, ...)\
class renum_name {\
public:\
    using integral = underlying_type;\
public:\
    enum enum_type : integral {\
        RENUM_FOR_EACH(\
            RENUM_MAKE_EQUALITY,\
            RENUM_SEGMENT,\
            __VA_ARGS__\
        )\
    };\
    inline static constexpr ::std::size_t s_size = RENUM_COUNTER(\
        __VA_ARGS__\
    );\
protected:\
    enum_type m_value = static_cast<enum_type>(values()[0]);\
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
    inline explicit constexpr operator integral() const noexcept { return m_value; };\
public:\
    inline constexpr static ::std::size_t size() noexcept { return s_size; };\
    inline constexpr static ::std::vector<renum_name> values() noexcept {/*TODO*/\
        return {\
            RENUM_FOR_EACH(\
                RENUM_PICK_FIRST,\
                RENUM_SEGMENT,\
                __VA_ARGS__\
            )\
        };\
    };\
public:\
    inline constexpr ::std::u8string to_string() const {\
        switch (m_value) {\
            RENUM_FOR_EACH(\
                RENUM_PICK_FIRST,\
                RENUM_TOSTRING_SEGMENT,\
                __VA_ARGS__\
            )\
            default: {\
                throw ::std::runtime_error{ "TODO" };\
            }\
        };\
    };\
    inline constexpr static renum_name from_string(const ::std::u8string_view& Str) {\
        RENUM_FOR_EACH(\
            RENUM_PICK_FIRST,\
            RENUM_FROMSTRING_SEGMENT,\
            __VA_ARGS__\
        )\
        {\
            throw ::std::runtime_error{ "TODO" };\
        };\
    };\
};\

UNS_RENUM(some, long int,
    (abc, ),
    (bcu, = -1229),
    (__, ),
    (engcef, )
);

int main() {
    ::std::cout << "START\n";

    auto strs = ::std::vector<::std::u8string>{};
    for (auto renum_val : some::values()) {
        strs.emplace_back(renum_val.to_string());
    };

    ::std::cout << "FINISH\n";
};
