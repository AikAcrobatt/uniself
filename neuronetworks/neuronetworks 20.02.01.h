#pragma once

#include <functional>

#include "uniself/strings.h"
#include "uniself/mathematics.h"
//#include "uniself/matrix.h"

namespace uns {
	namespace nn {

		using signal_t = double;
		using string_t = std::wstring;



		class neuron_interface {
		public:
			virtual uns::nn::string_t Type() const = 0;
			virtual signal_t R() const = 0;
			virtual void R(const signal_t R) = 0;
			virtual int Index() const = 0;
			virtual void React() = 0;
		};


		class activator {
		protected:
			string_t type = L"";
			signal_t(*main_functor) (signal_t, std::vector<signal_t>&);
			signal_t(*dF_dS) (const signal_t, const std::vector<signal_t>&);
			signal_t(*dF_dp) (size_t, const signal_t, const std::vector<signal_t>&);
		public:
			activator() :
				type(L"Zero"),
				main_functor([](signal_t s, std::vector<signal_t>& p) -> signal_t { return 0; }),
				dF_dS([](const signal_t s, const std::vector<signal_t>& p) -> signal_t { return 0; }),
				dF_dp([](size_t index, const signal_t s, const std::vector<signal_t>& p) -> signal_t { return 0; })
			{};
			activator(
				const string_t& activator_type,
				signal_t(* const main) (signal_t, std::vector<signal_t>&),
				signal_t(* const _dS) (const signal_t, const std::vector<signal_t>&),
				signal_t(* const _dp) (size_t, const signal_t, const std::vector<signal_t>&)
			) : type(activator_type), main_functor(main), dF_dS(_dS), dF_dp(_dp) {};
			activator(const activator& a) : type(a.type), main_functor(a.main_functor), dF_dS(a.dF_dS), dF_dp(a.dF_dp) {};
			activator& operator=(const activator& a) { type = a.type; main_functor = a.main_functor; dF_dS = a.dF_dS; dF_dp = a.dF_dp; return *this; };
			activator(activator&& a) : type(std::move(a.type)), main_functor(a.main_functor), dF_dS(a.dF_dS), dF_dp(a.dF_dp) {};
			activator& operator=(activator&& a) { type = std::move(a.type); main_functor = a.main_functor; dF_dS = a.dF_dS; dF_dp = a.dF_dp; return *this; };
			~activator() {};
			uns::nn::string_t Type() const { return type; };
			signal_t operator()(signal_t S, std::vector<signal_t>& params) const { return main_functor(S, params); };
			signal_t _dS(const signal_t S, const std::vector<signal_t>& params) const { return dF_dS(S, params); };
			signal_t _dp(size_t index, const signal_t S, const std::vector<signal_t>& params) const { return dF_dp(index, S, params); };
		};

	};


		template<>
		uns::nn::activator unified_cast<uns::nn::activator>(const std::wstring& str);
		template<>
		uns::nn::activator unified_cast<uns::nn::activator>(const std::string& str);


	namespace nn {


		class collector {
		protected:
			string_t type = L"";
			signal_t(*main_functor) (std::vector<std::pair<neuron_interface*, signal_t>>&, std::vector<signal_t>&);
			signal_t(*dS_dr) (size_t, const std::vector<std::pair<neuron_interface*, signal_t>>&, const std::vector<signal_t>&);
			signal_t(*dS_dw) (size_t, const std::vector<std::pair<neuron_interface*, signal_t>>&, const std::vector<signal_t>&);
			signal_t(*dS_dp) (size_t, const std::vector<std::pair<neuron_interface*, signal_t>>&, const std::vector<signal_t>&);
		public:
			collector() :
				type(L"Zero"),
				main_functor([](std::vector<std::pair<neuron_interface*, signal_t>>& l, std::vector<signal_t>& p) -> signal_t { return 0; }),
				dS_dr([](size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>& l, const std::vector<signal_t>& p) -> signal_t { return 0; }),
				dS_dw([](size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>& l, const std::vector<signal_t>& p) -> signal_t { return 0; }),
				dS_dp([](size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>& l, const std::vector<signal_t>& p) -> signal_t { return 0; })
			{};
			collector(
				const string_t& collector_type,
				signal_t(* const main) (std::vector<std::pair<neuron_interface*, signal_t>>&, std::vector<signal_t>&),
				signal_t(* const _dr) (size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>&, const std::vector<signal_t>&),
				signal_t(* const _dw) (size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>&, const std::vector<signal_t>&),
				signal_t(* const _dp) (size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>&, const std::vector<signal_t>&)
			) : type(collector_type), main_functor(main), dS_dr(_dr), dS_dw(_dw), dS_dp(_dp) {};
			collector(const collector& c) : type(c.type), main_functor(c.main_functor), dS_dr(c.dS_dr), dS_dw(c.dS_dw), dS_dp(c.dS_dp) {};
			collector& operator=(const collector& c) { type = c.type; main_functor = c.main_functor; dS_dr = c.dS_dr; dS_dw = c.dS_dw; dS_dp = c.dS_dp; return *this; };
			collector(collector&& c) : type(std::move(c.type)), main_functor(c.main_functor), dS_dr(c.dS_dr), dS_dw(c.dS_dw), dS_dp(c.dS_dp) {};
			collector& operator=(collector&& c) { type = std::move(c.type); main_functor = c.main_functor; dS_dr = c.dS_dr; dS_dw = c.dS_dw; dS_dp = c.dS_dp;  return *this; };
			~collector() {};
			uns::nn::string_t Type() const { return type; };
			signal_t operator()(std::vector<std::pair<neuron_interface*, signal_t>>& links, std::vector<signal_t>& params) const { return main_functor(links, params); };
			signal_t _dr(size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>& links, const std::vector<signal_t>& params) const { return dS_dr(index, links, params); };
			signal_t _dw(size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>& links, const std::vector<signal_t>& params) const { return dS_dw(index, links, params); };
			signal_t _dp(size_t index, const std::vector<std::pair<neuron_interface*, signal_t>>& links, const std::vector<signal_t>& params) const { return dS_dp(index, links, params); };
		};

	};
	

	template<>
	uns::nn::collector unified_cast<uns::nn::collector>(const std::wstring& str);
	template<>
	uns::nn::collector unified_cast<uns::nn::collector>(const std::string& str);


	namespace nn {


		class neuron_representation {
		public:
			string_t activator_type;
			string_t collector_type;
			signal_t r = 0;
			signal_t s = 0;
			std::vector<std::pair<int, signal_t>> links;
			std::vector<signal_t> params;

			neuron_representation() {};
			neuron_representation(string_t act_type, string_t coll_type, signal_t r_, signal_t s_, const std::vector<std::pair<int, signal_t>>& links_, const std::vector<signal_t>& params_) : activator_type(act_type), collector_type(coll_type), r(r_), s(s_), links(links_), params(params_) {};
			neuron_representation(const string_t& str) { Set(str); };
			neuron_representation(const neuron_representation& repr) : activator_type(repr.activator_type), collector_type(repr.collector_type), r(repr.r), s(repr.s), links(repr.links), params(repr.params) {};
			neuron_representation& operator=(const neuron_representation& repr) {
				activator_type = repr.activator_type;
				collector_type = repr.collector_type;
				r = repr.r;
				s = repr.s;
				links = repr.links;
				params = repr.params;
				return *this;
			};
			neuron_representation(neuron_representation&& repr) : activator_type(std::move(repr.activator_type)), collector_type(std::move(repr.collector_type)), r(repr.r), s(repr.s), links(std::move(repr.links)), params(std::move(repr.params)) {};
			neuron_representation& operator=(neuron_representation&& repr) {
				activator_type = std::move(repr.activator_type);
				collector_type = std::move(repr.collector_type);
				r = std::move(repr.r);
				s = std::move(repr.s);
				links = std::move(repr.links);
				params = std::move(repr.params);
				return *this;
			};
			~neuron_representation() {};
			virtual string_t String() const {
				//шаблон строчного представления:
				//[activator_collector(adress1,weight1;...;adressN,weightN;)param1,...paramN,(R,S)]

				string_t res = L"[" + activator_type + L"__" + collector_type + L"(";
				for (auto link : links) {
					res += uns::unified_cast<string_t>(link.first) + L"," + uns::unified_cast<string_t>(link.second) + L";";
				};
				res += L")";
				for (auto param : params) {
					res += uns::unified_cast<string_t>(param) + L",";
				};
				res += L"(" + uns::unified_cast<string_t>(r) + L"," + uns::unified_cast<string_t>(s) + L")]";

				return res;
			};
			virtual bool Set(const string_t& str) {
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, L"[", false, -1, L"]")) return false;
				string_t temp = L"";

				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L"__", false, -1, L"(")) return false;
				else activator_type = temp;
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L"(", false, -1, L"]")) return false;
				else collector_type = temp;

				string_t temp1 = L"";
				links.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L")")) break;
					else if (!uns::string::SeekerRead<string_t>(str, seeker, temp1, L";", false, -1, L")")) break;
					else {
						links.push_back(std::pair<int, signal_t>(uns::unified_cast<int>(temp), uns::unified_cast<signal_t>(temp1)));
					};
				} while (true);

				params.clear();
				if (!uns::string::SeekerSet<string_t>(str, seeker, L")", false, -1, L"]")) return false;
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L"(")) break;
					else params.push_back(uns::unified_cast<signal_t>(temp));
				} while (true);
				if (!uns::string::SeekerSet<string_t>(str, seeker, L"(", false, -1, L"]")) return false;
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L")")) return false;
				else r = uns::unified_cast<signal_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L")", false, -1, L"]")) return false;
				else s = uns::unified_cast<signal_t>(temp);

				return true;
			};
		};


		class nonrecursive_network;
		class nonrecursive_neuron : public neuron_interface {
			friend nonrecursive_network;
		protected:
			using weight_t = signal_t;
			using param_t = signal_t;

			int index_in_nn = 0;
			signal_t r = 0;
			signal_t s = 0;
			uns::nn::activator F;
			uns::nn::collector S;
			std::vector<std::pair<neuron_interface*, weight_t>> links;
			std::vector<param_t> params;
		public:
			nonrecursive_neuron() {};
			nonrecursive_neuron(const uns::nn::nonrecursive_neuron& n) = delete;
			nonrecursive_neuron& operator=(const uns::nn::nonrecursive_neuron& n) = delete;
			nonrecursive_neuron(uns::nn::nonrecursive_neuron&& n) = delete;
			nonrecursive_neuron operator=(uns::nn::nonrecursive_neuron&& n) = delete;
			~nonrecursive_neuron() {};
			neuron_representation Represent() const;
			uns::nn::string_t Type() const override { return F.Type() + S.Type(); };
			int Index() const override { return index_in_nn; };
			void Index(const int i) { index_in_nn = i; };
			signal_t R() const override { return r; };
			void R(const signal_t R) override { r = R; };
			signal_t C() const { return s; };
			void C(const signal_t S) { s = S; };
			void React() override { r = F(s, params); };
			void Collect() { s = S(links, params); };
		};

		inline uns::nn::neuron_representation uns::nn::nonrecursive_neuron::Represent() const {
			uns::nn::neuron_representation res;
			res.activator_type = F.Type();
			res.collector_type = S.Type();
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


		class reverse_neuron_representation : public neuron_representation {
		public:
			signal_t _r = 0;
			signal_t _s = 0;
			std::vector<std::pair<int, int>> _links;
			bool is_learning = true;
			signal_t not_dropout = 1.0;

			reverse_neuron_representation() {};
			reverse_neuron_representation(const neuron_representation& represent) : neuron_representation(represent.activator_type, represent.collector_type, represent.r, represent.s, represent.links, represent.params) { _links.clear(); };
			reverse_neuron_representation(const string_t& str) { Set(str); };
			reverse_neuron_representation(const reverse_neuron_representation& repr) : neuron_representation(repr), _r(repr._r), _s(repr._s), _links(repr._links), is_learning(repr.is_learning), not_dropout(repr.not_dropout) {};
			reverse_neuron_representation& operator=(const reverse_neuron_representation& repr) {
				activator_type = repr.activator_type;
				collector_type = repr.collector_type;
				links = repr.links;
				params = repr.params;
				r = repr.r;
				s = repr.s;
				_r = repr._r;
				_s = repr._s;
				_links = repr._links;
				is_learning = repr.is_learning;
				not_dropout = repr.not_dropout;
			};
			reverse_neuron_representation(reverse_neuron_representation&& repr) : neuron_representation(std::move(repr)), _r(repr._r), _s(repr._s), _links(std::move(repr._links)), is_learning(repr.is_learning), not_dropout(repr.not_dropout) {};
			reverse_neuron_representation& operator=(reverse_neuron_representation&& repr) {
				activator_type = std::move(repr.activator_type);
				collector_type = std::move(repr.collector_type);
				links = std::move(repr.links);
				params = std::move(repr.params);
				r = repr.r;
				s = repr.s;
				_r = repr._r;
				_s = repr._s;
				_links = std::move(repr._links);
				is_learning = repr.is_learning;
				not_dropout = repr.not_dropout;
			};
			reverse_neuron_representation& operator=(const neuron_representation& repr) {
				activator_type = repr.activator_type;
				collector_type = repr.collector_type;
				links = repr.links;
				params = repr.params;
				r = repr.r;
				s = repr.s;
				_r = 0;
				_s = 0;
				_links.clear();
				is_learning = true;
				not_dropout = 1.0;
			};
			reverse_neuron_representation& operator=(neuron_representation&& repr) {
				activator_type = std::move(repr.activator_type);
				collector_type = std::move(repr.collector_type);
				links = std::move(repr.links);
				params = std::move(repr.params);
				r = repr.r;
				s = repr.s;
				_r = 0;
				_s = 0;
				_links.clear();
				is_learning = true;
				not_dropout = 1.0;
			};
			~reverse_neuron_representation() {};
			virtual string_t String() const override {
				//шаблон строчного представления:
				//[Reverse_N(adress1,place1;...;adressN,placeN;)(_R,_S,not_dropout,is_learning)]

				string_t res = neuron_representation::String();
				res += L"[Reverse_N(";
				for (auto _link : _links) {
					res += uns::unified_cast<string_t>(_link.first) + L"," + uns::unified_cast<string_t>(_link.second) + L";";
				};
				res += L")";
				res += L"(" + uns::unified_cast<string_t>(_r)
					+ L"," + uns::unified_cast<string_t>(_s)
					+ L"," + uns::unified_cast<string_t>((uns::math::More(not_dropout, 1.0) ? 1.0 - 1.0 : (uns::math::Less(not_dropout, 0.0) ? 1.0 - 0.0 : 1.0 - not_dropout)))
					+ L"," + uns::unified_cast<string_t>(is_learning) + L")]";

				return res;
			};
			virtual bool Set(const string_t& str) override {
				if (!neuron_representation::Set(str)) return false;
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, L"[Reverse_N(", false, -1)) return false;

				string_t temp = L"";
				string_t temp1 = L"";
				_links.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L")")) break;
					else if (!uns::string::SeekerRead<string_t>(str, seeker, temp1, L";", false, -1, L")")) break;
					else {
						_links.push_back(std::pair<int, int>(uns::unified_cast<int>(temp), uns::unified_cast<int>(temp1)));
					};
				} while (true);

				if (!uns::string::SeekerSet<string_t>(str, seeker, L")(", false, -1, L")]")) return false;
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L")")) return false;
				else _r = uns::unified_cast<signal_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L")")) return false;
				else _s = uns::unified_cast<signal_t>(temp);
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L")")) return false;
				else not_dropout = uns::unified_cast<signal_t>(temp);
				not_dropout = (uns::math::More(not_dropout, 1.0) ? 1.0 - 1.0 : (uns::math::Less(not_dropout, 0.0) ? 1.0 - 0.0 : 1.0 - not_dropout));
				if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L")", false, -1, L"]")) return false;
				else is_learning = uns::unified_cast<bool>(temp);

				return true;
			};
		};


		class nonrecursive_reverse_network;
		class nonrecursive_reverse_neuron : public nonrecursive_neuron {
			friend nonrecursive_reverse_network;
		protected:
			signal_t _r = 0;
			signal_t _s = 0;
			std::vector<std::pair<nonrecursive_reverse_neuron*, size_t>> _links;
			bool is_learning = true;
			signal_t not_dropout = 1.0;
		public:
			nonrecursive_reverse_neuron() {};
			nonrecursive_reverse_neuron(const uns::nn::nonrecursive_reverse_neuron& n) = delete;
			nonrecursive_reverse_neuron& operator=(const uns::nn::nonrecursive_reverse_neuron& n) = delete;
			nonrecursive_reverse_neuron(uns::nn::nonrecursive_reverse_neuron&& n) = delete;
			nonrecursive_reverse_neuron& operator=(uns::nn::nonrecursive_reverse_neuron&& n) = delete;
			~nonrecursive_reverse_neuron() {};
			uns::nn::reverse_neuron_representation Represent() const;
			signal_t _R() const { return _r; };
			void _R(const signal_t R) { _r = R; };
			signal_t _C() const { return _s; };
			void _C(const signal_t S) { _s = S; };
			void _React() {
				_r = dF_dS() * _s;
			};
			void _Collect() {
				_s = uns::unified_cast<signal_t>(0);
				for (auto _link : _links) {
					_s += _link.first->_r * _link.first->dS_dr(_link.second);
				};
			};
			bool IsLearning() const { return is_learning; };
			void SetLearningStatus(bool islearning) { is_learning = islearning; };
			signal_t Dropout() const { return (1 - not_dropout); };
			void SetDropout(signal_t set_dropout_status) {
				if (uns::math::Less(set_dropout_status, 0.0)) not_dropout = 1.0;
				else if (uns::math::More(set_dropout_status, 1.0)) not_dropout = 0.0;
				else not_dropout = 1 - set_dropout_status;
			};
			signal_t dS_dr(size_t index) const { return S._dr(index, links, params); };
			signal_t dS_dw(size_t index) const { return S._dw(index, links, params); };
			signal_t dS_dp(size_t index) const { return S._dp(index, links, params); };
			signal_t dF_dS() const { return F._dS(s, params); };
			signal_t dF_dp(size_t index) const { return F._dp(index, s, params); };
		};

		inline uns::nn::reverse_neuron_representation uns::nn::nonrecursive_reverse_neuron::Represent() const {
			uns::nn::reverse_neuron_representation res(uns::nn::nonrecursive_neuron::Represent());
			res._r = _r;
			res._s = _s;
			for (auto _link : _links) {
				res._links.push_back(std::pair<int, size_t>(_link.first->Index(), _link.second));
			};

			return res;
		};


		class input_data_object_interface {
		public:
			virtual signal_t Get(size_t index) const = 0;
			virtual size_t Size() const = 0;
			virtual signal_t* GetElement(size_t index) const = 0;
		};


		class input_neuron : public neuron_interface {
		protected:
			signal_t* value_of_cell = nullptr;
			signal_t r = 0;
			size_t num_of_cell = 1;
		public:
			input_neuron() {};
			input_neuron(size_t num_of_input, const input_data_object_interface* const ido) { Set(num_of_input, ido); };
			input_neuron(const input_neuron& inp) = delete;
			input_neuron& operator=(const input_neuron& inp) = delete;
			input_neuron(input_neuron&& inp) = delete;
			input_neuron& operator=(input_neuron&& inp) = delete;
			~input_neuron() {};
			uns::nn::string_t Type() const override { return L"Input"; };
			signal_t R() const override { return r; };
			void R(const signal_t R) override { r = R; };
			int Index() const override { return -uns::unified_cast<int>(num_of_cell); };
			void React() override { r = *value_of_cell; };
			void Set(size_t num_of_input, const input_data_object_interface* const ido) {
				if (num_of_input = 0 || num_of_input >= ido->Size()) num_of_cell = 1;
				else num_of_cell = num_of_input;
				value_of_cell = ido->GetElement(num_of_cell);
			};
		};


		class network_representation {
		public:
			std::vector<neuron_representation> neurons;
			std::vector<size_t> outputs;

			network_representation() {};
			network_representation(const string_t& str, string_t delimiter) { Set(str, delimiter); };
			network_representation(const network_representation& repr) : neurons(repr.neurons), outputs(repr.outputs) {};
			network_representation& operator=(const network_representation& repr) { neurons = repr.neurons; outputs = repr.outputs;  return *this; };
			network_representation(network_representation&& repr) : neurons(std::move(repr.neurons)), outputs(std::move(repr.outputs)) {};
			network_representation& operator=(network_representation&& repr) { neurons = std::move(repr.neurons); outputs = std::move(repr.outputs); return *this; };
			~network_representation() {};
			string_t String(string_t delimiter = L"\n") const {
				string_t res = L"NETWORK:" + delimiter;
				for (auto neuron : neurons)
					res += neuron.String() + delimiter;
				res += L"OUTPUTS:";
				for (auto output : outputs)
					res += uns::unified_cast<string_t>(output) + L",";
				res += L":OUTPUTS" + delimiter;
				res += L":NETWORK";

				return res;

			};
			bool Set(const string_t& str, string_t delimiter) {
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, L"NETWORK:" + delimiter, false, -1)) return false;

				string_t temp = L"";
				neurons.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, delimiter, false, -1, L"OUTPUTS:")) break;
					else neurons.push_back(uns::nn::neuron_representation(temp));
				} while (true);

				if (!uns::string::SeekerSet<string_t>(str, seeker, L"OUTPUTS:", false, -1)) return false;
				temp = L"";
				outputs.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L":OUTPUTS")) break;
					else outputs.push_back(uns::unified_cast<size_t>(temp));
				} while (true);

				return true;
			};
		};


		class reverse_network_representation {
		public:
			std::vector<reverse_neuron_representation> neurons;
			std::vector<size_t> outputs;

			reverse_network_representation() {};
			reverse_network_representation(const string_t& str, string_t delimiter) { Set(str, delimiter); };
			reverse_network_representation(const reverse_network_representation& repr) : neurons(repr.neurons), outputs(repr.outputs) {};
			reverse_network_representation& operator=(const reverse_network_representation& repr) { neurons = repr.neurons; outputs = repr.outputs;  return *this; };
			reverse_network_representation(reverse_network_representation&& repr) : neurons(std::move(repr.neurons)), outputs(std::move(repr.outputs)) {};
			reverse_network_representation& operator=(reverse_network_representation&& repr) { neurons = std::move(repr.neurons); outputs = std::move(repr.outputs); return *this; };
			~reverse_network_representation() {};
			string_t String(string_t delimiter = L"\n") const {
				string_t res = L"NETWORK:" + delimiter;
				for (auto neuron : neurons)
					res += neuron.String() + delimiter;
				res += L"OUTPUTS:";
				for (auto output : outputs)
					res += uns::unified_cast<string_t>(output) + L",";
				res += L":OUTPUTS" + delimiter;
				res += L":NETWORK";

				return res;

			};
			bool Set(const string_t& str, string_t delimiter) {
				size_t seeker = 0;
				if (!uns::string::SeekerSet<string_t>(str, seeker, L"NETWORK:" + delimiter, false, -1)) return false;

				string_t temp = L"";
				neurons.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, delimiter, false, -1, L"OUTPUTS:")) break;
					else neurons.push_back(uns::nn::reverse_neuron_representation(temp));
				} while (true);

				if (!uns::string::SeekerSet<string_t>(str, seeker, L"OUTPUTS:", false, -1)) return false;
				temp = L"";
				outputs.clear();
				do {
					if (!uns::string::SeekerRead<string_t>(str, seeker, temp, L",", false, -1, L":OUTPUTS")) break;
					else outputs.push_back(uns::unified_cast<size_t>(temp));
				} while (true);

				return true;
			};
		};
	};

	
	template<typename network_convert_to_t>
	network_convert_to_t unified_cast(const uns::nn::network_representation& net) {
		static_assert(false, "Uniself unified_cast function does not support current type cast");
		return NULL;
	};
	template<>
	uns::nn::network_representation unified_cast<uns::nn::network_representation>(const uns::nn::network_representation& net) {
		return net;
	};
	template<>
	uns::nn::reverse_network_representation unified_cast<uns::nn::reverse_network_representation>(const uns::nn::network_representation& net) {
		uns::nn::reverse_network_representation res;
		for (auto neuron : net.neurons)
			res.neurons.push_back(neuron);
		res.outputs = net.outputs;
		return res;
	};
	template<typename network_convert_to_t>
	network_convert_to_t unified_cast(const uns::nn::reverse_network_representation& net) {
		static_assert(false, "Uniself unified_cast function does not support current type cast");
		return NULL;
	};
	template<>
	uns::nn::reverse_network_representation unified_cast<uns::nn::reverse_network_representation>(const uns::nn::reverse_network_representation& net) {
		return net;
	};
	template<>
	uns::nn::network_representation unified_cast<uns::nn::network_representation>(const uns::nn::reverse_network_representation& net) {
		uns::nn::network_representation res;
		for (auto neuron : net.neurons)
			res.neurons.push_back(neuron);
		res.outputs = net.outputs;
		return res;
	};

	namespace nn {


		class network_interface {
		protected:
			std::vector<neuron_interface*> outputs;
			std::vector<input_neuron*> inputs;
		public:
			virtual void React() = 0;

		};


		class nonrecursive_network : public network_interface {
		protected:
			std::vector<nonrecursive_neuron*> neurons;
		public:
			nonrecursive_network() {};
			nonrecursive_network(const network_representation& repr, const uns::nn::input_data_object_interface* const ido);
			nonrecursive_network(const nonrecursive_network& net) = delete;
			nonrecursive_network operator=(const nonrecursive_network& net) = delete;
			nonrecursive_network(nonrecursive_network&& net) = delete;
			nonrecursive_network operator=(nonrecursive_network&& net) = delete;
			~nonrecursive_network() {
				for (auto& neuron : neurons)
					delete neuron;
				for (auto& input : inputs)
					delete input;
			};
			network_representation Represent();
			void React(const input_data_object_interface& ido) {
				for (auto& input : inputs) {
					input->Set(input->Index(), &ido);
					input->React();
				};
				for (auto& neuron : neurons) {
					neuron->Collect();
					neuron->React();
				};
			};
			void React() override {
				for (auto& input : inputs)
					input->React();
				for (auto& neuron : neurons) {
					neuron->Collect();
					neuron->React();
				};
			};
			signal_t R(size_t index) const { return neurons[index]->R(); };
			signal_t O(size_t index) const { return outputs[index]->R(); };
			signal_t I(size_t index) const { return inputs[index]->R(); };
			size_t Neurons() const { return neurons.size(); };
			size_t Outputs() const { return outputs.size(); };
			size_t Inputs() const { return inputs.size(); };
		};

		inline nonrecursive_network::nonrecursive_network(const uns::nn::network_representation& repr, const uns::nn::input_data_object_interface* const ido) {
			inputs.clear();
			neurons.clear();
			outputs.clear();
			
			nonrecursive_neuron* next_neuron = nullptr;
			for (auto neuron : repr.neurons) {
				next_neuron = new nonrecursive_neuron();
				next_neuron->F = uns::unified_cast<uns::nn::activator>(neuron.activator_type);
				next_neuron->S = uns::unified_cast<uns::nn::collector>(neuron.collector_type);
				next_neuron->r = neuron.r;
				next_neuron->s = neuron.s;
				next_neuron->params = neuron.params;
				next_neuron->Index(uns::unified_cast<int>(neurons.size()));
				neurons.push_back(next_neuron);
			};
			
			std::vector<int> input_neurons_numbers;
			const int empty_meaning = -1;
			size_t input_number = 0;
			size_t neuron_number = 0;
			for (auto neuron : repr.neurons) {
				for (auto link : neuron.links) {
					input_number = uns::unified_cast<size_t>(-link.first);
					if (link.first < 0) {
						if (input_neurons_numbers.size() < input_number + 1)
							input_neurons_numbers.resize(input_number + 1, empty_meaning);
						if (input_neurons_numbers[input_number] == empty_meaning) {
							inputs.push_back(new input_neuron(input_number, ido));
							input_neurons_numbers[input_number] = uns::unified_cast<int>(inputs.size() - 1);
						};
						if (input_neurons_numbers[input_number] > empty_meaning)
							neurons[neuron_number]->links.push_back(std::pair<input_neuron*, signal_t>(inputs[uns::unified_cast<size_t>(input_neurons_numbers[input_number])], link.second));
					}
					else if (link.first < neurons.size())
							neurons[neuron_number]->links.push_back(std::pair<nonrecursive_neuron*, signal_t>(neurons[uns::unified_cast<size_t>(link.first)], link.second));
				};
				neuron_number++;
			};

			for (auto output : repr.outputs)
				if (output < repr.neurons.size())
					outputs.push_back(neurons[output]);
		};

		inline network_representation nonrecursive_network::Represent() {
			network_representation res;
			for (auto neuron : neurons)
				res.neurons.push_back(neuron->Represent());
			for (auto output : outputs)
				res.outputs.push_back(output->Index());

			return res;
		};


		class nonrecursive_reverse_network : public network_interface {
		protected:
			std::vector<nonrecursive_reverse_neuron*> neurons;
			std::vector<input_neuron*> _inputs;
		public:
			nonrecursive_reverse_network() {};
			nonrecursive_reverse_network(const reverse_network_representation& repr, const uns::nn::input_data_object_interface* const ido, const uns::nn::input_data_object_interface* const _ido);
			nonrecursive_reverse_network(const nonrecursive_reverse_network& net) = delete;
			nonrecursive_reverse_network operator=(const nonrecursive_reverse_network& net) = delete;
			nonrecursive_reverse_network(nonrecursive_reverse_network&& net) = delete;
			nonrecursive_reverse_network operator=(nonrecursive_reverse_network&& net) = delete;
			~nonrecursive_reverse_network() {
				for (auto& neuron : neurons)
					delete neuron;
				for (auto& input : inputs)
					delete input;
				for (auto& _input : _inputs)
					delete _input;
			};
			reverse_network_representation Represent();
			void React(const input_data_object_interface& const ido) {
				for (auto& input : inputs) {
					input->Set(input->Index(), &ido);
					input->React();
				};
				for (auto& neuron : neurons) {
					neuron->Collect();
					neuron->React();
				};
			};
			void React() override {
				for (auto& input : inputs)
					input->React();
				for (auto& neuron : neurons) {
					neuron->Collect();
					neuron->React();
				};
			};
			void _React(const input_data_object_interface& _ido) {
				for (auto& _input : _inputs) {
					_input->Set(_input->Index(), &_ido);
					_input->React();
				};
				for (auto neuron = neurons.rbegin(); neuron != neurons.rend(); neuron++) {
					(*neuron)->_Collect();
					(*neuron)->_React();
				};
			};
			void _React() {
				for (auto& _input : _inputs)
					_input->React();
				for (auto neuron = neurons.rbegin(); neuron != neurons.rend(); neuron++) {
					(*neuron)->_Collect();
					(*neuron)->_React();
				};
			};
			signal_t R(size_t index) const { return neurons[index]->R(); };
			signal_t O(size_t index) const { return outputs[index]->R(); };
			signal_t I(size_t index) const { return inputs[index]->R(); };
			size_t Neurons() const { return neurons.size(); };
			size_t Outputs() const { return outputs.size(); };
			size_t Inputs() const { return inputs.size(); };
			signal_t dS_dw(size_t neuron_idx, size_t weight_idx) const { return neurons[neuron_idx]->dS_dw(weight_idx); };
			signal_t dS_dp(size_t neuron_idx, size_t param_idx) const { return neurons[neuron_idx]->dS_dp(param_idx); };
			signal_t dF_dp(size_t neuron_idx, size_t param_idx) const { return neurons[neuron_idx]->dF_dp(param_idx); };
		};

		/*inline nonrecursive_reverse_network::nonrecursive_reverse_network(const uns::nn::reverse_network_representation& repr, const uns::nn::input_data_object_interface* const ido, const uns::nn::input_data_object_interface* const _ido) {
			inputs.clear();
			neurons.clear();
			outputs.clear();

			nonrecursive_reverse_neuron* next_neuron = nullptr;
			for (auto neuron : repr.neurons) {
				next_neuron = new nonrecursive_reverse_neuron();
				next_neuron->F = uns::unified_cast<uns::nn::activator>(neuron.activator_type);
				next_neuron->S = uns::unified_cast<uns::nn::collector>(neuron.collector_type);
				next_neuron->r = neuron.r;
				next_neuron->s = neuron.s;
				next_neuron->params = neuron.params;
				next_neuron->index_in_nn = uns::unified_cast<int>(neurons.size());
				next_neuron->_r = neuron._r;
				next_neuron->_s = neuron._s;
				next_neuron->SetLearningStatus(neuron.is_learning);
				next_neuron->SetDropout(uns::unified_cast<signal_t>(1) - neuron.not_dropout);
				neurons.push_back(next_neuron);
			};

			std::vector<int> input_neurons_numbers;
			std::vector<int> output_neurons_numbers;
			const int empty_meaning = -1;
			size_t input_number = 0;
			size_t neuron_number = 0;
			for (auto neuron : repr.neurons) {
				for (auto link : neuron.links) {
					if (link.first < 0) {
						input_number = uns::unified_cast<size_t>(-link.first);
						if (input_neurons_numbers.size() < input_number + 1)
							input_neurons_numbers.resize(input_number + 1, empty_meaning);
						if (input_neurons_numbers[input_number] == empty_meaning) {
							inputs.push_back(new input_neuron(input_number, ido));
							input_neurons_numbers[input_number] = uns::unified_cast<int>(inputs.size() - 1);
						};
						if (input_neurons_numbers[input_number] > empty_meaning)
							neurons[neuron_number]->links.push_back(std::pair<input_neuron*, signal_t>(inputs[uns::unified_cast<size_t>(input_neurons_numbers[input_number])], link.second));
					}
					else {
						if (link.first < neurons.size())
							neurons[neuron_number]->links.push_back(std::pair<nonrecursive_reverse_neuron*, signal_t>(neurons[uns::unified_cast<size_t>(link.first)], link.second));
					};
				};
				neuron_number++;
			};

			for (auto output : repr.outputs)
				if (output < repr.neurons.size())
					outputs.push_back(neurons[output]);
		};

		inline network_representation nonrecursive_reverse_network::Represent() {
			network_representation res;
			for (auto neuron : neurons)
				res.neurons.push_back(neuron->Represent());
			for (auto output : outputs)
				res.outputs.push_back(output->Index());

			return res;
		};*/

	};
};