#pragma once

#define UNS_HEADER_NEURONETWORKS "neuronetworks.h"

#include <iostream>
#include <string>
#include <limits>
#include <tuple>
#include <functional>

#include "boost/property_tree/ptree.hpp"

#ifndef UNS_HEADER_MATH
#include "uniself/math.h"
#endif
#ifndef UNS_HEADER_STRINGS
#include "uniself/strings.h"
#endif

#define UNS_DEV_EXCEPTION_MSG std::string{ __FUNCTION__ } + "[" + std::to_string(__LINE__) + "]"

namespace uns::nn {


	//an adress of neuron in every neuronetwork
	class adress {
	public:
		int layer = 0;
		int index = 0;

		bool operator==(const uns::nn::adress& obj) const noexcept {
			return layer == obj.layer && index == obj.index;
		};
		bool operator!=(const uns::nn::adress& obj) const noexcept {
			return !(*this == obj);
		};


		class hash {
		protected:
			std::hash<long long int> subhash;
		public:
			std::size_t operator()(const uns::nn::adress& adress) const noexcept {
				return subhash((static_cast<long long int>(adress.layer) << sizeof(int) * 8) + static_cast<long long int>(adress.index));
			};
		};
	};

	namespace general {
		//this namespace is intended to contain prototypes and interfaces



		//class that introduces a paramaters' container that is fit for the whole network but also for a particular neuron
		template<typename signal_t>
		class network_params {
		public:
			virtual signal_t get(int) const = 0;
			virtual network_params& forward(const std::vector<signal_t>&) const = 0;
		};


		//an interface of general neuron (a preliminary declaration)
		template<typename signal_t>
		class neuron;


		//an interface of input-data provider
		template<typename signal_t>
		class input_data_object {
		public:
			virtual std::size_t size() const = 0;
			virtual neuron<signal_t>* get(const uns::nn::adress&) const = 0;
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
				m_value(std::move(obj.m_value)) {};
			activator& operator=(activator&& obj) noexcept {
				if(this == &obj) return *this;

				m_value = std::move(obj.m_value);

				return *this;
			};
			activator& operator=(signal_t value) noexcept {
				m_value = value;
				return *this;
			};
			~activator() noexcept {};

			const signal_t& value() const noexcept { return m_value; };
			signal_t& value() noexcept { return m_value; };

			virtual std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(signal_t, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dS(signal_t, const network_params<signal_t>&) const { return signal_t(0); };

			virtual signal_t _dp(int, signal_t, const network_params<signal_t>&) const { return signal_t(0); };

			//basic auxiliary class intended to create activators from string
			//	actually it's a map from string to activator
			class caster {
			public:
				virtual std::unique_ptr<uns::nn::general::activator<signal_t>> operator()(const std::u8string_view&) const = 0;
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
				m_value(std::move(obj.m_value)) {};
			collector& operator=(collector&& obj) noexcept {
				if(this == &obj) return *this;

				m_value = std::move(obj.m_value);

				return *this;
			};
			collector& operator=(signal_t value) noexcept {
				m_value = value;
				return *this;
			};
			~collector() noexcept {};

			const signal_t& value() const noexcept { return m_value; };
			signal_t& value() noexcept { return m_value; };

			virtual std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dr(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dw(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			virtual signal_t _dp(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params<signal_t>&) { return signal_t(0); };

			//basic auxiliary class intended to create collector from string
			//	actually it's a map from string to collector
			class caster {
			public:
				virtual std::unique_ptr<uns::nn::general::collector<signal_t>> operator()(const std::u8string_view&) const = 0;
			};
		};


		//an interface of general neuron
		template<typename signal_t>
		class neuron {
		public:
			using repr_type = boost::property_tree::ptree;
			using signal_type = signal_t;

			virtual std::u8string type() const noexcept = 0;

			virtual repr_type represent() const noexcept = 0;
			virtual void set(const repr_type&, const uns::nn::adress&, const typename uns::nn::general::activator<signal_type>::caster&, const typename uns::nn::general::collector<signal_type>::caster&) = 0;
			virtual void link(std::vector<std::vector<uns::nn::general::neuron<signal_type>*>>&, std::unordered_map<uns::nn::adress, uns::nn::general::neuron<signal_type>*, uns::nn::adress::hash>&) = 0;

			virtual int subneurons_total() const noexcept { return 0; };
			virtual const neuron* subneuron(int connection_idx) const noexcept { return nullptr; };
			virtual neuron* subneuron(int connection_idx) noexcept { return nullptr; };

			virtual signal_type R() const noexcept { return signal_type{ 0 }; };
			virtual void R(signal_type) noexcept {};
			virtual signal_type C() const noexcept { return signal_type{ 0 }; };
			virtual void C(signal_type) noexcept {};

			virtual uns::nn::adress adress() const noexcept = 0;
			virtual bool is_reversible() const noexcept { return false; };
			virtual signal_type dropout() const noexcept { return signal_type{ 0 }; };
			virtual void dropout(signal_type) noexcept {};

			virtual void react(const network_params<signal_type>&) {};
			virtual void collect(const uns::nn::general::network_params<signal_t>& common_params) {};
		};


		//an interface of general neuro network
		template<typename neuron_t>
		class network {
		public:
			using repr_type = boost::property_tree::ptree;
			using signal_type = typename neuron_t::signal_type;
		protected:
			std::vector<uns::nn::general::neuron<signal_type>*> m_outputs;
			std::vector<uns::nn::general::neuron<signal_type>*> m_inputs;	//TODO here was an std::vector<input_neuron<signal_t>*> m_inputs
		public:
			virtual repr_type represent() const noexcept = 0;
			virtual void set(const repr_type&, const typename uns::nn::general::activator<signal_type>::caster&, const typename uns::nn::general::collector<signal_type>::caster&) = 0;
			virtual void link(const uns::nn::general::input_data_object<signal_type>&) = 0;
			virtual void react(const network_params<signal_type>&) {};

		};
	};


	//a class of neuron introduces the neurons supposed to consist sequential neural network's body
	template<class signal_t>
	//using signal_t = double;
	class sequential_neuron : public uns::nn::general::neuron<signal_t> {
	protected:
		using signal_type = signal_t;
		using weight_type = signal_t;
		using param_type = signal_t;
		using base = uns::nn::general::neuron<signal_t>;
	public:

		enum {
			neuron = 0,
			weight = 1
		};
	protected:
		uns::nn::adress m_adress;
		std::unique_ptr<uns::nn::general::activator<signal_type>> m_F = nullptr;
		std::unique_ptr<uns::nn::general::collector<signal_type>> m_S = nullptr;
		std::vector<std::pair<uns::nn::general::neuron<signal_type>*, weight_type>> m_links;
		std::vector<param_type> m_params;

		std::unique_ptr<std::vector<std::pair<uns::nn::adress, weight_type>>> m_adresses = nullptr;
	public:
		sequential_neuron() noexcept : m_F(nullptr), m_S(nullptr), m_adresses(nullptr) {};
		sequential_neuron(const uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron& operator=(const uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron(uns::nn::sequential_neuron<signal_t>&&) = delete;
		sequential_neuron& operator=(uns::nn::sequential_neuron<signal_t>&&) = delete;
		~sequential_neuron() noexcept {};

		uns::nn::general::neuron<signal_type>::repr_type represent() const noexcept override {
			using neuron_type = uns::nn::sequential_neuron<signal_type>;

			auto res = typename neuron_type::repr_type{};

			res.put("type", uns::string::u8_cast<std::string>(type()));

			res.put("R", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(R())));
			res.put("C", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(C())));

			res.put("links.total", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(m_links.size())));

			int idx = 0;
			for(auto link : m_links) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".layer"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_type::neuron>(link)->adress().layer
						)
					)
				);

				res.put(
					uns::string::u8_cast<std::string>(
						u8"links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".index"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_type::neuron>(link)->adress().index
						)
					)
				);

				res.put(
					uns::string::u8_cast<std::string>(
						u8"links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".weight"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_type::weight>(link)
						)
					)
				);

				++idx;
			};

			res.put("params.total", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(m_params.size())));

			idx = 0;
			for(auto param : m_params) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"params._"
						+ uns::string::u8_cast<std::u8string>(idx)
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							param
						)
					)
				);

				++idx;
			};

			return res;
		};

		void set(
			const uns::nn::general::neuron<signal_type>::repr_type& repr,
			const uns::nn::adress& adress,
			const typename uns::nn::general::activator<signal_type>::caster& activator_cast,
			const typename uns::nn::general::collector<signal_type>::caster& collector_cast
		) override {
			using neuron_type = uns::nn::sequential_neuron<signal_type>;

			m_adress = adress;

			{
				auto type = std::u8string{};
				try {
					type = uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("type")
					);
				}
				catch(...) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};

				auto activator_type = std::u8string{};
				auto collector_type = std::u8string{};

				auto seeker = type.begin();
				uns::string::seeker_read<std::u8string>(type, seeker, activator_type, u8".", false, -1);

				collector_type = type.substr(seeker - type.begin());

				m_F = activator_cast(activator_type);
				if(m_F == nullptr) throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);

				m_S = collector_cast(collector_type);
				if(m_S == nullptr) throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			};

			try {
				*m_F = uns::string::u8_cast<signal_type>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("R")
					)
				);
			}
			catch(...) {
				throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			};

			try {
				*m_S = uns::string::u8_cast<signal_type>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("C")
					)
				);
			}
			catch(...) {
				throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			};

			int idx = 0;
			{
				int params_total = 0;
				try {
					params_total = uns::string::u8_cast<int>(
						uns::string::u8_cast<std::u8string>(
							repr.get<std::string>("params.total")
						)
					);
				}
				catch(...) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};

				for(idx = 0; idx < params_total; idx++) {
					try {
						m_params.push_back(
							uns::string::u8_cast<signal_type>(
								uns::string::u8_cast<std::u8string>(
									repr.get<std::string>(
										uns::string::u8_cast<std::string>(
											u8"params._"
											+ uns::string::u8_cast<std::u8string>(idx)
										)
									)
								)
							)
						);
					}
					catch(...) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
				};
			};

			m_adresses = std::unique_ptr<std::vector<std::pair<uns::nn::adress, weight_type>>>(new std::vector<std::pair<uns::nn::adress, weight_type>>{});
			{
				int links_total = 0;
				try {
					links_total = uns::string::u8_cast<int>(
						uns::string::u8_cast<std::u8string>(
							repr.get<std::string>("links.total")
						)
					);
				}
				catch(...) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};

				auto link = std::pair<uns::nn::adress, weight_type>{};

				for(idx = 0; idx < links_total; idx++) {
					try {
						std::get<neuron_type::neuron>(link).layer = uns::string::u8_cast<int>(
							uns::string::u8_cast<std::u8string>(
								repr.get<std::string>(
									uns::string::u8_cast<std::string>(
										u8"links._"
										+ uns::string::u8_cast<std::u8string>(idx)
										+ u8".layer"
									)
								)
							)
						);
					}
					catch(...) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
					try {
						std::get<neuron_type::neuron>(link).index = uns::string::u8_cast<int>(
							uns::string::u8_cast<std::u8string>(
								repr.get<std::string>(
									uns::string::u8_cast<std::string>(
										u8"links._"
										+ uns::string::u8_cast<std::u8string>(idx)
										+ u8".index"
									)
								)
							)
						);
					}
					catch(...) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
					try {
						std::get<neuron_type::weight>(link) = uns::string::u8_cast<weight_type>(
							uns::string::u8_cast<std::u8string>(
								repr.get<std::string>(
									uns::string::u8_cast<std::string>(
										u8"links._"
										+ uns::string::u8_cast<std::u8string>(idx)
										+ u8".weight"
									)
								)
							)
						);
					}
					catch(...) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};

					m_adresses->push_back(link);
				};
			};
		};

		void link(
			std::vector<std::vector<uns::nn::general::neuron<signal_t>*>>& main_body,
			std::unordered_map<uns::nn::adress, uns::nn::general::neuron<signal_type>*, uns::nn::adress::hash>& inputs
		) override {
			using neuron_type = uns::nn::sequential_neuron<signal_type>;

			if(m_adresses == nullptr) throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);

			m_links.clear();

			for(const auto& adress : *m_adresses) {
				try {
					auto link = std::pair<uns::nn::general::neuron<signal_t>*, weight_type>{};
					if(std::get<neuron_type::neuron>(adress).layer < 0) {
						std::get<neuron_type::neuron>(link) = inputs.at(adress);
					}
					else {
						std::get<neuron_type::neuron>(link) = main_body.at(std::get<neuron_type::neuron>(adress).layer).at(std::get<neuron_type::neuron>(adress).index);
					};
					std::get<neuron_type::weight>(link) = std::get<neuron_type::weight>(adress);
					m_links.push_back(link);
				}
				catch(std::out_of_range&) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};
			};

			m_adresses = nullptr;
		};

		std::u8string type() const noexcept override { return m_F->type() + u8"." + m_S->type(); };

		uns::nn::adress adress() const noexcept override { return m_adress; };

		int subneurons_total() const noexcept override { return m_links.size(); };
		const uns::nn::general::neuron<signal_t>* subneuron(int connection_idx) const noexcept override {
			using neuron_type = uns::nn::sequential_neuron<signal_type>;

			if(connection_idx >= 0 && connection_idx < m_links.size()) {
				return std::get<neuron_type::neuron>(m_links[connection_idx]);
			}
			else {
				return nullptr;
			};
		};
		uns::nn::general::neuron<signal_t>* subneuron(int connection_idx) noexcept override {
			using neuron_type = uns::nn::sequential_neuron<signal_type>;

			if(connection_idx >= 0 && connection_idx < m_links.size()) {
				return std::get<neuron_type::neuron>(m_links[connection_idx]);
			}
			else {
				return nullptr;
			};
		};

		signal_t R() const noexcept override { return m_F->value(); };
		void R(const signal_t R) noexcept override { *m_F = R; };

		signal_t C() const noexcept { return m_S->value(); };
		void C(const signal_t c) noexcept { *m_S = c; };

		void react(const uns::nn::general::network_params<signal_t>& common_params) override { (*m_F)(m_S->value(), common_params.forward(m_params)); };

		void collect(const uns::nn::general::network_params<signal_t>& common_params) override { (*m_S)(m_links, common_params.forward(m_params)); };
	};


	//a class of neuron introduces the nonrecursive reversation neuron used for learning by gradient-down method
	template<typename signal_t>
	//using signal_t = double;
	class nonrecursive_reverse_neuron : public uns::nn::sequential_neuron<signal_t> {
	protected:
		using base = uns::nn::sequential_neuron<signal_t>;
		using place_type = int;
	public:
		enum {
			neuron = 0,
			place = 1
		};
	protected:
		class set_interruption {};

		base::signal_type m_r = 0;
		base::signal_type m_s = 0;
		std::vector<std::pair<nonrecursive_reverse_neuron<base::signal_type>*, place_type>> m__links;
		uns::nn::general::neuron<base::signal_type>* m_input = nullptr;

		bool m_is_learning = true;
		base::signal_type m_dropout = base::signal_type{ 0 };
		std::unique_ptr<std::vector<std::pair<uns::nn::adress, place_type>>> m__adresses = nullptr;
	public:
		nonrecursive_reverse_neuron() {};
		nonrecursive_reverse_neuron(const uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron<signal_t>& operator=(const uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron(uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		nonrecursive_reverse_neuron<signal_t>& operator=(uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		~nonrecursive_reverse_neuron() {};

		uns::nn::general::neuron<signal_t>::repr_type represent() const noexcept override {
			using neuron_type = uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>;

			auto res = base::represent();

			res.put("_R", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(_R())));
			res.put("_C", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(_C())));

			res.put("_links.total", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(m__links.size() + (m_input != nullptr ? 1 : 0)))));

			auto idx = static_cast<int>(0);
			for(auto _link : m__links) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".layer"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_type::neuron>(_link)->adress().layer
						)
					)
				);

				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".index"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_type::neuron>(_link)->adress().index
						)
					)
				);

				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".place"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_type::place>(_link)
						)
					)
				);

				++idx;
			};

			if(m_input != nullptr) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".layer"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							m_input->adress().layer
						)
					)
				);

				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".index"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							m_input->adress().index
						)
					)
				);

				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(-1)
						+ u8".place"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							static_cast<int>(0)	//TODO to enshure that this is right place
						)
					)
				);
			};

			return res;
		};

		void set(
			const uns::nn::general::neuron<signal_t>::repr_type& repr,
			const uns::nn::adress& adress,
			const typename uns::nn::general::activator<signal_t>::caster& activator_cast,
			const typename uns::nn::general::collector<signal_t>::caster& collector_cast
		) override {
			using neuron_type = uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>;

			base::set(repr, adress, activator_cast, collector_cast);

			try {
				m_r = uns::string::u8_cast<signal_t>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("_R")
					)
				);
			}
			catch(...) {
				m_r = typename base::signal_type{};
			};

			try {
				m_s = uns::string::u8_cast<signal_t>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("_C")
					)
				);
			}
			catch(...) {
				m_s = typename base::signal_type{};
			};


			try	{
				m__adresses = std::unique_ptr<std::vector<std::pair<uns::nn::adress, place_type>>>(new std::vector<std::pair<uns::nn::adress, place_type>>{});

				int idx = 0;
				int links_total = 0;
				try {
					links_total = uns::string::u8_cast<int>(
						uns::string::u8_cast<std::u8string>(
							repr.get<std::string>("_links.total")
						)
					);
				}
				catch(...) {
					throw set_interruption{};
				};

				auto link = std::pair<uns::nn::adress, place_type>{};

				for(idx = 0; idx < links_total; idx++) {
					try {
						std::get<neuron_type::neuron>(link).layer = uns::string::u8_cast<int>(
							uns::string::u8_cast<std::u8string>(
								repr.get<std::string>(
									uns::string::u8_cast<std::string>(
										u8"_links._"
										+ uns::string::u8_cast<std::u8string>(idx)
										+ u8".layer"
									)
								)
							)
						);
					}
					catch(...) {
						throw set_interruption{};
					};
					try {
						std::get<neuron_type::neuron>(link).index = uns::string::u8_cast<int>(
							uns::string::u8_cast<std::u8string>(
								repr.get<std::string>(
									uns::string::u8_cast<std::string>(
										u8"_links._"
										+ uns::string::u8_cast<std::u8string>(idx)
										+ u8".index"
									)
								)
							)
						);
					}
					catch(...) {
						throw set_interruption{};
					};
					try {
						std::get<neuron_type::place>(link) = uns::string::u8_cast<place_type>(
							uns::string::u8_cast<std::u8string>(
								repr.get<std::string>(
									uns::string::u8_cast<std::string>(
										u8"_links._"
										+ uns::string::u8_cast<std::u8string>(idx)
										+ u8".place"
									)
								)
							)
						);
					}
					catch(...) {
						throw set_interruption{};
					};

					m__adresses->push_back(link);
				};
			}
			catch(set_interruption&) {
				m__adresses = nullptr;
			};
		};

		void _link(
			std::vector<std::vector<uns::nn::nonrecursive_reverse_neuron<signal_t>*>>& main_body,
			std::vector<uns::nn::sequential_neuron<signal_t>*>& reverse_inputs
		) {
			using neuron_type = uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>;

			if(m__adresses != nullptr) {
				m__links.clear();

				for(const auto& _adress : *m__adresses) {
					try {
						auto link = std::pair<uns::nn::general::neuron<signal_t>*, place_type>{};
						if(std::get<neuron_type::neuron>(_adress).layer < 0) {
							for(auto reverse_input_ptr : reverse_inputs) {
								if(reverse_input_ptr->adress() == _adress) {
									m_input = reverse_input_ptr;
									break;
								};
							};
						}
						else {
							std::get<neuron_type::neuron>(_adress) = main_body.at(std::get<neuron_type::neuron>(_adress).layer).at(std::get<neuron_type::neuron>(_adress).index);
							std::get<neuron_type::place>(link) = std::get<neuron_type::place>(_adress);
							m__links.push_back(link);
						};
					}
					catch(std::out_of_range&) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
				};

				m__adresses = nullptr;
			}
			else {
				m__links.clear();

				for(const auto& layer : main_body) {
					for(const auto& neuron_ptr : layer) {
						if(neuron_ptr == nullptr) {
							throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
						};

						for(int subneuron_idx = 0; subneuron_idx < neuron_ptr->subneurons_total(); ++subneuron_idx) {
							if(neuron_ptr->subneuron(subneuron_idx)->adress() == this->adress()) {
								m__links.push_back(std::pair<nonrecursive_reverse_neuron<signal_t>*, place_type>{ neuron_ptr, subneuron_idx });
							};
						};
					};
				};

				for(auto reverse_input_ptr : reverse_inputs) {
					if(reverse_input_ptr == nullptr) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};

					for(int subneuron_idx = 0; subneuron_idx < reverse_input_ptr->subneurons_total(); ++subneuron_idx) {
						if(reverse_input_ptr->subneuron(subneuron_idx)->adress() == this->adress()) {
							m_input = reverse_input_ptr;
						};
					};
				};
			};
		};

		bool is_reversible() const noexcept override { return true; };

		signal_t dropout() const noexcept override { return m_dropout; };
		void dropout(signal_t new_dropout) noexcept override { m_dropout = (uns::math::more(new_dropout, signal_t(1)) ? signal_t(1) : (uns::math::less(new_dropout, signal_t(0)) ? signal_t(0) : new_dropout)); };

		signal_t _R() const noexcept { return m_r; };

		void _R(const signal_t R) noexcept { m_r = R; };

		signal_t _C() const noexcept { return m_s; };

		void _C(const signal_t c) noexcept { m_s = c; };

		void _react(const uns::nn::general::network_params<signal_t>& common_params) {
			m_r = dF_dS(common_params) * m_s;
		};

		void _collect(const uns::nn::general::network_params<signal_t>& common_params) {
			using neuron_type = uns::nn::nonrecursive_reverse_neuron<typename base::signal_type>;

			m_s = 0;
			for(auto _link : m__links) {
				m_s += std::get<neuron_type::neuron>(_link)->m_r * std::get<neuron_type::neuron>(_link)->dS_dr(std::get<neuron_type::place>(_link), common_params);
			};
			if(m_input != nullptr) {
				m_s += m_input->R();
			};
		};

		bool is_learning() const { return m_is_learning; };

		void set_learning(bool islearning) { m_is_learning = islearning; };

		signal_t dS_dr(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base::S->_dr(index, base::m_links, common_params.forward(base::m_params)); };	//TODO to think: forwarding params of this neuron can unintendedly replace params of subneurons

		signal_t dS_dw(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base::S->_dw(index, base::m_links, common_params.forward(base::m_params)); };

		signal_t dS_dp(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base::S->_dp(index, base::m_links, common_params.forward(base::m_params)); };

		signal_t dF_dS(const uns::nn::general::network_params<signal_t>& common_params) const { return base::F->_dS(base::C(), common_params.forward(base::m_params)); };

		signal_t dF_dp(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base::F->_dp(index, base::C(), common_params.forward(base::m_params)); };
	};


	//a class of neuron introduces the neurons supposed to consist sequential neural network's body
	//template<class neuron_t>
	using neuron_t = uns::nn::general::neuron<double>;
	class sequential_base_network : public uns::nn::general::network<neuron_t> {
	public:
		using signal_t = typename neuron_t::signal_type;
		using base = uns::nn::general::network<neuron_t>;
	protected:
		std::vector<std::vector<neuron_t*>> layers;
	public:
		sequential_base_network() {};
		sequential_base_network(const sequential_base_network& net) = delete;
		sequential_base_network& operator=(const sequential_base_network& net) = delete;
		sequential_base_network(sequential_base_network&& net) = delete;
		sequential_base_network& operator=(sequential_base_network&& net) = delete;
		~sequential_base_network() {
			for(auto& layer : layers) {
				for(auto neuron : layer) {
					delete neuron;
				};
			};
			for(auto layer : base::m_inputs) {
				for(auto input : layer) {
					delete input;
				};
			};
		};

		uns::nn::general::network<neuron_t>::repr_type represent() const noexcept override {
			auto res = typename uns::nn::general::network<neuron_t>::repr_type{};
			int idx1 = 0;
			int idx2 = 0;

			res.put("layers.total", layers.size());
			idx1 = 0;
			idx2 = 0;
			for(const auto& layer : layers) {
				res.put(
					"layers._"
					+ uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(idx1))
					+ ".neurons.total",
					layer.size()
				);
				
				for(const auto& neuron : layer) {
					res.add_child(
						"layers._"
						+ uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(idx1))
						+ ".neurons._"
						+ uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(idx2)),
						neuron->represent()
					);

					++idx2;
				};

				++idx1;
			};

			res.put("outputs.total", base::m_outputs.size());
			idx1 = 0;
			for(const auto& output : base::m_outputs) {
				res.put(
					"outputs._"
					+ uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(idx1))
					+ ".layer",
					output->adress().layer
				);
				res.put(
					"outputs._"
					+ uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(idx1))
					+ ".index",
					output->adress().index
				);
				++idx1;
			};

			return res;
		};
		void set(const typename uns::nn::general::network<neuron_t>::repr_type&, const typename uns::nn::general::activator<signal_t>::caster&, const typename uns::nn::general::collector<signal_t>::caster&) override;
		void link(const uns::nn::general::input_data_object<signal_t>&) override;

		void react(const uns::nn::general::network_params<signal_t>& common_params) override {
			for(auto input : base::m_inputs) {
				input->react(common_params);
			};
			for(auto& layer : layers) {
				for(auto neuron : layer) {
					neuron->collect(common_params);
					neuron->react(common_params);
				};
			};
		};

		signal_t R(size_t layer_index, size_t index) const { return layers[layer_index][index]->R(); };

		signal_t C(size_t layer_index, size_t index) const { return layers[layer_index][index]->C(); };

		signal_t O(size_t index) const { return base::m_outputs[index]->R(); };

		signal_t I(size_t index) const { return base::m_inputs[index]->R(); };

		size_t layers_total() const { return layers.size(); };
		size_t neurons_total(size_t index) const { return layers[index].size(); };
		size_t outputs_total() const { return base::m_outputs.size(); };
		size_t inputs_total() const { return base::m_inputs.size(); };
	};

	
};


