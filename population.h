#pragma once

#define UNS_HEADER_POPULATION "population.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>

namespace uns::population {

	//default unit of the population
	template<typename health_t, typename points_t>
	class unit {
	public:
		using points_type = points_t;
		using health_type = health_t;

		virtual void add_points(const points_type&) { /*pregnancy += points_gain;*/ };

		virtual void damage(const health_type&) { /*health -= health_decrease;*/ };

		virtual bool is_alive() const { return false; /*return uns::math::more(health, 0.0F);*/ };

		virtual bool is_pregnant() const { return false; /*return uns::math::more(pregnancy, 0.0F);*/ };

		virtual void condition() {};
	};


	//the population order mechanism declaration
	//an auxiliary class of element of the population order
	template<typename unit_t, typename order_t>
	class order_element {
	public:
		using unit_type = unit_t;
		using order_type = order_t;
		using index_type = typename order_t::size_t;
	protected:
		std::shared_ptr<unit_type> m_unit_ptr;
		order_type* m_order_ptr;
		index_type m_index = 0;
	public:
		order_element() = delete;
		order_element(const std::shared_ptr<unit_type>& unit, order_type& order, index_type idx) :
			m_unit_ptr(unit),
			m_order_ptr(&order),
			m_index(idx)
		{};
		order_element(const uns::population::order_element& obj) :
			m_unit_ptr(obj.m_unit_ptr),
			m_order_ptr(obj.m_order_ptr),
			m_index(obj.m_index)
		{};
		uns::population::order_element& operator=(const uns::population::order_element& obj) {
			if(this == &obj) return *this;

			m_unit_ptr = obj.m_unit_ptr;
			m_order_ptr = obj.m_order_ptr;
			m_index = obj.m_index;

			return *this;
		};
		order_element(uns::population::order_element&& obj) :
			m_unit_ptr(std::move(obj.m_unit_ptr)),
			m_order_ptr(obj.m_order_ptr),
			m_index(std::move(obj.m_index)) {};
		uns::population::order_element& operator=(uns::population::order_element&& obj) {
			if(this == &obj) return *this;

			m_unit_ptr = std::move(obj.m_unit_ptr);
			m_order_ptr = obj.m_order_ptr;
			m_index = std::move(obj.m_index);

			return *this;
		};
		~order_element() {};

		operator unit_t& () { return *m_unit_ptr; };

		bool operator==(const std::shared_ptr<unit_type>& unit) const { return (unit == m_unit_ptr); };

		const unit_type& unit() const { return *m_unit_ptr; };
		unit_type& unit() { return *m_unit_ptr; };

		const order_type& order() const { return *m_order_ptr; };
		order_type& order() { return *m_order_ptr; };

		const index_type& index() const {
			return m_index;
		};
		index_type& index() {
			return m_index;
		};

		std::shared_ptr<unit_type> unit_ptr() { return m_unit_ptr; };
	};


	template<typename unit_t, typename order_t>
	class linear_order {
	public:
		using unit_type = unit_t;
		using order_type = order_t;
		using element_type = uns::population::order_element<unit_type, order_type>;
		using distribution_type = std::uniform_real_distribution<float>;
	protected:
		using linear_container = std::vector<element_type>;
	public:
		using iterator_type = typename linear_container::iterator;
		using const_iterator_type = typename linear_container::const_iterator;
		using size_type = typename linear_container::size_type;
	protected:
		linear_container m_container;
		mutable std::mt19937 m_engine;
		mutable distribution_type m_distribution;
	public:
		linear_order(int seed = 0) :
			m_engine(seed),
			m_distribution(0.0F, 1.0F)
		{};
		linear_order(const uns::population::linear_order& obj) :
			m_container(obj.m_container),
			m_engine(obj.m_engine),
			m_distribution(obj.m_distribution)
		{};
		uns::population::linear_order& operator=(const uns::population::linear_order& obj) {
			if(this == &obj) return *this;

			m_container = obj.m_container;
			m_engine = obj.m_engine;
			m_distribution = obj.m_distribution;

			return *this;
		};
		linear_order(uns::population::linear_order&& obj) :
			m_container(std::move(obj.m_container)),
			m_engine(std::move(obj.m_engine)),
			m_distribution(std::move(obj.m_distribution))
		{};
		uns::population::linear_order& operator=(uns::population::linear_order&& obj) {
			if(this == &obj) return *this;

			m_container = std::move(obj.m_container);
			m_engine = std::move(obj.m_engine);
			m_distribution = std::move(obj.m_distribution);

			return *this;
		};
		~linear_order() noexcept {};

		const_iterator_type cbegin() const { return m_container.cbegin(); };

		const_iterator_type cend() const { return m_container.cend(); };

		const iterator_type begin() const { return m_container.begin(); };
		iterator_type begin() { return m_container.begin(); };

		const iterator_type end() const { return m_container.end(); };
		iterator_type end() { return m_container.end(); };

		size_type size() const { return m_container.size(); };

		element_type operator[](size_type index) const { return m_container[index]; };
		element_type& operator[](size_type index) { return m_container[index]; };

		void clear() {
			m_container.clear();
		};

		void push(const std::shared_ptr<unit_type>& unit_to_add) {
			m_container.push_back(element_type{ unit_to_add, *static_cast<order_type*>(this), m_container.size() });
		};

		void pop(const iterator_type& iter) {
			m_container.erase(iter);
		};
		void pop(size_type index) {
			m_container.erase(m_container.begin() + index);
		};

		void indexate() {
			auto index = typename element_type::index_type{ 0 };
			for(auto& unit : m_container) {
				unit.index() = index++;
			};
		};

	};


	template<typename unit_t>
	class unordered_order : public uns::population::linear_order<unit_t, uns::population::unordered_order<unit_t>> {
	protected:
		using base = uns::population::linear_order<unit_t, uns::population::unordered_order<unit_t>>;
	public:
		using element_type = typename base::element_type;
		using iterator_type = typename base::iterator_type;
		using const_iterator_type = typename base::const_iterator_type;
		using size_type = typename base::size_type;
		using unit_type = typename base::unit_type;
	public:
		unordered_order(int seed = 0) : base(seed) {};
		unordered_order(const unordered_order& obj) :
			base::m_container(obj.m_container),
			base::m_engine(obj.m_engine),
			base::m_distribution(obj.m_distribution)
		{};
		unordered_order& operator=(const unordered_order& obj) {
			if(this == &obj) return *this;

			base::m_container = obj.m_container;
			base::m_engine = obj.m_engine;
			base::m_distribution = obj.m_distribution;

			return *this;
		};
		unordered_order(unordered_order&& obj) :
			base::m_container(std::move(obj.m_container)),
			base::m_engine(std::move(obj.m_engine)),
			base::m_distribution(std::move(obj.m_distribution)) {};
		unordered_order& operator=(unordered_order&& obj) {
			if(this == &obj) return *this;

			base::m_container = std::move(obj.m_container);
			base::m_engine = std::move(obj.m_engine);
			base::m_distribution = std::move(obj.m_distribution);

			return *this;
		};
		~unordered_order() noexcept {};

		const_iterator_type pick() const {
			size_type index = 0;
			if(!(size() > 0)) return cbegin();

			index = static_cast<size_type>(static_cast<typename base::distribution_type::result_type>(size()) * base::m_distribution(base::m_engine));
			if(index >= size() || index == 0) {
				return cbegin();
			}
			else {
				index = size() - 1;
				return (cbegin() + index);
			};
		};
		iterator_type pick() {
			size_type index = 0;
			if(!(size() > 0)) return begin();

			index = static_cast<size_type>(size() * base::m_distribution(base::m_engine));
			if(index >= size() || index == 0) {
				return begin();
			}
			else {
				index = size() - 1;
				return (begin() + index);
			};
		};
	};


	template<typename unit_t, bool(*m_predicate)(const unit_t&, const unit_t&)>
	class ordered_order : public uns::population::linear_order<unit_t, uns::population::ordered_order<unit_t, m_predicate>> {
	protected:
		using base = uns::population::linear_order<unit_t, uns::population::ordered_order<unit_t, m_predicate>>;
	public:
		using element_type = typename base::element_type;
		using iterator_type = typename base::iterator_type;
		using const_iterator_type = typename base::const_iterator_type;
		using size_type = typename base::size_type;
		using unit_type = typename base::unit_type;
	public:
		ordered_order(int seed = 0) : base(seed) {};
		ordered_order(const uns::population::ordered_order& obj) :
			base::m_container(obj.m_container),
			base::m_engine(obj.m_engine),
			base::m_distribution(obj.m_distribution)
		{};
		uns::population::ordered_order& operator=(const uns::population::ordered_order& obj) {
			if(this == &obj) return *this;

			base::m_container = obj.m_container;
			base::m_engine = obj.m_engine;
			base::m_distribution = obj.m_distribution;

			return *this;
		};
		ordered_order(uns::population::ordered_order&& obj) :
			base::m_container(std::move(obj.m_container)),
			base::m_engine(std::move(obj.m_engine)),
			base::m_distribution(std::move(obj.m_distribution))
		{};
		uns::population::ordered_order& operator=(uns::population::ordered_order&& obj) {
			if(this == &obj) return *this;

			base::m_container = std::move(obj.m_container);
			base::m_engine = std::move(obj.m_engine);
			base::m_distribution = std::move(obj.m_distribution);
			return *this;
		};
		~ordered_order() noexcept {};

		const_iterator_type pick_less(const element_type& elem) const {
			size_type index = 0;
			if(!(size() > elem.index() + 1)) return cbegin();

			index = static_cast<size_type>(
				static_cast<typename base::distribution_type::result_type>(size() - elem.index() - 1) * base::m_distribution(base::m_engine)
				) + elem.index() + 1;

			if(index >= size() || index == 0) {
				return cbegin();
			}
			else {
				index = size() - 1;
				return cbegin() + index;
			};
		};
		iterator_type pick_less(const element_type& elem) {
			size_type index = 0;
			if(!(size() > elem.index() + 1)) return begin();

			index = static_cast<size_type>(
				static_cast<typename base::distribution_type::result_type>(size() - elem.index() - 1) * base::m_distribution(base::m_engine)
			) + elem.index() + 1;

			if(index >= size() || index == 0) {
				return begin();
			}
			else {
				index = size() - 1;
				return begin() + index;
			};
		};

		const_iterator_type pick_more(const element_type& elem) const {
			size_type index = 0;
			if(!(elem.index() > 1)) return cbegin();

			index = static_cast<size_type>(static_cast<typename base::distribution_type::result_type>(elem.index()) * base::m_distribution(base::m_engine));

			if(index >= size()) {
				return cbegin() + (size() - 1);
			}
			else if(index == 0) {
				return cbegin() + 1;
			}
			else {
				index = size() - 1;
				return cbegin() + index;
			};
		};
		iterator_type pick_more(const element_type& elem) {
			size_type index = 0;
			if(!(elem.index() > 1)) return begin();

			index = static_cast<size_type>(static_cast<typename base::distribution_type::result_type>(elem.index()) * base::m_distribution(base::m_engine));

			if(index >= size()) {
				return begin() + (size() - 1);
			}
			else if(index == 0) {
				return begin() + 1;
			}
			else {
				index = size() - 1;
				return begin() + index;
			};
		};

		virtual void sort() {
			std::sort(begin(), end(), m_predicate);
			base::indexate();
		};
	};


	template<typename order_element_t>
	class ration_source {
	public:
		virtual void feed(order_element_t& order_element) {};
	};


	template<typename order_element_t>
	class breed_manager {
	public:
		using unit_type = typename order_element_t::unit_type;
		virtual std::vector<std::shared_ptr<unit_type>> breed(order_element_t& order_element) {
			auto res = std::vector<std::shared_ptr<unit_type>>{};
			order_element.unit().add_points(typename unit_type::points_type(-1));
			res.push_back(std::shared_ptr<unit_type>{ new unit_type{} });
			return res;
		};
	};


	template<typename order_element_t>
	class fatal_act_operator {
	public:
		virtual void operator()(order_element_t& order_element) {};
	};


	class population_size_controller {
	public:
		template<typename population_t>
		void operator()(population_t& pop_obj) {};
	};


	template<typename order_t, typename ration_source_t, typename breed_manager_t, typename fatal_act_operator_t, typename size_control_t>
	class machine {
	public:
		using order_type = typename order_t;
		using unit_type = typename order_type::unit_type;
		using index_type = typename order_type::size_type;
		using order_elemnt_type = typename order_type::element_type;
		using points_type = typename unit_type::points_type;
		using health_type = typename unit_type::health_type;
		using ration_source_type = ration_source_t;
		using breed_manager_type = breed_manager_t;
		using fatal_act_operator_type = fatal_act_operator_t;
		using size_control_type = size_control_t;
	public:
		order_type units;
		std::deque<std::shared_ptr<unit_type>> birth_queue;
		ration_source_type ration_source;
		breed_manager_type breed_manager;
		fatal_act_operator_type fatal_act;
		size_control_type barrier;
		health_type life_decrease = 0.0F;
	public:
		machine(int random_seed, health_type life_decrement) : units(random_seed), life_decrease(life_decrement) {};
	protected:
		void breed(order_elemnt_type& element) {
			while(element.unit().is_pregnant()) {
				for(auto descendant : breed_manager.breed(element)) {
					if(descendant == nullptr) continue;

					birth_queue.push_back(descendant);
				};
			};
		};

		void life_control(order_elemnt_type& element) {
			element.unit().damage(life_decrease);
		};

		void sanitation() {
			std::vector<std::shared_ptr<unit_type>> living_units;

			for(auto element : units) {
				if(element.unit().is_alive()) {
					living_units.push_back(element.unit_ptr());
				};
			};

			units.clear();

			for(auto living : living_units) {
				units.push(living);
			};
		};

		//customizable events
		virtual void OnConditioningBegin() { units.indexate(); };
		virtual void OnConditioningEnd() {};
		virtual void OnConditioningUnitBegin(order_elemnt_type& elem) { elem.unit().condition(); };
		virtual void OnConditioningUnitEnd(order_elemnt_type& elem) {};
	public:
		void condition() { 
			OnConditioningBegin();

			for(auto& element : units) {
				OnConditioningUnitBegin(element);

				ration_source.feed(element);
				breed(element);
				life_control(element);
				fatal_act(element);

				OnConditioningUnitEnd(element);
			};

			barrier(*this);
			sanitation();

			OnConditioningEnd();
		};
	};
};