#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <tuple>
#include <functional>

#include "uniself/math.hpp"


#ifndef UNS_LIB_NEURONETWORKS
#define UNS_LIB_NEURONETWORKS "neuronetworks.hpp"


#define UNS_DEV_EXCEPTION_MSG ::std::string{ __FUNCTION__ } + "[" + ::std::to_string(__LINE__) + "]"

namespace uns::nn {


	//an adress of neuron in every neuronetwork
	class adress {
	public:
		int layer = 0;
		int index = 0;

		inline bool operator==(const ::uns::nn::adress& obj) const noexcept {
			return layer == obj.layer && index == obj.index;
		};
		inline bool operator!=(const ::uns::nn::adress& obj) const noexcept {
			return !(*this == obj);
		};

		inline ::std::size_t capacity() const noexcept { return sizeof(*this); };

		class hash {
		protected:
			::std::hash<long long int> subhash;
		public:
			inline ::std::size_t operator()(const ::uns::nn::adress& adress) const noexcept {
				return subhash((static_cast<long long int>(adress.layer) << sizeof(int) * 8) + static_cast<long long int>(adress.index));
			};
		};
	};


	namespace description {

		template<typename signal_t>
		class neuron {
		public:
			using signal_type = signal_t;
			using weight_type = signal_t;
			using param_type = signal_t;

			::std::u8string activator = u8"";
			signal_type r = signal_type{};
			::std::u8string collector = u8"";
			signal_type c = signal_type{};
			::std::vector<::std::pair<::uns::nn::adress, weight_type>> links;
			::std::vector<param_type> params;

			neuron() noexcept {};
			neuron(const neuron& obj) noexcept :
				activator(obj.activator),
				collector(obj.collector),
				links(obj.links),
				params(obj.params),
				r(obj.r),
				c(obj.c)
			{};
			neuron& operator=(const neuron & obj) {
				if(this == &obj) return *this;

				activator = obj.activator;
				collector = obj.collector;
				links = obj.links;
				params = obj.params;
				r = obj.r;
				c = obj.c;

				return *this;
			};
			neuron(neuron&& obj) noexcept :
				activator(::std::move(obj.activator)),
				collector(::std::move(obj.collector)),
				links(::std::move(obj.links)),
				params(::std::move(obj.params)),
				r(::std::move(obj.r)),
				c(::std::move(obj.c))
			{};
			neuron& operator=(neuron&& obj) {
				if(this == &obj) return *this;

				activator = ::std::move(obj.activator);
				collector = ::std::move(obj.collector);
				links = ::std::move(obj.links);
				params = ::std::move(obj.params);
				r = ::std::move(obj.r);
				c = ::std::move(obj.c);

				return *this;
			};
			~neuron() {};

			::std::u8string to_string() const noexcept { return u8""; };

			void set(const ::std::u8string_view& str) {};
		};


		template<typename neuron_descr_t>
		class network {
		public:
			using neuron_descr_type = neuron_descr_t;

			::std::vector<::std::vector<neuron_descr_t>> layers;
			::std::vector<::uns::nn::adress> outputs;

			network() noexcept {};
			network(const network& obj) noexcept :
				layers(obj.layers),
				outputs(obj.outputs)
			{};
			network& operator=(const network& obj) {
				if(this == &obj) return *this;

				layers = obj.layers;
				outputs = obj.outputs;

				return *this;
			};
			network(network&& obj) noexcept :
				layers(::std::move(obj.layers)),
				outputs(::std::move(obj.outputs))
			{};
			network& operator=(network&& obj) {
				if(this == &obj) return *this;

				layers = ::std::move(obj.layers);
				outputs = ::std::move(obj.outputs);

				return *this;
			};
			~network() {};

			::std::u8string to_string() const noexcept { return u8""; };

			void set(const ::std::u8string_view& str) {};
		};
	};


	namespace general {
		//this namespace is intended to contain prototypes and interfaces

		//class that introduces a paramaters' container that is fit for the whole network but also for a particular neuron
		template<typename signal_t>
		class network_params {
		public:
			virtual signal_t get(int) const = 0;
			virtual const network_params& forward(const ::std::vector<signal_t>&) const = 0;
		};


		//an interface of general neuron (a preliminary declaration)
		template<typename signal_t>
		class neuron;


		//an interface of input-data provider
		template<typename signal_t>
		class input_data_object {
		public:
			virtual ::std::size_t size() const = 0;
			virtual neuron<signal_t>* get(const ::uns::nn::adress&) = 0;
			virtual bool has_it(const ::uns::nn::adress&) const noexcept = 0;
		};


		//basic activator class can be used as default activator
		template<typename signal_t>
		class activator {
		protected:
			signal_t m_value = signal_t(0);
		public:
			activator() noexcept {};
			activator(const activator& obj) noexcept :
				m_value(obj.m_value) {};
			activator& operator=(const activator& obj) noexcept {
				if(this == &obj) return *this;

				m_value = obj.m_value;

				return *this;
			};
			activator(activator&& obj) noexcept :
				m_value(::std::move(obj.m_value)) {};
			activator& operator=(activator&& obj) noexcept {
				if(this == &obj) return *this;

				m_value = ::std::move(obj.m_value);

				return *this;
			};
			activator& operator=(signal_t value) noexcept {
				m_value = value;
				return *this;
			};
			~activator() noexcept {};

			virtual ::std::size_t capacity() const noexcept { return sizeof(*this); };

			const signal_t& value() const noexcept { return m_value; };
			signal_t& value() noexcept { return m_value; };

			virtual ::std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(signal_t, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dS(signal_t, const network_params<signal_t>&) const { return signal_t(0); };

			virtual signal_t _dp(int, signal_t, const network_params<signal_t>&) const { return signal_t(0); };

			//basic auxiliary class intended to create activators from string
			//	actually it's a map from string to activator
			class caster {
			public:
				virtual ::std::unique_ptr<::uns::nn::general::activator<signal_t>> operator()(const ::std::u8string_view&) const = 0;
			};
		};


		//basic collector class can be used as default collector
		template<typename signal_t>
		class collector {
		protected:
			signal_t m_value = signal_t(0);
		public:
			collector() noexcept {};
			collector(const collector& obj) noexcept :
				m_value(obj.m_value) {};
			collector& operator=(const collector& obj) noexcept {
				if(this == &obj) return *this;

				m_value = obj.m_value;

				return *this;
			};
			collector(collector&& obj) noexcept :
				m_value(::std::move(obj.m_value)) {};
			collector& operator=(collector&& obj) noexcept {
				if(this == &obj) return *this;

				m_value = ::std::move(obj.m_value);

				return *this;
			};
			collector& operator=(signal_t value) noexcept {
				m_value = value;
				return *this;
			};
			~collector() noexcept {};

			virtual ::std::size_t capacity() const noexcept { return sizeof(*this); };

			const signal_t& value() const noexcept { return m_value; };
			signal_t& value() noexcept { return m_value; };

			virtual ::std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(const ::std::vector<::std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dr(int, const ::std::vector<::std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dw(int, const ::std::vector<::std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dp(int, const ::std::vector<::std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			//basic auxiliary class intended to create collector from string
			//	actually it's a map from string to collector
			class caster {
			public:
				virtual ::std::unique_ptr<::uns::nn::general::collector<signal_t>> operator()(const ::std::u8string_view&) const = 0;
			};
		};


		//an interface of general neuron
		template<typename signal_t>
		class neuron {
		public:
			using signal_type = signal_t;
			using descr_type = ::uns::nn::description::neuron<signal_type>;

			virtual ::std::u8string type() const noexcept = 0;

			virtual descr_type descript() const noexcept = 0;
			virtual void set(
				const descr_type&,
				const ::uns::nn::adress&,
				const typename ::uns::nn::general::activator<signal_type>::caster&,
				const typename ::uns::nn::general::collector<signal_type>::caster&
			) = 0;
			virtual void link(
				::std::vector<::std::vector<::uns::nn::general::neuron<signal_type>*>>&,
				::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<signal_type>*, ::uns::nn::adress::hash>&,
				::uns::nn::general::input_data_object<signal_type>&
			) = 0;

			virtual ::std::size_t capacity() const noexcept { return sizeof(*this); };

			virtual int subneurons_total() const noexcept { return 0; };
			virtual const neuron* subneuron(int connection_idx) const noexcept { return nullptr; };
			virtual neuron* subneuron(int connection_idx) noexcept { return nullptr; };

			virtual signal_type R() const noexcept { return signal_type{ 0 }; };
			virtual void R(signal_type) noexcept {};
			virtual signal_type C() const noexcept { return signal_type{ 0 }; };
			virtual void C(signal_type) noexcept {};

			virtual ::uns::nn::adress adress() const noexcept = 0;
			virtual bool is_reversible() const noexcept { return false; };
			virtual signal_type dropout() const noexcept { return signal_type{ 0 }; };
			virtual void dropout(signal_type) noexcept {};

			virtual void react(const network_params<signal_type>&) {};
			virtual void collect(const ::uns::nn::general::network_params<signal_t>& common_params) {};
		};


		//an interface of general neuro network
		template<typename neuron_t>
		class network {
		public:
			using signal_type = typename neuron_t::signal_type;
			using neuron_type = neuron_t;
			using descr_type = ::uns::nn::description::network<typename neuron_type::descr_type>;
		protected:
			::std::vector<::uns::nn::general::neuron<signal_type>*> m_outputs;
			::std::vector<::uns::nn::general::neuron<signal_type>*> m_inputs;
		public:
			virtual descr_type descript() const noexcept = 0;
			virtual void set(
				const descr_type&,
				const typename ::uns::nn::general::activator<signal_type>::caster&,
				const typename ::uns::nn::general::collector<signal_type>::caster&,
				::uns::nn::general::input_data_object<signal_type>&
			) = 0;
			virtual void react(const network_params<signal_type>&) {};

			virtual ::std::size_t capacity() const noexcept {
				::std::size_t result = sizeof(*this)
					+ sizeof(m_outputs)
					+ m_outputs.capacity() * sizeof(typename decltype(m_outputs)::value_type)
					+ sizeof(m_inputs)
					+ m_inputs.capacity() * sizeof(typename decltype(m_inputs)::value_type);

				for(auto output_ptr : m_outputs) {
					result += output_ptr->capacity();
				};
				for(auto input_ptr : m_inputs) {
					result += input_ptr->capacity();
				};

				return result;
			};
		};
	};


	//a class of neuron introduces the neurons supposed to consist sequential neural network's body
	template<class signal_t>
	class sequential_neuron : public ::uns::nn::general::neuron<signal_t> {
	protected:
		using this_type = ::uns::nn::general::neuron<signal_t>;
	public:
		using signal_type = signal_t;
		using weight_type = signal_t;
		using param_type = signal_t;
		using base = ::uns::nn::general::neuron<signal_t>;
	public:
		enum part{
			_neuron_ = 0,
			_weight_ = 1
		};
	protected:
		::uns::nn::adress m_adress;
		::std::unique_ptr<::uns::nn::general::activator<signal_type>> m_F = nullptr;
		::std::unique_ptr<::uns::nn::general::collector<signal_type>> m_S = nullptr;
		::std::vector<::std::pair<::uns::nn::general::neuron<signal_type>*, weight_type>> m_links;
		::std::vector<param_type> m_params;

		::std::unique_ptr<::std::vector<::std::pair<::uns::nn::adress, weight_type>>> m_adresses = nullptr;
	public:
		sequential_neuron() noexcept : m_F(nullptr), m_S(nullptr), m_adresses(nullptr) {};
		sequential_neuron(const ::uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron& operator=(const ::uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron(::uns::nn::sequential_neuron<signal_t>&&) = delete;
		sequential_neuron& operator=(::uns::nn::sequential_neuron<signal_t>&&) = delete;
		~sequential_neuron() noexcept {};

		::uns::nn::general::neuron<signal_type>::descr_type descript() const noexcept override {
			auto res = typename ::uns::nn::general::neuron<signal_type>::descr_type{};

			if(m_F != nullptr) {
				res.activator = m_F->type();
			}
			else {
				res.activator = u8"";
			};

			if(m_S != nullptr) {
				res.collector = m_F->type();
			}
			else {
				res.collector = u8"";
			};

			res.params = m_params;

			for(auto link : m_links) {
				res.links.emplace_back(
					::std::pair<::uns::nn::adress, weight_type>{
						::std::get<part::_neuron_>(link)->adress(),
						::std::get<part::_weight_>(link)
					}
				);
			};

			return res;
		};

		void set(
			const ::uns::nn::general::neuron<signal_type>::descr_type& descriptor,
			const ::uns::nn::adress& adress,
			const typename ::uns::nn::general::activator<signal_type>::caster& activator_cast,
			const typename ::uns::nn::general::collector<signal_type>::caster& collector_cast
		) override {
			m_adress = adress;

			m_F = activator_cast(descriptor.activator);
			if(m_F == nullptr) throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			*m_F = descriptor.r;

			m_S = collector_cast(descriptor.collector);
			if(m_S == nullptr) throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			*m_S = descriptor.c;

			m_params = descriptor.params;

			m_adresses = ::std::unique_ptr<::std::vector<::std::pair<::uns::nn::adress, weight_type>>>(new ::std::vector<::std::pair<::uns::nn::adress, weight_type>>{});
			*m_adresses = descriptor.links;
		};

		void link(
			::std::vector<::std::vector<::uns::nn::general::neuron<signal_t>*>>& main_body,
			::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<signal_type>*, ::uns::nn::adress::hash>& inputs,
			::uns::nn::general::input_data_object<signal_type>& ido
		) override {
			if(m_adresses == nullptr) throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);

			m_links.clear();

			for(const auto& [adress, weight] : *m_adresses) {
				auto link = ::std::pair<::uns::nn::general::neuron<signal_t>*, weight_type>{};
				if(ido.has_it(adress)) {
					if(inputs.find(adress) == inputs.end()) {
						::std::get<part::_neuron_>(link) = inputs[adress] = ido.get(adress);
					}
					else {
						::std::get<part::_neuron_>(link) = inputs[adress];
					};
				}
				else {
					if(
						adress.layer >= 0 && adress.layer < main_body.size()
						&& adress.index >= 0 && adress.index < main_body[adress.layer].size()
					) {
						::std::get<part::_neuron_>(link) = main_body[adress.layer][adress.index];
					}
					else {
						throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
				};
				::std::get<part::_weight_>(link) = weight;
				m_links.push_back(link);
			};

			m_adresses = nullptr;
		};

		::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += m_adress.capacity();
			result += sizeof(m_F) + m_F->capacity();
			result += sizeof(m_S) + m_S->capacity();
			result += m_links.capacity() * sizeof(typename decltype(m_links)::value_type);
			result += m_params.capacity() * sizeof(typename decltype(m_params)::value_type);

			return result;
		};

		::std::u8string type() const noexcept override { return m_F->type() + u8"." + m_S->type(); };

		::uns::nn::adress adress() const noexcept override { return m_adress; };

		int subneurons_total() const noexcept override { return static_cast<int>(m_links.size()); };
		const ::uns::nn::general::neuron<signal_type>* subneuron(int connection_idx) const noexcept override {
			if(connection_idx >= 0 && connection_idx < m_links.size()) {
				return ::std::get<part::_neuron_>(m_links[connection_idx]);
			}
			else {
				return nullptr;
			};
		};
		::uns::nn::general::neuron<signal_type>* subneuron(int connection_idx) noexcept override {
			if(connection_idx >= 0 && connection_idx < m_links.size()) {
				return ::std::get<part::_neuron_>(static_cast<::std::pair<::uns::nn::general::neuron<signal_type>*, weight_type>>(m_links[connection_idx]));
			}
			else {
				return nullptr;
			};
		};

		signal_type R() const noexcept override { return m_F->value(); };
		void R(const signal_type R) noexcept override { *m_F = R; };

		signal_type C() const noexcept { return m_S->value(); };
		void C(const signal_type c) noexcept { *m_S = c; };

		void react(const ::uns::nn::general::network_params<signal_type>& common_params) override { (*m_F)(m_S->value(), common_params.forward(m_params)); };

		void collect(const ::uns::nn::general::network_params<signal_type>& common_params) override { (*m_S)(m_links, common_params.forward(m_params)); };
	};


	//a class of neuron introduces the nonrecursive reversation neuron used for learning by gradient-down method
	template<typename signal_t>
	class nonrecursive_reverse_neuron : public ::uns::nn::sequential_neuron<signal_t> {
	protected:
		using this_type = ::uns::nn::nonrecursive_reverse_neuron<signal_t>;
		using base = ::uns::nn::sequential_neuron<signal_t>;
		using place_type = int;
	public:
	protected:
		base::signal_type m_r = 0;
		base::signal_type m_s = 0;
		::std::vector<::std::pair<nonrecursive_reverse_neuron<typename base::signal_type>*, place_type>> m__links;
		::uns::nn::general::neuron<typename base::signal_type>* m_input = nullptr;
		bool m_is_learning = true;
		base::signal_type m_dropout = base::signal_type{ 0 };
	public:
		nonrecursive_reverse_neuron() {};
		nonrecursive_reverse_neuron(const ::uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron<signal_t>& operator=(const ::uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron(::uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		nonrecursive_reverse_neuron<signal_t>& operator=(::uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		~nonrecursive_reverse_neuron() {};

		void _link(
			::std::vector<::std::vector<::uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>*>>& main_body,
			::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<typename base::signal_type>*, ::uns::nn::adress::hash>& reverse_inputs,
			::uns::nn::general::input_data_object<typename base::signal_type>& odo
		) {
			m__links.clear();

			for(const auto& layer : main_body) {
				for(const auto& neuron_ptr : layer) {
					if(neuron_ptr == nullptr) {
						throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};

					for(int subneuron_idx = 0; subneuron_idx < neuron_ptr->subneurons_total(); ++subneuron_idx) {
						if(neuron_ptr->subneuron(subneuron_idx)->adress() == this->adress()) {
							m__links.emplace_back(::std::pair<nonrecursive_reverse_neuron<typename base::signal_type>*, place_type>{ neuron_ptr, subneuron_idx });
						};
					};
				};
			};

			if(odo.has_it(this->adress())) {
				if(reverse_inputs.find(this->adress()) == reverse_inputs.end()) {
					m_input = reverse_inputs[this->adress()] = odo.get(this->adress());
				}
				else {
					m_input = reverse_inputs[this->adress()];
				};
			};
		};

		::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += sizeof(m_r);
			result += sizeof(m_s);
			result += m__links.capacity() * sizeof(typename decltype(m__links)::value_type);
			result += sizeof(m_input);
			result += sizeof(m_is_learning);
			result += sizeof(m_dropout);

			return result;
		};

		bool is_reversible() const noexcept override { return true; };

		signal_t dropout() const noexcept override { return m_dropout; };
		void dropout(signal_t new_dropout) noexcept override { m_dropout = (::uns::math::more(new_dropout, signal_t(1)) ? signal_t(1) : (::uns::math::less(new_dropout, signal_t(0)) ? signal_t(0) : new_dropout)); };

		signal_t _R() const noexcept { return m_r; };

		void _R(const signal_t R) noexcept { m_r = R; };

		signal_t _C() const noexcept { return m_s; };

		void _C(const signal_t c) noexcept { m_s = c; };

		void _react(const ::uns::nn::general::network_params<signal_t>& common_params) {
			m_r = dF_dS(common_params) * m_s;
		};

		void _collect(const ::uns::nn::general::network_params<signal_t>& common_params) {
			using neuron_type = ::uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>;

			m_s = 0;
			for(auto _link : m__links) {
				m_s += ::std::get<neuron_type::neuron>(_link)->m_r * ::std::get<neuron_type::neuron>(_link)->dS_dr(::std::get<neuron_type::place>(_link), common_params);
			};
			if(m_input != nullptr) {
				m_s += m_input->R();
			};
		};

		bool is_learning() const noexcept { return m_is_learning; };

		void set_learning(bool islearning) noexcept { m_is_learning = islearning; };

		signal_t dS_dr(int index, const ::uns::nn::general::network_params<signal_t>& common_params) const { return base::S->_dr(index, base::m_links, common_params.forward(base::m_params)); };	//TODO to think: forwarding params of this neuron can unintendedly replace params of subneurons

		signal_t dS_dw(int index, const ::uns::nn::general::network_params<signal_t>& common_params) const { return base::S->_dw(index, base::m_links, common_params.forward(base::m_params)); };

		signal_t dS_dp(int index, const ::uns::nn::general::network_params<signal_t>& common_params) const { return base::S->_dp(index, base::m_links, common_params.forward(base::m_params)); };

		signal_t dF_dS(const ::uns::nn::general::network_params<signal_t>& common_params) const { return base::F->_dS(base::C(), common_params.forward(base::m_params)); };

		signal_t dF_dp(int index, const ::uns::nn::general::network_params<signal_t>& common_params) const { return base::F->_dp(index, base::C(), common_params.forward(base::m_params)); };
	};


	//a class of network introduces the sequential neural network
	template<class neuron_t, class inputs_allocator_t>
	class sequential_base_network : public ::uns::nn::general::network<neuron_t> {
	protected:
		using this_type = sequential_base_network<neuron_t, inputs_allocator_t>;
	public:
		using neuron_type = typename neuron_t;
		using signal_type = typename neuron_type::signal_type;
		using base = ::uns::nn::general::network<neuron_t>;
	protected:
		::std::vector<::std::vector<neuron_type*>> m_layers;
	public:
		sequential_base_network() {};
		sequential_base_network(const sequential_base_network& net) = delete;
		sequential_base_network& operator=(const sequential_base_network& net) = delete;
		sequential_base_network(sequential_base_network&& net) = delete;
		sequential_base_network& operator=(sequential_base_network&& net) = delete;
		~sequential_base_network() {
			for(auto& layer : m_layers) {
				for(auto neuron : layer) {
					delete neuron;
				};
			};

			for(auto input : base::m_inputs) {
				inputs_allocator_t::dealloc(input);
			};
		};

		typename ::uns::nn::general::network<neuron_t>::descr_type descript() const noexcept override {
			auto res = typename ::uns::nn::general::network<neuron_t>::descr_type{};

			for(const auto& layer : m_layers) {
				res.layers.push_back(::std::vector<typename neuron_type::descr_type>{});
				for(auto neuron_ptr : layer) {
					res.layers.back().emplace_back(neuron_ptr->descript());
				};
			};

			for(auto output_ptr : base::m_outputs) {
				res.outputs.emplace_back(output_ptr->adress());
			};

			return res;
		};
		
		void set(
			const typename ::uns::nn::general::network<neuron_type>::descr_type& descriptor,
			const typename ::uns::nn::general::activator<signal_type>::caster& activator_cast,
			const typename ::uns::nn::general::collector<signal_type>::caster& collector_cast,
			::uns::nn::general::input_data_object<signal_type>& ido
		) override {
			for(const auto& layer : descriptor.layers) {
				m_layers.push_back(::std::vector<neuron_type*>{});
				for(const auto& neuron : layer) {
					auto neuron_ptr = new neuron_type{};
					m_layers.back().push_back(neuron_ptr);
				};
			};

			base::m_outputs.clear();
			for(const auto& output : descriptor.outputs) {
				if(
					output.layer >= 0 && output.layer < m_layers.size()
					&& output.index >= 0 && output.index < m_layers[output.layer].size()
				) {
					base::m_outputs.push_back(m_layers[output.layer][output.index]);
				}
				else {
					throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};
			};

			for(::std::size_t layer_idx = 0; layer_idx < m_layers.size(); ++layer_idx) {
				for(::std::size_t neuron_idx = 0; neuron_idx < m_layers[layer_idx].size(); ++neuron_idx) {
					if(layer_idx < descriptor.layers.size() && neuron_idx < descriptor.layers[layer_idx].size()) {
						m_layers[layer_idx][neuron_idx]->set(
							descriptor.layers[layer_idx][neuron_idx],
							::uns::nn::adress{ static_cast<int>(layer_idx), static_cast<int>(neuron_idx) },
							activator_cast,
							collector_cast
						);
					}
					else {
						throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
				};
			};

			auto layers = ::std::vector<::std::vector<::uns::nn::general::neuron<signal_type>*>>{};
			for(const auto& layer : m_layers) {
				layers.push_back(::std::vector<::uns::nn::general::neuron<signal_type>*>{});
				for(auto neuron_ptr : layer) {
					layers.back().push_back(neuron_ptr);
				};
			};
			auto inputs_map = ::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<signal_type>*, ::uns::nn::adress::hash>{};
			for(const auto& layer : m_layers) {
				for(auto neuron_ptr : layer) {
					neuron_ptr->link(layers, inputs_map, ido);
				};
			};

			base::m_inputs.clear();
			for(const auto& [adress, input_ptr] : inputs_map) {
				base::m_inputs.push_back(input_ptr);
			};
		};

		::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += m_layers.capacity() * sizeof(typename decltype(m_layers)::value_type);

			for(const auto& layer : m_layers) {
				result += layer.capacity() * sizeof(neuron_type*);

				for(auto neuron_ptr : layer) {
					result += neuron_ptr->capacity();
				};
			};

			return result;
		};

		void react(const ::uns::nn::general::network_params<signal_type>& common_params) override {
			for(auto input : base::m_inputs) {
				input->react(common_params);
			};
			for(auto& layer : m_layers) {
				for(auto neuron : layer) {
					neuron->collect(common_params);
					neuron->react(common_params);
				};
			};
		};

		signal_type R(::std::size_t layer_index, ::std::size_t index) const { return m_layers[layer_index][index]->R(); };

		signal_type C(::std::size_t layer_index, ::std::size_t index) const { return m_layers[layer_index][index]->C(); };

		signal_type O(::std::size_t index) const { return base::m_outputs[index]->R(); };

		signal_type I(::std::size_t index) const { return base::m_inputs[index]->R(); };

		::std::size_t layers_total() const { return m_layers.size(); };
		::std::size_t neurons_total(size_t index) const { return m_layers[index].size(); };
		::std::size_t outputs_total() const { return base::m_outputs.size(); };
		::std::size_t inputs_total() const { return base::m_inputs.size(); };
	};

	
	//a class of network introduces the reversable neural network
	template<class signal_t, class inputs_allocator_t, class outputs_allocator_t>
	class nonrecursive_reverse_network: public ::uns::nn::sequential_base_network<::uns::nn::nonrecursive_reverse_neuron<signal_t>, inputs_allocator_t> {
	protected:
		using this_type = nonrecursive_reverse_network<signal_t, inputs_allocator_t, outputs_allocator_t>;
	public:
		using neuron_type = ::uns::nn::nonrecursive_reverse_neuron<signal_t>;
		using signal_type = typename neuron_type::signal_type;
		using base = ::uns::nn::sequential_base_network<::uns::nn::nonrecursive_reverse_neuron<signal_t>, inputs_allocator_t>;
	protected:
		::std::vector<::uns::nn::general::neuron<signal_type>*> m_reverse_inputs;
	public:
		nonrecursive_reverse_network() {};
		nonrecursive_reverse_network(const nonrecursive_reverse_network& net) = delete;
		nonrecursive_reverse_network& operator=(const nonrecursive_reverse_network& net) = delete;
		nonrecursive_reverse_network(nonrecursive_reverse_network&& net) = delete;
		nonrecursive_reverse_network& operator=(nonrecursive_reverse_network&& net) = delete;
		~nonrecursive_reverse_network() {
			for(auto neuron_ptr : m_reverse_inputs) {
				outputs_allocator_t::dealloc(neuron_ptr);
			};
		};

		void _link(::uns::nn::general::input_data_object<signal_type>& odo) {
			auto reverse_inputs = ::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<typename base::signal_type>*, ::uns::nn::adress::hash>{};

			for(const auto& layer : base::m_layers) {
				for(auto neuron_ptr : layer) {
					neuron_ptr->_link(base::m_layers, reverse_inputs, odo);
				};
			};

			for(auto& [adress, reverse_input_ptr] : reverse_inputs) {
				m_reverse_inputs.push_back(reverse_input_ptr);
			};
		};

		::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += m_reverse_inputs.capacity() * sizeof(typename decltype(m_reverse_inputs)::value_type);

			for(auto neuron_ptr : m_reverse_inputs) {
				result += neuron_ptr->capacity();
			};

			return result;
		};

		void _react(const ::uns::nn::general::network_params<signal_type>& common_params) {
			for(auto m_reverse_input : m_reverse_inputs) {
				m_reverse_input->_react(common_params);
			};
			for(auto layer_ptr = base::m_layers.rbegin(); layer_ptr < base::m_layers.rend(); ++layer_ptr) {
				for(auto neuron_ptr = layer_ptr->rbegin(); neuron_ptr < layer_ptr.rend(); ++neuron_ptr) {
					neuron_ptr->_collect(common_params);
					neuron_ptr->_react(common_params);
				};
			};
		};

		signal_type _R(::std::size_t layer_index, ::std::size_t index) const { return base::m_layers[layer_index][index]->_R(); };

		signal_type _C(::std::size_t layer_index, ::std::size_t index) const { return base::m_layers[layer_index][index]->_C(); };

		signal_type _O(::std::size_t index) const { return base::m_outputs[index]->_R(); };

		signal_type _I(::std::size_t index) const { return base::m_inputs[index]->_R(); };
	};
};


#endif