#pragma once

#include "uniself/neuronetworks/nn_networks.h"

namespace uns {
	namespace nn {
		namespace genetic {

			//перечисление возможных направлений распространения сигнала
			enum class SIGNAL_DIRECTION_TYPES {
				Forward = 1,
				None = 0,
				Backward = -1
			};


			//Типы соединений слоев
			UNS_ENUM_DECLARATOR(layer_connection_type, size_t,
				ToNoone = 0, //ни к одному
				OneToEach = 1, //один к каждому
				AllToEach = 2, //все к каждому
				SomeToEach = 3, //несколько к каждому
				Single = 4 //только один к каждому
			);


			//АКТИВАТОРЫ ГЕНЕТИЧЕСКИХ НЕЙРОНОВ
			//Базовый класс
			template<typename signal_t>
			class activator : public uns::nn::activator<signal_t> {
			protected:
			public:
				activator() {};
				activator(const activator<signal_t>& a) {};
				activator<signal_t>& operator=(const activator<signal_t>& a) { return *this; };
				activator(activator<signal_t>&& a) {};
				activator<signal_t>& operator=(activator<signal_t>&& a) { return *this; };
				~activator() {};

				virtual uns::nn::string_t Type() const noexcept override { return uns::string_cast<string_t>("Zero"); };

				virtual signal_t operator()(const signal_t S, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual signal_t _dS(const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual signal_t _dp(size_t index, const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual size_t Params() const noexcept { return 0; };	//минимальное количество параметров

				virtual size_t CommonParams() const noexcept { return 0; };	//минимальное количество общих параметров
			};


			//Конкретные классы активаторов
			template<typename signal_t>
			class Line : public uns::nn::genetic::activator<signal_t> {
			protected:
			public:
				Line() {};
				Line(const Line<signal_t>& a) {};
				Line<signal_t>& operator=(const Line<signal_t>& a) { return *this; };
				Line(Line<signal_t>&& a) {};
				Line<signal_t>& operator=(Line<signal_t>&& a) { return *this; };
				~Line() {};

				virtual uns::nn::string_t Type() const noexcept override { return uns::string_cast<string_t>("Line"); };

				virtual signal_t operator()(const signal_t S, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? params[0] * S : S); };

				virtual signal_t _dS(const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? params[0] : 1); };

				virtual signal_t _dp(size_t index, const signal_t S, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? S : 0); };

				virtual size_t Params() const noexcept override { return 1; };

				virtual size_t CommonParams() const noexcept override { return 1; };
			};


			//Реализация функции конвертации строки в активатор генетического нейрона
			template<typename signal_t>
			std::shared_ptr<uns::nn::genetic::activator<signal_t>> StringToActivator(const uns::nn::string_t& str) {
				if (str == uns::string_cast<string_t>("Line"))
					return std::make_shared<uns::nn::genetic::Line<signal_t>>();
				else return std::make_shared<uns::nn::genetic::activator<signal_t>>();
			};


			//КОЛЛЕКТОРЫ ГЕНЕТИЧЕСКИХ НЕЙРОНОВ
			//Базовый класс
			template<typename signal_t>
			class collector : public uns::nn::collector<signal_t> {
			protected:
			public:
				collector() {};
				collector(const collector<signal_t>& c) {};
				collector<signal_t>& operator=(const collector<signal_t>& c) { return *this; };
				collector(collector<signal_t>&& c) {};
				collector<signal_t>& operator=(collector<signal_t>&& c) { return *this; };
				~collector() {};

				virtual uns::nn::string_t Type() const noexcept override { return uns::string_cast<string_t>("Zero"); };

				virtual signal_t operator()(std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual signal_t _dr(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual signal_t _dw(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual signal_t _dp(size_t index, const std::vector<std::pair<neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return 0; };

				virtual int Links() const noexcept override { return 0; };	//предписываемое количество связей слоя (положительное требует именно столько, отрицательно = любое)

				virtual size_t Params() const noexcept { return 0; };	//минимальное количество параметров

				virtual size_t CommonParams() const noexcept { return 0; };	//минимальное количество общих параметров

				virtual SIGNAL_DIRECTION_TYPES Direction() const noexcept { return SIGNAL_DIRECTION_TYPES::None; };	//предписываемое направление распространения сигнала

				virtual layer_connection_type ConnectionTypeCheck(const layer_connection_type& conn) const noexcept { return layer_connection_type::ToNoone; }; //возвращает тот же самый тип соединения, если данный коллектор поддерживает conn, и возвращает тот, на который нужно заменить, если conn не поддерживается
			};


			//Конкретные классы коллекторов
			template<typename signal_t>
			class Perc : public uns::nn::genetic::collector<signal_t> {
			protected:
			public:
				Perc() {};
				Perc(const Perc<signal_t>& c) {};
				Perc<signal_t>& operator=(const Perc<signal_t>& c) { return *this; };
				Perc(Perc<signal_t>&& c) {};
				Perc<signal_t>& operator=(Perc<signal_t>&& c) { return *this; };
				~Perc() {};

				virtual uns::nn::string_t Type() const noexcept { return uns::string_cast<string_t>("Perc"); };

				virtual signal_t operator()(std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const {
					signal_t res = 0;
					for (auto link : links)
						res += link.first->R() * link.second;
					if (params.size() > 0) res += params[0];
					return res;
				};

				virtual signal_t _dr(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return links[index].second; };

				virtual signal_t _dw(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return links[index].first->R(); };

				virtual signal_t _dp(size_t index, const std::vector<std::pair<uns::nn::neuron_interface<signal_t>*, signal_t>>& links, const std::vector<signal_t>& params, const std::vector<signal_t>& common_params) const override { return (params.size() > 0 ? 1 : 0; };

				virtual int Links() const noexcept override { return -1; };	//предписываемое количество связей слоя (положительное требует именно столько, отрицательно = любое)

				virtual size_t Params() const noexcept override { return 1; };	//минимальное количество параметров

				virtual size_t CommonParams() const noexcept override { return 0; };	//минимальное количество общих параметров

				virtual SIGNAL_DIRECTION_TYPES Direction() const noexcept override { return SIGNAL_DIRECTION_TYPES::Forward; };	//предписываемое направление распространения сигнала

				virtual layer_connection_type ConnectionTypeCheck(const layer_connection_type& conn) const noexcept override {	 //возвращает тот же самый тип соединения, если данный коллектор поддерживает conn, и возвращает тот, на который нужно заменить, если conn не поддерживается
					if (conn == layer_connection_type::ToNoone)
						return layer_connection_type::AllToEach;
					return conn;
				};
			};


			//Реализация функции конвертации строки в коллектор генетического нейрона
			template<typename signal_t>
			std::shared_ptr<uns::nn::genetic::collector<signal_t>> StringToCollector(const uns::nn::string_t& str) {
				if (str == uns::string_cast<string_t>("Perc"))
					return std::make_shared<uns::nn::genetic::Perc<signal_t>>();
				else return std::make_shared<uns::nn::genetic::collector<signal_t>>();
			};


			//ДЕСКРИПТОР ТИПОВ НЕЙРОНОВ
			template<typename signal_t>
			class neuron_types_descriptor {
			private:
				std::vector<std::pair<std::shared_ptr<uns::nn::genetic::activator<signal_t>>, std::shared_ptr<uns::nn::genetic::collector<signal_t>>>> neuron_types;
			public:
				neuron_types_descriptor() {
					string_t str = string_t();
					string_t temp = string_t();
					string_t::size_type seeker = 0;
					string_t activator_type = string_t();
					string_t collector_type = string_t();

					for (size_t neuron_cascade = 0; neuron_cascade < ctx[CTX_CCASCADES::Neuro].Total(); neuron_cascade++) {
						str = ctx[CTX_CCASCADES::Neuro].Id(neuron_cascade);
						seeker = 0;
						if (!uns::string::SeekerRead<string_t>(str, seeker, temp, uns::string_cast<string_t>("_"), false, -1)) {
							activator_type = "Zero";
							collector_type = "Zero";
						}
						else {
							activator_type = temp;
							collector_type = str.substr(seeker);
						};

						neuron_types.push_back(
							std::make_pair<std::shared_ptr<uns::nn::genetic::activator<signal_t>>, std::shared_ptr<uns::nn::genetic::collector<signal_t>>>(
								uns::nn::genetic::StringToActivator<signal_t>(activator_type),
								uns::nn::genetic::StringToCollector<signal_t>(collector_type)
								));
					};
				};
				neuron_types_descriptor(const neuron_types_descriptor& copying_obj) = delete;
				neuron_types_descriptor& operator=(const neuron_types_descriptor& copying_obj) = delete;
				neuron_types_descriptor(neuron_types_descriptor&& moving_obj) = delete;
				neuron_types_descriptor& operator=(neuron_types_descriptor&& moving_obj) = delete;
				~neuron_types_descriptor() = default;

				size_t Params(size_t index) const { return fmax(neuron_types[index].first.Params(), neuron_types[index].second.Params()); };	//минимальное количество параметров

				size_t CommonParams(size_t index) const { return fmax(neuron_types[index].first.CommonParams(), neuron_types[index].second.CommonParams()); };	//минимальное количество общих параметров

				int Links(size_t index) const { return neuron_types[index].second.Links(); };	//предписываемое количество связей слоя (положительное требует именно столько, отрицательно = любое)

				SIGNAL_DIRECTION_TYPES Direction(size_t index) const { return neuron_types[index].second.Direction(); };	//предписываемое направление распространения сигнала

				layer_connection_type ConnectionTypeCheck(size_t index, const layer_connection_type& conn) const noexcept { return neuron_types[index].second.ConnectionTypeCheck(conn); }; 	 //возвращает тот же самый тип соединения, если данный коллектор поддерживает conn, и возвращает тот, на который нужно заменить, если conn не поддерживается
			};

		};

		//Реализация функции конвертации строки в активатор генетического нейрона
		template<typename signal_t>
		std::shared_ptr<uns::nn::activator<signal_t>> StringToActivator(const uns::nn::string_t& str) { return uns::nn::genetic::StringToActivator<signal_t>(str); };


		//Реализация функции конвертации строки в коллектор генетического нейрона
		template<typename signal_t>
		std::shared_ptr<uns::nn::collector<signal_t>> StringToCollector(const uns::nn::string_t& str) { return uns::nn::genetic::StringToCollector<signal_t>(str); };
	};
};