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

	namespace traits {
		
		template<
			typename signal_t,
			typename weight_t,
			typename params_t
		>
		class signal {
		public:
			using signal_type = signal_t;
			using weight_type = weight_t;
			using params_type = params_t;
		};


		template<
			typename signal_t,
			typename weight_t,
			typename params_t
		>
		class input_data_object {
		public:
			using signal_type = signal_t;
			using weight_type = weight_t;
			using params_type = params_t;
		};
	};

	template<
		typename signal_t,
		typename description_neuron_t,
		typename description_network_t,
		typename neuron_t,
		typename input_neuron_t,
		typename output_neuron_t,
		typename input_data_object_t,
		typename output_data_object_t,
		typename neuron_alloc_t,
		typename input_neuron_alloc_t,
		typename output_neuron_alloc_t,
		typename user_purposed_t
	>
	/*	requires (::std::same_as<signal_t, typename description_neuron_t::signal_type>
				|| ::std::same_as<void, description_neuron_t>)
			&& (::std::same_as<signal_t, typename description_neuron_t::signal_type>
				|| ::std::same_as<void, description_neuron_t>)
			&& (::std::same_as<signal_t, typename description_network_t::signal_type>
				|| ::std::same_as<void, description_network_t>)

			&& (::std::same_as<signal_t, typename neuron_t::signal_type>
				|| ::std::same_as<void, neuron_t>)
			&& (::std::same_as<signal_t, typename input_neuron_t::signal_type>
				|| ::std::same_as<void, input_neuron_t>)
			&& (::std::same_as<signal_t, typename output_neuron_t::signal_type>
				|| ::std::same_as<void, output_neuron_t>)
			&& (::std::same_as<neuron_t, typename neuron_alloc_t::value_type>
				|| ::std::same_as<void, neuron_alloc_t>)
			&& (::std::same_as<input_neuron_t, typename input_neuron_alloc_t::value_type>
				|| ::std::same_as<void, input_neuron_alloc_t>)
			&& (::std::same_as<output_neuron_t, typename output_neuron_alloc_t::value_type>
				|| ::std::same_as<void, output_neuron_alloc_t>)
	*/
	class traits {
	public:
		using signal_type = signal_t;
		using neuron_type = neuron_t;
		using description_neuron_type = description_neuron_t;
		using description_network_type = description_network_t;
		using input_neuron_type = input_neuron_t;
		using output_neuron_type = output_neuron_t;
		using input_data_object_type = input_data_object_t;
		using output_data_object_type = output_data_object_t;
		using neuron_alloc_type = neuron_alloc_t;
		using input_neuron_alloc_type = input_neuron_alloc_t;
		using output_neuron_alloc_type = output_neuron_alloc_t;
		using user_purposed_type = user_purposed_t;
	};


	template<
		typename nn_traits_t,
		typename signal_t = typename nn_traits_t::signal_type,
		typename description_neuron_t = typename nn_traits_t::description_neuron_type,
		typename description_network_t = typename nn_traits_t::description_network_type,
		typename neuron_t = typename nn_traits_t::neuron_type,
		typename input_neuron_t = typename nn_traits_t::input_neuron_type,
		typename output_neuron_t = typename nn_traits_t::output_neuron_type,
		typename input_data_object_t = typename nn_traits_t::input_data_object_type,
		typename output_data_object_t = typename nn_traits_t::output_data_object_type,
		typename neuron_alloc_t = typename nn_traits_t::neuron_alloc_type,
		typename input_neuron_alloc_t = typename nn_traits_t::input_neuron_alloc_type,
		typename output_neuron_alloc_t = typename nn_traits_t::output_neuron_alloc_type,
		typename user_purposed_t = typename nn_traits_t::user_purposed_t
	>
	concept is_traits = ::std::same_as<
		nn_traits_t,
		::uns::nn::traits<
			signal_t,
			description_neuron_t,
			description_network_t,
			neuron_t,
			input_neuron_t,
			output_neuron_t,
			input_data_object_t,
			output_data_object_t,
			neuron_alloc_t,
			input_neuron_alloc_t,
			output_neuron_alloc_t,
			user_purposed_t
		>
	>;


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
			::std::hash<long long int> m_subhash;
		public:
			inline ::std::size_t operator()(const ::uns::nn::adress& adress) const noexcept {
				return m_subhash((static_cast<long long int>(adress.layer) << sizeof(int) * 8) + static_cast<long long int>(adress.index));
			};
		};
	};


	namespace description {

		template<::uns::nn::is_traits traits_t>
		class neuron {
		public:
			using signal_type = traits_t::signal_type;
			using weight_type = traits_t::signal_type;
			using param_type = traits_t::signal_type;

			::std::u8string activator = u8"";
			signal_type r = signal_type{};
			::std::u8string collector = u8"";
			signal_type c = signal_type{};
			::std::vector<::std::pair<::uns::nn::adress, weight_type>> links;
			::std::vector<param_type> params;

			neuron() noexcept {};
			neuron(const ::uns::nn::description::neuron<traits_t>& obj) noexcept :
				activator(obj.activator),
				collector(obj.collector),
				links(obj.links),
				params(obj.params),
				r(obj.r),
				c(obj.c)
			{};
			::uns::nn::description::neuron<traits_t>& operator=(const ::uns::nn::description::neuron<traits_t>& obj) {
				if(this == &obj) return *this;

				activator = obj.activator;
				collector = obj.collector;
				links = obj.links;
				params = obj.params;
				r = obj.r;
				c = obj.c;

				return *this;
			};
			neuron(::uns::nn::description::neuron<traits_t>&& obj) noexcept :
				activator(::std::move(obj.activator)),
				collector(::std::move(obj.collector)),
				links(::std::move(obj.links)),
				params(::std::move(obj.params)),
				r(::std::move(obj.r)),
				c(::std::move(obj.c))
			{};
			::uns::nn::description::neuron<traits_t>& operator=(::uns::nn::description::neuron<traits_t>&& obj) {
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
		};


		template<::uns::nn::is_traits traits_t>
		class network {
		public:
			::std::vector<::std::vector<::uns::nn::description::neuron<traits_t>>> layers;
			::std::vector<::uns::nn::adress> outputs;

			network() noexcept {};
			network(const ::uns::nn::description::network<traits_t>& obj) noexcept :
				layers(obj.layers),
				outputs(obj.outputs)
			{};
			::uns::nn::description::network<traits_t>& operator=(const ::uns::nn::description::network<traits_t>& obj) {
				if(this == &obj) return *this;

				layers = obj.layers;
				outputs = obj.outputs;

				return *this;
			};
			network(::uns::nn::description::network<traits_t>&& obj) noexcept :
				layers(::std::move(obj.layers)),
				outputs(::std::move(obj.outputs))
			{};
			::uns::nn::description::network<traits_t>& operator=(::uns::nn::description::network<traits_t>&& obj) {
				if(this == &obj) return *this;

				layers = ::std::move(obj.layers);
				outputs = ::std::move(obj.outputs);

				return *this;
			};
			~network() {};
		};
	};


	namespace general {
		//this namespace is intended to contain prototypes and interfaces

		//an interface of general neuron (a preliminary declaration)
		template<::uns::nn::is_traits traits_t>
		class neuron;


		//an interface of input-data provider
		template<::uns::nn::is_traits traits_t>
		class input_data_object {
		public:
			virtual ::std::size_t size() const noexcept = 0;
			virtual typename traits_t::input_neuron_type* get(const ::uns::nn::adress&) = 0;
			virtual bool has_it(const ::uns::nn::adress&) const noexcept = 0;
		};


		//basic activator class can be used as default activator
		template<::uns::nn::is_traits traits_t>
		class activator {
		protected:
			traits_t::signal_type m_value = typename traits_t::signal_type{ 0 };
		public:
			activator() noexcept {};
			activator(const ::uns::nn::general::activator<traits_t>& obj) noexcept :
				m_value(obj.m_value)
			{};
			::uns::nn::general::activator<traits_t>& operator=(const ::uns::nn::general::activator<traits_t>& obj) noexcept {
				if(this == &obj) return *this;

				m_value = obj.m_value;

				return *this;
			};
			activator(::uns::nn::general::activator<traits_t>&& obj) noexcept :
				m_value(::std::move(obj.m_value))
			{};
			::uns::nn::general::activator<traits_t>& operator=(::uns::nn::general::activator<traits_t>&& obj) noexcept {
				if(this == &obj) return *this;

				m_value = ::std::move(obj.m_value);

				return *this;
			};
			::uns::nn::general::activator<traits_t>& operator=(traits_t::signal_type value) noexcept {
				m_value = value;
				return *this;
			};
			~activator() noexcept {};

			virtual ::std::size_t capacity() const noexcept { return sizeof(*this); };

			const traits_t::signal_type& value() const noexcept { return m_value; };
			traits_t::signal_type& value() noexcept { return m_value; };

			virtual ::std::u8string type() const noexcept { return u8"Zero"; };

			virtual traits_t::signal_type operator()(traits_t::signal_type, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) { return typename traits_t::signal_type{ 0 }; };

			virtual traits_t::signal_type _dS(traits_t::signal_type, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) const { return typename traits_t::signal_type{ 0 }; };

			virtual traits_t::signal_type _dp(int, traits_t::signal_type, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) const { return typename traits_t::signal_type{ 0 }; };

			//basic auxiliary class intended to create activators from string
			//	actually it's a map from string to activator
			class caster {
			public:
				virtual ::std::unique_ptr<::uns::nn::general::activator<traits_t>> operator()(const ::std::u8string_view&) const = 0;
			};
		};


		//basic collector class can be used as default collector
		template<::uns::nn::is_traits traits_t>
		class collector {
		protected:
			traits_t::signal_type m_value = typename traits_t::signal_type{ 0 };
		public:
			collector() noexcept {};
			collector(const ::uns::nn::general::collector<traits_t>& obj) noexcept :
				m_value(obj.m_value)
			{};
			::uns::nn::general::collector<traits_t>& operator=(const ::uns::nn::general::collector<traits_t>& obj) noexcept {
				if(this == &obj) return *this;

				m_value = obj.m_value;

				return *this;
			};
			collector(::uns::nn::general::collector<traits_t>&& obj) noexcept :
				m_value(::std::move(obj.m_value))
			{};
			::uns::nn::general::collector<traits_t>& operator=(::uns::nn::general::collector<traits_t>&& obj) noexcept {
				if(this == &obj) return *this;

				m_value = ::std::move(obj.m_value);

				return *this;
			};
			::uns::nn::general::collector<traits_t>& operator=(traits_t::signal_type value) noexcept {
				m_value = value;
				return *this;
			};
			~collector() noexcept {};

			virtual ::std::size_t capacity() const noexcept { return sizeof(*this); };

			const traits_t::signal_type& value() const noexcept { return m_value; };
			traits_t::signal_type& value() noexcept { return m_value; };

			virtual ::std::u8string type() const noexcept { return u8"Zero"; };

			virtual traits_t::signal_type operator()(const ::std::vector<::std::pair<::uns::nn::general::neuron<traits_t>*, traits_t::signal_type>>&, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) { return typename traits_t::signal_type(0); };

			virtual traits_t::signal_type _dr(int, const ::std::vector<::std::pair<::uns::nn::general::neuron<traits_t>*, traits_t::signal_type>>&, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) { return typename traits_t::signal_type(0); };

			virtual traits_t::signal_type _dw(int, const ::std::vector<::std::pair<::uns::nn::general::neuron<traits_t>*, traits_t::signal_type>>&, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) { return typename traits_t::signal_type(0); };

			virtual traits_t::signal_type _dp(int, const ::std::vector<::std::pair<::uns::nn::general::neuron<traits_t>*, traits_t::signal_type>>&, const ::std::vector<typename traits_t::signal_type>&, const ::std::vector<typename traits_t::signal_type>&) { return typename traits_t::signal_type(0); };

			//basic auxiliary class intended to create collector from string
			//	actually it's a map from string to collector
			class caster {
			public:
				virtual ::std::unique_ptr<::uns::nn::general::collector<traits_t>> operator()(const ::std::u8string_view&) const = 0;
			};
		};


		//an interface of general neuron
		template<::uns::nn::is_traits traits_t>
		class neuron {
		public:
			using signal_type = typename traits_t::signal_type;
			using descr_type = typename traits_t::description_neuron_type;

			virtual descr_type descript() const noexcept = 0;
			virtual void set(
				const descr_type&,
				const ::uns::nn::adress&,
				const typename ::uns::nn::general::activator<traits_t>::caster&,
				const typename ::uns::nn::general::collector<traits_t>::caster&
			) = 0;
			virtual void link(
				::std::vector<::std::vector<::uns::nn::general::neuron<traits_t>*>>&,
				::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<traits_t>*, ::uns::nn::adress::hash>&,
				::uns::nn::general::input_data_object<traits_t>&
			) = 0;

			virtual ::std::size_t capacity() const noexcept { return sizeof(*this); };

			virtual int subneurons_total() const noexcept { return 0; };
			virtual const ::uns::nn::general::neuron<traits_t>* subneuron(int connection_idx) const noexcept { return nullptr; };
			virtual ::uns::nn::general::neuron<traits_t>* subneuron(int connection_idx) noexcept { return nullptr; };

			virtual const signal_type& R() const noexcept = 0;
			virtual signal_type& R() noexcept = 0;
			virtual const signal_type& C() const noexcept = 0;
			virtual signal_type& C() noexcept = 0;

			virtual ::uns::nn::adress adress() const noexcept = 0;
			virtual bool is_reversible() const noexcept { return false; };
			virtual const signal_type& dropout() const noexcept = 0;
			virtual signal_type& dropout() noexcept = 0;

			virtual void react(const ::std::vector<signal_type>& common_params) {};
			virtual void collect(const ::std::vector<signal_type>& common_params) {};
		};


		//an interface of general neuro network
		template<::uns::nn::is_traits traits_t>
		class network {
		public:
			using signal_type = typename traits_t::signal_type;
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
			virtual void react(const ::std::vector<signal_type>& common_params) {};

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
		::std::vector<signal_type> m_params;
		signal_type m_dropout = 0;

		::std::unique_ptr<::std::vector<::std::pair<::uns::nn::adress, weight_type>>> m_adresses = nullptr;
	public:
		sequential_neuron() noexcept : m_F(nullptr), m_S(nullptr), m_adresses(nullptr) {};
		sequential_neuron(const ::uns::nn::sequential_neuron<signal_t>&) = delete;
		::uns::nn::sequential_neuron& operator=(const ::uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron(::uns::nn::sequential_neuron<signal_t>&&) = delete;
		::uns::nn::sequential_neuron& operator=(::uns::nn::sequential_neuron<signal_t>&&) = delete;
		~sequential_neuron() noexcept {};

		virtual ::uns::nn::general::neuron<signal_type>::descr_type descript() const noexcept override {
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

		virtual void set(
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

		virtual void link(
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

		virtual ::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += m_adress.capacity();
			result += sizeof(m_F) + m_F->capacity();
			result += sizeof(m_S) + m_S->capacity();
			result += m_links.capacity() * sizeof(typename decltype(m_links)::value_type);
			result += m_params.capacity() * sizeof(typename decltype(m_params)::value_type);

			return result;
		};
		
		virtual ::uns::nn::adress adress() const noexcept override { return m_adress; };
		virtual const signal_type& dropout() const noexcept override { return m_dropout; };
		virtual signal_type& dropout() noexcept override { return m_dropout; };

		virtual int subneurons_total() const noexcept override { return static_cast<int>(m_links.size()); };
		virtual const ::uns::nn::general::neuron<signal_type>* subneuron(int connection_idx) const noexcept override {
			if(connection_idx >= 0 && connection_idx < m_links.size()) {
				return ::std::get<part::_neuron_>(m_links[connection_idx]);
			}
			else {
				return nullptr;
			};
		};
		virtual ::uns::nn::general::neuron<signal_type>* subneuron(int connection_idx) noexcept override {
			if(connection_idx >= 0 && connection_idx < m_links.size()) {
				return ::std::get<part::_neuron_>(static_cast<::std::pair<::uns::nn::general::neuron<signal_type>*, weight_type>>(m_links[connection_idx]));
			}
			else {
				return nullptr;
			};
		};

		virtual const signal_type& R() const noexcept override { return m_F->value(); };
		virtual signal_type& R() noexcept override { return m_F->value(); };
		virtual const signal_type& C() const noexcept override { return m_S->value(); };
		virtual signal_type& C() noexcept override { return m_S->value(); };

		virtual void react(const ::std::vector<signal_type>& common_params) override { (*m_F)(m_S->value(), m_params, common_params); };

		virtual void collect(const ::std::vector<signal_type>& common_params) override { (*m_S)(m_links, m_params, common_params); };
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
	public:
		nonrecursive_reverse_neuron() {};
		nonrecursive_reverse_neuron(const ::uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		::uns::nn::nonrecursive_reverse_neuron<signal_t>& operator=(const ::uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron(::uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		::uns::nn::nonrecursive_reverse_neuron<signal_t>& operator=(::uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		~nonrecursive_reverse_neuron() {};

		virtual void _link(
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

		virtual ::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += sizeof(m_r);
			result += sizeof(m_s);
			result += m__links.capacity() * sizeof(typename decltype(m__links)::value_type);
			result += sizeof(m_input);
			result += sizeof(m_is_learning);
			result += sizeof(m_dropout);

			return result;
		};

		virtual bool is_reversible() const noexcept override { return true; };

		virtual const signal_t& _R() const noexcept { return m_r; };
		virtual signal_t& _R() const noexcept { return m_r; };
		virtual const signal_t& _C() const noexcept { return m_s; };
		virtual signal_t& _C() const noexcept { return m_s; };

		virtual void _react(const ::std::vector<signal_t>& common_params) {
			m_r = dF_dS(m_params, common_params) * m_s;
		};

		virtual void _collect(const ::std::vector<signal_t>& common_params) {
			using neuron_type = ::uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>;

			m_s = 0;
			for(auto _link : m__links) {
				m_s += ::std::get<neuron_type::neuron>(_link)->m_r * ::std::get<neuron_type::neuron>(_link)->dS_dr(::std::get<neuron_type::place>(_link), m_params, common_params);
			};
			if(m_input != nullptr) {
				m_s += m_input->R();
			};
		};

		virtual bool is_learning() const noexcept { return m_is_learning; };

		virtual void set_learning(bool islearning) noexcept { m_is_learning = islearning; };

		virtual signal_t dS_dr(int index, const ::std::vector<signal_t>& common_params) const { return base::S->_dr(index, base::m_links, base::m_params, common_params); };

		virtual signal_t dS_dw(int index, const ::std::vector<signal_t>& common_params) const { return base::S->_dw(index, base::m_links, base::m_params, common_params); };

		virtual signal_t dS_dp(int index, const ::std::vector<signal_t>& common_params) const { return base::S->_dp(index, base::m_links, base::m_params, common_params); };

		virtual signal_t dF_dS(const ::std::vector<signal_t>& common_params) const { return base::F->_dS(base::C(), base::m_params, common_params); };

		virtual signal_t dF_dp(int index, const ::std::vector<signal_t>& common_params) const { return base::F->_dp(index, base::C(), base::m_params, common_params); };
	};


	//a class of network introduces the sequential neural network
	template<class neuron_t, class inputs_allocator_t = ::std::allocator<neuron_t>>
	class sequential_network : public ::uns::nn::general::network<neuron_t> {
	protected:
		using this_type = sequential_network<neuron_t, inputs_allocator_t>;
	public:
		using neuron_type = typename neuron_t;
		using signal_type = typename neuron_type::signal_type;
		using base = ::uns::nn::general::network<neuron_t>;
	protected:
		::std::vector<::std::vector<neuron_type*>> m_layers;
		inputs_allocator_t m_inputs_allocator;
	public:
		sequential_network() {};
		sequential_network(const ::uns::nn::sequential_network& net) = delete;
		::uns::nn::sequential_network& operator=(const ::uns::nn::sequential_network& net) = delete;
		sequential_network(::uns::nn::sequential_network&& net) = delete;
		::uns::nn::sequential_network& operator=(::uns::nn::sequential_network&& net) = delete;
		~sequential_network() {
			for(auto& layer : m_layers) {
				for(auto neuron : layer) {
					delete neuron;
				};
			};

			for(auto input : base::m_inputs) {
				m_inputs_allocator.deallocate(input, 1);
			};
		};

		virtual typename ::uns::nn::general::network<neuron_t>::descr_type descript() const noexcept override {
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
		
		virtual void set(
			const typename ::uns::nn::general::network<neuron_type>::descr_type& descriptor,
			const typename ::uns::nn::general::activator<signal_type>::caster& activator_cast,
			const typename ::uns::nn::general::collector<signal_type>::caster& collector_cast,
			::uns::nn::general::input_data_object<signal_type, inputs_allocator_t>& ido
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

			m_inputs_allocator = ido.get_allocator();
		};

		virtual ::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += m_layers.capacity() * sizeof(typename decltype(m_layers)::value_type);

			for(const auto& layer : m_layers) {
				result += layer.capacity() * sizeof(neuron_type*);

				for(auto neuron_ptr : layer) {
					result += neuron_ptr->capacity();
				};
			};

			result += sizeof(m_inputs_allocator);

			return result;
		};

		virtual void react(const ::std::vector<signal_type>& common_params) override {
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

		virtual signal_type R(::std::size_t layer_index, ::std::size_t index) const { return m_layers[layer_index][index]->R(); };

		virtual signal_type C(::std::size_t layer_index, ::std::size_t index) const { return m_layers[layer_index][index]->C(); };

		virtual signal_type O(::std::size_t index) const { return base::m_outputs[index]->R(); };

		virtual signal_type I(::std::size_t index) const { return base::m_inputs[index]->R(); };

		virtual ::std::size_t layers_total() const { return m_layers.size(); };
		virtual ::std::size_t neurons_total(size_t index) const { return m_layers[index].size(); };
		virtual ::std::size_t outputs_total() const { return base::m_outputs.size(); };
		virtual ::std::size_t inputs_total() const { return base::m_inputs.size(); };
	};

	
	//a class of network introduces the reversable neural network
	template<class signal_t, class inputs_allocator_t = ::std::allocator<::uns::nn::nonrecursive_reverse_neuron<signal_t>>, class outputs_allocator_t = ::std::allocator<::uns::nn::nonrecursive_reverse_neuron<signal_t>>>
	class nonrecursive_reverse_network: public ::uns::nn::sequential_network<::uns::nn::nonrecursive_reverse_neuron<signal_t>, inputs_allocator_t> {
	protected:
		using this_type = nonrecursive_reverse_network<signal_t, inputs_allocator_t, outputs_allocator_t>;
	public:
		using neuron_type = ::uns::nn::nonrecursive_reverse_neuron<signal_t>;
		using signal_type = typename neuron_type::signal_type;
		using base = ::uns::nn::sequential_network<::uns::nn::nonrecursive_reverse_neuron<signal_t>, inputs_allocator_t>;
	protected:
		::std::vector<::uns::nn::general::neuron<signal_type>*> m_reverse_inputs;
		outputs_allocator_t m_outputs_allocator;
	public:
		nonrecursive_reverse_network() {};
		nonrecursive_reverse_network(const ::uns::nn::nonrecursive_reverse_network& net) = delete;
		::uns::nn::nonrecursive_reverse_network& operator=(const ::uns::nn::nonrecursive_reverse_network& net) = delete;
		nonrecursive_reverse_network(::uns::nn::nonrecursive_reverse_network&& net) = delete;
		::uns::nn::nonrecursive_reverse_network& operator=(::uns::nn::nonrecursive_reverse_network&& net) = delete;
		~nonrecursive_reverse_network() {
			for(auto neuron_ptr : m_reverse_inputs) {
				m_outputs_allocator.deallocate(neuron_ptr, 1);
			};
		};

		virtual void _link(::uns::nn::general::input_data_object<signal_type, outputs_allocator_t>& odo) {
			auto reverse_inputs = ::std::unordered_map<::uns::nn::adress, ::uns::nn::general::neuron<typename base::signal_type>*, ::uns::nn::adress::hash>{};

			for(const auto& layer : base::m_layers) {
				for(auto neuron_ptr : layer) {
					neuron_ptr->_link(base::m_layers, reverse_inputs, odo);
				};
			};

			for(auto& [adress, reverse_input_ptr] : reverse_inputs) {
				m_reverse_inputs.push_back(reverse_input_ptr);
			};

			m_outputs_allocator = odo.get_allocator();
		};

		virtual ::std::size_t capacity() const noexcept override {
			::std::size_t result = base::capacity();

			result += m_reverse_inputs.capacity() * sizeof(typename decltype(m_reverse_inputs)::value_type);

			for(auto neuron_ptr : m_reverse_inputs) {
				result += neuron_ptr->capacity();
			};

			result += sizeof(m_outputs_allocator);

			return result;
		};

		virtual void _react(const ::std::vector<signal_type>& common_params) {
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

		virtual signal_type _R(::std::size_t layer_index, ::std::size_t index) const { return base::m_layers[layer_index][index]->_R(); };

		virtual signal_type _C(::std::size_t layer_index, ::std::size_t index) const { return base::m_layers[layer_index][index]->_C(); };

		virtual signal_type _O(::std::size_t index) const { return base::m_outputs[index]->_R(); };

		virtual signal_type _I(::std::size_t index) const { return base::m_inputs[index]->_R(); };
	};
};


#endif