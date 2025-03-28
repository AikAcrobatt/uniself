#pragma once

#include <iostream>
#include <vector>

#ifndef UNS_LIB_VARIADIC_TREE
#define UNS_LIB_VARIADIC_TREE "variadic_tree.hpp"


namespace uns::variadic_tree {

	class value_type {};


	class traversal;


	class index {
	public:
		class reference {
		public:
			explicit reference(::std::size_t Idx) noexcept;
		public:
			explicit operator ::std::size_t() const noexcept;
		};
	protected:
	public:
		::uns::variadic_tree::index::reference root() const noexcept;
		::uns::variadic_tree::index::reference root() noexcept;
		::uns::variadic_tree::index::reference none() const noexcept;
	public:
		::std::size_t size() const noexcept;
		::std::size_t value_idx(
			const ::uns::variadic_tree::index::reference& Ref
		) const noexcept;
	public:
		::std::size_t size(const ::uns::variadic_tree::index::reference& Ref) const noexcept;
		::uns::variadic_tree::index::reference parent(const ::uns::variadic_tree::index::reference& Ref) const noexcept;
		::uns::variadic_tree::index::reference subnode(
			const ::uns::variadic_tree::index::reference& Ref,
			::std::size_t SubnodeIdx
		) const noexcept;
		void swap(
			const ::uns::variadic_tree::index::reference& SubTree1,
			const ::uns::variadic_tree::index::reference& SubTree2
		) noexcept;
		void shrink_to_fit() noexcept;
	};


	class traversal {
	public:
		static ::uns::variadic_tree::index::reference next(
			const ::uns::variadic_tree::index& Index,
			::uns::variadic_tree::index::reference Current
		) noexcept;
		static ::uns::variadic_tree::index::reference begin(
			const ::uns::variadic_tree::index& Index,
			::uns::variadic_tree::index::reference Current
		) noexcept;
		static ::uns::variadic_tree::index::reference end(
			const ::uns::variadic_tree::index& Index,
			::uns::variadic_tree::index::reference Current
		) noexcept;
	};


	class const_iterator;
	class iterator;
	class tree;


	class const_subnodes {
		friend const_iterator;
		friend tree;
	protected:
		const ::std::vector<value_type>& m_values;
		const ::uns::variadic_tree::index& m_index;
		const ::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	protected:
		inline const_subnodes(
			const ::uns::variadic_tree::index& Index,
			const ::std::vector<value_type>& Values,
			const ::std::size_t& TreeSize,
			::uns::variadic_tree::index::reference Reference
		) noexcept :
			m_index(Index),
			m_values(Values),
			m_size(TreeSize),
			m_ref(Reference)
		{};
		inline const_subnodes(const ::uns::variadic_tree::const_subnodes& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(Obj.m_ref)
		{};
		inline::uns::variadic_tree::const_subnodes& operator=(const ::uns::variadic_tree::const_subnodes& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&m_index != &Obj.m_index) return *this;
			if(&m_values != &Obj.m_values) return *this;
			if(&m_size != &Obj.m_size) return *this;

			m_ref = Obj.m_ref;

			return *this;
		};
		inline const_subnodes(::uns::variadic_tree::const_subnodes&& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(::std::move(Obj.m_ref))
		{};
		inline::uns::variadic_tree::const_subnodes& operator=(::uns::variadic_tree::const_subnodes&& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&m_index != &Obj.m_index) return *this;
			if(&m_values != &Obj.m_values) return *this;
			if(&m_size != &Obj.m_size) return *this;

			m_ref = ::std::move(Obj.m_ref);

			return *this;
		};
	public:
		~const_subnodes() noexcept = default;
	public:
		inline ::std::size_t size() const noexcept { return m_index.size(m_ref); };
		inline ::uns::variadic_tree::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
	protected:
		inline ::uns::variadic_tree::index::reference parent() const noexcept { return m_index.parent(m_ref); };
	};


	class subnodes {
		friend const_iterator;
		friend iterator;
		friend tree;
	protected:
		::std::vector<value_type>& m_values;
		::uns::variadic_tree::index& m_index;
		::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	protected:
		inline subnodes(
			::uns::variadic_tree::index& Index,
			::std::vector<value_type>& Values,
			::std::size_t& TreeSize,
			::uns::variadic_tree::index::reference Reference
		) noexcept :
			m_index(Index),
			m_values(Values),
			m_size(TreeSize),
			m_ref(Reference)
		{};
		inline subnodes(const ::uns::variadic_tree::subnodes& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(Obj.m_ref)
		{};
		inline::uns::variadic_tree::subnodes& operator=(const ::uns::variadic_tree::subnodes& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&m_index != &Obj.m_index) return *this;
			if(&m_values != &Obj.m_values) return *this;
			if(&m_size != &Obj.m_size) return *this;

			m_ref = Obj.m_ref;

			return *this;
		};
		inline subnodes(::uns::variadic_tree::subnodes&& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(::std::move(Obj.m_ref))
		{};
		inline::uns::variadic_tree::subnodes& operator=(::uns::variadic_tree::subnodes&& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&m_index != &Obj.m_index) return *this;
			if(&m_values != &Obj.m_values) return *this;
			if(&m_size != &Obj.m_size) return *this;

			m_ref = ::std::move(Obj.m_ref);

			return *this;
		};
	public:
		~subnodes() noexcept = default;
	public:
		inline ::std::size_t size() const noexcept { return m_index.size(m_ref); };
		inline ::uns::variadic_tree::const_iterator operator[](::std::size_t SubnodeIdx) const noexcept;
		inline ::uns::variadic_tree::iterator operator[](::std::size_t SubnodeIdx) noexcept;
	protected:
		inline ::uns::variadic_tree::index::reference parent() const noexcept { return m_index.parent(m_ref); };
	public:
		inline void push_back(::uns::variadic_tree::const_iterator SomeTree) noexcept;				//copies the SomeTree and makes it the last subnode
		inline void push_back(const value_type& SomeValue) noexcept;								//creates a new subnode and puts there a SomeValue
		inline bool insert(																			//inserts SomeTree in the position before InsertBeforeThis in the same current subnodes set
			::uns::variadic_tree::const_iterator InsertBeforeThis,
			::uns::variadic_tree::const_iterator SomeTree
		) noexcept;
		inline bool remove(::uns::variadic_tree::const_iterator SomeTree) noexcept;					//removes SomeTree from subnodes if it is a subnode (and returns true), else do nothing and returns false
	};


	class const_iterator {
		friend const_subnodes;
		friend iterator;
		friend tree;
	public:
		::uns::variadic_tree::const_subnodes subnodes;
	public:
		const_iterator(
			const ::uns::variadic_tree::iterator& Iterator
		) noexcept;
	protected:
		inline const_iterator(
			const ::uns::variadic_tree::index& Index,
			const ::std::vector<value_type>& Values,
			const ::std::size_t& TreeSize,
			::uns::variadic_tree::index::reference Reference
		) noexcept :
			subnodes(
				Index,
				Values,
				TreeSize,
				Reference
			)
		{};
	public:
		inline const_iterator(const ::uns::variadic_tree::const_iterator& Obj) noexcept :
			subnodes(Obj.subnodes)
		{};
		inline ::uns::variadic_tree::const_iterator& operator=(const ::uns::variadic_tree::const_iterator& Obj) noexcept {
			if(this == &Obj) return *this;

			subnodes = Obj.subnodes;

			return *this;
		};
		inline const_iterator(::uns::variadic_tree::const_iterator&& Obj) noexcept :
			subnodes(::std::move(Obj.subnodes)) {};
		inline ::uns::variadic_tree::const_iterator& operator=(::uns::variadic_tree::const_iterator&& Obj) noexcept {
			if(this == &Obj) return *this;

			subnodes = ::std::move(Obj.subnodes);

			return *this;
		};
		~const_iterator() noexcept = default;
	public:
		::uns::variadic_tree::const_iterator cbegin() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				traversal::begin(subnodes.m_index, subnodes.m_ref)
			};
		};
		::uns::variadic_tree::const_iterator cend() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				traversal::end(subnodes.m_index, subnodes.m_ref)
			};
		};
	public:
		::uns::variadic_tree::const_iterator parent() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
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
		inline const value_type* operator->() const noexcept {
			return &subnodes.m_values[
				subnodes.m_index.value_idx(
					subnodes.m_ref
				)
			];
		};
		inline ::uns::variadic_tree::iterator& operator++() noexcept {
			subnodes.m_ref = traversal::next(subnodes.m_index, subnodes.m_ref);
		};
	};


	class iterator {
		friend subnodes;
		friend const_iterator;
		friend tree;
	public:
		::uns::variadic_tree::subnodes subnodes;
	protected:
		inline iterator(
			::uns::variadic_tree::index& Index,
			::std::vector<value_type>& Values,
			::std::size_t& TreeSize,
			::uns::variadic_tree::index::reference Reference
		) noexcept :
			subnodes(
				Index,
				Values,
				TreeSize,
				Reference
			)
		{};
	public:
		inline iterator(const ::uns::variadic_tree::iterator& Obj) noexcept :
			subnodes(Obj.subnodes)
		{};
		inline ::uns::variadic_tree::iterator& operator=(const ::uns::variadic_tree::iterator& Obj) noexcept {
			if(this == &Obj) return *this;
			
			subnodes = Obj.subnodes;

			return *this;
		};
		inline iterator(::uns::variadic_tree::iterator&& Obj) noexcept :
			subnodes(::std::move(Obj.subnodes))
		{};
		inline ::uns::variadic_tree::iterator& operator=(::uns::variadic_tree::iterator&& Obj) noexcept {
			if(this == &Obj) return *this;
			
			subnodes = ::std::move(Obj.subnodes);

			return *this;
		};
		~iterator() noexcept = default;
	public:
		inline ::uns::variadic_tree::const_iterator cbegin() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				traversal::begin(subnodes.m_index, subnodes.m_ref)
			};
		};
		inline ::uns::variadic_tree::const_iterator cend() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				traversal::end(subnodes.m_index, subnodes.m_ref)
			};
		};
		inline ::uns::variadic_tree::iterator begin() noexcept {
			return ::uns::variadic_tree::iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				traversal::begin(subnodes.m_index, subnodes.m_ref)
			};
		};
		inline ::uns::variadic_tree::iterator end() noexcept {
			return ::uns::variadic_tree::iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				traversal::end(subnodes.m_index, subnodes.m_ref)
			};
		};
	public:
		inline ::uns::variadic_tree::const_iterator parent() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
				subnodes.m_index.parent(subnodes.m_ref)
			};
		};
		inline ::uns::variadic_tree::iterator parent() noexcept {										//returns an iterator to the parent node. If this is a root returns end()
			return ::uns::variadic_tree::iterator{
				subnodes.m_index,
				subnodes.m_values,
				subnodes.m_size,
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
		inline ::uns::variadic_tree::iterator& operator++() noexcept {
			subnodes.m_ref = traversal::next(subnodes.m_index, subnodes.m_ref);
		};
	};


	::uns::variadic_tree::const_iterator uns::variadic_tree::const_subnodes::operator[](::std::size_t SubnodeIdx) const noexcept {
		return ::uns::variadic_tree::const_iterator{
			m_index,
			m_values,
			m_size,
			m_index.subnode(m_ref, SubnodeIdx)
		};
	};
	::uns::variadic_tree::const_iterator uns::variadic_tree::subnodes::operator[](::std::size_t SubnodeIdx) const noexcept {
		return ::uns::variadic_tree::iterator{
			m_index,
			m_values,
			m_size,
			m_index.subnode(m_ref, SubnodeIdx)
		};

	};
	::uns::variadic_tree::iterator uns::variadic_tree::subnodes::operator[](::std::size_t SubnodeIdx) noexcept {
		return ::uns::variadic_tree::iterator{
			m_index,
			m_values,
			m_size,
			m_index.subnode(m_ref, SubnodeIdx)
		};
	
	};


	::uns::variadic_tree::const_iterator::const_iterator(
		const ::uns::variadic_tree::iterator& Iterator
	) noexcept :
		subnodes(
			Iterator.subnodes.m_index,
			Iterator.subnodes.m_values,
			Iterator.subnodes.m_size,
			Iterator.subnodes.m_ref
		)
	{};


	class tree {
	protected:
		::std::vector<value_type> m_values;
		::uns::variadic_tree::index m_index;
		::std::size_t m_size = 0;
	public:
		::uns::variadic_tree::const_iterator cbegin() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				m_index,
				m_values,
				m_size,
				m_index.root()
			};
		};
		::uns::variadic_tree::const_iterator cend() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				m_index,
				m_values,
				m_size,
				m_index.none()
			};
		};
		::uns::variadic_tree::iterator begin() noexcept {
			return ::uns::variadic_tree::iterator{
				m_index,
				m_values,
				m_size,
				m_index.root()
			};
		};
		::uns::variadic_tree::iterator end() noexcept {
			return ::uns::variadic_tree::iterator{
				m_index,
				m_values,
				m_size,
				m_index.none()
			};
		};
		::std::size_t size() const noexcept { return m_size; };
	public:
		::uns::variadic_tree::const_iterator root() const noexcept { return cbegin(); };		//returns an iterator pointing to the root, or end() if the tree is empty
		::uns::variadic_tree::iterator root() noexcept { return begin(); };						//returns an iterator pointing to the root, or end() if the tree is empty
	public:
		inline void set_root(const value_type& Value) noexcept {
			auto root_ref = m_index.root();
			auto root_idx = m_index.value_idx(root_ref);

			if(m_values.size() <= root_idx) {
				m_values.resize(root_idx + 1);
				++m_size;
			};

			m_values[root_idx] = Value;

			++m_size;
		};
		inline void swap(																		//swaps two trees
			const ::uns::variadic_tree::iterator& SubTree1,
			const ::uns::variadic_tree::iterator& SubTree2
		) noexcept {
			m_index.swap(SubTree1.subnodes.m_ref, SubTree2.subnodes.m_ref);
		};
		inline void shrink_to_fit() noexcept {
			m_index.shrink_to_fit();

			/*
			TODO to traverse all across the index tree and to mark unused m_values
				to delete unused m_values
			*/

			m_values.shrink_to_fit();
		};
	};

	/*
	EXAMPLE:

	auto tree = ::uns::variadic_tree::tree{};

	tree.set_root(value_type{ ... });
	auto root = tree.root();
	root.subnodes.push_back(value_type{ ... });

	static_assert(tree.size() == 2);
	static_assert(root.subnodes.size() == 1);

	for(auto tree_iterator = tree.begin(); tree_iterator != tree.end(); ++tree_iterator) {
		::std::cout << *tree_iterator << ::std::endl;
	};
	*/
};

#endif
