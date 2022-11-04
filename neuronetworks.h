#pragma once

#define UNS_HEADER_NEURONETWORKS

#include <string>



namespace uns::nn {

	namespace general {
		//this namespace is intended to contain prototypes and interfaces


		//an interface of input-data provider
		template<typename signal_t>
		class input_data_object {
		public:
			virtual signal_t* get(std::size_t index) const = 0;
			virtual int size() const = 0;
		};


		//an interface of general neuron
		template<typename signal_t>
		class neuron {
		public:
			virtual std::u8string type() const noexcept = 0;
			virtual signal_t R() const noexcept = 0;
			virtual void R(const signal_t R) noexcept = 0;
			virtual std::pair<int, std::size_t> index() const noexcept = 0;
			virtual bool is_reversible() const noexcept = 0;
			virtual signal_t dropout() const noexcept = 0;
			virtual void react(const std::vector<signal_t>& common_params) = 0;
		};


		//an interface of general neuro network
		template<typename signal_t>
		class network_interface {
		protected:
			std::vector<neuron<signal_t>*> outputs;
			std::vector<neuron<signal_t>*> inputs;	//TODO here was an std::vector<input_neuron<signal_t>*> inputs
		public:
			virtual void react() = 0;
		};

	};


};