#pragma once

#pragma once

#include <iostream>
#include <vector>
#include <any>
#include <exception>
#include <stdexcept>

#include "uniself\algorithms.h"

namespace uns::syntax {

    template<typename return_t>
    class node {
    public:
        using return_type = return_t;
        using cash_t = std::vector<std::any>;
    protected:
        std::vector<node*> subnodes;
    public:

        template<typename node_t>
        void AddNode(node_t* node_ptr) {
            subnodes.push_back(reinterpret_cast<node*>(node_ptr));
        };

        virtual return_t* Exec(cash_t&) = 0;

        virtual void Verify(cash_t&) const = 0;
    };


    class root : public node<void> {
    public:
        using base_t = node<void>;

        virtual void* Exec(base_t::cash_t& var_cash) override { base_t::subnodes[0]->Exec(var_cash); int res = 0; return &res; };

        virtual void Verify(base_t::cash_t& var_cash) const override {
            if(!(base_t::subnodes.size() >= 1))
                throw std::out_of_range("Invalid number of subnodes"); 
            base_t::subnodes[0]->Verify(var_cash);
        };
    };


    template<typename type_t>
    class access : public node<type_t> {
    public:
        using base_t = node<type_t>;
    protected:
        size_t ref;
    public:
        void SetRef(size_t idx) { ref = idx; };

        size_t GetRef() const { return ref; };

        virtual type_t* Exec(base_t::cash_t& var_cash) override { 
            return std::any_cast<type_t>(&var_cash[ref]);
        };

        virtual void Verify(base_t::cash_t& var_cash) const override {
            if(nullptr == std::any_cast<type_t>(&var_cash[ref]))
                throw std::invalid_argument("Type mismatch of argument #" + uns::string_cast<std::string>(ref));
        };
    };


    template<typename return_t>
    class assignment : public node<return_t> {
    public:
        using base_t = node<return_t>;

        virtual return_t* Exec(base_t::cash_t& var_cash) override {
            return &(*(base_t::subnodes[0]->Exec(var_cash)) = *(base_t::subnodes[1]->Exec(var_cash)));
        };

        virtual void Verify(base_t::cash_t& var_cash) const override {
            if(!(base_t::subnodes.size() >= 2))
                throw std::out_of_range("Invalid number of subnodes");
            base_t::subnodes[0]->Verify(var_cash);
            base_t::subnodes[1]->Verify(var_cash);
        };
    };


    template<typename return_t>
    class plus : public node<return_t> {
    public:
        using base_t = node<return_t>;

        virtual return_t* Exec(base_t::cash_t& var_cash) override {
            auto res = *(base_t::subnodes[0]->Exec(var_cash)) + *(base_t::subnodes[1]->Exec(var_cash));
            return &res;
        };

        virtual void Verify(base_t::cash_t& var_cash) const override {
            if(!(base_t::subnodes.size() >= 2))
                throw std::out_of_range("Invalid number of subnodes");
            base_t::subnodes[0]->Verify(var_cash);
            base_t::subnodes[1]->Verify(var_cash);
        };
    };


    template<typename return_t>
    class static_cast_op : public node<return_t> {
    public:
        using base_t = node<return_t>;

        virtual return_t* Exec(base_t::cash_t& var_cash) override {
            auto in = *(base_t::subnodes[0]->Exec(var_cash));
            auto res = static_cast<return_t>(in);
            return &res;
        };

        virtual void Verify(base_t::cash_t& var_cash) const override {
            if(!(base_t::subnodes.size() >= 1))
                throw std::out_of_range("Invalid number of subnodes");
            base_t::subnodes[0]->Verify(var_cash);
        };
    };


    class executable_tree {
    public:
        using cash_t = std::vector<std::any>;
    protected:
        std::vector<void*> nodes;
        size_t num_of_outer_vars = 0;
    public:
        executable_tree() : num_of_outer_vars(0) {
            nodes.clear();
            nodes.push_back(new root);
        };
        executable_tree(const executable_tree&) = delete;
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
        ~executable_tree() { Delete(); };
    protected:
        void Delete() {
            for(auto& node : nodes) {
                delete node;
            };
        };
    public:

        template<typename node_t>
        node_t* NewNode(bool is_outer_var, bool is_root = false) {
            auto new_node = new node_t;
            nodes.push_back(new_node);

            if(is_root) {
                reinterpret_cast<root*>(nodes[0])->AddNode<node_t>(new_node);
            };

            if(is_outer_var)
                num_of_outer_vars++;

            return new_node;
        };

        void Execute(cash_t& cash) {
            reinterpret_cast<root*>(nodes[0])->Exec(cash);
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

            void SetRef(size_t idx) { ptr->SetRef(idx); };;

            size_t GetRef() const { return ptr->GetRef(); };

            void Verify(cash_t& var_cash) {
                ptr->Verify(var_cash);
            };
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
