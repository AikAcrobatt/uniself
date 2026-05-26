#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <concepts>
#include <unordered_map>

#include "uniself/concepts.hpp"

#ifndef UNS_LIB_NEURONETWORKS
#define UNS_LIB_NEURONETWORKS "neuronetworks.hpp"


#define UNS_DEV_EXCEPTION_MSG ::std::string{ __FUNCTION__ } + "[" + ::std::to_string(__LINE__) + "]"


namespace uns::nn {


    //an address of neuron in every neuronetwork
    class address {
    public:
        int layer = 0;
        int index = 0;

        inline bool operator==(const ::uns::nn::address& Address) const {
            return layer == Address.layer && index == Address.index;
        };
        inline bool operator!=(const ::uns::nn::address& Address) const {
            return !(*this == Address);
        };

        inline ::std::size_t capacity() const { return sizeof(*this); };

        class hash {
        protected:
            ::std::hash<long long int> m_subhash;
        public:
            inline ::std::size_t operator()(const ::uns::nn::address& Address) const {
                return m_subhash((static_cast<long long int>(Address.layer) << sizeof(int) * 8) + static_cast<long long int>(Address.index));
            };
        };
    };

};

namespace uns::nn::traitset {

    template<typename signal_t, typename weight_t, typename params_t>
    class signal {
    public:
        using signal_type = signal_t;
        using weight_type = weight_t;
        using params_type = params_t;
    };


    template<typename signal_traitset_t, typename iterator_t, typename input_allocator_t>
        requires ::std::derived_from<
            signal_traitset_t
            , ::uns::nn::traitset::signal<
                typename signal_traitset_t::signal_type
                , typename signal_traitset_t::weight_type
                , typename signal_traitset_t::params_type
            >
        >
        && ::std::derived_from<input_allocator_t, ::std::allocator<typename input_allocator_t::value_type>>
        && ::std::same_as<typename input_allocator_t::value_type::neuron_traitset::signal_traitset, signal_traitset_t>
        && ::uns::legacy_iterator<iterator_t, ::uns::nn::address>
    class input {
    public:
        using signal_traitset = signal_traitset_t;
        using iterator_type = iterator_t;
        using input_neuron_type = input_allocator_t::value_type;
        using input_allocator_type = input_allocator_t;
    };


    template<typename signal_traitset_t, typename description_neuron_t, typename activator_caster_t, typename collector_caster_t>
        requires ::std::derived_from<
            signal_traitset_t
            , ::uns::nn::traitset::signal<
                typename signal_traitset_t::signal_type
                , typename signal_traitset_t::weight_type
                , typename signal_traitset_t::params_type
            >
        >
        && ::std::same_as<typename description_neuron_t::signal_traitset, signal_traitset_t>
        && ::std::same_as<typename activator_caster_t::signal_traitset, signal_traitset_t>
        && ::std::same_as<typename collector_caster_t::signal_traitset, signal_traitset_t>
    class neuron {
    public:
        using signal_traitset = signal_traitset_t;
        using description_type = description_neuron_t;
        using activator_caster_type = activator_caster_t;
        using collector_caster_type = collector_caster_t;
    };


    template<typename neuron_t, typename description_network_t, typename input_data_object_t>
        requires ::std::derived_from<
            typename neuron_t::neuron_traitset
            , ::uns::nn::traitset::neuron<
                typename neuron_t::neuron_traitset::signal_traitset
                , typename neuron_t::neuron_traitset::description_type
                , typename neuron_t::neuron_traitset::activator_caster_type
                , typename neuron_t::neuron_traitset::collector_caster_type
            >
        >
        && ::std::derived_from<
            typename input_data_object_t::input_traitset
            , ::uns::nn::traitset::input<
                typename input_data_object_t::input_traitset::signal_traitset
                , typename input_data_object_t::input_traitset::iterator_type
                , typename input_data_object_t::input_traitset::input_allocator_type
            >
        >
        && ::std::same_as<
            typename neuron_t::neuron_traitset::description_type
            , typename description_network_t::neuron_description_type
        >
    class network {
    public:
        using neuron_type = neuron_t;
        using description_type = description_network_t;
        using input_data_object_type = input_data_object_t;
    };
};


namespace uns::nn {


    template<typename description_t>
    concept is_description = requires (description_t Description) {
        Description.is_description();
    };


    namespace description {

        template<typename signal_traitset_t>
            requires ::std::derived_from<
                signal_traitset_t
                , ::uns::nn::traitset::signal<
                    typename signal_traitset_t::signal_type
                    , typename signal_traitset_t::weight_type
                    , typename signal_traitset_t::params_type
                >
            >
        class collector {
        public:
            using hashsum_type = uint64_t;
            using signal_type = typename signal_traitset_t::signal_type;
        public:
            hashsum_type hashsum = 0;
            signal_type value = 0;
        public:
            virtual void is_description() const noexcept {};
        };


        template<typename signal_traitset_t>
            requires ::std::derived_from<
                signal_traitset_t
                , ::uns::nn::traitset::signal<
                    typename signal_traitset_t::signal_type
                    , typename signal_traitset_t::weight_type
                    , typename signal_traitset_t::params_type
                >
            >
        class activator {
        public:
            using hashsum_type = uint64_t;
            using signal_type = typename signal_traitset_t::signal_type;
        public:
            hashsum_type hashsum = 0;
            signal_type value = 0;
        public:
            virtual void is_description() const noexcept {};
        };


        template<typename signal_traitset_t>
            requires ::std::derived_from<
                signal_traitset_t
                , ::uns::nn::traitset::signal<
                    typename signal_traitset_t::signal_type
                    , typename signal_traitset_t::weight_type
                    , typename signal_traitset_t::params_type
                >
            >
        class neuron {
        public:
            using signal_traitset = signal_traitset_t;
        public:
            ::uns::nn::description::activator<signal_traitset> activator;
            signal_traitset::signal_type r = typename signal_traitset::signal_type{ 0 };
            ::uns::nn::description::collector<signal_traitset> collector;
            signal_traitset::signal_type c = typename signal_traitset::signal_type{ 0 };
            ::std::vector<::std::pair<::uns::nn::address, typename signal_traitset::weight_type>> links;
            ::std::vector<typename signal_traitset::params_type> params;
        public:
            neuron() {};
            neuron(const ::uns::nn::description::neuron<signal_traitset>& Neuron) :
                activator(Neuron.activator),
                collector(Neuron.collector),
                links(Neuron.links),
                params(Neuron.params),
                r(Neuron.r),
                c(Neuron.c) {};
            ::uns::nn::description::neuron<signal_traitset>& operator=(const ::uns::nn::description::neuron<signal_traitset>& Neuron) {
                if(this == &Neuron) return *this;

                activator = Neuron.activator;
                collector = Neuron.collector;
                links = Neuron.links;
                params = Neuron.params;
                r = Neuron.r;
                c = Neuron.c;

                return *this;
            };
            neuron(::uns::nn::description::neuron<signal_traitset>&& Neuron) :
                activator(::std::move(Neuron.activator)),
                collector(::std::move(Neuron.collector)),
                links(::std::move(Neuron.links)),
                params(::std::move(Neuron.params)),
                r(::std::move(Neuron.r)),
                c(::std::move(Neuron.c)) {};
            ::uns::nn::description::neuron<signal_traitset>& operator=(::uns::nn::description::neuron<signal_traitset>&& Neuron) {
                if(this == &Neuron) return *this;

                activator = ::std::move(Neuron.activator);
                collector = ::std::move(Neuron.collector);
                links = ::std::move(Neuron.links);
                params = ::std::move(Neuron.params);
                r = ::std::move(Neuron.r);
                c = ::std::move(Neuron.c);

                return *this;
            };
            virtual ~neuron() {};
        public:
            virtual void is_description() const noexcept {};
        };


        template<::uns::nn::is_description neuron_description_t>
        class network {
        public:
            using neuron_description_type = neuron_description_t;
        public:
            ::std::vector<::std::vector<neuron_description_type>> layers;
            ::std::vector<::uns::nn::address> outputs;
        public:
            network() {};
            network(const ::uns::nn::description::network<neuron_description_type>& Network) :
                layers(Network.layers),
                outputs(Network.outputs)
            {};
            ::uns::nn::description::network<neuron_description_type>& operator=(const ::uns::nn::description::network<neuron_description_type>& Network) {
                if(this == &Network) return *this;

                layers = Network.layers;
                outputs = Network.outputs;

                return *this;
            };
            network(::uns::nn::description::network<neuron_description_type>&& Network) :
                layers(::std::move(Network.layers)),
                outputs(::std::move(Network.outputs))
            {};
            ::uns::nn::description::network<neuron_description_type>& operator=(::uns::nn::description::network<neuron_description_type>&& Network) {
                if(this == &Network) return *this;

                layers = ::std::move(Network.layers);
                outputs = ::std::move(Network.outputs);

                return *this;
            };
            virtual ~network() {};
        public:
            virtual void is_description() const noexcept {};
        };

    };


    namespace general {
        //this namespace is intended to contain prototypes and interfaces


        //the most common neuron-like class, beeing used for linking neurons purposes mostly
        template<typename signal_traitset_t>
            requires ::std::derived_from<
                signal_traitset_t
                , ::uns::nn::traitset::signal<
                    typename signal_traitset_t::signal_type
                    , typename signal_traitset_t::weight_type
                    , typename signal_traitset_t::params_type
                >
            >
        class neuron_view {
        public:
            using signal_traitset = signal_traitset_t;
        protected:
            ::uns::nn::address m_address;
            signal_traitset::signal_type m_dropout = typename signal_traitset::signal_type{ 0 };
        public:
            neuron_view() {};
            neuron_view(const ::uns::nn::address& Address) :
                m_address(Address)
            {};
            virtual ~neuron_view() {};
        public:
            virtual ::std::size_t capacity() const {
                ::std::size_t result = 0;

                result += m_address.capacity();
                result += sizeof(m_dropout);

                return result;
            };
        public:
            virtual ::std::size_t subneurons_total() const { return 0; };
            virtual const ::uns::nn::general::neuron_view<signal_traitset>* subneuron(::std::size_t ConnectionIdx) const { return nullptr; };
            virtual ::uns::nn::general::neuron_view<signal_traitset>* subneuron(::std::size_t ConnectionIdx) { return nullptr; };
        public:
            virtual ::std::size_t params_total() const { return 0; };
#pragma warning(push)
#pragma warning(disable: 4172)
            virtual const typename signal_traitset::signal_type& R() const { return typename signal_traitset::signal_type{ 0 }; };
            virtual const typename signal_traitset::signal_type& C() const { return typename signal_traitset::signal_type{ 0 }; };
#pragma warning(pop)
            virtual const ::uns::nn::address& address() const { return m_address; };
            virtual ::uns::nn::address& address() { return m_address; };
            virtual bool is_reversible() const { return false; };
            virtual const typename signal_traitset::signal_type& dropout() const { return m_dropout; };
            virtual typename signal_traitset::signal_type& dropout() { return m_dropout; };
        public:
            virtual void react(const ::std::vector<typename signal_traitset::params_type>& CommonParams) {};
            virtual void collect(const ::std::vector<typename signal_traitset::params_type>& CommonParams) {};
        };


        //basic activator class is intended to be used as polymorphic wrapper for all other activators
        template<typename signal_traitset_t>
            requires ::std::derived_from<
                signal_traitset_t
                , ::uns::nn::traitset::signal<
                    typename signal_traitset_t::signal_type
                    , typename signal_traitset_t::weight_type
                    , typename signal_traitset_t::params_type
                >
            >
        class activator {
        public:
            using signal_traitset = signal_traitset_t;
        protected:
            typename signal_traitset::signal_type m_value = typename signal_traitset::signal_type{ 0 };
        public:
            activator() {};
            activator(const ::uns::nn::general::activator<signal_traitset>& Activator) :
                m_value(Activator.m_value)
            {};
            ::uns::nn::general::activator<signal_traitset>& operator=(const ::uns::nn::general::activator<signal_traitset>& Activator) {
                if(this == &Activator) return *this;

                m_value = Activator.m_value;

                return *this;
            };
            activator(::uns::nn::general::activator<signal_traitset>&& Activator) :
                m_value(::std::move(Activator.m_value))
            {};
            ::uns::nn::general::activator<signal_traitset>& operator=(::uns::nn::general::activator<signal_traitset>&& Activator) {
                if(this == &Activator) return *this;

                m_value = ::std::move(Activator.m_value);

                return *this;
            };
            activator(const typename signal_traitset::signal_type Value) :
                m_value(Value) {
            };
            ::uns::nn::general::activator<signal_traitset>& operator=(const typename signal_traitset::signal_type Value) {
                m_value = Value;
                return *this;
            };
            virtual ~activator() noexcept {};
        public:
            virtual ::std::size_t capacity() const { return sizeof(*this); };
            virtual ::uns::nn::description::activator<signal_traitset> type() const {
                return ::uns::nn::description::activator<signal_traitset>{};
            };
        public:
            const typename signal_traitset::signal_type& value() const { return m_value; };
            typename signal_traitset::signal_type& value() { return m_value; };
        public:
            virtual typename signal_traitset::signal_type operator()(
                typename signal_traitset::signal_type C
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) {
                return m_value = typename signal_traitset::signal_type{ 0 };
            };
            virtual typename signal_traitset::signal_type _dS(
                typename signal_traitset::signal_type C
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) const {
                return typename signal_traitset::signal_type{ 0 };
            };
            virtual typename signal_traitset::signal_type _dp(
                int ProperParamIdx
                , typename signal_traitset::signal_type C
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) const {
                return typename signal_traitset::signal_type{ 0 };
            };
        public:
            //basic auxiliary class supposed to create activators from string
            //    actually it's a map from string to activator
            class caster {
            public:
                using signal_traitset = signal_traitset_t;
            public:
                virtual ::std::unique_ptr<::uns::nn::general::activator<signal_traitset>> operator()(
                    const ::uns::nn::description::activator<signal_traitset>&
                ) const = 0;
            };
        };


        //basic collector class is intended to be used as polymorphic wrapper for all other collectorss
        template<typename signal_traitset_t>
            requires ::std::derived_from<
                signal_traitset_t
                , ::uns::nn::traitset::signal<
                    typename signal_traitset_t::signal_type
                    , typename signal_traitset_t::weight_type
                    , typename signal_traitset_t::params_type
                >
            >
        class collector {
        public:
            using signal_traitset = signal_traitset_t;
        protected:
            typename signal_traitset::signal_type m_value = typename signal_traitset::signal_type{ 0 };
        public:
            collector() {};
            collector(const ::uns::nn::general::collector<signal_traitset>& Collector) :
                m_value(Collector.m_value)
            {};
            ::uns::nn::general::collector<signal_traitset>& operator=(const ::uns::nn::general::collector<signal_traitset>& Collector) {
                if(this == &Collector) return *this;

                m_value = Collector.m_value;

                return *this;
            };
            collector(::uns::nn::general::collector<signal_traitset>&& Collector) :
                m_value(::std::move(Collector.m_value))
            {};
            ::uns::nn::general::collector<signal_traitset>& operator=(::uns::nn::general::collector<signal_traitset>&& Collector) {
                if(this == &Collector) return *this;

                m_value = ::std::move(Collector.m_value);

                return *this;
            };
            collector(const typename signal_traitset::signal_type Value) :
                m_value(Value)
            {};
            ::uns::nn::general::collector<signal_traitset>& operator=(const typename signal_traitset::signal_type Value) {
                m_value = Value;
                return *this;
            };
            virtual ~collector() noexcept {};
        public:
            virtual ::std::size_t capacity() const { return sizeof(*this); };
            virtual ::uns::nn::description::collector<signal_traitset> type() const {
                return ::uns::nn::description::collector<signal_traitset>{};
            };
        public:
            const typename signal_traitset::signal_type& value() const { return m_value; };
            typename signal_traitset::signal_type& value() { return m_value; };
        public:
            virtual typename signal_traitset::signal_type operator()(
                const ::std::vector<
                    ::std::pair<
                        ::uns::nn::general::neuron_view<signal_traitset>*
                        , typename signal_traitset::weight_type
                    >
                >& Links
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) {
                return m_value = typename signal_traitset::signal_type{ 0 };
            };
            virtual typename signal_traitset::signal_type _dr(
                int LinkIdx
                , const ::std::vector<
                    ::std::pair<
                        ::uns::nn::general::neuron_view<signal_traitset>*
                        , typename signal_traitset::weight_type
                    >
                >& Links
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) const {
                return typename signal_traitset::signal_type{ 0 };
            };
            virtual typename signal_traitset::signal_type _dw(
                int LinkIdx
                , const ::std::vector<
                    ::std::pair<
                        ::uns::nn::general::neuron_view<signal_traitset>*
                        , typename signal_traitset::weight_type
                    >
                >& Links
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) const {
                return typename signal_traitset::signal_type{ 0 };
            };
            virtual typename signal_traitset::signal_type _dp(
                int ProperParamIdx
                , const ::std::vector<
                    ::std::pair<
                        ::uns::nn::general::neuron_view<signal_traitset>*
                        , typename signal_traitset::weight_type
                    >
                >& Links
                , const ::std::vector<typename signal_traitset::params_type>& ProperParams
                , const ::std::vector<typename signal_traitset::params_type>& CommonParams
            ) const {
                return typename signal_traitset::signal_type{ 0 };
            };
        public:
            //basic auxiliary class supposed to create collector from string
            //    actually it's a map from string to collector
            class caster {
            public:
                using signal_traitset = signal_traitset_t;
            public:
                virtual ::std::unique_ptr<::uns::nn::general::collector<signal_traitset>> operator()(
                    const ::uns::nn::description::collector<signal_traitset>&
                ) const = 0;
            };
        };


        //this neuron class is supposed to be the general ancestor for all kinds of neurons
        template<typename neuron_traitset_t>
            requires ::std::derived_from<
                neuron_traitset_t
                , ::uns::nn::traitset::neuron<
                    typename neuron_traitset_t::signal_traitset
                    , typename neuron_traitset_t::description_type
                    , typename neuron_traitset_t::activator_caster_type
                    , typename neuron_traitset_t::collector_caster_type
                >
            >
        class neuron : public ::uns::nn::general::neuron_view<typename neuron_traitset_t::signal_traitset> {
        protected:
            using base = ::uns::nn::general::neuron_view<typename neuron_traitset_t::signal_traitset>;
        public:
            using neuron_traitset = neuron_traitset_t;
        public:
            neuron() {};
            neuron(const ::uns::nn::address& Address) :
                base{ Address }
            {};
        public:
            virtual typename neuron_traitset::description_type descript() const = 0;
            virtual void set(
                const typename neuron_traitset::description_type& NeuronDescription
                , const ::uns::nn::address& ThisNeuronAddress
            ) = 0;
            virtual void link(
                ::std::vector<
                    ::std::vector<
                        ::uns::nn::general::neuron<neuron_traitset>*
                    >
                >& NetworkBody
                , ::std::unordered_map<
                    ::uns::nn::address
                    , ::uns::nn::general::neuron<neuron_traitset>*
                    , ::uns::nn::address::hash
                >& InputNeuronsAddressMap
            ) = 0;
        public:
            virtual ::std::size_t capacity() const override {
                return base::capacity();
            };
        public:
            virtual ::std::size_t subneurons_total() const override { return 0; };
            virtual const ::uns::nn::general::neuron<neuron_traitset>* subneuron(::std::size_t LinkIdx) const override { return nullptr; };
            virtual ::uns::nn::general::neuron<neuron_traitset>* subneuron(::std::size_t LinkIdx) override { return nullptr; };
        public:
            virtual ::std::size_t params_total() const override { return 0; };
        public:
            virtual const typename neuron_traitset::signal_traitset::signal_type& R() const override { return typename neuron_traitset::signal_traitset::signal_type{ 0 }; };
            virtual const typename neuron_traitset::signal_traitset::signal_type& C() const override { return typename neuron_traitset::signal_traitset::signal_type{ 0 }; };
        public:
            virtual bool is_reversible() const override { return false; };
        public:
            virtual void react(const ::std::vector<typename neuron_traitset::signal_traitset::params_type>& CommonParams) override {};
            virtual void collect(const ::std::vector<typename neuron_traitset::signal_traitset::params_type>& CommonParams) override {};
        };


        //all input data (either: straight or reversive) must be provided for all neural networks using some classes derived from this
        template<typename input_data_object_traitset_t>
            requires ::std::derived_from<
                input_data_object_traitset_t
                , ::uns::nn::traitset::input<
                    typename input_data_object_traitset_t::signal_traitset
                    , typename input_data_object_traitset_t::iterator_type
                    , typename input_data_object_traitset_t::input_allocator_type
                >
            >
        class input_data_object {
        public:
            using input_traitset = input_data_object_traitset_t;
            using iterator_type = typename input_data_object_traitset_t::iterator_type;
            using input_allocator_type = typename input_data_object_traitset_t::input_allocator_type;

            virtual typename input_traitset::input_neuron_type* get(const ::uns::nn::address& RequestedInputNeuronsAddress) = 0;
            virtual iterator_type begin() const = 0;
            virtual iterator_type end() const = 0;
        };


        //all neural networks must be inherited from that class
        template<typename network_traitset_t>
            requires ::std::derived_from<
                network_traitset_t
                , ::uns::nn::traitset::network<
                    typename network_traitset_t::neuron_type
                    , typename network_traitset_t::description_type
                    , typename network_traitset_t::input_data_object_type
                >
            >
        class network {
        public:
            using network_traitset = typename network_traitset_t;
        protected:
            ::std::vector<typename network_traitset::neuron_type*> m_outputs;
            ::std::vector<typename network_traitset::input_data_object_type::input_traitset::input_neuron_type*> m_inputs;
        public:
            virtual typename network_traitset::description_type descript() const = 0;
            virtual void set(
                const typename network_traitset::description_type& NetworkDescription
                , typename network_traitset::input_data_object_type& InputDataObject
            ) = 0;
            virtual void react(const ::std::vector<typename network_traitset::neuron_type::neuron_traitset::signal_traitset::params_type>& CommonParams) {};

            virtual ::std::size_t capacity() const {
                ::std::size_t result = sizeof(*this)
                    + sizeof(m_outputs)
                    + m_outputs.capacity() * sizeof(typename decltype(m_outputs)::value_type)
                    + sizeof(m_inputs)
                    + m_inputs.capacity() * sizeof(typename decltype(m_inputs)::value_type);

                for(auto output_ptr : m_outputs) {
                    result += output_ptr->capacity();
                };
                for(auto input_ptr : m_inputs) {
                    result += input_ptr->capacity();
                };

                return result;
            };
        };

    };


    //a class of neuron introduces the neurons supposed to consist sequential single-directional neural network's body
    template<typename neuron_traitset_t>
        requires ::std::derived_from<
            neuron_traitset_t
            , ::uns::nn::traitset::neuron<
                typename neuron_traitset_t::signal_traitset
                , typename neuron_traitset_t::description_type
                , typename neuron_traitset_t::activator_caster_type
                , typename neuron_traitset_t::collector_caster_type
            >
        >
    class sequential_neuron : public ::uns::nn::general::neuron<neuron_traitset_t> {
    public:
        using base = ::uns::nn::general::neuron<neuron_traitset_t>;
        using neuron_traitset = typename base::neuron_traitset;
        using signal_type = typename base::neuron_traitset::signal_traitset::signal_type;
        using weight_type = typename base::neuron_traitset::signal_traitset::weight_type;
    public:
        enum part {
            _neuron_ = 0,
            _weight_ = 1
        };
    protected:
        ::std::unique_ptr<::uns::nn::general::activator<typename base::neuron_traitset::signal_traitset>> m_F = nullptr;
        ::std::unique_ptr<::uns::nn::general::collector<typename base::neuron_traitset::signal_traitset>> m_S = nullptr;
        ::std::vector<::std::pair<::uns::nn::general::neuron_view<typename base::neuron_traitset::signal_traitset>*, typename base::neuron_traitset::signal_traitset::weight_type>> m_links;
        ::std::vector<typename base::neuron_traitset::signal_traitset::params_type> m_params;
    protected:
        ::std::unique_ptr<::std::vector<::std::pair<::uns::nn::address, typename base::neuron_traitset::signal_traitset::weight_type>>> m_addressses = nullptr;
    public:
        sequential_neuron() : m_F(nullptr), m_S(nullptr), m_addressses(nullptr) {};
        sequential_neuron(const ::uns::nn::sequential_neuron<typename base::neuron_traitset>&) = delete;
        ::uns::nn::sequential_neuron<typename base::neuron_traitset>& operator=(const ::uns::nn::sequential_neuron<typename base::neuron_traitset>&) = delete;
        sequential_neuron(::uns::nn::sequential_neuron<typename base::neuron_traitset>&&) = delete;
        ::uns::nn::sequential_neuron<typename base::neuron_traitset>& operator=(::uns::nn::sequential_neuron<typename base::neuron_traitset>&&) = delete;
        virtual ~sequential_neuron() {};
    protected:
        void clear() {
            m_F = nullptr;
            m_S = nullptr;

            m_links.clear();
            m_params.clear();

            m_addressses = nullptr;
        };
    public:
        virtual typename base::neuron_traitset::description_type descript() const override {
            auto res = typename base::neuron_traitset::description_type{};

            if(m_F != nullptr) {
                res.activator = m_F->type();
            };

            if(m_S != nullptr) {
                res.collector = m_S->type();
            };

            res.params = m_params;

            for(auto link : m_links) {
                res.links.emplace_back(
                    ::std::pair<::uns::nn::address, typename base::neuron_traitset::signal_traitset::weight_type>{
                        ::std::get<part::_neuron_>(link)->address(),
                        ::std::get<part::_weight_>(link)
                    }
                );
            };

            return res;
        };
        virtual void set(
            const typename base::neuron_traitset::description_type& NeuronDescription
            , const ::uns::nn::address& adr
        ) override {
            auto activator_cast = typename base::neuron_traitset::activator_caster_type{};
            auto collector_cast = typename base::neuron_traitset::collector_caster_type{};

            this->address() = adr;

            m_F = activator_cast(NeuronDescription.activator);
            if(m_F == nullptr) {
                clear();
                throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
            };

            m_S = collector_cast(NeuronDescription.collector);
            if(m_S == nullptr) {
                clear();
                throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
            };

            *m_F = NeuronDescription.r;
            *m_S = NeuronDescription.c;

            m_params = NeuronDescription.params;

            m_addressses = ::std::unique_ptr<::std::vector<::std::pair<::uns::nn::address, typename base::neuron_traitset::signal_traitset::weight_type>>>{
                new ::std::vector<::std::pair<::uns::nn::address, typename base::neuron_traitset::signal_traitset::weight_type>>{}
            };
            *m_addressses = NeuronDescription.links;
        };
        virtual void link(
            ::std::vector<
                ::std::vector<
                    ::uns::nn::general::neuron<
                        typename base::neuron_traitset
                    >*
                >
            >& NetworkBody
            , ::std::unordered_map<
                ::uns::nn::address
                , ::uns::nn::general::neuron<typename base::neuron_traitset>*
                , ::uns::nn::address::hash
            >& InputNeuronsAddressMap
        ) override {
            if(m_addressses == nullptr) {
                throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
            };

            m_links.clear();

            for(const auto& [address, weight] : *m_addressses) {
                auto link = ::std::pair<
                    ::uns::nn::general::neuron_view<typename base::neuron_traitset::signal_traitset>*
                    , typename base::neuron_traitset::signal_traitset::weight_type
                >{};

                if(
                    auto iter = InputNeuronsAddressMap.find(address);
                    iter != InputNeuronsAddressMap.end()
                ) {
                    ::std::get<part::_neuron_>(link) = iter->second;
                }
                else {
                    if(
                        address.layer >= 0 && address.layer < NetworkBody.size()
                        && address.index >= 0 && address.index < NetworkBody[address.layer].size()
                    ) {
                        ::std::get<part::_neuron_>(link) = NetworkBody[address.layer][address.index];
                    }
                    else {
                        clear();
                        throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
                    };
                };
                ::std::get<part::_weight_>(link) = weight;
                m_links.push_back(link);
            };

            m_addressses = nullptr;
        };
    public:
        virtual ::std::size_t capacity() const override {
            ::std::size_t result = base::capacity();

            result += sizeof(m_F) + m_F->capacity();
            result += sizeof(m_S) + m_S->capacity();
            result += m_links.capacity() * sizeof(typename decltype(m_links)::value_type);
            result += m_params.capacity() * sizeof(typename decltype(m_params)::value_type);

            return result;
        };
    public:
        virtual ::std::size_t links_total() const override { return m_links.size(); };
        virtual ::std::size_t params_total() const override { return m_params.size(); };
    public:
        virtual const ::uns::nn::general::neuron<typename base::neuron_traitset>* subneuron(::std::size_t LinkIdx) const override {
            if(LinkIdx >= 0 && LinkIdx < m_links.size()) {
                return static_cast<::uns::nn::general::neuron<typename base::neuron_traitset>*>(::std::get<part::_neuron_>(m_links[LinkIdx]));
            }
            else {
                return nullptr;
            };
        };
        virtual ::uns::nn::general::neuron<typename base::neuron_traitset>* subneuron(::std::size_t LinkIdx) override {
            if(LinkIdx >= 0 && LinkIdx < m_links.size()) {
                return static_cast<::uns::nn::general::neuron<typename base::neuron_traitset>*>(::std::get<part::_neuron_>(m_links[LinkIdx]));
            }
            else {
                return nullptr;
            };
        };
    public:
        virtual const typename base::neuron_traitset::signal_traitset::signal_type& R() const override { return m_F->value(); };
        virtual typename base::neuron_traitset::signal_traitset::signal_type& R() { return m_F->value(); };
        virtual const typename base::neuron_traitset::signal_traitset::signal_type& C() const override { return m_S->value(); };
        virtual typename base::neuron_traitset::signal_traitset::signal_type& C() { return m_S->value(); };
    public:
        virtual void react(const ::std::vector<typename base::neuron_traitset::signal_traitset::params_type>& CommonParams) override { (*m_F)(m_S->value(), m_params, CommonParams); };
        virtual void collect(const ::std::vector<typename base::neuron_traitset::signal_traitset::params_type>& CommonParams) override { (*m_S)(m_links, m_params, CommonParams); };
    };


    //a class of neuron introduces the nonrecursive(without recursive neurons) reverssive neuron using for learning with gradient-descendance method
    template<typename neuron_traitset_t>
    class nonrecursive_reversive_neuron : public ::uns::nn::sequential_neuron<neuron_traitset_t> {
    protected:
        using base = ::uns::nn::sequential_neuron<neuron_traitset_t>;
    public:
        using neuron_traitset = typename base::neuron_traitset;
        using signal_type = typename base::signal_type;
        using weight_type = typename base::weight_type;
        using place_type = int;
    protected:
        static const int _place_ = 1;
        typename base::neuron_traitset::signal_traitset::signal_type m_r = 0;
        typename base::neuron_traitset::signal_traitset::signal_type m_s = 0;
        ::std::vector<::std::pair<::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>*, place_type>> m__links;
        ::uns::nn::general::neuron<typename base::neuron_traitset>* m_input = nullptr;
        bool m_is_learning = true;
    public:
        nonrecursive_reversive_neuron() {};
        nonrecursive_reversive_neuron(const ::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>&) = delete;
        ::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>& operator=(const ::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>&) = delete;
        nonrecursive_reversive_neuron(::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>&&) = delete;
        ::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>& operator=(::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>&&) = delete;
        virtual ~nonrecursive_reversive_neuron() {};
    protected:
        void clear() {
            base::clear();

            m__links.clear();
            m_input = nullptr;
        };
    public:
        virtual void _link(
            ::std::vector<
                ::std::vector<
                    ::uns::nn::nonrecursive_reversive_neuron<
                        typename base::neuron_traitset
                    >*
                >
            >& NetworkBody
        ) {
            m__links.clear();

            for (const auto& layer : NetworkBody) {
                for (const auto& neuron_ptr : layer) {
                    if (neuron_ptr == nullptr) {
                        clear();
                        throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
                    };

                    for (::std::size_t subneuron_idx = 0; subneuron_idx < neuron_ptr->subneurons_total(); ++subneuron_idx) {
                        if (neuron_ptr->subneuron(subneuron_idx)->address() == this->address()) {
                            m__links.emplace_back(::std::pair<::uns::nn::nonrecursive_reversive_neuron<typename base::neuron_traitset>*, place_type>{ neuron_ptr, subneuron_idx });
                        };
                    };
                };
            };
        };
    public:
        virtual ::std::size_t capacity() const override {
            ::std::size_t result = base::capacity();

            result += sizeof(m_r);
            result += sizeof(m_s);
            result += m__links.capacity() * sizeof(typename decltype(m__links)::value_type);
            result += sizeof(m_input);
            result += sizeof(m_is_learning);

            return result;
        };
    public:
        virtual bool is_reversible() const override { return true; };
    public:
        virtual const typename base::neuron_traitset::signal_traitset::signal_type& _R() const { return m_r; };
        virtual typename base::neuron_traitset::signal_traitset::signal_type& _R() { return m_r; };
        virtual const typename base::neuron_traitset::signal_traitset::signal_type& _C() const { return m_s; };
        virtual typename base::neuron_traitset::signal_traitset::signal_type& _C() { return m_s; };
    public:
        virtual void _react(const ::std::vector<typename base::neuron_traitset::signal_traitset::params_type>& CommonParams) {
            m_r = dF_dS(CommonParams) * m_s;
        };
        virtual void _collect(const ::std::vector<typename base::neuron_traitset::signal_traitset::params_type>& common_params) {
            for(auto _link : m__links) {
                m_s += ::std::get<base::part::_neuron_>(_link)->m_r * ::std::get<base::part::_neuron_>(_link)->dS_dr(::std::get<_place_>(_link), common_params);
            };
            if(m_input != nullptr) {
                m_s += m_input->R();
            };
        };
    public:
        virtual bool is_learning() const { return m_is_learning; };
        virtual bool& is_learning() { return m_is_learning; };
    public:
        virtual typename base::neuron_traitset::signal_traitset::signal_type dS_dr(
            int LinkIndex
            , const ::std::vector<
                typename base::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) const {
            return base::m_S->_dr(LinkIndex, base::m_links, base::m_params, CommonParams);
        };
        virtual typename base::neuron_traitset::signal_traitset::signal_type dS_dw(
            int LinkIndex
            , const ::std::vector<
                typename base::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) const {
            return base::m_S->_dw(LinkIndex, base::m_links, base::m_params, CommonParams);
        };
        virtual typename base::neuron_traitset::signal_traitset::signal_type dS_dp(
            int ProperParamIdx
            , const ::std::vector<
                typename base::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) const {
            return base::m_S->_dp(ProperParamIdx, base::m_links, base::m_params, CommonParams);
        };
        virtual typename base::neuron_traitset::signal_traitset::signal_type dF_dS(
            const ::std::vector<
                typename base::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) const {
            return base::m_F->_dS(base::C(), base::m_params, CommonParams);
        };
        virtual typename base::neuron_traitset::signal_traitset::signal_type dF_dp(
            int ProperParamIdx
            , const ::std::vector<
                typename base::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) const {
            return base::m_F->_dp(ProperParamIdx, base::C(), base::m_params, CommonParams);
        };
    };


    //a class of network introduces the single-dimentional sequential neural network
    template<class network_traitset_t>
    class sequential_network : public ::uns::nn::general::network<network_traitset_t> {
    protected:
        using base = ::uns::nn::general::network<network_traitset_t>;
    public:
        using network_traitset = typename base::network_traitset;
    public:
        ::std::vector<::std::vector<typename base::network_traitset::neuron_type*>> m_layers;
    public:
        sequential_network() {};
        sequential_network(const ::uns::nn::sequential_network<typename base::network_traitset>& net) = delete;
        ::uns::nn::sequential_network<typename base::network_traitset>& operator=(const ::uns::nn::sequential_network<typename base::network_traitset>& net) = delete;
        sequential_network(::uns::nn::sequential_network<typename base::network_traitset>&& net) = delete;
        ::uns::nn::sequential_network<typename base::network_traitset>& operator=(::uns::nn::sequential_network<typename base::network_traitset>&& net) = delete;
        virtual ~sequential_network() { clear(); };
    public:
        void clear() {
            for(auto& layer : m_layers) {
                for(auto neuron : layer) {
                    delete neuron;
                };
            };
            m_layers.clear();

            auto inputs_allocator = typename base::network_traitset::input_data_object_type::input_allocator_type{};
            for(auto input : base::m_inputs) {
                inputs_allocator.deallocate(input, 1);
            };
            base::m_inputs.clear();

            base::m_outputs.clear();
        };
    public:
        virtual typename base::network_traitset::description_type descript() const override {
            auto res = typename base::network_traitset::description_type{};

            for(const auto& layer : m_layers) {
                res.layers.push_back(::std::vector<typename base::network_traitset::neuron_type::neuron_traitset::description_type>{});
                for(auto neuron_ptr : layer) {
                    res.layers.back().emplace_back(neuron_ptr->descript());
                };
            };

            for(const auto output_ptr : base::m_outputs) {
                res.outputs.push_back(output_ptr->address());
            };

            return res;
        };
        virtual void set(
            const typename base::network_traitset::description_type& NetworkDescription
            , typename base::network_traitset::input_data_object_type& InputDataObject
        ) override {
            clear();

            for(const auto& layer : NetworkDescription.layers) {
                m_layers.push_back(::std::vector<typename base::network_traitset::neuron_type*>{});
                for(const auto& neuron : layer) {
                    auto neuron_ptr = new typename base::network_traitset::neuron_type{};
                    m_layers.back().push_back(neuron_ptr);
                };
            };

            for(const auto& output : NetworkDescription.outputs) {
                if(
                    output.layer >= 0 && output.layer < m_layers.size()
                    && output.index >= 0 && output.index < m_layers[output.layer].size()
                ) {
                    base::m_outputs.push_back(m_layers[output.layer][output.index]);
                }
                else {
                    clear();
                    throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
                };
            };

            for(::std::size_t layer_idx = 0; layer_idx < m_layers.size(); ++layer_idx) {
                for(::std::size_t neuron_idx = 0; neuron_idx < m_layers[layer_idx].size(); ++neuron_idx) {
                    if(layer_idx < NetworkDescription.layers.size() && neuron_idx < NetworkDescription.layers[layer_idx].size()) {
                        try {
                            m_layers[layer_idx][neuron_idx]->set(
                                NetworkDescription.layers[layer_idx][neuron_idx],
                                ::uns::nn::address{ static_cast<int>(layer_idx), static_cast<int>(neuron_idx) }
                            );
                        }
                        catch(const ::std::exception& e) {
                            clear();
                            throw e;
                        };
                    }
                    else {
                        clear();
                        throw ::std::runtime_error(UNS_DEV_EXCEPTION_MSG);
                    };
                };
            };

            auto layers = ::std::vector<::std::vector<::uns::nn::general::neuron<typename base::network_traitset::neuron_type::neuron_traitset>*>>{};
            for(auto& layer : m_layers) {
                layers.push_back(::std::vector<::uns::nn::general::neuron<typename base::network_traitset::neuron_type::neuron_traitset>*>{});
                for(auto neuron_ptr : layer) {
                    layers.back().push_back(neuron_ptr);
                };
            };
            auto inputs_map = ::std::unordered_map<::uns::nn::address, ::uns::nn::general::neuron<typename base::network_traitset::neuron_type::neuron_traitset>*, ::uns::nn::address::hash>{};
            for(auto input_addresss = InputDataObject.begin(); input_addresss != InputDataObject.end(); ++input_addresss) {
                auto* input_neuron = InputDataObject.get(*input_addresss);
                inputs_map[*input_addresss] = input_neuron;
                base::m_inputs.push_back(input_neuron);
            };
            for(auto& layer : m_layers) {
                for(auto neuron_ptr : layer) {
                    try {
                        neuron_ptr->link(layers, inputs_map);
                    }
                    catch(const ::std::exception& e) {
                        clear();
                        throw e;
                    };
                };
            };
        };
    public:
        virtual ::std::size_t capacity() const override {
            ::std::size_t result = base::capacity();

            result += m_layers.capacity() * sizeof(typename decltype(m_layers)::value_type);

            for(const auto& layer : m_layers) {
                result += layer.capacity() * sizeof(typename base::network_traitset::neuron_type*);

                for(auto neuron_ptr : layer) {
                    result += neuron_ptr->capacity();
                };
            };

            return result;
        };
    public:
        virtual void react(
            const ::std::vector<
                typename base::network_traitset::neuron_type::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) override {
            for(auto input : base::m_inputs) {
                input->react(CommonParams);
            };
            for(auto& layer : m_layers) {
                for(auto neuron : layer) {
                    neuron->collect(CommonParams);
                    neuron->react(CommonParams);
                };
            };
        };
    public:
        virtual typename base::network_traitset::neuron_type::neuron_traitset::signal_traitset::signal_type O(::std::size_t index) const { return base::m_outputs[index]->R(); };
        virtual typename base::network_traitset::neuron_type::neuron_traitset::signal_traitset::signal_type I(::std::size_t index) const { return base::m_inputs[index]->R(); };
    public:
        virtual ::std::size_t layers_total() const { return m_layers.size(); };
        virtual ::std::size_t neurons_total(size_t LayerIdx) const { return m_layers.at(LayerIdx).size(); };
        virtual ::std::size_t outputs_total() const { return base::m_outputs.size(); };
        virtual ::std::size_t inputs_total() const { return base::m_inputs.size(); };
        virtual const ::uns::nn::address& output(::std::size_t OutputIdx) const { return base::m_outputs.at(OutputIdx)->address(); };
    public:
        virtual const typename base::network_traitset::neuron_type& access(const ::uns::nn::address& Address) const {
            return *m_layers.at(static_cast<::std::size_t>(Address.layer)).at(static_cast<::std::size_t>(Address.index));
        };
        virtual typename base::network_traitset::neuron_type& access(const ::uns::nn::address& Address) {
            return *m_layers.at(static_cast<::std::size_t>(Address.layer)).at(static_cast<::std::size_t>(Address.index));
        };
    };


    //a class of network introduces the reversable neural network
    template<class network_traitset_t>
        requires ::std::derived_from<
            network_traitset_t
            , ::uns::nn::traitset::network<
                typename network_traitset_t::neuron_type
                , typename network_traitset_t::description_type
                , typename network_traitset_t::input_data_object_type
            >
        >
        && ::std::derived_from<
            typename network_traitset_t::neuron_type
            , ::uns::nn::nonrecursive_reversive_neuron<
                typename network_traitset_t::neuron_type::neuron_traitset
            >
        >
    class nonrecursive_reversive_network : public ::uns::nn::sequential_network<network_traitset_t> {
    protected:
        using base = ::uns::nn::sequential_network<network_traitset_t>;
    public:
        using network_traitset = typename base::network_traitset;
    public:
        nonrecursive_reversive_network() {};
        nonrecursive_reversive_network(const ::uns::nn::nonrecursive_reversive_network<typename base::network_traitset>&) = delete;
        ::uns::nn::nonrecursive_reversive_network<typename base::network_traitset>& operator=(const ::uns::nn::nonrecursive_reversive_network<typename base::network_traitset>&) = delete;
        nonrecursive_reversive_network(::uns::nn::nonrecursive_reversive_network<typename base::network_traitset>&&) = delete;
        ::uns::nn::nonrecursive_reversive_network<typename base::network_traitset>& operator=(::uns::nn::nonrecursive_reversive_network<typename base::network_traitset>&&) = delete;
        virtual ~nonrecursive_reversive_network() { clear(); };
    public:
        void clear() {
            base::clear();
        };
    public:
        virtual void _link() {
            for (const auto& layer : base::m_layers) {
                for (auto neuron_ptr : layer) {
                    neuron_ptr->_link(base::m_layers);
                };
            };
        };
    public:
        virtual void _react(
            const ::std::vector<
                typename base::network_traitset::neuron_type::neuron_traitset::signal_traitset::params_type
            >& CommonParams
        ) {
            for(auto layer_ptr = base::m_layers.rbegin(); layer_ptr < base::m_layers.rend(); ++layer_ptr) {
                for(auto neuron_ptr = layer_ptr->rbegin(); neuron_ptr < layer_ptr->rend(); ++neuron_ptr) {
                    (*neuron_ptr)->_collect(CommonParams);
                    (*neuron_ptr)->_react(CommonParams);
                };
            };
        };
    };
};

#endif

//TODO to make a macro to alter performing array-bounds-check optionally
