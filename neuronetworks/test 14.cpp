// neuronetworks.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <array>

#include "neuronetworks.h"

#define UNS_FILENAME "test 14.cpp"

namespace uns {
	namespace nn {

		////АКТИВАТОРЫ НЕЙРОНОВ
		//Конкретные классы активаторов
		template<typename signal_t>
		class Line : public uns::nn::activator<signal_t> {
		protected:
			using base_t = uns::nn::activator<signal_t>;
		public:
			Line() {};
			Line(const Line<signal_t>& a) {};
			Line<signal_t>& operator=(const Line<signal_t>& a) { return *this; };
			Line(Line<signal_t>&& a) {};
			Line<signal_t>& operator=(Line<signal_t>&& a) { return *this; };
			~Line() {};

			virtual uns::nn::string_t Type() const noexcept override { return uns::string_cast<string_t>("Line"); };

			virtual signal_t operator()(const signal_t S, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) override { 
				base_t::value = (params.size() > 0 ? params[1] * S : S);
				return base_t::value;
			};

			virtual signal_t _dS(const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? params[1] : 1); };

			virtual signal_t _dp(size_t index, const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (index == 1 && params.size() > 1 ? S : 0); };

			//virtual size_t Params() const noexcept override { return 1; };

			//virtual size_t CommonParams() const noexcept override { return 1; };
		};


		//Реализация функции конвертации строки в активатор генетического нейрона
		template<typename signal_t>
		std::shared_ptr<uns::nn::activator<signal_t>> activator_cast(const uns::nn::string_t& str) {
			if (str == uns::string_cast<string_t>("Line"))
				return std::make_shared<uns::nn::Line<signal_t>>();
			else return std::make_shared<uns::nn::activator<signal_t>>();
		};


		//КОЛЛЕКТОРЫ ГЕНЕТИЧЕСКИХ НЕЙРОНОВ
		//Конкретные классы коллекторов
		template<typename signal_t>
		class Perc : public uns::nn::collector<signal_t> {
		protected:
			using base_t = uns::nn::collector<signal_t>;
		public:
			Perc() {};
			Perc(const Perc<signal_t>& c) {};
			Perc<signal_t>& operator=(const Perc<signal_t>& c) { return *this; };
			Perc(Perc<signal_t>&& c) {};
			Perc<signal_t>& operator=(Perc<signal_t>&& c) { return *this; };
			~Perc() {};

			virtual uns::nn::string_t Type() const noexcept { return uns::string_cast<string_t>("Perc"); };

			virtual signal_t operator()(std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) override {
				base_t::value = 0;
				for (auto link : links)
					base_t::value += link.first->R() * link.second;
				if (params.size() > 0) base_t::value += params[0];
				return base_t::value;
			};

			virtual signal_t _dr(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return links[index].second; };

			virtual signal_t _dw(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return links[index].first->R(); };

			virtual signal_t _dp(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? 1 : 0); };

			//virtual int Links() const noexcept override { return -1; };	//предписываемое количество связей слоя (положительное требует именно столько, отрицательно = любое)

			//virtual size_t Params() const noexcept override { return 1; };	//минимальное количество параметров

			//virtual size_t CommonParams() const noexcept override { return 0; };	//минимальное количество общих параметров
		};


		//Конкретные классы коллекторов
		template<typename signal_t>
		class RBF : public uns::nn::collector<signal_t> {
		protected:
			signal_t value = 0.0F;
		public:
			RBF() {};
			RBF(const RBF<signal_t>& c) {};
			RBF<signal_t>& operator=(const RBF<signal_t>& c) { return *this; };
			RBF(RBF<signal_t>&& c) {};
			RBF<signal_t>& operator=(RBF<signal_t>&& c) { return *this; };
			~RBF() {};

			virtual uns::nn::string_t Type() const noexcept { return uns::string_cast<string_t>("RBF"); };

			virtual signal_t operator()(std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) {
				signal_t res = 0;
				for (auto link : links)
					res += link.first->R() * link.second;
				if (params.size() > 0) res += params[0];
				return res;
			};

			virtual signal_t _dr(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return links[index].second; };

			virtual signal_t _dw(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return links[index].first->R(); };

			virtual signal_t _dp(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? 1 : 0); };

			//virtual int Links() const noexcept override { return -1; };	//предписываемое количество связей слоя (положительное требует именно столько, отрицательно = любое)

			//virtual size_t Params() const noexcept override { return 1; };	//минимальное количество параметров

			//virtual size_t CommonParams() const noexcept override { return 0; };	//минимальное количество общих параметров
		};


		//Реализация функции конвертации строки в коллектор генетического нейрона
		template<typename signal_t>
		std::shared_ptr<uns::nn::collector<signal_t>> collector_cast(const uns::nn::string_t& str) {
			if (str == uns::string_cast<string_t>("Perc"))
				return std::make_shared<uns::nn::Perc<signal_t>>();
			else if (str == uns::string_cast<string_t>("RBF"))
				return std::make_shared<uns::nn::RBF<signal_t>>();
			else return std::make_shared<uns::nn::collector<signal_t>>();
		};


		template<typename signal_t, size_t size>
		class input_data_object : public input_data_object_interface<signal_t> {
		protected:
			std::array<signal_t*, size> values;
		public:
			input_data_object() {
				for (auto& value : values)
					value = new signal_t;
			};
			input_data_object(const input_data_object& copying_obj) = delete;
			input_data_object& operator=(const input_data_object& copying_obj) = delete;
			input_data_object(input_data_object&& moving_obj) = delete;
			input_data_object& operator=(input_data_object&& moving_obj) = delete;
			~input_data_object() {
				for (auto& value : values)
					if (value != nullptr)
						delete value;
			};

			virtual signal_t* Get(size_t index) const override { return values[index]; };

			virtual int Size() const override { return values.size(); };

			void Set(size_t index, signal_t value) {
				if (index < values.size())
					*values[index] = value;
				else if (values.size() > 0)
					*values[values.size() - 1] = value;
			};
		};
	};
};

int main() {
	std::cout << "START" << std::endl;

	uns::nn::input_data_object<float, 4> ido;
	uns::nn::input_data_object<float, 2> odo;

	
	uns::nn::string_t str = L"";

	/*str += L"NETWORK:\n";
	str += L"0 Line_Perc:\n";
	str += L"\t0 [(2,1,1.230000;-1,2,2.230000;-1,3,3.230000;)-0.170000,5.960000,(1.000000,2.000000)]\n";
	str += L"\t1 [(-1,4,-4.230000;-1,5,-5.230000;-1,6,-6.230000;)-0.140000,7.960000,(-1.000000,-2.000000)]\n";
	str += L":0 Line_Perc;\n";
	str += L"1 Line_RBF:\n";
	str += L"\t0 [(-1,1,1.230000;0,0,2.230000;0,1,3.230000;)-0.170000,5.960000,(1.000000,2.000000)]\n";
	str += L"\t1 [(-1,4,-4.230000;0,1,-5.230000;0,0,-6.230000;)-0.140000,7.960000,(-1.000000,-2.000000)]\n";
	str += L":1 Line_RBF;\n";
	str += L"2 Line_Perc:\n";
	str += L"\t0 [(0,1,1.230000;0,0,2.230000;1,0,3.230000;)-0.170000,5.960000,(1.000000,2.000000)]\n";
	str += L"\t1 [(0,0,-4.230000;0,1,-5.230000;1,1,-6.230000;)-0.140000,7.960000,(-1.000000,-2.000000)]\n";
	str += L":2 Line_Perc;\n";
	str += L"OUTPUTS:2,0;2,1;:OUTPUTS\n";
	str += L"PARAMS:-8.440000,:PARAMS\n";
	str += L":NETWORK\n";*/

	str += L"NETWORK:\n";
	str += L"0 Line_Perc:\n";
	str += L"\t0 [(-1,0,1.000000;-1,1,2.000000;)1.000000,1.000000,(1.000000,2.000000)]\n";
	str += L"\t1 [(-1,2,-1.000000;-1,3,-2.000000;)0.000000,2.000000,(-1.000000,-2.000000)]\n";
	str += L":0 Line_Perc;\n";
	str += L"1 Line_Perc:\n";
	str += L"\t0 [(0,0,1.000000;0,1,-1.000000;)-1.000000,1.000000,(1.000000,2.000000)]\n";
	str += L":1 Line_Perc;\n";
	str += L"OUTPUTS:1,0;-1,0;:OUTPUTS\n";
	str += L"PARAMS::PARAMS\n";
	str += L":NETWORK\n";

	uns::nn::network_representation<uns::nn::neuron_representation<float>> repr(str);

	uns::nn::nonrecursive_reverse_network<float> net(repr, ido, odo);

	ido.Set(0, 1);
	ido.Set(1, 1);
	ido.Set(2, 1);
	ido.Set(3, 1);

	net.React();

	odo.Set(0, 1);

	net._React();

	auto repr2 = net.Represent();

	auto str1 = repr2.String() + L"\n";

	std::wcout << str << std::endl;
	std::wcout << str1 << std::endl;
	std::wcout << std::boolalpha << (str == str1) << std::endl;

	/*//str += L"NETWORK:\n";
	str += L"23 Line_Perc:\n";
	str += L"\t0 [(-1,1,1.230000;-2,2,2.230000;-3,3,3.230000;)-0.170000,5.960000,(1.000000,2.000000)][_(2,3,4;5,6,7;)(1.000000,2.000000,0.500000,true)]\n";
	str += L"\t1 [(-4,4,-4.230000;-5,5,-5.230000;-6,6,-6.230000;)-0.140000,7.960000,(-1.000000,-2.000000)][_(8,9,10;11,12,13;)(4.000000,5.000000,0.250000,false)]\n";
	str += L":23 Line_Perc";

	uns::nn::layer_representation<uns::nn::reverse_neuron_representation<float>> layer(str);

	size_t numb = 0;
	auto str1 = layer.String(23, 0, numb);

	std::wcout << str << std::endl;
	std::wcout << str1 << std::endl;
	std::wcout << std::boolalpha << (str == str1) << std::endl;*/

	std::cout << "FINISH" << std::endl;
}





#undef UNS_FILENAME