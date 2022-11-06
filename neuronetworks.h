#pragma once

#define UNS_HEADER_NEURONETWORKS

#include <iostream>
#include <string>
#include <limits>

#include "boost/property_tree/ptree.hpp"

#ifndef UNS_HEADER_MATH
#include "uniself/math.h"
#endif

namespace uns::nn {


	//an adress of neuron in every neuronetwork
	struct adress {
		int layer = 0;
		int index = 0;
	};

	namespace general {
		//this namespace is intended to contain prototypes and interfaces


		//an interface of input-data provider
		template<typename signal_t>
		class input_data_object {
		public:
			virtual signal_t* get(int) const = 0;
			virtual std::size_t size() const = 0;
		};


		//class that introduces a paramaters' container that is fit for the whole network but also for a particular neuron
		template<typename signal_t>
		class network_params {
		public:
			virtual signal_t get(int) const = 0;
			virtual network_params& forward(const std::vector<signal_t>&) const = 0;
		};


		//an interface of general neuron
		template<typename signal_t>
		class neuron {
		public:
			virtual std::u8string type() const noexcept = 0;
			virtual signal_t R() const noexcept = 0;
			virtual void R(signal_t) noexcept = 0;
			virtual uns::nn::adress adress() const noexcept = 0;
			virtual bool is_reversible() const noexcept = 0;
			virtual signal_t dropout() const noexcept = 0;
			virtual void react(const network_params&) = 0;
		};


		//an interface of general neuro network
		template<typename signal_t>
		class network {
		protected:
			std::vector<neuron<signal_t>*> _outputs;
			std::vector<neuron<signal_t>*> _inputs;	//TODO here was an std::vector<input_neuron<signal_t>*> _inputs
		public:
			virtual void react() = 0;
		};


		//basic activator class can be used as default activator
		template<typename signal_t>
		class activator {
		protected:
			signal_t _value = signal_t(0);
		public:
			activator() noexcept {};
			activator(const activator& obj) noexcept :
				_value(obj._value)
			{};
			activator& operator=(const activator& obj) noexcept {
				if(this == &obj) return *this;

				_value = obj._value;
				
				return *this;
			};
			activator(activator&& obj) noexcept :
				_value(std::move(obj._value))
			{};
			activator& operator=(activator&& obj) noexcept {
				if(this == &obj) return *this;

				_value = std::move(obj._value);

				return *this;
			};
			activator& operator=(signal_t value) noexcept {
				_value = value;
				return *this;
			};
			~activator() noexcept {};

			const signal_t& value() const noexcept { return _value; };
			signal_t& value() noexcept { return _value; };

			virtual std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(signal_t, const network_params&) { return signal_t(0); };

			virtual signal_t _dS(signal_t, const network_params&) const { return signal_t(0); };

			virtual signal_t _dp(int, signal_t, const network_params&) const { return signal_t(0); };
		};


		//basic collector class can be used as default collector
		template<typename signal_t>
		class collector {
		protected:
			signal_t _value = signal_t(0);
		public:
			collector() noexcept {}; 
			collector(const collector& obj) noexcept :
				_value(obj._value) {};
			collector& operator=(const collector& obj) noexcept {
				if(this == &obj) return *this;

				_value = obj._value;

				return *this;
			};
			collector(collector&& obj) noexcept :
				_value(std::move(obj._value)) {};
			collector& operator=(collector&& obj) noexcept {
				if(this == &obj) return *this;

				_value = std::move(obj._value);

				return *this;
			};
			collector& operator=(signal_t value) noexcept {
				_value = value;
				return *this;
			};
			~collector() noexcept {};

			const signal_t& value() const noexcept { return _value; };
			signal_t& value() noexcept { return _value; };

			virtual std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };

			virtual signal_t _dr(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };

			virtual signal_t _dw(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };

			virtual signal_t _dp(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };
		};
	};


	//a class of input_neuron introduces the neural network's data input
	template<typename signal_t>
	class input_neuron : public uns::nn::general::neuron<signal_t> {
	protected:
		signal_t* _value_of_cell = nullptr;
		signal_t r = 0;
		int _num_of_cell = 0;
		signal_t _dropout = signal_t{ 0 };
	public:
		input_neuron() {};
		input_neuron(const input_neuron& inp) = delete;
		input_neuron>& operator=(const input_neuron& inp) = delete;
		input_neuron(input_neuron&& inp) = delete;
		input_neuron& operator=(input_neuron&& inp) = delete;
		~input_neuron() {};

		std::u8string type() const noexcept override { return u8"Input"; };

		signal_t R() const noexcept override { return r; };

		void R(signal_t R) noexcept override { r = R; };

		uns::nn::adress adress() const noexcept override { return uns::nn::adress{ -1, _num_of_cell }; };

		bool is_reversible() const noexcept override { return false; };

		signal_t dropout() const noexcept override { return _dropout; };

		void dropout(signal_t Dropout) noexcept {
			_dropout = (
				uns::math::more(Dropout, signal_t{ 1 })
				? signal_t{ 1 }
				: (
					uns::math::less(Dropout, signal_t{ 0 })
					? signal_t{ 0 }
					: Dropout
				)
			);
		};

		void react(const uns::nn::general::network_params<signal_t>&) noexcept override { r = *_value_of_cell; };

		void set(int num_of_input, const uns::nn::general::input_data_object<signal_t>& ido) {
			if(num_of_input < 0) {
				throw std::out_of_range("input_neuron can't refer to a cell with negative index");
			};
			//TODO to think about how to check correctly the num_of_input to max_val violation
			_num_of_cell = num_of_input;
			_value_of_cell = ido.Get(_num_of_cell);
		};
	};


	//a class of input_neuron introduces the sequential neural network's data input
	template<class signal_t>
	class sequential_neuron : public uns::nn::general::neuron<signal_t> {
	protected:
		using weight_t = signal_t;
		using param_t = signal_t;
		using base_t = uns::nn::general::neuron<signal_t>;
	public:
		using representation_t = boost::property_tree::ptree;

		enum {
			neuron = 0,
			weight = 1
		};
	protected:
		int layer_index = 0;
		size_t index = 0;

		uns::nn::adress _adress;
		std::unique_ptr<uns::nn::general::activator<signal_t>> F;
		std::unique_ptr<uns::nn::general::collector<signal_t>> S;
		std::vector<std::pair<uns::nn::general::neuron<signal_t>*, weight_t>> _links;
		std::vector<param_t> _params;
	public:
		using neuron_signal_t = signal_t;

		sequential_neuron() noexcept : F(nullptr), S(nullptr) {};
		sequential_neuron(const uns::nn::sequential_neuron<signal_t>& n) = delete;
		sequential_neuron<signal_t>& operator=(const uns::nn::sequential_neuron<signal_t>& n) = delete;
		sequential_neuron(uns::nn::sequential_neuron<signal_t>&& n) = delete;
		sequential_neuron<signal_t>& operator=(uns::nn::sequential_neuron<signal_t>&& n) = delete;
		~sequential_neuron() noexcept {};

		representation_t represent() const;

		std::u8string type() const noexcept override { return F->type() + u8"." + S->type(); };

		uns::nn::adress adress() const noexcept override { return _adress; };
		void adress(uns::nn::adress Adress) const noexcept override { _adress = Adress; };

		bool is_reversible() const noexcept override { return false; };

		signal_t dropout() const noexcept override { return 0; };

		signal_t R() const noexcept override { return F->value(); };

		void R(const signal_t R) noexcept override { *F = R; };

		signal_t C() const noexcept { return S->value(); };

		void C(const signal_t c) noexcept { *S = c; };

		void react(const uns::nn::general::network_params& common_params) override { (*F)(S->Value(), common_params.forward(_params)); };

		void collect(const uns::nn::general::network_params& common_params) { (*S)(_links, common_params.forward(_params)); };
	};

	template<typename signal_t>
	inline typename uns::nn::sequential_neuron<signal_t>::representation_t uns::nn::sequential_neuron<signal_t>::represent() const {
		using neuron_t = uns::nn::sequential_neuron<signal_t>;
		using representation_t = uns::nn::neuron_representation<signal_t>;

		uns::nn::neuron_representation<signal_t> res;
		res.r = R();
		res.s = C();
		size_t last_link_index = 0;
		for(auto link : _links) {
			res._links.push_back(std::tuple<int, size_t, weight_t>(0, 0, std::get<neuron_t::weight>(link)));
			last_link_index = res._links.size() - 1;
			std::tie(
				std::get<representation_t::layer_adress>(res._links[last_link_index]),
				std::get<representation_t::neuron_adress>(res._links[last_link_index])
			) = std::get<neuron_t::neuron>(link)->Index();
		};
		for(auto param : _params) {
			res._params.push_back(param);
		};

		return res;
	};


};






		/*

			virtual std::u8string type() const noexcept = 0;
			virtual signal_t R() const noexcept = 0;
			virtual void R(signal_t) noexcept = 0;
			virtual uns::nn::adress adress() const noexcept = 0;
			virtual bool is_reversible() const noexcept = 0;
			virtual signal_t dropout() const noexcept = 0;
			virtual void react(const network_params&) = 0;

		*/