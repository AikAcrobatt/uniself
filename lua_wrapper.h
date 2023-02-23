#pragma once

#include <iostream>
#include <concepts>
#include <filesystem>
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
		::std::string m_text = "";
	public:
		error(
			const ::uns::lua::errcode& code = ::uns::lua::errcode::ok,
			const ::uns::lua::errtype& type = ::uns::lua::errtype::ok,
			const ::std::string text = ""
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
		::std::string text() const noexcept { return m_text; };

		::std::u8string to_string() const {
			static const auto bracket_op = ::std::u8string{ u8"["};
			static const auto bracket_cl = ::std::u8string{ u8"] " };
			static const auto comma = ::std::u8string{ u8", " };
			static const auto err_w = ::std::u8string{ u8"err = " };
			static const auto type_w = ::std::u8string{ u8"type = " };

			return bracket_op + err_w + ::uns::string::u8_cast<::std::u8string>(m_code) + comma
				+ type_w + ::uns::string::u8_cast<::std::u8string>(m_type) + bracket_cl
				+ ::uns::string::u8_cast<::std::u8string>(m_text);
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

		class state {
		protected:
			lua_State* m_state = nullptr;
			bool m_copied = false;
		public:
			state() noexcept :
				m_state(luaL_newstate()),
				m_copied(false)
			{
				luaL_openlibs(m_state);
			};
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


	namespace type {
		class nil {};

		using boolean = bool;
		using number = lua_Number;
		using integer = lua_Integer;
		using string = ::std::string;

		class table {
		public:
			template<class key_t>
			requires ::std::same_as<::uns::lua::type::number, key_t>
				|| ::std::same_as<::uns::lua::type::integer, key_t>
				|| ::std::same_as<::uns::lua::type::boolean, key_t>
				|| ::std::same_as<::uns::lua::type::string, key_t>
			using const_iterator = std::unordered_map<key_t, ::uns::lua::value>::const_iterator;

			template<class key_t>
			requires ::std::same_as<::uns::lua::type::number, key_t>
				|| ::std::same_as<::uns::lua::type::integer, key_t>
				|| ::std::same_as<::uns::lua::type::boolean, key_t>
				|| ::std::same_as<::uns::lua::type::string, key_t>
			using iterator = std::unordered_map<key_t, ::uns::lua::value>::iterator;
		protected:
			::std::unique_ptr<::uns::lua::auxiliary::table> m_ptr = nullptr;
		public:
			table() noexcept;
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

			template<class key_t>
			const_iterator<key_t> cbegin() const noexcept;
			template<class key_t>
			const_iterator<key_t> cend() const noexcept;

			template<class key_t>
			iterator<key_t> begin() noexcept;
			template<class key_t>
			iterator<key_t> end() noexcept;
		};
	};


	static constexpr auto nil = ::uns::lua::type::nil{};	


	class value {
	protected:
		using push_function_type = void(*)(::uns::lua::auxiliary::state&, const ::uns::lua::value&) noexcept;
		::uns::lua::value_type m_type = ::uns::lua::value_type::nil;
		::uns::lua::type::boolean m_boolean = false;
		::uns::lua::type::number m_number = static_cast<::uns::lua::type::number>(0);
		::uns::lua::type::integer m_integer = static_cast<::uns::lua::type::integer>(0);
		::uns::lua::type::string m_string = ::uns::lua::type::string{};
		::uns::lua::type::table m_table = ::uns::lua::type::table{};
		push_function_type m_push_function = push_nil;
	public:
		value() noexcept {};
		/*
#define UNS_LUA_VALUE_INIT_DECLARATOR(type_identifier)											\
		value(const ::uns::lua::type::##type_identifier& obj) noexcept :						\
			m_type(::uns::lua::value_type::##type_identifier),									\
			m_##type_identifier(obj),															\
			m_push_function(push_##type_identifier)												\
		{};																						\

		UNS_LUA_VALUE_INIT_DECLARATOR(boolean);
		UNS_LUA_VALUE_INIT_DECLARATOR(number);
		UNS_LUA_VALUE_INIT_DECLARATOR(integer);
		UNS_LUA_VALUE_INIT_DECLARATOR(string);
		UNS_LUA_VALUE_INIT_DECLARATOR(table);
#undef UNS_LUA_VALUE_INIT_DECLARATOR
*/
		template<typename val_t>
		requires(::std::integral<val_t> && !::std::same_as<bool, val_t>)
		value(val_t obj) noexcept : 
			m_type(::uns::lua::value_type::integer), 
			m_integer(static_cast<::uns::lua::type::integer>(obj)), 
			m_push_function(push_integer)												
		{};
		template<::std::floating_point val_t>
		value(val_t obj) noexcept :
			m_type(::uns::lua::value_type::number),
			m_number(static_cast<::uns::lua::type::number>(obj)),
			m_push_function(push_number)
		{};
		template<::std::same_as<bool> val_t>
		value(val_t obj) noexcept :
			m_type(::uns::lua::value_type::boolean),
			m_boolean(obj),
			m_push_function(push_boolean)
		{};
		value(const ::std::string obj) noexcept :
			m_type(::uns::lua::value_type::string),
			m_string(obj),
			m_push_function(push_string)
		{};
		value(const char* obj) noexcept : value(::std::string{ obj } ) {};
		value(const ::std::string_view obj) noexcept :
			m_type(::uns::lua::value_type::string),
			m_string(obj),
			m_push_function(push_string)
		{};
		value(const ::std::u8string obj) noexcept : value(::uns::string::u8_cast<::std::string>(obj)) {};
		value(const char8_t* obj) noexcept : value(::std::u8string{ obj }) {};
		value(const ::uns::lua::type::table& obj) noexcept :
			m_type(::uns::lua::value_type::table),
			m_table(obj),
			m_push_function(push_table)
		{};
		value(const ::uns::lua::type::nil& obj) noexcept :
			m_type(::uns::lua::value_type::nil),
			m_push_function(push_nil)
		{};
		value(const ::uns::lua::value& obj) noexcept :
			m_type(obj.m_type),
			m_boolean(obj.m_boolean),
			m_number(obj.m_number),
			m_integer(obj.m_integer),
			m_string(obj.m_string),
			m_table(obj.m_table),
			m_push_function(obj.m_push_function)
		{};
		::uns::lua::value& operator=(const ::uns::lua::value& obj) noexcept {
			if(this == &obj) {
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
		value(::uns::lua::value&& obj) noexcept :
			m_type(std::move(obj.m_type)),
			m_boolean(std::move(obj.m_boolean)),
			m_number(std::move(obj.m_number)),
			m_integer(std::move(obj.m_integer)),
			m_string(std::move(obj.m_string)),
			m_table(std::move(obj.m_table)),
			m_push_function(std::move(obj.m_push_function))
		{};
		::uns::lua::value& operator=(::uns::lua::value&& obj) noexcept {
			if(this == &obj) {
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
		~value() noexcept {};

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
		operator const ::uns::lua::type::##type_identifier&() const noexcept {					\
			return m_##type_identifier;															\
		};																						\
		operator ::uns::lua::type::##type_identifier&() noexcept {								\
			return m_##type_identifier;															\
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
		};			\
		bool operator!=(const ::uns::lua::type::##type_identifier& obj) const noexcept {		\
			return !(*this == obj);																\
		};			\

		UNS_LUA_VALUE_COMPARISON_DECLARATOR(boolean);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(number);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(integer);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(string);
		UNS_LUA_VALUE_COMPARISON_DECLARATOR(table);
#undef UNS_LUA_VALUE_COMPARISON_DECLARATOR

		::uns::lua::value_type type() const noexcept { return m_type; };

		void push_to(::uns::lua::auxiliary::state& lua_state) const noexcept {
			m_push_function(lua_state, *this);
		};

		static ::uns::lua::value make_from(::uns::lua::auxiliary::state& lua_state, int idx) noexcept;
	protected:
		static void push_nil(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {
			lua_pushnil(lua_state.get());
		};
#define UNS_LUA_VALUE_PUSH_DECLARATOR(type_identifier)											\
		static void push_##type_identifier(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {\
			lua_push##type_identifier(lua_state.get(), static_cast<::uns::lua::type::##type_identifier>(value));\
		};																						\

		UNS_LUA_VALUE_PUSH_DECLARATOR(boolean);
		UNS_LUA_VALUE_PUSH_DECLARATOR(number);
		UNS_LUA_VALUE_PUSH_DECLARATOR(integer);

		static void push_string(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {
			lua_pushstring(lua_state.get(), static_cast<::uns::lua::type::string>(value).c_str());
		};
		static void push_table(::uns::lua::auxiliary::state& lua_state, const ::uns::lua::value& value) noexcept {
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

#undef UNS_LUA_VALUE_PUSH_DECLARATOR
	};


	namespace auxiliary {
		class table {
		protected:
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
				m_key_string(obj.m_key_string) {};
			::uns::lua::auxiliary::table& operator=(const ::uns::lua::auxiliary::table& obj) noexcept {
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
				m_key_string(std::move(obj.m_key_string)) {};
			::uns::lua::auxiliary::table& operator=(::uns::lua::auxiliary::table&& obj) noexcept {
				if(this != &obj) {
					m_key_number = std::move(obj.m_key_number);
					m_key_integer = std::move(obj.m_key_integer);
					m_key_boolean = std::move(obj.m_key_boolean);
					m_key_string = std::move(obj.m_key_string);
				};

				return *this;
			};
			~table() noexcept {}

#define UNS_LUA_TABLE_IDX_DECLARATOR(type_identifier)											\
			::uns::lua::value operator[] (const ::uns::lua::type::##type_identifier& key) const noexcept {\
				if(auto value_iter = m_key_##type_identifier.find(key); value_iter != m_key_##type_identifier.end()) {\
					return value_iter->second;													\
				}																				\
				else {																			\
					return ::uns::lua::nil;														\
				};																				\
			};																					\
			::uns::lua::value& operator[] (const ::uns::lua::type::##type_identifier& key) noexcept { return m_key_##type_identifier[key]; };\

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

			::std::size_t size() const noexcept {
				return m_key_number.size() + m_key_integer.size() + m_key_boolean.size() + m_key_string.size();
			};

			template<class key_t>
			::uns::lua::type::table::const_iterator<key_t> cbegin() const noexcept { return ::uns::lua::type::table::const_iterator<key_t>{}; };
			template<class key_t>
			::uns::lua::type::table::const_iterator<key_t> cend() const noexcept { return ::uns::lua::type::table::const_iterator<key_t>{}; };
			template<class key_t>
			::uns::lua::type::table::iterator<key_t> begin() noexcept { return ::uns::lua::type::table::iterator<key_t>{}; };
			template<class key_t>
			::uns::lua::type::table::iterator<key_t> end() noexcept { return ::uns::lua::type::table::iterator<key_t>{}; };

#define UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(type_identifier)								\
			template<>																			\
			::uns::lua::type::table::const_iterator<::uns::lua::type::##type_identifier> cbegin<::uns::lua::type::##type_identifier>() const noexcept { return m_key_##type_identifier.cbegin(); };\
			template<>																			\
			::uns::lua::type::table::const_iterator<::uns::lua::type::##type_identifier> cend<::uns::lua::type::##type_identifier>() const noexcept { return m_key_##type_identifier.cend(); };\
			template<>																			\
			::uns::lua::type::table::iterator<::uns::lua::type::##type_identifier> begin<::uns::lua::type::##type_identifier>() noexcept { return m_key_##type_identifier.begin(); };\
			template<>																			\
			::uns::lua::type::table::iterator<::uns::lua::type::##type_identifier> end<::uns::lua::type::##type_identifier>() noexcept { return m_key_##type_identifier.end(); };


			UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(number);
			UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(integer);
			UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(boolean);
			UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(string);

#undef UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR
		};
	};

	//definitions of ::uns::lua::type::table methods =>
	::uns::lua::type::table::table() noexcept : m_ptr(::std::unique_ptr<::uns::lua::auxiliary::table>{ new ::uns::lua::auxiliary::table{} }) {};
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

	template<class key_t>
	::uns::lua::type::table::const_iterator<key_t> uns::lua::type::table::cbegin() const noexcept { return m_ptr->cbegin<key_t>(); };
	template<class key_t>
	::uns::lua::type::table::const_iterator<key_t> uns::lua::type::table::cend() const noexcept { return m_ptr->cend<key_t>(); };
	template<class key_t>
	::uns::lua::type::table::iterator<key_t> uns::lua::type::table::begin() noexcept { return m_ptr->begin<key_t>(); };
	template<class key_t>
	::uns::lua::type::table::iterator<key_t> uns::lua::type::table::end() noexcept { return m_ptr->end<key_t>(); };
	//<= definitions of ::uns::lua::type::table methods 

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
				};

				return ::uns::lua::value{ res };
			}
		};
	};


	class script;


	class function {
		friend ::uns::lua::script;

		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::std::string m_function_name = "";
		::uns::lua::error m_err;
	public:
		function() noexcept {};
	protected:
		function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name) noexcept :
			m_function_name(lua_global_function_name),
			m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state{} })
		{
			if(lua_script != nullptr) {
				*m_stack = *lua_script;
			};
		};
	public:
		function(const ::uns::lua::function&) noexcept = default;
		::uns::lua::function& operator=(const ::uns::lua::function&) noexcept = default;
		function(::uns::lua::function&& obj) noexcept = default;
		::uns::lua::function& operator=(::uns::lua::function&& obj) noexcept = default;
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

			if(int lua_retcode = lua_pcall(m_stack->get(), args.size(), expected_results, 0); lua_retcode != LUA_OK) {
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

		void gc() noexcept {
			lua_gc(m_stack->get(), LUA_GCCOLLECT);
		};
	};


	class global {
		friend ::uns::lua::script;

		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::std::string m_global_name = "";
		::uns::lua::error m_err;
	public:
		global() noexcept {};
	protected:
		global(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_variable_name) noexcept :
			m_global_name(lua_global_variable_name),
			m_stack(::std::shared_ptr<::uns::lua::auxiliary::state>{ new ::uns::lua::auxiliary::state})
		{//TODO to rewrite this constructor with script first argument eliminating friend directive
			if(lua_script != nullptr) {
				*m_stack = *lua_script;
			};
		};
	public:
		global(const global&) noexcept = default;
		global& operator=(const global&) noexcept = default;
		global(global&& obj) noexcept = default;
		global& operator=(global&& obj) noexcept = default;
		~global() noexcept {
			lua_gc(m_stack->get(), LUA_GCCOLLECT);
		};

		const ::uns::lua::error& error() const noexcept { return m_err; };
		::uns::lua::error& error() noexcept { return m_err; };

		::std::u8string name() const noexcept { return ::uns::string::u8_cast<::std::u8string>(m_global_name); };

		bool valid() const noexcept { return m_stack != nullptr; };

		::uns::lua::value get() noexcept {
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

		void set(const ::uns::lua::value& value) noexcept {
			m_err = ::uns::lua::error{};

			if(!valid()) {
				m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid };
				return;
			};

			value.push_to(*m_stack);
			lua_setglobal(m_stack->get(), m_global_name.c_str());
		};

		void gc() noexcept {
			lua_gc(m_stack->get(), LUA_GCCOLLECT);
		};
	};


	class script {
	protected:
		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::uns::lua::error m_err;
	public:
		script() noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {};
		script(const ::std::u8string& text) noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {
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
		script(const ::std::filesystem::path& file) noexcept : m_stack(new ::uns::lua::auxiliary::state{}) {
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
		script(const ::uns::lua::script& obj) noexcept :
			m_stack(obj.m_stack),
			m_err(obj.m_err)
		{};
		::uns::lua::script operator=(const ::uns::lua::script& obj) noexcept {
			if(this != &obj) {
				m_stack = obj.m_stack;
				m_err = obj.m_err;
			};
			return *this;
		};
		script(::uns::lua::script&& obj) noexcept :
			m_stack(::std::move(obj.m_stack)),
			m_err(::std::move(obj.m_err))
		{};
		::uns::lua::script operator=(::uns::lua::script&& obj) noexcept {
			if(this != &obj) {
				m_stack = ::std::move(obj.m_stack);
				m_err = ::std::move(obj.m_err);
			};
			return *this;
		};
		~script() noexcept {};

		bool valid() const noexcept { return m_stack != nullptr; };

		const ::uns::lua::error& error() const noexcept { return m_err; };
		::uns::lua::error& error() noexcept { return m_err; };

		::uns::lua::function get_function(const ::std::u8string& lua_global_function_name) noexcept {
			if(!valid()) {
				m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::u8_cast<::std::string>(lua_global_function_name)};
				return ::uns::lua::function{};
			}
			else {
				return ::uns::lua::function{ m_stack, ::uns::string::u8_cast<::std::string>(lua_global_function_name) };
			};
		};
		::uns::lua::global get_global(const ::std::u8string& lua_global_variable_name) noexcept {
			if(!valid()) {
				m_err = ::uns::lua::error{ ::uns::lua::errcode::errcall, ::uns::lua::errtype::invalid, ::uns::string::u8_cast<::std::string>(lua_global_variable_name) };
				return ::uns::lua::global{};
			}
			else {
				return ::uns::lua::global{ m_stack, ::uns::string::u8_cast<::std::string>(lua_global_variable_name) };
			};
		};

		void run() noexcept {
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
		
		void gc() noexcept {
			lua_gc(m_stack->get(), LUA_GCCOLLECT);
		};
	};
};