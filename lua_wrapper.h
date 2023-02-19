#pragma once

#include <iostream>
#include <unordered_map>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "uniself/strings.h"
#include "uniself/benum.h"


namespace uns::lua {


	UNS_BENUM_DECLARATOR(errcode, int,
		ok = LUA_OK,
		yeld = LUA_YIELD,
		errrun = LUA_ERRRUN,
		errsyntax = LUA_ERRSYNTAX,
		errmem = LUA_ERRMEM,
		errerr = LUA_ERRERR,
		errcall
	);


	UNS_BENUM_DECLARATOR(errtype, int,
		ok = 0x0,
		lua_specific,
		invalid,
		uncallable,
		unrepresentable,
		not_found
	);


	class error {
	protected:
		::uns::lua::errcode m_code = ::uns::lua::errcode::ok;
		::uns::lua::errtype m_type = ::uns::lua::errtype::ok;
		::std::u8string m_text = u8"";
	public:
		error(
			const ::uns::lua::errcode& code = ::uns::lua::errcode::ok,
			const ::uns::lua::errtype& type = ::uns::lua::errtype::ok,
			const ::std::u8string text = u8""
		) noexcept :
			m_text(text),
			m_code(code),
			m_type(type)
		{};
		error(const ::uns::lua::error& obj) noexcept :
			m_text(obj.m_text),
			m_code(obj.m_code),
			m_type(obj.m_type)
		{};
		::uns::lua::error& operator=(const ::uns::lua::error& obj) noexcept {
			if(this != &obj) {
				m_text = obj.m_text;
				m_code = obj.m_code;
				m_type = obj.m_type;
			};
			return *this;
		};
		error(::uns::lua::error&& obj) noexcept :
			m_text(std::move(obj.m_text)),
			m_code(std::move(obj.m_code)),
			m_type(std::move(obj.m_type))
		{};
		::uns::lua::error& operator=(::uns::lua::error&& obj) noexcept {
			if(this != &obj) {
				m_text = obj.m_text;
				m_code = obj.m_code;
				m_type = obj.m_type;
			};
			return *this;
		};
		~error() noexcept {};

		void swap(::uns::lua::error& obj) noexcept {
			::std::swap(m_text, obj.m_text);
			::std::swap(m_code, obj.m_code);
			::std::swap(m_type, obj.m_type);
		};

		bool is() const noexcept { return (m_code != ::uns::lua::errcode::ok) || (m_type != ::uns::lua::errtype::ok); };

		::uns::lua::errcode code() const noexcept { return m_code; };
		::uns::lua::errtype type() const noexcept { return m_type; };
		::std::u8string text() const noexcept { return m_text; };

		::std::u8string to_string() const {
			static const auto bracket_op = ::std::u8string{ u8"["};
			static const auto bracket_cl = ::std::u8string{ u8"] " };
			static const auto comma = ::std::u8string{ u8", " };
			static const auto err_w = ::std::u8string{ u8"err = " };
			static const auto type_w = ::std::u8string{ u8"type = " };

			return bracket_op + err_w + ::uns::string::u8_cast<::std::u8string>(m_code) + comma
				+ type_w + ::uns::string::u8_cast<::std::u8string>(m_type) + bracket_cl
				+ m_text;
		};
	};


	UNS_BENUM_DECLARATOR(value_type, int,
		nil = -1,
		boolean = 0,
		number = 1,
		integer = 2,
		string = 3,
		table = 4
	);


	class value;


	namespace auxiliary {
		class table;
	};


	namespace type {
		class nil {};

		using boolean = bool;
		using number = lua_Number;
		using integer = lua_Integer;
		using string = ::std::string;

		class table {
		protected:
			::std::unique_ptr<::uns::lua::auxiliary::table> m_ptr = nullptr;
		public:
			table() noexcept {};
			table(const ::uns::lua::type::table& obj) noexcept;
			::uns::lua::type::table& operator=(const ::uns::lua::type::table&) noexcept;
			table(::uns::lua::type::table&&) noexcept;
			::uns::lua::type::table& operator=(::uns::lua::type::table&&) noexcept;
			~table() noexcept {};

			bool operator==(const ::uns::lua::type::table&) const noexcept { return false; };
			bool operator!=(const ::uns::lua::type::table&) const noexcept { return true; };

			::uns::lua::value operator[] (const ::uns::lua::type::number&) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::type::number&) noexcept;
			::uns::lua::value operator[] (const ::uns::lua::type::integer&) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::type::integer&) noexcept;
			::uns::lua::value operator[] (const ::uns::lua::type::boolean&) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::type::boolean&) noexcept;
			::uns::lua::value operator[] (const ::uns::lua::type::string&) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::type::string&) noexcept;
			::uns::lua::value operator[] (const ::uns::lua::value&) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::value&) noexcept;

			::std::size_t size() const noexcept;
		};
	};


	static constexpr auto nil = ::uns::lua::type::nil{};	


	class value {
	protected:
		::uns::lua::value_type m_type = ::uns::lua::value_type::nil;
		::uns::lua::type::boolean m_boolean = false;
		::uns::lua::type::number m_number = static_cast<::uns::lua::type::number>(0);
		::uns::lua::type::integer m_integer = static_cast<::uns::lua::type::integer>(0);
		::uns::lua::type::string m_string = ::uns::lua::type::string{};
		::uns::lua::type::table m_table = ::uns::lua::type::table{};
	public:
		value() noexcept {};
#define UNS_LUA_VALUE_INIT_DECLARATOR(type_identifier)											\
		value(const ::uns::lua::type::##type_identifier& obj) noexcept :						\
			m_type(::uns::lua::value_type::##type_identifier),									\
			m_##type_identifier(obj)															\
		{};																						\

		UNS_LUA_VALUE_INIT_DECLARATOR(boolean);
		UNS_LUA_VALUE_INIT_DECLARATOR(number);
		UNS_LUA_VALUE_INIT_DECLARATOR(integer);
		UNS_LUA_VALUE_INIT_DECLARATOR(string);
		UNS_LUA_VALUE_INIT_DECLARATOR(table);
#undef UNS_LUA_VALUE_INIT_DECLARATOR

		value(const ::uns::lua::type::nil& obj) noexcept :
			m_type(::uns::lua::value_type::nil)
		{};

		value(const ::uns::lua::value& obj) noexcept :
			m_type(obj.m_type),
			m_boolean(obj.m_boolean),
			m_number(obj.m_number),
			m_integer(obj.m_integer),
			m_string(obj.m_string),
			m_table(obj.m_table)
		{};
		::uns::lua::value& operator=(const ::uns::lua::value& obj) noexcept {
			if(this == &obj) {
				m_type = obj.m_type;
				m_boolean = obj.m_boolean;
				m_number = obj.m_number;
				m_integer = obj.m_integer;
				m_string = obj.m_string;
				m_table = obj.m_table;
			};
			return *this;
		};
		value(::uns::lua::value&& obj) noexcept :
			m_type(std::move(obj.m_type)),
			m_boolean(std::move(obj.m_boolean)),
			m_number(std::move(obj.m_number)),
			m_integer(std::move(obj.m_integer)),
			m_string(std::move(obj.m_string)),
			m_table(std::move(obj.m_table))
		{};
		::uns::lua::value& operator=(::uns::lua::value&& obj) noexcept {
			if(this == &obj) {
				m_type = std::move(obj.m_type);
				m_boolean = std::move(obj.m_boolean);
				m_number = std::move(obj.m_number);
				m_integer = std::move(obj.m_integer);
				m_string = std::move(obj.m_string);
				m_table = std::move(obj.m_table);
			};
			return *this;
		};
		~value() noexcept { destruct(); };

		bool operator==(const ::uns::lua::value& obj) const noexcept {
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
		bool operator!=(const ::uns::lua::value& obj) const noexcept {
			return !(*this == obj);
		};

#define UNS_LUA_VALUE_CONVERT_DECLARATOR(type_identifier)										\
		operator ::uns::lua::type::##type_identifier() const noexcept {							\
			if(m_type == ::uns::lua::value_type::##type_identifier) {							\
				return m_##type_identifier;														\
			}																					\
			else {																				\
				return ::uns::lua::type::##type_identifier{};									\
			};																					\
		};																						\

		UNS_LUA_VALUE_CONVERT_DECLARATOR(boolean);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(number);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(integer);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(string);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(table);
#undef UNS_LUA_VALUE_CONVERT_DECLARATOR

#define UNS_LUA_VALUE_COMPARISON_DECLARATOR(type_identifier)									\
		bool operator==(const ::uns::lua::type::##type_identifier& obj) const noexcept {		\
			return (m_type == ::uns::lua::value_type::##type_identifier)						\
				&& (m_##type_identifier == obj);												\
		};																						\
		bool operator!=(const ::uns::lua::type::##type_identifier& obj) const noexcept {		\
			return !(*this == obj);																\
		};																						\

		UNS_LUA_VALUE_COMPARISON_DECLARATOR(boolean);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(number);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(integer);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(string);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(table);
#undef UNS_LUA_VALUE_COMPARISON_DECLARATOR

		::uns::lua::value_type type() const noexcept { return m_type; };

	protected:
		void destruct() noexcept {
			if(m_type == ::uns::lua::value_type::string) {
				m_string = ::uns::lua::type::string{};
			};
			if(m_type == ::uns::lua::value_type::table) {
				m_table = ::uns::lua::type::table{};
			};
		};
	};


	namespace auxiliary {
		class table {
		protected:
			::uns::lua::value nil_replacer = ::uns::lua::nil;
			::std::unordered_map<::uns::lua::type::number, ::uns::lua::value> m_key_number;
			::std::unordered_map<::uns::lua::type::integer, ::uns::lua::value> m_key_integer;
			::std::unordered_map<::uns::lua::type::boolean, ::uns::lua::value> m_key_boolean;
			::std::unordered_map<::uns::lua::type::string, ::uns::lua::value> m_key_string;
		public:
			table() noexcept {};
			table(const ::uns::lua::auxiliary::table& obj) noexcept :
				m_key_number(obj.m_key_number),
				m_key_integer(obj.m_key_integer),
				m_key_boolean(obj.m_key_boolean),
				m_key_string(obj.m_key_string)
			{};
			::uns::lua::auxiliary::table& operator=(const ::uns::lua::auxiliary::table& obj) noexcept {
				nil_replacer = ::uns::lua::nil;

				if(this != &obj) {
					m_key_number = obj.m_key_number;
					m_key_integer = obj.m_key_integer;
					m_key_boolean = obj.m_key_boolean;
					m_key_string = obj.m_key_string;
				};

				return *this;
			};
			table(::uns::lua::auxiliary::table&& obj) noexcept :
				m_key_number(std::move(obj.m_key_number)),
				m_key_integer(std::move(obj.m_key_integer)),
				m_key_boolean(std::move(obj.m_key_boolean)),
				m_key_string(std::move(obj.m_key_string))
			{};
			::uns::lua::auxiliary::table& operator=(::uns::lua::auxiliary::table&& obj) noexcept {
				nil_replacer = ::uns::lua::nil;
				obj.nil_replacer = ::uns::lua::nil;

				if(this != &obj) {
					m_key_number = std::move(obj.m_key_number);
					m_key_integer = std::move(obj.m_key_integer);
					m_key_boolean = std::move(obj.m_key_boolean);
					m_key_string = std::move(obj.m_key_string);
				};

				return *this;
			};
			~table() noexcept;

#define UNS_LUA_TABLE_IDX_DECLARATOR(type_identifier)											\
			::uns::lua::value operator[] (const ::uns::lua::type::##type_identifier& key) const noexcept {\
				if(auto value_iter = m_key_##type_identifier.find(key); value_iter != m_key_##type_identifier.end()) {\
					return value_iter->second;													\
				}																				\
				else {																			\
					return ::uns::lua::nil;														\
				};																				\
			};																					\
			::uns::lua::value& operator[] (const ::uns::lua::type::number& key) noexcept { return m_key_number[key]; };\

			UNS_LUA_TABLE_IDX_DECLARATOR(number);
			UNS_LUA_TABLE_IDX_DECLARATOR(integer);
			UNS_LUA_TABLE_IDX_DECLARATOR(boolean);
			UNS_LUA_TABLE_IDX_DECLARATOR(string);
#undef UNS_LUA_TABLE_IDX_DECLARATOR
			::uns::lua::value operator[] (const ::uns::lua::value& key) const noexcept {
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
			::uns::lua::value& operator[] (const ::uns::lua::value& key) noexcept {
				nil_replacer = ::uns::lua::nil;
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

			::std::size_t size() const noexcept {
				return m_key_number.size() + m_key_integer.size() + m_key_boolean.size() + m_key_string.size();
			};
		};
	};


	//definitions of ::uns::lua::type::table methods =>
	::uns::lua::type::table::table(const ::uns::lua::type::table& obj) noexcept : m_ptr(new ::uns::lua::auxiliary::table{ *obj.m_ptr}) {};
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

	::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::number& key) const noexcept { return m_ptr->operator[](key); };
	::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::number& key) noexcept { return m_ptr->operator[](key); };
	::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::integer& key) const noexcept { return m_ptr->operator[](key); };
	::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::integer& key) noexcept { return m_ptr->operator[](key); };
	::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::boolean& key) const noexcept { return m_ptr->operator[](key); };
	::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::boolean& key) noexcept { return m_ptr->operator[](key); };
	::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) const noexcept { return m_ptr->operator[](key); };
	::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) noexcept { return m_ptr->operator[](key); };
	::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::value& key) const noexcept { return m_ptr->operator[](key); };
	::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::value& key) noexcept { return m_ptr->operator[](key); };

	::std::size_t uns::lua::type::table::size() const noexcept { return m_ptr->size(); };
	//<= definitions of ::uns::lua::type::table methods 


	namespace auxiliary {

		class state {
		protected:
			lua_State* m_state = nullptr;
			bool m_copied = false;
		public:
			state() noexcept :
				m_state(luaL_newstate()),
				m_copied(false)
			{};
			state(const ::uns::lua::auxiliary::state& obj) noexcept :
				m_state(lua_newthread(obj.m_state)),
				m_copied(true)
			{};
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
				m_copied(true) 
			{ 
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


	class script;


	class function {
		friend ::uns::lua::script;

		::std::shared_ptr<::uns::lua::auxiliary::state> m_script = nullptr;
		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::std::string m_function_name = "";
		::uns::lua::error m_err;
	public:
		function() noexcept {};
	protected:
		function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name) noexcept :
			m_script(lua_script),
			m_function_name(lua_global_function_name),
			m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{})
		{
			if(lua_script != nullptr) {
				*m_stack = *lua_script;
			};
		};
	public:
		function(const function&) noexcept = default;
		function& operator=(const function&) noexcept = default;
		function(function&& obj) noexcept = default;
		function& operator=(function&& obj) noexcept = default;
		~function() noexcept {
			lua_gc(m_stack->get(), LUA_GCCOLLECT);
		};

		const ::uns::lua::error& error() const noexcept { return m_err; };
		::uns::lua::error& error() noexcept { return m_err; };

		::std::u8string name() const noexcept { return ::uns::string::u8_cast<::std::u8string>(m_function_name); };

		bool valid() const noexcept { return m_stack != nullptr; };

		::std::vector<::uns::lua::value> operator() (const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args) noexcept {
			m_err = ::uns::lua::error{};

			if(!valid()) {
				m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
				return ::std::vector<::uns::lua::value>{};
			};

			lua_getglobal(m_stack->get(), m_function_name.c_str());
			const ::std::size_t function_idx = lua_gettop(m_stack->get());

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
				this->push_value(arg);
			};

			if(int lua_retcode = lua_pcall(m_stack->get(), args.size(), expected_results, 0); lua_retcode != LUA_OK) {
				std::string err_str = "";
				if(lua_isstring(m_stack->get(), -1)) {
					err_str = lua_tostring(m_stack->get(), -1);
				};

				lua_pop(m_stack->get(), static_cast<int>(function_idx));
				lua_gc(m_stack->get(), LUA_GCCOLLECT);

				m_err = { static_cast<::uns::lua::errcode::_enumerated>(lua_retcode), ::uns::lua::errtype::lua_specific, ::uns::string::u8_cast<::std::u8string>(err_str) };
				return ::std::vector<::uns::lua::value>{};
			};

			auto results = ::std::vector<::uns::lua::value>{};
			results.reserve(expected_results);
			for(auto idx = function_idx; idx <= lua_gettop(m_stack->get()) && idx < function_idx + expected_results; ++idx) {
				results.push_back(this->to_value(idx));
			};

			lua_pop(m_stack->get(), static_cast<int>(function_idx));
			lua_gc(m_stack->get(), LUA_GCCOLLECT);

			return results;
		};
	protected:
		void push_value(const ::uns::lua::value& value) noexcept {
			switch(value.type()) {
				default:
				case ::uns::lua::value_type::nil:
				{
					lua_pushnil(m_stack->get());
					break;
				}
				case ::uns::lua::value_type::number:
				{
					lua_pushnumber(m_stack->get(), static_cast<::uns::lua::type::number>(value));
					break;
				}
				case ::uns::lua::value_type::integer:
				{
					lua_pushinteger(m_stack->get(), static_cast<::uns::lua::type::integer>(value));
					break;
				}
				case ::uns::lua::value_type::string:
				{
					lua_pushstring(m_stack->get(), static_cast<::uns::lua::type::string>(value).c_str());
					break;
				}
				case ::uns::lua::value_type::boolean:
				{
					lua_pushboolean(m_stack->get(), static_cast<::uns::lua::type::boolean>(value));
					break;
				}
				case ::uns::lua::value_type::table:
				{
					lua_newtable(m_stack->get());
					//TODO it requires some methods to run over the table type
					break;
				}
			};
		};

		::uns::lua::value to_value(::std::size_t idx) noexcept {
			switch(lua_type(m_stack->get(), static_cast<int>(idx))) {
				default:
				case LUA_TNIL:
				{
					return ::uns::lua::nil;
				}
				case LUA_TNUMBER:
				{
					if(lua_isinteger(m_stack->get(), static_cast<int>(idx))) {
						return ::uns::lua::value{ lua_tointeger(m_stack->get(), static_cast<int>(idx)) };
					}
					else {
						return ::uns::lua::value{ lua_tonumber(m_stack->get(), static_cast<int>(idx)) };
					};
				}
				case LUA_TSTRING:
				{
					return ::uns::lua::value{ static_cast<::uns::lua::type::string>(lua_tostring(m_stack->get(), static_cast<int>(idx))) };
				}
				case LUA_TBOOLEAN:
				{
					return ::uns::lua::value{ static_cast<::uns::lua::type::boolean>(lua_toboolean(m_stack->get(), static_cast<int>(idx))) };
				}
				case LUA_TTABLE:
				{
					//TODO it requires some methods to run over the table type
					return ::uns::lua::value{};
				}
			};
		};
	};
};