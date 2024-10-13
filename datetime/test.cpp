#include <iostream>

#include "uniself/datetime.hpp"

int main() {
    ::std::cout << "START\n";

    auto now = ::uns::timestamp();
    
    auto now2 = static_cast<::uns::datetime>(now);

    auto now3 = static_cast<::std::chrono::system_clock::time_point>(now2);

    auto now4 = static_cast<::uns::datetime>(now3);

    ::std::cout << "FINISH\n";
};