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
			reference() noexcept = default;
			explicit reference(::std::size_t Idx) noexcept;
		public:
			bool operator==(const reference&) const noexcept;
			bool operator!=(const reference&) const noexcept;
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
		::std::size_t set(
			const index::reference& Ref,
			::std::size_t ValueNewIdx
		) const noexcept;
	public:
		void swap(
			const index::reference& SubTree1,
			const index::reference& SubTree2
		) noexcept;
		void gc() noexcept;
	};


	template<typename value_t>
	using value_storage_type = ::std::vector<value_t>;


	template<typename value_t>
	class tree_carcase {
	public:
		::uns::trees::auxiliary::flex::index index;
		::uns::trees::auxiliary::flex::value_storage_type<value_t> storage;
	};


	class proxy {
	public:
		virtual bool init(
			::uns::trees::auxiliary::flex::tree_carcase<value_type>*,
			::uns::trees::auxiliary::flex::index::reference
		) noexcept = 0;
		virtual bool init(
			proxy&,
			::uns::trees::auxiliary::flex::tree_carcase<value_type>*,
			::uns::trees::auxiliary::flex::index::reference
		) const noexcept = 0;
	public:
		virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase() const noexcept = 0;
	protected:
		virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase(
			const proxy&
		) const noexcept = 0;
	public:
		virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept = 0;
		virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference&) noexcept = 0;
	protected:
		virtual ::uns::trees::auxiliary::flex::index::reference get_ref(
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
			::uns::trees::auxiliary::flex::tree_carcase<value_type>* m_carcase = nullptr;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			const_subnodes() noexcept = default;
		protected:
			inline const_subnodes(const ::uns::trees::flex::const_subnodes& Obj) noexcept :
				m_carcase(Obj.m_carcase),
				m_ref(Obj.m_ref)
			{};
			inline ::uns::trees::flex::const_subnodes& operator=(const ::uns::trees::flex::const_subnodes& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_carcase != &Obj.m_carcase) return *this;

				m_ref = Obj.m_ref;

				return *this;
			};
			inline const_subnodes(::uns::trees::flex::const_subnodes&& Obj) noexcept :
				m_carcase(Obj.m_carcase),
				m_ref(::std::move(Obj.m_ref))
			{};
			inline ::uns::trees::flex::const_subnodes& operator=(::uns::trees::flex::const_subnodes&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_carcase != &Obj.m_carcase) return *this;

				m_ref = ::std::move(Obj.m_ref);

				return *this;
			};
		public:
			~const_subnodes() noexcept = default;
		protected:
			virtual bool init(
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept override {
				if(m_carcase != nullptr) return false;

				m_carcase = Carcase;
				m_ref = Reference;

				return true;
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept override {
				return Proxy.init(Carcase, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase() const noexcept override {
				return m_carcase;
			};
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_carcase();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept override {
				return m_ref;
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				m_ref = Reference;
			};
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_ref();
			};
			virtual void set_ref(
				proxy& Proxy,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Proxy.set_ref(Reference);
			};
		public:
			inline ::std::size_t size() const noexcept { return m_carcase->index.get_subnodes_total(m_ref); };
			inline ::uns::trees::flex::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
		protected:
			inline ::uns::trees::auxiliary::flex::index::reference parent() const noexcept { return m_carcase->index.get_parent(m_ref); };
		};
	public:
		class subnodes: public ::uns::trees::auxiliary::flex::proxy {
		protected:
			::uns::trees::auxiliary::flex::tree_carcase<value_type>* m_carcase = nullptr;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			subnodes() noexcept = default;
		protected:
			inline subnodes(const ::uns::trees::flex::subnodes& Obj) noexcept :
				m_carcase(Obj.m_carcase),
				m_ref(Obj.m_ref) {};
			inline ::uns::trees::flex::subnodes& operator=(const ::uns::trees::flex::subnodes& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_carcase != &Obj.m_carcase) return *this;

				m_ref = Obj.m_ref;

				return *this;
			};
			inline subnodes(::uns::trees::flex::subnodes&& Obj) noexcept :
				m_carcase(Obj.m_carcase),
				m_ref(::std::move(Obj.m_ref)) {};
			inline ::uns::trees::flex::subnodes& operator=(::uns::trees::flex::subnodes&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(&m_carcase != &Obj.m_carcase) return *this;

				m_ref = ::std::move(Obj.m_ref);

				return *this;
			};
		public:
			~subnodes() noexcept = default;
		protected:
			virtual bool init(
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept override {
				if(m_carcase != nullptr) return false;

				m_carcase = Carcase;
				m_ref = Reference;

				return true;
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept override {
				return Proxy.init(Carcase, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase() const noexcept override {
				return m_carcase;
			};
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_carcase();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept override {
				return m_ref;
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				m_ref = Reference;
			};
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_ref();
			};
			virtual void set_ref(
				proxy& Proxy,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Proxy.set_ref(Reference);
			};
		public:
			inline ::std::size_t size() const noexcept { return m_carcase->index.get_subnodes_total(m_ref); };
			inline ::uns::trees::flex::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
			inline ::uns::trees::flex::iterator operator[](::std::size_t SubnodeIdx) noexcept;
		protected:
			inline ::uns::trees::auxiliary::flex::index::reference parent() const noexcept { return m_carcase->index.get_parent(m_ref); };
		public:
			inline void push_back(const ::uns::trees::flex::const_iterator& SomeTree) noexcept;				//copies SomeTree and makes it the last subnode
			inline void push_back(const value_type& SomeValue) noexcept;								//creates a new subnode, pushes it as the last subnode and puts there a SomeValue
			inline bool insert(																			//removes the last subnode and inserts it in the position PosIdx in the same current subnodes set
				::std::size_t PosIdx
			) noexcept;
			void replace(const ::uns::trees::flex::const_iterator& SomeTree) noexcept;
		};
	public:
		class const_iterator: public ::uns::trees::auxiliary::flex::proxy {
		public:
			::uns::trees::flex::const_subnodes subnodes;
		public:
			const_iterator() noexcept = default;
			const_iterator(const ::uns::trees::flex::iterator& Obj) noexcept;
			inline const_iterator(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				init(
					subnodes,
					get_carcase(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::const_iterator& operator=(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_carcase(subnodes) != Obj.get_carcase()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline const_iterator(::uns::trees::flex::const_iterator&& Obj) noexcept {
				init(
					subnodes,
					get_carcase(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::const_iterator& operator=(::uns::trees::flex::const_iterator&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_carcase(subnodes) != Obj.get_carcase()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~const_iterator() noexcept = default;
		protected:
			virtual bool init(
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept override {
				return init(subnodes, Carcase, Reference);
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept override {
				return Proxy.init(Carcase, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase() const noexcept override {
				return get_carcase(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_carcase();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept override {
				return get_ref(subnodes);
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				set_ref(subnodes, Reference);
			};
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_ref();
			};
			virtual void set_ref(
				proxy& Proxy,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Proxy.set_ref(Reference);
			};
		public:
			::uns::trees::flex::const_iterator cbegin() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_carcase(subnodes),
					traversal::begin(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::const_iterator cend() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_carcase(subnodes),
					traversal::end(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			::uns::trees::flex::const_iterator parent() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_carcase(subnodes),
					get_carcase(subnodes)->index.get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline const value_type& operator*() const noexcept {
				return get_carcase(subnodes)->storage[
					get_carcase(subnodes)->index.get(
						get_ref(subnodes)
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &get_carcase(subnodes)->storage[
					get_carcase(subnodes)->index.get(
						get_ref(subnodes)
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(
					subnodes,
					traversal::next(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);
			};
		};
	public:
		class iterator: public ::uns::trees::auxiliary::flex::proxy {
		public:
			::uns::trees::flex::subnodes subnodes;
		public:
			iterator() noexcept = default;
			inline iterator(const ::uns::trees::flex::iterator& Obj) noexcept {
				init(
					subnodes,
					get_carcase(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::iterator& operator=(const ::uns::trees::flex::iterator& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_carcase(subnodes) != Obj.get_carcase()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline iterator(::uns::trees::flex::iterator&& Obj) noexcept {
				init(
					subnodes,
					get_carcase(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::iterator& operator=(::uns::trees::flex::iterator&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_carcase(subnodes) != Obj.get_carcase()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~iterator() noexcept = default;
		protected:
			virtual bool init(
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept override {
				return init(subnodes, Carcase, Reference);
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept override {
				return Proxy.init(Carcase, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase() const noexcept override {
				return get_carcase(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_carcase();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept override {
				return get_ref(subnodes);
			};
			virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {
				set_ref(subnodes, Reference);
			};
			virtual ::uns::trees::auxiliary::flex::index::reference get_ref(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_ref();
			};
			virtual void set_ref(
				proxy& Proxy,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Proxy.set_ref(Reference);
			};
		public:
			::uns::trees::flex::const_iterator cbegin() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_carcase(subnodes),
					traversal::begin(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::iterator begin() noexcept {
				auto result = ::uns::trees::flex::iterator{};

				init(
					result,
					get_carcase(subnodes),
					traversal::begin(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::const_iterator cend() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_carcase(subnodes),
					traversal::end(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::iterator end() noexcept {
				auto result = ::uns::trees::flex::iterator{};

				init(
					result,
					get_carcase(subnodes),
					traversal::end(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			::uns::trees::flex::const_iterator parent() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_carcase(subnodes),
					get_carcase(subnodes)->index.get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::iterator parent() noexcept {
				auto result = ::uns::trees::flex::iterator{};

				init(
					result,
					get_carcase(subnodes),
					get_carcase(subnodes)->index.get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline const value_type& operator*() const noexcept {
				return get_carcase(subnodes)->storage[
					get_carcase(subnodes)->index.get(
						get_ref(subnodes)
					)
				];
			};
			inline value_type& operator*() noexcept {
				return get_carcase(subnodes)->storage[
					get_carcase(subnodes)->index.get(
						get_ref(subnodes)
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &get_carcase(subnodes)->storage[
					get_carcase(subnodes)->index.get(
						get_ref(subnodes)
					)
				];
			};
			inline value_type* operator->() noexcept {
				return &get_carcase(subnodes)->storage[
					get_carcase(subnodes)->index.get(
						get_ref(subnodes)
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(
					subnodes,
					traversal::next(
						get_carcase(subnodes)->index,
						get_ref(subnodes)
					)
				);
			};
		};
	protected:
		mutable ::uns::trees::auxiliary::flex::tree_carcase<value_type> m_carcase;
	protected:
		virtual bool init(
			::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
			::uns::trees::auxiliary::flex::index::reference Reference
		) noexcept override {
			return false;
		};
		virtual bool init(
			proxy& Proxy,
			::uns::trees::auxiliary::flex::tree_carcase<value_type>* Carcase,
			::uns::trees::auxiliary::flex::index::reference Reference
		) const noexcept override {
			return Proxy.init(Carcase, Reference);
		};
	protected:
		virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase() const noexcept override {
			return &m_carcase;
		};
		virtual ::uns::trees::auxiliary::flex::tree_carcase<value_type>* get_carcase(
			const ::uns::trees::auxiliary::flex::proxy& Proxy
		) const noexcept override {
			return Proxy.get_carcase();
		};
	protected:
	protected:
		virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept override {
			return m_carcase.index.get_root();
		};
		virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference& Reference) noexcept override {};
		virtual ::uns::trees::auxiliary::flex::index::reference get_ref(
			const ::uns::trees::auxiliary::flex::proxy& Proxy
		) const noexcept override {
			return Proxy.get_ref();
		};
		virtual void set_ref(
			proxy& Proxy,
			const ::uns::trees::auxiliary::flex::index::reference& Reference
		) const noexcept override {
			Proxy.set_ref(Reference);
		};
	public:
		::uns::trees::flex::const_iterator cbegin() const noexcept {
			return root();
		};
		::uns::trees::flex::iterator begin() noexcept {
			return root();
		};
		::uns::trees::flex::const_iterator cend() const noexcept {
			auto result = ::uns::trees::flex::const_iterator{};

			init(
				result,
				&m_carcase,
				m_carcase.index.get_null()
			);

			return result;
		};
		::uns::trees::flex::iterator end() noexcept {
			auto result = ::uns::trees::flex::iterator{};

			init(
				result,
				&m_carcase,
				m_carcase.index.get_null()
			);

			return result;
		};
		::std::size_t size() const noexcept { return m_carcase.index.size(); };
	public:
		::uns::trees::flex::const_iterator root() const noexcept {
			auto result = ::uns::trees::flex::const_iterator{};

			init(
				result,
				&m_carcase,
				m_carcase.index.get_root()
			);

			return result;
		};
		::uns::trees::flex::iterator root() noexcept {
			auto result = ::uns::trees::flex::iterator{};

			init(
				result,
				&m_carcase,
				m_carcase.index.get_root()
			);

			return result;
		};
	public:
		inline void set_root(const value_type& Value) noexcept {
			auto root_ref = m_carcase.index.get_root();

			if(root_ref == m_carcase.index.get_null()) {
				if(m_carcase.storage.size() == 0) {
					m_carcase.storage.push_back(Value);
				}
				else {
					m_carcase.storage[0] = Value;
				};

				m_carcase.index.set_root(0);
			}
			else {
				auto root_idx = m_carcase.index.get(root_ref);

				if(m_carcase.storage.size() <= root_idx) {
					m_carcase.storage.resize(root_idx + 1);
				};

				m_carcase.storage[root_idx] = Value;
			};
		};
		inline void swap(
			const ::uns::trees::flex::const_iterator& SubTree1,
			const ::uns::trees::flex::const_iterator& SubTree2
		) noexcept {
			m_carcase.index.swap(
				get_ref(SubTree1.subnodes),
				get_ref(SubTree2.subnodes)
			);
		};
		inline void remove(
			const ::uns::trees::flex::const_iterator& SubTree
		) noexcept {
			m_carcase.index.set_parent(
				get_ref(SubTree.subnodes),
				m_carcase.index.get_null()
			);
		};
	protected:
		inline void check_references(
			::std::vector<::std::pair<::uns::trees::auxiliary::flex::index::reference, ::std::size_t>>& References,
			::uns::trees::auxiliary::flex::index::reference CurrentNode
		) noexcept {
			References.push_back({
				CurrentNode,
				m_carcase.index.get(CurrentNode)
			});

			const auto subnodes_total = m_carcase.index.get_subnodes_total(CurrentNode);
			for(::std::size_t subnode_idx = 0; subnode_idx < subnodes_total; ++subnode_idx) {
				check_references(
					References,
					m_carcase.index.get_subnode(
						CurrentNode, 
						subnode_idx
					)
				);
			};
		};
	public:
		inline void gc() noexcept {
			m_carcase.index.gc();

			auto references = ::std::vector<::std::pair<::uns::trees::auxiliary::flex::index::reference, ::std::size_t>>{};

			if(m_carcase.index.get_root() != m_carcase.index.get_null()) {
				check_references(
					references,
					m_carcase.index.get_root()
				);
			};

			auto new_storage = decltype(m_carcase.storage){};
			for(auto [node_ref, value_idx] : references) {
				new_storage.push_back(
					m_carcase.storage[value_idx]
				);

				m_carcase.index.set(
					node_ref,
					new_storage.size() - 1
				);
			};

			::std::swap(m_carcase.storage, new_storage);
		};
	};
};


::uns::trees::flex::const_iterator uns::trees::flex::const_subnodes::operator[](::std::size_t SubnodeIdx) const noexcept {
	auto result = ::uns::trees::flex::const_iterator{};

	init(
		result,
		m_carcase,
		m_carcase->index.get_subnode(m_ref, SubnodeIdx)
	);

	return result;
};

::uns::trees::flex::const_iterator uns::trees::flex::subnodes::operator[](::std::size_t SubnodeIdx) const noexcept {
	auto result = ::uns::trees::flex::const_iterator{};

	init(
		result,
		m_carcase,
		m_carcase->index.get_subnode(m_ref, SubnodeIdx)
	);

	return result;
};
::uns::trees::flex::iterator uns::trees::flex::subnodes::operator[](::std::size_t SubnodeIdx) noexcept {
	auto result = ::uns::trees::flex::iterator{};

	init(
		result,
		m_carcase,
		m_carcase->index.get_subnode(m_ref, SubnodeIdx)
	);

	return result;
};

void ::uns::trees::flex::subnodes::push_back(const ::uns::trees::flex::const_iterator& SomeTree) noexcept {
	auto original_value_idx = get_carcase(SomeTree.subnodes)->index.get(
		get_ref(SomeTree.subnodes)
	);

	m_carcase->storage.push_back(
		m_carcase->storage[original_value_idx]
	);

	auto copy_ref = m_carcase->index.push(m_carcase->storage.size() - 1);
	m_carcase->index.set_parent(
		copy_ref,
		m_ref
	);

	auto just_added_node = this->operator[](size() - 1);
	for(::std::size_t subnode_idx = 0; subnode_idx < SomeTree.subnodes.size(); ++subnode_idx) {
		just_added_node.subnodes.push_back(
			SomeTree.subnodes[subnode_idx]
		);
	};
};
void ::uns::trees::flex::subnodes::push_back(const value_type& SomeValue) noexcept {
	m_carcase->storage.push_back(SomeValue);

	auto copy_ref = m_carcase->index.push(m_carcase->storage.size() - 1);
	m_carcase->index.set_parent(
		copy_ref,
		m_ref
	);
};
bool ::uns::trees::flex::subnodes::insert(
	::std::size_t PosIdx
) noexcept {
	for(::std::size_t subnode_idx = m_carcase->index.get_subnodes_total(m_ref) - 1; subnode_idx > PosIdx; --subnode_idx) {
		m_carcase->index.swap(
			m_carcase->index.get_subnode(m_ref, subnode_idx),
			m_carcase->index.get_subnode(m_ref, subnode_idx - 1)
		);
	};
};
void ::uns::trees::flex::subnodes::replace(const ::uns::trees::flex::const_iterator& SomeTree) noexcept {
	m_carcase->index.set_parent(
		get_ref(SomeTree.subnodes),
		m_ref
	);
};

#endif
