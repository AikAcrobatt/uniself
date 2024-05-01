
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
) noexcept :
	m_text(text),
	m_code(code),
	m_type(type)
{};
::uns::lua::error::error(const ::uns::lua::error& obj) noexcept :
	m_text(obj.m_text),
	m_code(obj.m_code),
	m_type(obj.m_type)
{};
::uns::lua::error& ::uns::lua::error::operator=(const ::uns::lua::error& obj) noexcept {
	if(this != &obj) {
		m_text = obj.m_text;
		m_code = obj.m_code;
		m_type = obj.m_type;
	};
	return *this;
};
::uns::lua::error::error(::uns::lua::error&& obj) noexcept :
	m_text(std::move(obj.m_text)),
	m_code(std::move(obj.m_code)),
	m_type(std::move(obj.m_type))
{};
::uns::lua::error& ::uns::lua::error::operator=(::uns::lua::error&& obj) noexcept {
	if(this != &obj) {
		m_text = obj.m_text;
		m_code = obj.m_code;
		m_type = obj.m_type;
	};
	return *this;
};
::uns::lua::error::~error() noexcept {};

void ::uns::lua::error::swap(::uns::lua::error& obj) noexcept {
	::std::swap(m_text, obj.m_text);
	::std::swap(m_code, obj.m_code);
	::std::swap(m_type, obj.m_type);
};

bool ::uns::lua::error::is() const noexcept { return (m_code != ::uns::lua::errcode{ ::uns::lua::errcode::ok }) || (m_type != ::uns::lua::errtype{ ::uns::lua::errtype::ok }); };

::uns::lua::errcode uns::lua::error::code() const noexcept { return m_code; };
::uns::lua::errtype uns::lua::error::type() const noexcept { return m_type; };
::std::string uns::lua::error::text() const noexcept { return m_text; };

::std::u8string uns::lua::error::to_string() const {
	return u8"'" + ::uns::string::u8_cast<::std::u8string>(m_text)
		+ u8"' [err = " + ::uns::string::u8_cast<::std::u8string>(m_code)
		+ u8", type = " + ::uns::string::u8_cast<::std::u8string>(m_type) + u8"]";
};
//<= class ::uns::lua::error


namespace uns::lua::auxiliary {

	::uns::lua::errcode lua_native_error_to_wrapper(int lua_native_error_code) noexcept {
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
	protected:
		::uns::lua::alias::lua_state m_state = nullptr;
		bool m_copied = false;
	public:
		state() noexcept :
			m_state(luaL_newstate()),
			m_copied(false) {
			luaL_openlibs(reinterpret_cast<lua_State*>(m_state));
		};
		state(const ::uns::lua::auxiliary::state& obj) noexcept :
			m_state(lua_newthread(reinterpret_cast<lua_State*>(obj.m_state))),
			m_copied(true) {};
		::uns::lua::auxiliary::state& operator=(const ::uns::lua::auxiliary::state& obj) noexcept {
			if(this != &obj) {
				reset();
				m_state = lua_newthread(reinterpret_cast<lua_State*>(obj.m_state));
				m_copied = true;
			};
			return *this;
		};
		state(::uns::lua::auxiliary::state&& obj) noexcept :
			m_state(obj.m_state),
			m_copied(obj.m_copied) {//recently here was 'm_copied(false)'
			obj.m_state = nullptr;
			obj.m_copied = false;
		};
		::uns::lua::auxiliary::state& operator=(::uns::lua::auxiliary::state&& obj) noexcept {
			if(this != &obj) {
				std::swap(m_state, obj.m_state);
				std::swap(m_copied, obj.m_copied);
			};
			return *this;
		};
		~state() {
			reset();
		};

		const ::uns::lua::alias::lua_state get() const noexcept { return m_state; };
		::uns::lua::alias::lua_state get() noexcept { return m_state; };
	protected:
		void reset() noexcept {
			if(m_state != nullptr) {
				if(!m_copied) {
					lua_close(reinterpret_cast<lua_State*>(m_state));
				};
			};
		};
	};

};


//class ::uns::lua::value =>
::uns::lua::value::value(const ::std::string obj) noexcept :
	m_type(::uns::lua::value_type::string),
	m_string(obj),
	m_push_function(push_string)
{};
::uns::lua::value::value(const char* obj) noexcept : value(::std::string{ obj }) {};
::uns::lua::value::value(const ::std::string_view obj) noexcept :
	m_type(::uns::lua::value_type::string),
	m_string(obj),
	m_push_function(push_string)
{};
::uns::lua::value::value(const ::std::u8string obj) noexcept : value(::uns::string::u8_cast<::std::string>(obj)) {};
::uns::lua::value::value(const char8_t* obj) noexcept : value(::std::u8string{ obj }) {};
::uns::lua::value::value(const ::uns::lua::type::table& obj) noexcept :
	m_type(::uns::lua::value_type::table),
	m_table(obj),
	m_push_function(push_table)
{};
::uns::lua::value::value(const ::uns::lua::type::nil& obj) noexcept :
	m_type(::uns::lua::value_type::nil),
	m_push_function(push_nil)
{};
::uns::lua::value::value(const ::uns::lua::value& obj) noexcept :
	m_type(obj.m_type),
	m_boolean(obj.m_boolean),
	m_number(obj.m_number),
	m_integer(obj.m_integer),
	m_string(obj.m_string),
	m_table(obj.m_table),
	m_push_function(obj.m_push_function)
{};
::uns::lua::value& ::uns::lua::value::operator=(const ::uns::lua::value& obj) noexcept {
	if(this != &obj) {
		m_type = obj.m_type;
		m_boolean = obj.m_boolean;
		m_number = obj.m_number;
		m_integer = obj.m_integer;
		m_string = obj.m_string;
		m_table = obj.m_table;
		m_push_function = obj.m_push_function;
	};
	return *this;
};
::uns::lua::value::value(::uns::lua::value&& obj) noexcept :
	m_type(std::move(obj.m_type)),
	m_boolean(std::move(obj.m_boolean)),
	m_number(std::move(obj.m_number)),
	m_integer(std::move(obj.m_integer)),
	m_string(std::move(obj.m_string)),
	m_table(std::move(obj.m_table)),
	m_push_function(std::move(obj.m_push_function))
{};
::uns::lua::value& ::uns::lua::value::operator=(::uns::lua::value&& obj) noexcept {
	if(this != &obj) {
		m_type = std::move(obj.m_type);
		m_boolean = std::move(obj.m_boolean);
		m_number = std::move(obj.m_number);
		m_integer = std::move(obj.m_integer);
		m_string = std::move(obj.m_string);
		m_table = std::move(obj.m_table);
		m_push_function = std::move(obj.m_push_function);
	};
	return *this;
};
::uns::lua::value::~value() noexcept {};

bool ::uns::lua::value::operator==(const ::uns::lua::value& obj) const noexcept {
	return (m_type == obj.m_type)
		&& (
			(m_type == ::uns::lua::value_type{ ::uns::lua::value_type::nil })
			|| ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::boolean }) && (m_boolean == static_cast<::uns::lua::type::boolean>(obj)))
			|| ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::number }) && (m_number == static_cast<::uns::lua::type::number>(obj)))
			|| ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::integer }) && (m_integer == static_cast<::uns::lua::type::integer>(obj)))
			|| ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::string }) && (m_string == static_cast<::uns::lua::type::string>(obj)))
			|| ((m_type == ::uns::lua::value_type{ ::uns::lua::value_type::table }) && (m_table == static_cast<::uns::lua::type::table>(obj)))
		);
};
bool ::uns::lua::value::operator!=(const ::uns::lua::value& obj) const noexcept {
	return !(*this == obj);
};

#define UNS_LUA_VALUE_CONVERT_DESCRIPTOR(type_identifier)										\
		::uns::lua::value::operator const ::uns::lua::type::##type_identifier&() const noexcept {\
			return m_##type_identifier;															\
		};																						\
		::uns::lua::value::operator ::uns::lua::type::##type_identifier&() noexcept {			\
			return m_##type_identifier;															\
		};																						\

UNS_LUA_VALUE_CONVERT_DESCRIPTOR(boolean);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(number);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(integer);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(string);
UNS_LUA_VALUE_CONVERT_DESCRIPTOR(table);
#undef UNS_LUA_VALUE_CONVERT_DESCRIPTOR

void ::uns::lua::value::push_to(::uns::lua::auxiliary::state& thread) const noexcept {
	push_to(thread.get());
};
void ::uns::lua::value::push_to(::uns::lua::alias::lua_state stack) const noexcept {
	m_push_function(stack, *this);
};

void ::uns::lua::value::push_nil(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) noexcept {
	lua_pushnil(reinterpret_cast<lua_State*>(stack));
};
#define UNS_LUA_VALUE_PUSH_DESCRIPTOR(type_identifier)											\
		void ::uns::lua::value::push_##type_identifier(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) noexcept {\
			lua_push##type_identifier(reinterpret_cast<lua_State*>(stack), static_cast<::uns::lua::type::##type_identifier>(value));\
		};																						\

UNS_LUA_VALUE_PUSH_DESCRIPTOR(boolean);
UNS_LUA_VALUE_PUSH_DESCRIPTOR(number);
UNS_LUA_VALUE_PUSH_DESCRIPTOR(integer);

void ::uns::lua::value::push_string(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) noexcept {
	lua_pushstring(reinterpret_cast<lua_State*>(stack), static_cast<::uns::lua::type::string>(value).c_str());
};
void ::uns::lua::value::push_table(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) noexcept {
	lua_newtable(reinterpret_cast<lua_State*>(stack));

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::number>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::number>();
		++iterator
	) {
		lua_pushnumber(reinterpret_cast<lua_State*>(stack), iterator->first);
		iterator->second.push_to(stack);
		lua_settable(reinterpret_cast<lua_State*>(stack), -3);
	};

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::integer>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::integer>();
		++iterator
	) {
		lua_pushinteger(reinterpret_cast<lua_State*>(stack), iterator->first);
		iterator->second.push_to(stack);
		lua_settable(reinterpret_cast<lua_State*>(stack), -3);
	};

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::boolean>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::boolean>();
		++iterator
	) {
		lua_pushboolean(reinterpret_cast<lua_State*>(stack), iterator->first);
		iterator->second.push_to(stack);
		lua_settable(reinterpret_cast<lua_State*>(stack), -3);
	};

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::string>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::string>();
		++iterator
	) {
		lua_pushstring(reinterpret_cast<lua_State*>(stack), iterator->first.c_str());
		iterator->second.push_to(stack);
		lua_settable(reinterpret_cast<lua_State*>(stack), -3);
	};
};
#undef UNS_LUA_VALUE_PUSH_DESCRIPTOR


::std::u8string uns::lua::value::to_string() const noexcept {
	switch(type()) {
		default:
		case ::uns::lua::value_type::nil:
		{
			return u8"nil";
		}
		case ::uns::lua::value_type::number:
		{
			return ::uns::string::u8_cast<::std::u8string>(static_cast<::uns::lua::type::number>(*this));
		}
		case ::uns::lua::value_type::integer:
		{
			return ::uns::string::u8_cast<::std::u8string>(static_cast<::uns::lua::type::integer>(*this));
		}
		case ::uns::lua::value_type::boolean:
		{
			return ::uns::string::u8_cast<::std::u8string>(static_cast<::uns::lua::type::boolean>(*this));
		}
		case ::uns::lua::value_type::string:
		{
			return u8"\"" + ::uns::string::u8_cast<::std::u8string>(static_cast<::uns::lua::type::string>(*this)) + u8"\"";
		}
		case ::uns::lua::value_type::table:
		{
			return u8"table " + ::uns::string::u8_cast<::std::u8string>(reinterpret_cast<uint64_t>(this));
		}
	};
};
//<= class ::uns::lua::value


//class ::uns::lua::type::table =>
::uns::lua::type::table::table() noexcept : m_ptr(::std::unique_ptr<::uns::lua::auxiliary::table>{ new ::uns::lua::auxiliary::table{} }) {};
::uns::lua::type::table::table(const ::uns::lua::type::table& obj) noexcept : m_ptr(new ::uns::lua::auxiliary::table{ *obj.m_ptr }) {};
::uns::lua::type::table& ::uns::lua::type::table::operator=(const ::uns::lua::type::table& obj) noexcept {
	if(this != &obj) {
		*m_ptr = *obj.m_ptr;
	};
	return *this;
};
::uns::lua::type::table::table(::uns::lua::type::table&& obj) noexcept : m_ptr(std::move(obj.m_ptr)) {};
::uns::lua::type::table& ::uns::lua::type::table::operator=(::uns::lua::type::table&& obj) noexcept {
	if(this != &obj) {
		m_ptr = std::move(obj.m_ptr);
	};
	return *this;
};
::uns::lua::type::table::~table() noexcept {};

bool ::uns::lua::type::table::operator==(const ::uns::lua::type::table&) const noexcept { return false; };
bool ::uns::lua::type::table::operator!=(const ::uns::lua::type::table&) const noexcept { return true; };

::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) const noexcept { return m_ptr->operator[](key); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) noexcept { return m_ptr->operator[](key); };
::uns::lua::value uns::lua::type::table::operator[] (const char* key) const noexcept { return m_ptr->operator[](::uns::lua::type::string{ key }); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const char* key) noexcept { return m_ptr->operator[](::uns::lua::type::string{ key }); };
::uns::lua::value uns::lua::type::table::operator[] (const ::std::u8string& key) const noexcept { return m_ptr->operator[](::uns::string::u8_cast<::uns::lua::type::string>(key)); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::std::u8string& key) noexcept { return m_ptr->operator[](::uns::string::u8_cast<::uns::lua::type::string>(key)); };
::uns::lua::value uns::lua::type::table::operator[] (const char8_t* key) const noexcept { return this->operator[](::std::u8string{ key }); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const char8_t* key) noexcept { return this->operator[](::std::u8string{ key }); };
::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::value& key) const noexcept { return m_ptr->operator[](key); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::value& key) noexcept { return m_ptr->operator[](key); };

::std::size_t uns::lua::type::table::size() const noexcept { return m_ptr->size(); };
//<= class ::uns::lua::type::table


//class ::uns::lua::type::table =>
::uns::lua::auxiliary::table::table(const ::uns::lua::auxiliary::table& obj) noexcept :
	m_key_number(obj.m_key_number),
	m_key_integer(obj.m_key_integer),
	m_key_boolean(obj.m_key_boolean),
	m_key_string(obj.m_key_string)
{};
::uns::lua::auxiliary::table& ::uns::lua::auxiliary::table::operator=(const ::uns::lua::auxiliary::table& obj) noexcept {
	if(this != &obj) {
		m_key_number = obj.m_key_number;
		m_key_integer = obj.m_key_integer;
		m_key_boolean = obj.m_key_boolean;
		m_key_string = obj.m_key_string;
	};

	return *this;
};
::uns::lua::auxiliary::table::table(::uns::lua::auxiliary::table&& obj) noexcept :
	m_key_number(std::move(obj.m_key_number)),
	m_key_integer(std::move(obj.m_key_integer)),
	m_key_boolean(std::move(obj.m_key_boolean)),
	m_key_string(std::move(obj.m_key_string))
{};
::uns::lua::auxiliary::table& ::uns::lua::auxiliary::table::operator=(::uns::lua::auxiliary::table&& obj) noexcept {
	if(this != &obj) {
		m_key_number = std::move(obj.m_key_number);
		m_key_integer = std::move(obj.m_key_integer);
		m_key_boolean = std::move(obj.m_key_boolean);
		m_key_string = std::move(obj.m_key_string);
	};

	return *this;
};
::uns::lua::auxiliary::table::~table() noexcept {}

#define UNS_LUA_TABLE_IDX_DESCRIPTOR(type_identifier)											\
	::uns::lua::value uns::lua::auxiliary::table::operator[] (const ::uns::lua::type::##type_identifier& key) const noexcept {\
		if(auto value_iter = m_key_##type_identifier.find(key); value_iter != m_key_##type_identifier.end()) {\
			return value_iter->second;													\
		}																				\
		else {																			\
			return ::uns::lua::nil;														\
		};																				\
	};								\
	::uns::lua::value& ::uns::lua::auxiliary::table::operator[] (const ::uns::lua::type::##type_identifier& key) noexcept { return m_key_##type_identifier[key]; };\

UNS_LUA_TABLE_IDX_DESCRIPTOR(number);
UNS_LUA_TABLE_IDX_DESCRIPTOR(integer);
UNS_LUA_TABLE_IDX_DESCRIPTOR(boolean);
UNS_LUA_TABLE_IDX_DESCRIPTOR(string);
#undef UNS_LUA_TABLE_IDX_DESCRIPTOR

::uns::lua::value uns::lua::auxiliary::table::operator[] (const ::uns::lua::value& key) const noexcept {
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
::uns::lua::value& ::uns::lua::auxiliary::table::operator[] (const ::uns::lua::value& key) noexcept {
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

::std::size_t uns::lua::auxiliary::table::size() const noexcept {
	return m_key_number.size() + m_key_integer.size() + m_key_boolean.size() + m_key_string.size();
};
//<= class ::uns::lua::type::table


::uns::lua::value uns::lua::value::make_from(::uns::lua::alias::lua_state stack, int idx) noexcept {
	switch(lua_type(reinterpret_cast<lua_State*>(stack), idx)) {
		default:
		case LUA_TNIL:
		{
			return ::uns::lua::nil;
		}
		case LUA_TNUMBER:
		{
			if(lua_isinteger(reinterpret_cast<lua_State*>(stack), idx)) {
				return ::uns::lua::value{ lua_tointeger(reinterpret_cast<lua_State*>(stack), idx) };
			}
			else {
				return ::uns::lua::value{ lua_tonumber(reinterpret_cast<lua_State*>(stack), idx) };
			};
		}
		case LUA_TSTRING:
		{
			return ::uns::lua::value{ static_cast<::uns::lua::type::string>(lua_tostring(reinterpret_cast<lua_State*>(stack), idx)) };
		}
		case LUA_TBOOLEAN:
		{
			return ::uns::lua::value{ static_cast<::uns::lua::type::boolean>(lua_toboolean(reinterpret_cast<lua_State*>(stack), idx)) };
		}
		case LUA_TTABLE:
		{
			auto res = ::uns::lua::type::table{};

			lua_pushnil(reinterpret_cast<lua_State*>(stack));
			while(lua_next(reinterpret_cast<lua_State*>(stack), idx) != 0) {
				auto key_idx = lua_gettop(reinterpret_cast<lua_State*>(stack)) - 1;
				auto val_idx = lua_gettop(reinterpret_cast<lua_State*>(stack));

				switch(lua_type(reinterpret_cast<lua_State*>(stack), key_idx)) {
					default:
					case LUA_TNIL:
					{
						break;
					}
					case LUA_TNUMBER:
					{
						if(lua_isinteger(reinterpret_cast<lua_State*>(stack), key_idx)) {
							res[lua_tointeger(reinterpret_cast<lua_State*>(stack), key_idx)] = ::uns::lua::value::make_from(stack, val_idx);
						}
						else {
							res[lua_tonumber(reinterpret_cast<lua_State*>(stack), key_idx)] = ::uns::lua::value::make_from(stack, val_idx);
						};

						break;
					}
					case LUA_TSTRING:
					{
						res[static_cast<::uns::lua::type::string>(lua_tostring(reinterpret_cast<lua_State*>(stack), key_idx))] = ::uns::lua::value::make_from(stack, val_idx);
						break;
					}
					case LUA_TBOOLEAN:
					{
						res[static_cast<::uns::lua::type::boolean>(lua_toboolean(reinterpret_cast<lua_State*>(stack), key_idx))] = ::uns::lua::value::make_from(stack, val_idx);
						break;
					}
				};

				lua_pop(reinterpret_cast<lua_State*>(stack), 1);
			};

			return ::uns::lua::value{ res };
		}
	};
};
::uns::lua::value uns::lua::value::make_from(::uns::lua::auxiliary::state& thread, int idx) noexcept {
	return ::uns::lua::value::make_from(thread.get(), idx);
};




namespace uns::lua::auxiliary {

	::std::pair<::uns::lua::error, ::std::vector<::uns::lua::value>> execute(::uns::lua::alias::lua_state stack, const ::std::string& function_name, const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args) noexcept {
		using result_t = ::std::pair<::uns::lua::error, ::std::vector<::uns::lua::value>>;
		
		if(stack == nullptr) {
			return result_t{
				::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
				::std::vector<::uns::lua::value>{}
			};
		};

		lua_getglobal(reinterpret_cast<lua_State*>(stack), function_name.c_str());
		const auto function_idx = lua_gettop(reinterpret_cast<lua_State*>(stack));

		if(!lua_isfunction(reinterpret_cast<lua_State*>(stack), function_idx)) {
			if(!lua_isnil(reinterpret_cast<lua_State*>(stack), function_idx)) {
				return result_t{
					::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable },
					::std::vector<::uns::lua::value>{}
				};
			}
			else {
				return result_t{
					::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
					::std::vector<::uns::lua::value>{}
				};
			};
		};

		for(const auto& arg : args) {
			arg.push_to(stack);
		};

		if(int lua_retcode = lua_pcall(reinterpret_cast<lua_State*>(stack), static_cast<int>(args.size()), static_cast<int>(expected_results), 0); lua_retcode != LUA_OK) {//TODO args.size() must be < than int.max()
			std::string err_str = "";
			if(lua_isstring(reinterpret_cast<lua_State*>(stack), -1)) {
				err_str = lua_tostring(reinterpret_cast<lua_State*>(stack), -1);
			};

			if(function_idx - lua_gettop(reinterpret_cast<lua_State*>(stack)) < 0) {
				lua_pop(reinterpret_cast<lua_State*>(stack), function_idx - lua_gettop(reinterpret_cast<lua_State*>(stack)));
			};

			return result_t{
				::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str },
				::std::vector<::uns::lua::value>{}
			};
		};

		auto results = ::std::vector<::uns::lua::value>{};
		results.reserve(expected_results);
		for(auto idx = function_idx; idx <= lua_gettop(reinterpret_cast<lua_State*>(stack)) && idx <= function_idx + expected_results - 1; ++idx) {
			results.push_back(::uns::lua::value::make_from(stack, idx));
		};

		if(function_idx - lua_gettop(reinterpret_cast<lua_State*>(stack)) < 0) {
			lua_pop(reinterpret_cast<lua_State*>(stack), function_idx - lua_gettop(reinterpret_cast<lua_State*>(stack)));
		};

		return result_t{
			::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok },
			results
		};
	};

	::uns::lua::error load(::uns::lua::alias::lua_state stack, const ::uns::lua::library& library) noexcept {
		if(stack == nullptr) {
			return ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		};

		if(!library.text.empty()) {
			auto narrow_text = ::uns::string::u8_cast<::std::string>(library.text);

			if(int lua_retcode = luaL_loadstring(reinterpret_cast<lua_State*>(stack), narrow_text.c_str()); lua_retcode != LUA_OK) {
				std::string err_str = "";

				if(lua_isstring(reinterpret_cast<lua_State*>(stack), -1)) {
					err_str = lua_tostring(reinterpret_cast<lua_State*>(stack), -1);
					lua_pop(reinterpret_cast<lua_State*>(stack), -1);
				};

				return ::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
			};
		};

		if(!library.name_space.empty()) {
			lua_newtable(reinterpret_cast<lua_State*>(stack));

			for(const auto& entry : library.api) {
				if(entry.name() == nullptr || entry.func() == nullptr) {
					break;
				};

				lua_pushstring(reinterpret_cast<lua_State*>(stack), entry.name());
				lua_pushcfunction(reinterpret_cast<lua_State*>(stack), reinterpret_cast<lua_CFunction>(entry.func()));
				lua_settable(reinterpret_cast<lua_State*>(stack), -3);
			};

			lua_setglobal(reinterpret_cast<lua_State*>(stack), ::uns::string::u8_cast<::std::string>(library.name_space).c_str());
		}
		else {
			for(const auto& entry : library.api) {
				if(entry.name() == nullptr || entry.func() == nullptr) {
					break;
				};

				lua_pushcfunction(reinterpret_cast<lua_State*>(stack), reinterpret_cast<lua_CFunction>(entry.func()));
				lua_setglobal(reinterpret_cast<lua_State*>(stack), entry.name());

			};
		};

		return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
	};
	::uns::lua::error load(::uns::lua::alias::lua_state stack, const ::std::u8string& text) noexcept {
		if(stack == nullptr) {
			return ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		};

		auto narrow_text = ::uns::string::u8_cast<::std::string>(text);

		if(int lua_retcode = luaL_loadstring(reinterpret_cast<lua_State*>(stack), narrow_text.c_str()); lua_retcode != LUA_OK) {
			std::string err_str = "";

			if(lua_isstring(reinterpret_cast<lua_State*>(stack), -1)) {
				err_str = lua_tostring(reinterpret_cast<lua_State*>(stack), -1);
				lua_pop(reinterpret_cast<lua_State*>(stack), -1);
			};

			return ::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		};

		return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
	};
	::uns::lua::error load(::uns::lua::alias::lua_state stack, const ::std::filesystem::path& file) noexcept {
		auto narrow_path = ::uns::string::u8_cast<::std::string>(::uns::string::u8_cast<::std::u8string>(file.lexically_normal().native()));

		if(int lua_retcode = luaL_loadfile(reinterpret_cast<lua_State*>(stack), narrow_path.c_str()); lua_retcode != LUA_OK) {
			std::string err_str = "";

			if(lua_isstring(reinterpret_cast<lua_State*>(stack), -1)) {
				err_str = lua_tostring(reinterpret_cast<lua_State*>(stack), -1);
				lua_pop(reinterpret_cast<lua_State*>(stack), -1);
			};

			return ::uns::lua::error{ ::uns::lua::auxiliary::lua_native_error_to_wrapper(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		};

		return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
	};

	::std::pair<::uns::lua::error, ::uns::lua::value> global_get(::uns::lua::alias::lua_state stack, const ::std::string& global_name) noexcept {
		using result_t = ::std::pair<::uns::lua::error, ::uns::lua::value>;

		if(stack == nullptr) {
			return result_t{
				::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid },
				::uns::lua::nil
			};
		};

		lua_getglobal(reinterpret_cast<lua_State*>(stack), global_name.c_str());
		auto global_idx = lua_gettop(reinterpret_cast<lua_State*>(stack));

		if(lua_isnil(reinterpret_cast<lua_State*>(stack), global_idx)) {
			return result_t{
				::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found },
				::uns::lua::nil
			};
		};

		if(
			!(
				lua_isnil(reinterpret_cast<lua_State*>(stack), global_idx)
				|| lua_isnumber(reinterpret_cast<lua_State*>(stack), global_idx)
				|| lua_isinteger(reinterpret_cast<lua_State*>(stack), global_idx)
				|| lua_isboolean(reinterpret_cast<lua_State*>(stack), global_idx)
				|| lua_isstring(reinterpret_cast<lua_State*>(stack), global_idx)
				|| lua_istable(reinterpret_cast<lua_State*>(stack), global_idx)
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
	::uns::lua::error global_set(::uns::lua::alias::lua_state stack, const ::std::string& global_name, const ::uns::lua::value& value) noexcept {
		if(stack == nullptr) {
			return ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		};

		value.push_to(stack);
		lua_setglobal(reinterpret_cast<lua_State*>(stack), global_name.c_str());

		return ::uns::lua::error{ ::uns::lua::errcode::ok, ::uns::lua::errtype::ok };
	};

	void gc(::uns::lua::alias::lua_state stack) noexcept {
		if(stack != nullptr) {
			lua_gc(reinterpret_cast<lua_State*>(stack), LUA_GCCOLLECT);
		};
	};
};


//class ::uns::lua::function =>
::uns::lua::function::function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name) noexcept :
	m_function_name(lua_global_function_name),
	m_stack_wrapper(),
	m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state{} }) 
{
	if(lua_script != nullptr) {
		*m_stack = *lua_script;
	};
};
::uns::lua::function::function(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_function_name) noexcept :
	m_function_name(lua_global_function_name),
	m_stack_wrapper(lua_script),
	m_stack(nullptr) 
{};
::uns::lua::function::~function() noexcept {
	if(valid()) {
		gc();
	};
};
::std::u8string uns::lua::function::name() const noexcept { return ::uns::string::u8_cast<::std::u8string>(m_function_name); };

::std::vector<::uns::lua::value> uns::lua::function::operator() (const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args) noexcept {
	::uns::lua::alias::lua_state state = nullptr;
	if(m_stack != nullptr) {
		state = m_stack->get();
	}
	else if(m_stack_wrapper.valid()) {
		state = m_stack_wrapper.get();
	};
	
	auto [l_err, results] = ::uns::lua::auxiliary::execute(state, m_function_name, expected_results, args);
	
	m_err = l_err;

	return results;
};

void uns::lua::function::gc() noexcept {
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


//class ::uns::lua::global =>
::uns::lua::global::global(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_variable_name) noexcept :
	m_global_name(lua_global_variable_name),
	m_stack_wrapper(),
	m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state}) {
	if(lua_script != nullptr) {
		*m_stack = *lua_script;
	};
};
::uns::lua::global::global(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_variable_name) noexcept :
	m_global_name(lua_global_variable_name),
	m_stack_wrapper(lua_script),
	m_stack(nullptr)
{};
::uns::lua::global::~global() noexcept {
	if(valid()) {
		gc();
	};
};
::std::u8string uns::lua::global::name() const noexcept { return ::uns::string::u8_cast<::std::u8string>(m_global_name); };

::uns::lua::value uns::lua::global::get() noexcept {
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
void uns::lua::global::set(const ::uns::lua::value& value) noexcept {
	::uns::lua::alias::lua_state state = nullptr;
	if(m_stack != nullptr) {
		state = m_stack->get();
	}
	else if(m_stack_wrapper.valid()) {
		state = m_stack_wrapper.get();
	};

	m_err = ::uns::lua::auxiliary::global_set(state, m_global_name, value);
};

void uns::lua::global::gc() noexcept {
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
::uns::lua::thread::thread(::uns::lua::alias::lua_state stack) noexcept :
	m_stack(stack)
{};
::uns::lua::thread::thread(const ::uns::lua::thread& obj) noexcept :
	m_stack(obj.m_stack)
{};
::uns::lua::thread& ::uns::lua::thread::operator=(const ::uns::lua::thread& obj) noexcept {
	if(this == &obj) return *this;

	m_stack = obj.m_stack;

	return *this;
};
::uns::lua::thread::thread(::uns::lua::thread&& obj) noexcept :
	m_stack(::std::move(obj.m_stack))
{};
::uns::lua::thread& ::uns::lua::thread::operator=(::uns::lua::thread&& obj) noexcept {
	if(this == &obj) return *this;

	m_stack = ::std::move(obj.m_stack);

	return *this;
};

void ::uns::lua::thread::load(const ::uns::lua::library& library) noexcept {
	m_err = ::uns::lua::auxiliary::load(m_stack.get(), library);
};
void ::uns::lua::thread::load(const ::std::u8string& text) noexcept {
	m_err = ::uns::lua::auxiliary::load(m_stack.get(), text);
};

::uns::lua::function uns::lua::thread::get_function(const ::std::u8string& lua_global_function_name) noexcept {
	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::u8_cast<::std::string>(lua_global_function_name) };
		return ::uns::lua::function{};
	}
	else {
		return ::uns::lua::function{ m_stack, ::uns::string::u8_cast<::std::string>(lua_global_function_name) };
	};
};
::uns::lua::global uns::lua::thread::get_global(const ::std::u8string& lua_global_variable_name) noexcept {
	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::u8_cast<::std::string>(lua_global_variable_name) };
		return ::uns::lua::global{};
	}
	else {
		return ::uns::lua::global{ m_stack, ::uns::string::u8_cast<::std::string>(lua_global_variable_name) };
	};
};

::std::size_t uns::lua::thread::size() const noexcept {
	if(valid()) {
		return static_cast<::std::size_t>(lua_gettop(reinterpret_cast<lua_State*>(m_stack.get())));
	}
	else {
		return 0;
	};
};
::uns::lua::value uns::lua::thread::get_value(int input_index) noexcept {
	if(valid()) {
		return ::uns::lua::value::make_from(m_stack.get(), input_index);
	}
	else {
		return 0;
	};
};

void ::uns::lua::thread::gc() noexcept {
	if(valid()) {
		lua_gc(reinterpret_cast<lua_State*>(m_stack.get()), LUA_GCCOLLECT);
	};
};
//<= ::uns::lua::thread


::uns::lua::lib_entry::lib_entry(const ::std::u8string& name, ::uns::lua::alias::lua_cfunction lua_function) noexcept :
	m_name(::uns::string::u8_cast<::std::string>(name)),
	m_function(lua_function)
{};


//class ::uns::lua::script =>
::uns::lua::script::script() noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {};
::uns::lua::script::script(const ::std::u8string& text) noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {
	load(text);
};
::uns::lua::script::script(const ::std::filesystem::path& file) noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {
	load(file);
};
::uns::lua::script::script(const ::uns::lua::library& library) noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {
	load(library);
};
::uns::lua::script::script(const ::uns::lua::script& obj) noexcept :
	m_stack(obj.m_stack),
	m_err(obj.m_err)
{};
::uns::lua::script& ::uns::lua::script::operator=(const ::uns::lua::script& obj) noexcept {
	if(this != &obj) {
		m_stack = obj.m_stack;
		m_err = obj.m_err;
	};
	return *this;
};
::uns::lua::script::script(::uns::lua::script&& obj) noexcept :
	m_stack(::std::move(obj.m_stack)),
	m_err(::std::move(obj.m_err))
{};
::uns::lua::script& ::uns::lua::script::operator=(::uns::lua::script&& obj) noexcept {
	if(this != &obj) {
		m_stack = ::std::move(obj.m_stack);
		m_err = ::std::move(obj.m_err);
	};
	return *this;
};
::uns::lua::script::~script() noexcept {};

void ::uns::lua::script::load(const ::uns::lua::library& library) noexcept {
	m_err = ::uns::lua::auxiliary::load(m_stack->get(), library);
};
void ::uns::lua::script::load(const ::std::u8string& text) noexcept {
	m_err = ::uns::lua::auxiliary::load(m_stack->get(), text);
};
void ::uns::lua::script::load(const ::std::filesystem::path& file) noexcept {
	m_err = ::uns::lua::auxiliary::load(m_stack->get(), file);
};


::uns::lua::function uns::lua::script::get_function(const ::std::u8string& lua_global_function_name) noexcept {
	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::u8_cast<::std::string>(lua_global_function_name) };
		return ::uns::lua::function{};
	}
	else {
		return ::uns::lua::function{ m_stack, ::uns::string::u8_cast<::std::string>(lua_global_function_name) };
	};
};
::uns::lua::global uns::lua::script::get_global(const ::std::u8string& lua_global_variable_name) noexcept {
	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::u8_cast<::std::string>(lua_global_variable_name) };
		return ::uns::lua::global{};
	}
	else {
		return ::uns::lua::global{ m_stack, ::uns::string::u8_cast<::std::string>(lua_global_variable_name) };
	};
};

void uns::lua::script::run() noexcept {
	m_err = ::uns::lua::error{};

	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		return;
	};

	if(int lua_retcode = lua_pcall(reinterpret_cast<lua_State*>(m_stack->get()), 0, 0, 0); lua_retcode != LUA_OK) {
		std::string err_str = "";
		if(lua_isstring(reinterpret_cast<lua_State*>(m_stack->get()), -1)) {
			err_str = lua_tostring(reinterpret_cast<lua_State*>(m_stack->get()), -1);
		};

		lua_pop(reinterpret_cast<lua_State*>(m_stack->get()), -1);

		m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		return;
	};

	return;
};

void uns::lua::script::gc() noexcept {
	if(valid()) {
		lua_gc(reinterpret_cast<lua_State*>(m_stack->get()), LUA_GCCOLLECT);
	};
};

long double uns::lua::script::total_memory() const noexcept {
	if(valid()) {
		long double res = lua_gc(reinterpret_cast<lua_State*>(m_stack->get()), LUA_GCCOUNT);
		res += static_cast<long double>(lua_gc(reinterpret_cast<lua_State*>(m_stack->get()), LUA_GCCOUNTB)) / 1024.0;
		return res;
	}
	else {
		return 0;
	};
};
//<= class ::uns::lua::script

::uns::lua::value uns::lua::make_table() noexcept { return ::uns::lua::type::table{}; };