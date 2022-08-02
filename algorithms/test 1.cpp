

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#pragma warning(disable: 4514)
#include "algorithms.h"

namespace abyss {

    UNS_ENUM_DECLARATOR(
        layers, int,
        l1 = 1,
        l2 = 2,
        l3 = 7
    );

};
#pragma warning(default: 4514)

int main() {
    std::cout << "START" << "\n";
    
    abyss::layers l = abyss::layers::l1;

    std::cout << std::boolalpha << (7 == abyss::layers::l3) << "\n";

    std::cout << "FINISH" << "\n";
};