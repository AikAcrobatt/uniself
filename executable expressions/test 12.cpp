// executable expressions.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <variant>

#include "uniself/time.h"

#include "new executable expressions.h"

int main() {
    std::cout << "START" << std::endl;

    uns::ex_ex::executable_tree t;
    {
        auto a = t.NewNode<uns::ex_ex::assignment>(true);
        auto v1 = t.NewNode<uns::ex_ex::access>(); v1->SetNode(0, 0);
        auto v2 = t.NewNode<uns::ex_ex::access>(); v2->SetNode(0, 1);
        auto v3 = t.NewNode<uns::ex_ex::access>(); v3->SetNode(0, 2);
        auto p = t.NewNode<uns::ex_ex::plus>();

        a->AddNode(3);
        a->AddNode(4);
        p->AddNode(1);
        p->AddNode(2);
    };

    uns::ex_ex::executable_tree::expt_t cash(3);
    cash[0] = 1;
    cash[1] = 2;
    cash[2] = 0;

    std::cout << cash[2].Cast<int>() << std::endl;

    t.Execute(cash);

    std::cout << cash[2].Cast<int>() << std::endl;

    int iterations = 1000000;

    {
        volatile long double v1 = 1;
        volatile long double v2 = 2;
        volatile long double v3 = 0;

        uns::timer T;
        for(int i = 0; i < iterations; i++)
            v3 = v1 + v2;
        std::cout << "1: " << T.Count() << std::endl;
    };

    {
        uns::ex_ex::executable_tree::cash_t c;
        uns::timer T;
        for(int i = 0; i < iterations; i++)
            t.Execute(cash, c);
        std::cout << "2: " << T.Count() << std::endl;
    };

    std::cout << "FINISH" << std::endl;
}

