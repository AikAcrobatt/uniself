
#include "lua_wrapper.hpp"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

//class ::uns::lua::error =>
::uns::lua::error::error(
    const ::uns::lua::errcode& code,
    const ::uns::lua::errtype& type,
    const ::std::string text
) :
    m_text(text),
    m_code(code),
    m_type(type)
{};
::uns::lua::error::error(const ::uns::lua::error& obj) :
    m_text(obj.m_text),
    m_code(obj.m_code),
    m_type(obj.m_type)
{};
::uns::lua::error& ::uns::lua::error::operator=(const ::uns::lua::error& obj) {
    if(this != &obj) {
        m_text = obj.m_text;
        m_code = obj.m_code;
        m_type = obj.m_type;
    };
    return *this;
};
::uns::lua::error::error(::uns::lua::error&& obj) :
    m_text(std::move(obj.m_text)),
    m_code(std::move(obj.m_code)),
    m_type(std::move(obj.m_type))
{};
::uns::lua::error& ::uns::lua::error::operator=(::uns::lua::error&& obj) {
    if(this != &obj) {
        m_text = obj.m_text;
        m_code = obj.m_code;
        m_type = obj.m_type;
    };
    return *this;
};
::uns::lua::error::~error() {};

void ::uns::lua::error::swap(::uns::lua::error& obj) {
    ::std::swap(m_text, obj.m_text);
    ::std::swap(m_code, obj.m_code);
    ::std::swap(m_type, obj.m_type);
};

bool ::uns::lua::error::is() const { return (m_code != ::uns::lua::errcode{ ::uns::lua::errcode::ok }) || (m_type != ::uns::lua::errtype{ ::uns::lua::errtype::ok }); };

::uns::lua::errcode uns::lua::error::code() const { return m_code; };
::uns::lua::errtype uns::lua::error::type() const { return m_type; };
::std::string uns::lua::error::text() const { return m_text; };

::std::u32string uns::lua::error::to_string() const {
    return U"'" + ::uns::string::cast<::std::u32string>(m_text)
        + U"' [err = " + ::uns::string::cast<::std::u32string>(m_code)
        + U", type = " + ::uns::string::cast<::std::u32string>(m_type) + U"]";
};
//<= class ::uns::lua::error


namespace uns::lua::auxiliary {

    ::uns::lua::errcode lua_native_error_to_wrapper(int lua_native_error_code) {
        if(lua_native_error_code == LUA_OK) {
            return ::uns::lua::errcode::ok;
        }
        else if(lua_native_error_code == LUA_YIELD) {
            return ::uns::lua::errcode::yeld;
        }
        else if(lua_native_error_code == LUA_ERRRUN) {
            return ::uns::lua::errcode::errrun;
        }
        else if(lua_native_error_code == LUA_ERRSYNTAX) {
            return ::uns::lua::errcode::errsyntax;
        }
        else if(lua_native_error_code == LUA_ERRMEM) {
            return ::uns::lua::errcode::errmem;
        }
        else if(lua_native_error_code == LUA_ERRERR) {
            return ::uns::lua::errcode::errerr;
        }
        else {
            return ::uns::lua::errcode::errcall;
        };
    };

    class table;

    class state {
    private:
        ::uns::lua::alias::lua_state m_state = nullptr;
        bool m_copied = false;
    public:
        state() :
            m_state(luaL_newstate()),
            m_copied(false)
        {
            luaL_openlibs((m_state));
        };
        state(const ::uns::lua::auxiliary::state& obj) :
            m_state(lua_newthread((obj.m_state))),
            m_copied(true)
        {};
        ::uns::lua::auxiliary::state& operator=(const ::uns::lua::auxiliary::state& obj) {
            if(this != &obj) {
                reset();
                m_state = lua_newthread((obj.m_state));
                m_copied = true;
            };
            return *this;
        };
        state(::uns::lua::auxiliary::state&& obj) :
            m_state(obj.m_state),
            m_copied(obj.m_copied)
        {
            obj.m_state = nullptr;
            obj.m_copied = false;
        };
        ::uns::lua::auxiliary::state& operator=(::uns::lua::auxiliary::state&& obj) {
            if(this != &obj) {
                std::swap(m_state, obj.m_state);
                std::swap(m_copied, obj.m_copied);
            };
            return *this;
        };
        ~state() {
            reset();
        };
    public:
        const ::uns::lua::alias::lua_state get() const { return m_state; };
        ::uns::lua::alias::lua_state get() { return m_state; };
    private:
        void reset() {
            if(m_state != nullptr) {
                if(!m_copied) {
                    lua_close((m_state));
                };
            };
        };
    };

};


//class ::uns::lua::value =>
::uns::lua::value::value(const ::std::string obj) :
    m_type(::uns::lua::value_type::string),
    m_string(obj),
    m_push_function(push_string)
{};
::uns::lua::value::value(const char* obj) : value(::std::string{ obj }) {};
::uns::lua::value::value(const ::std::string_view obj) :
    m_type(::uns::lua::value_type::string),
    m_string(obj),
    m_push_function(push_string)
{};
::uns::lua::value::value(const ::std::u32string obj) : value(::uns::string::cast<::std::string>(obj)) {};
::uns::lua::value::value(const char32_t* obj) : value(::std::u32string{ obj }) {};
::uns::lua::value::value(const ::uns::lua::type::table& obj) :
    m_type(::uns::lua::value_type::table),
    m_table(obj),
    m_push_function(push_table)
{};
::uns::lua::value::value(::uns::lua::type::userdata obj) :
    m_type(::uns::lua::value_type::userdata),
    m_userdata(obj),
    m_push_function(push_userdata)
{};
::uns::lua::value::value(const ::uns::lua::type::nil& obj) :
    m_type(::uns::lua::value_type::nil),
    m_push_function(push_nil)
{};
::uns::lua::value::value(const ::uns::lua::value& obj) :
    m_type(obj.m_type),
    m_boolean(obj.m_boolean),
    m_number(obj.m_number),
    m_integer(obj.m_integer),
    m_string(obj.m_string),
    m_table(obj.m_table),
    m_userdata(obj.m_userdata),
    m_push_function(obj.m_push_function)
{};
::uns::lua::value& ::uns::lua::value::operator=(const ::uns::lua::value& obj) {
    if(this != &obj) {
        m_type = obj.m_type;
        m_boolean = obj.m_boolean;
        m_number = obj.m_number;
        m_integer = obj.m_integer;
        m_string = obj.m_string;
        m_table = obj.m_table;
        m_userdata = obj.m_userdata;
        m_push_function = obj.m_push_function;
    };
    return *this;
};
::uns::lua::value::value(::uns::lua::value&& obj) :
    m_type(std::move(obj.m_type)),
    m_boolean(std::move(obj.m_boolean)),
    m_number(std::move(obj.m_number)),
    m_integer(std::move(obj.m_integer)),
    m_string(std::move(obj.m_string)),
    m_table(std::move(obj.m_table)),
    m_userdata(std::move(obj.m_userdata)),
    m_push_function(std::move(obj.m_push_function))
{};
::uns::lua::value& ::uns::lua::value::operator=(::uns::lua::value&& obj) {
    if(this != &obj) {
        m_type = std::move(obj.m_type);
        m_boolean = std::move(obj.m_boolean);
        m_number = std::move(obj.m_number);
        m_integer = std::move(obj.m_integer);
        m_string = std::move(obj.m_string);
        m_table = std::move(obj.m_table);
        m_userdata = std::move(obj.m_userdata);
        m_push_function = std::move(obj.m_push_function);
    };
    return *this;
};
::uns::lua::value::~value() {};

bool ::uns::lua::value::operator==(const ::uns::lua::value& obj) const {
    return (m_type == obj.m_type)
        && (
            (m_type == ::uns::lua::value_type{ ::uns::lua::value_type::nil })
            || ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::boolean }) && (m_boolean == static_cast<const ::uns::lua::type::boolean&>(obj)))
            || ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::number }) && (m_number == static_cast<const ::uns::lua::type::number&>(obj)))
            || ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::integer }) && (m_integer == static_cast<const ::uns::lua::type::integer&>(obj)))
            || ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::string }) && (m_string == static_cast<const ::uns::lua::type::string&>(obj)))
            || ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::table }) && (m_table == static_cast<const ::uns::lua::type::table&>(obj)))
            || ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::userdata }) && (m_userdata == static_cast<::uns::lua::type::userdata>(obj)))
        );
};
bool ::uns::lua::value::operator!=(const ::uns::lua::value& obj) const {
    return !(*this == obj);
};

#define UNS_LUA_VALUE_CONVERT_DESCRIPTOR(type_identifier)                                        \
::uns::lua::value::operator const ::uns::lua::type::##type_identifier&() const {        \
    return m_##type_identifier;                                                                    \
};                                                                                                \
::uns::lua::value::operator ::uns::lua::type::##type_identifier&() {                    \
    return m_##type_identifier;                                                                    \
};                                                                                                \

UNS_LUA_VALUE_CONVERT_DESCRIPTOR(boolean);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(number);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(integer);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(string);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(table);

::uns::lua::value::operator const ::uns::lua::type::userdata() const {
    return m_userdata;
};
::uns::lua::value::operator ::uns::lua::type::userdata& () {
    return m_userdata;
};

#undef UNS_LUA_VALUE_CONVERT_DESCRIPTOR

void ::uns::lua::value::push_to(::uns::lua::thread& thread) const {
    push_to(thread.m_stack.get());
};
void ::uns::lua::value::push_to(::uns::lua::auxiliary::state& thread) const {
    push_to(thread.get());
};
void ::uns::lua::value::push_to(::uns::lua::alias::lua_state stack) const {
    m_push_function(stack, *this);
};

void ::uns::lua::value::push_nil(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) {
    lua_pushnil((stack));
};
#define UNS_LUA_VALUE_PUSH_DESCRIPTOR(type_identifier)                                                                            \
void ::uns::lua::value::push_##type_identifier(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) {    \
    lua_push##type_identifier((stack), static_cast<::uns::lua::type::##type_identifier>(value));    \
};                                                                                                                                \

UNS_LUA_VALUE_PUSH_DESCRIPTOR(boolean);
UNS_LUA_VALUE_PUSH_DESCRIPTOR(number);
UNS_LUA_VALUE_PUSH_DESCRIPTOR(integer);

void ::uns::lua::value::push_string(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) {
    lua_pushstring((stack), static_cast<::uns::lua::type::string>(value).c_str());
};
void ::uns::lua::value::push_table(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) {
    lua_newtable((stack));

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::number>();
        iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::number>();
        ++iterator
    ) {
        lua_pushnumber((stack), iterator->first);
        iterator->second.push_to(stack);
        lua_settable((stack), -3);
    };

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::integer>();
        iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::integer>();
        ++iterator
    ) {
        lua_pushinteger((stack), iterator->first);
        iterator->second.push_to(stack);
        lua_settable((stack), -3);
    };

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::boolean>();
        iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::boolean>();
        ++iterator
    ) {
        lua_pushboolean((stack), iterator->first);
        iterator->second.push_to(stack);
        lua_settable((stack), -3);
    };

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::string>();
        iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::string>();
        ++iterator
    ) {
        lua_pushstring((stack), iterator->first.c_str());
        iterator->second.push_to(stack);
        lua_settable((stack), -3);
    };
};
void ::uns::lua::value::push_userdata(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) {
    lua_pushlightuserdata((stack), static_cast<::uns::lua::type::userdata>(value));
};

#undef UNS_LUA_VALUE_PUSH_DESCRIPTOR


::std::u32string uns::lua::value::to_string() const {
    switch(type()) {
        default:
        case ::uns::lua::value_type::nil:
        {
            return U"nil";
        }
        case ::uns::lua::value_type::number:
        {
            return ::uns::string::cast<::std::u32string>(static_cast<::uns::lua::type::number>(*this));
        }
        case ::uns::lua::value_type::integer:
        {
            return ::uns::string::cast<::std::u32string>(static_cast<::uns::lua::type::integer>(*this));
        }
        case ::uns::lua::value_type::boolean:
        {
            return ::uns::string::cast<::std::u32string>(static_cast<::uns::lua::type::boolean>(*this));
        }
        case ::uns::lua::value_type::string:
        {
            return U"\"" + ::uns::string::cast<::std::u32string>(static_cast<::uns::lua::type::string>(*this)) + U"\"";
        }
        case ::uns::lua::value_type::table:
        {
            return static_cast<const ::uns::lua::type::table&>(*this).to_string();
        }
        case ::uns::lua::value_type::userdata:
        {
            return U"userdata " + ::uns::string::cast<::std::u32string>(reinterpret_cast<uint64_t>(m_userdata));
        }
    };
};
//<= class ::uns::lua::value


//class ::uns::lua::type::table =>
::uns::lua::type::table::table() : m_ptr(::std::unique_ptr<::uns::lua::auxiliary::table>{ new ::uns::lua::auxiliary::table{} }) {};
::uns::lua::type::table::table(const ::uns::lua::type::table& obj) : m_ptr(new ::uns::lua::auxiliary::table{ *obj.m_ptr }) {};
::uns::lua::type::table& ::uns::lua::type::table::operator=(const ::uns::lua::type::table& obj) {
    if(this != &obj) {
        *m_ptr = *obj.m_ptr;
    };
    return *this;
};
::uns::lua::type::table::table(::uns::lua::type::table&& obj) : m_ptr(std::move(obj.m_ptr)) {};
::uns::lua::type::table& ::uns::lua::type::table::operator=(::uns::lua::type::table&& obj) {
    if(this != &obj) {
        m_ptr = std::move(obj.m_ptr);
    };
    return *this;
};
::uns::lua::type::table::~table() {};

::std::u32string uns::lua::type::table::to_string() const {
    return m_ptr->to_string();
};

bool ::uns::lua::type::table::operator==(const ::uns::lua::type::table& obj) const { return *m_ptr == *obj.m_ptr; };
bool ::uns::lua::type::table::operator!=(const ::uns::lua::type::table& obj) const { return !(*this == obj); };

::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) const { return const_cast<const ::uns::lua::auxiliary::table&>(*m_ptr).operator[](key); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) { return m_ptr->operator[](key); };
::uns::lua::value uns::lua::type::table::operator[] (const char* key) const { return const_cast<const ::uns::lua::auxiliary::table&>(*m_ptr).operator[](::uns::lua::type::string{ key }); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const char* key) { return m_ptr->operator[](::uns::lua::type::string{ key }); };
::uns::lua::value uns::lua::type::table::operator[] (const ::std::u32string& key) const { return const_cast<const ::uns::lua::auxiliary::table&>(*m_ptr).operator[](::uns::string::cast<::uns::lua::type::string>(key)); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::std::u32string& key) { return m_ptr->operator[](::uns::string::cast<::uns::lua::type::string>(key)); };
::uns::lua::value uns::lua::type::table::operator[] (const char32_t* key) const { return this->operator[](::std::u32string{ key }); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const char32_t* key) { return this->operator[](::std::u32string{ key }); };
::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::value& key) const { return const_cast<const ::uns::lua::auxiliary::table&>(*m_ptr).operator[](key); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::value& key) { return m_ptr->operator[](key); };

::std::size_t uns::lua::type::table::size() const { return m_ptr->size(); };
//<= class ::uns::lua::type::table


//class ::uns::lua::auxiliary::table =>
::uns::lua::auxiliary::table::table(const ::uns::lua::auxiliary::table& obj) :
    m_key_number(obj.m_key_number),
    m_key_integer(obj.m_key_integer),
    m_key_boolean(obj.m_key_boolean),
    m_key_string(obj.m_key_string)
{};
::uns::lua::auxiliary::table& ::uns::lua::auxiliary::table::operator=(const ::uns::lua::auxiliary::table& obj) {
    if(this != &obj) {
        m_key_number = obj.m_key_number;
        m_key_integer = obj.m_key_integer;
        m_key_boolean = obj.m_key_boolean;
        m_key_string = obj.m_key_string;
    };

    return *this;
};
::uns::lua::auxiliary::table::table(::uns::lua::auxiliary::table&& obj) :
    m_key_number(std::move(obj.m_key_number)),
    m_key_integer(std::move(obj.m_key_integer)),
    m_key_boolean(std::move(obj.m_key_boolean)),
    m_key_string(std::move(obj.m_key_string))
{};
::uns::lua::auxiliary::table& ::uns::lua::auxiliary::table::operator=(::uns::lua::auxiliary::table&& obj) {
    if(this != &obj) {
        m_key_number = std::move(obj.m_key_number);
        m_key_integer = std::move(obj.m_key_integer);
        m_key_boolean = std::move(obj.m_key_boolean);
        m_key_string = std::move(obj.m_key_string);
    };

    return *this;
};
::uns::lua::auxiliary::table::~table() {}

::std::u32string uns::lua::auxiliary::table::to_string() const {
    return U"table " + ::uns::string::cast<::std::u32string>(reinterpret_cast<uint64_t>(this)); //TODO this convertation Lua-unlike (more human-readable)
};

bool ::uns::lua::auxiliary::table::operator==(const ::uns::lua::auxiliary::table& obj) const {
    return m_key_number == obj.m_key_number
        && m_key_integer == obj.m_key_integer
        && m_key_boolean == obj.m_key_boolean
        && m_key_string == obj.m_key_string
    ;
};
bool ::uns::lua::auxiliary::table::operator!=(const ::uns::lua::auxiliary::table& obj) const { return !(*this == obj); };

#define UNS_LUA_TABLE_IDX_DESCRIPTOR(type_identifier)                                            \
    ::uns::lua::value uns::lua::auxiliary::table::operator[] (const ::uns::lua::type::##type_identifier& key) const {\
        if(auto value_iter = m_key_##type_identifier.find(key); value_iter != m_key_##type_identifier.end()) {\
            return value_iter->second;                                                    \
        }                                                                                \
        else {                                                                            \
            return ::uns::lua::nil;                                                        \
        };                                                                                \
    };                                \
    ::uns::lua::value& ::uns::lua::auxiliary::table::operator[] (const ::uns::lua::type::##type_identifier& key) { return m_key_##type_identifier[key]; };\

UNS_LUA_TABLE_IDX_DESCRIPTOR(number);
UNS_LUA_TABLE_IDX_DESCRIPTOR(integer);
UNS_LUA_TABLE_IDX_DESCRIPTOR(boolean);
UNS_LUA_TABLE_IDX_DESCRIPTOR(string);
#undef UNS_LUA_TABLE_IDX_DESCRIPTOR

::uns::lua::value uns::lua::auxiliary::table::operator[] (const ::uns::lua::value& key) const {
    switch(key.type()) {
        default:
        case ::uns::lua::value_type::nil:
        {
            return ::uns::lua::nil;
        }
        case ::uns::lua::value_type::number:
        {
            return this->operator[](static_cast<::uns::lua::type::number>(key));
        }
        case ::uns::lua::value_type::integer:
        {
            return this->operator[](static_cast<::uns::lua::type::integer>(key));
        }
        case ::uns::lua::value_type::boolean:
        {
            return this->operator[](static_cast<::uns::lua::type::boolean>(key));
        }
        case ::uns::lua::value_type::string:
        {
            return this->operator[](static_cast<::uns::lua::type::string>(key));
        }
    };
};
#pragma warning(push)
#pragma warning(disable: 4172)
::uns::lua::value& ::uns::lua::auxiliary::table::operator[] (const ::uns::lua::value& key) {
    auto nil_replacer = ::uns::lua::value{ ::uns::lua::nil };

    switch(key.type()) {
        default:
        case ::uns::lua::value_type::nil:
        {
            return nil_replacer;
        }
        case ::uns::lua::value_type::number:
        {
            return this->operator[](static_cast<::uns::lua::type::number>(key));
        }
        case ::uns::lua::value_type::integer:
        {
            return this->operator[](static_cast<::uns::lua::type::integer>(key));
        }
        case ::uns::lua::value_type::boolean:
        {
            return this->operator[](static_cast<::uns::lua::type::boolean>(key));
        }
        case ::uns::lua::value_type::string:
        {
            return this->operator[](static_cast<::uns::lua::type::string>(key));
        }
    };
};
#pragma warning(pop)

::std::size_t uns::lua::auxiliary::table::size() const {
    return m_key_number.size() + m_key_integer.size() + m_key_boolean.size() + m_key_string.size();
};
//<= class ::uns::lua::auxiliary::table


::uns::lua::value uns::lua::value::make_from(::uns::lua::alias::lua_state stack, int idx) {
    switch(lua_type((stack), idx)) {
        default:
        case LUA_TNIL:
        {
            return ::uns::lua::nil;
        }
        case LUA_TNUMBER:
        {
            if(lua_isinteger((stack), idx)) {
                return ::uns::lua::value{ lua_tointeger((stack), idx) };
            }
            else {
                return ::uns::lua::value{ lua_tonumber((stack), idx) };
            };
        }
        case LUA_TSTRING:
        {
            return ::uns::lua::value{ static_cast<::uns::lua::type::string>(lua_tostring((stack), idx)) };
        }
        case LUA_TBOOLEAN:
        {
            return ::uns::lua::value{ static_cast<::uns::lua::type::boolean>(lua_toboolean((stack), idx)) };
        }
        case LUA_TTABLE:
        {
            auto res = ::uns::lua::type::table{};

            lua_pushnil((stack));
            while(lua_next((stack), idx) != 0) {
                auto key_idx = lua_gettop((stack)) - 1;
                auto val_idx = lua_gettop((stack));

                switch(lua_type((stack), key_idx)) {
                    default:
                    case LUA_TNIL:
                    {
                        break;
                    }
                    case LUA_TNUMBER:
                    {
                        if(lua_isinteger((stack), key_idx)) {
                            res[static_cast<::uns::lua::type::integer>(lua_tointeger((stack), key_idx))] = ::uns::lua::value::make_from(stack, val_idx);
                        }
                        else {
                            res[static_cast<::uns::lua::type::number>(lua_tonumber((stack), key_idx))] = ::uns::lua::value::make_from(stack, val_idx);
                        };

                        break;
                    }
                    case LUA_TSTRING:
                    {
                        res[static_cast<::uns::lua::type::string>(lua_tostring((stack), key_idx))] = ::uns::lua::value::make_from(stack, val_idx);
                        break;
                    }
                    case LUA_TBOOLEAN:
                    {
                        res[static_cast<::uns::lua::type::boolean>(lua_toboolean((stack), key_idx))] = ::uns::lua::value::make_from(stack, val_idx);
                        break;
                    }
                };

                lua_pop((stack), 1);
            };

            return ::uns::lua::value{ res };
        }
        case LUA_TUSERDATA:
        case LUA_TLIGHTUSERDATA:
        {
            return ::uns::lua::value{ lua_touserdata((stack), idx) };
        }
    };
};
::uns::lua::value uns::lua::value::make_from(::uns::lua::auxiliary::state& thread, int idx) {
    return ::uns::lua::value::make_from(thread.get(), idx);
};


namespace uns::lua::auxiliary {

    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::std::vector<::uns::lua::value>& args) {
        int function_idx = 0;
        
        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        for(const auto& arg : args) {
            arg.push_to(stack);
        };

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name) {
        int function_idx = 0;

        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::uns::lua::value& arg1) {
        int function_idx = 0;

        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        arg1.push_to(stack);

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2) {
        int function_idx = 0;

        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        arg1.push_to(stack);
        arg2.push_to(stack);

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3) {
        int function_idx = 0;

        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        arg1.push_to(stack);
        arg2.push_to(stack);
        arg3.push_to(stack);

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4) {
        int function_idx = 0;

        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        arg1.push_to(stack);
        arg2.push_to(stack);
        arg3.push_to(stack);
        arg4.push_to(stack);

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    ::std::pair<::uns::lua::error, int> prepare(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4, const ::uns::lua::value& arg5) {
        int function_idx = 0;

        if(stack == nullptr) {
            return {
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                function_idx
            };
        };

        lua_getglobal((stack), function_name.c_str());
        function_idx = lua_gettop((stack));

        if(!lua_isfunction((stack), function_idx)) {
            if(!lua_isnil((stack), function_idx)) {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
                    function_idx
                };
            }
            else {
                return {
                    ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                    function_idx
                };
            };
        };

        arg1.push_to(stack);
        arg2.push_to(stack);
        arg3.push_to(stack);
        arg4.push_to(stack);
        arg5.push_to(stack);

        return {
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            function_idx
        };
    };
    
    ::uns::lua::error execute(::uns::lua::alias::lua_state stack, int function_idx, int expected_results) {
        auto args_total = lua_gettop((stack)) - function_idx;
        if(
            int lua_retcode = lua_pcall((stack), args_total, expected_results, 0);
            lua_retcode != LUA_OK
        ) {
            ::std::string err_str = "";
            if(lua_isstring((stack), -1)) {
                err_str = lua_tostring((stack), -1);
            };

            if(
                auto garbage = lua_gettop((stack)) - function_idx;
                garbage > 0
            ) {
                lua_pop((stack), -garbage);
            };

            return ::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
        };

        return {};
    };

    ::std::vector<::uns::lua::value> extract_expected_all(::uns::lua::alias::lua_state stack, int function_idx) {
        auto result = ::std::vector<::uns::lua::value>{};
        if(lua_gettop((stack)) >= function_idx) {
            result.reserve(lua_gettop((stack)) - function_idx + 1);
        }
        else {
            return result;
        };

        for(auto idx = function_idx; idx <= lua_gettop((stack)); ++idx) {
            result.push_back(::uns::lua::value::make_from(stack, idx));
        };

        if(
            auto garbage = lua_gettop((stack)) - function_idx + 1;
            garbage > 0
        ) {
            lua_pop((stack), -garbage);
        };

        return result;
    };
    ::uns::lua::value extract_expected_1(::uns::lua::alias::lua_state stack, int function_idx) {
        auto result = ::uns::lua::value{};
        if(
            constexpr auto idx = 0;
            lua_gettop((stack)) >= idx
        ) {
            result = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            auto garbage = lua_gettop((stack)) - function_idx + 1;
            garbage > 0
        ) {
            lua_pop((stack), -garbage);
        };

        return result;
    };
    ::std::array<::uns::lua::value, 2> extract_expected_2(::uns::lua::alias::lua_state stack, int function_idx) {
        auto result = ::std::array<::uns::lua::value, 2>{};
        if(
            constexpr auto idx = 0;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 1;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            auto garbage = lua_gettop((stack)) - function_idx + 1;
            garbage > 0
        ) {
            lua_pop((stack), -garbage);
        };

        return result;
    };
    ::std::array<::uns::lua::value, 3> extract_expected_3(::uns::lua::alias::lua_state stack, int function_idx) {
        auto result = ::std::array<::uns::lua::value, 3>{};
        if(
            constexpr auto idx = 0;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 1;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 2;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            auto garbage = lua_gettop((stack)) - function_idx + 1;
            garbage > 0
        ) {
            lua_pop((stack), -garbage);
        };

        return result;
    };
    ::std::array<::uns::lua::value, 4> extract_expected_4(::uns::lua::alias::lua_state stack, int function_idx) {
        auto result = ::std::array<::uns::lua::value, 4>{};
        if(
            constexpr auto idx = 0;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 1;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 2;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 3;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            auto garbage = lua_gettop((stack)) - function_idx + 1;
            garbage > 0
        ) {
            lua_pop((stack), -garbage);
        };

        return result;
    };
    ::std::array<::uns::lua::value, 5> extract_expected_5(::uns::lua::alias::lua_state stack, int function_idx) {
        auto result = ::std::array<::uns::lua::value, 5>{};
        if(
            constexpr auto idx = 0;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 1;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 2;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 3;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            constexpr auto idx = 4;
            lua_gettop((stack)) >= function_idx + idx
        ) {
            ::std::get<idx>(result) = ::uns::lua::value::make_from(stack, function_idx + idx);
        };

        if(
            auto garbage = lua_gettop((stack)) - function_idx + 1;
            garbage > 0
        ) {
            lua_pop((stack), -garbage);
        };

        return result;
    };

    ::uns::lua::error load(::uns::lua::alias::lua_state stack, const ::std::u32string& text) {
        if(stack == nullptr) {
            return ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
        };

        auto narrow_text = ::uns::string::cast<::std::string>(text);

        if(int lua_retcode = luaL_loadstring(stack, narrow_text.c_str()); lua_retcode != LUA_OK) {
            std::string err_str = "";

            if(lua_isstring(stack, -1)) {
                err_str = lua_tostring(stack, -1);
                lua_pop(stack, -1);
            };

            return ::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
        };

        return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
    };
    ::uns::lua::error load(::uns::lua::alias::lua_state stack, const ::std::filesystem::path& file) {
        auto narrow_path = ::uns::string::cast<::std::string>(::uns::string::cast<::std::u32string>(file.lexically_normal().native()));

        if(int lua_retcode = luaL_loadfile(stack, narrow_path.c_str()); lua_retcode != LUA_OK) {
            ::std::string err_str = "";

            if(lua_isstring(stack, -1)) {
                err_str = lua_tostring(stack, -1);
                lua_pop(stack, -1);
            };

            return ::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
        };

        return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
    };
    ::uns::lua::error load(::uns::lua::alias::lua_state stack, const ::uns::lua::library& library) {
        if(stack == nullptr) {
            return ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
        };

        if(library.module_name.empty()) {
            return ::uns::lua::error{ ::uns::lua::errcode::errsyntax, ::uns::lua::errtype::invalid, "Module name is empty"};
        };

        if(library.api.size() > 0) {
            lua_getglobal(stack, "package");
            lua_pushstring(stack, "loaded");
            lua_gettable(stack, -2);

            auto module_name = ::uns::string::cast<::std::string>(library.module_name + U"_api");
            lua_pushstring(stack, module_name.c_str());
            lua_newtable(stack);

            for(const auto& entry : library.api) {
                if(entry.name() == nullptr || entry.func() == nullptr) {
                    continue;
                };

                lua_pushstring(stack, entry.name());
                lua_pushcfunction(stack, entry.func());
                lua_settable(stack, -3);
            };

            lua_settable(stack, -3);
            lua_pop(stack, -2);
        };

        if(!library.text.empty()) {
            auto err = ::uns::lua::auxiliary::load(stack, library.text);
            if(err.is()) {
                return err;
            };

            err = ::uns::lua::auxiliary::execute(stack, lua_gettop((stack)), 0);
            if (err.is()) {
                return err;
            };
        };

        return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
    };

    ::std::pair<::uns::lua::error, ::uns::lua::value> global_get(::uns::lua::alias::lua_state stack, const ::std::string& global_name) {
        using result_t = ::std::pair<::uns::lua::error, ::uns::lua::value>;

        if(stack == nullptr) {
            return result_t{
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
                ::uns::lua::nil
            };
        };

        lua_getglobal((stack), global_name.c_str());
        auto global_idx = lua_gettop((stack));

        if(lua_isnil((stack), global_idx)) {
            return result_t{
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
                ::uns::lua::nil
            };
        };

        if(
            !(
                lua_isnil((stack), global_idx)
                || lua_isnumber((stack), global_idx)
                || lua_isinteger((stack), global_idx)
                || lua_isboolean((stack), global_idx)
                || lua_isstring((stack), global_idx)
                || lua_istable((stack), global_idx)
                || lua_isuserdata((stack), global_idx)
                || lua_islightuserdata((stack), global_idx)
            )
        ) {
            return result_t{
                ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::unrepresentable },
                ::uns::lua::nil
            };
        };

        return result_t{
            ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
            ::uns::lua::value::make_from(stack, global_idx)
        };
    };
    ::uns::lua::error global_set(::uns::lua::alias::lua_state stack, const ::std::string& global_name, const ::uns::lua::value& value) {
        if(stack == nullptr) {
            return ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
        };

        value.push_to(stack);
        lua_setglobal((stack), global_name.c_str());

        return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
    };

    void gc(::uns::lua::alias::lua_state stack) {
        if(stack != nullptr) {
            lua_gc((stack), LUA_GCCOLLECT);
        };
    };
};


//class ::uns::lua::function =>
::uns::lua::function::function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name) :
    m_function_name(lua_global_function_name),
    m_stack_wrapper(),
    m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state{} }) 
{
    if(lua_script != nullptr) {
        *m_stack = *lua_script;

        int function_idx = 0;

        ::std::tie(m_err, function_idx) = ::uns::lua::auxiliary::prepare(
            m_stack->get()
            , ::uns::string::cast<::std::string>(lua_global_function_name)
        );

        lua_pop(m_stack->get(), function_idx);
    };
};
::uns::lua::function::function(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_function_name) :
    m_function_name(lua_global_function_name),
    m_stack_wrapper(lua_script),
    m_stack(nullptr) 
{
    int function_idx = 0;

    ::std::tie(m_err, function_idx) = ::uns::lua::auxiliary::prepare(
        m_stack_wrapper.get()
        , ::uns::string::cast<::std::string>(lua_global_function_name)
    );

    lua_pop(m_stack->get(), function_idx);
};
::uns::lua::function::~function() {
    if(valid()) {
        gc();
    };
};
::std::u32string uns::lua::function::name() const { return ::uns::string::cast<::std::u32string>(m_function_name); };

bool ::uns::lua::function::valid() const {
    bool result = (m_stack != nullptr || m_stack_wrapper.valid());
    if(!result) return false;

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    lua_getglobal((state), m_function_name.c_str());
    result = lua_isfunction((state), lua_gettop((state)));
    lua_pop((state), -1);

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name, args);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name, arg1);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name, arg1, arg2);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name, arg1, arg2, arg3);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name, arg1, arg2, arg3, arg4);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};
::uns::lua::function::results uns::lua::function::operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4, const ::uns::lua::value& arg5) {
    auto result = ::uns::lua::function::results{};
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
        result.m_stack = m_stack;
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
        result.m_stack_wrapper = m_stack_wrapper;
    };

    ::std::tie(m_err, result.m_function_idx) = ::uns::lua::auxiliary::prepare(state, m_function_name, arg1, arg2, arg3, arg4, arg5);

    if(m_err.is()) {
        result = {};
        return result;
    };

    m_err = ::uns::lua::auxiliary::execute(state, result.m_function_idx, expected_results);
    if(m_err.is()) {
        result = {};
    };

    return result;
};

void uns::lua::function::gc() {
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    ::uns::lua::auxiliary::gc(state);
};
//<= class ::uns::lua::function
 
//class ::uns::lua::function::results =>
::std::vector<::uns::lua::value> uns::lua::function::results::get_all() const {
    if(!valid()) return {};

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    return ::uns::lua::auxiliary::extract_expected_all(state, m_function_idx);
};
::uns::lua::value uns::lua::function::results::get() const {
    if(!valid()) return {};

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    return ::uns::lua::auxiliary::extract_expected_1(state, m_function_idx);

};
::std::array<::uns::lua::value, 2> uns::lua::function::results::get_2() const {
    if(!valid()) return {};

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    return ::uns::lua::auxiliary::extract_expected_2(state, m_function_idx);

};
::std::array<::uns::lua::value, 3> uns::lua::function::results::get_3() const {
    if(!valid()) return {};

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    return ::uns::lua::auxiliary::extract_expected_3(state, m_function_idx);

};
::std::array<::uns::lua::value, 4> uns::lua::function::results::get_4() const {
    if(!valid()) return {};

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    return ::uns::lua::auxiliary::extract_expected_4(state, m_function_idx);

};
::std::array<::uns::lua::value, 5> uns::lua::function::results::get_5() const {
    if(!valid()) return {};

    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    return ::uns::lua::auxiliary::extract_expected_5(state, m_function_idx);
};
//<= class ::uns::lua::function::results

//class ::uns::lua::global =>
::uns::lua::global::global(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_variable_name) :
    m_global_name(lua_global_variable_name),
    m_stack_wrapper(),
    m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state}) {
    if(lua_script != nullptr) {
        *m_stack = *lua_script;
    };
};
::uns::lua::global::global(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_variable_name) :
    m_global_name(lua_global_variable_name),
    m_stack_wrapper(lua_script),
    m_stack(nullptr)
{};
::uns::lua::global& ::uns::lua::global::operator=(const ::uns::lua::value & value) {
    set(value);
    return *this;
};
::uns::lua::global::~global() {
    if(valid()) {
        gc();
    };
};
::std::u32string uns::lua::global::name() const { return ::uns::string::cast<::std::u32string>(m_global_name); };

::uns::lua::value uns::lua::global::get() const {
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    auto [l_err, result] = ::uns::lua::auxiliary::global_get(state, m_global_name);

    m_err = l_err;

    return result;
};
void uns::lua::global::set(const ::uns::lua::value& value) {
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    m_err = ::uns::lua::auxiliary::global_set(state, m_global_name, value);
};

void uns::lua::global::gc() {
    ::uns::lua::alias::lua_state state = nullptr;
    if(m_stack != nullptr) {
        state = m_stack->get();
    }
    else if(m_stack_wrapper.valid()) {
        state = m_stack_wrapper.get();
    };

    ::uns::lua::auxiliary::gc(state);
};
//<= class ::uns::lua::global


//class ::uns::lua::thread =>
::uns::lua::thread::thread(::uns::lua::alias::lua_state stack) :
    m_stack(stack)
{};
::uns::lua::thread::thread(const ::uns::lua::thread& obj) :
    m_stack(obj.m_stack)
{};
::uns::lua::thread& ::uns::lua::thread::operator=(const ::uns::lua::thread& obj) {
    if(this == &obj) return *this;

    m_stack = obj.m_stack;

    return *this;
};
::uns::lua::thread::thread(::uns::lua::thread&& obj) :
    m_stack(::std::move(obj.m_stack))
{};
::uns::lua::thread& ::uns::lua::thread::operator=(::uns::lua::thread&& obj) {
    if(this == &obj) return *this;

    m_stack = ::std::move(obj.m_stack);

    return *this;
};

void ::uns::lua::thread::load(const ::uns::lua::library& library) {
    m_err = ::uns::lua::auxiliary::load(m_stack.get(), library);
};
void ::uns::lua::thread::load(const ::std::u32string& text) {
    m_err = ::uns::lua::auxiliary::load(m_stack.get(), text);
};
void ::uns::lua::thread::load(const ::std::filesystem::path& file) {
    m_err = ::uns::lua::auxiliary::load(m_stack.get(), file);
};

void ::uns::lua::thread::call(int results_expected_total) {
    m_err = ::uns::lua::auxiliary::execute(
        m_stack.get(),
        lua_gettop(m_stack.get()),
        results_expected_total
    );
};

::uns::lua::function uns::lua::thread::get_function(const ::std::u32string& lua_global_function_name) {
    if(!valid()) {
        m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::cast<::std::string>(lua_global_function_name) };
        return ::uns::lua::function{};
    }
    else {
        return ::uns::lua::function{ m_stack, ::uns::string::cast<::std::string>(lua_global_function_name) };
    };
};
::uns::lua::global uns::lua::thread::get_global(const ::std::u32string& lua_global_variable_name) {
    if(!valid()) {
        m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::cast<::std::string>(lua_global_variable_name) };
        return ::uns::lua::global{};
    }
    else {
        return ::uns::lua::global{ m_stack, ::uns::string::cast<::std::string>(lua_global_variable_name) };
    };
};

::std::size_t uns::lua::thread::size() const {
    if(valid()) {
        return static_cast<::std::size_t>(lua_gettop((m_stack.get())));
    }
    else {
        return 0;
    };
};
::uns::lua::value uns::lua::thread::get_value(int input_index) {
    if(valid()) {
        return ::uns::lua::value::make_from(m_stack.get(), input_index);
    }
    else {
        return 0;
    };
};

int ::uns::lua::thread::raise_error(const ::std::string& errtext) {
    luaL_error((m_stack.get()), errtext.c_str());
    return 0;
};
int ::uns::lua::thread::raise_error(const ::std::u32string& errtext) {
    auto l_errtext = ::uns::string::cast<::std::string>(errtext);
    return raise_error(l_errtext);
};

void ::uns::lua::thread::gc() {
    if(valid()) {
        lua_gc((m_stack.get()), LUA_GCCOLLECT);
    };
};
//<= ::uns::lua::thread


::uns::lua::lib_entry::lib_entry(const ::std::u32string& name, ::uns::lua::alias::lua_cfunction lua_function) :
    m_name(::uns::string::cast<::std::string>(name)),
    m_function(lua_function)
{};


//class ::uns::lua::script =>
::uns::lua::script::script() : m_stack(new ::uns::lua::auxiliary::state{}) {};
::uns::lua::script::script(const ::std::u32string& text) : m_stack(new ::uns::lua::auxiliary::state{}) {
    load(text);
};
::uns::lua::script::script(const ::std::filesystem::path& file) : m_stack(new ::uns::lua::auxiliary::state{}) {
    load(file);
};
::uns::lua::script::script(const ::uns::lua::library& library) : m_stack(new ::uns::lua::auxiliary::state{}) {
    load(library);
};
::uns::lua::script::script(const ::uns::lua::script& obj) :
    m_stack(obj.m_stack),
    m_err(obj.m_err)
{};
::uns::lua::script& ::uns::lua::script::operator=(const ::uns::lua::script& obj) {
    if(this != &obj) {
        m_stack = obj.m_stack;
        m_err = obj.m_err;
    };
    return *this;
};
::uns::lua::script::script(::uns::lua::script&& obj) :
    m_stack(::std::move(obj.m_stack)),
    m_err(::std::move(obj.m_err))
{};
::uns::lua::script& ::uns::lua::script::operator=(::uns::lua::script&& obj) {
    if(this != &obj) {
        m_stack = ::std::move(obj.m_stack);
        m_err = ::std::move(obj.m_err);
    };
    return *this;
};
::uns::lua::script::~script() {};

void ::uns::lua::script::load(const ::uns::lua::library& library) {
    m_err = ::uns::lua::auxiliary::load(m_stack->get(), library);
};
void ::uns::lua::script::load(const ::std::u32string& text) {
    m_err = ::uns::lua::auxiliary::load(m_stack->get(), text);
};
void ::uns::lua::script::load(const ::std::filesystem::path& file) {
    m_err = ::uns::lua::auxiliary::load(m_stack->get(), file);
};


::uns::lua::function uns::lua::script::get_function(const ::std::u32string& lua_global_function_name) {
    if(!valid()) {
        m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::cast<::std::string>(lua_global_function_name) };
        return ::uns::lua::function{};
    }
    else {
        return ::uns::lua::function{ m_stack, ::uns::string::cast<::std::string>(lua_global_function_name) };
    };
};
::uns::lua::global uns::lua::script::get_global(const ::std::u32string& lua_global_variable_name) {
    if(!valid()) {
        m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::cast<::std::string>(lua_global_variable_name) };
        return ::uns::lua::global{};
    }
    else {
        return ::uns::lua::global{ m_stack, ::uns::string::cast<::std::string>(lua_global_variable_name) };
    };
};

void uns::lua::script::run() {
    m_err = ::uns::lua::error{};

    if(!valid()) {
        m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
        return;
    };

    m_err = ::uns::lua::auxiliary::execute(m_stack->get(), lua_gettop(m_stack->get()), 0);

    return;
};

void uns::lua::script::gc() {
    if(valid()) {
        lua_gc((m_stack->get()), LUA_GCCOLLECT);
    };
};

long double uns::lua::script::total_memory() const {
    if(valid()) {
        long double res = lua_gc((m_stack->get()), LUA_GCCOUNT);
        res += static_cast<long double>(lua_gc((m_stack->get()), LUA_GCCOUNTB)) / 1024.0;
        return res;
    }
    else {
        return 0;
    };
};
//<= class ::uns::lua::script

::uns::lua::value uns::lua::make_table() { return ::uns::lua::type::table{}; };
