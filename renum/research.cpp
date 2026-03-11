
#include <iostream>

/*
UNS_RENUM_DECLARATOR(renum_name, underline_type,
    renum_identifier_0001 = -128,
    renum_identifier_2  = 0,
    renum_identifier__03,
    renum_identifier_10 = 10
);

renum_name val = renum_name::renum_identifier_2;

*/

using underline_type = int;

//renum-specific macros

#define U8INSTRUCTION(arg) u8#arg
#define U8LIT(arg) U8INSTRUCTION(##arg)

#define RENUM_SEGMENT_FIRST(apl, renum_val) apl##renum_val
#define RENUM_SEGMENT(apl, renum_val) , apl##renum_val

#define RENUM_TOSTRING_SEGMENT(apl, renum_val)\
case apl##renum_val: { return U8LIT(apl##renum_val); }

#define RENUM_FROMSTRING_SEGMENT(apl, renum_val)\
if (Str == U8LIT(apl##renum_val)) { return apl##renum_val; } else

//enum contents support

#define RENUM_EMPTY(...)
#define RENUM_EXPAND(x) x
#define RENUM_GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define RENUM_FOR_EACH(apl, macro, ...) \
    RENUM_EXPAND(RENUM_GET_MACRO(__VA_ARGS__, RENUM_FOR_4, RENUM_FOR_3, RENUM_FOR_2, RENUM_FOR_1)(apl, macro, __VA_ARGS__))

#define RENUM_FOR_1(a, m, x) m(a, x)
#define RENUM_FOR_2(a, m, x, ...) m(a, x) RENUM_EXPAND(RENUM_FOR_1(a, m, __VA_ARGS__))
#define RENUM_FOR_3(a, m, x, ...) m(a, x) RENUM_EXPAND(RENUM_FOR_2(a, m, __VA_ARGS__))
#define RENUM_FOR_4(a, m, x, ...) m(a, x) RENUM_EXPAND(RENUM_FOR_3(a, m, __VA_ARGS__))

#define RENUM_COUNTER(...)\
    RENUM_EXPAND(RENUM_GET_MACRO(__VA_ARGS__, 4, 3, 2, 1))

//enum value definition support

#define RENUM_FOR_EACH_WITH_FIRST(apl, first_macro, repetitive_macro, first, ...)\
    RENUM_FOR_1(apl, first_macro, first)\
    RENUM_FOR_EACH(apl, repetitive_macro, __VA_ARGS__)

#define RENUM_PICK_FIRST(arg1, ...) arg1

#define RENUM_GET_MACRO_OFPAIR(_1, _2,  NAME, ...) NAME

#define RENUM_MAKE_EQUALITY(arg1, ...)\
    arg1 RENUM_GET_MACRO_OFPAIR(,__VA_ARGS__, = __VA_ARGS__)

#include <vector>

class renum_name {
public:
    using integral = underline_type;
public:
    enum enum_type : integral {
        RENUM_FOR_EACH_WITH_FIRST(
            RENUM_MAKE_EQUALITY,
            RENUM_SEGMENT_FIRST,
            RENUM_SEGMENT,
            (renum_identifier_0001),
            (renum_identifier_2),
            (renum_identifier__03, -127),
            (renum_identifier_10)
        )
    };
    inline static constexpr ::std::size_t s_size = RENUM_COUNTER(
        (renum_identifier_0001),
        (renum_identifier_2),
        (renum_identifier__03, -127),
        (renum_identifier_10)
    );
protected:
    enum_type m_value = RENUM_FOR_EACH_WITH_FIRST(
        RENUM_PICK_FIRST,
        RENUM_SEGMENT_FIRST,
        RENUM_EMPTY,
        (renum_identifier_0001),
        (renum_identifier_2),
        (renum_identifier__03, -127),
        (renum_identifier_10)
    );
public:
    constexpr renum_name() noexcept : m_value{
        RENUM_FOR_EACH_WITH_FIRST(
            RENUM_PICK_FIRST,
            RENUM_SEGMENT_FIRST,
            RENUM_EMPTY,
            (renum_identifier_0001),
            (renum_identifier_2),
            (renum_identifier__03, -127),
            (renum_identifier_10)
        )
    } {};
    constexpr renum_name(enum_type EnumVal) noexcept : m_value{ EnumVal } {};
    constexpr renum_name(const renum_name& Obj) noexcept : m_value{ Obj.m_value } {};
    constexpr renum_name& operator=(const renum_name& Obj) noexcept {
        if (this == &Obj) return *this;

        m_value = Obj.m_value;

        return *this;
    };
    constexpr renum_name(renum_name&& Obj) noexcept : m_value{ ::std::move(Obj.m_value) } {};
    constexpr renum_name& operator=(renum_name&& Obj) noexcept {
        if (this == &Obj) return *this;

        m_value = ::std::move(Obj.m_value);

        return *this;
    };
    constexpr ~renum_name() noexcept {};
public:
    explicit constexpr operator enum_type() const noexcept { return m_value; };
public:
    constexpr static ::std::size_t size() noexcept { return s_size; };
    constexpr static ::std::vector<renum_name> values() noexcept {
        return {
            RENUM_FOR_EACH_WITH_FIRST(
                RENUM_PICK_FIRST,
                RENUM_SEGMENT_FIRST,
                RENUM_SEGMENT,
                (renum_identifier_0001),
                (renum_identifier_2),
                (renum_identifier__03, -127),
                (renum_identifier_10)
            )
        };
    };
public:
    constexpr ::std::u8string to_string() const {
        switch (m_value) {
            RENUM_FOR_EACH(
                RENUM_PICK_FIRST,
                RENUM_TOSTRING_SEGMENT,
                (renum_identifier_0001),
                (renum_identifier_2),
                (renum_identifier__03, -127),
                (renum_identifier_10)
            )
            default: {
                throw ::std::runtime_error{ "TODO" };
            }
        };
    };
    constexpr static renum_name from_string(const ::std::u8string_view& Str) {
        RENUM_FOR_EACH(
            RENUM_PICK_FIRST,
            RENUM_FROMSTRING_SEGMENT,
            (renum_identifier_0001),
            (renum_identifier_2),
            (renum_identifier__03, -127),
            (renum_identifier_10)
        )
        {
            throw ::std::runtime_error{ "TODO" };
        };
    };
};


int main() {
    ::std::cout << "START\n";

    renum_name val = renum_name::renum_identifier_10;

    ::std::u8string str_val = val.to_string();

    ::std::cout << renum_name::size() << ::std::endl;

    ::std::cout << "FINISH\n";
};
