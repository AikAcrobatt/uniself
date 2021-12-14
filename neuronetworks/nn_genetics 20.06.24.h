#pragma once

#include "uniself/neuronetworks/nn_general.h"
#include "uniself/neuronetworks/nn_networks.h"
#include "uniself/neuronetworks/nn_neurons.h"

#define UNS_FILENAME "nn_genetics.h"

namespace uns {
    namespace nn {
        namespace genetic {

            //СЛУЖЕБНЫЕ ПЕРЕЧИСЛЕНИЯ
            //Типы ограниченности переменных при вариации
            UNS_ENUM_DECLARATOR(VARIATION_LIMITS_TYPE, int,
                left,
                right
            );

            enum data_gene_values {
                DATATYPE = 0,
                DATA
            };

            enum general_gene_values {
                NEYROTYPE = 0,
                ADRESS,
                NUMBER,
                OUT,
                CORE1,
                CORE2
            };

            using signal_t = float;
            using dominancy_t = float;
            using prob_t = float;
            class adress;
            template<typename data_t> class simple_gene;
            class datatype_gene;
            class data_gene;
            class neurotype_gene;
            class adress_gene;
            class core_gene;
            using data_slot_t = std::tuple<datatype_gene, data_gene>;
            using general_slot_t = std::tuple<neurotype_gene, adress_gene, simple_gene<size_t>, simple_gene<int>, core_gene, core_gene>;
            class chromosome;



            //Типы соединений слоев
            UNS_ENUM_DECLARATOR(layer_connection_type, size_t,
                ToNoone = 0, //ни к одному
                OneToEach = 1, //один к каждому
                AllToEach = 2, //все к каждому
                SomeToEach = 3, //несколько к каждому
                Single = 4, //только один к каждому
                Chain = 5 //соединение по цепочке
            );
        };
    };
};

UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::layer_connection_type);

namespace uns {
    namespace nn {
        namespace genetic {


            //Вспомогательный класс слотового адреса:
            class adress {
            protected:
                uns::nn::genetic::layer_connection_type conn = layer_connection_type::ToNoone;
                int group_id = 0;
                size_t slot_id = 0;
                data_slot_t* data_slot = nullptr;
                general_slot_t* general_slot = nullptr;
            public:
                adress() noexcept {};
                adress(uns::nn::genetic::layer_connection_type set_conn, int set_group_id, size_t set_slot_id) noexcept : conn(set_conn), group_id(set_group_id), slot_id(set_slot_id) { Format(); };
                adress(const adress& moving_obj) noexcept { *this = moving_obj; };
                adress& operator=(const adress& copying_obj) noexcept { conn = copying_obj.conn; group_id = copying_obj.group_id; slot_id = copying_obj.slot_id; data_slot = nullptr; general_slot = nullptr; Format(); return *this; };
                adress(adress&& moving_obj) noexcept { *this = std::move(moving_obj); };
                adress& operator=(adress&& moving_obj) noexcept { conn = std::move(moving_obj.conn); group_id = std::move(moving_obj.group_id); slot_id = std::move(moving_obj.slot_id); data_slot = nullptr; general_slot = nullptr; Format(); return *this; };
                adress& operator=(data_slot_t* data_slot_ptr) noexcept {
                    data_slot = data_slot_ptr;
                    if (data_slot != nullptr) general_slot = nullptr;
                    return *this;
                };
                adress& operator=(general_slot_t* general_slot_ptr) noexcept {
                    general_slot = general_slot_ptr;
                    if (general_slot != nullptr)  data_slot = nullptr;
                    return *this;
                };
                ~adress() noexcept {};

                uns::nn::string_t String() const noexcept {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");

                    return uns::string_cast<uns::nn::string_t>(conn) + comma +
                        uns::string_cast<uns::nn::string_t>(group_id < -1 ? -1 : group_id) + comma +
                        uns::string_cast<uns::nn::string_t>(slot_id);
                };

                bool Set(const uns::nn::string_t& str) noexcept {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");

                    uns::nn::string_t::size_type seeker = 0;

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, comma, false, -1)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else conn = uns::string_cast<uns::nn::genetic::layer_connection_type>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, comma, false, -1)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else group_id = uns::string_cast<int>(temp);
                    slot_id = uns::string_cast<size_t>(str.substr(seeker));

                    data_slot = nullptr;
                    general_slot = nullptr;

                    Format();

                    return true;
                };

                void Format() noexcept {
                    if (group_id < -1) group_id = -1;

                    if (general_slot != nullptr && data_slot != nullptr) data_slot = nullptr;
                };

                bool IsExcited() const noexcept { return (general_slot != nullptr || data_slot != nullptr); };

                uns::nn::genetic::layer_connection_type Connection() const noexcept { return conn; };
                uns::nn::genetic::layer_connection_type& Connection() noexcept { return conn; };

                int Group() const noexcept { return group_id; };
                int& Group() noexcept { return group_id; };

                size_t Slot() const noexcept { return slot_id; };
                size_t& Slot() noexcept { return slot_id; };

                bool operator==(data_slot_t* data_slot_ptr) const noexcept { return data_slot != nullptr && data_slot == data_slot_ptr; };
                bool operator==(general_slot_t* general_slot_ptr) const noexcept { return general_slot != nullptr && general_slot == general_slot_ptr; };
                bool operator==(const adress& comparing_adress) const noexcept {
                    if (IsExcited() && comparing_adress.IsExcited())
                        return (general_slot == comparing_adress.general_slot && data_slot == comparing_adress.data_slot);

                    return (group_id == comparing_adress.group_id && slot_id == comparing_adress.slot_id);
                };

                template<typename compared_t>
                bool operator!=(compared_t compared_arg) const noexcept { return !(*this == compared_arg); };

            };
            //:Вспомогательный класс слотового адреса


            //Базовый ген:
            template<typename data_t>
            class base_gene {
            public:
                using value_t = data_t;
            protected:
                bool block = false;
                uns::nn::genetic::dominancy_t dominant = 0.0F;
                data_t data;
            public:
                base_gene() : block(false), dominant(0.0F), data() {};
                base_gene(const base_gene<data_t>& copying_obj) : block(copying_obj.block), dominant(copying_obj.dominant), data(copying_obj.data) {};
                base_gene<data_t>& operator=(const base_gene<data_t>& copying_obj) { block = copying_obj.block; dominant = copying_obj.dominant; data = copying_obj.data; return *this; };
                base_gene(base_gene<data_t>&& moving_obj) : block(moving_obj.block), dominant(moving_obj.dominant), data(moving_obj.data) {};
                base_gene<data_t>& operator=(base_gene<data_t>&& moving_obj) { block = moving_obj.block; dominant = moving_obj.dominant; data = std::move(moving_obj.data); return *this; };
                ~base_gene() {};

                uns::nn::string_t String() const {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");
                    const static auto l_bracket = uns::string_cast<uns::nn::string_t>("[");
                    const static auto r_bracket = uns::string_cast<uns::nn::string_t>("]");
                    const static auto l_parent = uns::string_cast<uns::nn::string_t>("(");
                    const static auto r_parent = uns::string_cast<uns::nn::string_t>(")");
                    return l_bracket + uns::string_cast<uns::nn::string_t>(block) + comma + uns::string_cast<uns::nn::string_t>(dominant) + l_parent + ValueString(data) + r_parent + r_bracket;
                };

                bool Set(const uns::nn::string_t& str) {
                    uns::nn::string_t::size_type seeker = 0;
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");
                    const static auto l_bracket = uns::string_cast<uns::nn::string_t>("[");
                    const static auto r_bracket = uns::string_cast<uns::nn::string_t>("]");
                    const static auto l_parent = uns::string_cast<uns::nn::string_t>("(");
                    const static auto r_parent = uns::string_cast<uns::nn::string_t>(")");

                    if (!uns::string::SeekerSet(str, seeker, l_bracket, false, -1, r_parent + r_bracket)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, r_parent + r_bracket) == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, comma, false, -1, r_parent + r_bracket)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else block = uns::string_cast<bool>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, l_parent, false, -1, r_parent + r_bracket)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else dominant = uns::string_cast<uns::nn::genetic::dominancy_t>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, r_parent, false, 0, r_parent + r_bracket)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!ValueSet(data, temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };
            protected:
                virtual uns::nn::string_t ValueString(const data_t& stringifying_data) const = 0;

                virtual bool ValueSet(data_t& data_to_set, const uns::nn::string_t& str) = 0;

            public:

                void Clear(const data_t& empty_data) {
                    block = false;
                    dominant = 0.0F;
                    data = empty_data;
                };

                bool& Block() noexcept { return block; };
                bool Block() const noexcept { return block; };

                uns::nn::genetic::dominancy_t& Dominant() noexcept { return dominant; };
                uns::nn::genetic::dominancy_t Dominant() const noexcept { return dominant; };

                data_t& Value() noexcept { return data; };
                data_t Value() const noexcept { return data; };
            };
            //:Базовый ген


            //Конкретные гены:
            template<typename data_t>
            class simple_gene : public uns::nn::genetic::base_gene<data_t> {
            protected:
                virtual uns::nn::string_t ValueString(const data_t& stringifying_data) const override {
                    return uns::string_cast<uns::nn::string_t>(stringifying_data);
                };

                virtual bool ValueSet(data_t& data_to_set, const uns::nn::string_t& str) override {
                    data_to_set = uns::string_cast<data_t>(str);

                    return true;
                };
            };


            class datatype_gene : public uns::nn::genetic::base_gene<size_t> {
            protected:
                virtual uns::nn::string_t ValueString(const size_t& stringifying_data) const override {
                    return ctx[CTX_CCASCADES::Frame].Id(stringifying_data);
                };

                virtual bool ValueSet(size_t& data_to_set, const uns::nn::string_t& str) override {
                    data_to_set = ctx[CTX_CCASCADES::Frame].Find(str);

                    return (data_to_set < ctx[CTX_CCASCADES::Frame].Total());
                };
            };


            class neurotype_gene : public uns::nn::genetic::base_gene<size_t> {
            protected:
                virtual uns::nn::string_t ValueString(const size_t& stringifying_data) const override {
                    return ctx[CTX_CCASCADES::Neuro].Id(stringifying_data);
                };

                virtual bool ValueSet(size_t& data_to_set, const uns::nn::string_t& str) override {
                    data_to_set = ctx[CTX_CCASCADES::Neuro].Find(str);

                    return (data_to_set < ctx[CTX_CCASCADES::Frame].Total());
                };
            };


            class data_gene : public uns::nn::genetic::base_gene<std::pair<size_t, size_t>> {
            protected:
                virtual uns::nn::string_t ValueString(const std::pair<size_t, size_t>& stringifying_data) const override {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");

                    return uns::string_cast<uns::nn::string_t>(stringifying_data.first) + comma + uns::string_cast<uns::nn::string_t>(stringifying_data.second);
                };

                virtual bool ValueSet(std::pair<size_t, size_t>& data_to_set, const uns::nn::string_t& str) override {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");

                    uns::nn::string_t::size_type seeker = 0;

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, comma, false, -1)) return false;
                    else data_to_set.first = uns::string_cast<size_t>(temp);
                    data_to_set.second = uns::string_cast<size_t>(str.substr(seeker));

                    return true;
                };
            };


            class adress_gene : public uns::nn::genetic::base_gene<std::vector<adress>> {
            protected:
                virtual uns::nn::string_t ValueString(const std::vector<adress>& stringifying_data) const override {
                    const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");

                    uns::nn::string_t res = uns::nn::string_t();

                    for (auto one_adress : stringifying_data)
                        res += one_adress.String() + semicolon;

                    return res;
                };

                virtual bool ValueSet(std::vector<adress>& data_to_set, const uns::nn::string_t& str) override {
                    const static auto semicolon = uns::string_cast<uns::nn::string_t>(";");

                    uns::nn::string_t::size_type seeker = 0;

                    data_to_set.clear();
                    uns::nn::string_t temp = uns::nn::string_t();
                    while (uns::string::SeekerRead(str, seeker, temp, semicolon, false, -1)) {
                        data_to_set.push_back(adress());
                        if (!data_to_set[data_to_set.size() - 1].Set(temp)) return false;
                    };

                    return true;
                };
            };


            class core_gene : public uns::nn::genetic::base_gene<ublas::matrix<uns::nn::genetic::signal_t>> {
            protected:
                virtual uns::nn::string_t ValueString(const ublas::matrix<uns::nn::genetic::signal_t>& stringifying_data) const override {
                    return uns::math::matrix_to_string<uns::nn::string_t>(stringifying_data);
                };

                virtual bool ValueSet(ublas::matrix<uns::nn::genetic::signal_t>& data_to_set, const uns::nn::string_t& str) override {
                    return uns::math::string_to_matrix(data_to_set, str);
                };
            };
            //:Конкретные гены


            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Real_chars, size_t,
                MUTATION_CHROMOSOME = 0,                //вероятность мутации для всей хромосомы
                MUTATION_GENE = 1,                       //вероятность мутации для каждого гена
                RANDOM_MUTATION,
                UNLIMITMUTATION_MODIFICATOR,
                BLOCK_MODIFICATOR,
                PARAMETER_INVERTION,
                PERCENT_MODIFICATOR,
                MFACTOR
            );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Int_chars, size_t,
                int_0 = 0,
                int_1 = 1
            );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Long_chars, size_t,
                long_0 = 0,
                long_1 = 1
            );


            //Группа генов характеристик:
            template<typename char_t, typename enum_t>
            class char_group {
            public:
                using value_type = char_t;
                using enum_type = enum_t;
            protected:
                std::array<uns::nn::genetic::simple_gene<char_t>, enum_t::_size()> genes;
            public:
                char_group() noexcept {};
                char_group(const char_group& copying_obj) noexcept : genes(copying_obj.genes) {};
                char_group& operator=(const char_group& copying_obj) noexcept { genes = copying_obj.genes; return *this; };
                char_group(char_group&& moving_obj) noexcept : genes(std::move(moving_obj.genes)) {};
                char_group& operator=(char_group&& moving_obj) noexcept { genes = std::move(moving_obj.genes); return *this; };
                ~char_group() noexcept { Clear(); };

                uns::nn::string_t String() const {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto tab = uns::string_cast<uns::nn::string_t>("\t");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto CHAR_GROUP = GroupName();

                    auto res = CHAR_GROUP + colon + delimiter;
                    for (auto gene : genes)
                        res += tab + gene.String() + delimiter;
                    res += colon + CHAR_GROUP;

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto CHAR_GROUP = GroupName();

                    uns::nn::string_t::size_type seeker = 0;
                    uns::nn::string_t temp = uns::nn::string_t();

                    if (!uns::string::SeekerSet(str, seeker, CHAR_GROUP + colon + delimiter, false, -1, colon + CHAR_GROUP)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    const auto char_group = uns::string::SeekerFind(str, seeker, colon + CHAR_GROUP);
                    if (char_group == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    for (auto& gene : genes) {
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, char_group)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                        else if (!gene.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    };

                    return true;
                };

                void Format(const uns::nn::genetic::chromosome& nch) {
                    for (auto enum_val : enum_t::_values) {
                        if (IsLimited(enum_val, VARIATION_LIMITS_TYPE::left, nch) && uns::math::Less(genes[enum_val], Limit(enum_val, VARIATION_LIMITS_TYPE::left, nch)))
                            genes[enum_val] = Limit(enum_val, VARIATION_LIMITS_TYPE::left, nch);
                        if (IsLimited(enum_val, VARIATION_LIMITS_TYPE::right, nch) && uns::math::Less(genes[enum_val], Limit(enum_val, VARIATION_LIMITS_TYPE::right, nch)))
                            genes[enum_val] = Limit(enum_val, VARIATION_LIMITS_TYPE::right, nch);
                    };
                };

                void Clear() noexcept {};

                /*auto begin() { return genes.begin(); };

                auto end() { return genes.end(); };

                auto cbegin() { return genes.cbegin(); };

                auto cend() { return genes.cend(); };*/

                auto Gene(const enum_t& enum_val) const { return genes[enum_val]; };
                auto Gene(const enum_t& enum_val) { return genes[enum_val]; };

                virtual uns::nn::string_t GroupName() const = 0;

                virtual bool IsLimited(const enum_t& enum_val, const VARIATION_LIMITS_TYPE& limit_Type, const uns::nn::genetic::chromosome& nch) const = 0;

                virtual char_t Limit(const enum_t& enum_val, const VARIATION_LIMITS_TYPE& limit_Type, const uns::nn::genetic::chromosome& nch) const = 0;
            };
            //:Группа генов характеристик


            //Производные группы генов характеристик:
            class real_char_group : public char_group<float, uns::nn::genetic::Real_chars> {
                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("FLOAT CHAR GROUP"); };

                virtual bool IsLimited(const uns::nn::genetic::Real_chars& enum_val, const VARIATION_LIMITS_TYPE& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        switch (limit_type) {
                        case VARIATION_LIMITS_TYPE::left: return false;
                        case VARIATION_LIMITS_TYPE::right: return false;
                        };
                    };
                };

                virtual float Limit(const uns::nn::genetic::Real_chars& enum_val, const VARIATION_LIMITS_TYPE& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        switch (limit_type) {
                        case VARIATION_LIMITS_TYPE::left: return 0.0F;
                        case VARIATION_LIMITS_TYPE::right: return 0.0F;
                        };
                    };
                };
            };

            class int_char_group : public char_group<int, uns::nn::genetic::Int_chars> {
                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("INT CHAR GROUP"); };

                virtual bool IsLimited(const uns::nn::genetic::Int_chars& enum_val, const VARIATION_LIMITS_TYPE& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        switch (limit_type) {
                        case VARIATION_LIMITS_TYPE::left: return false;
                        case VARIATION_LIMITS_TYPE::right: return false;
                        };
                    };
                };

                virtual int Limit(const uns::nn::genetic::Int_chars& enum_val, const VARIATION_LIMITS_TYPE& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        switch (limit_type) {
                        case VARIATION_LIMITS_TYPE::left: return 0;
                        case VARIATION_LIMITS_TYPE::right: return 0;
                        };
                    };
                };
            };

            class long_char_group : public char_group<long long int, uns::nn::genetic::Long_chars> {
                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("LONG CHAR GROUP"); };

                virtual bool IsLimited(const uns::nn::genetic::Long_chars& enum_val, const VARIATION_LIMITS_TYPE& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        switch (limit_type) {
                        case VARIATION_LIMITS_TYPE::left: return false;
                        case VARIATION_LIMITS_TYPE::right: return false;
                        };
                    };
                };

                virtual long long int Limit(const uns::nn::genetic::Long_chars& enum_val, const VARIATION_LIMITS_TYPE& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        switch (limit_type) {
                        case VARIATION_LIMITS_TYPE::left: return 0;
                        case VARIATION_LIMITS_TYPE::right: return 0;
                        };
                    };
                };
            };
            //:Производные группы генов характеристик


            //Группа генов входных данных:
            class data_group {
            protected:
                std::vector<std::tuple<datatype_gene, data_gene>> slots;
            public:
                data_group() noexcept {};
                data_group(const data_group& copying_obj) noexcept : slots(copying_obj.slots) {};
                data_group& operator=(const data_group& copying_obj) noexcept { slots = copying_obj.slots; return *this; };
                data_group(data_group&& moving_obj) noexcept : slots(std::move(moving_obj.slots)) {};
                data_group& operator=(data_group&& moving_obj) noexcept { slots = std::move(moving_obj.slots); return *this; };
                ~data_group() noexcept { /*Clear();*/ };

                uns::nn::string_t String() const {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto tab = uns::string_cast<uns::nn::string_t>("\t");
                    const static auto space = uns::string_cast<uns::nn::string_t>(" ");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto DATA_GROUP = GroupName();

                    auto res = DATA_GROUP + colon + delimiter;
                    for (auto slot : slots)
                        res += tab +
                        std::get<data_gene_values::DATATYPE>(slot).String() + space +
                        std::get<data_gene_values::DATA>(slot).String() + delimiter;
                    res += colon + DATA_GROUP;

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto space = uns::string_cast<uns::nn::string_t>(" ");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto DATA_GROUP = GroupName();

                    uns::nn::string_t::size_type seeker = 0;
                    uns::nn::string_t temp = uns::nn::string_t();

                    if (!uns::string::SeekerSet(str, seeker, DATA_GROUP + colon + delimiter, false, -1, colon + DATA_GROUP)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    const auto data_group = uns::string::SeekerFind(str, seeker, colon + DATA_GROUP);
                    if (data_group == uns::nn::string_t::npos || seeker >= data_group) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    auto next_data_slot = data_slot_t();
                    do {
                        next_data_slot = data_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > data_group) break;
                        else if (!std::get<data_gene_values::DATATYPE>(next_data_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, colon + DATA_GROUP) || seeker > data_group) break;
                        else if (!std::get<data_gene_values::DATA>(next_data_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        slots.push_back(next_data_slot);
                    }
                    while (seeker < data_group);

                    return true;
                };

                void Format() {
                    for (auto& slot : slots) {
                        if (ctx[CTX_CCASCADES::Frame].Total() == 0) return;

                        if (std::get<data_gene_values::DATATYPE>(slot).Value() >= ctx[CTX_CCASCADES::Frame].Total())
                            std::get<data_gene_values::DATATYPE>(slot).Value() = 0;

                        if (std::get<data_gene_values::DATA>(slot).Value().second > ctx[CTX_CCASCADES::Frame].Width(std::get<data_gene_values::DATATYPE>(slot).Value()))
                            std::get<data_gene_values::DATA>(slot).Value().second = ctx[CTX_CCASCADES::Frame].Width(std::get<data_gene_values::DATATYPE>(slot).Value());

                        if (std::get<data_gene_values::DATA>(slot).Value().first > std::get<data_gene_values::DATA>(slot).Value().second)
                            std::get<data_gene_values::DATA>(slot).Value().first = std::get<data_gene_values::DATA>(slot).Value().second;
                    };
                };

                //void Clear() noexcept {};

                /*auto begin() { return slots.begin(); };

                auto end() { return slots.end(); };

                auto cbegin() { return slots.cbegin(); };

                auto cend() { return slots.cend(); };*/

                template<data_gene_values enum_val> auto Gene(const size_t& slot_num) const { return std::get<enum_val>(slots[slot_num]); };
                template<data_gene_values enum_val> auto Gene(const size_t& slot_num) { return std::get<enum_val>(slots[slot_num]); };

                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("DATA GROUP"); };
            };
            //:Группа генов входных данных


            //Группа основных данных:
            class general_group {
            protected:
                std::vector<uns::nn::genetic::general_slot_t> slots;
            public:
                general_group() noexcept {};
                general_group(const general_group& copying_obj) noexcept : slots(copying_obj.slots) {};
                general_group& operator=(const general_group& copying_obj) noexcept { slots = copying_obj.slots; return *this; };
                general_group(general_group&& moving_obj) noexcept : slots(std::move(moving_obj.slots)) {};
                general_group& operator=(general_group&& moving_obj) noexcept { slots = std::move(moving_obj.slots); return *this; };
                ~general_group() noexcept { Clear(); };

                uns::nn::string_t String() const {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto tab = uns::string_cast<uns::nn::string_t>("\t");
                    const static auto space = uns::string_cast<uns::nn::string_t>(" ");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto GENERAL_GROUP = GroupName();

                    auto res = GENERAL_GROUP + colon + delimiter;
                    for (auto slot : slots) {
                        res += tab +
                            std::get<general_gene_values::NEYROTYPE>(slot).String() + space +
                            std::get<general_gene_values::NUMBER>(slot).String() + space +
                            std::get<general_gene_values::OUT>(slot).String() + space +
                            std::get<general_gene_values::ADRESS>(slot).String() + space +
                            std::get<general_gene_values::CORE1>(slot).String() + space +
                            std::get<general_gene_values::CORE2>(slot).String() + delimiter;
                    };
                    res += colon + GENERAL_GROUP;

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto space = uns::string_cast<uns::nn::string_t>(" ");
                    const static auto GENERAL_GROUP = uns::string_cast<uns::nn::string_t>("GENERAL GROUP");

                    uns::nn::string_t::size_type seeker = 0;
                    uns::nn::string_t temp = uns::nn::string_t();

                    if (!uns::string::SeekerSet(str, seeker, GENERAL_GROUP + colon + delimiter, false, -1, colon + GENERAL_GROUP)) { return false; };
                    const auto general_group = uns::string::SeekerFind(str, seeker, colon + GENERAL_GROUP);
                    if (general_group == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    auto next_slot = general_slot_t();
                    do {
                        next_slot = general_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<general_gene_values::NEYROTYPE>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<general_gene_values::NUMBER>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<general_gene_values::OUT>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<general_gene_values::ADRESS>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<general_gene_values::CORE1>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, colon + GENERAL_GROUP) || seeker > general_group) break;
                        else if (!std::get<general_gene_values::CORE2>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        slots.push_back(next_slot);
                    }
                    while (seeker < general_group);

                    return true;
                };

                void Format() {};

                void Clear() noexcept {};

                auto begin() { return slots.begin(); };

                auto end() { return slots.end(); };

                auto cbegin() { return slots.cbegin(); };

                auto cend() { return slots.cend(); };

                template<general_gene_values enum_val> auto Gene(const size_t& slot_num) const { return std::get<enum_val>(slots[slot_num]); };
                template<general_gene_values enum_val> auto Gene(const size_t& slot_num) { return std::get<enum_val>(slots[slot_num]); };

                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("GENERAL GROUP"); };
            };
            //:Группа основных данных
        };
    };
};

UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Real_chars);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Int_chars);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Long_chars);


namespace uns {
    namespace nn {
        namespace genetic {



            //НЕЙРОХРОМОСОМА
            class chromosome {
            protected:

                uns::nn::genetic::real_char_group real_char_genes;
                uns::nn::genetic::int_char_group int_char_genes;
                uns::nn::genetic::long_char_group long_char_genes;
                uns::nn::genetic::data_group data_slots;
                std::vector<general_group> general_groups;

            public:
                chromosome() {};
                chromosome(const chromosome& copying_obj) :
                    real_char_genes(copying_obj.real_char_genes),
                    int_char_genes(copying_obj.int_char_genes),
                    long_char_genes(copying_obj.long_char_genes),
                    data_slots(copying_obj.data_slots),
                    general_groups(copying_obj.general_groups) {
                };
                chromosome& operator=(const chromosome& copying_obj) {
                    real_char_genes = copying_obj.real_char_genes;
                    int_char_genes = copying_obj.int_char_genes;
                    long_char_genes = copying_obj.long_char_genes;
                    data_slots = copying_obj.data_slots;
                    general_groups = copying_obj.general_groups;
                };
                chromosome(chromosome&& moving_obj) :
                    real_char_genes(std::move(moving_obj.real_char_genes)),
                    int_char_genes(std::move(moving_obj.int_char_genes)),
                    long_char_genes(std::move(moving_obj.long_char_genes)),
                    data_slots(std::move(moving_obj.data_slots)),
                    general_groups(std::move(moving_obj.general_groups)) {
                };
                chromosome& operator=(chromosome&& moving_obj) {
                    real_char_genes = std::move(moving_obj.real_char_genes);
                    int_char_genes = std::move(moving_obj.int_char_genes);
                    long_char_genes = std::move(moving_obj.long_char_genes);
                    data_slots = std::move(moving_obj.data_slots);
                    general_groups = std::move(moving_obj.general_groups);
                };
                ~chromosome() { /*Clear();*/ };

                uns::nn::string_t String() const {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto NEUROCHROMOSOME = uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME");

                    auto res = NEUROCHROMOSOME + colon;

                    res += delimiter + delimiter;
                    res += real_char_genes.String() + delimiter + delimiter;
                    res += int_char_genes.String() + delimiter + delimiter;
                    res += long_char_genes.String() + delimiter + delimiter;

                    res += data_slots.String() + delimiter + delimiter;

                    for (auto general_group : general_groups) {
                        res += general_group.String() + delimiter + delimiter;
                    };

                    res += colon + NEUROCHROMOSOME;

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto NEUROCHROMOSOME = uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME");

                    uns::nn::string_t::size_type seeker = 0;

                    if (!uns::string::SeekerSet(str, seeker, NEUROCHROMOSOME + colon + delimiter + delimiter, false, -1, colon + NEUROCHROMOSOME)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    const auto neurochromosome = uns::string::SeekerFind(str, seeker, colon + NEUROCHROMOSOME);
                    if (neurochromosome == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();

                    if (!uns::string::SeekerRead(str, seeker, temp, delimiter + delimiter, false, -1, neurochromosome)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!real_char_genes.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    if (!uns::string::SeekerRead(str, seeker, temp, delimiter + delimiter, false, -1, neurochromosome)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!int_char_genes.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    if (!uns::string::SeekerRead(str, seeker, temp, delimiter + delimiter, false, -1, neurochromosome)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!long_char_genes.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    if (!uns::string::SeekerRead(str, seeker, temp, delimiter + delimiter, false, -1, neurochromosome)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!data_slots.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    auto next_general_group = general_group();
                    do {
                        next_general_group = general_group();
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter + delimiter, false, -1, neurochromosome)) break;
                        else if (!next_general_group.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        general_groups.push_back(next_general_group);
                    }
                    while (seeker < neurochromosome);
                    if (seeker > neurochromosome) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };

                /*void Format();

                void Clear();

                void RandomFill();*/

                template<class var_t, class randomizer_t> void Variate(var_t& var, bool left_limited, bool right_limited, const var_t& left_limit, const var_t& right_limit, bool block, randomizer_t& rand) {
                    if (left_limited && right_limited) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            real_char_genes.Gene(Real_chars::RANDOM_MUTATION).Value() *
                            real_char_genes.Gene(Real_chars::MFACTOR).Value() *
                            block ? real_char_genes.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            if (rand(real_char_genes.Gene(Real_chars::PARAMETER_INVERTION).Value())) var = right_limit - (var - left_limit);
                            else var = rand(left_limit, right_limit);
                        else var = rand(
                            fmax(fmin(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())), left_limit),
                            fmin(fmax(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())), right_limit)
                        );
                    };
                    if (!left_limited && !right_limited) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            real_char_genes.Gene(Real_chars::RANDOM_MUTATION).Value() *
                            real_char_genes.Gene(Real_chars::MFACTOR).Value() *
                            block ? real_char_genes.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        ))) {
                            if (rand(real_char_genes.Gene(Real_chars::PARAMETER_INVERTION).Value())) var = -var;
                            else {
                                var = (uns::math::Equals(var, 0.0F) ? 1.0F : var);
                                var = uns::math::Abs(var) * static_cast<var_t>(rand(
                                    -(1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())),
                                    1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())
                                ));
                            };
                        }
                        else var = rand(
                            fmin(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())),
                            fmax(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()))
                        );
                    };
                    if (left_limited && !right_limited) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            real_char_genes.Gene(Real_chars::RANDOM_MUTATION).Value() *
                            real_char_genes.Gene(Real_chars::MFACTOR).Value() *
                            block ? real_char_genes.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        ))) {
                            if (rand(real_char_genes.Gene(Real_chars::PARAMETER_INVERTION).Value())) var = uns::math::Abs(var) * static_cast<var_t>(1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())) - (var - left_limit);
                            else {
                                var = (uns::math::Equals(var, 0.0F) ? 1.0F : var);
                                var = rand(
                                    left_limit,
                                    uns::math::Abs(var) * static_cast<var_t>(1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value()))
                                );
                            };
                        }
                        else var = rand(
                            fmax(fmin(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())), left_limit),
                            fmax(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()))
                        );
                    };
                    if (!left_limited && right_limited) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            real_char_genes.Gene(Real_chars::RANDOM_MUTATION).Value() *
                            real_char_genes.Gene(Real_chars::MFACTOR).Value() *
                            block ? real_char_genes.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        ))) {
                            if (rand(real_char_genes.Gene(Real_chars::PARAMETER_INVERTION).Value())) var = right_limit - (var - (-uns::math::Abs(var) * static_cast<var_t>(1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value()))));
                            else {
                                var = (uns::math::Equals(var, 0.0F) ? 1.0F : var);
                                var = rand(
                                    -uns::math::Abs(var) * static_cast<var_t>(1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())),
                                    right_limit
                                );
                            };
                        }
                        else var = rand(
                            fmin(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())),
                            fmin(fmax(var * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()), var * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())), right_limit)
                        );
                    };
                };



                /*bool Reproduct(const chromosome& parent1, const chromosome& parent2);

                bool Repair();

                bool Mutate();

                bool Express();

                bool Adapt();*/
            };


        };
    };
};

#undef UNS_FILENAME