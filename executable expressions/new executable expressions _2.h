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

        virtual return_t& Exec(std::vector<node*>&) = 0;
    };


    template<typename type_t>
    class variable : public node<type_t, 0> {
    protected:
        type_t value;
    public:

        virtual type_t& Exec(std::vector<node*>& var_cash) override { return value; };
    };


    class root : public node<int, 1> {
    public:
        using base_t = node<int, 1>;

        virtual int& Exec(std::vector<node*>& var_cash) override { base_t::subnodes[0]->Exec(var_cash); int res = 0; return res; };

        void Start(void* cash) {
            Exec(*(reinterpret_cast<std::vector<node*>*>(cash)));
        };

        void* CreateCash(size_t cash_size = 1) {
            auto res = new std::vector<node*>(cash_size);
            for(auto& var_ptr : *res)
                var_ptr = nullptr;
            return res;
        };

        template<typename val_type_t>
        void SetCash(void* cash, size_t idx, val_type_t& value) {
            auto var_ptr = (*(reinterpret_cast<std::vector<node*>*>(cash)))[idx] = new variable<val_type_t>;
            var_ptr->Exec(reinterpret_cast<std::vector<node*>*>(cash)) = value;
        };

        template<typename val_type_t>
        static val_type_t& GetCash(void* cash, size_t idx) {
            if((*reinterpret_cast<std::vector<node*>*>(cash))[idx] != nullptr)
                return (*reinterpret_cast<std::vector<node*>*>(cash))[idx]->Exec(reinterpret_cast<std::vector<node*>*>(cash));
            else return val_type_t();
        };

        void FreeCash(void* cash) {
            for(auto& one_node_ptr : *(reinterpret_cast<std::vector<node*>*>(cash)))
                if(one_node_ptr != nullptr)
                    delete one_node_ptr;
            delete cash;
        };
    };


    template<typename type_t>
    class access : public node<type_t, 1> {
    protected:
        size_t ref;
    public:
        void SetRef(size_t idx) { ref = idx; };

        size_t GetRef() const { return ref; };

        virtual type_t& Exec(std::vector<node*>& var_cash) override { return var_cash[ref]->Exec(var_cash); };
    };


    template<typename return_t>
    class assignment : public node<return_t, 2> {
    public:
        using base_t = node<return_t, 2>;

        virtual return_t& Exec(std::vector<node*>& var_cash) override { return base_t::subnodes[0]->Exec(var_cash) = base_t::subnodes[1]->Exec(var_cash); };
    };


    template<typename return_t>
    class plus : public node<return_t, 2> {
    public:
        using base_t = node<return_t, 2>;

        virtual return_t& Exec(std::vector<node*>& var_cash) override {
            return_t value = base_t::subnodes[0]->Exec(var_cash) + base_t::subnodes[1]->Exec(var_cash);
            return value;
        };
    };


    class executable_tree {
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
            new_node->SetNodeIndex(nodes.size() - 1);

            if(is_root) {
                reinterpret_cast<root*>(nodes[0])->SetNode<node_t>(0, new_node);
            };

            if(is_outer_var)
                num_of_outer_vars++;

            return new_node;
        };

        void* CreateCash() {
            return reinterpret_cast<root*>(nodes[0])->CreateCash(num_of_outer_vars);
        };

        template<typename val_type_t>
        void SetOuterValue(void* cash, size_t idx, const val_type_t& value) {
            reinterpret_cast<root*>(nodes[0])->SetCash<val_type_t>(cash, idx, value);
        };

        template<typename val_type_t>
        val_type_t& GetOuterValue(void* cash, size_t idx) {
            //return reinterpret_cast<root*>(nodes[0])->GetCash<val_type_t>(cash, idx);
            return root::GetCash<val_type_t>(cash, idx);

        };

        void Execute(void* cash) {
            reinterpret_cast<root*>(nodes[0])->Start(cash);
        };

        void FreeCash(void* cash) {
            reinterpret_cast<root*>(nodes[0])->FreeCash(cash);
        };

    };
};
