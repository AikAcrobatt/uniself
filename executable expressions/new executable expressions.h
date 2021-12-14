#pragma once

#pragma once

#include <iostream>
#include <vector>
#include <any>
#include <exception>
#include <stdexcept>
#include <array>

#include "uniself\algorithms.h"


namespace uns::ex_ex11 {

    class memory_allocator {
    public:
     /*аллокатор призван играть роль аналога —++ ссылки
     он не просто указывает на €чейку пам€ти, но и содержит информацию о типе*/
        size_t idx = 0;
    public:
        memory_allocator(size_t index = 0) : idx(index) {};
    };


    class memory_storage {
    public:

     /*ћоделирует динамическую пам€ть конкретного запуска вычисл€емого выражени€
     предполагаетс€, что хранит данные в виде коллекций разных типов, так что не требуетс€ посто€нно их перегон€ть из некоего базового в локально нужные
     должна быть возможность хранени€ аллокаторов на эту же пам€ть в качестве одного из типов
     должна быть возможность динамического выделени€ пам€ти, без повреждени€ уже существующей и(!) при об€зательном сохранении валидности аллокаторов на уже существующую пам€ть
     при копировании в €чейки с типом и свойством "аллокатор" должно быть возможно копировать туда и значени€ того же типа, но не аллокаторы*/

        std::vector<long double> str;

        //при копировании в €чейки с типом и свойством "аллокатор" должно быть возможно копировать туда и значени€ того же типа, но не аллокаторы
        //копирует данные в €чейку по адресу (в качестве присваиваемых данных может быть аллокатор)
        template<typename input_data_t>
        bool —opy(const memory_allocator& alloc, const input_data_t& value) {
            str[alloc.idx] = value;
            return true;
        };

        //при копировании в €чейки с типом и свойством "аллокатор" должно быть возможно копировать туда и значени€ того же типа, но не аллокаторы
        //копирует данные из €чейки в €чейку с учетом типизации
        bool CopyFrom(const memory_allocator& to, const memory_allocator& from) {
            str[to.idx] = str[from.idx];
            return true;
        };

        //возвращает значение, содержащеес€ в €чейке по адресу
        template<typename output_data_t>
        bool Get(const memory_allocator& alloc, output_data_t& value) {
            value = str[alloc.idx];
            return true;
        };
        template<typename output_data_t>
        output_data_t& Get(const memory_allocator& alloc) { return str[alloc.idx]; };

        //выдел€ет пам€ть и возвращает аллокатор на нее
        template<typename input_data_t>
        memory_allocator Set() { return memory_allocator(); };
        //выдел€ет пам€ть и помещает в нее указанное значчение и возвращает аллокатор
        template<typename input_data_t>
        memory_allocator Set(const input_data_t& value = input_data_t()) {
            auto alloc = Set<input_data_t>();
            —opy<input_data_t>(alloc, value);
            return alloc;
        };

        //освобождает указанную €чейку пам€ти
        bool Pop(const memory_allocator& alloc) { return true; };
    };


    enum node_type {
        root,
        assignment,
        plus,
        access
    };


    class node {
    public:
        using param_t = memory_storage;
        using type_execute = void(*)(node&, param_t&);
    protected:
        std::vector<node> subnodes;
        node_type type = node_type();
        memory_allocator alloc;
        type_execute execute;

        static type_execute execute_generator(node_type n_type);
    public:
        node(node_type set_type = node_type::root) : type(set_type) {
            SetType(type);
        };

        void SetType(node_type set_type = node_type::root) {
            execute = execute_generator(type);
        };

        node_type GetType() const {
            return type;
        };

        node* AddNode(node_type set_type) {
            return &(subnodes.emplace_back(node(set_type)));
        };
        void AddNode(node* set_node) {
            subnodes.emplace_back(*set_node);
        };

        size_t NodesTotal() const { return subnodes.size(); };

        node& At(size_t idx) {
            return subnodes[idx];
        };

        void SetVar(const memory_allocator& variable_alloc) {
            alloc = variable_alloc;
        };

        memory_allocator GetVar() {
            return alloc;
        };

        void Execute(param_t& params) {
            return execute(*this, params);
        };

    };


    void root_exec(node& this_node, typename node::param_t& params) {
        this_node.At(0).Execute(params);
    };

    void assignment_exec(node& this_node, typename node::param_t& params) {
        this_node.At(1).Execute(params);
        params.CopyFrom(this_node.GetVar(), this_node.At(1).GetVar());
        this_node.At(0).Execute(params);
        params.CopyFrom(this_node.At(0).GetVar(), this_node.GetVar());
    };

    void long_double_plus_exec(node& this_node, typename node::param_t& params) {
        this_node.At(1).Execute(params);
        this_node.At(0).Execute(params);
        params.—opy(this_node.GetVar(), params.Get<long double>(this_node.At(0).GetVar()) + params.Get<long double>(this_node.At(1).GetVar()));
    };

    void access_exec(node& this_node, typename node::param_t& params) {};

    typename node::type_execute node::execute_generator(node_type n_type) {
        switch(n_type) {
            default:
            case root: {
                return root_exec;
            }
            case assignment: {
                return assignment_exec;
            }
            case plus: {
                return long_double_plus_exec;
            }
            case access: {
                return access_exec;
            };
        };
    };
};

namespace uns::ex_ex10 {

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

        virtual return_t& Exec(std::vector<return_t>&) = 0;

        virtual bool IsVariable() const { return false; };
    };


    template<typename type_t>
    class root : public node<type_t, 1> {
    public:
        using base_t = node<type_t, 1>;

        virtual type_t& Exec(std::vector<type_t>& params) override {
            auto res = base_t::subnodes[0]->Exec(params);
            return res;
        };
    };


    template<typename type_t>
    class variable : public node<type_t, 1> {
    protected:
        size_t value;
    public:
        void SetIdx(size_t idx) { value = idx; };

        virtual type_t& Exec(std::vector<type_t>& params) override {
            return params[value];
        };

        virtual bool IsVariable() const { return true; };
    };


    template<typename return_t>
    class assignment : public node<return_t, 2> {
    public:
        using base_t = node<return_t, 2>;

        virtual return_t& Exec(std::vector<return_t>& params) override { return base_t::subnodes[0]->Exec(params) = base_t::subnodes[1]->Exec(params); };
    };


    template<typename return_t>
    class plus : public node<return_t, 2> {
    public:
        using base_t = node<return_t, 2>;

        virtual return_t& Exec(std::vector<return_t>& params) override {
            return_t value = base_t::subnodes[0]->Exec(params) + base_t::subnodes[1]->Exec(params);
            return value;
        };
    };


    template<typename type_t>
    class executable_tree {
    protected:
        std::vector<void*> nodes;
        std::vector<size_t> variables;
    public:
        executable_tree() {
            nodes.clear();
            NewNode<root<type_t>>();
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
                reinterpret_cast<root<type_t>*>(nodes[0])->SetNode<node_t>(0, new_node);
            };

            if(new_node->IsVariable()) {
                variables.push_back(nodes.size() - 1);
            };

            return new_node;
        };

        void Execute(std::vector<type_t>& params) { if(nodes.size() > 0) reinterpret_cast<root<type_t>*>(nodes[0])->Exec(params); };
    };
};

namespace uns::ex_ex9 {

    using val_type = long double;

    enum node_type {
        root,
        assignment,
        plus,
        access
    };


    class node {
    public:
        using param_t = std::vector<val_type>;
        typedef val_type(*type_execute)(node&, node&, node&, param_t&);
        typedef val_type(*type_set_val)(node&, val_type&, param_t&);
    protected:
        std::vector<node> subnodes;
        node_type type = node_type();
        size_t var_idx = 0;
        type_execute execute;
        type_set_val set;

        static type_execute execute_generator(node_type n_type);

        static type_set_val set_val_generator(node_type n_type);
    public:
        node(node_type set_type = node_type::root) : type(set_type) {
            execute = execute_generator(type);
            set = set_val_generator(type);
        };

        node* AddNode(node_type set_type) {
            return &(subnodes.emplace_back(node(set_type)));
        };
        void AddNode(node* set_node) {
            subnodes.emplace_back(*set_node);
        };

        node& At(size_t idx) {
            return subnodes[idx];
        };

        void SetVarIdx(size_t idx) {
            var_idx = idx;
        };

        size_t VarIdx() {
            return var_idx;
        };

        val_type Execute(param_t& params) {
            /*switch(type) {
            default:
            case root: {
                return subnodes[0].Execute(params);
            }
            case assignment: {
                return subnodes[0].Set(subnodes[1].Execute(params), params);
            }
            case plus: {
                return subnodes[0].Execute(params) + subnodes[1].Execute(params);
            }
            case access: {
                return params[var_idx];
            };
            };*/
            return execute(*this, subnodes[0], subnodes[1], params);
        };

        val_type Set(val_type value, param_t& params) {
            /*switch(type) {
            default:
            case root: {
                return Execute(params);
            }
            case assignment: {
                Execute(params);
                return subnodes[0].Set(value, params);
            }
            case plus: {
                return Execute(params);
            }
            case access: {
                return params[var_idx] = value;
            };
            };*/
            return set(*this, value, params);
        };

    };


    val_type root_exec(node& this_node, node& n1, node& n2, node::param_t& params) {
        return n1.Execute(params);
    };

    val_type assignment_exec(node& this_node, node& n1, node& n2, node::param_t& params) {
        return n1.Set(n2.Execute(params), params);
    };

    val_type plus_exec(node& this_node, node& n1, node& n2, node::param_t& params) {
        return n1.Execute(params) + n2.Execute(params);
    };

    val_type access_exec(node& this_node, node& n1, node& n2, node::param_t& params) {
        return params[this_node.VarIdx()];
    };

    node::type_execute node::execute_generator(node_type n_type) {
        switch(n_type) {
            default:
            case root:
            {
                return root_exec;
            }
            case assignment:
            {
                return assignment_exec;
            }
            case plus:
            {
                return plus_exec;
            }
            case access:
            {
                return access_exec;
            };
        };
    };


    val_type root_set(node& this_node, val_type& value, node::param_t& params) {
        return this_node.Execute(params);
    };

    val_type assignment_set(node& this_node, val_type& value, node::param_t& params) {
        this_node.Execute(params);
        return this_node.At(0).Set(value, params);
    };

    val_type plus_set(node& this_node, val_type& value, node::param_t& params) {
        return this_node.Execute(params);
    };

    val_type access_set(node& this_node, val_type& value, node::param_t& params) {
        return params[this_node.VarIdx()] = value;
    };

    node::type_set_val node::set_val_generator(node_type n_type) {
        switch(n_type) {
            default:
            case root:
            {
                return root_set;
            }
            case assignment:
            {
                return assignment_set;
            }
            case plus:
            {
                return plus_set;
            }
            case access:
            {
                return access_set;
            };
        };
    };

};

namespace uns::ex_ex8 {

/*
    using val_t = long double;

    enum op_t {
        root,
        assignment,
        plus,
        access,
        variable
    };

    class node {
    protected:
        size_t index_in_tree = 0;
        op_t type;
        size_t arg1 = 0;
        size_t arg2 = 0;
    public:
        node(op_t oper_type = op_t::root) : type(oper_type) {};

        size_t Index() const { return index_in_tree; };

        void SetIndex(size_t index) { index_in_tree = index; };

        void SetType(op_t oper_type) { type = oper_type; };

        virtual void SetNode(size_t subnode_idx_in_node, size_t subnode_idx_in_tree) {
            if(subnode_idx_in_node == 0)
                arg1 = subnode_idx_in_tree + 1;
            else if(subnode_idx_in_node == 1)
                arg2 = subnode_idx_in_tree + 1;
        };

        virtual size_t NumOfNodes() const { return 2; };

        virtual size_t MinCashSize() const { return 0; };

        virtual size_t MinExtputsNumber() const { return 0; };

        virtual val_t Execute(const std::vector<node>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const {
            switch(type) {
                default:
                case op_t::root: {
                    return tree[arg1].Execute(tree, extputs, cash);
                }
                case op_t::assignment: {
                    return tree[arg1].Set(tree[arg2].Execute(tree, extputs, cash), tree, extputs, cash);
                }
                case op_t::plus: {
                    return tree[arg1].Execute(tree, extputs, cash) + tree[arg2].Execute(tree, extputs, cash);
                }
                case op_t::access: {
                    return extputs[arg1];
                }
                case op_t::variable: {
                    return cash[arg1];
                };
            };
        };

        virtual val_t Set(const val_t& value, const std::vector<node>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const {
            switch(type) {
                default: {
                    return Execute(tree, extputs, cash);
                }
                case op_t::access: {
                    return extputs[arg1] = value;
                }
                case op_t::variable: {
                    return cash[arg1] = value;
                };
            };
        };
    };


    class executable_tree {
    public:
        using cash_t = std::vector<val_t>;
        using expt_t = std::vector<val_t>;
    protected:
        std::vector<node> nodes;
        size_t num_of_extputs = 0;
        size_t num_of_vars = 0;
    public:
        executable_tree() {
            nodes.clear();
            nodes.emplace_back(node());
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
        ~executable_tree() { Delete(); };
    protected:
        /*void Delete() {
            for(auto& node : nodes) {
                delete node;
            };
        };
    public:

        node* NewNode(bool is_root = false) {
            auto new_node = node();
            nodes.push_back(new_node);
            nodes[nodes.size() - 1].SetIndex(nodes.size() - 1);

            if(is_root) {
                nodes[0].SetNode(0, nodes.size() - 1);
            };

            for(auto subnode : nodes) {
                num_of_extputs = fmax(num_of_extputs, subnode.MinExtputsNumber());
                num_of_vars = fmax(num_of_vars, subnode.MinCashSize());
            };

            return &new_node;
        };

        void Execute(expt_t& exputs) {
            cash_t cash(num_of_vars);
            nodes[0].Execute(nodes, exputs, cash);
        };

        void Execute(expt_t& exputs, cash_t& cash) {
            //cash_t cash(num_of_vars);
            nodes[0].Execute(nodes, exputs, cash);
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
        };

    };*/
};

namespace uns::ex_ex7 {



    using val_t = long double;


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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const = 0;

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const = 0;
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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            return tree[next_subnode]->Execute(tree, extputs, cash);
        };

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            for(auto subnode : subnodes)
                tree[subnode]->Execute(tree, extputs, cash);

            return val_t();
        };

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            return extputs[input_cell_idx];
        };

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            return cash[cash_cell_idx];
        };

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            //auto value  = tree[assign_what]->Execute(tree, extputs, cash);
            //return tree[assign_to]->Set(value, tree, extputs, cash);
            return tree[assign_to]->Set(tree[assign_what]->Execute(tree, extputs, cash), tree, extputs, cash);
        };

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
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

        virtual val_t Execute(const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            return (tree[argument1]->Execute(tree, extputs, cash) + tree[argument2]->Execute(tree, extputs, cash));
        };

        virtual val_t Set(const val_t& value, const std::vector<std::shared_ptr<node>>& tree, std::vector<val_t>& extputs, std::vector<val_t>& cash) const override {
            return Execute(tree, extputs, cash);
        };
    };


    class executable_tree {
    public:
        using cash_t = std::vector<val_t>;
        using expt_t = std::vector<val_t>;
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

namespace uns::ex_ex6 {

    UNS_ENUM_DECLARATOR(common_type, int,
        Null,
        Number,
        Bool,
        String
    );

    class common {
    protected:
        std::any value;
        common_type type;
    public:
        explicit common(const common_type& set_type = common_type::Null) noexcept : type(set_type) {
            switch(type) {
                default:
                case common_type::Null:
                    return;
                case common_type::Number: {
                    value = static_cast<long double>(0.0);
                    return;
                }
                case common_type::Bool: {
                    value = false;
                    return;
                }
                case common_type::String: {
                    value = std::wstring();
                    return;
                };
            };
        };

        template<typename assign_t>
        common(const assign_t& assign_what) noexcept : type(common_type::Null) {};
        template<>
        common(const long double& assign_what) noexcept : type(common_type::Number), value(assign_what) {};
        template<>
        common(const bool& assign_what) noexcept : type(common_type::Bool), value(assign_what) {};
        template<>
        common(const std::wstring& assign_what) noexcept : type(common_type::String), value(assign_what) {};
        template<>
        common(const double& assign_what) noexcept : type(common_type::Number), value(static_cast<long double>(assign_what)) {};
        template<>
        common(const int& assign_what) noexcept : type(common_type::Number), value(static_cast<long double>(assign_what)) {};

        template<typename assign_t>
        common& operator=(const assign_t& assign_what) {
            return (*this = common(assign_what));
        };

        template<typename return_t>
        return_t Cast() const {
            switch(type) {
                default:
                case common_type::Null:
                    return return_t();
                case common_type::Number: {
                    if(typeid(return_t) == typeid(long double))
                        return std::any_cast<long double>(value);
                    return static_cast<return_t>(std::any_cast<long double>(value));
                }
                case common_type::Bool: {
                    if(typeid(return_t) == typeid(bool))
                        return std::any_cast<bool>(value);
                    return static_cast<return_t>(std::any_cast<bool>(value));
                }/*
                case common_type::String: {
                    if(typeid(return_t) == typeid(std::wstring))
                        return std::any_cast<std::wstring>(value);
                    return uns::string_cast<return_t>(std::any_cast<std::wstring>(value));
                }*/;
            };
        };

        common_type Type() const { return type; };
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

namespace uns::ex_ex3 {

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

        virtual long double& Exec() override { 
            auto res = base_t::subnodes[0]->Exec();
            return res;
        };
    };


    template<typename type_t>
    class variable : public node<type_t, 1> {
    protected:
        type_t value;
    public:

        virtual type_t& Exec() override {
            return value;
        };

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
            return_t value = base_t::subnodes[0]->Exec();
            value += base_t::subnodes[1]->Exec();
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