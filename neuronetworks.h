#pragma once

#define UNS_HEADER_NEURONETWORKS "neuronetworks.h"

#include <iostream>
#include <string>
#include <limits>
#include <tuple>

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


		//an interface of general neuron (a preliminary declaration)
		template<typename signal_t>
		class neuron;


		//basic activator class can be used as default activator
		template<typename signal_t>
		class activator {
		protected:
			signal_t _value = signal_t(0);
		public:
			activator() noexcept {};
			activator(const activator& obj) noexcept :
				_value(obj._value) {};
			activator& operator=(const activator& obj) noexcept {
				if(this == &obj) return *this;

				_value = obj._value;

				return *this;
			};
			activator(activator&& obj) noexcept :
				_value(std::move(obj._value)) {};
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
			virtual void link(const std::vector<std::vector<uns::nn::general::neuron<signal_type>*>>&, const std::vector<std::vector<uns::nn::general::neuron<signal_type>*>>&) = 0;

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
			std::vector<uns::nn::general::neuron<signal_type>*> _outputs;
			std::vector<std::vector<uns::nn::general::neuron<signal_type>*>> _inputs;	//TODO here was an std::vector<input_neuron<signal_t>*> _inputs
		public:
			virtual repr_type represent() const noexcept;
			virtual void set(const repr_type&, const typename uns::nn::general::activator<signal_type>::caster, const typename uns::nn::general::collector<signal_type>::caster&);
			virtual void link(const uns::nn::general::input_data_object<signal_type>&);
			virtual void react(const network_params<signal_type>&) = 0;

		};
	};


	//a class of input_neuron introduces the neural network's data input
	template<typename signal_t>
	//using signal_t = double;
	class input_neuron : public uns::nn::general::neuron<signal_t> {
	protected:
		signal_t* _value_of_cell = nullptr;
		signal_t r = 0;
		int _num_of_cell = 0;
		signal_t _dropout = signal_t{ 0 };
	public:
		input_neuron() {};
		input_neuron(const input_neuron& inp) = delete;
		input_neuron& operator=(const input_neuron& inp) = delete;
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
			_value_of_cell = ido.get(_num_of_cell);
		};
	};


	//a class of neuron introduces the neurons supposed to consist sequential neural network's body
	template<class signal_t>
	//using signal_t = double;
	class sequential_neuron : public uns::nn::general::neuron<signal_t> {
	protected:
		using weight_t = signal_t;
		using param_t = signal_t;
		using base_t = uns::nn::general::neuron<signal_t>;
	public:

		enum {
			neuron = 0,
			weight = 1
		};
	protected:
		uns::nn::adress _adress;
		std::unique_ptr<uns::nn::general::activator<signal_t>> F;
		std::unique_ptr<uns::nn::general::collector<signal_t>> S;
		std::vector<std::pair<uns::nn::general::neuron<signal_t>*, weight_t>> links;
		std::vector<param_t> _params;

		std::unique_ptr<std::vector<std::pair<uns::nn::adress, weight_t>>> _adresses = nullptr;
	public:
		sequential_neuron() noexcept : F(nullptr), S(nullptr) {};
		sequential_neuron(const uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron& operator=(const uns::nn::sequential_neuron<signal_t>&) = delete;
		sequential_neuron(uns::nn::sequential_neuron<signal_t>&&) = delete;
		sequential_neuron& operator=(uns::nn::sequential_neuron<signal_t>&&) = delete;
		~sequential_neuron() noexcept {};

		uns::nn::general::neuron<signal_t>::repr_type represent() const noexcept override {
			using neuron_t = uns::nn::sequential_neuron<signal_t>;

			auto res = typename uns::nn::general::neuron<signal_t>::repr_type{};

			res.put("type", uns::string::u8_cast<std::string>(type()));

			res.put("R", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(R())));
			res.put("C", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(C())));

			res.put("links.total", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(links.size())));

			auto idx = static_cast<int>(0);
			for(auto link : links) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".layer"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron_t::neuron>(link)->adress().layer
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
							std::get<neuron_t::neuron>(link)->adress().index
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
							std::get<neuron_t::weight>(link)
						)
					)
				);

				++idx;
			};

			res.put("params.total", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(_params.size())));

			idx = 0;
			for(auto param : _params) {
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
			const uns::nn::general::neuron<signal_t>::repr_type& repr,
			const uns::nn::adress& adress,
			const typename uns::nn::general::activator<signal_t>::caster& activator_cast,
			const typename uns::nn::general::collector<signal_t>::caster& collector_cast
		) override {
			_adress = adress;

			{
				auto type = std::u8string{};
				try {
					auto type = uns::string::u8_cast<std::u8string>(
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

				F = activator_cast(activator_type);
				if(F == nullptr) throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);

				S = collector_cast(collector_type);
				if(S == nullptr) throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			};

			try {
				*F = uns::string::u8_cast<signal_t>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("R")
					)
				);
			}
			catch(...) {
				throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
			};

			try {
				*S = uns::string::u8_cast<signal_t>(
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
				int _params_total = 0;
				try {
					_params_total = uns::string::u8_cast<int>(
						uns::string::u8_cast<std::u8string>(
							repr.get<std::string>("params.total")
						)
					);
				}
				catch(...) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};

				for(idx = 0; idx < _params_total; idx++) {
					try {
						_params.push_back(
							uns::string::u8_cast<signal_t>(
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

			_adresses = std::unique_ptr<std::vector<std::pair<uns::nn::adress, weight_t>>>(new std::vector<std::pair<uns::nn::adress, weight_t>>{});
			{
				int _links_total = 0;
				try {
					_links_total = uns::string::u8_cast<int>(
						uns::string::u8_cast<std::u8string>(
							repr.get<std::string>("links.total")
						)
					);
				}
				catch(...) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};

				auto link = std::pair<uns::nn::adress, weight_t>{};

				for(idx = 0; idx < _links_total; idx++) {
					try {
						link.first.layer = uns::string::u8_cast<int>(
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
						link.first.index = uns::string::u8_cast<int>(
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
						link.second = uns::string::u8_cast<weight_t>(
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

					_adresses->push_back(link);
				};
			};
		};

		void link(
			const std::vector<std::vector<uns::nn::general::neuron<signal_t>*>>& main_body,
			const std::vector<std::vector<uns::nn::general::neuron<signal_t>*>>& inputs_
		) override {
			if(_adresses == nullptr) throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);

			links.clear();

			for(const auto& adress : *_adresses) {
				try {
					auto link = std::pair<uns::nn::general::neuron<signal_t>*, weight_t>{};
					if(adress.first.layer < 0) {
						link.first = inputs_.at(-adress.first.layer - 1).at(adress.first.index);
					}
					else {
						link.first = main_body.at(adress.first.layer).at(adress.first.index);
					};
					link.second = adress.second;
					links.push_back(link);
				}
				catch(std::out_of_range&) {
					throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
				};
			};

			_adresses = nullptr;
		};

		std::u8string type() const noexcept override { return F->type() + u8"." + S->type(); };

		uns::nn::adress adress() const noexcept override { return _adress; };

		int subneurons_total() const noexcept override { return links.size(); };
		const uns::nn::general::neuron<signal_t>* subneuron(int connection_idx) const noexcept override {
			if(connection_idx >= 0 && connection_idx < links.size()) {
				return links[connection_idx].first;
			}
			else {
				return nullptr;
			};
		};
		uns::nn::general::neuron<signal_t>* subneuron(int connection_idx) noexcept override {
			if(connection_idx >= 0 && connection_idx < links.size()) {
				return links[connection_idx].first;
			}
			else {
				return nullptr;
			};
		};

		signal_t R() const noexcept override { return F->value(); };
		void R(const signal_t R) noexcept override { *F = R; };

		signal_t C() const noexcept { return S->value(); };
		void C(const signal_t c) noexcept { *S = c; };

		void react(const uns::nn::general::network_params<signal_t>& common_params) override { (*F)(S->value(), common_params.forward(_params)); };

		void collect(const uns::nn::general::network_params<signal_t>& common_params) override { (*S)(links, common_params.forward(_params)); };
	};

	//a class of neuron introduces the nonrecursive reversation neuron used for learning by gradient-down method
	template<typename signal_t>
	//using signal_t = double;
	class nonrecursive_reverse_neuron : public uns::nn::sequential_neuron<signal_t> {
	protected:
		using base_t = uns::nn::sequential_neuron<signal_t>;
		using place_t = int;
	public:
		enum {
			neuron = 0,
			place = 1
		};
	protected:
		class set_interruption {};

		signal_t _r = 0;
		signal_t _s = 0;
		std::vector<std::pair<nonrecursive_reverse_neuron<signal_t>*, place_t>> _links;
		uns::nn::general::neuron<signal_t>* _input = nullptr;

		bool _is_learning = true;
		signal_t _dropout = signal_t{ 0 };
		std::unique_ptr<std::vector<std::pair<uns::nn::adress, place_t>>> __adresses = nullptr;
	public:
		nonrecursive_reverse_neuron() {};
		nonrecursive_reverse_neuron(const uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron<signal_t>& operator=(const uns::nn::nonrecursive_reverse_neuron<signal_t>&) = delete;
		nonrecursive_reverse_neuron(uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		nonrecursive_reverse_neuron<signal_t>& operator=(uns::nn::nonrecursive_reverse_neuron<signal_t>&&) = delete;
		~nonrecursive_reverse_neuron() {};

		uns::nn::general::neuron<signal_t>::repr_type represent() const noexcept override {
			using neuron_t = uns::nn::nonrecursive_reverse_neuron<signal_t>;

			auto res = base_t::represent();

			res.put("_R", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(_R())));
			res.put("_C", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(_C())));

			res.put("_links.total", uns::string::u8_cast<std::string>(uns::string::u8_cast<std::u8string>(_links.size() + (_input != nullptr ? 1 : 0)))));

			auto idx = static_cast<int>(0);
			for(auto _link : _links) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".layer"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							std::get<neuron>(_link)->adress().layer
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
							std::get<neuron>(_link)->adress().index
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
							std::get<place>(_link)
						)
					)
				);

				++idx;
			};

			if(_input != nullptr) {
				res.put(
					uns::string::u8_cast<std::string>(
						u8"_links._"
						+ uns::string::u8_cast<std::u8string>(idx)
						+ u8".layer"
					),
					uns::string::u8_cast<std::string>(
						uns::string::u8_cast<std::u8string>(
							static_cast<int>(_input->adress().layer)
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
							static_cast<int>(_input->adress().index)
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
			base_t::set(repr, adress, activator_cast, collector_cast);

			try {
				_r = uns::string::u8_cast<signal_t>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("_R")
					)
				);
			}
			catch(...) {
				_r = signal_t{};
			};

			try {
				_s = uns::string::u8_cast<signal_t>(
					uns::string::u8_cast<std::u8string>(
						repr.get<std::string>("_C")
					)
				);
			}
			catch(...) {
				_s = signal_t{};
			};


			try	{
				__adresses = std::unique_ptr<std::vector<std::pair<uns::nn::adress, place_t>>>(new std::vector<std::pair<uns::nn::adress, place_t>>{});

				int idx = 0;
				int _links_total = 0;
				try {
					_links_total = uns::string::u8_cast<int>(
						uns::string::u8_cast<std::u8string>(
							repr.get<std::string>("_links.total")
						)
					);
				}
				catch(...) {
					throw set_interruption{};
				};

				auto link = std::pair<uns::nn::adress, place_t>{};

				for(idx = 0; idx < _links_total; idx++) {
					try {
						link.first.layer = uns::string::u8_cast<int>(
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
						link.first.index = uns::string::u8_cast<int>(
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
						link.second = uns::string::u8_cast<place_t>(
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

					__adresses->push_back(link);
				};
			}
			catch(set_interruption&) {
				__adresses = nullptr;
			};
		};

		void link(
			const std::vector<std::vector<uns::nn::general::neuron<signal_t>*>>& main_body,
			const std::vector<std::vector<uns::nn::general::neuron<signal_t>*>>& reverse_inputs
		) override {
			base_t::link(main_body(), reverse_inputs);

			if(__adresses != nullptr) {
				_links.clear();

				for(const auto& _adress : *__adresses) {
					try {
						auto link = std::pair<uns::nn::general::neuron<signal_t>*, place_t>{};
						if(_adress.first.layer < 0) {
							_input = reverse_inputs.at(_adress.first.index);
						}
						else {
							link.first = main_body.at(_adress.first.layer).at(_adress.first.index);
						};
						link.second = _adress.second;
						_links.push_back(link);
					}
					catch(std::out_of_range&) {
						throw std::runtime_error(UNS_DEV_EXCEPTION_MSG);
					};
				};

				__adresses = nullptr;
			}
			else {
				_links.clear();

				for(const auto& layer : main_body) {
					for(const auto& neuron_ptr : layer) {
						for(int subneuron_idx = 0; subneuron_idx < neuron_ptr->subneurons_total(); ++subneuron_idx) {
							if(neuron_ptr->subneuron(subneuron_idx)->adress() == this->adress()) {
								_links.push_back(std::pair<nonrecursive_reverse_neuron<signal_t>*, place_t>{ neuron_ptr, subneuron_idx });
							};
						};
					};
				};

				for(const auto& reverse_input_ptr : reverse_inputs) {
					for(int subneuron_idx = 0; subneuron_idx < reverse_input_ptr->subneurons_total(); ++subneuron_idx) {
						if(reverse_input_ptr->subneuron(subneuron_idx)->adress() == this->adress()) {
							_input = reverse_input_ptr;
						};
					};
				};
			};
		};

		bool is_reversible() const noexcept override { return true; };

		signal_t dropout() const noexcept override { return _dropout; };
		void dropout(signal_t new_dropout) noexcept override { _dropout = (uns::math::more(new_dropout, signal_t(1)) ? signal_t(1) : (uns::math::less(new_dropout, signal_t(0)) ? signal_t(0) : new_dropout)); };

		signal_t _R() const noexcept { return _r; };

		void _R(const signal_t R) noexcept { _r = R; };

		signal_t _C() const noexcept { return _s; };

		void _C(const signal_t c) noexcept { _s = c; };

		void _react(const uns::nn::general::network_params<signal_t>& common_params) {
			_r = dF_dS(common_params) * _s;
		};

		void _collect(const uns::nn::general::network_params<signal_t>& common_params) {
			_s = 0;
			for(auto _link : _links) {
				_s += std::get<neuron>(_link)->_r * std::get<neuron>(_link)->dS_dr(std::get<place>(_link), common_params);
			};
			if(_input != nullptr) {
				_s += _input->R();
			};
		};

		bool is_learning() const { return _is_learning; };

		void set_learning(bool islearning) { _is_learning = islearning; };

		signal_t dS_dr(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base_t::S->_dr(index, base_t::links, common_params.forward(base_t::_params)); };	//TODO to think: forwarding params of this neuron can unintendedly replace params of subneurons

		signal_t dS_dw(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base_t::S->_dw(index, base_t::links, common_params.forward(base_t::_params)); };

		signal_t dS_dp(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base_t::S->_dp(index, base_t::links, common_params.forward(base_t::_params)); };

		signal_t dF_dS(const uns::nn::general::network_params<signal_t>& common_params) const { return base_t::F->_dS(base_t::C(), common_params.forward(base_t::_params)); };

		signal_t dF_dp(int index, const uns::nn::general::network_params<signal_t>& common_params) const { return base_t::F->_dp(index, base_t::C(), common_params.forward(base_t::_params)); };
	};


	//a class of neuron introduces the neurons supposed to consist sequential neural network's body
	template<class neuron_t>
	//using neuron_t = uns::nn::general::neuron<double>;
	class sequential_base_network : public uns::nn::general::network<neuron_t> {
	public:
		using signal_t = typename neuron_t::signal_type;
		using base_t = uns::nn::general::network<neuron_t>;
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
			for(auto input : base_t::_inputs) {
				delete input;
			};
		};

		uns::nn::general::network<neuron_t>::repr_type represent() const noexcept override;
		void set(const uns::nn::general::network<neuron_t>::repr_type&, const typename uns::nn::general::activator<signal_t>::caster&, const typename uns::nn::general::collector<signal_t>::caster&) override;
		void link(const uns::nn::general::input_data_object<signal_t>&) override;

		void react(const uns::nn::general::network_params<signal_t>& common_params) override {
			for(auto input : base_t::_inputs) {
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

		signal_t O(size_t index) const { return base_t::_outputs[index]->R(); };

		signal_t I(size_t index) const { return base_t::_inputs[index]->R(); };

		size_t layers_total() const { return layers.size(); };
		size_t neurons_total(size_t index) const { return layers[index].size(); };
		size_t outputs_total() const { return base_t::_outputs.size(); };
		size_t inputs_total() const { return base_t::_inputs.size(); };
	};

	
};


