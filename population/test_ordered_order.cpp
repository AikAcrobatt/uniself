
#include <iostream>
#include <vector>

#include "uniself/population.h"

class unit : public ::uns::population::unit<float, float> {
protected:
    unsigned int name = 0;
public:
    unit(int n) : name(n) {};

    int show() const { return name; };
};

bool comp(unit& a, unit& b) { return (b.show() < a.show()); };

using order_element = ::uns::population::order_element<unit, ::uns::population::ordered_order<unit, comp>>;


int main() {
    ::std::cout << "START" << "\n";

    ::std::random_device rand;

    typename order_element::order_type pop(rand());

    pop.push(::std::shared_ptr<unit>(new unit(1)));
    pop.push(::std::shared_ptr<unit>(new unit(9)));
    pop.push(::std::shared_ptr<unit>(new unit(3)));
    pop.push(::std::shared_ptr<unit>(new unit(5)));
    pop.push(::std::shared_ptr<unit>(new unit(8)));
    pop.push(::std::shared_ptr<unit>(new unit(2)));
    pop.push(::std::shared_ptr<unit>(new unit(7)));
    pop.push(::std::shared_ptr<unit>(new unit(6)));
    pop.push(::std::shared_ptr<unit>(new unit(0)));
    pop.push(::std::shared_ptr<unit>(new unit(4)));

    for(auto p : pop) {
        ::std::cout << p.index() << " " << p.unit().show() << "\n";
    };
    ::std::cout << "-------------------------\n";

    pop.sort();

    for(auto p : pop) {
        ::std::cout << p.index() << " " << p.unit().show() << "\n";
    };
    ::std::cout << "-------------------------\n";

    for(int i = 0; i < 10; i++) {
        ::std::cout << pop.pick_less(*(pop.begin() + 3))->unit().show() << "\n";
    };

    ::std::cout << "FINISH" << "\n";
};