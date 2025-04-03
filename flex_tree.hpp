#pragma once

#include <iostream>
#include <vector>

#ifndef UNS_LIB_FLEX_TREE
#define UNS_LIB_FLEX_TREE "flex_tree.hpp"


class value_type {};


namespace uns::trees::auxiliary::flex {

	class index {
	public:
		class reference {
		public:
			explicit reference(::std::size_t Idx) noexcept;
		};
	protected:
	public:
		index::reference get_root() const noexcept;						//returns a root node; if the tree is empty returns null
		index::reference set_root(										//adds a root node (if there isn't) and sets it's idx to the passed one
			::std::size_t ValueIdx
		) noexcept;
	public:
		index::reference get_null() const noexcept;						//returns 'parent' of the root node
	public:
		index::reference get_parent(									//returns parent of this node; if this node == root, returns null
			const index::reference& CurrentSubTree
		) const noexcept;
		void set_parent(												//changes parent of this node to the passed one (have no effect if the NewParent == null)
			const index::reference& CurrentSubTree,
			const index::reference& NewParent
		) noexcept;
		index::reference push(											//returns a new node without any parent
			::std::size_t ValueIdx
		) noexcept;
		index::reference copy(											//returns a new node without any parent, but with copies of all sub-, sub-sub-... etc. subnodes of the original node
			const index::reference& CopyingSubTree
		) noexcept;
	public:
		::std::size_t get_subnodes_total(
			const index::reference& CurrentSubTree
		) const noexcept;
		index::reference get_subnode(
			const index::reference& CurrentSubTree,
			::std::size_t SubnodeIdx
		) const noexcept;
	public:
		::std::size_t size() const noexcept;
		::std::size_t get(
			const index::reference& Ref
		) const noexcept;
	public:
		void swap(
			const index::reference& SubTree1,
			const index::reference& SubTree2
		) noexcept;
		void shrink_to_fit() noexcept;
	};


	template<typename value_t>
	using value_storage_type = ::std::vector<value_t>;


	class proxy {
	public:
		virtual ::uns::trees::auxiliary::flex::index& get_index() const noexcept = 0;
	protected:
		virtual ::uns::trees::auxiliary::flex::index& get_index(
			const proxy&
		) const noexcept = 0;
	public:
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept = 0;
	protected:
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
			const proxy&
		) const noexcept = 0;
	public:
		virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept = 0;
		virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference&) noexcept = 0;
	protected:
		virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
			const proxy&
		) const noexcept = 0;
		virtual void set_ref(
			proxy&,
			const ::uns::trees::auxiliary::flex::index::reference&
		) const noexcept = 0;
	};
};

namespace uns::trees {


	class traversal {
	public:
		static ::uns::trees::auxiliary::flex::index::reference next(
			const ::uns::trees::auxiliary::flex::index& Index,
			::uns::trees::auxiliary::flex::index::reference Current
		) noexcept;
		static ::uns::trees::auxiliary::flex::index::reference begin(
			const ::uns::trees::auxiliary::flex::index& Index,
			::uns::trees::auxiliary::flex::index::reference Current
		) noexcept;
		static ::uns::trees::auxiliary::flex::index::reference end(
			const ::uns::trees::auxiliary::flex::index& Index,
			::uns::trees::auxiliary::flex::index::reference Current
		) noexcept;
	};


	class flex: public ::uns::trees::auxiliary::flex::proxy {
	public:
		class const_subnodes;
		class subnodes;
		class const_iterator;
		class iterator;
	public:
		class const_subnodes: public ::uns::trees::auxiliary::flex::proxy {
		protected:
			::uns::trees::auxiliary::flex::index& m_index;
			::std::vector<value_type>& m_storage;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			const_subnodes() = delete;
			inline const_subnodes(const ::uns::trees::auxiliary::flex::proxy& Accessor) noexcept :
				m_index(Accessor.get_index()),
				m_storage(Accessor.get_storage()),
				m_ref(Accessor.get_ref())
			{};
		protected:
			inline const_subnodes(const ::uns::trees::flex::const_subnodes& Obj) noexcept :
				m_index(Obj.m_index),
				m_storage(Obj.m_storage),
				m_ref(Obj.m_ref)
			{};
			inline ::uns::trees::flex::const_subnodes& operator=(const ::uns::trees::flex::const_subnodes& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_index != &Obj.m_index) return *this;
				if(&m_storage != &Obj.m_storage) return *this;

				m_ref = Obj.m_ref;

				return *this;
			};
			inline const_subnodes(::uns::trees::flex::const_subnodes&& Obj) noexcept :
				m_index(Obj.m_index),
				m_storage(Obj.m_storage),
				m_ref(::std::move(Obj.m_ref))
			{};
			inline ::uns::trees::flex::const_subnodes& operator=(::uns::trees::flex::const_subnodes&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_index != &Obj.m_index) return *this;
				if(&m_storage != &Obj.m_storage) return *this;

				m_ref = ::std::move(Obj.m_ref);

				return *this;
			};
		public:
			~const_subnodes() noexcept = default;
		protected:
			virtual ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return m_index;
			};
			virtual ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return m_storage;
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_storage();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept override {
				return m_ref;
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				m_ref = Reference;
			};
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				proxy& Accessor,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Accessor.set_ref(Reference);
			};
		public:
			inline ::std::size_t size() const noexcept { return m_index.get_subnodes_total(m_ref); };
			inline ::uns::trees::flex::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
		protected:
			inline ::uns::trees::auxiliary::flex::index::reference parent() const noexcept { return m_index.get_parent(m_ref); };
		};
	public:
		class subnodes: public ::uns::trees::auxiliary::flex::proxy {
		protected:
			::uns::trees::auxiliary::flex::index& m_index;
			::std::vector<value_type>& m_storage;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			subnodes() = delete;
			inline subnodes(const ::uns::trees::auxiliary::flex::proxy& Accessor) noexcept :
				m_index(Accessor.get_index()),
				m_storage(Accessor.get_storage()),
				m_ref(Accessor.get_ref())
			{};
		protected:
			inline subnodes(const ::uns::trees::flex::subnodes& Obj) noexcept :
				m_index(Obj.m_index),
				m_storage(Obj.m_storage),
				m_ref(Obj.m_ref) 
			{};
			inline ::uns::trees::flex::subnodes& operator=(const ::uns::trees::flex::subnodes& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_index != &Obj.m_index) return *this;
				if(&m_storage != &Obj.m_storage) return *this;

				m_ref = Obj.m_ref;

				return *this;
			};
			inline subnodes(::uns::trees::flex::subnodes&& Obj) noexcept :
				m_index(Obj.m_index),
				m_storage(Obj.m_storage),
				m_ref(::std::move(Obj.m_ref)) {};
			inline ::uns::trees::flex::subnodes& operator=(::uns::trees::flex::subnodes&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_index != &Obj.m_index) return *this;
				if(&m_storage != &Obj.m_storage) return *this;

				m_ref = ::std::move(Obj.m_ref);

				return *this;
			};
		public:
			~subnodes() noexcept = default;
		protected:
			virtual ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return m_index;
			};
			virtual ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return m_storage;
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_storage();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept override {
				return m_ref;
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				m_ref = Reference;
			};
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				proxy& Accessor,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Accessor.set_ref(Reference);
			};
		public:
			inline ::std::size_t size() const noexcept { return m_index.get_subnodes_total(m_ref); };
			inline ::uns::trees::flex::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
			inline ::uns::trees::flex::iterator operator[](::std::size_t SubnodeIdx) noexcept;
		protected:
			inline ::uns::trees::auxiliary::flex::index::reference parent() const noexcept { return m_index.get_parent(m_ref); };
		public:
			inline void push_back(::uns::trees::flex::const_iterator SomeTree) noexcept;				//copies SomeTree and makes it the last subnode
			inline void push_back(const value_type& SomeValue) noexcept;								//creates a new subnode, pushes it as the last subnode and puts there a SomeValue
			inline bool insert(																			//removes the last subnode and inserts it in the position before InsertBeforeThis in the same current subnodes set
				const ::uns::trees::flex::const_iterator& InsertBeforeThis
			) noexcept;
		};
	public:
		class const_iterator: public ::uns::trees::auxiliary::flex::proxy {
		public:
			::uns::trees::flex::const_subnodes subnodes;
		public:
			const_iterator() = delete;
			inline const_iterator(const ::uns::trees::auxiliary::flex::proxy& Obj) noexcept :
				subnodes(Obj)
			{};
			inline const_iterator(const ::uns::trees::flex::const_iterator& Obj) noexcept :
				subnodes(Obj)
			{};
			inline ::uns::trees::flex::const_iterator& operator=(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&get_index(subnodes) != &Obj.get_index()) return *this;
				if(&get_storage(subnodes) != &Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline const_iterator(::uns::trees::flex::const_iterator&& Obj) noexcept :
				subnodes(Obj)
			{};
			inline ::uns::trees::flex::const_iterator& operator=(::uns::trees::flex::const_iterator&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&get_index(subnodes) != &Obj.get_index()) return *this;
				if(&get_storage(subnodes) != &Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~const_iterator() noexcept = default;
		protected:
			virtual ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return get_index(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return get_storage(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_storage();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept override {
				return get_ref(subnodes);
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				set_ref(subnodes, Reference);
			};
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				proxy& Accessor,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Accessor.set_ref(Reference);
			};
		public:
			::uns::trees::flex::const_iterator cbegin() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{
					subnodes
				};

				result.set_ref(
					traversal::begin(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::const_iterator cend() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{
					subnodes
				};

				result.set_ref(
					traversal::end(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			::uns::trees::flex::const_iterator parent() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{
					subnodes
				};

				result.set_ref(
					get_index(subnodes).get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline const value_type& operator*() const noexcept {
				return get_storage(subnodes)[
					get_index(subnodes).get(
						get_ref(subnodes)
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &get_storage(subnodes)[
					get_index(subnodes).get(
						get_ref(subnodes)
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(
					subnodes,
					traversal::next(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);
			};
		};
	public:
		class iterator: public ::uns::trees::auxiliary::flex::proxy {
			friend ::uns::trees::flex;
		public:
			::uns::trees::flex::subnodes subnodes;
		public:
			iterator() = delete;
		protected:
			inline iterator(const ::uns::trees::auxiliary::flex::proxy& Obj) noexcept :
				subnodes(Obj)
			{};
		public:
			inline iterator(const ::uns::trees::flex::iterator& Obj) noexcept :
				subnodes(Obj)
			{};
			inline ::uns::trees::flex::iterator& operator=(const ::uns::trees::flex::iterator& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&get_index(subnodes) != &Obj.get_index()) return *this;
				if(&get_storage(subnodes) != &Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline iterator(::uns::trees::flex::iterator&& Obj) noexcept :
				subnodes(Obj)
			{};
			inline ::uns::trees::flex::iterator& operator=(::uns::trees::flex::iterator&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&get_index(subnodes) != &Obj.get_index()) return *this;
				if(&get_storage(subnodes) != &Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~iterator() noexcept = default;
		protected:
			virtual ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return get_index(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return get_storage(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_storage();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept override {
				return get_ref(subnodes);
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				set_ref(subnodes, Reference);
			};
			virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				proxy& Accessor,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Accessor.set_ref(Reference);
			};
		public:
			inline ::uns::trees::flex::const_iterator cbegin() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{
					subnodes
				};

				set_ref(
					result,
					traversal::begin(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			inline ::uns::trees::flex::const_iterator cend() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{
					subnodes
				};

				set_ref(
					result,
					traversal::end(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			inline ::uns::trees::flex::iterator begin() noexcept {
				auto result = ::uns::trees::flex::iterator{
					subnodes
				};

				set_ref(
					result,
					traversal::begin(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			inline ::uns::trees::flex::iterator end() noexcept {
				auto result = ::uns::trees::flex::iterator{
					subnodes
				};

				set_ref(
					result,
					traversal::end(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline ::uns::trees::flex::const_iterator parent() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{
					subnodes
				};

				set_ref(
					result,
					get_index(subnodes).get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
			inline ::uns::trees::flex::iterator parent() noexcept {
				auto result = ::uns::trees::flex::iterator{
					subnodes
				};

				set_ref(
					result,
					get_index(subnodes).get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline const value_type& operator*() const noexcept {
				return get_storage(subnodes)[
					get_index(subnodes).get(
						get_ref(subnodes)
					)
				];
			};
			inline value_type& operator*() noexcept {
				return get_storage(subnodes)[
					get_index(subnodes).get(
						get_ref(subnodes)
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &get_storage(subnodes)[
					get_index(subnodes).get(
						get_ref(subnodes)
					)
				];
			};
			inline value_type* operator->() noexcept {
				return &get_storage(subnodes)[
					get_index(subnodes).get(
						get_ref(subnodes)
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(
					subnodes,
					traversal::next(
						get_index(subnodes),
						get_ref(subnodes)
					)
				);
			};
		};
	protected:
		mutable ::uns::trees::auxiliary::flex::index m_index;
		mutable ::std::vector<value_type> m_values;
	protected:
		virtual ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
			return m_index;
		};
		virtual ::uns::trees::auxiliary::flex::index& get_index(
			const ::uns::trees::auxiliary::flex::proxy& Accessor
		) const noexcept override {
			return Accessor.get_index();
		};
	protected:
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
			return m_values;
		};
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
			const ::uns::trees::auxiliary::flex::proxy& Accessor
		) const noexcept override {
			return Accessor.get_storage();
		};
	protected:
		virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept override {
			return m_index.get_root();
		};
		virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {};
		virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
			const ::uns::trees::auxiliary::flex::proxy& Accessor
		) const noexcept override {
			return Accessor.get_ref();
		};
		virtual void set_ref(
			proxy& Accessor,
			const ::uns::trees::auxiliary::flex::index::reference& Reference
		) const noexcept override {
			Accessor.set_ref(Reference);
		};
	public:
		::uns::trees::flex::const_iterator cbegin() const noexcept {
			return root();
		};
		::uns::trees::flex::const_iterator cend() const noexcept {
			auto result = ::uns::trees::flex::const_iterator{
				*this
			};

			set_ref(
				result.subnodes,
				m_index.get_root()
			);

			return result;
		};
		::uns::trees::flex::iterator begin() noexcept {
			auto result = ::uns::trees::flex::iterator{
				*this
			};

			set_ref(
				result.subnodes,
				m_index.get_root()
			);

			return result;
		};
		::uns::variadic_tree::iterator end() noexcept {
			return ::uns::variadic_tree::iterator{
				m_index,
				m_values,
				m_index.none()
			};
		};
		::std::size_t size() const noexcept { return m_index.size(); };
	public:
		::uns::variadic_tree::const_iterator root() const noexcept {							//returns an iterator pointing to the root, or end() if the tree is empty
			return ::uns::variadic_tree::const_iterator{
				m_index,
				m_values,
				m_index.root()
			};
		};
		::uns::variadic_tree::iterator root() noexcept {										//returns an iterator pointing to the root, or end() if the tree is empty
			return ::uns::variadic_tree::iterator{
				m_index,
				m_values,
				m_index.root()
			};
		};
	public:
		inline void set_root(const value_type& Value) noexcept {
			auto root_ref = m_index.root();
			auto root_idx = m_index.value_idx(root_ref);

			if(m_values.size() <= root_idx) {
				m_values.resize(root_idx + 1);
			};

			m_values[root_idx] = Value;
		};
		inline void swap(																		//swaps two trees
			const ::uns::variadic_tree::iterator& SubTree1,
			const ::uns::variadic_tree::iterator& SubTree2
		) noexcept {
			m_index.swap(SubTree1.subnodes.m_ref, SubTree2.subnodes.m_ref);
		};
	protected:
		inline void check_references(
			::std::vector<bool>& Referenced,
			::uns::variadic_tree::index::reference Ref
		) noexcept {
			Referenced[
				m_index.value_idx(Ref)
			] = true;

			const auto subnodes_total = m_index.subnodes_total(Ref);
			for(::std::size_t subnode_idx = 0; subnode_idx < subnodes_total; ++subnode_idx) {
				check_references(
					Referenced,
					m_index.subnode(Ref, subnode_idx)
				);
			};
		};
	public:
		inline void shrink_to_fit() noexcept {
			m_index.shrink_to_fit();

			auto referenced = ::std::vector<bool>{};
			referenced.resize(m_values.size(), false);

			check_references(
				referenced,
				m_index.root()
			);

			auto values_iter = m_values.cbegin();
			auto referenced_iter = referenced.cbegin();
			auto values_end = m_values.cend();
			auto referenced_end = referenced.cend();

			while(
				values_iter != values_end
				&& referenced_iter != referenced_end
				) {
				if(*referenced_iter) {
					++values_iter;
					++referenced_iter;
				}
				else {
					values_iter = m_values.erase(values_iter);
					++referenced_iter;
				};
			};

			m_values.shrink_to_fit();
		};
















	
	};

};

#endif
