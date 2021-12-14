
#include <iostream>
#include <vector>

#include "population.h"

class unit : public uns::population::unit_interface<float, float> {
protected:
    float rate = 0.0F;
    points_t pregnancy = 0.0F;
    health_t health = 5.0F;
public:

    health_t Health() const { return health; };

    virtual void SetPoints(const points_t& points_gain) { pregnancy += points_gain; };

    virtual void Damage(const health_t health_decrease) { health -= health_decrease; };

    virtual bool IsAlive() const { return uns::math::More(health, 0.0F); };

    virtual bool StilPregnant() { return uns::math::More(pregnancy--, 0.0F); };
};

using order_element = uns::population::order_element<unit, uns::population::unordered_order<unit>>;

class ration_source : public uns::population::ration_source_interface<order_element> {
protected:
    float stock = 0.0F;
public:
    void Refresh(float set_stock) {
        if (uns::math::LessEq(stock, 0.0F))
            stock = set_stock;
        else
            stock += set_stock;
    };

    virtual void Feed(order_element& order_element) {
        float meal = 1.0F;

        if (uns::math::More(stock - meal, -1.0F)) {
            order_element.Unit().Damage(-meal);
            stock -= meal;
        };
    };
};


int main() {
    std::cout << "START" << std::endl;

    //std::random_device rand;

    uns::population::basic_machine<
        uns::population::unordered_order<unit>,
        ration_source,
        uns::population::breed_manager_interface<order_element>,
        uns::population::fatal_act_operator_interface<order_element>,
        uns::population::basic_number_controller
    > pop(0, 1.1F);

    pop.units.Push(std::shared_ptr<unit>(new unit()));

    int interations = 100;
    for (int i = 0; i < interations; i++) {
        pop.ration_source.Refresh(10);
        pop.Condition();

        int j = 0;
        while (pop.pregnancy.size() > 0 && j++ < 200) {
            pop.units.Push(pop.pregnancy[0]);
            pop.pregnancy.pop_front();
        };

        std::cout << "Iter:" << i << ", size:" << pop.units.Size() << std::endl;
    };

    std::cout << "FINISH" << std::endl;
};