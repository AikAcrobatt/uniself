#pragma once

#include <vector>
#include <optional>

#ifndef UNS_LIB_VARIADIC_TREE
#define UNS_LIB_VARIADIC_TREE "variadic_tree.hpp"


namespace uns::variadic_tree {

	namespace pseudo {

		class value_type {};


		class iterator;
		class subnodes;


		class tree {
		public:
			iterator begin();
			iterator end();
			::std::size_t size();
		public:
			subnodes roots();											//returns a proxy object representing a set of roots of the forest
			iterator roots(::std::size_t idx);							//returns an iterator pointing to #idx root, or end() if there is no such root
		public:
			void swap(iterator SubTree1, iterator SubTree2);			//swaps two trees
			void shrink_to_fit();
		};


		class iterator {
		public:
			subnodes subnodes();										//returns a proxy object representing a set of subnodes of the current node
			iterator subnodes(::std::size_t idx);						//returns an iterator pointing to #idx subnode, or end() if there is no such subnode
		public:
			value_type operator*();
			value_type operator->();
			iterator operator++();
		};


		class subnodes {
		public:
			iterator begin();
			iterator end();
			::std::size_t size();
		public:
			void push_back(iterator SomeTree);							//copies the SomeTree and makes it the last subnode
			void push_back(value_type SomeValue);						//creates a new subnode and puts there a SomeValue
			bool insert(iterator InsertBeforeThis, iterator SomeTree);	//inserts SomeTree in the position before InsertBeforeThis in the same current subnodes set
			bool remove(iterator SomeTree);								//removes SomeTree from subnodes if it is a subnode (and returns true), else do nothing and returns false
		};
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
