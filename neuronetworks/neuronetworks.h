#pragma once

#include <tuple>
#include <vector>

#include "uniself/algorithms.h"
#include "uniself/strings.h"
#include "uniself/mathematics.h"


namespace uns::nn {

	using string_t = std::wstring;

	template<typename signal_t>
	class input_neuron;


	template<class signal_t>
	class sequential_neuron;


	template<class signal_t>
	class nonrecursive_reverse_neuron;


	template<class neuron_t>
	class sequential_base_network;


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
		virtual std::pair<int, size_t> Index() const noexcept = 0;
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
		signal_t value = signal_t();
	public:
		activator() noexcept {};
		activator(const activator<signal_t>& a) noexcept : value(a.value) {};
		activator<signal_t>& operator=(const activator<signal_t>& a) noexcept { value = a.value; return *this; };
		activator(activator<signal_t>&& a) noexcept : value(a.value) {};
		activator<signal_t>& operator=(activator<signal_t>&& a) noexcept { value = a.value; return *this; };
		signal_t& operator=(const signal_t& val) noexcept { value = val; return value; };
		~activator() noexcept {};

		signal_t Value() const noexcept { return value; };
		signal_t& Value() noexcept { return value; };

		virtual uns::nn::string_t Type() const noexcept { return uns::string_cast<string_t>("Zero"); };

		virtual signal_t operator()(const signal_t S, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) { return 0; };

		virtual signal_t _dS(const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };

		virtual signal_t _dp(size_t index, const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
	};


	template<typename signal_t>
	class collector {
	protected:
		signal_t value = signal_t();
	public:
		collector() noexcept {};
		collector(const collector<signal_t>& a) noexcept : value(a.value) {};
		collector<signal_t>& operator=(const collector<signal_t>& a) noexcept { value = a.value; return *this; };
		collector(collector<signal_t>&& a) noexcept : value(a.value) {};
		collector<signal_t>& operator=(collector<signal_t>&& a) noexcept { value = a.value; return *this; };
		signal_t& operator=(const signal_t& val) noexcept { value = val; return value; };
		~collector() noexcept {};

		signal_t Value() const noexcept { return value; };
		signal_t& Value() noexcept { return value; };

		virtual uns::nn::string_t Type() const noexcept { return uns::string_cast<string_t>("Zero"); };

		virtual signal_t operator()(std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) { return 0; };

		virtual signal_t _dr(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };

		virtual signal_t _dw(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };

		virtual signal_t _dp(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const { return 0; };
	};

	//Функция конвертации строки в активатор нейрона
	template<typename signal_t>
	std::shared_ptr<uns::nn::activator<signal_t>> activator_cast(const uns::nn::string_t& str); /*введены отдельно вместо string_cast, изза того что они должны быть шаблонами от signal_t*/
	template<typename signal_t>
	uns::nn::string_t activator_cast(const uns::nn::activator<signal_t>& obj) {
		return obj.Type();
	};
	template<typename signal_t>
	uns::nn::string_t activator_cast(const std::shared_ptr<uns::nn::activator<signal_t>>& ptr) {
		return ptr->Type();
	};


	//Функция конвертации строки в коллектор нейрона
	template<typename signal_t>
	std::shared_ptr<uns::nn::collector<signal_t>> collector_cast(const uns::nn::string_t& str);
	template<typename signal_t>
	uns::nn::string_t collector_cast(const uns::nn::collector<signal_t>& obj) {
		return obj.Type();
	};
	template<typename signal_t>
	uns::nn::string_t collector_cast(const std::shared_ptr<uns::nn::collector<signal_t>>& ptr) {
		return ptr->Type();
	};


	//Функция получения строковой сигнатуры типа нейрона по коллектору и активатору
	uns::nn::string_t Type(const uns::nn::string_t& str_activator_t, const uns::nn::string_t& str_collector_t) {
		return str_activator_t + uns::string_cast<string_t>("_") + str_collector_t;
	};
	template<typename signal_t>
	uns::nn::string_t Type(const uns::nn::activator<signal_t>& Activator, const uns::nn::collector<signal_t>& Collector) {
		return uns::nn::Type(Activator.Type(), Collector.Type());
	};


	//Функция получения строковых сигнатур активатора и коллектора по строковой сигнатуре типа нейрона
	std::tuple<string_t, string_t> Type(const string_t& str) {
		return uns::string::Split<string_t>(str, uns::string_cast<string_t>("_"));
	};


	template<typename signal_t>
	class neuron_representation {
	public:
		using neuron_signal_t = signal_t;

		enum {
			layer_adress = 0,
			neuron_adress = 1,
			weight = 2
		};

		signal_t r = 0;
		signal_t s = 0;
		std::vector<std::tuple<int, size_t, signal_t>> links;
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

		virtual uns::nn::string_t String() const noexcept {
			//шаблон строчного представления:
			//[(adress1,weight1;...;adressN,weightN;)param1,...paramN,(R,S)]
			const static auto comma = uns::string_cast<uns::nn::string_t>(",");
			const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");
			const static auto left_parenthesis = uns::string_cast<uns::nn::string_t>("(");
			const static auto right_parenthesis = uns::string_cast<uns::nn::string_t>(")");
			const static auto left_bracket = uns::string_cast<uns::nn::string_t>("[");
			const static auto right_bracket = uns::string_cast<uns::nn::string_t>("]");

			uns::nn::string_t res = left_bracket + left_parenthesis;
			for (auto link : links)
				res += uns::string_cast<uns::nn::string_t>(std::get<layer_adress>(link)) + comma + uns::string_cast<uns::nn::string_t>(std::get<neuron_adress>(link)) + comma + uns::string_cast<uns::nn::string_t>(std::get<weight>(link)) + semicolon;
			res += right_parenthesis;
			for (auto param : params)
				res += uns::string_cast<uns::nn::string_t>(param) + comma;
			res += left_parenthesis + uns::string_cast<uns::nn::string_t>(r) + comma + uns::string_cast<uns::nn::string_t>(s) + right_parenthesis + right_bracket;

			return res;
		};

		virtual bool Set(const string_t& str) {
			const static auto comma = uns::string_cast<uns::nn::string_t>(",");
			const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");
			const static auto left_parenthesis = uns::string_cast<uns::nn::string_t>("(");
			const static auto right_parenthesis = uns::string_cast<uns::nn::string_t>(")");
			const static auto left_bracket = uns::string_cast<uns::nn::string_t>("[");
			const static auto right_bracket = uns::string_cast<uns::nn::string_t>("]");

			uns::nn::string_t::size_type seeker = 0;
			if (!uns::string::SeekerSet<uns::nn::string_t>(str, seeker, left_bracket + left_parenthesis, false, -1, right_bracket)) return false;
			uns::nn::string_t temp = uns::nn::string_t();
			{
				uns::nn::string_t temp1 = uns::nn::string_t();
				uns::nn::string_t temp2 = uns::nn::string_t();
				links.clear();
				do {
					if (!uns::string::SeekerRead<uns::nn::string_t>(str, seeker, temp, comma, false, -1, right_parenthesis)) break;
					else if (!uns::string::SeekerRead<uns::nn::string_t>(str, seeker, temp1, comma, false, -1, right_parenthesis)) break;
					else if (!uns::string::SeekerRead<uns::nn::string_t>(str, seeker, temp2, semicolon, false, -1, right_parenthesis)) break;
					else {
						links.push_back(std::make_tuple<int, size_t, signal_t>(uns::string_cast<int>(temp), uns::string_cast<size_t>(temp1), uns::string_cast<signal_t>(temp2)));
					};
				}
				while (true);
			};

			params.clear();
			if (!uns::string::SeekerSet<uns::nn::string_t>(str, seeker, right_parenthesis, false, -1, right_bracket)) return false;
			do {
				if (!uns::string::SeekerRead<uns::nn::string_t>(str, seeker, temp, comma, false, -1, left_parenthesis)) break;
				else params.push_back(uns::string_cast<signal_t>(temp));
			}
			while (true);
			if (!uns::string::SeekerSet<uns::nn::string_t>(str, seeker, left_parenthesis, false, -1, right_bracket)) return false;
			if (!uns::string::SeekerRead<uns::nn::string_t>(str, seeker, temp, comma, false, -1, right_parenthesis)) return false;
			else r = uns::string_cast<signal_t>(temp);
			if (!uns::string::SeekerRead<uns::nn::string_t>(str, seeker, temp, right_parenthesis, false, -1, right_bracket)) return false;
			else s = uns::string_cast<signal_t>(temp);

			return true;
		};
	};


	template<typename signal_t>
	class reverse_neuron_representation : public neuron_representation<signal_t> {
	protected:
		using base_t = neuron_representation<signal_t>;
	public:
		enum {
			layer_adress = 0,
			neuron_adress = 1,
			place = 2
		};
	protected:
		signal_t dropout = 0.0;
	public:
		signal_t _r = 0;
		signal_t _s = 0;
		std::vector<std::tuple<int, size_t, size_t>> _links;
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
			//[_(layer_adress1,neuron_adress1,place1;...;layer_adressN,neuron_adressN,placeN;)(_R,_S,dropout,is_learning)]
			const static auto reverse_neuron_starter = uns::string_cast<uns::nn::string_t>("[_(");
			const static auto comma = uns::string_cast<uns::nn::string_t>(",");
			const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");
			const static auto left_parenthesis = uns::string_cast<uns::nn::string_t>("(");
			const static auto right_parenthesis = uns::string_cast<uns::nn::string_t>(")");
			const static auto right_bracket = uns::string_cast<uns::nn::string_t>("]");

			string_t res = base_t::String();
			res += reverse_neuron_starter;
			for (auto _link : _links) {
				res += uns::string_cast<string_t>(std::get<layer_adress>(_link)) + comma + uns::string_cast<string_t>(std::get<neuron_adress>(_link)) + comma + uns::string_cast<string_t>(std::get<place>(_link)) + semicolon;
			};
			res += right_parenthesis;
			res += left_parenthesis + uns::string_cast<string_t>(_r)
				+ comma + uns::string_cast<string_t>(_s)
				+ comma + uns::string_cast<string_t>(Dropout())
				+ comma + uns::string_cast<string_t>(is_learning) + right_parenthesis + right_bracket;

			return res;
		};

		virtual bool Set(const string_t& str) override {
			const static auto reverse_neuron_starter = uns::string_cast<uns::nn::string_t>("[_(");
			const static auto comma = uns::string_cast<uns::nn::string_t>(",");
			const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");
			const static auto left_parenthesis = uns::string_cast<uns::nn::string_t>("(");
			const static auto right_parenthesis = uns::string_cast<uns::nn::string_t>(")");
			const static auto right_bracket = uns::string_cast<uns::nn::string_t>("]");

			if (!base_t::Set(str)) return false;
			string_t::size_type seeker = 0;
			if (!uns::string::SeekerSet<string_t>(str, seeker, reverse_neuron_starter, false, -1)) return false;

			string_t temp = string_t();
			{
				string_t temp1 = string_t();
				string_t temp2 = string_t();
				_links.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, comma, false, -1, right_parenthesis)) break;
					else if (!uns::string::SeekerRead<string_t>(str, seeker, temp1, comma, false, -1, right_parenthesis)) break;
					else if (!uns::string::SeekerRead<string_t>(str, seeker, temp2, semicolon, false, -1, right_parenthesis)) break;
					else {
						_links.push_back(std::make_tuple<int, size_t, size_t>(uns::string_cast<int>(temp), uns::string_cast<size_t>(temp1), uns::string_cast<size_t>(temp2)));
					};
				}
				while (true);
			};

			if (!uns::string::SeekerSet<string_t>(str, seeker, right_parenthesis + left_parenthesis, false, -1, right_parenthesis + right_bracket)) return false;
			if (!uns::string::SeekerRead<string_t>(str, seeker, temp, comma, false, -1, right_parenthesis)) return false;
			else _r = uns::string_cast<signal_t>(temp);
			if (!uns::string::SeekerRead<string_t>(str, seeker, temp, comma, false, -1, right_parenthesis)) return false;
			else _s = uns::string_cast<signal_t>(temp);
			if (!uns::string::SeekerRead<string_t>(str, seeker, temp, comma, false, -1, right_parenthesis)) return false;
			else Dropout(uns::string_cast<signal_t>(temp));
			if (!uns::string::SeekerRead<string_t>(str, seeker, temp, right_parenthesis, false, -1, right_bracket)) return false;
			else is_learning = uns::string_cast<bool>(temp);

			return true;
		};

		signal_t Dropout() const noexcept { return dropout; };

		void Dropout(signal_t dropout_) noexcept {
			dropout = (uns::math::More(dropout_, 1) ? 1 : (uns::math::Less(dropout_, 0) ? 0 : dropout_));
		};
	};


	template<typename neuron_representation_t>
	class layer_representation {
	public:
		string_t activator_type = uns::string_cast<string_t>("Zero");
		string_t collector_type = uns::string_cast<string_t>("Zero");

		std::vector<neuron_representation_t> neurons;

		layer_representation() {};
		layer_representation(const string_t& str) { Set(str); };
		layer_representation(const layer_representation<neuron_representation_t>& repr) : neurons(repr.neurons), activator_type(repr.activator_type), collector_type(repr.collector_type) {};
		layer_representation<neuron_representation_t>& operator=(const layer_representation<neuron_representation_t>& repr) { neurons = repr.neurons;  activator_type = repr.activator_type; collector_type = repr.collector_type; return *this; };
		layer_representation(layer_representation<neuron_representation_t>&& repr) noexcept : neurons(std::move(repr.neurons)), activator_type(std::move(repr.activator_type)), collector_type(std::move(repr.collector_type)) {};
		layer_representation<neuron_representation_t>& operator=(layer_representation<neuron_representation_t>&& repr) { neurons = std::move(repr.neurons); activator_type = std::move(repr.activator_type); collector_type = std::move(repr.collector_type); return *this; };
		~layer_representation() noexcept {};

		string_t String(size_t layer_index) const {
			//шаблон строчного представления:
			//layer_index Type:\n	neuron_index1 neuron1\n	neuron_index2 neuron2\n...	neuron_indexN neuronN\n:layer_index Type
			const static auto colon = uns::string_cast<uns::nn::string_t>(":");
			const static auto space = uns::string_cast<uns::nn::string_t>(" ");
			const static auto tab = uns::string_cast<uns::nn::string_t>("\t");
			const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");

			size_t neuron_index = 0;

			string_t res = uns::string_cast<string_t>(layer_index) + space + uns::nn::Type(activator_type, collector_type) + colon + delimiter;
			for (auto neuron : neurons)
				res += tab + uns::string_cast<string_t>(neuron_index++) + space + neuron.String() + delimiter;
			res += colon + uns::string_cast<string_t>(layer_index) + space + uns::nn::Type(activator_type, collector_type);

			return res;
		};

		bool Set(const string_t& str) {
			const static auto colon = uns::string_cast<uns::nn::string_t>(":");
			const static auto space = uns::string_cast<uns::nn::string_t>(" ");
			const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
			const static auto LAYER = uns::string_cast<uns::nn::string_t>("LAYER");

			string_t::size_type seeker = 0;
			if (!uns::string::SeekerSet<string_t>(str, seeker, space, false, -1)) return false;
			string_t layer_index = str.substr(0, seeker);
			if (layer_index.empty()) return false;

			string_t temp = string_t();
			if (!uns::string::SeekerRead<string_t>(str, seeker, temp, colon + delimiter, false, -1, colon + layer_index)) return false;
			else std::tie(activator_type, collector_type) = Type(temp);

			neurons.clear();
			do {
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, delimiter, false, -1, colon + layer_index)) break;
				else neurons.push_back(neuron_representation_t(temp));
			}
			while (true);

			return true;
		};
	};


	template<typename  neuron_representation_t>
	class network_representation {
	public:
		std::vector<layer_representation<neuron_representation_t>> layers;
		std::vector<std::pair<int, size_t>> outputs;
		std::vector<typename neuron_representation_t::neuron_signal_t> common_params;
	public:
		network_representation() {};
		network_representation(const string_t& str) { Set(str); };
		network_representation(const network_representation<neuron_representation_t>& repr) : layers(repr.layers), outputs(repr.outputs), common_params(repr.common_params) {};
		network_representation<neuron_representation_t>& operator=(const network_representation<neuron_representation_t>& repr) { layers = repr.layers; outputs = repr.outputs;  common_params = repr.common_params;  return *this; };
		network_representation(network_representation<neuron_representation_t>&& repr) noexcept : layers(std::move(repr.layers)), outputs(std::move(repr.outputs)), common_params(std::move(repr.common_params)) {};
		network_representation<neuron_representation_t>& operator=(network_representation<neuron_representation_t>&& repr) { layers = std::move(repr.layers); outputs = std::move(repr.outputs); common_params = std::move(repr.common_params); return *this; };
		~network_representation() noexcept {};

		string_t String() const {
			const static auto comma = uns::string_cast<uns::nn::string_t>(",");
			const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");
			const static auto colon = uns::string_cast<uns::nn::string_t>(":");
			const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
			const static auto NETWORK = uns::string_cast<string_t>("NETWORK");
			const static auto OUTPUTS = uns::string_cast<string_t>("OUTPUTS");
			const static auto PARAMS = uns::string_cast<string_t>("PARAMS");

			size_t layer_index = 0;
			string_t res = NETWORK + colon + delimiter;
			for (auto layer : layers)
				res += layer.String(layer_index++) + semicolon + delimiter;
			res += OUTPUTS + colon;
			for (auto output : outputs)
				res += uns::string_cast<string_t>(std::get<neuron_representation_t::layer_adress>(output)) + comma + uns::string_cast<string_t>(std::get<neuron_representation_t::neuron_adress>(output)) + semicolon;
			res += colon + OUTPUTS + delimiter;
			res += PARAMS + colon;
			for (auto common_param : common_params)
				res += uns::string_cast<string_t>(common_param) + comma;
			res += colon + PARAMS + delimiter;
			res += colon + NETWORK;

			return res;

		};

		bool Set(const string_t& str) {
			const static auto comma = uns::string_cast<uns::nn::string_t>(",");
			const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");
			const static auto colon = uns::string_cast<uns::nn::string_t>(":");
			const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
			const static auto NETWORK = uns::string_cast<string_t>("NETWORK");
			const static auto OUTPUTS = uns::string_cast<string_t>("OUTPUTS");
			const static auto PARAMS = uns::string_cast<string_t>("PARAMS");

			string_t::size_type seeker = 0;
			if (!uns::string::SeekerSet<string_t>(str, seeker, NETWORK + colon + delimiter, false, -1)) return false;

			string_t temp = string_t();
			layers.clear();
			do {
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, semicolon + delimiter, false, -1, OUTPUTS + colon)) break;
				else layers.push_back(layer_representation<neuron_representation_t>(temp));
			}
			while (true);

			if (!uns::string::SeekerSet<string_t>(str, seeker, OUTPUTS + colon, false, -1)) return false;
			temp = string_t();
			outputs.clear();
			int output_layer_index = 0;
			size_t output_neuron_index = 0;
			do {
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, comma, false, -1, colon + OUTPUTS)) break;
				else output_layer_index = uns::string_cast<size_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, semicolon, false, -1, colon + OUTPUTS)) break;
				else output_neuron_index = uns::string_cast<size_t>(temp);

				outputs.push_back(std::pair<int, size_t>(output_layer_index, output_neuron_index));
			}
			while (true);

			if (!uns::string::SeekerSet<string_t>(str, seeker, PARAMS + colon, false, -1)) return false;
			temp = string_t();
			common_params.clear();
			do {
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, comma, false, -1, colon + PARAMS)) break;
				else common_params.push_back(uns::string_cast<typename neuron_representation_t::neuron_signal_t>(temp));
			}
			while (true);

			return true;
		};
	};


	template<typename network_representation_out_t, typename network_representation_in_t>
	auto network_representation_cast(const network_representation_in_t& repr) {
		network_representation_out_t res;

		res.layers.resize(repr.layers.size());
		size_t layer_index = 0;
		for (auto layer : repr.layers) {
			res.layers[layer_index].activator_type = layer.activator_type;
			res.layers[layer_index].collector_type = layer.collector_type;
			for (auto neuron : layer.neurons)
				res.layers[layer_index].neurons.push_back(neuron);
			layer_index++;
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
		size_t num_of_cell = 0;
		signal_t dropout = 0.0F;
	public:
		input_neuron() {};
		input_neuron(int num_of_input, const input_data_object_interface<signal_t>& ido) noexcept { Set(num_of_input, ido); };
		input_neuron(const input_neuron<signal_t>& inp) = delete;
		input_neuron<signal_t>& operator=(const input_neuron<signal_t>& inp) = delete;
		input_neuron(input_neuron<signal_t>&& inp) = delete;
		input_neuron<signal_t>& operator=(input_neuron<signal_t>&& inp) = delete;
		~input_neuron() {};

		uns::nn::string_t Type() const noexcept override { return uns::string_cast<string_t>("Input"); };

		signal_t R() const noexcept override { return r; };

		void R(const signal_t R) noexcept override { r = R; };

		std::pair<int, size_t> Index() const noexcept override { return std::make_pair<int, size_t>(-1, static_cast<int>(num_of_cell)); };

		bool IsReversable() const noexcept override { return false; };

		signal_t Dropout() const noexcept override { return 0.0F; };

		void Dropout(signal_t dropout_) noexcept {
			dropout = (uns::math::More(dropout_, 1) ? 1 : (uns::math::Less(dropout_, 0) ? 0 : dropout_));
		};

		void React(const std::vector<signal_t>& common_params) noexcept override { r = *value_of_cell; };

		void Set(size_t num_of_input, const input_data_object_interface<signal_t>& ido) noexcept {
			num_of_cell = num_of_input;
			value_of_cell = ido.Get(num_of_cell);
		};
	};


	template<class signal_t>
	class sequential_neuron : public neuron_interface<signal_t> {
		friend sequential_base_network<sequential_neuron<signal_t>>;
		friend nonrecursive_reverse_network<signal_t>;
	protected:
		using weight_t = signal_t;
		using param_t = signal_t;
		using base_t = neuron_interface<signal_t>;
	public:
		enum {
			neuron = 0,
			weight = 1
		};
	protected:
		int layer_index = 0;
		size_t index = 0;
		std::shared_ptr<uns::nn::activator<signal_t>> F;
		std::shared_ptr<uns::nn::collector<signal_t>> S;
		std::vector<std::pair<neuron_interface<signal_t>*, weight_t>> links;
		std::vector<param_t> params;
	public:
		using neuron_signal_t = signal_t;

		sequential_neuron() noexcept : F(nullptr), S(nullptr) {};
		sequential_neuron(const uns::nn::sequential_neuron<signal_t>& n) = delete;
		sequential_neuron<signal_t>& operator=(const uns::nn::sequential_neuron<signal_t>& n) = delete;
		sequential_neuron(uns::nn::sequential_neuron<signal_t>&& n) = delete;
		sequential_neuron<signal_t>& operator=(uns::nn::sequential_neuron<signal_t>&& n) = delete;
		~sequential_neuron() noexcept {};

		neuron_representation<signal_t> Represent() const;

		uns::nn::string_t Type() const noexcept override { return uns::nn::Type(F->Type(), S->Type()); };

		std::pair<int, size_t> Index() const noexcept override { return std::pair<int, size_t>(layer_index, static_cast<int>(index)); };

		void Index(const int layer, const size_t neuron) noexcept { layer_index = layer; index = neuron; };

		bool IsReversable() const noexcept override { return false; };

		signal_t Dropout() const noexcept override { return 0; };

		signal_t R() const noexcept override { return F->Value(); };

		void R(const signal_t R) noexcept override { *F = R; };

		signal_t C() const noexcept { return S->Value(); };

		void C(const signal_t c) noexcept { S = c; };

		void React(const std::vector<signal_t>& common_params) override { (*F)(S->Value(), params, common_params); };

		void Collect(const std::vector<signal_t>& common_params) { (*S)(links, params, common_params); };
	};

	template<typename signal_t>
	inline uns::nn::neuron_representation<signal_t> uns::nn::sequential_neuron<signal_t>::Represent() const {
		using neuron_t = uns::nn::sequential_neuron<signal_t>;
		using representation_t = uns::nn::neuron_representation<signal_t>;

		uns::nn::neuron_representation<signal_t> res;
		res.r = R();
		res.s = C();
		size_t last_link_index = 0;
		for (auto link : links) {
			res.links.push_back(std::tuple<int, size_t, weight_t>(0, 0, std::get<neuron_t::weight>(link)));
			last_link_index = res.links.size() - 1;
			std::tie(
				std::get<representation_t::layer_adress>(res.links[last_link_index]),
				std::get<representation_t::neuron_adress>(res.links[last_link_index])
			) = std::get<neuron_t::neuron>(link)->Index();
		};
		for (auto param : params) {
			res.params.push_back(param);
		};

		return res;
	};


	template<typename signal_t>
	class nonrecursive_reverse_neuron : public sequential_neuron<signal_t> {
		friend sequential_base_network<nonrecursive_reverse_neuron<signal_t>>;
		friend nonrecursive_reverse_network<signal_t>;
	protected:
		using base_t = sequential_neuron<signal_t>;
	public:
		enum {
			neuron = 0,
			place = 1
		};
	protected:
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

		void _C(const signal_t c) noexcept { _s = c; };

		void _React(const std::vector<signal_t>& common_params) {
			_r = dF_dS(common_params) * _s;
		};

		void _Collect(const std::vector<signal_t>& common_params) {
			_s = 0;
			for (auto _link : _links)
				_s += std::get<neuron>(_link)->_r * std::get<neuron>(_link)->dS_dr(std::get<place>(_link), common_params);
			if (_input != nullptr)
				_s += _input->R();
		};

		bool IsLearning() const { return is_learning; };

		void SetLearning(bool islearning) { is_learning = islearning; };

		signal_t Dropout() const noexcept override { return dropout; };

		void Dropout(signal_t dropout_) { dropout = (uns::math::More(dropout_, 1) ? signal_t(1) : (uns::math::Less(dropout_, 0) ? signal_t(0) : dropout_)); };

		signal_t dS_dr(size_t index, const std::vector<signal_t>& common_params) const { return base_t::S->_dr(index, base_t::links, base_t::params, common_params); };

		signal_t dS_dw(size_t index, const std::vector<signal_t>& common_param) const { return base_t::S->_dw(index, base_t::links, base_t::params, common_param); };

		signal_t dS_dp(size_t index, const std::vector<signal_t>& common_param) const { return base_t::S->_dp(index, base_t::links, base_t::params, common_param); };

		signal_t dF_dS(const std::vector<signal_t>& common_param) const { return base_t::F->_dS(base_t::C(), base_t::params, common_param); };

		signal_t dF_dp(size_t index, const std::vector<signal_t>& common_param) const { return base_t::F->_dp(index, base_t::C(), base_t::params, common_param); };
	};

	template<typename signal_t>
	inline uns::nn::reverse_neuron_representation<signal_t> uns::nn::nonrecursive_reverse_neuron<signal_t>::_Represent() const {
		using neuron_t = uns::nn::nonrecursive_reverse_neuron<signal_t>;
		using representation_t = uns::nn::reverse_neuron_representation<signal_t>;

		representation_t res(base_t::Represent());
		res._r = _r;
		res._s = _s;
		res.is_learning = is_learning;
		res.Dropout(Dropout());
		size_t last_link_index = 0;
		if (_input != nullptr) {
			res._links.push_back(std::make_tuple<int, size_t, size_t>(0, 0, 0));
			last_link_index = res._links.size() - 1;
			std::tie(
				std::get<representation_t::layer_adress>(res._links[last_link_index]),
				std::get<representation_t::neuron_adress>(res._links[last_link_index])
			) = _input->Index();
		};
		for (auto _link : _links) {
			res._links.push_back(std::tuple<int, size_t, size_t>(0, 0, std::get<neuron_t::place>(_link)));
			last_link_index = res._links.size() - 1;
			std::tie(
				std::get<representation_t::layer_adress>(res._links[last_link_index]),
				std::get<representation_t::neuron_adress>(res._links[last_link_index])
			) = std::get<neuron_t::neuron>(_link)->Index();
		};

		return res;
	};


	template<class neuron_t>
	class sequential_base_network : public uns::nn::network_interface<typename neuron_t::neuron_signal_t> {
	public:
		using signal_t = typename neuron_t::neuron_signal_t;
		using base_t = uns::nn::network_interface<signal_t>;
	protected:
		std::vector<std::vector<neuron_t*>> layers;
		std::vector<typename neuron_t::neuron_signal_t> common_params;
	public:
		sequential_base_network() {};
		sequential_base_network(const uns::nn::network_representation<neuron_representation<signal_t>>& repr, const uns::nn::input_data_object_interface<signal_t>& ido);
		sequential_base_network(const sequential_base_network<neuron_t>& net) = delete;
		sequential_base_network<neuron_t>& operator=(const sequential_base_network<neuron_t>& net) = delete;
		sequential_base_network(sequential_base_network<neuron_t>&& net) = delete;
		sequential_base_network<neuron_t>& operator=(sequential_base_network<neuron_t>&& net) = delete;
		~sequential_base_network() {
			for (auto& layer : layers)
				for (auto neuron : layer) {
					delete neuron;
				};
			for (auto input : base_t::inputs)
				delete input;
		};

		uns::nn::network_representation<neuron_representation<signal_t>> Represent();

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
	sequential_base_network<neuron_t>::sequential_base_network(const uns::nn::network_representation<neuron_representation<signal_t>>& repr, const uns::nn::input_data_object_interface<signal_t>& ido) {
		base_t::inputs.clear();
		base_t::outputs.clear();
		layers.clear();
		layers.resize(repr.layers.size());

		int layer_index = 0;
		size_t neuron_index = 0;
		size_t input_index = 0;

		neuron_t* next_neuron = nullptr;
		std::vector<neuron_t*> neurons;
		for (auto& layer : repr.layers) {
			for (auto neuron : layer.neurons) {
				next_neuron = new neuron_t();
				next_neuron->F = uns::nn::activator_cast<signal_t>(layer.activator_type);
				next_neuron->S = uns::nn::collector_cast<signal_t>(layer.collector_type);
				*(next_neuron->F) = neuron.r;
				*(next_neuron->S) = neuron.s;
				next_neuron->params = neuron.params;
				neurons.push_back(next_neuron);
				layers[layer_index].push_back(next_neuron);
				next_neuron->Index(layer_index, layers[layer_index].size() - 1);
			};
			layer_index++;
		};

		std::vector<int> input_neurons_indexes;
		const int empty_meaning = -1;
		input_index = 0;
		neuron_index = 0;
		for (auto& layer : repr.layers) {
			for (auto neuron : layer.neurons) {
				for (auto link : neuron.links) {
					layer_index = std::get<uns::nn::neuron_representation<signal_t>::layer_adress>(link);
					if (layer_index == -1) {
						input_index = std::get<uns::nn::neuron_representation<signal_t>::neuron_adress>(link);
						if (input_neurons_indexes.size() < input_index + 1)
							input_neurons_indexes.resize(input_index + 1, empty_meaning);
						if (input_neurons_indexes[input_index] == empty_meaning) {
							base_t::inputs.push_back(new input_neuron<signal_t>(input_index, ido));
							input_neurons_indexes[input_index] = static_cast<int>(base_t::inputs.size() - 1);
						};
						if (input_neurons_indexes[input_index] > empty_meaning)
							neurons[neuron_index]->links.push_back(
								std::pair<neuron_interface<signal_t>*, signal_t>(
									dynamic_cast<neuron_interface<signal_t>*>(
										base_t::inputs[static_cast<size_t>(input_neurons_indexes[input_index])]
										),
									std::get<uns::nn::neuron_representation<signal_t>::weight>(link)
									)
							);
					}
					else if (layer_index >= 0 && layer_index < layers.size()) {
						input_index = std::get<uns::nn::neuron_representation<signal_t>::neuron_adress>(link);
						if (input_index < layers[layer_index].size()) {
							neurons[neuron_index]->links.push_back(
								std::pair<neuron_t*, signal_t>(
									layers[layer_index][input_index],
									std::get<uns::nn::neuron_representation<signal_t>::weight>(link)
									)
							);
						};
					};
				};
				neuron_index++;
			};
		};

		for (auto output : repr.outputs) {
			layer_index = std::get<uns::nn::neuron_representation<signal_t>::layer_adress>(output);
			if (layer_index >= 0 && layer_index < layers.size()) {
				neuron_index = std::get<uns::nn::neuron_representation<signal_t>::neuron_adress>(output);
				if (neuron_index < layers[layer_index].size())
					base_t::outputs.push_back(layers[layer_index][neuron_index]);
			}
			else if (layer_index == -1) {
				input_index = std::get<uns::nn::neuron_representation<signal_t>::neuron_adress>(output);
				if (input_index < base_t::inputs.size())
					base_t::outputs.push_back(
						dynamic_cast<neuron_interface<signal_t>*>(
							base_t::inputs[static_cast<size_t>(input_neurons_indexes[input_index])]
							)
					);
			};
		};

		for (auto common_param : repr.common_params)
			common_params.push_back(common_param);
	};

	template<class neuron_t>
	inline network_representation<neuron_representation<typename neuron_t::neuron_signal_t>> sequential_base_network<neuron_t>::Represent() {
		network_representation<neuron_representation<signal_t>> res;

		res.layers.resize(layers.size());
		size_t layer_index = 0;
		for (auto& layer : layers) {
			for (auto neuron : layer)
				res.layers[layer_index].neurons.push_back(neuron->Represent());
			if (layer.size() > 0) {
				res.layers[layer_index].activator_type = layer[0]->F->Type();
				res.layers[layer_index].collector_type = layer[0]->S->Type();
			}
			else {
				res.layers[layer_index].activator_type = uns::nn::activator_cast<typename neuron_t::neuron_signal_t>(uns::nn::activator<typename neuron_t::neuron_signal_t>());
				res.layers[layer_index].collector_type = uns::nn::collector_cast<typename neuron_t::neuron_signal_t>(uns::nn::collector<typename neuron_t::neuron_signal_t>());
			};
			layer_index++;
		};

		for (auto output : base_t::outputs)
			res.outputs.push_back(output->Index());

		for (auto common_param : common_params)
			res.common_params.push_back(common_param);

		return res;
	};


	template<typename signal_t>
	using sequential_network = sequential_base_network<sequential_neuron<signal_t>>;


	template<typename signal_t>
	class nonrecursive_reverse_network : public sequential_base_network<uns::nn::nonrecursive_reverse_neuron<signal_t>> {
	protected:
		using neuron_t = uns::nn::nonrecursive_reverse_neuron<signal_t>;
		using base_t = sequential_base_network<neuron_t>;

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

		signal_t _C(size_t layer_index, size_t index) const { return base_t::layers[layer_index][index]->_C(); };

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

		/*std::vector<neuron_t*> neurons;
		for (auto& layer : base_t::layers)
			for (auto neuron : layer)
				neurons.push_back(neuron);*/

		int output_index = 0;
		int layer_index = 0;
		size_t neuron_index = 0;
		for (auto output : base_t::outputs) {
			_inputs.push_back(new input_neuron<signal_t>(output_index++, _ido));
			if (output->IsReversable())
				dynamic_cast<uns::nn::nonrecursive_reverse_neuron<signal_t>*>(output)->_input = _inputs[_inputs.size() - 1];
		};

		size_t link_number = 0;
		uns::nn::nonrecursive_reverse_neuron<signal_t>* reverse_neuron_ptr = nullptr;
		for (auto layer : base_t::layers)
			for (auto neuron : layer) {
				link_number = 0;
				for (auto link : neuron->links) {
					if (std::get<uns::nn::sequential_neuron<signal_t>::neuron>(link)->IsReversable()) {
						reverse_neuron_ptr = dynamic_cast<uns::nn::nonrecursive_reverse_neuron<signal_t>*>(std::get<uns::nn::sequential_neuron<signal_t>::neuron>(link));
						reverse_neuron_ptr->_links.push_back(
							std::pair<uns::nn::nonrecursive_reverse_neuron<signal_t>*, size_t>(
								neuron,
								link_number
								)
						);
					};
					link_number++;
				};
			};
	};

	template<typename signal_t>
	inline network_representation<reverse_neuron_representation<signal_t>> nonrecursive_reverse_network<signal_t>::_Represent() {
		network_representation<reverse_neuron_representation<signal_t>> res;

		res.layers.resize(base_t::layers.size());
		size_t layer_index = 0;
		for (auto& layer : base_t::layers) {
			for (auto neuron : layer)
				res.layers[layer_index].neurons.push_back(neuron->_Represent());
			if (layer.size() > 0) {
				res.layers[layer_index].activator_type = layer[0]->F->Type();
				res.layers[layer_index].collector_type = layer[0]->S->Type();
			}
			else {
				res.layers[layer_index].activator_type = uns::nn::activator_cast<typename neuron_t::neuron_signal_t>(uns::nn::activator<typename neuron_t::neuron_signal_t>());
				res.layers[layer_index].collector_type = uns::nn::collector_cast<typename neuron_t::neuron_signal_t>(uns::nn::collector<typename neuron_t::neuron_signal_t>());
			};
			layer_index++;
		};
		for (auto output : base_t::outputs)
			res.outputs.push_back(output->Index());

		for (auto common_param : base_t::common_params)
			res.common_params.push_back(common_param);

		return res;
	};


};