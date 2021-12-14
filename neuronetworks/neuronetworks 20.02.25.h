#pragma once

#include <vector>

#include "uniself/strings.h"
#include "uniself/mathematics.h"

namespace uns {
	namespace nn {

#ifdef UNS_NN_STRING_T_CHAR
		using string_t = std::string;
#define UNS_NN_LITERAL(str_literal) str_literal
#endif
#ifdef UNS_NN_STRING_T_WCHAR
		using string_t = std::wstring;
#define UNS_NN_LITERAL(str_literal) L##str_literal 
#endif


		template<typename signal_t>
		class input_neuron;


		template<class signal_t>
		class nonrecursive_neuron;


		template<class signal_t>
		class nonrecursive_reverse_neuron;


		template<class neuron_t>
		class nonrecursive_base_network;


		template<typename signal_t>
		class nonrecursive_reverse_network;


		template<typename signal_t>
		class input_data_object_interface {
		public:
			virtual signal_t* Get(size_t index) const = 0;
			virtual int Size() const = 0;
		};


		template<typename signal_t>
		class neuron_interface {
		public:
			virtual string_t Type() const noexcept = 0;
			virtual signal_t R() const noexcept = 0;
			virtual void R(const signal_t R) noexcept = 0;
			virtual int Index() const noexcept = 0;
			virtual bool IsReversable() const noexcept = 0;
			virtual signal_t Dropout() const noexcept = 0;
			virtual void React(const std::vector<signal_t>& common_params) = 0;
		};


		template<typename signal_t>
		class network_interface {
		protected:
			std::vector<neuron_interface<signal_t>*> outputs;
			std::vector<input_neuron<signal_t>*> inputs;
		public:
			virtual void React() = 0;

		};


		template<typename signal_t>
		class activator {
		protected:
		public:
			activator() {};
			activator(const activator<signal_t>& a) {};
			activator<signal_t>& operator=(const activator<signal_t>& a) { return *this; };
			activator(activator<signal_t>&& a) {};
			activator<signal_t>& operator=(activator<signal_t>&& a) { return *this; };
			~activator() {};
			virtual uns::nn::string_t Type() const noexcept { return uns::unified_cast<string_t>(UNS_NN_LITERAL("Zero")); };
			virtual signal_t operator()(const signal_t S, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
			virtual signal_t _dS(const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
			virtual signal_t _dp(size_t index, const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
		};


		template<typename signal_t>
		uns::nn::activator<signal_t>* StringToActivator(const uns::nn::string_t& str);


		template<typename signal_t>
		class collector {
		protected:
		public:
			collector() {};
			collector(const collector<signal_t>& c) {};
			collector<signal_t>& operator=(const collector<signal_t>& c) { return *this; };
			collector(collector<signal_t>&& c) {};
			collector<signal_t>& operator=(collector<signal_t>&& c) { return *this; };
			~collector() {};
			virtual uns::nn::string_t Type() const noexcept { return uns::unified_cast<string_t>(UNS_NN_LITERAL("Zero")); };
			virtual signal_t operator()(std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
			virtual signal_t _dr(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
			virtual signal_t _dw(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
			virtual signal_t _dp(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
		};


		template<typename signal_t>
		uns::nn::collector<signal_t>* StringToCollector(const uns::nn::string_t& str);


		template<typename signal_t>
		class neuron_representation {
		public:
			using neuron_signal_t = signal_t;

			signal_t r = 0;
			signal_t s = 0;
			std::vector<std::pair<int, signal_t>> links;
			std::vector<signal_t> params;
		public:
			neuron_representation() {};
			neuron_representation(const string_t& str) { Set(str); };
			neuron_representation(const neuron_representation<signal_t>& repr) noexcept : r(repr.r), s(repr.s), links(repr.links), params(repr.params) {};
			neuron_representation<signal_t>& operator=(const neuron_representation<signal_t>& repr) {
				r = repr.r;
				s = repr.s;
				links = repr.links;
				params = repr.params;
				return *this;
			};
			neuron_representation(neuron_representation<signal_t>&& repr) noexcept : r(repr.r), s(repr.s), links(std::move(repr.links)), params(std::move(repr.params)) {};
			neuron_representation<signal_t>& operator=(neuron_representation<signal_t>&& repr) {
				r = std::move(repr.r);
				s = std::move(repr.s);
				links = std::move(repr.links);
				params = std::move(repr.params);
				return *this;
			};
			~neuron_representation() {};
			virtual string_t String() const noexcept {
				//шаблон строчного представления:
				//[(adress1,weight1;...;adressN,weightN;)param1,...paramN,(R,S)]

				string_t res = UNS_NN_LITERAL("[(");
				for (auto link : links)
					res += uns::unified_cast<string_t>(link.first) + UNS_NN_LITERAL(",") + uns::unified_cast<string_t>(link.second) + UNS_NN_LITERAL(";");
				res += UNS_NN_LITERAL(")");
				for (auto param : params)
					res += uns::unified_cast<string_t>(param) + UNS_NN_LITERAL(",");
				res += UNS_NN_LITERAL("(") + uns::unified_cast<string_t>(r) + UNS_NN_LITERAL(",") + uns::unified_cast<string_t>(s) + UNS_NN_LITERAL(")]");

				return res;
			};
			virtual bool Set(const string_t& str) {
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("[("), false, -1, UNS_NN_LITERAL("]"))) return false;
				string_t temp = L"";
				string_t temp1 = L"";
				links.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(")"))) break;
					else if (!uns::string::SeekerRead<string_t>(str, seeker, temp1, UNS_NN_LITERAL(";"), false, -1, UNS_NN_LITERAL(")"))) break;
					else {
						links.push_back(std::pair<int, signal_t>(uns::unified_cast<int>(temp), uns::unified_cast<signal_t>(temp1)));
					};
				} while (true);

				params.clear();
				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL(")"), false, -1, UNS_NN_LITERAL("]"))) return false;
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL("("))) break;
					else params.push_back(uns::unified_cast<signal_t>(temp));
				} while (true);
				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("("), false, -1, UNS_NN_LITERAL("]"))) return false;
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(")"))) return false;
				else r = uns::unified_cast<signal_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(")"), false, -1, UNS_NN_LITERAL("]"))) return false;
				else s = uns::unified_cast<signal_t>(temp);

				return true;
			};
		};


		template<typename signal_t>
		class reverse_neuron_representation : public neuron_representation<signal_t> {
		protected:
			using base_t = neuron_representation<signal_t>;

			signal_t dropout = 0.0;
		public:
			signal_t _r = 0;
			signal_t _s = 0;
			std::vector<std::pair<int, int>> _links;
			bool is_learning = true;
		public:
			reverse_neuron_representation() {};
			reverse_neuron_representation(const base_t& represent) {
				base_t::r = represent.r;
				base_t::s = represent.s;
				base_t::links = represent.links;
				base_t::params = represent.params;
				_links.clear();
			};
			reverse_neuron_representation(const string_t& str) { Set(str); };
			reverse_neuron_representation(const reverse_neuron_representation<signal_t>& repr) noexcept : _r(repr._r), _s(repr._s), _links(repr._links), is_learning(repr.is_learning) {
				base_t::r = repr.r;
				base_t::s = repr.s;
				base_t::links = repr.links;
				base_t::params = repr.params;
				Dropout(repr.Dropout());
			};
			reverse_neuron_representation<signal_t>& operator=(const reverse_neuron_representation<signal_t>& repr) {
				base_t::links = repr.links;
				base_t::params = repr.params;
				base_t::r = repr.r;
				base_t::s = repr.s;
				_r = repr._r;
				_s = repr._s;
				_links = repr._links;
				is_learning = repr.is_learning;
				Dropout(repr.Dropout());
				return *this;
			};
			reverse_neuron_representation(reverse_neuron_representation<signal_t>&& repr) noexcept : _r(repr._r), _s(repr._s), _links(std::move(repr._links)), is_learning(repr.is_learning) {
				base_t::r = repr.r;
				base_t::s = repr.s;
				base_t::links = std::move(repr.links);
				base_t::params = std::move(repr.params);
				Dropout(repr.Dropout());
			};
			reverse_neuron_representation<signal_t>& operator=(reverse_neuron_representation<signal_t>&& repr) {
				base_t::links = std::move(repr.links);
				base_t::params = std::move(repr.params);
				base_t::r = repr.r;
				base_t::s = repr.s;
				_r = repr._r;
				_s = repr._s;
				_links = std::move(repr._links);
				is_learning = repr.is_learning;
				Dropout(repr.Dropout());
				return *this;
			};
			reverse_neuron_representation<signal_t>& operator=(const neuron_representation<signal_t>& repr) {
				base_t::links = repr.links;
				base_t::params = repr.params;
				base_t::r = repr.r;
				base_t::s = repr.s;
				_r = 0;
				_s = 0;
				_links.clear();
				is_learning = true;
				Dropout(0.0);
				return *this;
			};
			reverse_neuron_representation<signal_t>& operator=(neuron_representation<signal_t>&& repr) {
				base_t::links = std::move(repr.links);
				base_t::params = std::move(repr.params);
				base_t::r = repr.r;
				base_t::s = repr.s;
				_r = 0;
				_s = 0;
				_links.clear();
				is_learning = true;
				Dropout(0.0);
				return *this;
			};
			~reverse_neuron_representation() {};
			virtual string_t String() const noexcept override {
				//шаблон строчного представления:
				//[_(adress1,place1;...;adressN,placeN;)(_R,_S,dropout,is_learning)]

				string_t res = base_t::String();
				res += UNS_NN_LITERAL("[_(");
				for (auto _link : _links) {
					res += uns::unified_cast<string_t>(_link.first) + UNS_NN_LITERAL(",") + uns::unified_cast<string_t>(_link.second) + UNS_NN_LITERAL(";");
				};
				res += UNS_NN_LITERAL(")");
				res += UNS_NN_LITERAL("(") + uns::unified_cast<string_t>(_r)
					+ UNS_NN_LITERAL(",") + uns::unified_cast<string_t>(_s)
					+ UNS_NN_LITERAL(",") + uns::unified_cast<string_t>(Dropout())
					+ UNS_NN_LITERAL(",") + uns::unified_cast<string_t>(is_learning) + UNS_NN_LITERAL(")]");

				return res;
			};
			virtual bool Set(const string_t& str) override {
				if (!base_t::Set(str)) return false;
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("[_("), false, -1)) return false;

				string_t temp = UNS_NN_LITERAL("");
				string_t temp1 = UNS_NN_LITERAL("");
				_links.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(")"))) break;
					else if (!uns::string::SeekerRead<string_t>(str, seeker, temp1, UNS_NN_LITERAL(";"), false, -1, UNS_NN_LITERAL(")"))) break;
					else {
						_links.push_back(std::pair<int, int>(uns::unified_cast<int>(temp), uns::unified_cast<int>(temp1)));
					};
				} while (true);

				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL(")("), false, -1, UNS_NN_LITERAL(")]"))) return false;
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(")"))) return false;
				else _r = uns::unified_cast<signal_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(")"))) return false;
				else _s = uns::unified_cast<signal_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(")"))) return false;
				else Dropout(uns::unified_cast<signal_t>(temp));
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(")"), false, -1, UNS_NN_LITERAL("]"))) return false;
				else is_learning = uns::unified_cast<bool>(temp);

				return true;
			};
			signal_t Dropout() const noexcept { return dropout; };
			void Dropout(signal_t dropout_) noexcept {
				dropout = (uns::math::More(dropout_, 1.0) ? 1.0 : (uns::math::Less(dropout_, 0.0) ? 0.0 : dropout_));
			};
		};


		template<typename neuron_representation_t>
		class layer_representation {
		public:
			string_t activator_type = UNS_NN_LITERAL("Zero");
			string_t collector_type = UNS_NN_LITERAL("Zero");
			std::vector<neuron_representation_t> neurons;

			layer_representation() {};
			layer_representation(const string_t& str, string_t delimiter) { Set(str, delimiter); };
			layer_representation(const layer_representation<neuron_representation_t>& repr) : neurons(repr.neurons), activator_type(repr.activator_type), collector_type(repr.collector_type) {};
			layer_representation<neuron_representation_t>& operator=(const layer_representation<neuron_representation_t>& repr) { neurons = repr.neurons;  activator_type = repr.activator_type; collector_type = repr.collector_type; return *this; };
			layer_representation(layer_representation<neuron_representation_t>&& repr) noexcept : neurons(std::move(repr.neurons)), activator_type(std::move(repr.activator_type)), collector_type(std::move(repr.collector_type)) {};
			layer_representation<neuron_representation_t>& operator=(layer_representation<neuron_representation_t>&& repr) { neurons = std::move(repr.neurons); activator_type = std::move(repr.activator_type); collector_type = std::move(repr.collector_type); return *this; };
			~layer_representation() noexcept {};
			string_t String(string_t delimiter = UNS_NN_LITERAL("\n")) const {
				string_t res = UNS_NN_LITERAL("LAYER ") + activator_type + UNS_NN_LITERAL("__") + collector_type + UNS_NN_LITERAL(":") + delimiter;
				for (auto neuron : neurons)
					res += neuron.String() + delimiter;
				res += UNS_NN_LITERAL(":LAYER");

				return res;
			};
			bool Set(const string_t& str, string_t delimiter) {
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("LAYER "), false, -1)) return false;
				string_t temp = UNS_NN_LITERAL("");
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL("__"), false, -1, UNS_NN_LITERAL(":"))) return false;
				else activator_type = temp;
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(":") + delimiter, false, -1, UNS_NN_LITERAL(":LAYER"))) return false;
				else collector_type = temp;

				neurons.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, delimiter, false, -1, UNS_NN_LITERAL(":LAYER"))) break;
					else neurons.push_back(neuron_representation_t(temp));
				} while (true);

				return true;
			};
		};

		template<typename  neuron_representation_t>
		class network_representation {
		public:
			std::vector<layer_representation<neuron_representation_t>> layers;
			std::vector<size_t> outputs;
			std::vector<typename neuron_representation_t::neuron_signal_t> common_params;
		public:
			network_representation() {};
			network_representation(const string_t& str, string_t delimiter) { Set(str, delimiter); };
			network_representation(const network_representation<neuron_representation_t>& repr) : layers(repr.layers), outputs(repr.outputs), common_params(repr.common_params) {};
			network_representation<neuron_representation_t>& operator=(const network_representation<neuron_representation_t>& repr) { layers = repr.layers; outputs = repr.outputs;  common_params = repr.common_params;  return *this; };
			network_representation(network_representation<neuron_representation_t>&& repr) noexcept : layers(std::move(repr.layers)), outputs(std::move(repr.outputs)), common_params(std::move(repr.common_params)) {};
			network_representation<neuron_representation_t>& operator=(network_representation<neuron_representation_t>&& repr) { layers = std::move(repr.layers); outputs = std::move(repr.outputs); common_params = std::move(repr.common_params); return *this; };
			~network_representation() noexcept {};
			string_t String(string_t delimiter = UNS_NN_LITERAL("\n")) const {
				string_t res = UNS_NN_LITERAL("NETWORK:") + delimiter;
				for (auto layer : layers)
					res += layer.String() + UNS_NN_LITERAL(";") +delimiter;
				res += UNS_NN_LITERAL("OUTPUTS:");
				for (auto output : outputs)
					res += uns::unified_cast<string_t>(output) + UNS_NN_LITERAL(",");
				res += UNS_NN_LITERAL(":OUTPUTS") + delimiter;
				res += UNS_NN_LITERAL("PARAMS:");
				for (auto common_param : common_params)
					res += uns::unified_cast<string_t>(common_param) + UNS_NN_LITERAL(",");
				res += UNS_NN_LITERAL(":PARAMS") + delimiter;
				res += UNS_NN_LITERAL(":NETWORK");

				return res;

			};
			bool Set(const string_t& str, string_t delimiter) {
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("NETWORK:") + delimiter, false, -1)) return false;

				string_t temp = UNS_NN_LITERAL("");
				layers.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(";") + delimiter, false, -1, UNS_NN_LITERAL("OUTPUTS:"))) break;
					else layers.push_back(layer_representation<neuron_representation_t>(temp, delimiter));
				} while (true);

				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("OUTPUTS:"), false, -1)) return false;
				temp = L"";
				outputs.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(":OUTPUTS"))) break;
					else outputs.push_back(uns::unified_cast<size_t>(temp));
				} while (true);

				if (!uns::string::SeekerSet<string_t>(str, seeker, UNS_NN_LITERAL("PARAMS:"), false, -1)) return false;
				temp = UNS_NN_LITERAL("");
				common_params.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, UNS_NN_LITERAL(","), false, -1, UNS_NN_LITERAL(":PARAMS"))) break;
					else common_params.push_back(uns::unified_cast<typename neuron_representation_t::neuron_signal_t>(temp));
				} while (true);

				return true;
			};
		};


		template<typename signal_t>
		network_representation<reverse_neuron_representation<signal_t>> Convert(const network_representation<neuron_representation<signal_t>>& repr) {
			network_representation<reverse_neuron_representation<signal_t>> res;

			res.layers.resize(repr.layers.size());
			size_t layer_number = 0;
			for (auto layer : repr.layers) {
				res.layers[layer_number].activator_type = layer.activator_type;
				res.layers[layer_number].collector_type = layer.collector_type;
				for (auto neuron : layer.neurons)
					res.layers[layer_number].neurons.push_back(neuron);
				layer_number++;
			};
			res.outputs = repr.outputs;
			res.common_params = repr.common_params;

			return res;
		};
		template<typename signal_t>
		network_representation<neuron_representation<signal_t>> Convert(const network_representation<reverse_neuron_representation<signal_t>>& repr) {
			network_representation<neuron_representation<signal_t>> res;

			res.layers.resize(repr.layers.size());
			size_t layer_number = 0;
			for (auto layer : repr.layers) {
				res.layers[layer_number].activator_type = layer.activator_type;
				res.layers[layer_number].collector_type = layer.collector_type;
				for (auto neuron : layer.neurons)
					res.layers[layer_number].neurons.push_back(neuron);
				layer_number++;
			};
			res.outputs = repr.outputs;
			res.common_params = repr.common_params;

			return res;
		};


		template<typename signal_t>
		class input_neuron : public neuron_interface<signal_t> {
		protected:
			signal_t* value_of_cell = nullptr;
			signal_t r = 0;
			size_t num_of_cell = 1;
		public:
			input_neuron() {};
			input_neuron(int num_of_input, const input_data_object_interface<signal_t>& ido) noexcept { Set(num_of_input, ido); };
			input_neuron(const input_neuron<signal_t>& inp) = delete;
			input_neuron<signal_t>& operator=(const input_neuron<signal_t>& inp) = delete;
			input_neuron(input_neuron<signal_t>&& inp) = delete;
			input_neuron<signal_t>& operator=(input_neuron<signal_t>&& inp) = delete;
			~input_neuron() {};
			uns::nn::string_t Type() const noexcept override { return UNS_NN_LITERAL("Input"); };
			signal_t R() const noexcept override { return r; };
			void R(const signal_t R) noexcept override { r = R; };
			int Index() const noexcept override { return -uns::unified_cast<int>(num_of_cell); };
			bool IsReversable() const noexcept override { return false; };
			signal_t Dropout() const noexcept override { return 0.0; };
			void React(const std::vector<signal_t>& common_params) noexcept override { r = *value_of_cell; };
			void Set(int num_of_input, const input_data_object_interface<signal_t>& ido) noexcept {
				if (num_of_input <= -ido.Size() || num_of_input >= 0) num_of_cell = 1;
				else num_of_cell = uns::unified_cast<size_t>(-num_of_input);
				value_of_cell = ido.Get(num_of_cell);
			};
		};


		template<class signal_t>
		class nonrecursive_neuron : public neuron_interface<signal_t> {
			friend nonrecursive_base_network<nonrecursive_neuron<signal_t>>;
			friend nonrecursive_reverse_network<signal_t>;
		protected:
			using weight_t = signal_t;
			using param_t = signal_t;
			using base_t = neuron_interface<signal_t>;

			int index_in_nn = 0;
			signal_t r = 0;
			signal_t s = 0;
			uns::nn::activator<signal_t>* F;
			uns::nn::collector<signal_t>* S;
			std::vector<std::pair<neuron_interface<signal_t>*, weight_t>> links;
			std::vector<param_t> params;
		public:
			using neuron_signal_t = signal_t;

			nonrecursive_neuron() noexcept : F(nullptr), S(nullptr) {};
			nonrecursive_neuron(const uns::nn::nonrecursive_neuron<signal_t>& n) = delete;
			nonrecursive_neuron<signal_t>& operator=(const uns::nn::nonrecursive_neuron<signal_t>& n) = delete;
			nonrecursive_neuron(uns::nn::nonrecursive_neuron<signal_t>&& n) = delete;
			nonrecursive_neuron<signal_t>& operator=(uns::nn::nonrecursive_neuron<signal_t>&& n) = delete;
			~nonrecursive_neuron() noexcept {
				delete F;
				delete S;
			};
			neuron_representation<signal_t> Represent() const;
			uns::nn::string_t Type() const noexcept override { return F->Type() + S->Type(); };
			int Index() const noexcept override { return index_in_nn; };
			void Index(const int i) noexcept { index_in_nn = i; };
			bool IsReversable() const noexcept override { return false; };
			signal_t Dropout() const noexcept override { return 0.0; };
			signal_t R() const noexcept override { return r; };
			void R(const signal_t R) noexcept override { r = R; };
			signal_t C() const noexcept { return s; };
			void C(const signal_t S) noexcept { s = S; };
			void React(const std::vector<signal_t>& common_params) override { r = (*F)(s, params, common_params); };
			void Collect(const std::vector<signal_t>& common_params) { s = (*S)(links, params, common_params); };
		};

		template<typename signal_t>
		inline uns::nn::neuron_representation<signal_t> uns::nn::nonrecursive_neuron<signal_t>::Represent() const {
			uns::nn::neuron_representation<signal_t> res;
			res.r = r;
			res.s = s;
			for (auto link : links) {
				res.links.push_back(std::pair<int, weight_t>(link.first->Index(), link.second));
			};
			for (auto param : params) {
				res.params.push_back(param);
			};

			return res;
		};


		template<typename signal_t>
		class nonrecursive_reverse_neuron : public nonrecursive_neuron<signal_t> {
			friend nonrecursive_base_network<nonrecursive_reverse_neuron<signal_t>>;
			friend nonrecursive_reverse_network<signal_t>;
		protected:
			using base_t = nonrecursive_neuron<signal_t>;

			signal_t _r = 0;
			signal_t _s = 0;
			std::vector<std::pair<nonrecursive_reverse_neuron<signal_t>*, size_t>> _links;
			input_neuron<signal_t>* _input = nullptr;
			bool is_learning = true;
			signal_t dropout = 0.0;
		public:
			nonrecursive_reverse_neuron() {};
			nonrecursive_reverse_neuron(const uns::nn::nonrecursive_reverse_neuron<signal_t>& n) = delete;
			nonrecursive_reverse_neuron<signal_t>& operator=(const uns::nn::nonrecursive_reverse_neuron<signal_t>& n) = delete;
			nonrecursive_reverse_neuron(uns::nn::nonrecursive_reverse_neuron<signal_t>&& n) = delete;
			nonrecursive_reverse_neuron<signal_t>& operator=(uns::nn::nonrecursive_reverse_neuron<signal_t>&& n) = delete;
			~nonrecursive_reverse_neuron() {};
			uns::nn::reverse_neuron_representation<signal_t> _Represent() const;
			bool IsReversable() const noexcept override { return true; };
			signal_t _R() const noexcept { return _r; };
			void _R(const signal_t R) noexcept { _r = R; };
			signal_t _C() const noexcept { return _s; };
			void _C(const signal_t S) noexcept { _s = S; };
			void _React(const std::vector<signal_t>& common_params) {
				_r = dF_dS(common_params) * _s;
			};
			void _Collect(const std::vector<signal_t>& common_params) {
				_s = uns::unified_cast<signal_t>(0);
				for (auto _link : _links)
					_s += _link.first->_r * _link.first->dS_dr(_link.second, common_params);
				if (_input != nullptr)
					_s += _input->R();
			};
			bool IsLearning() const { return is_learning; };
			void SetLearning(bool islearning) { is_learning = islearning; };
			signal_t Dropout() const noexcept override { return dropout; };
			void Dropout(signal_t dropout_) {
				dropout = (uns::math::More(dropout_, 1.0) ? 1.0 : (uns::math::Less(dropout_, 0.0) ? 0.0 : dropout_));
			};
			signal_t dS_dr(size_t index, const std::vector<signal_t>& common_params) const { return base_t::S->_dr(index, base_t::links, base_t::params, common_params); };
			signal_t dS_dw(size_t index, const std::vector<signal_t>& common_param) const { return base_t::S->_dw(index, base_t::links, base_t::params, common_param); };
			signal_t dS_dp(size_t index, const std::vector<signal_t>& common_param) const { return base_t::S->_dp(index, base_t::links, base_t::params, common_param); };
			signal_t dF_dS(const std::vector<signal_t>& common_param) const { return base_t::F->_dS(base_t::s, base_t::params, common_param); };
			signal_t dF_dp(size_t index, const std::vector<signal_t>& common_param) const { return base_t::F->_dp(index, base_t::s, base_t::params, common_param); };
		};

		template<typename signal_t>
		inline uns::nn::reverse_neuron_representation<signal_t> uns::nn::nonrecursive_reverse_neuron<signal_t>::_Represent() const {
			uns::nn::reverse_neuron_representation<signal_t> res(base_t::Represent());
			res._r = _r;
			res._s = _s;
			res.is_learning = is_learning;
			res.Dropout(Dropout());
			if (_input != nullptr)
				res._links.push_back(std::pair<int, int>(_input->Index(), 0));
			for (auto _link : _links)
				res._links.push_back(std::pair<int, int>(_link.first->Index(), _link.second));

			return res;
		};


		template<class neuron_t>
		class nonrecursive_base_network : public network_interface<typename neuron_t::neuron_signal_t> {
		protected:
			std::vector<std::vector<neuron_t*>> layers;
			std::vector<typename neuron_t::neuron_signal_t> common_params;
		public:
			using signal_t = typename neuron_t::neuron_signal_t;
			using base_t = network_interface<signal_t>;

			nonrecursive_base_network() {};
			nonrecursive_base_network(const network_representation<neuron_representation<signal_t>>& repr, const uns::nn::input_data_object_interface<signal_t>& ido);
			nonrecursive_base_network(const nonrecursive_base_network<neuron_t>& net) = delete;
			nonrecursive_base_network<neuron_t>& operator=(const nonrecursive_base_network<neuron_t>& net) = delete;
			nonrecursive_base_network(nonrecursive_base_network<neuron_t>&& net) = delete;
			nonrecursive_base_network<neuron_t>& operator=(nonrecursive_base_network<neuron_t>&& net) = delete;
			~nonrecursive_base_network() {
				for (auto& layer : layers)
					for (auto neuron : layer) {
						delete neuron;
					};
				for (auto input : base_t::inputs)
					delete input;
			};
			network_representation<neuron_representation<signal_t>> Represent();
			void React() override {
				for (auto input : base_t::inputs)
					input->React(common_params);
				for (auto& layer : layers)
					for (auto neuron : layer) {
						neuron->Collect(common_params);
						neuron->React(common_params);
					};
			};
			void React(const input_data_object_interface<signal_t>& ido) {
				for (auto input : base_t::inputs) {
					input->Set(input->Index(), ido);
					input->React(common_params);
				};
				for (auto& layer : layers)
					for (auto neuron : layer) {
						neuron->Collect(common_params);
						neuron->React(common_params);
					};
			};
			signal_t R(size_t layer_index, size_t index) const { return layers[layer_index][index]->R(); };
			signal_t C(size_t layer_index, size_t index) const { return layers[layer_index][index]->C(); };
			signal_t O(size_t index) const { return base_t::outputs[index]->R(); };
			signal_t I(size_t index) const { return base_t::inputs[index]->R(); };
			size_t Layers() const { return layers.size(); };
			size_t Neurons(size_t index) const { return layers[index].size(); };
			size_t Outputs() const { return base_t::outputs.size(); };
			size_t Inputs() const { return base_t::inputs.size(); };
			size_t Weights(size_t layer_index, size_t index) const { return layers[layer_index][index].links.size(); };
			size_t Params(size_t layer_index, size_t index) const { return layers[layer_index][index].params.size(); };
		};

		template<class neuron_t>
		nonrecursive_base_network<neuron_t>::nonrecursive_base_network(const uns::nn::network_representation<neuron_representation<signal_t>>& repr, const uns::nn::input_data_object_interface<signal_t>& ido) {
			base_t::inputs.clear();
			base_t::outputs.clear();
			layers.clear();

			layers.resize(repr.layers.size());

			std::vector<neuron_t*> neurons;
			size_t layer_number = 0;
			neuron_t* next_neuron = nullptr;
			for (auto& layer : repr.layers) {
				for (auto neuron : layer.neurons) {
					next_neuron = new neuron_t();
					next_neuron->F = uns::nn::StringToActivator<signal_t>(layer.activator_type);
					next_neuron->S = uns::nn::StringToCollector<signal_t>(layer.collector_type);
					next_neuron->r = neuron.r;
					next_neuron->s = neuron.s;
					next_neuron->params = neuron.params;
					next_neuron->Index(uns::unified_cast<int>(neurons.size()));
					neurons.push_back(next_neuron);
					layers[layer_number].push_back(next_neuron);
				};
				layer_number++;
			};

			std::vector<int> input_neurons_numbers;
			const int empty_meaning = -1;
			size_t input_number = 0;
			size_t neuron_number = 0;
			for (auto& layer : repr.layers) {
				for (auto neuron : layer.neurons) {
					for (auto link : neuron.links)
						if (link.first < 0) {
							input_number = uns::unified_cast<size_t>(-link.first);
							if (input_neurons_numbers.size() < input_number + 1)
								input_neurons_numbers.resize(input_number + 1, empty_meaning);
							if (input_neurons_numbers[input_number] == empty_meaning) {
								base_t::inputs.push_back(new input_neuron<signal_t>(link.first, ido));
								input_neurons_numbers[input_number] = uns::unified_cast<int>(base_t::inputs.size() - 1);
							};
							if (input_neurons_numbers[input_number] > empty_meaning)
								neurons[neuron_number]->links.push_back(std::pair<neuron_interface<signal_t>*, signal_t>(dynamic_cast<neuron_interface<signal_t>*>(base_t::inputs[uns::unified_cast<size_t>(input_neurons_numbers[input_number])]), link.second));
						}
						else if (link.first < neurons.size())
							neurons[neuron_number]->links.push_back(std::pair<neuron_t*, signal_t>(neurons[uns::unified_cast<size_t>(link.first)], link.second));
					neuron_number++;
				};
			};

			for (auto output : repr.outputs)
				if (output < neurons.size())
					base_t::outputs.push_back(neurons[output]);

			for (auto common_param : repr.common_params)
				common_params.push_back(common_param);
		};

		template<class neuron_t>
		inline network_representation<neuron_representation<typename neuron_t::neuron_signal_t>> nonrecursive_base_network<neuron_t>::Represent() {
			network_representation<neuron_representation<signal_t>> res;

			res.layers.resize(layers.size());
			size_t layer_number = 0;
			for (auto& layer : layers) {
				for (auto neuron_ptr : layer)
					res.layers[layer_number].neurons.push_back(neuron_ptr->Represent());
				if (layer.size()>0) {
					res.layers[layer_number].activator_type = layer[0]->F->Type();
					res.layers[layer_number].collector_type = layer[0]->S->Type();
				};
				layer_number++;
			};

			for (auto output : base_t::outputs)
				res.outputs.push_back(output->Index());

			for (auto common_param : common_params)
				res.common_params.push_back(common_param);

			return res;
		};


		template<typename signal_t>
		using nonrecursive_network = nonrecursive_base_network<nonrecursive_neuron<signal_t>>;


		template<typename signal_t>
		class nonrecursive_reverse_network : public nonrecursive_base_network<uns::nn::nonrecursive_reverse_neuron<signal_t>> {
		protected:
			using neuron_t = uns::nn::nonrecursive_reverse_neuron<signal_t>;
			using base_t = nonrecursive_base_network<neuron_t>;

			std::vector<input_neuron<signal_t>*> _inputs;
		public:
			nonrecursive_reverse_network() {};
			nonrecursive_reverse_network(const network_representation<neuron_representation<signal_t>>& repr, const uns::nn::input_data_object_interface<signal_t>& ido, const uns::nn::input_data_object_interface<signal_t>& _ido);
			nonrecursive_reverse_network(const nonrecursive_reverse_network<signal_t>& net) = delete;
			nonrecursive_reverse_network<signal_t>& operator=(const nonrecursive_reverse_network<signal_t>& net) = delete;
			nonrecursive_reverse_network(nonrecursive_reverse_network<signal_t>&& net) = delete;
			nonrecursive_reverse_network<signal_t>& operator=(nonrecursive_reverse_network<signal_t>&& net) = delete;
			~nonrecursive_reverse_network() {
				for (auto _input : _inputs)
					delete _input;
			};
			network_representation<reverse_neuron_representation<signal_t>> _Represent();
			void _React() {
				for (auto _input : _inputs)
					_input->React(base_t::common_params);
				for (auto layer = base_t::layers.crbegin(); layer != base_t::layers.crend(); layer++)
					for (auto neuron = layer->crbegin(); neuron != layer->crend(); neuron++) {
						(*neuron)->_Collect(base_t::common_params);
						(*neuron)->_React(base_t::common_params);
					};
			};
			void _React(const input_data_object_interface<signal_t>& _ido) {
				for (auto _input : _inputs) {
					_input->Set(_input->Index(), _ido);
					_input->React(base_t::common_params);
				};
				for (auto layer = base_t::layers.crbegin(); layer != base_t::layers.crend(); layer++)
					for (auto neuron = layer->crbegin(); neuron != layer->crend(); neuron++) {
						(*neuron)->_Collect(base_t::common_params);
						(*neuron)->_React(base_t::common_params);
					};
			};
			signal_t _R(size_t layer_index, size_t index) const { return base_t::layers[layer_index][index]->_R(); };
			signal_t _C(size_t layer_index, size_t index) const { return base_t::layers[layer_index][index]->_S(); };
			signal_t dS_dw(size_t layer_index, size_t neuron_idx, size_t weight_idx) const { return base_t::layers[layer_index][neuron_idx]->dS_dw(weight_idx, base_t::common_params); };
			signal_t dS_dp(size_t layer_index, size_t neuron_idx, size_t param_idx) const { return base_t::layers[layer_index][neuron_idx]->dS_dp(param_idx, base_t::common_params); };
			signal_t dF_dp(size_t layer_index, size_t neuron_idx, size_t param_idx) const { return base_t::layers[layer_index][neuron_idx]->dF_dp(param_idx, base_t::common_params); };
			bool IsLearning(size_t layer_index, size_t neuron_idx) const { return base_t::layers[layer_index][neuron_idx]->IsLearning(); };
			signal_t Dropout(size_t layer_index, size_t neuron_idx) const noexcept { return base_t::layers[layer_index][neuron_idx]->Dropout(); };
			void Dropout(signal_t dropout, size_t layer_index, size_t neuron_idx) { base_t::layers[layer_index][neuron_idx]->Dropout(dropout); };
		};

		template<typename signal_t>
		nonrecursive_reverse_network<signal_t>::nonrecursive_reverse_network(const network_representation<neuron_representation<signal_t>>& repr, const uns::nn::input_data_object_interface<signal_t>& ido, const uns::nn::input_data_object_interface<signal_t>& _ido) : base_t(repr, ido) {
			_inputs.clear();

			std::vector<neuron_t*> neurons;
			for (auto& layer : base_t::layers)
				for (auto neuron : layer)
					neurons.push_back(neuron);

			int output_number = 0;
			for (auto output : base_t::outputs) {
				_inputs.push_back(new input_neuron<signal_t>(-(output_number++), _ido));
				if (output->Index() >= 0 && output->Index() < neurons.size()) neurons[output->Index()]->_input = _inputs[_inputs.size() - 1];
			};

			size_t link_number = 0;
			for (auto neuron = neurons.crbegin(); neuron != neurons.crend(); neuron++) {
				link_number = 0;
				for (auto link : ((*neuron)->links)) {
					if (link.first->IsReversable())
						dynamic_cast<neuron_t*>(link.first)->_links.push_back(std::pair<neuron_t*, size_t>((*neuron), link_number++));
				};
			};
		};

		template<typename signal_t>
		inline network_representation<reverse_neuron_representation<signal_t>> nonrecursive_reverse_network<signal_t>::_Represent() {
			network_representation<reverse_neuron_representation<signal_t>> res;

			res.layers.resize(base_t::layers.size());
			size_t layer_number = 0;
			for (auto& layer : base_t::layers) {
				for (auto neuron_ptr : layer)
					res.layers[layer_number].neurons.push_back(neuron_ptr->_Represent());
				if (layer.size() > 0) {
					res.layers[layer_number].activator_type = layer[0]->F->Type();
					res.layers[layer_number].collector_type = layer[0]->S->Type();
				};
				layer_number++;
			};
			for (auto output : base_t::outputs)
				res.outputs.push_back(output->Index());

			for (auto common_param : base_t::common_params)
				res.common_params.push_back(common_param);

			return res;
		};

#ifdef UNS_NN_LITERAL
#undef UNS_NN_LITERAL
#endif
	};
};