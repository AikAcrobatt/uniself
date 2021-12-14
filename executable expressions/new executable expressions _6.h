#pragma once

#pragma once

#include <iostream>
#include <vector>
#include <any>
#include <exception>
#include <stdexcept>

#include "uniself\algorithms.h"


namespace uns::ex_ex {

    UNS_ENUM_DECLARATOR(common_type, int,
        Null,
        Number,
        Bool,
        String
    );

    class common {
    protected:
        std::any value;
        uns::ex_ex::common_type type;
    public:
        explicit common(const uns::ex_ex::common_type& set_type = uns::ex_ex::common_type::Null) noexcept : type(set_type) {
            switch(type) {
                default:
                case uns::ex_ex::common_type::Null:
                    return;
                case uns::ex_ex::common_type::Number: {
                    value = static_cast<long double>(0.0);
                    return;
                }
                case uns::ex_ex::common_type::Bool: {
                    value = false;
                    return;
                }
                case uns::ex_ex::common_type::String: {
                    value = std::wstring();
                    return;
                };
            };
        };

        template<typename assign_t>
        common(const assign_t& assign_what) noexcept : type(uns::ex_ex::common_type::Null) {};
        template<>
        common(const long double& assign_what) noexcept : type(uns::ex_ex::common_type::Number), value(assign_what) {};
        template<>
        common(const bool& assign_what) noexcept : type(uns::ex_ex::common_type::Bool), value(assign_what) {};
        template<>
        common(const std::wstring& assign_what) noexcept : type(uns::ex_ex::common_type::String), value(assign_what) {};
        template<>
        common(const double& assign_what) noexcept : type(uns::ex_ex::common_type::Number), value(static_cast<long double>(assign_what)) {};
        template<>
        common(const int& assign_what) noexcept : type(uns::ex_ex::common_type::Number), value(static_cast<long double>(assign_what)) {};

        template<typename assign_t>
        common& operator=(const assign_t& assign_what) {
            return (*this = common(assign_what));
        };

        template<typename return_t>
        return_t Cast() const {
            switch(type) {
                default:
                case uns::ex_ex::common_type::Null:
                    return return_t();
                case uns::ex_ex::common_type::Number: {
                    if(typeid(return_t) == typeid(long double))
                        return std::any_cast<long double>(value);
                    return static_cast<return_t>(std::any_cast<long double>(value));
                }
                case uns::ex_ex::common_type::Bool: {
                    if(typeid(return_t) == typeid(bool))
                        return std::any_cast<bool>(value);
                    return static_cast<return_t>(std::any_cast<bool>(value));
                }/*
                case uns::ex_ex::common_type::String: {
                    if(typeid(return_t) == typeid(std::wstring))
                        return std::any_cast<std::wstring>(value);
                    return uns::string_cast<return_t>(std::any_cast<std::wstring>(value));
                }*/;
            };
        };

        uns::ex_ex::common_type Type() const { return type; };
    };


    class node {
    protected:
        size_t index_in_tree = 0;
    public:
        size_t Index() const { return index_in_tree; };

        void SetIndex(size_t index) { index_in_tree = index; };

        virtual void SetNode(size_t subnode_idx_in_node, size_t subnode_idx_in_tree) = 0;

        virtual void AddNode(size_t subnode_idx_in_tree) = 0;

        virtual size_t NumOfNodes() const = 0;

        virtual size_t MinCashSize() const { return 0; };

        virtual size_t MinExtputsNumber() const { return 0; };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const = 0;

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const = 0;
    };


    class root : virtual public node {
    protected:
        size_t next_subnode = 0;
    public:
        virtual void SetNode(size_t subnode_idx_in_node, size_t subnode_idx_in_tree) override {
            next_subnode = subnode_idx_in_tree;
        };

        virtual void AddNode(size_t subnode_idx_in_tree) override { SetNode(0, subnode_idx_in_tree); };

        virtual size_t NumOfNodes() const override { return 1; };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return tree[next_subnode]->Execute(tree, extputs, cash);
        };

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return Execute(tree, extputs, cash);
        };

    };


    class sequence : virtual public node {
    protected:
        std::vector<size_t> subnodes;
    public:
        virtual void SetNode(size_t subnode_idx_in_node, size_t subnode_idx_in_tree) override {
            if(NumOfNodes() <= subnode_idx_in_node) subnodes.resize(subnode_idx_in_node);
            subnodes[subnode_idx_in_node] = subnode_idx_in_tree + 1;
        };

        virtual void AddNode(size_t subnode_idx_in_tree) override {
            subnodes.push_back(subnode_idx_in_tree + 1);
        };

        virtual size_t NumOfNodes() const override { return subnodes.size(); };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            for(auto subnode : subnodes)
                tree[subnode]->Execute(tree, extputs, cash);

            return common();
        };

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return value;
        };
    };


    class access : virtual public node {
    protected:
        size_t input_cell_idx = 0;
    public:
        virtual void SetNode(size_t subnode_idx_in_node, size_t cell_in_input) override { input_cell_idx = cell_in_input; };

        virtual void AddNode(size_t cell_in_input) override { SetNode(0, cell_in_input); };

        virtual size_t NumOfNodes() const override { return 1; };

        virtual size_t MinExtputsNumber() const { return input_cell_idx + 1; };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return extputs[input_cell_idx];
        };

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return extputs[input_cell_idx] = value;
        };
    };


    class variable : virtual public node {
    protected:
        size_t cash_cell_idx = 0;
    public:
        virtual void SetNode(size_t subnode_idx_in_node, size_t cell_in_cash) override { cash_cell_idx = cell_in_cash; };

        virtual void AddNode(size_t cell_in_cash) override { SetNode(0, cell_in_cash); };

        virtual size_t NumOfNodes() const override { return 1; };

        virtual size_t MinCashSize() const { return cash_cell_idx + 1; };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return cash[cash_cell_idx];
        };

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return cash[cash_cell_idx] = value;
        };
    };


    class assignment : virtual public node {
    protected:
        size_t assign_to = 0;
        bool filled1 = false;
        size_t assign_what = 0;
        bool filled2 = false;
    public:
        virtual void SetNode(size_t subnode_idx_in_node, size_t cell_in_tree) override {
            if(subnode_idx_in_node == 0) {
                assign_to = cell_in_tree + 1;
                filled1 = true;
            };
            if(subnode_idx_in_node == 1) {
                assign_what = cell_in_tree + 1;
                filled2 = true;
            };
        };

        virtual void AddNode(size_t cell_in_tree) override {
            if(filled1) {
                if(!filled2) {
                    assign_what = cell_in_tree + 1;
                    filled2 = true;
                };
            }
            else if(!filled2) {
                assign_to = cell_in_tree + 1;
                filled1 = true;
            }
            else
                filled1 = true;
        };

        virtual size_t NumOfNodes() const override { return (filled1 ? 1 : 0) + (filled2 ? 1 : 0); };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            auto value = tree[assign_what]->Execute(tree, extputs, cash);
            return tree[assign_to]->Set(value, tree, extputs, cash);
        };

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            Execute(tree, extputs, cash);
            return tree[assign_to]->Set(value, tree, extputs, cash);

        };
    };


    class plus : virtual public node {
    protected:
        size_t argument1 = 0;
        bool filled1 = false;
        size_t argument2 = 0;
        bool filled2 = false;
    public:
        virtual void SetNode(size_t subnode_idx_in_node, size_t cell_in_tree) override {
            if(subnode_idx_in_node == 0) {
                argument1 = cell_in_tree + 1;
                filled1 = true;
            };
            if(subnode_idx_in_node == 1) {
                argument2 = cell_in_tree + 1;
                filled2 = true;
            };
        };

        virtual void AddNode(size_t cell_in_tree) override {
            if(filled1) {
                if(!filled2) {
                    argument2 = cell_in_tree + 1;
                    filled2 = true;
                };
            }
            else if(!filled2) {
                argument1 = cell_in_tree + 1;
                filled1 = true;
            }
            else
                filled1 = true;
        };

        virtual size_t NumOfNodes() const override { return (filled1 ? 1 : 0) + (filled2 ? 1 : 0); };

        virtual common Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return common(tree[argument1]->Execute(tree, extputs, cash).Cast<long double>() + tree[argument2]->Execute(tree, extputs, cash).Cast<long double>());
        };

        virtual common Set(const common& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<common>& extputs, std::vector<common>& cash) const override {
            return Execute(tree, extputs, cash);
        };
    };


    class executable_tree {
    public:
        using cash_t = std::vector<common>;
        using expt_t = std::vector<common>;
    protected:
        std::vector<std::shared_ptr<node>> nodes;
        size_t num_of_extputs = 0;
        size_t num_of_vars = 0;
    public:
        executable_tree() {
            nodes.clear();
            nodes.emplace_back(std::make_shared<root>());
        };
        /*executable_tree(const executable_tree&) = delete;
        executable_tree& operator=(const executable_tree& copying_obj) = delete;
        executable_tree(executable_tree&& moving_obj) : nodes(std::move(moving_obj.nodes)), num_of_outer_vars(moving_obj.num_of_outer_vars) {};
        executable_tree& operator=(executable_tree&& moving_obj) {
            if(&moving_obj != this) {
                Delete();
                nodes = std::move(moving_obj.nodes);

                num_of_outer_vars = moving_obj.num_of_outer_vars;
            };
            return *this;
        };
        ~executable_tree() { Delete(); };*/
    protected:
        /*void Delete() {
            for(auto& node : nodes) {
                delete node;
            };
        };*/
    public:

        template<typename node_t>
        std::shared_ptr<node_t> NewNode(bool is_root = false) {
            auto new_node = std::make_shared<node_t>();
            nodes.push_back(new_node);
            nodes[nodes.size() - 1]->SetIndex(nodes.size() - 1);

            if(is_root) {
                nodes[0]->AddNode(nodes.size() - 1);
            };
            
            for(auto subnode : nodes) {
                num_of_extputs = fmax(num_of_extputs, subnode->MinExtputsNumber());
                num_of_vars = fmax(num_of_vars, subnode->MinCashSize());
            };

            return new_node;
        };

        void Execute(expt_t& exputs) {
            cash_t cash(num_of_vars);
            nodes[0]->Execute(nodes, exputs, cash);
        };

        void Execute(expt_t& exputs, cash_t& cash) {
            //cash_t cash(num_of_vars);
            nodes[0]->Execute(nodes, exputs, cash);
        };

        /*template<typename node_t>
        class node_ptr {
        public:
            using node_type = node_t;
        protected:
            node_t* ptr;
        public:
            node_ptr(node_t* set_node = nullptr) : ptr(set_node) {};

            template<typename subnode_t>
            void AddNode(subnode_t& subnode_ptr) {
                ptr->AddNode<subnode_t>(subnode_ptr.ptr);
            };

            void SetRef(size_t idx) { ptr->SetIndex(idx); };;

            size_t GetRef() const { return ptr->Index(); };
        };

        template<typename node_t>
        node_ptr<node_t> NewNode(bool is_outer_var, bool is_root = false) {
            auto new_node = new node_t;
            nodes.push_back(new_node);

            if(is_root) {
                reinterpret_cast<root*>(nodes[0])->AddNode<node_t>(new_node);
            };

            if(is_outer_var)
                num_of_outer_vars++;

            return node_ptr<node_t>(new_node);
        };*/

    };
};
