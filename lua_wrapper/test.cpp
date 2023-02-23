
#include <iostream>

#include "uniself/lua_wrapper.h"

void print_vals(const ::uns::lua::value& val, const ::std::string shift = "") {
    switch(val.type()) {
        default:
        {
            ::std::cout << shift << "unknown" << "\n";
            break;
        }
        case ::uns::lua::value_type::nil:
        {
            ::std::cout << shift << "nil" << "\n";
            break;
        }
        case ::uns::lua::value_type::integer:
        {
            ::std::cout << shift << static_cast<::uns::lua::type::integer>(val) << "\n";
            break;
        }
        case ::uns::lua::value_type::number:
        {
            ::std::cout << shift << static_cast<::uns::lua::type::number>(val) << "\n";
            break;
        }
        case ::uns::lua::value_type::boolean:
        {
            ::std::cout << shift << ::std::boolalpha << static_cast<::uns::lua::type::boolean>(val) << "\n";
            break;
        }
        case ::uns::lua::value_type::string:
        {
            ::std::cout << shift << static_cast<::uns::lua::type::string>(val) << "\n";
            break;
        }
        case ::uns::lua::value_type::table:
        {
            print_vals(val, shift);
            break;
        }
    };
};
void print_vals(const ::std::vector<::uns::lua::value>& vals, const ::std::string shift = "") {
    for(const auto& val : vals) {
        print_vals(val, shift);
    };

    if(vals.size() == 0) {
        ::std::cout << "no args\n";
    };
};
void print_vals(const ::uns::lua::type::table& vals, const ::std::string shift = "") {
    ::std::cout << shift << "{\n";

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(vals).cbegin<::uns::lua::type::number>();
        iterator != static_cast<const ::uns::lua::type::table&>(vals).cend<::uns::lua::type::number>();
        ++iterator
    ) {
        print_vals(iterator->second, shift + "\t");
    };

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(vals).cbegin<::uns::lua::type::integer>();
        iterator != static_cast<const ::uns::lua::type::table&>(vals).cend<::uns::lua::type::integer>();
        ++iterator
    ) {
        print_vals(iterator->second, shift + "\t");
    };

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(vals).cbegin<::uns::lua::type::boolean>();
        iterator != static_cast<const ::uns::lua::type::table&>(vals).cend<::uns::lua::type::boolean>();
        ++iterator
    ) {
        print_vals(iterator->second, shift + "\t");
    };

    for(
        auto iterator = static_cast<const ::uns::lua::type::table&>(vals).cbegin<::uns::lua::type::string>();
        iterator != static_cast<const ::uns::lua::type::table&>(vals).cend<::uns::lua::type::string>();
        ++iterator
    ) {
        print_vals(iterator->second, shift + "\t");
    };

    ::std::cout << shift << "}\n";
};

int main() {
    ::std::cout << "START\n";

    auto script = ::uns::lua::script{
        ::std::u8string{
            u8R"^^(
                function print_args(...)
                    print("start:")
                    print(...)
                    print(":finish")
                    return "Hello from the other side", ...
                end
            )^^"
        }
    };

    script.run();
    auto print_args = script.get_function(u8"print_args");

    ::std::cout << print_args.error().to_string() << "\n";

    if(!print_args.error().is()) {
        auto arg1 = ::uns::lua::value{ 14023 };
        auto arg2 = ::uns::lua::value{ 0.0045006 };
        auto arg3 = ::uns::lua::value{ true };
        auto arg4 = ::uns::lua::value{ "It's working!" };
        auto results = print_args(3, { arg1, arg2, arg3, arg4 });

        print_vals(results);
    };
    ::std::cout << print_args.error().to_string() << "\n";

    ::std::cout << "FINISH\n";
};