
#include <iostream>

#include "uniself/lua_wrapper.h"

int main() {
    ::std::cout << "START\n";

    auto err = ::uns::lua::error{ uns::lua::errcode::errmem, ::uns::lua::errtype::ok, u8"A problem occured. Ooops!" };

    ::std::cout << err.to_string() << "\n";

    ::std::cout << "FINISH\n";
};