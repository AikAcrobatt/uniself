#pragma once


#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <tuple>

#include "uniself/strings.hpp"
#include "uniself/benum.hpp"

#ifndef UNS_LIB_LUA_WRAPPER
#define UNS_LIB_LUA_WRAPPER

namespace uns::lua {

	
	UNS_BENUM_DECLARATOR(errcode, int,
		ok,
		yeld,
		errrun,
		errsyntax,
		errmem,
		errerr,
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
		) noexcept;
		error(const ::uns::lua::error& obj) noexcept;
		::uns::lua::error& operator=(const ::uns::lua::error& obj) noexcept;
		error(::uns::lua::error&& obj) noexcept;
		::uns::lua::error& operator=(::uns::lua::error&& obj) noexcept;
		~error() noexcept;

		void swap(::uns::lua::error& obj) noexcept;

		bool is() const noexcept;

		::uns::lua::errcode code() const noexcept;
		::uns::lua::errtype type() const noexcept;
		::std::string text() const noexcept;

		::std::u8string to_string() const;
	};


	namespace alias {
		using lua_state = void*;
		using lua_cfunction = int(*)(::uns::lua::alias::lua_state);
	};


	namespace auxiliary {
		class table;

		class state;

		class state_wrapper {
		protected:
			::uns::lua::alias::lua_state m_state = nullptr;
		public:
			inline state_wrapper() noexcept :
				m_state(nullptr) {};
			inline state_wrapper(::uns::lua::alias::lua_state state) noexcept :
				m_state(state) {};
			inline state_wrapper(const ::uns::lua::auxiliary::state_wrapper& obj) noexcept :
				m_state(obj.m_state) {};
			inline ::uns::lua::auxiliary::state_wrapper& operator=(const ::uns::lua::auxiliary::state_wrapper& obj) noexcept {
				if(this == &obj) return *this;

				m_state = obj.m_state;

				return *this;
			};
			inline state_wrapper(::uns::lua::auxiliary::state_wrapper&& obj) noexcept :
				m_state(obj.m_state) {};
			inline ::uns::lua::auxiliary::state_wrapper& operator=(::uns::lua::auxiliary::state_wrapper&& obj) noexcept {
				if(this == &obj) return *this;

				m_state = obj.m_state;

				return *this;
			};
			inline ~state_wrapper() {};

			inline bool valid() const noexcept { return m_state != nullptr; };

			inline const ::uns::lua::alias::lua_state get() const noexcept { return m_state; };
			inline ::uns::lua::alias::lua_state get() noexcept { return m_state; };
		};


	};


	UNS_BENUM_DECLARATOR(value_type, int,
		nil = -1,
		boolean = 0,
		number = 1,
		integer = 2,
		string = 3,
		table = 4,
		userdata = 5
	);


	class value;


	namespace type {
		class nil {};

		using boolean = bool;
		using number = long double;
		using integer = long long int;
		using string = ::std::string;
		using userdata = void*;

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
			~table() noexcept;

			bool operator==(const ::uns::lua::type::table&) const noexcept;
			bool operator!=(const ::uns::lua::type::table&) const noexcept;

			template<::std::floating_point key_t>
			::uns::lua::value operator[] (const key_t&) const noexcept;
			template<::std::floating_point key_t>
			::uns::lua::value& operator[] (const key_t&) noexcept;
			template<typename key_t>
				requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
			::uns::lua::value operator[] (const key_t&) const noexcept;
			template<typename key_t>
				requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
			::uns::lua::value& operator[] (const key_t&) noexcept;
			template<::std::same_as<::uns::lua::type::boolean> key_t>
			::uns::lua::value operator[] (const key_t&) const noexcept;
			template<::std::same_as<::uns::lua::type::boolean> key_t>
			::uns::lua::value& operator[] (const key_t&) noexcept;
			::uns::lua::value operator[] (const ::uns::lua::type::string&) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::type::string&) noexcept;
			::uns::lua::value operator[] (const char*) const noexcept;
			::uns::lua::value& operator[] (const char*) noexcept;
			::uns::lua::value operator[] (const ::std::u8string&) const noexcept;
			::uns::lua::value& operator[] (const ::std::u8string&) noexcept;
			::uns::lua::value operator[] (const char8_t*) const noexcept;
			::uns::lua::value& operator[] (const char8_t*) noexcept;
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
		using push_function_type = void(*)(::uns::lua::alias::lua_state, const ::uns::lua::value&) noexcept;
		::uns::lua::value_type m_type = ::uns::lua::value_type::nil;
		::uns::lua::type::boolean m_boolean = false;
		::uns::lua::type::number m_number = static_cast<::uns::lua::type::number>(0);
		::uns::lua::type::integer m_integer = static_cast<::uns::lua::type::integer>(0);
		::uns::lua::type::string m_string = ::uns::lua::type::string{};
		::uns::lua::type::table m_table = ::uns::lua::type::table{};
		::uns::lua::type::userdata m_userdata = nullptr;
		push_function_type m_push_function = push_nil;
	public:
		inline value() noexcept {};
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
		value(const ::std::string obj) noexcept;
		value(const char* obj) noexcept;
		value(const ::std::string_view obj) noexcept;
		value(const ::std::u8string obj) noexcept;
		value(const char8_t* obj) noexcept;
		value(const ::uns::lua::type::table& obj) noexcept;
		value(::uns::lua::type::userdata obj) noexcept;
		value(const ::uns::lua::type::nil& obj) noexcept;
		value(const ::uns::lua::value& obj) noexcept;
		::uns::lua::value& operator=(const ::uns::lua::value& obj) noexcept;
		value(::uns::lua::value&& obj) noexcept;
		::uns::lua::value& operator=(::uns::lua::value&& obj) noexcept;
		~value() noexcept;

		template<typename val_t>
		::uns::lua::value& operator=(const val_t& obj) noexcept {
			return (*this = ::uns::lua::value{ obj });
		};

		bool operator==(const ::uns::lua::value& obj) const noexcept;
		bool operator!=(const ::uns::lua::value& obj) const noexcept;

#define UNS_LUA_VALUE_CONVERT_DECLARATOR(type_identifier)										\
		operator const ::uns::lua::type::##type_identifier&() const noexcept;					\
		operator ::uns::lua::type::##type_identifier&() noexcept;								\

		UNS_LUA_VALUE_CONVERT_DECLARATOR(boolean);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(number);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(integer);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(string);
		UNS_LUA_VALUE_CONVERT_DECLARATOR(table);

		operator const ::uns::lua::type::userdata() const noexcept;
		operator ::uns::lua::type::userdata& () noexcept;
#undef UNS_LUA_VALUE_CONVERT_DECLARATOR

		inline ::uns::lua::value_type type() const noexcept { return m_type; };

		void push_to(::uns::lua::auxiliary::state& thread) const noexcept;
		void push_to(::uns::lua::alias::lua_state stack) const noexcept;

		::std::u8string to_string() const noexcept;

		static ::uns::lua::value make_from(::uns::lua::auxiliary::state& thread, int idx) noexcept;
		static ::uns::lua::value make_from(::uns::lua::alias::lua_state stack, int idx) noexcept;
	protected:
		static void push_nil(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) noexcept;
#define UNS_LUA_VALUE_PUSH_DECLARATOR(type_identifier)											\
		static void push_##type_identifier(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value) noexcept;																						\

		UNS_LUA_VALUE_PUSH_DECLARATOR(boolean);
		UNS_LUA_VALUE_PUSH_DECLARATOR(number);
		UNS_LUA_VALUE_PUSH_DECLARATOR(integer);
		UNS_LUA_VALUE_PUSH_DECLARATOR(string);
		UNS_LUA_VALUE_PUSH_DECLARATOR(table);
		UNS_LUA_VALUE_PUSH_DECLARATOR(userdata);
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
			inline table() noexcept {};
			table(const ::uns::lua::auxiliary::table& obj) noexcept;
			::uns::lua::auxiliary::table& operator=(const ::uns::lua::auxiliary::table& obj) noexcept;
			table(::uns::lua::auxiliary::table&& obj) noexcept;
			::uns::lua::auxiliary::table& operator=(::uns::lua::auxiliary::table&& obj) noexcept;
			~table() noexcept;

			bool operator==(const ::uns::lua::auxiliary::table&) const noexcept;
			bool operator!=(const ::uns::lua::auxiliary::table&) const noexcept;

#define UNS_LUA_TABLE_IDX_DECLARATOR(type_identifier)											\
			::uns::lua::value operator[] (const ::uns::lua::type::##type_identifier& key) const noexcept;\
			::uns::lua::value& operator[] (const ::uns::lua::type::##type_identifier& key) noexcept;\

			UNS_LUA_TABLE_IDX_DECLARATOR(number);
			UNS_LUA_TABLE_IDX_DECLARATOR(integer);
			UNS_LUA_TABLE_IDX_DECLARATOR(boolean);
			UNS_LUA_TABLE_IDX_DECLARATOR(string);
#undef UNS_LUA_TABLE_IDX_DECLARATOR

			::uns::lua::value operator[] (const ::uns::lua::value& key) const noexcept;
			::uns::lua::value& operator[] (const ::uns::lua::value& key) noexcept;

			::std::size_t size() const noexcept;

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


	//definitions of ::uns::type::table methods =>
	template<::std::floating_point key_t>
	::uns::lua::value uns::lua::type::table::operator[] (const key_t& key) const noexcept { return m_ptr->operator[](static_cast<::uns::lua::type::number>(key)); };
	template<::std::floating_point key_t>
	::uns::lua::value& ::uns::lua::type::table::operator[] (const key_t& key) noexcept { return m_ptr->operator[](static_cast<::uns::lua::type::number>(key)); };
	template<typename key_t>
		requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
	::uns::lua::value uns::lua::type::table::operator[] (const key_t& key) const noexcept { return m_ptr->operator[](static_cast<::uns::lua::type::integer>(key)); };
	template<typename key_t>
		requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
	::uns::lua::value& ::uns::lua::type::table::operator[] (const key_t& key) noexcept { return m_ptr->operator[](static_cast<::uns::lua::type::integer>(key)); };
	template<::std::same_as<::uns::lua::type::boolean> key_t>
	::uns::lua::value uns::lua::type::table::operator[] (const key_t& key) const noexcept { return m_ptr->operator[](static_cast<::uns::lua::type::boolean>(key)); };
	template<::std::same_as<::uns::lua::type::boolean> key_t>
	::uns::lua::value& ::uns::lua::type::table::operator[] (const key_t& key) noexcept { return m_ptr->operator[](static_cast<::uns::lua::type::boolean>(key)); };

	template<class key_t>
	::uns::lua::type::table::const_iterator<key_t> uns::lua::type::table::cbegin() const noexcept { return m_ptr->cbegin<key_t>(); };
	template<class key_t>
	::uns::lua::type::table::const_iterator<key_t> uns::lua::type::table::cend() const noexcept { return m_ptr->cend<key_t>(); };
	template<class key_t>
	::uns::lua::type::table::iterator<key_t> uns::lua::type::table::begin() noexcept { return m_ptr->begin<key_t>(); };
	template<class key_t>
	::uns::lua::type::table::iterator<key_t> uns::lua::type::table::end() noexcept { return m_ptr->end<key_t>(); };
	//<= definitions of ::uns::type::table methods 

	class thread;
	class script;


	class function {
		friend ::uns::lua::script;
		friend ::uns::lua::thread;
	protected:
		::uns::lua::auxiliary::state_wrapper m_stack_wrapper;
		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::std::string m_function_name = "";
		::uns::lua::error m_err;
	public:
		inline function() noexcept {};
	protected:
		function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name) noexcept;
		function(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_function_name) noexcept;
	public:
		function(const ::uns::lua::function&) noexcept = default;
		::uns::lua::function& operator=(const ::uns::lua::function&) noexcept = default;
		function(::uns::lua::function&& obj) noexcept = default;
		::uns::lua::function& operator=(::uns::lua::function&& obj) noexcept = default;
		~function() noexcept;

		inline const ::uns::lua::error& error() const noexcept { return m_err; };
		inline ::uns::lua::error& error() noexcept { return m_err; };

		::std::u8string name() const noexcept;

		inline bool valid() const noexcept { return m_stack != nullptr || m_stack_wrapper.valid(); };

		::std::vector<::uns::lua::value> operator() (const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args) noexcept;

		void gc() noexcept;
	};


	class global {
		friend ::uns::lua::script;
		friend ::uns::lua::thread;
	protected:
		::uns::lua::auxiliary::state_wrapper m_stack_wrapper;
		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::std::string m_global_name = "";
		::uns::lua::error m_err;
	public:
		inline global() noexcept {};
	protected:
		global(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_variable_name) noexcept;
		global(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_variable_name) noexcept;
	public:
		global(const ::uns::lua::global&) noexcept = default;
		::uns::lua::global& operator=(const ::uns::lua::global&) noexcept = default;
		global(::uns::lua::global&& obj) noexcept = default;
		::uns::lua::global& operator=(::uns::lua::global&& obj) noexcept = default;
		::uns::lua::global& operator=(const ::uns::lua::value& value) noexcept;
		~global() noexcept;

		inline const ::uns::lua::error& error() const noexcept { return m_err; };
		inline ::uns::lua::error& error() noexcept { return m_err; };

		::std::u8string name() const noexcept;

		inline bool valid() const noexcept { return m_stack != nullptr || m_stack_wrapper.valid(); };

		::uns::lua::value get() noexcept;
		void set(const ::uns::lua::value& value) noexcept;

		void gc() noexcept;
	};


	class lib_entry;
	class library;


	class thread {
		friend ::uns::lua::lib_entry;
	protected:
		mutable ::uns::lua::auxiliary::state_wrapper m_stack;
		::uns::lua::error m_err;
	protected:
		thread() = delete;
	public:
		thread(::uns::lua::alias::lua_state) noexcept;
		thread(const ::uns::lua::thread&) noexcept;
		::uns::lua::thread& operator=(const ::uns::lua::thread&) noexcept;
		thread(::uns::lua::thread&&) noexcept;
		::uns::lua::thread& operator=(::uns::lua::thread&&) noexcept;
		~thread() = default;

		inline bool valid() const noexcept { return m_stack.valid(); };

		inline const ::uns::lua::error& error() const noexcept { return m_err; };
		inline ::uns::lua::error& error() noexcept { return m_err; };

		void load(const ::uns::lua::library& library) noexcept;
		void load(const ::std::u8string& text) noexcept;

		::uns::lua::function get_function(const ::std::u8string& lua_global_function_name) noexcept;
		::uns::lua::global get_global(const ::std::u8string& lua_global_variable_name) noexcept;

		::std::size_t size() const noexcept;
		::uns::lua::value get_value(int input_index) noexcept;

		void gc() noexcept;
	};


	namespace auxiliary {

		template<::std::size_t pos, typename ... uns_lua_value_t>
		void results_push_recursive(::uns::lua::alias::lua_state stack, const ::std::tuple<uns_lua_value_t ...>& results) noexcept {
			if constexpr(
				::std::tuple_size<::std::tuple<uns_lua_value_t ...>>::value > 0
			) {
				if constexpr(
					pos <= ::std::tuple_size<::std::tuple<uns_lua_value_t ...>>::value
					&& pos > 0
				) {
					::uns::lua::auxiliary::results_push_recursive<pos - 1>(stack, results);
					::std::get<pos - 1>(results).push_to(stack);
				}
				else {
					::std::get<0>(results).push_to(stack);
				};
			};
		};

		template<typename ::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&) noexcept>
		int wrapper_returning_all(::uns::lua::alias::lua_state stack) noexcept {
			auto l_thread = ::uns::lua::thread{ stack };
			auto results = wrapped(l_thread);

			for(const auto& result : results) {
				result.push_to(stack);
			};

			return results.size();
		};

		template<typename tupled_lua_values, typename tupled_lua_values(*wrapped)(::uns::lua::thread&) noexcept>
			requires (::std::tuple_size<tupled_lua_values>::value >= 0)
		int wrapper_returning_some(::uns::lua::alias::lua_state stack) noexcept {
			auto l_thread = ::uns::lua::thread{ stack };
			auto results = wrapped(l_thread);

			::uns::lua::auxiliary::results_push_recursive<::std::tuple_size<tupled_lua_values>::value>(stack, results);

			return ::std::tuple_size<tupled_lua_values>::value;
		};
	
	};


	class lib_entry {
	protected:
		::std::string m_name;
		::uns::lua::alias::lua_cfunction m_function = nullptr;
	public:
		inline lib_entry() noexcept {};
		lib_entry(const ::std::u8string& name, ::uns::lua::alias::lua_cfunction lua_function) noexcept;
		lib_entry(const ::uns::lua::lib_entry&) noexcept = default;
		::uns::lua::lib_entry& operator=(const ::uns::lua::lib_entry&) noexcept = default;
		lib_entry(::uns::lua::lib_entry&&) noexcept = default;
		::uns::lua::lib_entry& operator=(::uns::lua::lib_entry&&) noexcept = default;
		~lib_entry() noexcept = default;

		inline const char* name() const noexcept { return m_name.c_str(); };
		inline ::uns::lua::alias::lua_cfunction func() const noexcept { return m_function; };

		template<typename ::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&) noexcept>
		static ::uns::lua::lib_entry make(const ::std::u8string& name) noexcept {
			auto res = ::uns::lua::lib_entry{};

			res.m_name = name;
			res.m_function = ::uns::lua::auxiliary::wrapper_returning_all<wrapped>;

			return res;
		};
		template<typename tupled_lua_values, typename tupled_lua_values(*wrapped)(::uns::lua::thread&) noexcept>
			requires (::std::tuple_size<tupled_lua_values>::value >= 0)
		static ::uns::lua::lib_entry make(const ::std::u8string& name) noexcept {
			auto res = ::uns::lua::lib_entry{};

			res.m_name = ::uns::string::u8_cast<::std::string>(name);
			res.m_function = ::uns::lua::auxiliary::wrapper_returning_some<tupled_lua_values, wrapped>;

			return res;
		};

	};


	class library {
	public:
		::std::u8string name_space = u8"";
		::std::vector<::uns::lua::lib_entry> api;
		::std::u8string text = u8"";
	};


	class script {
	protected:
		::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
		::uns::lua::error m_err;
	public:
		script() noexcept;
		script(const ::std::u8string& text) noexcept;
		script(const ::std::filesystem::path& file) noexcept;
		script(const ::uns::lua::library& library) noexcept;
		script(const ::uns::lua::script& obj) noexcept;
		::uns::lua::script& operator=(const ::uns::lua::script& obj) noexcept;
		script(::uns::lua::script&& obj) noexcept;
		::uns::lua::script& operator=(::uns::lua::script&& obj) noexcept;
		~script() noexcept;

		void load(const ::uns::lua::library& library) noexcept;
		void load(const ::std::u8string& text) noexcept;
		void load(const ::std::filesystem::path& file) noexcept;

		bool valid() const noexcept { return m_stack != nullptr; };

		inline const ::uns::lua::error& error() const noexcept { return m_err; };
		inline ::uns::lua::error& error() noexcept { return m_err; };

		::uns::lua::function get_function(const ::std::u8string& lua_global_function_name) noexcept;
		::uns::lua::global get_global(const ::std::u8string& lua_global_variable_name) noexcept;

		void run() noexcept;
		
		void gc() noexcept;
		long double total_memory() const noexcept;
	};


	::uns::lua::value make_table() noexcept;
};

#endif