#pragma once

#pragma once

#include <iostream>
#include <array>
#include <vector>


namespace uns::syntax {

    template<typename return_t, size_t args_number>
    class node {
    public:
        using return_type = return_t;
    protected:
        std::array<node*, args_number> subnodes;
    public:

        template<typename node_t>
        void SetNode(size_t idx, node_t* node_ptr) {
            subnodes[idx] = reinterpret_cast<node*>(node_ptr);
        };

        virtual return_t& Exec() = 0;

        virtual bool IsVariable() const { return false; };
    };


    class root : public node<long double, 1> {
    public:
        using base_t = node<long double, 1>;

        virtual long double& Exec() override { base_t::subnodes[0]->Exec(); long double res = 0; return res; };
    };


    template<typename type_t>
    class variable : public node<type_t, 1> {
    protected:
        type_t value;
    public:

        virtual type_t& Exec() override { return value; };

        virtual bool IsVariable() const { return true; };
    };


    template<typename return_t>
    class assignment : public node<return_t, 2> {
    public:
        using base_t = node<return_t, 2>;

        virtual return_t& Exec() override { return base_t::subnodes[0]->Exec() = base_t::subnodes[1]->Exec(); };
    };


    template<typename return_t>
    class plus : public node<return_t, 2> {
    public:
        using base_t = node<return_t, 2>;

        virtual return_t& Exec() override {
            return_t value = base_t::subnodes[0]->Exec() + base_t::subnodes[1]->Exec();
            return value;
        };
    };


    class executable_tree {
    protected:
        std::vector<void*> nodes;
        std::vector<size_t> variables;
    public:
        executable_tree() {
            nodes.clear();
            NewNode<root>();
        };
        executable_tree(const executable_tree&) = delete;
        executable_tree& operator=(const executable_tree& copying_obj) = delete;
        executable_tree(executable_tree&& moving_obj) : nodes(std::move(moving_obj.nodes)), variables(moving_obj.variables) {};
        executable_tree& operator=(executable_tree&& moving_obj) {
            if(&moving_obj != this) {
                Delete();
                nodes = std::move(moving_obj.nodes);
                variables = std::move(moving_obj.variables);
            };
            return *this;
        };
        ~executable_tree() { Delete(); };
    protected:
        void Delete() {
            for(auto& node : nodes) {
                delete node;
            };
        };
    public:

        size_t Variables() const { return variables.size(); };

        template<typename value_t, typename node_t = variable<value_t>>
        auto& Variable(size_t idx) {
            return reinterpret_cast<node_t*>(nodes[variables[idx]])->Exec();
        };

        template<typename node_t>
        node_t* NewNode(bool is_root = false) {
            auto new_node = new node_t;
            nodes.push_back(new_node);

            if(is_root) {
                reinterpret_cast<root*>(nodes[0])->SetNode<node_t>(0, new_node);
            };

            if(new_node->IsVariable()) {
                variables.push_back(nodes.size() - 1);
            };

            return new_node;
        };

        void Execute() { if(nodes.size() > 0) reinterpret_cast<root*>(nodes[0])->Exec(); };
    };
};
