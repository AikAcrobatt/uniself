
#include <iostream>

#include "uniself/neuronetworks.hpp"


namespace test {

	using signal = ::uns::nn::traitset::signal<float, float, float>;


	template<typename signal_traitset_t>
	class line_activator : public ::uns::nn::general::activator<signal_traitset_t> {
	protected:
		using base = ::uns::nn::general::activator<signal_traitset_t>;
	public:
		virtual ::std::u8string type() const noexcept override { return u8"Line"; };

		virtual typename base::signal_traitset::signal_type operator()(
			typename base::signal_traitset::signal_type C,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept override {
			return
				static_cast<base::signal_traitset::signal_type>(lps[0]) * C
				+ static_cast<base::signal_traitset::signal_type>(lps[1]);
		};

		virtual typename base::signal_traitset::signal_type _dS(
			typename base::signal_traitset::signal_type C,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept override {
			return
				static_cast<base::signal_traitset::signal_type>(lps[0]);
		};

		virtual typename base::signal_traitset::signal_type _dp(
			int param_idx,
			typename base::signal_traitset::signal_type C,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept override {
			switch(param_idx) {
				case 0:
				{
					return C;
					break;
				}
				case 1:
				{
					return typename base::signal_traitset::signal_type{ 1 };
					break;
				}
				default:
				{
					return typename base::signal_traitset::signal_type{ 0 };
					break;
				}
			};
		};

	};


	template<typename signal_traitset_t>
	class perc_collector : public ::uns::nn::general::collector<signal_traitset_t> {
	protected:
		using base = ::uns::nn::general::collector<signal_traitset_t>;
	public:
		virtual ::std::u8string type() const noexcept override { return u8"Perc"; };

		virtual typename base::signal_traitset::signal_type operator()(
			const ::std::vector<::std::pair<::uns::nn::general::neuron_view<typename base::signal_traitset>*, typename base::signal_traitset::weight_type>>& conns,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept override {
			auto res = typename base::signal_traitset::signal_type{ 0 };

			for(const auto& conn : conns) {
				res += conn.first->R() * static_cast<typename base::signal_traitset::signal_type>(conn.second);
			};

			return res + lps[2];
		};

		virtual typename base::signal_traitset::signal_type _dr(
			int conn_idx,
			const ::std::vector<::std::pair<::uns::nn::general::neuron_view<typename base::signal_traitset>*, typename base::signal_traitset::weight_type>>& conns,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept {
			return static_cast<typename base::signal_traitset::signal_type>(conns[conn_idx].second);
		};

		virtual typename base::signal_traitset::signal_type _dw(
			int conn_idx,
			const ::std::vector<::std::pair<::uns::nn::general::neuron_view<typename base::signal_traitset>*, typename base::signal_traitset::weight_type>>& conns,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept {
			return conns[conn_idx].first->R();
		};

		virtual typename base::signal_traitset::signal_type _dp(
			int param_idx,
			const ::std::vector<::std::pair<::uns::nn::general::neuron_view<typename base::signal_traitset>*, typename base::signal_traitset::weight_type>>& conns,
			const ::std::vector<typename base::signal_traitset::params_type>& lps,
			const ::std::vector<typename base::signal_traitset::params_type>& gps
		) const noexcept {
			switch(param_idx) {
				case 2:
				{
					return typename base::signal_traitset::signal_type{ 1 };
					break;
				}
				default:
				{
					return typename base::signal_traitset::signal_type{ 0 };
					break;
				}
			};
		};

	};


	class activator_caster : public ::uns::nn::general::activator<::test::signal>::caster {
	public:
		virtual ::std::unique_ptr<::uns::nn::general::activator<::test::signal>> operator()(const ::std::u8string_view& str) const {
			if(str == u8"Line") {
				return ::std::unique_ptr<::uns::nn::general::activator<::test::signal>>{ new ::test::line_activator<::test::signal>{} };
			}
			else {
				return nullptr;
			};
		};
	};


	class collector_caster : public ::uns::nn::general::collector<::test::signal>::caster {
	public:
		virtual ::std::unique_ptr<::uns::nn::general::collector<::test::signal>> operator()(const ::std::u8string_view& str) const {
			if(str == u8"Perc") {
				return ::std::unique_ptr<::uns::nn::general::collector<::test::signal>>{ new ::test::perc_collector<::test::signal>{} };
			}
			else {
				return nullptr;
			};
		};
	};


	class neur_descr : public ::uns::nn::description::neuron<::test::signal> {};


	using traitset_neuron = ::uns::nn::traitset::neuron<::test::signal, ::test::neur_descr, ::test::activator_caster, ::test::collector_caster>;


	class input_neuron : public ::uns::nn::general::neuron<::test::traitset_neuron> {
	protected:
		using base = ::uns::nn::general::neuron<::test::traitset_neuron>;

		typename base::neuron_traitset::signal_traitset::signal_type m_val = 0;
	public:

		virtual const typename signal_traitset::signal_type& R() const noexcept override { return m_val; };
		virtual typename signal_traitset::signal_type& R() noexcept { return m_val; };

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
			if(auto iter = m_map.find(adress); iter == m_map.end()) {
				m_map[adress] = new ::test::input_neuron{};
			};
			return m_map[adress];
		};
		virtual typename base::iterator_type begin() noexcept override { return m_map.begin(); };
		virtual typename base::iterator_type end() noexcept override { return m_map.end(); };
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

	auto repr = ::uns::nn::description::network<::test::traitset_neuron>{};

	auto ido = ::test::ido{};
	auto ido_set = [&ido] (int i) {
		ido.get(::uns::nn::adress{ -1, 0 })->adress() = ::uns::nn::adress{ -1, 0 };
		ido.get(::uns::nn::adress{ -1, 1 })->adress() = ::uns::nn::adress{ -1, 1 };
		ido.get(::uns::nn::adress{ -2, 0 })->adress() = ::uns::nn::adress{ -2, 0 };
		ido.get(::uns::nn::adress{ -2, 1 })->adress() = ::uns::nn::adress{ -2, 1 };

		ido.get(::uns::nn::adress{ -1, 0 })->R() = 2.0 * static_cast<long double>(i) + 0.5;
		ido.get(::uns::nn::adress{ -1, 1 })->R() = 2.0 * static_cast<long double>(i) + 1.0;
		ido.get(::uns::nn::adress{ -2, 0 })->R() = 2.0 * static_cast<long double>(i) + 1.5;
		ido.get(::uns::nn::adress{ -2, 1 })->R() = 2.0 * static_cast<long double>(i) + 2.0;
	};


	repr.layers.push_back(::std::vector<::test::neur_descr>{});

	repr.layers.back().push_back(::test::neur_descr{});
	repr.layers.back().back().activator = u8"Line";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 0;
	repr.layers.back().back().c = 0;
	repr.layers.back().back().links = {
		{ { -1, 0 }, 1.0 },
		{ { -1, 1 }, 0.5 },
		{ { -2, 0 }, -1.0 },
		{ { -2, 1 }, -2.0 }
	};
	repr.layers.back().back().params = { 1, 0, 0 };

	repr.layers.back().push_back(::test::neur_descr{});
	repr.layers.back().back().activator = u8"Line";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 0;
	repr.layers.back().back().c = 0;
	repr.layers.back().back().links = {
		{ { -1, 0 }, 1.0 },
		{ { -1, 1 }, 3.0 },
		{ { -2, 0 }, -1.0 },
		{ { -2, 1 }, -1.0 }
	};
	repr.layers.back().back().params = { -1, 0, 0 };

	repr.layers.push_back(::std::vector<::test::neur_descr>{});

	repr.layers.back().push_back(::test::neur_descr{});
	repr.layers.back().back().activator = u8"Line";
	repr.layers.back().back().collector = u8"Perc";
	repr.layers.back().back().r = 0;
	repr.layers.back().back().c = 0;
	repr.layers.back().back().links = {
		{ { 0, 0 }, 1 },
		{ { 0, 1 }, 2 }
	};
	repr.layers.back().back().params = { 1, -1, 0 };

	repr.outputs.push_back(::uns::nn::adress{ 1, 0 });

	ido_set(0);

	auto nn = ::uns::nn::nonrecursive_reversive_network<::test::traitset_reversive_network<::test::ido>>{};
	nn.set(repr, ido);

	auto odo = ::test::ido{};
	nn._link(odo);

	auto all_print = [] (::uns::nn::nonrecursive_reversive_network<::test::traitset_reversive_network<::test::ido>>& n) {
		::std::cout << "[0, 0] (" << n.C(0, 0) << ", " << n.R(0, 0) << ")\n";
		::std::cout << "[0, 1] (" << n.C(0, 1) << ", " << n.R(0, 1) << ")\n";
		::std::cout << "[1, 0] (" << n.C(1, 0) << ", " << n.R(1, 0) << ")\n";
		::std::cout << "out[0] = " << n.O(0) << "\n";
	};

	auto _all_print = [] (::uns::nn::nonrecursive_reversive_network<::test::traitset_reversive_network<::test::ido>>& n) {
		::std::cout << "[0, 0] (" << n._C(0, 0) << ", " << n._R(0, 0) << ")\n";
		::std::cout << "[0, 1] (" << n._C(0, 1) << ", " << n._R(0, 1) << ")\n";
		::std::cout << "[1, 0] (" << n._C(1, 0) << ", " << n._R(1, 0) << ")\n";
	};

	for(int i = 0; i < 10; ++i) {
		::std::cout << "------------------" << i << "\n";

		ido_set(i);		
		nn.react({});
		all_print(nn);

		odo.get(::uns::nn::adress{ 1, 0 })->R() = static_cast<long double>(i);
		nn._react({});
		_all_print(nn);
	};

    ::std::cout << "FINISH\n";
};