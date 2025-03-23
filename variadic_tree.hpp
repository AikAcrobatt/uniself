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

	};


	class iterator;
	class subnodes;


	class const_iterator {
	protected:
		const ::std::vector<value_type>& m_values;
		const ::uns::variadic_tree::index& m_index;
		const ::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	public:
		const_iterator(
			const ::uns::variadic_tree::iterator& Iterator
		) noexcept;
		const_iterator(
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
	public:
		::uns::variadic_tree::const_iterator cbegin() const noexcept;
		::uns::variadic_tree::const_iterator cend() const noexcept;
	public:
		const ::uns::variadic_tree::subnodes& subnodes() const noexcept {						//returns a proxy object representing a set of subnodes of the current node
			return ::uns::variadic_tree::subnodes{
				m_index,
				m_values,
				m_size,
				m_ref
			};
		};
		::uns::variadic_tree::const_iterator subnodes(::std::size_t idx) const noexcept;		//returns an iterator pointing to #idx subnode, or end() if there is no such subnode
		::uns::variadic_tree::const_iterator parent() const noexcept { return cend(); };		//returns an iterator to the parent node. If this is a root returns end()
	public:
		const value_type& operator*() const noexcept;
		const value_type& operator->() const noexcept;
		::uns::variadic_tree::const_iterator& operator++() noexcept;
	};


	class iterator {
		friend const_iterator;
	protected:
		::std::vector<value_type>& m_values;
		::uns::variadic_tree::index& m_index;
		::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	public:
		iterator(
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
	public:
		::uns::variadic_tree::const_iterator cbegin() const noexcept;
		::uns::variadic_tree::const_iterator cend() const noexcept;
		::uns::variadic_tree::iterator begin() noexcept;
		::uns::variadic_tree::iterator end() noexcept;
	public:
		const ::uns::variadic_tree::subnodes& subnodes() const noexcept;						//returns a proxy object representing a set of subnodes of the current node
		::uns::variadic_tree::subnodes& subnodes() noexcept;									//returns a proxy object representing a set of subnodes of the current node
		::uns::variadic_tree::const_iterator subnodes(::std::size_t idx) const noexcept;		//returns an iterator pointing to #idx subnode, or end() if there is no such subnode
		::uns::variadic_tree::iterator subnodes(::std::size_t idx) noexcept;					//returns an iterator pointing to #idx subnode, or end() if there is no such subnode
		::uns::variadic_tree::const_iterator parent() const noexcept { return cend(); };		//returns an iterator to the parent node. If this is a root returns end()
		::uns::variadic_tree::iterator parent() noexcept { return end(); };						//returns an iterator to the parent node. If this is a root returns end()
	public:
		const value_type& operator*() const noexcept;
		value_type& operator*() noexcept;
		const value_type& operator->() const noexcept;
		value_type& operator->() noexcept;
		::uns::variadic_tree::iterator& operator++() noexcept;
	};


	class subnodes {
	protected:
		::std::vector<value_type>& m_values;
		::uns::variadic_tree::index& m_index;
		::std::size_t& m_size;
		::uns::variadic_tree::index::reference m_ref;
	public:
		subnodes(
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
	public:
		::std::size_t size() const noexcept;
	public:
		void push_back(::uns::variadic_tree::const_iterator SomeTree) noexcept;					//copies the SomeTree and makes it the last subnode
		void push_back(const value_type& SomeValue) noexcept;									//creates a new subnode and puts there a SomeValue
		bool insert(
			::uns::variadic_tree::const_iterator InsertBeforeThis,
			::uns::variadic_tree::const_iterator SomeTree
		) noexcept;																				//inserts SomeTree in the position before InsertBeforeThis in the same current subnodes set
		bool remove(::uns::variadic_tree::const_iterator SomeTree) noexcept;					//removes SomeTree from subnodes if it is a subnode (and returns true), else do nothing and returns false
	};


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

	tree.roots().push_back(value_type{ ... });
	tree.roots(0).subnodes().push_back(value_type{ ... });

	static_assert(tree.size() == 2);
	static_assert(tree.roots().size() == 1);
	static_assert(tree.roots(0).subnodes().size() == 1);

	for(auto tree_iterator = tree.begin(); tree_iterator != tree.end(); ++tree_iterator) {
		::std::cout << *tree_iterator << ::std::endl;
	};
	*/
};

#endif
