
#include <iostream>
#include <array>
#include <optional>

#include "uniself/strings.hpp"
#include "uniself/neuronetworks.hpp"

#include <gtest/gtest.h>


namespace uns::tests {

    using signal = ::uns::nn::traitset::signal<long double, float, float>;


    namespace hashsum {
        constexpr uint64_t line_activator = 117;
        constexpr uint64_t perc_collector = 13;
    };


    class line_activator : public ::uns::nn::general::activator<::uns::tests::signal> {
    protected:
        using base = ::uns::nn::general::activator<::uns::tests::signal>;
    public:
        line_activator(const typename signal_traitset::signal_type value) :
            base{ value }
        {};
    public:
        virtual ::uns::nn::description::activator<::uns::tests::signal> type() const override {
            auto descriptor = ::uns::nn::description::activator<::uns::tests::signal>{};
            descriptor.hashsum = ::uns::tests::hashsum::line_activator;
            descriptor.value = 0;
            return descriptor;
        };
    public:
        virtual typename base::signal_traitset::signal_type operator() (
            typename base::signal_traitset::signal_type C,
            const ::std::vector<typename base::signal_traitset::params_type>& lps,
            const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) override {
            return base::m_value = static_cast<base::signal_traitset::signal_type>(lps[0]) * C
                + static_cast<base::signal_traitset::signal_type>(lps[1]);
        };
        virtual typename base::signal_traitset::signal_type _dS(
            typename base::signal_traitset::signal_type C,
            const ::std::vector<typename base::signal_traitset::params_type>& lps,
            const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) const override {
            return static_cast<base::signal_traitset::signal_type>(lps[0]);
        };
        virtual typename base::signal_traitset::signal_type _dp(
            int param_idx,
            typename base::signal_traitset::signal_type C,
            const ::std::vector<typename base::signal_traitset::params_type>& lps,
            const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) const override {
            switch (param_idx) {
                case 0:
                {
                    return C;
                    break;
                }
                case 1:
                {
                    return typename base::signal_traitset::signal_type{ 1 };
                    break;
                }
                default:
                {
                    return typename base::signal_traitset::signal_type{ 0 };
                    break;
                }
            };
        };
    };


    class perc_collector : public ::uns::nn::general::collector<::uns::tests::signal> {
    protected:
        using base = ::uns::nn::general::collector<::uns::tests::signal>;
    public:
        perc_collector(const typename signal_traitset::signal_type value) :
            base{ value }
        {};
    public:
        virtual ::uns::nn::description::collector<::uns::tests::signal> type() const override {
            auto descriptor = ::uns::nn::description::collector<::uns::tests::signal>{};
            descriptor.hashsum = ::uns::tests::hashsum::perc_collector;
            descriptor.value = 0;
            return descriptor;
        };
    public:
        virtual typename base::signal_traitset::signal_type operator()(
            const ::std::vector<
                ::std::pair<
                    ::uns::nn::general::neuron_view<typename base::signal_traitset>*
                    , typename base::signal_traitset::weight_type
                >
            >& conns
            , const ::std::vector<typename base::signal_traitset::params_type>& lps
            , const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) override {
            base::m_value = typename base::signal_traitset::signal_type{ 0 };

            for (const auto& conn : conns) {
                base::m_value += conn.first->R() * static_cast<typename base::signal_traitset::signal_type>(conn.second);
            };

            return base::m_value += lps[2];
        };
    public:
        virtual typename base::signal_traitset::signal_type _dr(
            int conn_idx
            , const ::std::vector<
                ::std::pair<
                    ::uns::nn::general::neuron_view<typename base::signal_traitset>*
                    , typename base::signal_traitset::weight_type
                >
            >& conns
            , const ::std::vector<typename base::signal_traitset::params_type>& lps
            , const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) const {
            return static_cast<typename base::signal_traitset::signal_type>(conns.at(conn_idx).second);
        };
        virtual typename base::signal_traitset::signal_type _dw(
            int conn_idx
            , const ::std::vector<
                ::std::pair<
                    ::uns::nn::general::neuron_view<typename base::signal_traitset>*
                    , typename base::signal_traitset::weight_type
                >
            >& conns
            , const ::std::vector<typename base::signal_traitset::params_type>& lps
            , const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) const {
            return conns.at(conn_idx).first->R();
        };
        virtual typename base::signal_traitset::signal_type _dp(
            int param_idx
            , const ::std::vector<
                ::std::pair<
                    ::uns::nn::general::neuron_view<typename base::signal_traitset>*
                    , typename base::signal_traitset::weight_type
                >
            >& conns
            , const ::std::vector<typename base::signal_traitset::params_type>& lps
            , const ::std::vector<typename base::signal_traitset::params_type>& gps
        ) const {
            switch (param_idx) {
                case 2:
                {
                    return typename base::signal_traitset::signal_type{ 1 };
                    break;
                }
                default:
                {
                    return typename base::signal_traitset::signal_type{ 0 };
                    break;
                }
            };
        };
    };


    class activator_caster : public ::uns::nn::general::activator<::uns::tests::signal>::caster {
    public:
        virtual ::std::unique_ptr<::uns::nn::general::activator<::uns::tests::signal>> operator()(
            const ::uns::nn::description::activator<::uns::tests::signal>& Descriptor
        ) const override {
            if (Descriptor.hashsum == ::uns::tests::hashsum::line_activator) {
                return ::std::make_unique<::uns::tests::line_activator>(Descriptor.value);
            }
            else {
                return nullptr;
            };
        };
    };


    class collector_caster : public ::uns::nn::general::collector<::uns::tests::signal>::caster {
    public:
        virtual ::std::unique_ptr<::uns::nn::general::collector<::uns::tests::signal>> operator()(
            const ::uns::nn::description::collector<::uns::tests::signal>& Descriptor
        ) const override {
            if (Descriptor.hashsum == ::uns::tests::hashsum::perc_collector) {
                return ::std::make_unique<::uns::tests::perc_collector>(Descriptor.value);
            }
            else {
                return nullptr;
            };
        };
    };


    using traitset_neuron = ::uns::nn::traitset::neuron<
        ::uns::tests::signal
        , ::uns::nn::description::neuron<::uns::tests::signal>
        , ::uns::tests::activator_caster
        , ::uns::tests::collector_caster
    >;


    template<typename value_type>
    class buffer_handler {
    protected:
        const value_type& m_buffer;
    public:
        buffer_handler() noexcept = default;
        buffer_handler(const value_type& signal_buffer) noexcept : m_buffer(signal_buffer) {};
        buffer_handler(const ::uns::tests::buffer_handler<value_type>&) noexcept = default;
        ::uns::tests::buffer_handler<value_type>& operator=(const ::uns::tests::buffer_handler<value_type>&) noexcept = default;
        buffer_handler(::uns::tests::buffer_handler<value_type>&&) noexcept = default;
        ::uns::tests::buffer_handler<value_type>& operator=(::uns::tests::buffer_handler<value_type>&&) noexcept = default;
        ~buffer_handler() noexcept = default;
    public:
        ::std::size_t capacity() const noexcept { return sizeof(&m_buffer); };
    public:
        const value_type& get() const noexcept { return m_buffer; };
    };


    template<typename neuron_traitset_t>
    class input_neuron : public ::uns::nn::general::neuron<typename neuron_traitset_t> {
    public:
        using base = ::uns::nn::general::neuron<typename neuron_traitset_t>;
        using neuron_traitset = neuron_traitset_t;
    protected:
        using buffer_wrapper = ::uns::tests::buffer_handler<typename neuron_traitset::signal_traitset::signal_type>;
    protected:
        ::std::unique_ptr<buffer_wrapper> m_buffer = nullptr;
        typename neuron_traitset::signal_traitset::signal_type m_value = typename neuron_traitset::signal_traitset::signal_type{ 0 };
    public:
        input_neuron() noexcept {};
        input_neuron(const ::uns::nn::address& address, const typename neuron_traitset::signal_traitset::signal_type& input_buffer) noexcept :
            ::uns::nn::general::neuron<neuron_traitset_t>{ address },
            m_buffer(new buffer_wrapper{ input_buffer })
        {
        };
        input_neuron(const ::uns::tests::input_neuron<neuron_traitset>&) = delete;
        ::uns::tests::input_neuron<neuron_traitset>& operator=(const ::uns::tests::input_neuron<neuron_traitset>&) = delete;
        input_neuron(::uns::tests::input_neuron<neuron_traitset>&&) = delete;
        ::uns::tests::input_neuron<neuron_traitset>& operator=(::uns::tests::input_neuron<neuron_traitset>&&) = delete;
        ~input_neuron() noexcept {};
    public:
        virtual ::std::size_t capacity() const override { return base::capacity() + sizeof(m_value) + sizeof(m_buffer) + (m_buffer != nullptr ? m_buffer->capacity() : 0); };
    public:
        virtual const typename neuron_traitset::signal_traitset::signal_type& R() const override { return m_value; };
        virtual const typename neuron_traitset::signal_traitset::signal_type& C() const override { return m_value; };
    public:
        virtual void react(const ::std::vector<typename neuron_traitset::signal_traitset::params_type>& common_params) override {
            if (m_buffer == nullptr) {
                m_value = 0;
            }
            else {
                m_value = m_buffer->get();
            };
        };
        virtual void collect(const ::std::vector<typename neuron_traitset::signal_traitset::params_type>& common_params) override {};
        virtual typename neuron_traitset::description_type descript() const override {
            return {};
        };
        virtual void set(
            const typename neuron_traitset::description_type&,
            const ::uns::nn::address&
        ) override {};
        virtual void link(
            ::std::vector<::std::vector<::uns::nn::general::neuron<neuron_traitset>*>>&,
            ::std::unordered_map<::uns::nn::address, ::uns::nn::general::neuron<neuron_traitset>*, ::uns::nn::address::hash>&
        ) override {};
    };



    class const_iterator {
    protected:
        ::std::size_t m_end_pointer = 1;
        ::std::size_t m_page_size = 1;
        ::std::size_t m_position = 0;
    public:
        const_iterator() noexcept = default;
        inline const_iterator(
            ::std::size_t Position,
            ::std::size_t End,
            ::std::size_t PageSize
        ) noexcept :
            m_end_pointer(End),
            m_page_size(PageSize),
            m_position(Position) {
            if (m_page_size == 0) {
                m_page_size = m_end_pointer;
            };

            if (m_position >= m_end_pointer) {
                m_position = m_end_pointer;
            };
        };
        const_iterator(const ::uns::tests::const_iterator&) noexcept = default;
        ::uns::tests::const_iterator& operator=(const ::uns::tests::const_iterator&) noexcept = default;
        const_iterator(::uns::tests::const_iterator&&) noexcept = default;
        ::uns::tests::const_iterator& operator=(::uns::tests::const_iterator&&) noexcept = default;
        ~const_iterator() noexcept = default;
    public:
        inline ::uns::tests::const_iterator& operator++() noexcept {
            if (m_position < m_end_pointer) {
                ++m_position;
            }
            else {
                m_position = m_end_pointer;
            };

            return *this;
        };
        ::uns::nn::address operator*() const noexcept {
            return ::uns::nn::address{
                -static_cast<int>(m_position / m_page_size + 1),
                static_cast<int>(m_position % m_page_size)
            };
        };
    public:
        bool operator==(const ::uns::tests::const_iterator& Iter) const noexcept {
            return (m_end_pointer == Iter.m_end_pointer)
                && (m_page_size == Iter.m_page_size)
                && (m_position == Iter.m_position);
        };
        bool operator!=(const ::uns::tests::const_iterator& Iter) const noexcept {
            return !(*this == Iter);
        };
    };


    using input = ::uns::nn::traitset::input<
        ::uns::tests::signal
        , typename ::uns::tests::const_iterator
        , typename ::std::allocator<
            ::uns::tests::input_neuron<::uns::tests::traitset_neuron>
        >
    >;


    constexpr ::std::size_t data_buffer_capacity = 10;


    class object : public ::uns::nn::general::input_data_object<::uns::tests::input> {
    protected:
        using traitset = ::uns::tests::input;
        using base = ::uns::nn::general::input_data_object<traitset>;
    protected:
        ::std::array<typename traitset::signal_traitset::signal_type, ::uns::tests::data_buffer_capacity> m_buffer;
        ::std::size_t m_page_size = m_buffer.size();
        ::std::size_t m_actual_size = m_buffer.size();
    public:
        inline object() {
            m_buffer.fill(
                typename traitset::signal_traitset::signal_type{ 0 }
            );
        };
        object(const ::uns::tests::object&) = delete;
        ::uns::tests::object& operator=(const ::uns::tests::object&) = delete;
        object(::uns::tests::object&&) = delete;
        ::uns::tests::object& operator=(::uns::tests::object&&) = delete;
        ~object() = default;
    protected:
        inline typename decltype(m_buffer)::const_iterator find(const ::uns::nn::address& address) const noexcept {
            if (
                address.layer < 0
                && static_cast<::std::size_t>(-address.layer - 1) < (m_buffer.size() / m_page_size)   //if the last page does not fullfill the full size -> it is concidered as invalid
            ) {
                if (
                    address.index >= 0
                    && static_cast<::std::size_t>(address.index) < m_page_size
                    && m_page_size * static_cast<::std::size_t>(-address.layer - 1)
                    + static_cast<::std::size_t>(address.index) < m_buffer.size()
                ) {
                    return m_buffer.cbegin()
                        + m_page_size * static_cast<::std::size_t>(-address.layer - 1)
                        + static_cast<::std::size_t>(address.index);
                }
                else {
                    return m_buffer.cend();
                };
            }
            else {
                return m_buffer.cend();
            };
        };
    public:
        inline virtual typename traitset::input_neuron_type* get(const ::uns::nn::address& address) override {
            auto buffer_cell = find(address);

            if (buffer_cell != m_buffer.cend()) {
                return new typename traitset::input_neuron_type{
                    address,
                    *buffer_cell
                };
            }
            else {
                return nullptr;
            };
        };
        inline virtual typename traitset::iterator_type begin() const override {
            return {
                0,
                m_actual_size,
                m_page_size
            };
        };
        inline virtual typename traitset::iterator_type end() const override {
            return {
                m_actual_size,
                m_actual_size,
                m_page_size
            };
        };
    public:
        inline void set(
            const ::uns::nn::address& IntputAddress,
            typename base::input_traitset::signal_traitset::signal_type Value
        ) {
            auto buffer_cell = find(IntputAddress);

            if (buffer_cell != m_buffer.cend()) {
                *(m_buffer.begin() + (buffer_cell - m_buffer.cbegin())) = Value;
            }
            else {
                throw ::std::out_of_range{
                    "Requested address ("
                    + ::uns::string::cast<::std::string>(IntputAddress.layer)
                    + ", "
                    + ::uns::string::cast<::std::string>(IntputAddress.index)
                    + ") does not fit the 'actual size' = "
                    + ::uns::string::cast<::std::string>(m_actual_size)
                };
            };
        };
    };


    using traitset_network = ::uns::nn::traitset::network<
        ::uns::nn::sequential_neuron<::uns::tests::traitset_neuron>
        , ::uns::nn::description::network<
            ::uns::nn::description::neuron<
                ::uns::tests::signal
            >
        >
        , ::uns::tests::object
    >;


    class nn_environment: public ::testing::Test
    {
    public:
        using ido_buffer = ::std::vector<
            typename ::uns::tests::signal::signal_type
        >;
    public:
        ::uns::tests::object ido;
        ::uns::tests::activator_caster activator_caster;
        ::uns::tests::collector_caster collector_caster;
    public:
        typename ::uns::tests::signal::signal_type n00 = 0;
        typename ::uns::tests::signal::signal_type n01 = 0;
        typename ::uns::tests::signal::signal_type n10 = 0;
    public:
        const ::std::vector<::uns::tests::signal::weight_type> n00_weights = {
                1.0
                , 0.5
                , -1.0
                , -2.0
        };
        const ::std::vector<::uns::tests::signal::weight_type> n01_weights = {
                1.0
                , 3.0
                , -1.0
                , -1.0
        };
        const ::std::vector<::uns::tests::signal::weight_type> n10_weights = {
                1.0
                , 2.0
        };
    public:
        const ::std::vector<::uns::tests::signal::params_type> n00_params = {
                1.0
                , 0.0
                , 0.0
        };
        const ::std::vector<::uns::tests::signal::params_type> n01_params = {
                -1.0
                , 0.0
                , 0.0
        };
        const ::std::vector<::uns::tests::signal::params_type> n10_params = {
                1.0
                , -1.0
                , 0.0
        };
    public:
        ::uns::nn::description::network<
            ::uns::nn::description::neuron<
                ::uns::tests::signal
            >
        > make_nn_description() const {
            auto repr = ::uns::nn::description::network<
                ::uns::nn::description::neuron<
                    ::uns::tests::signal
                >
            >{};

            repr.layers.push_back(::std::vector<::uns::nn::description::neuron<::uns::tests::signal>>{});

            repr.layers.back().push_back(::uns::nn::description::neuron<::uns::tests::signal>{});
            repr.layers.back().back().activator.hashsum = ::uns::tests::hashsum::line_activator;
            repr.layers.back().back().collector.hashsum = ::uns::tests::hashsum::perc_collector;
            repr.layers.back().back().r = 0;
            repr.layers.back().back().c = 0;
            repr.layers.back().back().links = {
                { { -1, 0 }, n00_weights[0] }
                , { { -1, 1 }, n00_weights[1] }
                , { { -1, 2 }, n00_weights[2] }
                , { { -1, 3 }, n00_weights[3] }
            };
            repr.layers.back().back().params = n00_params;

            repr.layers.back().push_back(::uns::nn::description::neuron<::uns::tests::signal>{});
            repr.layers.back().back().activator.hashsum = ::uns::tests::hashsum::line_activator;
            repr.layers.back().back().collector.hashsum = ::uns::tests::hashsum::perc_collector;
            repr.layers.back().back().r = 0;
            repr.layers.back().back().c = 0;
            repr.layers.back().back().links = {
                { { -1, 0 }, n01_weights[0] }
                , { { -1, 1 }, n01_weights[1] }
                , { { -1, 2 }, n01_weights[2] }
                , { { -1, 3 }, n01_weights[3] }
            };
            repr.layers.back().back().params = n01_params;

            repr.layers.push_back(::std::vector<::uns::nn::description::neuron<::uns::tests::signal>>{});

            repr.layers.back().push_back(::uns::nn::description::neuron<::uns::tests::signal>{});
            repr.layers.back().back().activator.hashsum = ::uns::tests::hashsum::line_activator;
            repr.layers.back().back().collector.hashsum = ::uns::tests::hashsum::perc_collector;
            repr.layers.back().back().r = 0;
            repr.layers.back().back().c = 0;
            repr.layers.back().back().links = {
                { { 0, 0 }, n10_weights[0] }
                , { { 0, 1 }, n10_weights[1] }
            };
            repr.layers.back().back().params = n10_params;

            repr.outputs.push_back(::uns::nn::address{ 1, 0 });

            return repr;
        };
    public:
        typename ::uns::tests::signal::signal_type predict_output(const ido_buffer& Buffer) {
            n00 = 0;
            n01 = 0;
            for (::std::size_t buffer_cell_idx = 0; buffer_cell_idx < Buffer.size(); ++buffer_cell_idx) {
                n00 += n00_weights[buffer_cell_idx] * Buffer[buffer_cell_idx];
                n01 += n01_weights[buffer_cell_idx] * Buffer[buffer_cell_idx];
            };
            n00 = n00_params[0] * n00 + n00_params[1] + n00_params[2];
            n01 = n01_params[0] * n01 + n01_params[1] + n01_params[2];

            n10 = 0;
            n10 += n10_weights[0] * n00;
            n10 += n10_weights[1] * n01;
            n10 = n10_params[0] * n10 + n10_params[1] + n10_params[2];

            return n10;
        };
        void set_ido(const ido_buffer& Buffer) {
            auto buffer_cell_addres = ::uns::nn::address{
                .layer = -1
                , .index = 0
            };

            for (const auto& buffer_cell_value : Buffer) {
                ido.set(
                    buffer_cell_addres
                    , buffer_cell_value
                );

                ++buffer_cell_addres.index;
            };
        };
    };


};


class DirectPropagation :
    public ::uns::tests::nn_environment
    , public ::testing::WithParamInterface<::uns::tests::nn_environment::ido_buffer>
{
private:
    using base = ::uns::tests::nn_environment;
    using param_set = ::uns::tests::nn_environment::ido_buffer;
public:
    static ::std::vector<base::ido_buffer> generate_tests() {
        auto single_cell_data_flow = ::std::array<
            typename ::uns::tests::signal::signal_type
            , ::uns::tests::data_buffer_capacity
        >{};
        typename ::uns::tests::signal::signal_type single_cell = -1.8;
        for (auto& single_cell_data : single_cell_data_flow) {
            single_cell += 0.3;
            single_cell_data = single_cell;
        };

        auto result = ::std::vector<base::ido_buffer>{};
        for (const auto& single_cell_data0 : single_cell_data_flow) {
            for (const auto& single_cell_data1 : single_cell_data_flow) {
                for (const auto& single_cell_data2 : single_cell_data_flow) {
                    for (const auto& single_cell_data3 : single_cell_data_flow) {
                        const auto single_test_data = base::ido_buffer{
                            single_cell_data0
                            , single_cell_data1
                            , single_cell_data2
                            , single_cell_data3
                        };

                        result.push_back(single_test_data);
                    };
                };
            };
        };

        return result;
    };

};


template<>
::std::string testing::PrintToString(const ::DirectPropagation::param_set& Params) {
    ::std::string result = "[ ";

    for (const auto& ido_cell_value : Params) {
        result += testing::PrintToString(ido_cell_value);
        result += ", ";
    };
    result += "]";

    return result;
};


TEST_P(DirectPropagation, SequentialNetwork) {
    auto network = ::uns::nn::sequential_network<
        ::uns::nn::traitset::network<
            ::uns::nn::sequential_neuron<::uns::tests::traitset_neuron>
            , ::uns::nn::description::network<
                ::uns::nn::description::neuron<
                    ::uns::tests::signal
                >
            >
            , ::uns::tests::object
        >
    >{};

    network.set(
        make_nn_description()
        , ido
    );

    set_ido(GetParam());

    network.react({});
    ASSERT_FLOAT_EQ(
        predict_output(GetParam())
        , network.O(0)
    );
};
TEST_P(DirectPropagation, NonrecursiveReversiveNetwork) {
    auto network = ::uns::nn::nonrecursive_reversive_network<
        ::uns::nn::traitset::network<
            ::uns::nn::nonrecursive_reversive_neuron<::uns::tests::traitset_neuron>
            , ::uns::nn::description::network<
                ::uns::nn::description::neuron<
                    ::uns::tests::signal
                >
            >
            , ::uns::tests::object
        >
    >{};

    network.set(
        make_nn_description()
        , ido
    );
    network._link();

    set_ido(GetParam());

    network.react({});
    ASSERT_FLOAT_EQ(
        predict_output(GetParam())
        , network.O(0)
    );
};
INSTANTIATE_TEST_CASE_P(NnBaseTests, DirectPropagation,
    ::testing::ValuesIn(
        ::DirectPropagation::generate_tests()
    )
);


namespace uns::tests::param_set {

    struct RevertPropagation {
        ::uns::tests::signal::signal_type reversive_input = 0;
        ::uns::tests::nn_environment::ido_buffer ido_buffer;
    };

};


class RevertPropagation :
    public ::uns::tests::nn_environment
    , public ::testing::WithParamInterface<::uns::tests::param_set::RevertPropagation>
{
private:
    using base = ::uns::tests::nn_environment;
    using param_set = ::uns::tests::param_set::RevertPropagation;
public:
    struct neuron_predicted_values {
        ::uns::tests::signal::signal_type _C = 0;
        ::uns::tests::signal::signal_type _R = 0;
    };
public:
    ::std::vector<neuron_predicted_values> predict_reversive_values() const {
        typename ::uns::tests::signal::signal_type n00 = 0;
        typename ::uns::tests::signal::signal_type n01 = 0;
        for (::std::size_t buffer_cell_idx = 0; buffer_cell_idx < Buffer.size(); ++buffer_cell_idx) {
            n00 += n00_weights[buffer_cell_idx] * Buffer[buffer_cell_idx];
            n01 += n01_weights[buffer_cell_idx] * Buffer[buffer_cell_idx];
        };
        n00 = n00_params[0] * n00 + n00_params[1] + n00_params[2];
        n01 = n01_params[0] * n01 + n01_params[1] + n01_params[2];

        typename ::uns::tests::signal::signal_type n10 = 0;
        n10 += n10_weights[0] * n00;
        n10 += n10_weights[1] * n01;
        n10 = n10_params[0] * n10 + n10_params[1] + n10_params[2];

        return n10;
    };
    template<typename network_t>
    void set_odo(
        network_t& Network
        , const param_set& Params
    ) const {
        Network.access(Network.O(0))._C() = Params.reversive_input;
    };
public:
    static ::std::vector<param_set> generate_tests() {
        auto test_ido_buffers = ::DirectPropagation::generate_tests();

        auto result = ::std::vector<param_set>{};

        int phase_marker = 0;
        for (const auto& test_ido_buffer : test_ido_buffers) {
            result.push_back(
                param_set{
                    .reversive_input = phase_marker * 0.1L
                    , .ido_buffer = test_ido_buffer
                }
            );

            ++phase_marker;
            phase_marker %= 7;
        };

        return result;
    };
};


template<>
::std::string testing::PrintToString(const ::RevertPropagation::param_set& Params) {
    ::std::string result = "{ ";

    result += testing::PrintToString(Params.reversive_input);
    result += ", ";
    result += testing::PrintToString(Params.ido_buffer);

    result += " }";

    return result;
};


TEST_P(RevertPropagation, NonrecursiveReversiveNetwork) {
    auto network = ::uns::nn::nonrecursive_reversive_network<
        ::uns::nn::traitset::network<
            ::uns::nn::nonrecursive_reversive_neuron<::uns::tests::traitset_neuron>
            , ::uns::nn::description::network<
                ::uns::nn::description::neuron<
                    ::uns::tests::signal
                >
            >
            , ::uns::tests::object
        >
    >{};

    network.set(
        make_nn_description()
        , ido
    );
    network._link();

    set_ido(GetParam().ido_buffer);

    network.react({});
    EXPECT_FLOAT_EQ(
        predict_output(GetParam().ido_buffer)
        , network.O(0)
    );

    set_odo(network, GetParam());
    auto neurons_reversive_values = predict_reversive_values();
    ASSERT_FLOAT_EQ(
        neurons_reversive_values[0]._C
        , network.access(
            {
                .layer = 0
                , .index = 0
            }
        )._C()
    );
    ASSERT_FLOAT_EQ(
        neurons_reversive_values[0]._R
        , network.access(
            {
                .layer = 0
                , .index = 0
            }
        )._R()
    );
    ASSERT_FLOAT_EQ(
        neurons_reversive_values[1]._C
        , network.access(
            {
                .layer = 0
                , .index = 1
            }
        )._C()
    );
    ASSERT_FLOAT_EQ(
        neurons_reversive_values[1]._R
        , network.access(
            {
                .layer = 0
                , .index = 1
            }
        )._R()
    );
    ASSERT_FLOAT_EQ(
        neurons_reversive_values[2]._C
        , network.access(
            {
                .layer = 1
                , .index = 0
            }
        )._C()
    );
    ASSERT_FLOAT_EQ(
        neurons_reversive_values[2]._R
        , network.access(
            {
                .layer = 1
                , .index = 0
            }
        )._R()
    );
};
INSTANTIATE_TEST_CASE_P(NnBaseTests, RevertPropagation,
    ::testing::ValuesIn(
        ::RevertPropagation::generate_tests()
    )
);
