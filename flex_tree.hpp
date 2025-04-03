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
		void gc() noexcept;
	};


	template<typename value_t>
	using value_storage_type = ::std::vector<value_t>;


	class proxy {
	public:
		virtual bool init(
			::uns::trees::auxiliary::flex::index*,
			::uns::trees::auxiliary::flex::value_storage_type<value_type>*,
			::uns::trees::auxiliary::flex::index::reference
		) noexcept = 0;
		virtual bool init(
			proxy&,
			::uns::trees::auxiliary::flex::index*,
			::uns::trees::auxiliary::flex::value_storage_type<value_type>*,
			::uns::trees::auxiliary::flex::index::reference
		) const noexcept = 0;
	public:
		virtual ::uns::trees::auxiliary::flex::index* get_index() const noexcept = 0;
	protected:
		virtual ::uns::trees::auxiliary::flex::index* get_index(
			const proxy&
		) const noexcept = 0;
	public:
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage() const noexcept = 0;
	protected:
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage(
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
			::uns::trees::auxiliary::flex::index* m_index = nullptr;
			::std::vector<value_type>* m_storage = nullptr;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			const_subnodes() noexcept = default;
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
			virtual bool init(
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept {
				if(m_index != nullptr && m_storage != nullptr) return false;

				m_index = Index;
				m_storage = Storage;
				m_ref = Reference;

				return true;
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept {
				return Proxy.init(Index, Storage, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index* get_index() const noexcept override {
				return m_index;
			};
			virtual ::uns::trees::auxiliary::flex::index* get_index(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage() const noexcept override {
				return m_storage;
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_storage();
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
			inline ::std::size_t size() const noexcept { return m_index->get_subnodes_total(m_ref); };
			inline ::uns::trees::flex::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
		protected:
			inline ::uns::trees::auxiliary::flex::index::reference parent() const noexcept { return m_index->get_parent(m_ref); };
		};
	public:
		class subnodes: public ::uns::trees::auxiliary::flex::proxy {
		protected:
			::uns::trees::auxiliary::flex::index* m_index = nullptr;
			::std::vector<value_type>* m_storage = nullptr;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			subnodes() noexcept = default;
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
			virtual bool init(
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept {
				if(m_index != nullptr && m_storage != nullptr) return false;

				m_index = Index;
				m_storage = Storage;
				m_ref = Reference;

				return true;
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept {
				return Proxy.init(Index, Storage, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index* get_index() const noexcept override {
				return m_index;
			};
			virtual ::uns::trees::auxiliary::flex::index* get_index(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage() const noexcept override {
				return m_storage;
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_storage();
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
			inline ::std::size_t size() const noexcept { return m_index->get_subnodes_total(m_ref); };
			inline ::uns::trees::flex::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
			inline ::uns::trees::flex::iterator operator[](::std::size_t SubnodeIdx) noexcept;
		protected:
			inline ::uns::trees::auxiliary::flex::index::reference parent() const noexcept { return m_index->get_parent(m_ref); };
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
			const_iterator() noexcept = default;
			const_iterator(const ::uns::trees::flex::iterator& Obj) noexcept;
			inline const_iterator(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				init(
					subnodes,
					get_index(Obj.subnodes),
					get_storage(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::const_iterator& operator=(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_index(subnodes) != Obj.get_index()) return *this;
				if(get_storage(subnodes) != Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline const_iterator(::uns::trees::flex::const_iterator&& Obj) noexcept {
				init(
					subnodes,
					get_index(Obj.subnodes),
					get_storage(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::const_iterator& operator=(::uns::trees::flex::const_iterator&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_index(subnodes) != Obj.get_index()) return *this;
				if(get_storage(subnodes) != Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~const_iterator() noexcept = default;
		protected:
			virtual bool init(
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept {
				return init(subnodes,Index, Storage, Reference);
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept {
				return Proxy.init(Index, Storage, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index* get_index() const noexcept override {
				return get_index(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::index* get_index(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage() const noexcept override {
				return get_storage(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_storage();
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
					get_index(subnodes),
					get_storage(subnodes),
					traversal::begin(
						*get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::const_iterator cend() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_index(subnodes),
					get_storage(subnodes),
					traversal::end(
						*get_index(subnodes),
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
					get_index(subnodes),
					get_storage(subnodes),
					get_index(subnodes)->get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline const value_type& operator*() const noexcept {
				return (*get_storage(subnodes))[
					get_index(subnodes)->get(
						get_ref(subnodes)
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &(*get_storage(subnodes))[
					get_index(subnodes)->get(
						get_ref(subnodes)
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(
					subnodes,
					traversal::next(
						*get_index(subnodes),
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
			iterator() noexcept = default;
			inline iterator(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				init(
					subnodes,
					get_index(Obj.subnodes),
					get_storage(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::iterator& operator=(const ::uns::trees::flex::iterator& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_index(subnodes) != Obj.get_index()) return *this;
				if(get_storage(subnodes) != Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline iterator(::uns::trees::flex::iterator&& Obj) noexcept {
				init(
					subnodes,
					get_index(Obj.subnodes),
					get_storage(Obj.subnodes),
					get_ref(Obj.subnodes)
				);
			};
			inline ::uns::trees::flex::iterator& operator=(::uns::trees::flex::iterator&& Obj) noexcept {
				if(this == &Obj) return *this;
				if(get_index(subnodes) != Obj.get_index()) return *this;
				if(get_storage(subnodes) != Obj.get_storage()) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~iterator() noexcept = default;
		protected:
			virtual bool init(
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept {
				return init(subnodes, Index, Storage, Reference);
			};
			virtual bool init(
				proxy& Proxy,
				::uns::trees::auxiliary::flex::index* Index,
				::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
				::uns::trees::auxiliary::flex::index::reference Reference
			) const noexcept {
				return Proxy.init(Index, Storage, Reference);
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::index* get_index() const noexcept override {
				return get_index(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::index* get_index(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_index();
			};
		protected:
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage() const noexcept override {
				return get_storage(subnodes);
			};
			virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage(
				const ::uns::trees::auxiliary::flex::proxy& Proxy
			) const noexcept override {
				return Proxy.get_storage();
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
					get_index(subnodes),
					get_storage(subnodes),
					traversal::begin(
						*get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::iterator begin() noexcept {
				auto result = ::uns::trees::flex::iterator{};

				init(
					result,
					get_index(subnodes),
					get_storage(subnodes),
					traversal::begin(
						*get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::const_iterator cend() const noexcept {
				auto result = ::uns::trees::flex::const_iterator{};

				init(
					result,
					get_index(subnodes),
					get_storage(subnodes),
					traversal::end(
						*get_index(subnodes),
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::iterator end() const noexcept {
				auto result = ::uns::trees::flex::iterator{};

				init(
					result,
					get_index(subnodes),
					get_storage(subnodes),
					traversal::end(
						*get_index(subnodes),
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
					get_index(subnodes),
					get_storage(subnodes),
					get_index(subnodes)->get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
			::uns::trees::flex::iterator parent() noexcept {
				auto result = ::uns::trees::flex::iterator{};

				init(
					result,
					get_index(subnodes),
					get_storage(subnodes),
					get_index(subnodes)->get_parent(
						get_ref(subnodes)
					)
				);

				return result;
			};
		public:
			inline const value_type& operator*() const noexcept {
				return (*get_storage(subnodes))[
					get_index(subnodes)->get(
						get_ref(subnodes)
					)
				];
			};
			inline value_type& operator*() noexcept {
				return (*get_storage(subnodes))[
					get_index(subnodes)->get(
						get_ref(subnodes)
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &(*get_storage(subnodes))[
					get_index(subnodes)->get(
						get_ref(subnodes)
					)
				];
			};
			inline value_type* operator->() noexcept {
				return &(*get_storage(subnodes))[
					get_index(subnodes)->get(
						get_ref(subnodes)
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(
					subnodes,
					traversal::next(
						*get_index(subnodes),
						get_ref(subnodes)
					)
				);
			};
		};
	protected:
		mutable ::uns::trees::auxiliary::flex::index m_index;
		mutable ::std::vector<value_type> m_storage;
	protected:
		virtual bool init(
			::uns::trees::auxiliary::flex::index* Index,
			::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
			::uns::trees::auxiliary::flex::index::reference Reference
		) noexcept {
			return false;
		};
		virtual bool init(
			proxy& Proxy,
			::uns::trees::auxiliary::flex::index* Index,
			::uns::trees::auxiliary::flex::value_storage_type<value_type>* Storage,
			::uns::trees::auxiliary::flex::index::reference Reference
		) const noexcept {
			return Proxy.init(Index, Storage, Reference);
		};
	protected:
		virtual ::uns::trees::auxiliary::flex::index* get_index() const noexcept override {
			return &m_index;
		};
		virtual ::uns::trees::auxiliary::flex::index* get_index(
			const ::uns::trees::auxiliary::flex::proxy& Proxy
		) const noexcept override {
			return Proxy.get_index();
		};
	protected:
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage() const noexcept override {
			return &m_storage;
		};
		virtual ::uns::trees::auxiliary::flex::value_storage_type<value_type>* get_storage(
			const ::uns::trees::auxiliary::flex::proxy& Proxy
		) const noexcept override {
			return Proxy.get_storage();
		};
	protected:
		virtual ::uns::trees::auxiliary::flex::index::reference get_ref() const noexcept override {
			return m_index.get_root();
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
				&m_index,
				&m_storage,
				m_index.get_null()
			);

			return result;
		};
		::uns::trees::flex::iterator end() noexcept {
			auto result = ::uns::trees::flex::iterator{};

			init(
				result,
				&m_index,
				&m_storage,
				m_index.get_null()
			);

			return result;
		};
		::std::size_t size() const noexcept { return m_index.size(); };
	public:
		::uns::trees::flex::const_iterator root() const noexcept {
			auto result = ::uns::trees::flex::const_iterator{};

			init(
				result,
				&m_index,
				&m_storage,
				m_index.get_root()
			);

			return result;
		};
		::uns::trees::flex::iterator root() noexcept {
			auto result = ::uns::trees::flex::iterator{};

			init(
				result,
				&m_index,
				&m_storage,
				m_index.get_root()
			);

			return result;
		};
	public:
		inline void set_root(const value_type& Value) noexcept {
			auto root_ref = m_index.get_root();

			if(root_ref == m_index.get_null()) {
				if(m_storage.size() == 0) {
					m_storage.push_back(Value);
				}
				else {
					m_storage[0] = Value;
				};

				m_index.set_root(0);
			}
			else {
				auto root_idx = m_index.get(root_ref);

				if(m_storage.size() <= root_idx) {
					m_storage.resize(root_idx + 1);
				};

				m_storage[root_idx] = Value;
			};
		};
		inline void swap(
			const ::uns::trees::flex::const_iterator& SubTree1,
			const ::uns::trees::flex::const_iterator& SubTree2
		) noexcept {
			m_index.swap(
				get_ref(SubTree1.subnodes),
				get_ref(SubTree2.subnodes)
			);
		};
	protected:
		inline void check_references(
			::std::vector<bool>& Referenced,
			::uns::trees::auxiliary::flex::index::reference Ref
		) noexcept {
			Referenced[
				m_index.get(Ref)
			] = true;

			const auto subnodes_total = m_index.get_subnodes_total(Ref);
			for(::std::size_t subnode_idx = 0; subnode_idx < subnodes_total; ++subnode_idx) {
				check_references(
					Referenced,
					m_index.get_subnode(Ref, subnode_idx)
				);
			};
		};
	public:
		inline void gc() noexcept {
			m_index.gc();

			auto referenced = ::std::vector<bool>{};
			referenced.resize(m_storage.size(), false);

			if(m_index.get_root() != m_index.get_null()) {
				check_references(
					referenced,
					m_index.get_root()
				);
			};

			auto values_iter = m_storage.cbegin();
			auto referenced_iter = referenced.cbegin();
			auto values_end = m_storage.cend();
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
					values_iter = m_storage.erase(values_iter);
					++referenced_iter;
				};
			};

			m_storage.shrink_to_fit();
		};
	};

};

#endif
