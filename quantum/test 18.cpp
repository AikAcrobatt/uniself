#include <iostream>
 

#include "quantum.h"

template<size_t N>
class oracle {
protected:
    std::bitset<N> pattern;
public:
    oracle() { pattern.reset(); };
    oracle(const std::bitset<N>& classical_register) { pattern = classical_register; };
    oracle(std::string str) { pattern = str; };

    bool operator==(const std::bitset<N>& classical_register) const { return pattern == classical_register; };
    template<typename floating_point_t, typename engine_t>
    bool operator==( uns::quantum::reg<uns::quantum::bit<floating_point_t, engine_t>, N>& quantum_register) const { 
        bool res = true;
        int idx = 0;// pattern.size() - 1;
        for (idx = 0; idx < N; idx++)
            res = res && (quantum_register[idx].Estimation() == pattern[N - 1 - idx]);
        return res; 
    };
};


class Neg {
public:

    template<typename bit_t>
    void operator()(bit_t& qbit) const {
        qbit.State0() = -qbit.State0();
        qbit.State1() = -qbit.State1();
    };
};


int main() {
    std::cout << "START" << std::endl;

    std::bitset<10> breg("0100110101");
    std::cout << breg << std::endl;

    uns::quantum::reg<uns::quantum::bit<float, std::mt19937>, 10> qreg(std::bitset<10>("0100110101"));

    uns::quantum::single_bit_register_operation<uns::quantum::Pauli_operator_1> NOT;
    uns::quantum::single_bit_register_operation<uns::quantum::Pauli_operator_2> P2;
    uns::quantum::single_bit_register_operation<uns::quantum::Pauli_operator_3> P3;
    uns::quantum::single_bit_register_operation<uns::quantum::Hadamar_operator> H;
    uns::quantum::single_bit_register_operation<uns::quantum::Phase> Ph; 
    uns::quantum::single_bit_register_operation<uns::quantum::Pi_d_8> P_8;
    //P_8(qreg);
    oracle<10> O(breg);
    auto resreg = qreg.Estimation();

    std::cout << resreg << std::endl;
    std::cout << std::boolalpha << (O == resreg) << std::endl;

    std::cout << "FINISH" << std::endl;
};