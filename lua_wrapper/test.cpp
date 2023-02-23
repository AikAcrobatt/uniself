
#include <iostream>

#include "uniself/lua_wrapper.h"

int main() {
    ::std::cout << "START\n";

    auto script = ::uns::lua::script{
        ::std::u8string{
            u8R"^^(
                function print_args(...)
                    print("start:")
                    print(...)
                    print(":finish")
                end
            )^^"
        }
    };

    script.run();
    auto print_args = script.get_function(u8"print_args");

    ::std::cout << print_args.error().to_string() << "\n";

    if(!print_args.error().is()) {
        auto arg1 = ::uns::lua::value{ ::uns::lua::type::integer{ 14023 } };
        auto arg2 = ::uns::lua::value{ ::uns::lua::type::number{ 0.0045006 } };
        auto arg3 = ::uns::lua::value{ ::uns::lua::type::boolean{ true } };
        auto arg4 = ::uns::lua::value{ ::uns::lua::type::string{ "It's working!" }};
        print_args(0, { arg1, arg2, arg3, arg4 });
    };
    ::std::cout << print_args.error().to_string() << "\n";

    ::std::cout << "FINISH\n";
};