
#include <iostream>
#include <chrono>

#include "uniself/math.h"
#include "uniself/population.h"

class unit : public ::uns::population::unit<float, float> {
protected:
    float m_rate = 0.0F;
    points_type m_pregnancy = 0.0F;
    health_type m_health = 1.0F;
public:

    health_type health() const noexcept { return m_health; };

    virtual void add_points(const points_type& points_gain) noexcept override { m_pregnancy += points_gain; };

    virtual void damage(const health_type& health_decrease) noexcept override { m_health -= health_decrease; };

    virtual bool is_alive() const noexcept override { return ::uns::math::more(m_health, 0.0F); };

    virtual points_type points() const noexcept override { return m_pregnancy; };

    void condition() noexcept { add_points(0.40); };
};

using order_element = ::uns::population::order_element<unit, ::uns::population::unordered_order<unit>>;

class ration_source : public ::uns::population::ration_source<order_element> {
protected:
    float stock = 0.0F;
public:
    void refresh(float set_stock) {
        if(::uns::math::lesseq(stock, 0.0F)) {
            stock = set_stock;
        }
        else {
            stock += set_stock;
        };
    };

    void feed(order_element& order_element) override {
        float meal = 1.0F;

        if(::uns::math::more(stock - meal, -meal)) {
            order_element.unit().damage(-meal);
            stock -= meal;
        };
    };
};


int main() {
    ::std::cout << "START" << "\n";
    int interations = 100;

    ::uns::population::machine<
        typename order_element::order_type,
        ration_source,
        ::uns::population::breed_manager<order_element>,
        ::uns::population::fatal_act_operator<order_element>,
        ::uns::population::population_size_controller
    > pop(0, 1.1F);

    pop.units.push(::std::shared_ptr<unit>(new unit()));
    {
        auto t3 = ::std::chrono::steady_clock::now();
        auto t4 = ::std::chrono::steady_clock::now();
        auto t1 = ::std::chrono::steady_clock::now();
        auto cout_total = t1 - t1;
        for(int i = 0; i < interations; i++) {
            pop.ration_source.refresh(100);
            for(auto unit_ptr : pop.units) {
                unit_ptr.unit().condition();
            };
            pop.condition();

            t3 = ::std::chrono::steady_clock::now();
            ::std::cout << "Iter:" << i << ", size:" << pop.units.size() << "\n";
            t4 = ::std::chrono::steady_clock::now();
            cout_total += (t4 - t3);
        };
        auto t2 = ::std::chrono::steady_clock::now();
        ::std::cout << static_cast<long double>(::std::chrono::duration_cast<::std::chrono::microseconds>((t2 - t1) - cout_total).count()) * 0.000001 << "\n";
    };

    ::std::cout << "FINISH" << "\n";
};