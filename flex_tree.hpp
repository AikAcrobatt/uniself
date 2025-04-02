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


	class member_accessor {
	public:
		virtual const ::uns::trees::auxiliary::flex::index& get_index() const noexcept = 0;
	protected:
		virtual const ::uns::trees::auxiliary::flex::index& get_index(
			const member_accessor&
		) const noexcept = 0;
	public:
		virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept = 0;
	protected:
		virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
			const member_accessor&
		) const noexcept = 0;
	public:
		virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref() const noexcept = 0;
		virtual void set_ref(const ::uns::trees::auxiliary::flex::index::reference&) noexcept = 0;
	protected:
		virtual const ::uns::trees::auxiliary::flex::index::reference& get_ref(
			const member_accessor&
		) const noexcept = 0;
		virtual void set_ref(
			member_accessor&,
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

	class flex: public ::uns::trees::auxiliary::flex::member_accessor {
	public:
		class const_subnodes;
		class subnodes;
		class const_iterator;
		class iterator;
	public:
		class const_subnodes: public ::uns::trees::auxiliary::flex::member_accessor {
		protected:
			const ::uns::trees::auxiliary::flex::index& m_index;
			const ::std::vector<value_type>& m_storage;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			inline const_subnodes(
				const ::uns::trees::auxiliary::flex::index& Index,
				const ::std::vector<value_type>& Values,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept :
				m_index(Index),
				m_storage(Values),
				m_ref(Reference)
			{};
			inline const_subnodes(const ::uns::trees::auxiliary::flex::member_accessor& Accessor) noexcept :
				const_subnodes(
					Accessor.get_index(),
					Accessor.get_storage(),
					Accessor.get_ref()
				)
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
			virtual const ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return m_index;
			};
			virtual const ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return m_storage;
			};
			virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
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
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				member_accessor& Accessor,
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
		class subnodes: public ::uns::trees::auxiliary::flex::member_accessor {
		protected:
			::uns::trees::auxiliary::flex::index& m_index;
			::std::vector<value_type>& m_storage;
			::uns::trees::auxiliary::flex::index::reference m_ref;
		public:
			inline subnodes(
				::uns::trees::auxiliary::flex::index& Index,
				::std::vector<value_type>& Values,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept :
				m_index(Index),
				m_storage(Values),
				m_ref(Reference)
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
			virtual const ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return m_index;
			};
			virtual const ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return m_storage;
			};
			virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
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
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				member_accessor& Accessor,
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
		class const_iterator: public ::uns::trees::auxiliary::flex::member_accessor {
		public:
			::uns::trees::flex::const_subnodes subnodes;
		public:
			const_iterator(
				const ::uns::trees::flex::iterator& Iterator
			) noexcept;
			inline const_iterator(
				const ::uns::trees::auxiliary::flex::index& Index,
				const ::std::vector<value_type>& Values,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept :
				subnodes(
					Index,
					Values,
					Reference
				)
			{};
			inline const_iterator(const ::uns::trees::flex::const_iterator& Obj) noexcept :
				subnodes(
					Obj.get_index(),
					Obj.get_storage(),
					Obj.get_ref()
				)
			{};
			inline ::uns::trees::flex::const_iterator& operator=(const ::uns::trees::flex::const_iterator& Obj) noexcept {
				if(this == &Obj) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline const_iterator(::uns::trees::flex::const_iterator&& Obj) noexcept :
				subnodes(
					get_index(Obj.subnodes),
					get_storage(Obj.subnodes),
					get_ref(Obj.subnodes)
				)
			{};
			inline ::uns::trees::flex::const_iterator& operator=(::uns::trees::flex::const_iterator&& Obj) noexcept {
				if(this == &Obj) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~const_iterator() noexcept = default;
		protected:
			virtual const ::uns::trees::auxiliary::flex::index& get_index() const noexcept override {
				return get_index(subnodes);
			};
			virtual const ::uns::trees::auxiliary::flex::index& get_index(
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
			) const noexcept override {
				return Accessor.get_index();
			};
		protected:
			virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage() const noexcept override {
				return get_storage(subnodes);
			};
			virtual const ::uns::trees::auxiliary::flex::value_storage_type<value_type>& get_storage(
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
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
				const ::uns::trees::auxiliary::flex::member_accessor& Accessor
			) const noexcept override {
				return Accessor.get_ref();
			};
			virtual void set_ref(
				member_accessor& Accessor,
				const ::uns::trees::auxiliary::flex::index::reference& Reference
			) const noexcept override {
				Accessor.set_ref(Reference);
			};
		public:
			::uns::trees::flex::const_iterator cbegin() const noexcept {
				return ::uns::trees::flex::const_iterator{
					get_index(subnodes),
					get_storage(subnodes),
					traversal::begin(get_index(subnodes), get_ref(subnodes))
				};
			};
			::uns::trees::flex::const_iterator cend() const noexcept {
				return ::uns::trees::flex::const_iterator{
					get_index(subnodes),
					get_storage(subnodes),
					traversal::end(get_index(subnodes), get_ref(subnodes))
				};
			};
		public:
			::uns::trees::flex::const_iterator parent() const noexcept {
				return ::uns::trees::flex::const_iterator{
					get_index(subnodes),
					get_storage(subnodes),
					get_index(subnodes).get_parent(get_ref(subnodes))
				};
			};
		public:
			inline const value_type& operator*() const noexcept {
				return get_storage(subnodes)[
					get_index(subnodes).get(get_ref(subnodes))
				];
			};
			inline const value_type* operator->() const noexcept {
				return &get_storage(subnodes)[
					get_index(subnodes).get(get_ref(subnodes))
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				set_ref(subnodes, traversal::next(get_index(subnodes), get_ref(subnodes)));
			};
		};
	public:
		class iterator: public ::uns::trees::auxiliary::flex::member_accessor {
		public:
			::uns::trees::flex::subnodes subnodes;
		public:
			inline iterator(
				::uns::trees::auxiliary::flex::index& Index,
				::std::vector<value_type>& Values,
				::uns::trees::auxiliary::flex::index::reference Reference
			) noexcept :
				subnodes(
					Index,
					Values,
					Reference
				)
			{};
			inline iterator(const ::uns::trees::flex::iterator& Obj) noexcept :
				iterator(
					Obj.get_index(),
					Obj.get_storage(),
					Obj.get_ref()
				)
			{};
			inline ::uns::trees::flex::iterator& operator=(const ::uns::trees::flex::iterator& Obj) noexcept {
				if(this == &Obj) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			inline iterator(::uns::trees::flex::iterator&& Obj) noexcept :
				subnodes(
					get_index(Obj.subnodes),
					get_storage(Obj.subnodes),
					get_ref(Obj.subnodes)
				)
			{};
			inline ::uns::trees::flex::iterator& operator=(::uns::trees::flex::iterator&& Obj) noexcept {
				if(this == &Obj) return *this;

				set_ref(subnodes, get_ref(Obj.subnodes));

				return *this;
			};
			~iterator() noexcept = default;
		public:
			inline ::uns::trees::flex::const_iterator cbegin() const noexcept {
				return ::uns::trees::flex::const_iterator{
					subnodes.m_index,
					subnodes.m_values,
					traversal::begin(subnodes.m_index, subnodes.m_ref)
				};
			};
			inline ::uns::trees::flex::const_iterator cend() const noexcept {
				return ::uns::trees::flex::const_iterator{
					subnodes.m_index,
					subnodes.m_values,
					traversal::end(subnodes.m_index, subnodes.m_ref)
				};
			};
			inline ::uns::trees::flex::iterator begin() noexcept {
				return ::uns::trees::flex::iterator{
					subnodes.m_index,
					subnodes.m_values,
					subnodes.m_size,
					traversal::begin(subnodes.m_index, subnodes.m_ref)
				};
			};
			inline ::uns::trees::flex::iterator end() noexcept {
				return ::uns::trees::flex::iterator{
					subnodes.m_index,
					subnodes.m_values,
					traversal::end(subnodes.m_index, subnodes.m_ref)
				};
			};
		public:
			inline ::uns::trees::flex::const_iterator parent() const noexcept {
				return ::uns::trees::flex::const_iterator{
					subnodes.m_index,
					subnodes.m_values,
					subnodes.m_index.parent(subnodes.m_ref)
				};
			};
			inline ::uns::trees::flex::iterator parent() noexcept {										//returns an iterator to the parent node. If this is a root returns end()
				return ::uns::trees::flex::iterator{
					subnodes.m_index,
					subnodes.m_values,
					subnodes.m_index.parent(subnodes.m_ref)
				};
			};
		public:
			inline const value_type& operator*() const noexcept {
				return subnodes.m_values[
					subnodes.m_index.value_idx(
						subnodes.m_ref
					)
				];
			};
			inline value_type& operator*() noexcept {
				return subnodes.m_values[
					subnodes.m_index.value_idx(
						subnodes.m_ref
					)
				];
			};
			inline const value_type* operator->() const noexcept {
				return &subnodes.m_values[
					subnodes.m_index.value_idx(
						subnodes.m_ref
					)
				];
			};
			inline value_type* operator->() noexcept {
				return &subnodes.m_values[
					subnodes.m_index.value_idx(
						subnodes.m_ref
					)
				];
			};
			inline ::uns::trees::flex::iterator& operator++() noexcept {
				subnodes.m_ref = traversal::next(subnodes.m_index, subnodes.m_ref);
			};
		};
















	
	};

};

#endif
