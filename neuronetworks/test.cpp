
#include <iostream>
#include <sstream>

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
class _input_data_object: public uns::nn::general::input_data_object<signal_type> {
public:
	_input_data_object() {};

	std::size_t size() const override { return 10; };
	uns::nn::general::neuron<signal_type>* get(const uns::nn::adress&) const override {
		return new uns::nn::sequential_neuron<signal_type>{};
	};


};

int main() {
    std::cout << "START\n";

    auto repr = boost::property_tree::ptree{};

    std::stringstream s;

    s << R"^^(
{
	"layers" : {
		"total": "1",
		"#0": {
			"neurons": {
				"total": "2",
				"#0": {
					"type": "Line.Perc",
					"R": "1.2056",
					"C": "-0.070099999999999996",
					"links": {
						"total": "3",
						"#0": {
							"layer": "-1",
							"index": "0",
							"weight": "8.0470000000000006"
						},
						"#1": {
							"layer": "-1",
							"index": "1",
							"weight": "5.1600000000000001"
						},
						"#2": {
							"layer": "-1",
							"index": "2",
							"weight": "-0.01"
						}
					},
					"params": {
						"total": "2",
						"#0": "5",
						"#1": "6"
					}
				},
				"#1": {
					"type": "Sigma.Perc",
					"R": "1.2056",
					"C": "-0.070099999999999996",
					"links": {
						"total": "1",
						"#0": {
							"layer": "0",
							"index": "0",
							"weight": "1.0470000000000006"
						}
					},
					"params": {
						"total": "1",
						"#0": "5"
					}
				}
			}
		}
	},
	"outputs": {
		"total": "1",
		"#0": {
			"layer": "0",
			"index": "1"
		}
	}
}
    )^^";

    boost::property_tree::read_json(s, repr);

	auto nn = uns::nn::sequential_base_network<uns::nn::sequential_neuron<double>>{};

	auto act = _activator_cast<double>{};
	auto col = _collector_cast<double>{};
	auto ido = _input_data_object<double>{};
	nn.set(repr, act, col, ido);

    std::cout << "FINISH\n";
};