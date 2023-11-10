
#include <iostream>

#include "uniself/neuronetworks.hpp"


namespace test {

	using signal = ::uns::nn::traitset::signal<float, float, float>;


	class activator_caster : public ::uns::nn::general::activator<::test::signal>::caster {
	public:
		virtual ::std::unique_ptr<::uns::nn::general::activator<::test::signal>> operator()(const ::std::u8string_view&) const { return nullptr; };
	};


	class collector_caster : public ::uns::nn::general::collector<::test::signal>::caster {
	public:
		virtual ::std::unique_ptr<::uns::nn::general::collector<::test::signal>> operator()(const ::std::u8string_view&) const { return nullptr; };
	};


	class neur_descr : public ::uns::nn::description::neuron<::test::signal> {};


	using traitset_neuron = ::uns::nn::traitset::neuron<::test::signal, ::test::neur_descr, ::test::activator_caster, ::test::collector_caster>;


	class input_neuron : public ::uns::nn::general::neuron<::test::traitset_neuron> {
	protected:
		using base = ::uns::nn::general::neuron<::test::traitset_neuron>;
	public:
		virtual typename base::neuron_traitset::description_type descript() const noexcept override {
			return typename base::neuron_traitset::description_type{};
		};
		virtual void set(
			const typename base::neuron_traitset::description_type&,
			const ::uns::nn::adress&
		) override {};
		virtual void link(
			::std::vector<::std::vector<::uns::nn::general::neuron<typename base::neuron_traitset>*>>&,
			::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<typename base::neuron_traitset>*, ::uns::nn::adress::hash>&
		) override {};

		virtual ::std::size_t capacity() const noexcept override { return sizeof(*this); };

		virtual int subneurons_total() const noexcept override { return 0; };
		virtual const ::uns::nn::general::neuron<typename base::neuron_traitset>* subneuron(int connection_idx) const noexcept override { return nullptr; };
		virtual ::uns::nn::general::neuron<typename base::neuron_traitset>* subneuron(int connection_idx) noexcept override { return nullptr; };

		virtual const typename base::neuron_traitset::signal_traitset::signal_type& R() const noexcept override { return 0; };
		virtual typename base::neuron_traitset::signal_traitset::signal_type& R() noexcept {
			static typename base::neuron_traitset::signal_traitset::signal_type s_res = 0;
			return s_res;
		};
		virtual const typename base::neuron_traitset::signal_traitset::signal_type& C() const noexcept override { return 0; };
		virtual typename base::neuron_traitset::signal_traitset::signal_type& C() noexcept {
			static typename base::neuron_traitset::signal_traitset::signal_type s_res = 0;
			return s_res;
		};

	};


	using input = ::uns::nn::traitset::input<
		::test::signal,
		typename ::std::unordered_map<::uns::nn::adress, ::test::input_neuron*, ::uns::nn::adress::hash>::iterator,
		::std::allocator<::test::input_neuron>
	>;


	class ido : public ::uns::nn::general::input_data_object<::test::input> {
	protected:
		::std::unordered_map<::uns::nn::adress, ::test::input_neuron*, ::uns::nn::adress::hash> m_map;
	public:
		using base = ::uns::nn::general::input_data_object<::test::input>;

		virtual ::std::size_t size() const noexcept override {
			return m_map.size();
		};
		virtual typename base::input_traitset::input_neuron_type* get(const ::uns::nn::adress& adress) noexcept override {
			return m_map[adress];
		};
		virtual typename base::iterator_type begin() noexcept override { return m_map.begin(); };
		virtual typename base::iterator_type end() noexcept override { return m_map.end(); };
		virtual void clear() noexcept override { m_map.clear(); };
	};


	using traitset_network = ::uns::nn::traitset::network<::uns::nn::sequential_neuron<::test::traitset_neuron>, ::uns::nn::description::network<::test::traitset_neuron>, ::test::ido>;


	template<typename odo_t>
		requires ::std::derived_from<
			odo_t,
			typename ::uns::nn::general::input_data_object<
				typename odo_t::input_traitset
			>
		>
	class traitset_reversive_network : public ::uns::nn::traitset::network<::uns::nn::nonrecursive_reversive_neuron<::test::traitset_neuron>, ::uns::nn::description::network<::test::traitset_neuron>, ::test::ido> {
	public:
		using output_data_object_type = odo_t;
	};
};


int main() {
    ::std::cout << "START\n";

	auto repr = ::uns::nn::description::network<::uns::nn::description::neuron<double>>{};

	auto act = _activator_cast<double>{};
	auto col = _collector_cast<double>{};
	auto ido = _input_data_object<double>{};

	repr.layers.push_back(::std::vector<::uns::nn::description::neuron<double>>{});

	repr.layers.back().push_back(::uns::nn::description::neuron<double>{});
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

	repr.layers.back().push_back(::uns::nn::description::neuron<double>{});
	repr.layers.back().back().activator = u8"Sigma";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 50.21;
	repr.layers.back().back().c = 8.08;
	repr.layers.back().back().links = {
		{ { 0, 0 }, 15.01 }
	};
	repr.layers.back().back().params = { 1 };

	repr.layers.push_back(::std::vector<::uns::nn::description::neuron<double>>{});

	repr.layers.back().push_back(::uns::nn::description::neuron<double>{});
	repr.layers.back().back().activator = u8"Line";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 0;
	repr.layers.back().back().c = -1;
	repr.layers.back().back().links = {
		{ { 0, 0 }, -1.5 },
		{ { 0, 1 }, 7000.0 }
	};
	repr.layers.back().back().params = { 100, 300 };

	repr.outputs.push_back(::uns::nn::adress{ 1, 0 });


	auto тт = ::uns::nn::nonrecursive_reversive_network<::test::traitset_reversive_network<::test::ido>>{};
	auto nn = ::uns::nn::nonrecursive_reverse_network<double, _input_data_object<double>, _output_data_object<double>>{};
	nn.set(repr, act, col, ido);

	auto odo = _output_data_object<double>{};
	nn._link(odo);

    ::std::cout << "FINISH\n";
};