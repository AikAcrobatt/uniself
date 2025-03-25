#pragma once

#include <iostream>
#include <vector>

#ifndef UNS_LIB_VARIADIC_TREE
#define UNS_LIB_VARIADIC_TREE "variadic_tree.hpp"


namespace uns::variadic_tree {

	class value_type {};


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
		::std::size_t size() const noexcept;
		void resize(::std::size_t NewSize) noexcept;
	public:
		::std::size_t size(const ::uns::variadic_tree::index::reference& Ref) const noexcept;
		::uns::variadic_tree::index::reference parent(const ::uns::variadic_tree::index::reference& Ref) const noexcept;
	};


	class const_iterator;
	class iterator;


	class const_subnodes {
		friend const_iterator;
	protected:
		const ::std::vector<value_type>& m_values;
		const ::uns::variadic_tree::index& m_index;
		const ::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	public:
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
	protected:
		inline const_subnodes(const ::uns::variadic_tree::const_subnodes& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(Obj.m_ref) {};
		inline::uns::variadic_tree::const_subnodes& operator=(const ::uns::variadic_tree::const_subnodes& Obj) = delete;
		inline const_subnodes(::uns::variadic_tree::const_subnodes&& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(::std::move(Obj.m_ref)) {};
		inline::uns::variadic_tree::const_subnodes& operator=(::uns::variadic_tree::const_subnodes&& Obj) = delete;
	public:
		~const_subnodes() noexcept = default;
	public:
		inline ::std::size_t size() const noexcept { return m_index.size(m_ref); };
	protected:
		inline ::uns::variadic_tree::index::reference parent() const noexcept { return m_index.parent(m_ref); };
	};


	class subnodes {
		friend const_iterator;
		friend iterator;
	protected:
		::std::vector<value_type>& m_values;
		::uns::variadic_tree::index& m_index;
		::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	public:
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
	protected:
		inline subnodes(const ::uns::variadic_tree::subnodes& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(Obj.m_ref)
		{};
		inline::uns::variadic_tree::subnodes& operator=(const ::uns::variadic_tree::subnodes& Obj) = delete;
		inline subnodes(::uns::variadic_tree::subnodes&& Obj) noexcept :
			m_index(Obj.m_index),
			m_values(Obj.m_values),
			m_size(Obj.m_size),
			m_ref(::std::move(Obj.m_ref))
		{};
		inline::uns::variadic_tree::subnodes& operator=(::uns::variadic_tree::subnodes&& Obj) = delete;
	public:
		~subnodes() noexcept = default;
	public:
		inline ::std::size_t size() const noexcept { return m_index.size(m_ref); };
	protected:
		::uns::variadic_tree::index::reference parent() const noexcept { return m_index.parent(m_ref); };
	public:
		void push_back(::uns::variadic_tree::const_iterator SomeTree) noexcept;					//copies the SomeTree and makes it the last subnode
		void push_back(const value_type& SomeValue) noexcept;									//creates a new subnode and puts there a SomeValue
		bool insert(																			//inserts SomeTree in the position before InsertBeforeThis in the same current subnodes set
			::uns::variadic_tree::const_iterator InsertBeforeThis,
			::uns::variadic_tree::const_iterator SomeTree
		) noexcept;
		bool remove(::uns::variadic_tree::const_iterator SomeTree) noexcept;					//removes SomeTree from subnodes if it is a subnode (and returns true), else do nothing and returns false
	};


	class const_iterator {
	public:
		::uns::variadic_tree::const_subnodes subnodes;
	public:
		const_iterator(
			const ::uns::variadic_tree::iterator& Iterator
		) noexcept;
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
		inline const_iterator(const ::uns::variadic_tree::const_iterator& Obj) noexcept :
			subnodes(Obj.subnodes)
		{};
		inline ::uns::variadic_tree::const_iterator& operator=(const ::uns::variadic_tree::const_iterator& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&subnodes.m_index != &Obj.subnodes.m_index) return *this;
			if(&subnodes.m_values != &Obj.subnodes.m_values) return *this;
			if(&subnodes.m_size != &Obj.subnodes.m_size) return *this;

			subnodes.m_ref = Obj.subnodes.m_ref;

			return *this;
		};
		inline const_iterator(const ::uns::variadic_tree::const_iterator&& Obj) noexcept :
			subnodes(::std::move(Obj.subnodes)) {};
		inline ::uns::variadic_tree::const_iterator& operator=(::uns::variadic_tree::const_iterator&& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&subnodes.m_index != &Obj.subnodes.m_index) return *this;
			if(&subnodes.m_values != &Obj.subnodes.m_values) return *this;
			if(&subnodes.m_size != &Obj.subnodes.m_size) return *this;

			subnodes.m_ref = ::std::move(Obj.subnodes.m_ref);

			return *this;
		};
		~const_iterator() noexcept = default;
	public:
		::uns::variadic_tree::const_iterator cbegin() const noexcept;
		::uns::variadic_tree::const_iterator cend() const noexcept;
	public:
		::uns::variadic_tree::const_iterator parent() const noexcept { return cend(); };		//returns an iterator to the parent node. If this is a root returns end()
	public:
		const value_type& operator*() const noexcept;
		const value_type& operator->() const noexcept;
		::uns::variadic_tree::const_iterator& operator++() noexcept;
	};


	class iterator {
		friend const_iterator;
	public:
		::uns::variadic_tree::subnodes subnodes;
	public:
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
		inline iterator(const ::uns::variadic_tree::iterator& Obj) noexcept :
			subnodes(Obj.subnodes)
		{};
		inline ::uns::variadic_tree::iterator& operator=(const ::uns::variadic_tree::iterator& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&subnodes.m_index != &Obj.subnodes.m_index) return *this;
			if(&subnodes.m_values != &Obj.subnodes.m_values) return *this;
			if(&subnodes.m_size != &Obj.subnodes.m_size) return *this;

			subnodes.m_ref = Obj.subnodes.m_ref;

			return *this;
		};
		inline iterator(const ::uns::variadic_tree::iterator&& Obj) noexcept :
			subnodes(::std::move(Obj.subnodes))
		{};
		inline ::uns::variadic_tree::iterator& operator=(::uns::variadic_tree::iterator&& Obj) noexcept {
			if(this == &Obj) return *this;
			if(&subnodes.m_index != &Obj.subnodes.m_index) return *this;
			if(&subnodes.m_values != &Obj.subnodes.m_values) return *this;
			if(&subnodes.m_size != &Obj.subnodes.m_size) return *this;

			subnodes.m_ref = ::std::move(Obj.subnodes.m_ref);

			return *this;
		};
		~iterator() noexcept = default;
	public:
		::uns::variadic_tree::const_iterator cbegin() const noexcept;
		::uns::variadic_tree::const_iterator cend() const noexcept;
		::uns::variadic_tree::iterator begin() noexcept;
		::uns::variadic_tree::iterator end() noexcept;
	public:
		::uns::variadic_tree::const_iterator parent() const noexcept { return cend(); };		//returns an iterator to the parent node. If this is a root returns end()
		::uns::variadic_tree::iterator parent() noexcept { return end(); };						//returns an iterator to the parent node. If this is a root returns end()
	public:
		const value_type& operator*() const noexcept;
		value_type& operator*() noexcept;
		const value_type& operator->() const noexcept;
		value_type& operator->() noexcept;
		::uns::variadic_tree::iterator& operator++() noexcept;
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
				::uns::variadic_tree::index::reference{ 0 }
			};
		};
		::uns::variadic_tree::const_iterator cend() const noexcept {
			return ::uns::variadic_tree::const_iterator{
				m_index,
				m_values,
				m_size,
				::uns::variadic_tree::index::reference{ m_index.size() }
			};
		};
		::uns::variadic_tree::iterator begin() noexcept {
			return ::uns::variadic_tree::iterator{
				m_index,
				m_values,
				m_size,
				::uns::variadic_tree::index::reference{ 0 }
			};
		};
		::uns::variadic_tree::iterator end() noexcept {
			return ::uns::variadic_tree::iterator{
				m_index,
				m_values,
				m_size,
				::uns::variadic_tree::index::reference{ m_index.size() }
			};
		};
		::std::size_t size() const noexcept { return m_size; };
	public:
		::uns::variadic_tree::const_iterator root() const noexcept { return cbegin(); };		//returns an iterator pointing to the root, or end() if the tree is empty
		::uns::variadic_tree::iterator root() noexcept { return begin(); };						//returns an iterator pointing to the root, or end() if the tree is empty
	public:
		void set_root(const value_type& Value) noexcept {
			if(m_index.size() == 0) {
				m_index.resize(1);
			};
			if(m_values.size() == 0) {
				m_values.resize(1);
			};

			m_values[0] = Value;

			++m_size;
		};
		void swap(
			::uns::variadic_tree::iterator SubTree1,
			::uns::variadic_tree::iterator SubTree2
		) noexcept;																				//swaps two trees
		void shrink_to_fit() noexcept;
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
