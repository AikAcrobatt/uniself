
#include <iostream>
#include <sstream>
#include <vector>
#include <array>

#include <boost/property_tree/ptree.hpp>
#include "boost/property_tree/json_parser.hpp"

#include "uniself/neuronetworks.h"
/*
#pragma warning(default: 4668; default: 4365)

#define BOOST_TEST_MODULE uns_neuronetworks
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

#pragma warning(disable: 4514)

BOOST_AUTO_TEST_SUITE(neuronetworks_str_test)

    BOOST_AUTO_TEST_CASE(correct1) {

        BOOST_TEST(true);
    };

BOOST_AUTO_TEST_SUITE_END();
*/

template<typename signal_type>
class Line: public uns::nn::general::activator<signal_type> {
public:
	signal_type operator()(signal_type collector, const uns::nn::general::network_params<signal_type>& params) override { return collector; };
	std::u8string type() const noexcept override { return u8"Line"; };
};
template<typename signal_type>
class Sigma: public uns::nn::general::activator<signal_type> {
public:
	signal_type operator()(signal_type collector, const uns::nn::general::network_params<signal_type>& params) override { return -collector; };
	std::u8string type() const noexcept override { return u8"Sigma"; };
};

template<typename signal_type>
class _activator_cast: public uns::nn::general::activator<signal_type>::caster {
public:
	_activator_cast() {};

	std::unique_ptr<uns::nn::general::activator<signal_type>> operator()(const std::u8string_view& activator_id) const override {
		if(activator_id == u8"Line") {
			return std::unique_ptr<uns::nn::general::activator<signal_type>>{ new Line<signal_type>{} };
		};
		if(activator_id == u8"Sigma") {
			return std::unique_ptr<uns::nn::general::activator<signal_type>>{ new Sigma<signal_type>{} };
		};

		return nullptr;
	};
};

template<typename signal_type>
class Perc: public uns::nn::general::collector<signal_type> {
public:
	using base = uns::nn::general::collector<signal_type>;

	signal_type operator()(
		const std::vector<std::pair<uns::nn::general::neuron<signal_type>*, signal_type>>& links,
		const uns::nn::general::network_params<signal_type>& params
	) override {
		base::m_value = 0;

		for(auto [neuron_ptr, weight] : links) {
			base::m_value += neuron_ptr->R() * weight;
		};

		return base::m_value;
	};

	std::u8string type() const noexcept override { return u8"Perc"; };
};

template<typename signal_type>
class _collector_cast: public uns::nn::general::collector<signal_type>::caster {
public:
	_collector_cast() {};

	std::unique_ptr<uns::nn::general::collector<signal_type>> operator()(const std::u8string_view& activator_id) const override {
		if(activator_id == u8"Perc") {
			return std::unique_ptr<uns::nn::general::collector<signal_type>>{ new Perc<signal_type>{} };
		};

		return nullptr;
	};
};

template<typename signal_type>
class network_params : public uns::nn::general::network_params<signal_type> {
public:
	signal_type get(int) const override {
		return signal_type{ 0 };
	};
	const uns::nn::general::network_params<signal_type>& forward(const std::vector<signal_type>&) const override { return *this; };
};


template<typename signal_type>
class input_neuron: public uns::nn::general::neuron<signal_type> {
public:
	using base = uns::nn::general::neuron<signal_type>;
	using repr_type = uns::nn::representation::neuron<signal_type>;

	uns::nn::adress m_adress;
	signal_type* data = nullptr;

	input_neuron() noexcept {};
	input_neuron(const input_neuron&) = delete;
	input_neuron& operator=(const input_neuron&) = delete;
	input_neuron(input_neuron&&) = delete;
	input_neuron& operator=(input_neuron&&) = delete;
	~input_neuron() noexcept {};

	std::u8string type() const noexcept override { return u8"input"; };

	repr_type represent() const noexcept override { return repr_type{}; };
	void set(
		const repr_type& repr,
		const uns::nn::adress& adress,
		const typename uns::nn::general::activator<signal_type>::caster& act,
		const typename uns::nn::general::collector<signal_type>::caster& col
	) override {
		m_adress = adress;
	};
	void link(
		std::vector<std::vector<uns::nn::general::neuron<signal_type>*>>&,
		std::unordered_map<uns::nn::adress, uns::nn::general::neuron<signal_type>*, uns::nn::adress::hash>&,
		uns::nn::general::input_data_object<signal_type>&
	) override {};

	int subneurons_total() const noexcept override { return 0; };
	const uns::nn::general::neuron<signal_type>* subneuron(int connection_idx) const noexcept override { return nullptr; };
	uns::nn::general::neuron<signal_type>* subneuron(int connection_idx) noexcept override { return nullptr; };

	signal_type R() const noexcept override { return *data; };

	uns::nn::adress adress() const noexcept override { return m_adress; };
};


template<typename signal_type>
class _input_data_object: public uns::nn::general::input_data_object<signal_type> {
public:
	std::array<signal_type, 10> data;

	_input_data_object() {};

	std::size_t size() const override { return data.size(); };
	uns::nn::general::neuron<signal_type>* get(const uns::nn::adress& adress) override {
		auto res = new input_neuron<signal_type>{};
		res->m_adress = adress;
		if(adress.index >= 0 && adress.index < 10) {
			res->data = &(data[adress.index]);
		}
		else {
			res->data = nullptr;
		};

		return res;
	};
	bool has_it(const uns::nn::adress& adress) const noexcept override {
		return (adress.layer < 0 && adress.index >= 0 && adress.index < data.size());
	};

	static void dealloc(uns::nn::general::neuron<signal_type>* reverse_input_neuron_ptr) {
		delete reinterpret_cast<input_neuron<signal_type>*>(reverse_input_neuron_ptr);
	};
};

template<typename signal_type>
class _output_data_object: public uns::nn::general::input_data_object<signal_type> {
public:
	std::array<signal_type, 10> data;

	_output_data_object() {};

	std::size_t size() const override { return data.size(); };
	uns::nn::general::neuron<signal_type>* get(const uns::nn::adress& adress) override {
		auto res = new input_neuron<signal_type>{};
		res->m_adress = adress;
		if(adress.index >= 0 && adress.index < 10) {
			res->data = &(data[adress.index]);
		}
		else {
			res->data = nullptr;
		};

		return res;
	};
	bool has_it(const uns::nn::adress& adress) const noexcept override {
		return (adress.layer == 1 && adress.index >= 0 && adress.index < data.size());
	};

	static void dealloc(uns::nn::general::neuron<signal_type>* reverse_input_neuron_ptr) {
		delete reinterpret_cast<input_neuron<signal_type>*>(reverse_input_neuron_ptr);
	};
};

int main() {
    std::cout << "START\n";

	auto repr = uns::nn::representation::network<uns::nn::representation::neuron<double>>{};

	auto act = _activator_cast<double>{};
	auto col = _collector_cast<double>{};
	auto ido = _input_data_object<double>{};

	repr.layers.push_back(std::vector<uns::nn::representation::neuron<double>>{});

	repr.layers.back().push_back(uns::nn::representation::neuron<double>{});
	repr.layers.back().back().activator = u8"Line";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 0.12;
	repr.layers.back().back().c = -0.3;
	repr.layers.back().back().links = {
		{ { -1, 0 }, 1.01 },
		{ { -1, 1 }, 1.02 },
		{ { -1, 2 }, 1.02 }
	};
	repr.layers.back().back().params = { 1, 3, 5 };

	repr.layers.back().push_back(uns::nn::representation::neuron<double>{});
	repr.layers.back().back().activator = u8"Sigma";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 50.21;
	repr.layers.back().back().c = 8.08;
	repr.layers.back().back().links = {
		{ { 0, 0 }, 15.01 }
	};
	repr.layers.back().back().params = { 1 };

	repr.layers.push_back(std::vector<uns::nn::representation::neuron<double>>{});

	repr.layers.back().push_back(uns::nn::representation::neuron<double>{});
	repr.layers.back().back().activator = u8"Line";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 0;
	repr.layers.back().back().c = -1;
	repr.layers.back().back().links = {
		{ { 0, 0 }, -1.5 },
		{ { 0, 1 }, 7000.0 }
	};
	repr.layers.back().back().params = { 100, 300 };

	repr.outputs.push_back(uns::nn::adress{ 1, 0 });

	auto nn = uns::nn::nonrecursive_reverse_network<double, _input_data_object<double>, _output_data_object<double>>{};
	nn.set(repr, act, col, ido);

	auto odo = _output_data_object<double>{};
	nn._link(odo);

    std::cout << "FINISH\n";
};