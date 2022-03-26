#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>

#include "uniself/mathematics.h"

namespace uns::population {

	//ÈÍÒÅĞÔÅÉÑ ŞÍÈÒÀ ÏÎÏÓËßÖÈÈ
	template<typename health_type, typename points_type>
	class unit_interface {
	public:
		using points_t = points_type;
		using health_t = health_type;

		virtual void AddPoints(const points_t& points_gain) { /*pregnancy += points_gain;*/ };

		virtual void Damage(const health_t health_decrease) { /*health -= health_decrease;*/ };

		virtual bool IsAlive() const { return false; /*return uns::math::More(health, 0.0F);*/ };

		virtual bool IsPregnant() const { return false; /*return uns::math::More(pregnancy, 0.0F);*/ };

		virtual void Condition() {};
	};


	//ÌÅÕÀÍÈÇÌ ÏÎĞßÄÊÀ ÏÎÏÓËßÖÈÈ
	//âñïîìîãàòåëüíûé êëàññ ıëåìåíòà ïîğÿäêà
	template<typename unit_t, typename order_t>
	class order_element {
	public:
		using unit_type = unit_t;
		using order_type = order_t;
		using index_t = typename order_t::size_t;
	protected:
		std::shared_ptr<unit_t> unit_ptr;
		order_t* order_ptr;
		index_t index = 0;
	public:
		order_element() = delete;
		order_element(const std::shared_ptr<unit_t>& unit, order_t& order, index_t idx) : 
			unit_ptr(unit), 
			order_ptr(&order), 
			index(idx) 
		{};
		order_element(const order_element& obj) : 
			unit_ptr(obj.unit_ptr),
			order_ptr(obj.order_ptr),
			index(obj.index)
		{};
		order_element& operator=(const order_element& obj) {
			if (this == &obj) return *this;

			unit_ptr = obj.unit_ptr;
			order_ptr = obj.order_ptr;
			index = obj.index;
			return *this; 
		};
		order_element(order_element&& obj) : 
			unit_ptr(std::move(obj.unit_ptr)), 
			order_ptr(obj.order_ptr), 
			index(std::move(obj.index)) 
		{};
		order_element& operator=(order_element&& obj) {
			if (this == &obj) return *this;

			unit_ptr = std::move(obj.unit_ptr);
			order_ptr = obj.order_ptr;
			index = std::move(obj.index);
			return *this; 
		};
		~order_element() {};

		operator unit_t& () { return *unit_ptr; };

		bool operator==(const std::shared_ptr<unit_t>& unit) const { return (unit == unit_ptr); };

		const unit_t& Unit() const { return *unit_ptr; };
		unit_t& Unit() { return *unit_ptr; };

		const order_t& Order() const { return *order_ptr; };
		order_t& Order() { return *order_ptr; };

		const index_t& Index() const { 
			return index;
		};
		index_t& Index() { 
			return index; 
		};

		std::shared_ptr<unit_t> Unit_ptr() { return unit_ptr; };
	};


	//Êëàññ èíòåğôåéñà ëèíåéíîãî îäíîíàïğàâëåííîãî ïîğÿäêà
	template<typename unit_t, typename order_t>
	class linear_order_interface {
	public:
		using element_t = order_element<unit_t, order_t>;
	protected:
		using linear_container = std::vector<element_t>;
	public:
		using iterator_t = typename linear_container::iterator;
		using size_t = typename linear_container::size_type;
		using unit_type = unit_t;
	protected:
		linear_container container;
		std::mt19937 engine;
		std::uniform_real_distribution<float> distribution;
	public:
		linear_order_interface(int seed = 0) : 
			engine(seed), 
			distribution(0.0F, 1.0F) 
		{};
		linear_order_interface(const linear_order_interface& obj) : 
			container(obj.container), 
			engine(obj.engine), 
			distribution(obj.distribution) 
		{};
		linear_order_interface& operator=(const linear_order_interface& obj) {
			if (this == &obj) return *this;

			container = obj.container; 
			engine = obj.engine; 
			distribution = obj.distribution; 
			return *this; 
		};
		linear_order_interface(linear_order_interface&& obj) : 
			container(std::move(obj.container)), 
			engine(std::move(obj.engine)), 
			distribution(std::move(obj.distribution)) 
		{};
		linear_order_interface& operator=(linear_order_interface&& obj) {
			if (this == &obj) return *this;

			container = std::move(obj.container); 
			engine = std::move(obj.engine); 
			distribution = std::move(obj.distribution); 
			return *this; 
		};
		~linear_order_interface() noexcept {};

		const iterator_t begin() const { return container.begin(); };
		iterator_t begin() { return container.begin(); };

		const iterator_t end() const { return container.end(); };
		iterator_t end() { return container.end(); };

		size_t Size() const { return container.size(); };

		auto operator[](size_t index) const { return container[index]; };
		auto& operator[](size_t index) { return container[index]; };

		void Clear() {
			container.clear();
		};

		void Push(const std::shared_ptr<unit_t>& unit_to_add) {
			container.push_back(element_t{ unit_to_add, *static_cast<order_t*>(this), container.size() });
		};

		void Pop(const iterator_t& iter) {
			container.erase(iter);
		};
		void Pop(const size_t& index) {
			container.erase(container.begin() + index);
		};

		void Indexate() {
			size_t index = 0;
			for (auto& unit : container)
				unit.Index() = index++;
		};

	};


	//Êëàññ íåóïîğÿäî÷åííîãî îäíîíàïğàâëåííîãî ïîğÿäêà
	template<typename unit_t>
	class unordered_order : public linear_order_interface<unit_t, unordered_order<unit_t>> {
	protected:
		using base_t = linear_order_interface<unit_t, unordered_order<unit_t>>;
	public:
		using element_t = typename base_t::element_t;
		using iterator_t = typename base_t::iterator_t;
		using size_t = typename base_t::size_t;
		using unit_type = unit_t;
	public:
		unordered_order(int seed = 0) : base_t(seed) {};
		unordered_order(const unordered_order& obj) : 
			base_t::container(obj.container), 
			base_t::engine(obj.engine), 
			base_t::distribution(obj.distribution)
		{};
		unordered_order& operator=(const unordered_order& obj) {
			if (this == &obj) return *this;

			base_t::container = obj.container; 
			base_t::engine = obj.engine; 
			base_t::distribution = obj.distribution; 
			return *this; 
		};
		unordered_order(unordered_order&& obj) : 
			base_t::container(std::move(obj.container)), 
			base_t::engine(std::move(obj.engine)), 
			base_t::distribution(std::move(obj.distribution)) 
		{};
		unordered_order& operator=(unordered_order&& obj) {
			if (this == &obj) return *this;

			base_t::container = std::move(obj.container); 
			base_t::engine = std::move(obj.engine); 
			base_t::distribution = std::move(obj.distribution); 
			return *this; 
		};
		~unordered_order() noexcept {};

		iterator_t Pick() {
			typename base_t::size_t index = 0;
			if (!(base_t::container.size() > 0)) return base_t::container.begin();

			index = static_cast<typename base_t::size_t>(base_t::container.size() * base_t::distribution(base_t::engine));
			if (index >= base_t::container.size()) index = base_t::container.size() - 1;

			return (base_t::container.begin() + index);
		};
	};


	//Êëàññ óïîğÿäî÷åííîãî îäíîíàïğàâëåííîãî ïîğÿäêà
	template<typename unit_t, bool(*predicate)(unit_t&, unit_t&)>
	class ordered_order : public linear_order_interface<unit_t, ordered_order<unit_t, predicate>> {
	protected:
		using base_t = linear_order_interface<unit_t, ordered_order<unit_t, predicate>>;
	public:
		using element_t = typename base_t::element_t;
		using iterator_t = typename base_t::iterator_t;
		using size_t = typename base_t::size_t;
		using unit_type = unit_t;
	public:
		ordered_order(int seed = 0) : base_t(seed) {};
		ordered_order(const ordered_order& obj) : 
			base_t::container(obj.container), 
			base_t::engine(obj.engine), 
			base_t::distribution(obj.distribution) 
		{};
		ordered_order& operator=(const ordered_order& obj) {
			if (this == &obj) return *this;

			base_t::container = obj.container; 
			base_t::engine = obj.engine; 
			base_t::distribution = obj.distribution; 
			return *this; 
		};
		ordered_order(ordered_order&& obj) : 
			base_t::container(std::move(obj.container)), 
			base_t::engine(std::move(obj.engine)), 
			base_t::distribution(std::move(obj.distribution)) 
		{};
		ordered_order& operator=(ordered_order&& obj) {
			if (this == &obj) return *this;

			base_t::container = std::move(obj.container); 
			base_t::engine = std::move(obj.engine); 
			base_t::distribution = std::move(obj.distribution); 
			return *this; 
		};
		~ordered_order() noexcept {};

		iterator_t PickLess(const element_t& elem) {
			size_t index = 0;
			if (!(base_t::container.size() - 1 - elem.Index() > 0)) return base_t::container.begin();

			index = static_cast<size_t>(
				(base_t::container.size() - elem.Index() - 1) * base_t::distribution(base_t::engine)
			) + elem.Index() + 1;

			if (index >= base_t::container.size()) index = base_t::container.size() - 1;
			return (base_t::container.begin() + index);
		};

		iterator_t PickMore(const element_t& elem) {
			size_t index = 0;
			if (!(elem.Index() > 1)) return base_t::container.begin();

			index = static_cast<size_t>(elem.Index() * base_t::distribution(base_t::engine));

			if (index >= elem.Index()) index = elem.Index() - 1;
			return (base_t::container.begin() + index);
		};

		virtual void Sort() {
			std::sort(base_t::container.begin(), base_t::container.end(), predicate);
			base_t::Indexate();
		};
	};


	//ÈÍÒÅĞÔÅÉÑ ÈÑÒÎ×ÍÈÊÀ ÏÈÒÀÍÈß
	template<typename order_element_t>
	class ration_source_interface {
	public:
		virtual void Feed(order_element_t& order_element) {};
	};


	//ÈÍÒÅĞÔÅÉÑ ÌÅÍÅÄÆÅĞÀ ĞÀÇÌÍÎÆÅÍÈß
	template<typename order_element_t>
	class breed_manager_interface {
	public:
		using unit_t = typename order_element_t::unit_type;
		virtual std::vector<std::shared_ptr<unit_t>> Breed(order_element_t& order_element) { 
			order_element.Unit().AddPoints(typename unit_t::points_t(-1));
			return { std::shared_ptr<unit_t>(new unit_t()) };
		};
	};


	//ÈÍÒÅĞÔÅÉÑ ÎÏÅĞÀÒÎĞÀ FATAL_ACT'À
	template<typename order_element_t>
	class fatal_act_operator_interface {
	public:
		virtual void operator()(order_element_t& order_element) {};
	};


	//ÈÍÒÅĞÔÅÉÑ ÌÅÕÀÍÈÇÌÀ ÊÎÍÒĞÎËß ×ÈÑËÅÍÍÎÑÒÈ
	class basic_number_controller {
	public:
		template<typename population_t>
		void operator()(population_t& pop_obj) {};
	};

	
	//ÁÀÇÎÂÛÉ ÊËÀÑÑ ÏÎÏÓËßÖÈÎÍÍÎÉ ÌÀØÈÍÛ
	template<typename order_t, typename ration_source_t, typename breed_manager_t, typename fatal_act_operator_t, typename number_control_t>
	class basic_machine {
	public:
		using unit_t = typename order_t::unit_type;
		using index_t = typename order_t::size_t;
		using order_elemnt_t = typename order_t::element_t;
		using points_t = typename unit_t::points_t;
		using health_t = typename unit_t::health_t;
	public:
		order_t units;
		std::deque<std::shared_ptr<unit_t>> birth_queue;
		ration_source_t ration_source;
		breed_manager_t breed_manager;
		fatal_act_operator_t fatal_act;
		number_control_t barrier;
		health_t life_decrease = 0.0F;
	public:
		basic_machine(int random_seed, health_t life_decrement) : units(random_seed), life_decrease(life_decrement) {};
	protected:
		void Breed(order_elemnt_t& element) {
			while (element.Unit().IsPregnant()) {
				auto descendants = breed_manager.Breed(element);
				for (auto descendant : descendants) {
					if (descendant == nullptr) continue;

					birth_queue.push_back(descendant);
				};
			};
		};

		void LifeControl(order_elemnt_t& element) {
			element.Unit().Damage(life_decrease);
		};

		void Sanitation() {
			std::vector<std::shared_ptr<unit_t>> living_units;

			for (auto element : units) {
				if (element.Unit().IsAlive())
					living_units.push_back(element.Unit_ptr());
			};

			units.Clear();

			for (auto living : living_units)
				units.Push(living);
		};

		virtual void CustomCondition() { units.Indexate(); };
		virtual void CustomCondition(order_elemnt_t& elem) { elem.Unit().Condition(); };
	public:
		void Condition() {
			CustomCondition();

			for (auto& element : units) {
				CustomCondition(element);
				ration_source.Feed(element);
				Breed(element);
				LifeControl(element);
				fatal_act(element);
			};

			barrier(*this);
			Sanitation();
		};
	};
};
