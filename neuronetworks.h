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
			virtual signal_t* get(int) const = 0;
			virtual int size() const = 0;
		};


		template<typename signal_t>
		class network_params {
		public:
			virtual signal_t get(int) const = 0;
		};


		//an interface of general neuron
		template<typename signal_t>
		class neuron {
		public:
			virtual std::u8string type() const noexcept = 0;
			virtual signal_t R() const noexcept = 0;
			virtual void R(const signal_t) noexcept = 0;
			virtual std::pair<int, int> adress() const noexcept = 0;
			virtual bool is_reversible() const noexcept = 0;
			virtual signal_t dropout() const noexcept = 0;
			virtual void react(const network_params&) = 0;
		};


		//an interface of general neuro network
		template<typename signal_t>
		class network {
		protected:
			std::vector<neuron<signal_t>*> outputs;
			std::vector<neuron<signal_t>*> inputs;	//TODO here was an std::vector<input_neuron<signal_t>*> inputs
		public:
			virtual void react() = 0;
		};


		//basic activator class can be used as default activator
		template<typename signal_t>
		class activator {
		protected:
			signal_t value_ = signal_t(0);
		public:
			activator() noexcept {};
			activator(const activator& obj) noexcept :
				value_(obj.value_)
			{};
			activator& operator=(const activator& obj) noexcept {
				if(this == &obj) return *this;

				value_ = obj.value_;
				
				return *this;
			};
			activator(activator&& obj) noexcept :
				value_(std::move(obj.value_))
			{};
			activator& operator=(activator&& obj) noexcept {
				if(this == &obj) return *this;

				value_ = std::move(obj.value_);

				return *this;
			};
			~activator() noexcept {};

			const signal_t& value() const noexcept { return value_; };
			signal_t& value() noexcept { return value_; };

			virtual std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(signal_t, const network_params&) { return signal_t(0); };

			virtual signal_t _dS(signal_t, const network_params&) const { return signal_t(0); };

			virtual signal_t _dp(int, signal_t, const network_params&) const { return signal_t(0); };
		};


		//basic collector class can be used as default collector
		template<typename signal_t>
		class collector {
		protected:
			signal_t value_ = signal_t(0);
		public:
			collector() noexcept {}; 
			collector(const collector& obj) noexcept :
				value_(obj.value_) {};
			collector& operator=(const collector& obj) noexcept {
				if(this == &obj) return *this;

				value_ = obj.value_;

				return *this;
			};
			collector(collector&& obj) noexcept :
				value_(std::move(obj.value_)) {};
			collector& operator=(collector&& obj) noexcept {
				if(this == &obj) return *this;

				value_ = std::move(obj.value_);

				return *this;
			};
			~collector() noexcept {};

			const signal_t& value() const noexcept { return value_; };
			signal_t& value() noexcept { return value_; };

			virtual std::u8string type() const noexcept { return u8"Zero"; };

			virtual signal_t operator()(const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };

			virtual signal_t _dr(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };

			virtual signal_t _dw(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };

			virtual signal_t _dp(int, const std::vector<std::pair<neuron<signal_t>*, signal_t>>&, const network_params&) { return signal_t(0); };
		};
	};


};