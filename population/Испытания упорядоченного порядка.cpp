
#include <iostream>
#include <vector>

#include "population.h"

class unit : public uns::population::unit_interface<float, float> {
protected:
    unsigned int name = 0;
public:
    unit(int n) : name(n) {};

    int Show() const { return name; };
};

bool Comp(unit& a, unit& b) { return (b.Show() < a.Show()); };

using order_element = uns::population::order_element<unit, uns::population::ordered_order<unit, Comp>>;


int main() {
    std::cout << "START" << std::endl;

    std::random_device rand;

    uns::population::ordered_order<unit, Comp> pop(rand());

    pop.Push(std::shared_ptr<unit>(new unit(1)));
    pop.Push(std::shared_ptr<unit>(new unit(9)));
    pop.Push(std::shared_ptr<unit>(new unit(3)));
    pop.Push(std::shared_ptr<unit>(new unit(5)));
    pop.Push(std::shared_ptr<unit>(new unit(8)));
    pop.Push(std::shared_ptr<unit>(new unit(2)));
    pop.Push(std::shared_ptr<unit>(new unit(7)));
    pop.Push(std::shared_ptr<unit>(new unit(6)));
    pop.Push(std::shared_ptr<unit>(new unit(0)));
    pop.Push(std::shared_ptr<unit>(new unit(4)));

    for (auto p : pop)
        std::cout << p.Index() << " " << p.Unit().Show() << std::endl;

    pop.Sort();

    for (auto p : pop)
        std::cout << p.Index() << " " << p.Unit().Show() << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << pop.PickLess(*(pop.begin() + 3))->Unit().Show() << std::endl;
    };

    std::cout << "FINISH" << std::endl;
};