#pragma once

#include <vector>
#include <optional>

#ifndef UNS_LIB_VARIADIC_TREE
#define UNS_LIB_VARIADIC_TREE "variadic_tree.hpp"

namespace uns {

	template<typename contained_t>
	class variadic_tree {
	public:
		using value_type = contained_t;
	protected:
		class adjacency_list_node {
		public:
			using reference_type = ::std::size_t;
		public:
			::std::optional<reference_type> parent;
			::std::vector<reference_type> subnodes;
		};
	protected:
		::std::vector<value_type> m_values;
		::std::vector<adjacency_list_node> m_adjacency_list;
		::std::vector<typename adjacency_list_node::reference_type> m_roots;
	public:
	protected:
		typename adjacency_list_node::reference_type access(const adjacency_list_node& current_node, ::std::size_t subnode_idx, ::std::size_t ... subnodes_indexes) const noexcept {
			return access(m_adjacency_list[current_node.subnodes[subnode_idx]], subnodes_indexes);
		};
		typename adjacency_list_node::reference_type access(const adjacency_list_node& current_node, ::std::size_t leaf_idx) const noexcept {
			return current_node.subnodes[leaf_idx];
		};
		typename adjacency_list_node::reference_type access(::std::size_t root_idx, ::std::size_t ... subnodes_indexes) const noexcept {
			return access(m_adjacency_list[m_roots[root_idx]], subnodes_indexes);
		};
		typename adjacency_list_node::reference_type access(::std::size_t root_idx) const noexcept {
			return m_roots[root_idx];
		};
	public:
		const value_type& operator() (::std::size_t ... indexes) const noexcept {
			return m_values[access(indexes)];
		};
		value_type& operator() (::std::size_t ... indexes) noexcept {
			return m_values[access(indexes)];
		};
	public:
	};

};

#endif
