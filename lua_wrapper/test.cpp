
#include <iostream>

#include "uniself/lua_wrapper.hpp"

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
        print_vals(iterator->second, shift + "\t");
        ::std::cout << "\n";
    };

    for(
        auto iterator = vals.cbegin<::uns::lua::type::integer>();
        iterator != vals.cend<::uns::lua::type::integer>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << iterator->first << " -- ";
        print_vals(iterator->second, shift + "\t");
        ::std::cout << "\n";
    };

    for(
        auto iterator = vals.cbegin<::uns::lua::type::boolean>();
        iterator != vals.cend<::uns::lua::type::boolean>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << ::std::boolalpha << iterator->first << " -- ";
        print_vals(iterator->second, shift + "\t");
        ::std::cout << "\n";
    };

    for(
        auto iterator = vals.cbegin<::uns::lua::type::string>();
        iterator != vals.cend<::uns::lua::type::string>();
        ++iterator
    ) {
        ::std::cout << shift + "\t" << iterator->first << " -- ";
        print_vals(iterator->second, shift + "\t");
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


void test_print(::uns::lua::thread& th) noexcept {
    auto values_on_stack_total = th.size();
    for(::std::size_t i = 1; i <= values_on_stack_total; ++i) {

        print_vals(th.get_value(i));
        ::std::cout << ::std::endl;
    };
};


::std::array<::uns::lua::value, 5> lib_func_5_5(::uns::lua::thread&, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4, const ::uns::lua::value& arg5) {
    return { arg5, arg4, arg1, arg3, arg2 };
};

void lib_func_0_0(::uns::lua::thread&) {
    throw ::std::exception{ "Expected exception" };
    print_vals("!!lib_func_0_0!!");
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

        auto results = print_args(4, arg1, arg2, arg3, arg4);

        print_vals(results.get_all());
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
        ::uns::lua::library{ u8"test1", { ::uns::lua::lib_entry::make<test_print>(::std::u8string{ u8"func1" }) }, u8"" }
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

                    return "!!!", { true, 17, ... }
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

    if(print_args.valid()) {
        auto arg1 = ::uns::lua::make_table();
        auto arg2 = ::uns::lua::value{ 0.0045006 };
        auto arg3 = ::uns::lua::value{ true };
        auto arg4 = ::uns::lua::value{ u8"It's working!" };

        static_cast<::uns::lua::type::table&>(arg1)[arg2] = arg3;
        static_cast<::uns::lua::type::table&>(arg1)[::uns::lua::value{ "What?" }] = arg4;
        static_cast<::uns::lua::type::table&>(arg1)[u8"Some subtable"] = ::uns::lua::make_table();
        static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(arg1)["Some subtable"])[123] = "Yeah baby!";

        auto results = print_args(2, arg1, arg2, arg3, arg4);

        if(print_args.error().is()) {
            ::std::cout << print_args.error().to_string() << "\n";
            return;
        };

        print_vals(results.get_all());
    }
    else {
        ::std::cout << print_args.error().to_string() << " !!IS NOT VALID!!" << "\n";
    };
};

void lib2_test() {
    auto script = ::uns::lua::script{
        ::uns::lua::library{ u8"", { ::uns::lua::lib_entry::make<test_print>(::std::u8string{ u8"func1" }) }, u8"" }
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

        auto results = print_args(2, arg1, arg2, arg3, arg4);

        if(print_args.error().is()) {
            ::std::cout << print_args.error().to_string() << "\n";
            return;
        };

        print_vals(results.get_all());
    }
    else {
        ::std::cout << print_args.error().to_string() << "\n";
    };
};

void lib3_test() {
    auto script = ::uns::lua::script{
        ::uns::lua::library{ u8"", { ::uns::lua::lib_entry::make<5, lib_func_5_5>(::std::u8string{ u8"lib_func" }) }, u8"" }
    };
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.load(
        ::std::u8string{
            u8R"^^(
                print(lib_func(1, nil, 3))
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

};

void script_loading_test() {
    auto script = ::uns::lua::script{};
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.load(::std::filesystem::path{ L"G:/Visual_Studio/uniself/lua_wrapper/script_loading_test.lua" });
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.run();
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    ::std::cout << "total memory used = " << script.total_memory() << "\n";

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

        auto results = print_args(5, arg1, arg2, arg3, arg4);

        auto [res1, res2, res3, res4, res5] = results.get<5>();
        
        print_vals({ res1, res2, res3, res4, res5 });
    };
    ::std::cout << print_args.error().to_string() << "\n";
};

void error_rising() {
    auto script = ::uns::lua::script{
        ::uns::lua::library{ u8"module", { ::uns::lua::lib_entry::make<lib_func_0_0>(::std::u8string{ u8"lib_func" }) }, u8"" }
    };
    if(script.error().is()) {
        ::std::cout << script.error().to_string() << "\n";
        return;
    };

    script.load(
        ::std::u8string{
            u8R"^^(
                print("LUA:START")
                module = require "module_api"
                print(pcall(module.lib_func, 1, nil, 3))
                print("LUA:FINISH")
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

};

void invalid_handler() {
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
    auto print_args = script.get_function(u8"print_args2");

    ::std::cout << "print_args.error()='" << print_args.error().to_string() << "'\n";

    ::std::cout << ::std::boolalpha << "print_args.valid()=" << print_args.valid() << "\n";
};

void invalid_function_call() {
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
    auto print_args = script.get_function(u8"print_args2");

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

        auto results = print_args(4, arg1, arg2, arg3, arg4);

        print_vals(results.get_all());
    };
    ::std::cout << print_args.error().to_string() << "\n";
};

int main() {
    ::std::cout << "START\n";

    invalid_function_call();

    ::std::cout << "FINISH\n";
};