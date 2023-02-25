
#include <iostream>

#include "uniself/lua_wrapper.h"

void print_vals(const ::uns::lua::value&, const ::std::string);
void print_vals(const ::uns::lua::type::table&, const ::std::string);
void print_vals(const ::std::vector<::uns::lua::value>&, const ::std::string);

void print_vals(const ::uns::lua::type::table& vals, const ::std::string shift = "") {
    ::std::cout << shift << "{\n";

    for(
        auto iterator = vals.cbegin<::uns::lua::type::number>();
        iterator != vals.cend<::uns::lua::type::number>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << iterator->first << " -- ";
        print_vals(iterator->second, shift);
        ::std::cout << "\n";
    };

    for(
        auto iterator = vals.cbegin<::uns::lua::type::integer>();
        iterator != vals.cend<::uns::lua::type::integer>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << iterator->first << " -- ";
        print_vals(iterator->second, shift);
        ::std::cout << "\n";
    };

    for(
        auto iterator = vals.cbegin<::uns::lua::type::boolean>();
        iterator != vals.cend<::uns::lua::type::boolean>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << ::std::boolalpha << iterator->first << " -- ";
        print_vals(iterator->second, shift);
        ::std::cout << "\n";
    };

    for(
        auto iterator = vals.cbegin<::uns::lua::type::string>();
        iterator != vals.cend<::uns::lua::type::string>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << iterator->first << " -- ";
        print_vals(iterator->second, shift);
        ::std::cout << "\n";
    };

    ::std::cout << shift << "}\n";
};
void print_vals(const ::uns::lua::value& val, const ::std::string shift = "") {
    switch(val.type()) {
        default:
        {
            ::std::cout << shift << "unknown";
            break;
        }
        case ::uns::lua::value_type::nil:
        {
            ::std::cout << shift << "nil";
            break;
        }
        case ::uns::lua::value_type::integer:
        {
            ::std::cout << shift << static_cast<::uns::lua::type::integer>(val);
            break;
        }
        case ::uns::lua::value_type::number:
        {
            ::std::cout << shift << static_cast<::uns::lua::type::number>(val);
            break;
        }
        case ::uns::lua::value_type::boolean:
        {
            ::std::cout << shift << ::std::boolalpha << static_cast<::uns::lua::type::boolean>(val);
            break;
        }
        case ::uns::lua::value_type::string:
        {
            ::std::cout << shift << static_cast<::uns::lua::type::string>(val);
            break;
        }
        case ::uns::lua::value_type::table:
        {
            print_vals(static_cast<::uns::lua::type::table>(val), shift);
            break;
        }
    };
};
void print_vals(const ::std::vector<::uns::lua::value>& vals, const ::std::string shift = "") {
    for(const auto& val : vals) {
        print_vals(val, shift);
        ::std::cout << "\n";
    };

    if(vals.size() == 0) {
        ::std::cout << "no args\n";
    };
};

int test_print(lua_State* L) {
    int total = lua_gettop(L);

    for(int i = -total; i < 0; ++i) {
        switch(lua_type(L, i)) {
            default:
            {
                ::std::cout << "unknown\n";
                break;
            }
            case LUA_TNIL:
            {
                ::std::cout << "nil\n";
                break;
            }
            case LUA_TNUMBER:
            {
                ::std::cout << lua_tonumber(L, i) << "\n";
                break;
            }
            case LUA_TBOOLEAN:
            {
                ::std::cout << ::std::boolalpha << static_cast<bool>(lua_toboolean(L, i)) << "\n";
                break;
            }
            case LUA_TSTRING:
            {
                ::std::cout << lua_tostring(L, i) << "\n";
                break;
            }
        };
    };

    return 0;
};


void function_n_value_test() {
    auto script = ::uns::lua::script{
        ::std::u8string{
            u8R"^^(
                function print_args(...)
                    return ...
                end
            )^^"
        }
    };

    script.run();
    auto print_args = script.get_function(u8"print_args");

    ::std::cout << print_args.error().to_string() << "\n";

    if(!print_args.error().is()) {
        auto arg1 = ::uns::lua::make_table();
        auto arg2 = ::uns::lua::value{ 0.0045006 };
        auto arg3 = ::uns::lua::value{ true };
        auto arg4 = ::uns::lua::value{ u8"It's working!" };

        static_cast<::uns::lua::type::table&>(arg1)[arg2] = arg3;
        static_cast<::uns::lua::type::table&>(arg1)[::uns::lua::value{ "What?" }] = arg4;
        static_cast<::uns::lua::type::table&>(arg1)[u8"Some subtable"] = ::uns::lua::make_table();
        static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(arg1)["Some subtable"])[123] = "Yeah baby!";

        auto results = print_args(3, { arg1 });

        print_vals(results);
    };
    ::std::cout << print_args.error().to_string() << "\n";
};

void global1_test() {
    auto script = ::uns::lua::script{
        ::std::u8string{
            u8R"^^(
            )^^"
    }
    };

    script.run();
    auto global1 = script.get_global(u8"global1");

    ::std::cout << global1.error().to_string() << "\n";

    if(!global1.error().is()) {
        auto arg1 = ::uns::lua::make_table();
        auto arg2 = ::uns::lua::value{ 0.0045006 };
        auto arg3 = ::uns::lua::value{ true };
        auto arg4 = ::uns::lua::value{ u8"It's working!" };

        static_cast<::uns::lua::type::table&>(arg1)[arg2] = arg3;
        static_cast<::uns::lua::type::table&>(arg1)[::uns::lua::value{ "What?" }] = arg4;
        static_cast<::uns::lua::type::table&>(arg1)[u8"Some subtable"] = ::uns::lua::make_table();
        static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(arg1)["Some subtable"])[123] = "Yeah baby!";

        global1.set(arg1);

        auto result = global1.get();

        print_vals(result);
    };
    ::std::cout << global1.error().to_string() << "\n";
};

void global2_test() {
    auto script = ::uns::lua::script{
        ::std::u8string{
            u8R"^^(
                global2 = {}

                global2["field1"] = "value1"
                global2["field2"] = -404
                global2.field3 = true
            )^^"
        }
    };
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.run();
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    auto global2 = script.get_global(u8"global2");

    ::std::cout << global2.error().to_string() << "\n";

    if(!global2.error().is()) {

        auto result = global2.get();

        print_vals(result);
    };
    ::std::cout << global2.error().to_string() << "\n";
};

void lib1_test() {
    auto script = ::uns::lua::script{
        ::uns::lua::library{ u8"test1", { { u8"func1", test_print } }, u8"" }
    };
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.load(
        ::std::u8string{
            u8R"^^(
                function print_args(...)
                    test1.func1(...)
                end
            )^^"
        }
    );
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.run();
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    auto print_args = script.get_function(u8"print_args");

    if(!print_args.error().is()) {
        auto arg1 = ::uns::lua::make_table();
        auto arg2 = ::uns::lua::value{ 0.0045006 };
        auto arg3 = ::uns::lua::value{ true };
        auto arg4 = ::uns::lua::value{ u8"It's working!" };

        static_cast<::uns::lua::type::table&>(arg1)[arg2] = arg3;
        static_cast<::uns::lua::type::table&>(arg1)[::uns::lua::value{ "What?" }] = arg4;
        static_cast<::uns::lua::type::table&>(arg1)[u8"Some subtable"] = ::uns::lua::make_table();
        static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(arg1)["Some subtable"])[123] = "Yeah baby!";

        auto results = print_args(4, { arg1, arg2, arg3, arg4 });

        if(print_args.error().is()) {
            ::std::cout << print_args.error().to_string() << "\n";
            return;
        };

        print_vals(results);
    }
    else {
        ::std::cout << print_args.error().to_string() << "\n";
    };
};

void lib2_test() {
    auto script = ::uns::lua::script{
        ::uns::lua::library{ u8"", { { u8"func1", test_print } }, u8"" }
    };
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.load(
        ::std::u8string{
            u8R"^^(
                function print_args(...)
                    func1(...)
                end
            )^^"
        }
    );
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.run();
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    auto print_args = script.get_function(u8"print_args");

    if(!print_args.error().is()) {
        auto arg1 = ::uns::lua::make_table();
        auto arg2 = ::uns::lua::value{ 0.0045006 };
        auto arg3 = ::uns::lua::value{ true };
        auto arg4 = ::uns::lua::value{ u8"It's working!" };

        static_cast<::uns::lua::type::table&>(arg1)[arg2] = arg3;
        static_cast<::uns::lua::type::table&>(arg1)[::uns::lua::value{ "What?" }] = arg4;
        static_cast<::uns::lua::type::table&>(arg1)[u8"Some subtable"] = ::uns::lua::make_table();
        static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(arg1)["Some subtable"])[123] = "Yeah baby!";

        auto results = print_args(4, { arg1, arg2, arg3, arg4 });

        if(print_args.error().is()) {
            ::std::cout << print_args.error().to_string() << "\n";
            return;
        };

        print_vals(results);
    }
    else {
        ::std::cout << print_args.error().to_string() << "\n";
    };
};

int main() {
    ::std::cout << "START\n";

    lib2_test();

    ::std::cout << "FINISH\n";
};