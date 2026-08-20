#pragma once


#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <tuple>
#include <array>

#include "uniself/strings.hpp"
#include "uniself/renum.hpp"

#ifndef UNS_LIB_LUA_WRAPPER
#define UNS_LIB_LUA_WRAPPER

struct lua_State;

namespace uns::lua {

    UNS_RENUM(errcode, int
        , (ok,)
        , (yeld,)
        , (errrun,)
        , (errsyntax,)
        , (errmem,)
        , (errerr,)
        , (errcall,)
    );


    UNS_RENUM(errtype, int
        , (ok, = 0x0)
        , (lua_specific,)
        , (invalid,)
        , (uncallable,)
        , (unrepresentable,)
        , (not_found,)
    );


    class error {
    private:
        ::uns::lua::errcode m_code = ::uns::lua::errcode::ok;
        ::uns::lua::errtype m_type = ::uns::lua::errtype::ok;
        ::std::string m_text = "";
    public:
        error(
            const ::uns::lua::errcode& code = ::uns::lua::errcode::ok,
            const ::uns::lua::errtype& type = ::uns::lua::errtype::ok,
            const ::std::string text = ""
        );
        error(const ::uns::lua::error& obj);
        ::uns::lua::error& operator=(const ::uns::lua::error& obj);
        error(::uns::lua::error&& obj);
        ::uns::lua::error& operator=(::uns::lua::error&& obj);
        ~error();
    public:
        void swap(::uns::lua::error& obj);
    public:
        bool is() const;
        ::uns::lua::errcode code() const;
        ::uns::lua::errtype type() const;
        ::std::string text() const;
    public:
        ::std::u32string to_string() const;
    };


    namespace alias {
        using lua_state = ::lua_State*;
        using lua_cfunction = int(*)(::uns::lua::alias::lua_state);
    };


    namespace auxiliary {

        class table;

        class state;

        class state_wrapper {
        private:
            ::uns::lua::alias::lua_state m_state = nullptr;
        public:
            inline state_wrapper() :
                m_state(nullptr) {};
            inline state_wrapper(::uns::lua::alias::lua_state state) :
                m_state(state) {};
            inline state_wrapper(const ::uns::lua::auxiliary::state_wrapper& obj) :
                m_state(obj.m_state) {};
            inline ::uns::lua::auxiliary::state_wrapper& operator=(const ::uns::lua::auxiliary::state_wrapper& obj) {
                if(this == &obj) return *this;

                m_state = obj.m_state;

                return *this;
            };
            inline state_wrapper(::uns::lua::auxiliary::state_wrapper&& obj) :
                m_state(obj.m_state) {};
            inline ::uns::lua::auxiliary::state_wrapper& operator=(::uns::lua::auxiliary::state_wrapper&& obj) {
                if(this == &obj) return *this;

                m_state = obj.m_state;

                return *this;
            };
            inline ~state_wrapper() {};
        public:
            inline bool valid() const { return m_state != nullptr; };
        public:
            inline const ::uns::lua::alias::lua_state get() const { return m_state; };
            inline ::uns::lua::alias::lua_state get() { return m_state; };
        };
    };


    UNS_RENUM(value_type, int
        , (nil, = -1)
        , (boolean, = 0)
        , (number, = 1)
        , (integer, = 2)
        , (string, = 3)
        , (table, = 4)
        , (userdata, = 5)
    );


    class value;
    class thread;


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
        public:
            template<class key_t>
            requires ::std::same_as<::uns::lua::type::number, key_t>
                || ::std::same_as<::uns::lua::type::integer, key_t>
                || ::std::same_as<::uns::lua::type::boolean, key_t>
                || ::std::same_as<::uns::lua::type::string, key_t>
            using iterator = std::unordered_map<key_t, ::uns::lua::value>::iterator;
        private:
            ::std::unique_ptr<::uns::lua::auxiliary::table> m_ptr = nullptr;
        public:
            table();
            table(const ::uns::lua::type::table& obj);
            ::uns::lua::type::table& operator=(const ::uns::lua::type::table&);
            table(::uns::lua::type::table&&);
            ::uns::lua::type::table& operator=(::uns::lua::type::table&&);
            ~table();
        public:
            ::std::u32string to_string() const;
        public:
            bool operator==(const ::uns::lua::type::table&) const;
            bool operator!=(const ::uns::lua::type::table&) const;
        public:
            template<::std::floating_point key_t>
            ::uns::lua::value operator[] (const key_t&) const;
            template<::std::floating_point key_t>
            ::uns::lua::value& operator[] (const key_t&);
            template<typename key_t>
                requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
            ::uns::lua::value operator[] (const key_t&) const;
            template<typename key_t>
                requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
            ::uns::lua::value& operator[] (const key_t&);
            template<::std::same_as<::uns::lua::type::boolean> key_t>
            ::uns::lua::value operator[] (const key_t&) const;
            template<::std::same_as<::uns::lua::type::boolean> key_t>
            ::uns::lua::value& operator[] (const key_t&);
            ::uns::lua::value operator[] (const ::uns::lua::type::string&) const;
            ::uns::lua::value& operator[] (const ::uns::lua::type::string&);
            ::uns::lua::value operator[] (const char*) const;
            ::uns::lua::value& operator[] (const char*);
            ::uns::lua::value operator[] (const ::std::u32string&) const;
            ::uns::lua::value& operator[] (const ::std::u32string&);
            ::uns::lua::value operator[] (const char32_t*) const;
            ::uns::lua::value& operator[] (const char32_t*);
            ::uns::lua::value operator[] (const ::uns::lua::value&) const;
            ::uns::lua::value& operator[] (const ::uns::lua::value&);
        public:
            ::std::size_t size() const;
        public:
            template<class key_t>
            const_iterator<key_t> cbegin() const;
            template<class key_t>
            const_iterator<key_t> cend() const;
        public:
            template<class key_t>
            iterator<key_t> begin();
            template<class key_t>
            iterator<key_t> end();
        };
    };


    static constexpr auto nil = ::uns::lua::type::nil{};


    class value {
    private:
        using push_function_type = void(*)(::uns::lua::alias::lua_state, const ::uns::lua::value&);
    private:
        ::uns::lua::value_type m_type = ::uns::lua::value_type::nil;
        ::uns::lua::type::boolean m_boolean = false;
        ::uns::lua::type::number m_number = static_cast<::uns::lua::type::number>(0);
        ::uns::lua::type::integer m_integer = static_cast<::uns::lua::type::integer>(0);
        ::uns::lua::type::string m_string = ::uns::lua::type::string{};
        ::uns::lua::type::table m_table = ::uns::lua::type::table{};
        ::uns::lua::type::userdata m_userdata = nullptr;
        push_function_type m_push_function = push_nil;
    public:
        inline value() {};
        template<typename val_t>
            requires(::std::integral<val_t> && !::std::same_as<bool, val_t>)
        value(val_t obj) :
            m_type(::uns::lua::value_type::integer),
            m_integer(static_cast<::uns::lua::type::integer>(obj)),
            m_push_function(push_integer)                                                
        {};
        template<::std::floating_point val_t>
        value(val_t obj) :
            m_type(::uns::lua::value_type::number),
            m_number(static_cast<::uns::lua::type::number>(obj)),
            m_push_function(push_number)
        {};
        template<::std::same_as<bool> val_t>
        value(val_t obj) :
            m_type(::uns::lua::value_type::boolean),
            m_boolean(obj),
            m_push_function(push_boolean)
        {};
        value(const ::std::string obj);
        value(const char* obj);
        value(const ::std::string_view obj);
        value(const ::std::u32string obj);
        value(const char32_t* obj);
        value(const ::uns::lua::type::table& obj);
        value(::uns::lua::type::userdata obj);
        value(const ::uns::lua::type::nil& obj);
        value(const ::uns::lua::value& obj);
        ::uns::lua::value& operator=(const ::uns::lua::value& obj);
        value(::uns::lua::value&& obj);
        ::uns::lua::value& operator=(::uns::lua::value&& obj);
        ~value();
    public:
        template<typename val_t>
        ::uns::lua::value& operator=(const val_t& obj) {
            return (*this = ::uns::lua::value{ obj });
        };
    public:
        bool operator==(const ::uns::lua::value& obj) const;
        bool operator!=(const ::uns::lua::value& obj) const;
    public:
#define UNS_LUA_VALUE_CONVERT_DECLARATOR(type_identifier)                               \
        operator const ::uns::lua::type::##type_identifier&() const;                    \
        operator ::uns::lua::type::##type_identifier&();                                \

        UNS_LUA_VALUE_CONVERT_DECLARATOR(boolean);
        UNS_LUA_VALUE_CONVERT_DECLARATOR(number);
        UNS_LUA_VALUE_CONVERT_DECLARATOR(integer);
        UNS_LUA_VALUE_CONVERT_DECLARATOR(string);
        UNS_LUA_VALUE_CONVERT_DECLARATOR(table);
    public:
        operator const ::uns::lua::type::userdata() const;
        operator ::uns::lua::type::userdata& ();
#undef UNS_LUA_VALUE_CONVERT_DECLARATOR
    public:
        inline ::uns::lua::value_type type() const { return m_type; };
    public:
        void push_to(::uns::lua::thread& thread) const;
        void push_to(::uns::lua::auxiliary::state& thread) const;
        void push_to(::uns::lua::alias::lua_state stack) const;
    public:
        ::std::u32string to_string() const;
    public:
        static ::uns::lua::value make_from(::uns::lua::auxiliary::state& thread, int idx);
        static ::uns::lua::value make_from(::uns::lua::alias::lua_state stack, int idx);
    private:
        static void push_nil(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value);
#define UNS_LUA_VALUE_PUSH_DECLARATOR(type_identifier)                                            \
        static void push_##type_identifier(::uns::lua::alias::lua_state stack, const ::uns::lua::value& value);                                                                                        \

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
        private:
            ::std::unordered_map<::uns::lua::type::number, ::uns::lua::value> m_key_number;
            ::std::unordered_map<::uns::lua::type::integer, ::uns::lua::value> m_key_integer;
            ::std::unordered_map<::uns::lua::type::boolean, ::uns::lua::value> m_key_boolean;
            ::std::unordered_map<::uns::lua::type::string, ::uns::lua::value> m_key_string;
        public:
            inline table() {};
            table(const ::uns::lua::auxiliary::table& obj);
            ::uns::lua::auxiliary::table& operator=(const ::uns::lua::auxiliary::table& obj);
            table(::uns::lua::auxiliary::table&& obj);
            ::uns::lua::auxiliary::table& operator=(::uns::lua::auxiliary::table&& obj);
            ~table();
        public:
            ::std::u32string to_string() const;
        public:
            bool operator==(const ::uns::lua::auxiliary::table&) const;
            bool operator!=(const ::uns::lua::auxiliary::table&) const;
        public:
#define UNS_LUA_TABLE_IDX_DECLARATOR(type_identifier)                                            \
            ::uns::lua::value operator[] (const ::uns::lua::type::##type_identifier& key) const;\
            ::uns::lua::value& operator[] (const ::uns::lua::type::##type_identifier& key);\

            UNS_LUA_TABLE_IDX_DECLARATOR(number);
            UNS_LUA_TABLE_IDX_DECLARATOR(integer);
            UNS_LUA_TABLE_IDX_DECLARATOR(boolean);
            UNS_LUA_TABLE_IDX_DECLARATOR(string);
#undef UNS_LUA_TABLE_IDX_DECLARATOR
        public:
            ::uns::lua::value operator[] (const ::uns::lua::value& key) const;
            ::uns::lua::value& operator[] (const ::uns::lua::value& key);
        public:
            ::std::size_t size() const;
        public:
            template<class key_t>
            ::uns::lua::type::table::const_iterator<key_t> cbegin() const { return ::uns::lua::type::table::const_iterator<key_t>{}; };
            template<class key_t>
            ::uns::lua::type::table::const_iterator<key_t> cend() const { return ::uns::lua::type::table::const_iterator<key_t>{}; };
            template<class key_t>
            ::uns::lua::type::table::iterator<key_t> begin() { return ::uns::lua::type::table::iterator<key_t>{}; };
            template<class key_t>
            ::uns::lua::type::table::iterator<key_t> end() { return ::uns::lua::type::table::iterator<key_t>{}; };
        public:
#define UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(type_identifier)                                \
            template<>                                                                            \
            ::uns::lua::type::table::const_iterator<::uns::lua::type::##type_identifier> cbegin<::uns::lua::type::##type_identifier>() const { return m_key_##type_identifier.cbegin(); };\
            template<>                                                                            \
            ::uns::lua::type::table::const_iterator<::uns::lua::type::##type_identifier> cend<::uns::lua::type::##type_identifier>() const { return m_key_##type_identifier.cend(); };\
            template<>                                                                            \
            ::uns::lua::type::table::iterator<::uns::lua::type::##type_identifier> begin<::uns::lua::type::##type_identifier>() { return m_key_##type_identifier.begin(); };\
            template<>                                                                            \
            ::uns::lua::type::table::iterator<::uns::lua::type::##type_identifier> end<::uns::lua::type::##type_identifier>() { return m_key_##type_identifier.end(); };


            UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(number);
            UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(integer);
            UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(boolean);
            UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR(string);

#undef UNS_LUA_TABLE_ITERATION_METHODS_DECLARATOR
        };
    };


    //definitions of ::uns::type::table methods =>
    template<::std::floating_point key_t>
    ::uns::lua::value uns::lua::type::table::operator[] (const key_t& key) const { return m_ptr->operator[](static_cast<::uns::lua::type::number>(key)); };
    template<::std::floating_point key_t>
    ::uns::lua::value& ::uns::lua::type::table::operator[] (const key_t& key) { return m_ptr->operator[](static_cast<::uns::lua::type::number>(key)); };
    template<typename key_t>
        requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
    ::uns::lua::value uns::lua::type::table::operator[] (const key_t& key) const { return m_ptr->operator[](static_cast<::uns::lua::type::integer>(key)); };
    template<typename key_t>
        requires(::std::integral<key_t> && !::std::same_as<::uns::lua::type::boolean, key_t>)
    ::uns::lua::value& ::uns::lua::type::table::operator[] (const key_t& key) { return m_ptr->operator[](static_cast<::uns::lua::type::integer>(key)); };
    template<::std::same_as<::uns::lua::type::boolean> key_t>
    ::uns::lua::value uns::lua::type::table::operator[] (const key_t& key) const { return m_ptr->operator[](static_cast<::uns::lua::type::boolean>(key)); };
    template<::std::same_as<::uns::lua::type::boolean> key_t>
    ::uns::lua::value& ::uns::lua::type::table::operator[] (const key_t& key) { return m_ptr->operator[](static_cast<::uns::lua::type::boolean>(key)); };

    template<class key_t>
    ::uns::lua::type::table::const_iterator<key_t> uns::lua::type::table::cbegin() const { return m_ptr->cbegin<key_t>(); };
    template<class key_t>
    ::uns::lua::type::table::const_iterator<key_t> uns::lua::type::table::cend() const { return m_ptr->cend<key_t>(); };
    template<class key_t>
    ::uns::lua::type::table::iterator<key_t> uns::lua::type::table::begin() { return m_ptr->begin<key_t>(); };
    template<class key_t>
    ::uns::lua::type::table::iterator<key_t> uns::lua::type::table::end() { return m_ptr->end<key_t>(); };
    //<= definitions of ::uns::type::table methods 

    class thread;
    class script;

    class function {
        friend ::uns::lua::script;
        friend ::uns::lua::thread;
    public:
        class results {
            friend ::uns::lua::function;
        private:
            ::uns::lua::auxiliary::state_wrapper m_stack_wrapper;
            ::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
            long int m_function_idx = 0;
        public:
            inline bool valid() const { return m_stack != nullptr || m_stack_wrapper.valid(); };

            ::std::vector<::uns::lua::value> get_all() const;
            template<::std::size_t result_values_number>
            ::std::array<::uns::lua::value, result_values_number> get() const {
                return {};
            };
            template<>
            ::std::array<::uns::lua::value, 1> get<1>() const {
                return { get() };
            };
            template<>
            ::std::array<::uns::lua::value, 2> get<2>() const {
                return get_2();
            };
            template<>
            ::std::array<::uns::lua::value, 3> get<3>() const {
                return get_3();
            };
            template<>
            ::std::array<::uns::lua::value, 4> get<4>() const {
                return get_4();
            };
            template<>
            ::std::array<::uns::lua::value, 5> get<5>() const {
                return get_5();
            };

            ::uns::lua::value get() const;
        private:
            ::std::array<::uns::lua::value, 2> get_2() const;
            ::std::array<::uns::lua::value, 3> get_3() const;
            ::std::array<::uns::lua::value, 4> get_4() const;
            ::std::array<::uns::lua::value, 5> get_5() const;
        };
    private:
        ::uns::lua::auxiliary::state_wrapper m_stack_wrapper;
        ::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
        ::std::string m_function_name = "";
        mutable ::uns::lua::error m_err;
    public:
        inline function() {};
    private:
        function(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_function_name);
        function(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_function_name);
    public:
        function(const ::uns::lua::function&) = default;
        ::uns::lua::function& operator=(const ::uns::lua::function&) = default;
        function(::uns::lua::function&& obj) = default;
        ::uns::lua::function& operator=(::uns::lua::function&& obj) = default;
        ~function();
    public:
        inline const ::uns::lua::error& error() const { return m_err; };
        inline ::uns::lua::error& error() { return m_err; };
    public:
        ::std::u32string name() const;
    public:
        bool valid() const;
    public:
        ::uns::lua::function::results operator() (const ::std::size_t expected_results, const ::std::vector<::uns::lua::value>& args);
        ::uns::lua::function::results operator() (const ::std::size_t expected_results);
        ::uns::lua::function::results operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1);
        ::uns::lua::function::results operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2);
        ::uns::lua::function::results operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3);
        ::uns::lua::function::results operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4);
        ::uns::lua::function::results operator() (const ::std::size_t expected_results, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4, const ::uns::lua::value& arg5);
    public:
        void gc();
    };


    class global {
        friend ::uns::lua::script;
        friend ::uns::lua::thread;
    private:
        ::uns::lua::auxiliary::state_wrapper m_stack_wrapper;
        ::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
        ::std::string m_global_name = "";
        mutable ::uns::lua::error m_err;
    public:
        inline global() {};
    private:
        global(const ::std::shared_ptr<::uns::lua::auxiliary::state>& lua_script, const ::std::string& lua_global_variable_name);
        global(::uns::lua::auxiliary::state_wrapper& lua_script, const ::std::string& lua_global_variable_name);
    public:
        global(const ::uns::lua::global&) = default;
        ::uns::lua::global& operator=(const ::uns::lua::global&) = default;
        global(::uns::lua::global&& obj) = default;
        ::uns::lua::global& operator=(::uns::lua::global&& obj) = default;
        ::uns::lua::global& operator=(const ::uns::lua::value& value);
        ~global();
    public:
        inline const ::uns::lua::error& error() const { return m_err; };
        inline ::uns::lua::error& error() { return m_err; };
    public:
        ::std::u32string name() const;
    public:
        inline bool valid() const { return m_stack != nullptr || m_stack_wrapper.valid(); };
    public:
        ::uns::lua::value get() const;
        void set(const ::uns::lua::value& value);
    public:
        void gc();
    };


    class lib_entry;
    class library;


    class thread {
        friend ::uns::lua::value;
        friend ::uns::lua::lib_entry;
    private:
        mutable ::uns::lua::auxiliary::state_wrapper m_stack;
        mutable ::uns::lua::error m_err;
    private:
        thread() = delete;
    public:
        thread(::uns::lua::alias::lua_state);
        thread(const ::uns::lua::thread&);
        ::uns::lua::thread& operator=(const ::uns::lua::thread&);
        thread(::uns::lua::thread&&);
        ::uns::lua::thread& operator=(::uns::lua::thread&&);
        ~thread() = default;
    public:
        inline bool valid() const { return m_stack.valid(); };
    public:
        inline const ::uns::lua::error& error() const { return m_err; };
        inline ::uns::lua::error& error() { return m_err; };
    public:
        void load(const ::uns::lua::library& library);
        void load(const ::std::u32string& text);
        void load(const ::std::filesystem::path& file);
    public:
        void call(int results_expected_total);
    public:
        ::uns::lua::function get_function(const ::std::u32string& lua_global_function_name);
        ::uns::lua::global get_global(const ::std::u32string& lua_global_variable_name);
    public:
        ::std::size_t size() const;
        ::uns::lua::value get_value(int input_index);
    public:
        int raise_error(const ::std::string& errtext);
        int raise_error(const ::std::u32string& errtext);
    public:
        void gc();
    };


    namespace auxiliary {

        namespace wrappers::returning_all {

            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try{
                    auto args = ::std::vector<::uns::lua::value>{};
                    for(::std::size_t arg_idx = 1; arg_idx < l_thread.size(); ++arg_idx) {
                        args.push_back(l_thread.get_value(arg_idx));
                    };

                    auto results = wrapped(l_thread, args);

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = wrapped(l_thread);

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::vector<::uns::lua::value>{};
                    if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1));
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::vector<::uns::lua::value>{};
                    if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2));
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::vector<::uns::lua::value>{};
                    if(l_thread.size() >= 3) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3));
                    }
                    else if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::vector<::uns::lua::value>{};
                    if(l_thread.size() >= 4) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4));
                    }
                    else if(l_thread.size() >= 3) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::vector<::uns::lua::value>{};
                    if(l_thread.size() >= 5) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), l_thread.get_value(5));
                    }
                    else if(l_thread.size() >= 4) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 3) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };

        };

        namespace wrappers::returning_some {

            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto args = ::std::vector<::uns::lua::value>{};
                    for(::std::size_t arg_idx = 1; arg_idx < l_thread.size(); ++arg_idx) {
                        args.push_back(l_thread.get_value(arg_idx));
                    };

                    auto results = wrapped(l_thread, args);

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = wrapped(l_thread);
                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::array<::uns::lua::value, result_values_number>{};
                    if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1));
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::array<::uns::lua::value, result_values_number>{};
                    if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2));
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::array<::uns::lua::value, result_values_number>{};
                    if(l_thread.size() >= 3) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3));
                    }
                    else if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::array<::uns::lua::value, result_values_number>{};
                    if(l_thread.size() >= 4) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4));
                    }
                    else if(l_thread.size() >= 3) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto results = ::std::array<::uns::lua::value, result_values_number>{};
                    if(l_thread.size() >= 5) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), l_thread.get_value(5));
                    }
                    else if(l_thread.size() >= 4) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 3) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        results = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        results = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        results = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    for(const auto& result : results) {
                        result.push_to(stack);
                    };

                    return static_cast<int>(results.size());
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };

        };

        namespace wrappers::returning_one {

            template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto args = ::std::vector<::uns::lua::value>{};
                    for(::std::size_t arg_idx = 1; arg_idx < l_thread.size(); ++arg_idx) {
                        args.push_back(l_thread.get_value(arg_idx));
                    };

                    auto result = wrapped(l_thread, args);
                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::uns::lua::value(*wrapped)(::uns::lua::thread&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto result = wrapped(l_thread);
                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto result = ::uns::lua::value{};
                    if(l_thread.size() >= 1) {
                        result = wrapped(l_thread, l_thread.get_value(1));
                    }
                    else {
                        result = wrapped(l_thread, ::uns::lua::nil);
                    };

                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto result = ::uns::lua::value{};
                    if(l_thread.size() >= 2) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2));
                    }
                    else if(l_thread.size() >= 1) {
                        result = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil);
                    }
                    else {
                        result = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto result = ::uns::lua::value{};
                    if(l_thread.size() >= 3) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3));
                    }
                    else if(l_thread.size() >= 2) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        result = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        result = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto result = ::uns::lua::value{};
                    if(l_thread.size() >= 4) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4));
                    }
                    else if(l_thread.size() >= 3) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        result = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        result = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto result = ::uns::lua::value{};
                    if(l_thread.size() >= 5) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), l_thread.get_value(5));
                    }
                    else if(l_thread.size() >= 4) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 3) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        result = wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        result = wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        result = wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    result.push_to(stack);

                    return 1;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };

        };

        namespace wrappers::returning_none {

            template<void(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    auto args = ::std::vector<::uns::lua::value>{};
                    for(::std::size_t arg_idx = 1; arg_idx < l_thread.size(); ++arg_idx) {
                        args.push_back(l_thread.get_value(arg_idx));
                    };

                    wrapped(l_thread, args);

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<void(*wrapped)(::uns::lua::thread&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    wrapped(l_thread);

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    if(l_thread.size() >= 1) {
                        wrapped(l_thread, l_thread.get_value(1));
                    }
                    else {
                        wrapped(l_thread, ::uns::lua::nil);
                    };

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    if(l_thread.size() >= 2) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2));
                    }
                    else if(l_thread.size() >= 1) {
                        wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil);
                    }
                    else {
                        wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    if(l_thread.size() >= 3) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3));
                    }
                    else if(l_thread.size() >= 2) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    if(l_thread.size() >= 4) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4));
                    }
                    else if(l_thread.size() >= 3) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };
            template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
            int wrap(::uns::lua::alias::lua_state stack) {
                auto l_thread = ::uns::lua::thread{ stack };

                try {
                    if(l_thread.size() >= 5) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), l_thread.get_value(5));
                    }
                    else if(l_thread.size() >= 4) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), l_thread.get_value(4), ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 3) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), l_thread.get_value(3), ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 2) {
                        wrapped(l_thread, l_thread.get_value(1), l_thread.get_value(2), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else if(l_thread.size() >= 1) {
                        wrapped(l_thread, l_thread.get_value(1), ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    }
                    else {
                        wrapped(l_thread, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil, ::uns::lua::nil);
                    };

                    return 0;
                }
                catch(const ::std::exception& e) {
                    return l_thread.raise_error(e.what());
                }
                catch(...) {
                    return l_thread.raise_error("Unknown exception");
                };
            };

        };

    };


    class lib_entry {
    private:
        ::std::string m_name;
        ::uns::lua::alias::lua_cfunction m_function = nullptr;
    public:
        inline lib_entry() {};
        lib_entry(const ::std::u32string& name, ::uns::lua::alias::lua_cfunction lua_function);
        lib_entry(const ::uns::lua::lib_entry&) = default;
        ::uns::lua::lib_entry& operator=(const ::uns::lua::lib_entry&) = default;
        lib_entry(::uns::lua::lib_entry&&) = default;
        ::uns::lua::lib_entry& operator=(::uns::lua::lib_entry&&) = default;
        ~lib_entry() = default;

        inline const char* name() const { return m_name.c_str(); };
        inline ::uns::lua::alias::lua_cfunction func() const { return m_function; };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&, const ::std::vector<::uns::lua::value>&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };

        template<::std::vector<::uns::lua::value>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = name;
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_all::wrap<wrapped>;

            return res;
        };
        template<::std::size_t result_values_number, ::std::array<::uns::lua::value, result_values_number>(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_some::wrap<result_values_number, wrapped>;

            return res;
        };
        template<::uns::lua::value(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_one::wrap<wrapped>;

            return res;
        };
        template<void(*wrapped)(::uns::lua::thread&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&, const ::uns::lua::value&)>
        static ::uns::lua::lib_entry make(const ::std::u32string& name) {
            auto res = ::uns::lua::lib_entry{};

            res.m_name = ::uns::string::cast<::std::string>(name);
            res.m_function = ::uns::lua::auxiliary::wrappers::returning_none::wrap<wrapped>;

            return res;
        };
    };


    class library {
    public:
        ::std::u32string module_name = U"";
        ::std::vector<::uns::lua::lib_entry> api;
        ::std::u32string text = U"";
    };


    class script {
    private:
        ::std::shared_ptr<::uns::lua::auxiliary::state> m_stack = nullptr;
        mutable ::uns::lua::error m_err;
    public:
        script();
        script(const ::std::u32string& text);
        script(const ::std::filesystem::path& file);
        script(const ::uns::lua::library& library);
        script(const ::uns::lua::script& obj);
        ::uns::lua::script& operator=(const ::uns::lua::script& obj);
        script(::uns::lua::script&& obj);
        ::uns::lua::script& operator=(::uns::lua::script&& obj);
        ~script();
    public:
        void load(const ::uns::lua::library& library);
        void load(const ::std::u32string& text);
        void load(const ::std::filesystem::path& file);
    public:
        bool valid() const { return m_stack != nullptr; };
    public:
        inline const ::uns::lua::error& error() const { return m_err; };
        inline ::uns::lua::error& error() { return m_err; };
    public:
        ::uns::lua::function get_function(const ::std::u32string& lua_global_function_name);
        ::uns::lua::global get_global(const ::std::u32string& lua_global_variable_name);
    public:
        void run();
    public:
        void gc();
        long double total_memory() const;
    };


    ::uns::lua::value make_table();


    namespace dll {

        template<::uns::lua::library(*lualib_returning_function)()>
        int libexport(::uns::lua::alias::lua_state L) {
            auto thread = ::uns::lua::thread{ L };

            auto lualib = lualib_returning_function();

            thread.load(lualib);
            thread.call(0);

            return 0;
        };

    };
};

#endif