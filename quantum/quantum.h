#pragma once

#include <array>
#include <bitset>
#include <complex>
#include <random>

/*namespace uns {

	class bit {
	protected:
		unsigned val : 1;
	public:
		bit(bool value = false) : val(value ? 1 : 0) {};
		bit(int value) : val(value != 0 ? 1 : 0) {};

		bit& operator=(int assignment_value) { val = (assignment_value != 0 ? 1 : 0); return *this; };
		bit& operator=(bool assignment_value) { val = (assignment_value ? 1 : 0); return *this; };

		bool operator==(const bit& comparing_obj) const { return val == comparing_obj.val; };
		bool operator==(bool comparing_val) const { return (val == 1 && comparing_val) || (val == 0 && !comparing_val); };
		bool operator==(int comparing_val) const { return (val == 1 && comparing_val != 0) || (val == 0 && comparing_val == 0); };

		bool operator!=(const bit& comparing_obj) const { return !(*this == comparing_obj); };
		bool operator!=(bool comparing_val) const { return !(*this == comparing_val); };
		bool operator!=(int comparing_val) const { return !(*this == comparing_val); };

		bit& operator+(const bit& obj) const {
			bit res;
			res.val = val ^ obj.val;
			return res;
		};

		bit& operator-(const bit& obj) const {
			return *this + obj;
		};

		bit& operator|(const bit& obj) const {
			bit res;
			res.val = val | obj.val;
			return res;
		};

		bit& operator~() const {
			bit res;
			res.val = ~val;
			return res;
		};

		bit& operator!() const {
			return ~(*this);
		};

		bit& operator*(const bit& obj) const {
			bit res;
			res.val = val & obj.val;
			return res;
		};

		bit& operator&(const bit& obj) const {
			return *this * obj;
		};

		operator unsigned short() const { return val == 1 ? 1 : 0; };
	};
};*/


namespace uns::quantum {

	template<typename floating_point_t, typename engine_t>
	class bit {
	public:
		using value_t = floating_point_t;
	protected:
		std::complex<floating_point_t> base1;	//true
		std::complex<floating_point_t> base0;	//false

		engine_t engine;
		std::uniform_real_distribution<floating_point_t> distribution = std::uniform_real_distribution<floating_point_t>(0.0, 1.0);

		static const floating_point_t pi;
		static engine_t common_deafult_engine;
	public:
		bit() : engine(common_deafult_engine()) { ForceValue(false); };
		bit(bool value) : engine(common_deafult_engine()) { ForceValue(value); };
		bit(const bit& copying_obj) noexcept : engine(common_deafult_engine()), base1(copying_obj.base1), base0(copying_obj.base0) {};
		bit& operator=(const bit& copying_obj) noexcept {
			base1 = copying_obj.base1;
			base0 = copying_obj.base0;
			return *this;
		};
		bit(bit&& moving_obj) noexcept : engine(common_deafult_engine()), base1(std::move(moving_obj.base1)), base0(std::move(moving_obj.base0)) {};
		bit& operator=(bit&& moving_obj) noexcept {
			base1 = std::move(moving_obj.base1);
			base0 = std::move(moving_obj.base0);
			return *this;
		};

		bool IsNormalized() const { return std::abs(std::norm(base1) + std::norm(base0) - 1.0) < 0.000001; };

		bool Estimation() const {
			return std::norm(base1) > std::norm(base0);
		};

		const std::complex<floating_point_t>& State1() const { return base1; };
		std::complex<floating_point_t>& State1() { return base1; };

		const std::complex<floating_point_t>& State0() const { return base0; };
		std::complex<floating_point_t>& State0() { return base0; };

		void Normalize() {
			floating_point_t modulus = std::sqrt(std::norm(base1) + std::norm(base0));
			base1 /= modulus;
			base0 /= modulus;
		};

		void ForceValue(bool value) {
			if (value) {
				base1 = std::polar(floating_point_t(1), 2 * pi * distribution(engine));
				base0 = 0;
			}
			else {
				base1 = 0;
				base0 = std::polar(floating_point_t(1), 2 * pi * distribution(engine));
			};
		};

		bool Measurement() {
			bool res = distribution(engine) < std::norm(base1);

			floating_point_t init_phase = 0;
			if (res) {
				init_phase = std::arg(base1);
				base1 = std::polar(floating_point_t(1), init_phase);
				base0 = 0;
			}
			else {
				init_phase = std::arg(base0);
				base1 = 0;
				base0 = std::polar(floating_point_t(1), init_phase);
			};

			return res;
		};
	};

	template<typename floating_point_t, typename engine_t>
	const floating_point_t uns::quantum::bit<floating_point_t, engine_t>::pi = 3.14159265359;
	
	std::random_device real_random_initializer;
	template<typename floating_point_t, typename engine_t>
	engine_t uns::quantum::bit<floating_point_t, engine_t>::common_deafult_engine = engine_t(real_random_initializer());
	

	template<typename bit_t, size_t N>
	class reg {
	public:
		using iterator = typename std::array<bit_t, N>::iterator;
	protected:
		std::array<bit_t, N> bits;
	public:
		reg() {};
		reg(const std::bitset<N>& classical_reg) {
			*this = classical_reg;
		};

		reg& operator=(const std::bitset<N>& classical_reg) {
			size_t bitset_idx = classical_reg.size() - 1;
			for (auto& qbit : bits)
				qbit.ForceValue(classical_reg[bitset_idx--]);
			return *this;
		};

		size_t Size() const { return N; };

		std::bitset<N>& Estimation() const {
			std::bitset<N> res;
			size_t bitset_idx = res.size() - 1;
			for (auto qbit : bits)
				res[bitset_idx--] = qbit.Estimation();

			return res;
		};

		auto& operator[](int idx) { return bits[idx]; };

		auto begin() { return bits.begin(); };

		auto end() { return bits.end(); };

		std::bitset<N>& Measurement() {
			std::bitset<N> res;
			size_t bitset_idx = res.size() - 1;
			for (auto qbit : bits)
				res[bitset_idx--] = qbit.Measurement();
			
			return res;
		};
	};


	template<typename single_bit_operation_t>
	class single_bit_register_operation {
	protected:
		//using single_bit_operation_t = Pauli_operator_1;

		single_bit_operation_t operation;
	public:
		template<typename floating_point_t, typename engine_t>
		void operator()(uns::quantum::bit<floating_point_t, engine_t>& quantum_bit) const {
			operation(quantum_bit);
		};

		template<typename bit_t, size_t N>
		void operator()(uns::quantum::reg<bit_t, N>& quantum_reg) const {
			for (auto& qbit : quantum_reg)
				operation(qbit);
		};

		template<typename iterator_t>
		void operator()(
			iterator_t quantum_reg_begin,
			iterator_t quantum_reg_end
		) const {
			for (iterator_t iterator = quantum_reg_begin; iterator < quantum_reg_end; iterator++)
				operation(*iterator);
		};
	};


	class Pauli_operator_1 {
	public:

		template<typename bit_t>
		void operator()(bit_t& qbit) const {
			auto temp = qbit.State1();
			qbit.State1() = qbit.State0();
			qbit.State0() = temp;
		};
	};


	class Pauli_operator_2 {
	public:

		template<typename bit_t>
		void operator()(bit_t& qbit) const {
			auto temp0 = qbit.State0();
			auto temp1 = qbit.State1();
			const auto _i = std::complex<typename bit_t::value_t>(0, 1);
			qbit.State0() = -_i * temp1;
			qbit.State1() = _i * temp0;
		};
	};


	class Pauli_operator_3 {
	public:

		template<typename bit_t>
		void operator()(bit_t& qbit) const {
			qbit.State0() = qbit.State0();
			qbit.State1() = -qbit.State1();
		};
	};


	class Hadamar_operator {
	public:

		template<typename bit_t>
		void operator()(bit_t& qbit) const {
			auto temp0 = qbit.State0();
			auto temp1 = qbit.State1();
			const typename bit_t::value_t _1_sqrt_2 = 1.0 / std::sqrt(2);
			qbit.State0() = _1_sqrt_2 * (temp0 + temp1);
			qbit.State1() = _1_sqrt_2 * (temp0 - temp1);
		};
	};


	class Phase {
	public:

		template<typename bit_t>
		void operator()(bit_t& qbit) const {
			const auto _i = std::complex<typename bit_t::value_t>(0, 1);
			qbit.State0() = qbit.State0();
			qbit.State1() = _i * qbit.State1();
		};
	};


	class Pi_d_8 {
	public:

		template<typename bit_t>
		void operator()(bit_t& qbit) const {
			const auto e_i_pi_d_4 = std::exp(std::complex<typename bit_t::value_t>(0, 1) * static_cast<typename bit_t::value_t>(3.14159265359) / static_cast<typename bit_t::value_t>(4));
			qbit.State0() = qbit.State0();
			qbit.State1() = e_i_pi_d_4 * qbit.State1();
		};
	};

};