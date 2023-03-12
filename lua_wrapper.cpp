
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "uniself/lua_wrapper.hpp"


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

bool ::uns::lua::error::is() const noexcept { return (m_code != ::uns::lua::errcode::ok) || (m_type != ::uns::lua::errtype::ok); };

::uns::lua::errcode uns::lua::error::code() const noexcept { return m_code; };
::uns::lua::errtype uns::lua::error::type() const noexcept { return m_type; };
::std::string uns::lua::error::text() const noexcept { return m_text; };

::std::u8string uns::lua::error::to_string() const {
	static const auto bracket_op = ::std::u8string{ u8"[" };
	static const auto bracket_cl = ::std::u8string{ u8"] " };
	static const auto comma = ::std::u8string{ u8", " };
	static const auto err_w = ::std::u8string{ u8"err = " };
	static const auto type_w = ::std::u8string{ u8"type = " };

	return bracket_op + err_w + ::uns::string::u8_cast<::std::u8string>(m_code) + comma
		+ type_w + ::uns::string::u8_cast<::std::u8string>(m_type) + bracket_cl
		+ ::uns::string::u8_cast<::std::u8string>(m_text);
};
//<= class ::uns::lua::error


namespace uns::lua::auxiliary {
	class table;

	class state {
	protected:
		lua_State* m_state = nullptr;
		bool m_copied = false;
	public:
		state() noexcept :
			m_state(luaL_newstate()),
			m_copied(false) {
			luaL_openlibs(m_state);
		};
		state(const ::uns::lua::auxiliary::state& obj) noexcept :
			m_state(lua_newthread(obj.m_state)),
			m_copied(true) {};
		::uns::lua::auxiliary::state& operator=(const ::uns::lua::auxiliary::state& obj) noexcept {
			if(this != &obj) {
				reset();
				m_state = lua_newthread(obj.m_state);
				m_copied = true;
			};
			return *this;
		};
		state(::uns::lua::auxiliary::state&& obj) noexcept :
			m_state(obj.m_state),
			m_copied(true) {
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

		const lua_State* get() const noexcept { return m_state; };
		lua_State* get() noexcept { return m_state; };
	protected:
		void reset() noexcept {
			if(m_state != nullptr) {
				if(!m_copied) {
					lua_close(m_state);
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
			(m_type == ::uns::lua::value_type::nil)
			|| ((m_type == ::uns::lua::value_type::boolean) && (m_boolean == static_cast<::uns::lua::type::boolean>(obj)))
			|| ((m_type == ::uns::lua::value_type::number) && (m_number == static_cast<::uns::lua::type::number>(obj)))
			|| ((m_type == ::uns::lua::value_type::integer) && (m_integer == static_cast<::uns::lua::type::integer>(obj)))
			|| ((m_type == ::uns::lua::value_type::string) && (m_string == static_cast<::uns::lua::type::string>(obj)))
			|| ((m_type == ::uns::lua::value_type::table) && (m_table == static_cast<::uns::lua::type::table>(obj)))
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

void ::uns::lua::value::push_to(::uns::lua::auxiliary::state& lua_state) const noexcept {
	m_push_function(lua_state, *this);
};

void ::uns::lua::value::push_nil(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {
	lua_pushnil(lua_state.get());
};
#define UNS_LUA_VALUE_PUSH_DESCRIPTOR(type_identifier)											\
		void ::uns::lua::value::push_##type_identifier(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {\
			lua_push##type_identifier(lua_state.get(), static_cast<::uns::lua::type::##type_identifier>(value));\
		};																						\

UNS_LUA_VALUE_PUSH_DESCRIPTOR(boolean);
UNS_LUA_VALUE_PUSH_DESCRIPTOR(number);
UNS_LUA_VALUE_PUSH_DESCRIPTOR(integer);

void ::uns::lua::value::push_string(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {
	lua_pushstring(lua_state.get(), static_cast<::uns::lua::type::string>(value).c_str());
};
void ::uns::lua::value::push_table(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {
	lua_newtable(lua_state.get());

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::number>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::number>();
		++iterator
		) {
		lua_pushnumber(lua_state.get(), iterator->first);
		iterator->second.push_to(lua_state);
		lua_settable(lua_state.get(), -3);
	};

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::integer>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::integer>();
		++iterator
		) {
		lua_pushinteger(lua_state.get(), iterator->first);
		iterator->second.push_to(lua_state);
		lua_settable(lua_state.get(), -3);
	};

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::boolean>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::boolean>();
		++iterator
		) {
		lua_pushboolean(lua_state.get(), iterator->first);
		iterator->second.push_to(lua_state);
		lua_settable(lua_state.get(), -3);
	};

	for(
		auto iterator = static_cast<const ::uns::lua::type::table&>(value).cbegin<::uns::lua::type::string>();
		iterator != static_cast<const ::uns::lua::type::table&>(value).cend<::uns::lua::type::string>();
		++iterator
		) {
		lua_pushstring(lua_state.get(), iterator->first.c_str());
		iterator->second.push_to(lua_state);
		lua_settable(lua_state.get(), -3);
	};
};
#undef UNS_LUA_VALUE_PUSH_DESCRIPTOR
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


::uns::lua::value uns::lua::value::make_from(::uns::lua::auxiliary::state& lua_state, int idx) noexcept {
	switch(lua_type(lua_state.get(), idx)) {
		default:
		case LUA_TNIL:
		{
			return ::uns::lua::nil;
		}
		case LUA_TNUMBER:
		{
			if(lua_isinteger(lua_state.get(), idx)) {
				return ::uns::lua::value{ lua_tointeger(lua_state.get(), idx) };
			}
			else {
				return ::uns::lua::value{ lua_tonumber(lua_state.get(), idx) };
			};
		}
		case LUA_TSTRING:
		{
			return ::uns::lua::value{ static_cast<::uns::lua::type::string>(lua_tostring(lua_state.get(), idx)) };
		}
		case LUA_TBOOLEAN:
		{
			return ::uns::lua::value{ static_cast<::uns::lua::type::boolean>(lua_toboolean(lua_state.get(), idx)) };
		}
		case LUA_TTABLE:
		{
			auto res = ::uns::lua::type::table{};

			lua_pushnil(lua_state.get());
			while(lua_next(lua_state.get(), idx) != 0) {
				auto key_idx = lua_gettop(lua_state.get()) - 1;
				auto val_idx = lua_gettop(lua_state.get());

				switch(lua_type(lua_state.get(), key_idx)) {
					default:
					case LUA_TNIL:
					{
						break;
					}
					case LUA_TNUMBER:
					{
						if(lua_isinteger(lua_state.get(), key_idx)) {
							res[lua_tointeger(lua_state.get(), key_idx)] = ::uns::lua::value::make_from(lua_state, val_idx);
						}
						else {
							res[lua_tonumber(lua_state.get(), key_idx)] = ::uns::lua::value::make_from(lua_state, val_idx);
						};

						break;
					}
					case LUA_TSTRING:
					{
						res[static_cast<::uns::lua::type::string>(lua_tostring(lua_state.get(), key_idx))] = ::uns::lua::value::make_from(lua_state, val_idx);
						break;
					}
					case LUA_TBOOLEAN:
					{
						res[static_cast<::uns::lua::type::boolean>(lua_toboolean(lua_state.get(), key_idx))] = ::uns::lua::value::make_from(lua_state, val_idx);
						break;
					}
				};

				lua_pop(lua_state.get(), 1);
			};

			return ::uns::lua::value{ res };
		}
	};
};


//class ::uns::lua::function =>
::uns::lua::function::function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name) noexcept :
	m_function_name(lua_global_function_name),
	m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state{} }) {
	if(lua_script != nullptr) {
		*m_stack = *lua_script;
	};
};
::uns::lua::function::~function() noexcept {
	if(valid()) {
		lua_gc(m_stack->get(), LUA_GCCOLLECT);
	};
};

::std::u8string uns::lua::function::name() const noexcept { return ::uns::string::u8_cast<::std::u8string>(m_function_name); };

::std::vector<::uns::lua::value> uns::lua::function::operator() (const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args) noexcept {
	m_err = ::uns::lua::error{};

	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		return ::std::vector<::uns::lua::value>{};
	};

	lua_getglobal(m_stack->get(), m_function_name.c_str());
	const auto function_idx = lua_gettop(m_stack->get());

	if(!lua_isfunction(m_stack->get(), function_idx)) {
		if(!lua_isnil(m_stack->get(), function_idx)) {
			m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::uncallable };
			return ::std::vector<::uns::lua::value>{};
		}
		else {
			m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found };
			return ::std::vector<::uns::lua::value>{};
		};
	};

	for(const auto& arg : args) {
		arg.push_to(*m_stack);
	};

	if(int lua_retcode = lua_pcall(m_stack->get(), static_cast<int>(args.size()), expected_results, 0); lua_retcode != LUA_OK) {//TODO args.size() must be < than int.max()
		std::string err_str = "";
		if(lua_isstring(m_stack->get(), -1)) {
			err_str = lua_tostring(m_stack->get(), -1);
		};

		if(function_idx - lua_gettop(m_stack->get()) < 0) {
			lua_pop(m_stack->get(), function_idx - lua_gettop(m_stack->get()));
		};

		m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		return ::std::vector<::uns::lua::value>{};
	};

	auto results = ::std::vector<::uns::lua::value>{};
	results.reserve(expected_results);
	for(auto idx = function_idx; idx <= lua_gettop(m_stack->get()) && idx <= function_idx + expected_results - 1; ++idx) {
		results.push_back(::uns::lua::value::make_from(*m_stack, idx));
	};

	if(function_idx - lua_gettop(m_stack->get()) < 0) {
		lua_pop(m_stack->get(), function_idx - lua_gettop(m_stack->get()));
	};

	return results;
};

void uns::lua::function::gc() noexcept {
	lua_gc(m_stack->get(), LUA_GCCOLLECT);
};
//<= class ::uns::lua::function


//class ::uns::lua::global =>
::uns::lua::global::global(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_variable_name) noexcept :
	m_global_name(lua_global_variable_name),
	m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state}) {
	if(lua_script != nullptr) {
		*m_stack = *lua_script;
	};
};
::uns::lua::global::~global() noexcept {
	if(valid()) {
		lua_gc(m_stack->get(), LUA_GCCOLLECT);
	};
};

::std::u8string uns::lua::global::name() const noexcept { return ::uns::string::u8_cast<::std::u8string>(m_global_name); };

::uns::lua::value uns::lua::global::get() noexcept {
	m_err = ::uns::lua::error{};

	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		return ::uns::lua::value{};
	};

	lua_getglobal(m_stack->get(), m_global_name.c_str());
	auto global_idx = lua_gettop(m_stack->get());

	if(lua_isnil(m_stack->get(), global_idx)) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::not_found };
		return ::uns::lua::value{};
	};

	if(
		!(
			lua_isnil(m_stack->get(), global_idx)
			|| lua_isnumber(m_stack->get(), global_idx)
			|| lua_isinteger(m_stack->get(), global_idx)
			|| lua_isboolean(m_stack->get(), global_idx)
			|| lua_isstring(m_stack->get(), global_idx)
			|| lua_istable(m_stack->get(), global_idx)
			)
		) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::unrepresentable };
		return ::uns::lua::value{};
	};

	auto result = ::uns::lua::value::make_from(*m_stack, global_idx);
	lua_pop(m_stack->get(), -1);

	return result;
};
void uns::lua::global::set(const ::uns::lua::value& value) noexcept {
	m_err = ::uns::lua::error{};

	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		return;
	};

	value.push_to(*m_stack);
	lua_setglobal(m_stack->get(), m_global_name.c_str());
};

void uns::lua::global::gc() noexcept {
	lua_gc(m_stack->get(), LUA_GCCOLLECT);
};
//<= class ::uns::lua::global


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
	m_err = ::uns::lua::error{};

	if(!valid()) {
		m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
		return;
	};

	if(!library.text.empty()) {
		auto narrow_text = ::uns::string::u8_cast<::std::string>(library.text);

		if(int lua_retcode = luaL_loadstring(m_stack->get(), narrow_text.c_str()); lua_retcode != LUA_OK) {
			std::string err_str = "";

			if(lua_isstring(m_stack->get(), -1)) {
				err_str = lua_tostring(m_stack->get(), -1);
				lua_pop(m_stack->get(), -1);
			};

			m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
			m_stack = nullptr;
			return;
		};
	};

	if(!library.name_space.empty()) {
		lua_newtable(m_stack->get());

		for(const auto& entry : library.api) {
			if(entry.name() == nullptr || entry.func() == nullptr) {
				break;
			};

			lua_pushstring(m_stack->get(), entry.name());
			lua_pushcfunction(m_stack->get(), reinterpret_cast<lua_CFunction>(entry.func()));
			lua_settable(m_stack->get(), -3);
		};

		lua_setglobal(m_stack->get(), ::uns::string::u8_cast<::std::string>(library.name_space).c_str());
	}
	else {
		for(const auto& entry : library.api) {
			if(entry.name() == nullptr || entry.func() == nullptr) {
				break;
			};

			lua_pushcfunction(m_stack->get(), reinterpret_cast<lua_CFunction>(entry.func()));
			lua_setglobal(m_stack->get(), entry.name());

		};
	};
};
void ::uns::lua::script::load(const ::std::u8string& text) noexcept {
	auto narrow_text = ::uns::string::u8_cast<::std::string>(text);

	if(int lua_retcode = luaL_loadstring(m_stack->get(), narrow_text.c_str()); lua_retcode != LUA_OK) {
		std::string err_str = "";

		if(lua_isstring(m_stack->get(), -1)) {
			err_str = lua_tostring(m_stack->get(), -1);
			lua_pop(m_stack->get(), -1);
		};

		m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		m_stack = nullptr;
	};
};
void ::uns::lua::script::load(const ::std::filesystem::path& file) noexcept {
	auto narrow_path = ::uns::string::u8_cast<::std::string>(::uns::string::u8_cast<::std::u8string>(file.lexically_normal().native()));

	if(int lua_retcode = luaL_loadfile(m_stack->get(), narrow_path.c_str()); lua_retcode != LUA_OK) {
		std::string err_str = "";

		if(lua_isstring(m_stack->get(), -1)) {
			err_str = lua_tostring(m_stack->get(), -1);
			lua_pop(m_stack->get(), -1);
		};

		m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		m_stack = nullptr;
	};
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

	if(int lua_retcode = lua_pcall(m_stack->get(), 0, 0, 0); lua_retcode != LUA_OK) {
		std::string err_str = "";
		if(lua_isstring(m_stack->get(), -1)) {
			err_str = lua_tostring(m_stack->get(), -1);
		};

		lua_pop(m_stack->get(), -1);

		m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, err_str };
		return;
	};

	return;
};

void uns::lua::script::gc() noexcept {
	lua_gc(m_stack->get(), LUA_GCCOLLECT);
};
//<= class ::uns::lua::script

::uns::lua::value uns::lua::make_table() noexcept { return ::uns::lua::type::table{}; };