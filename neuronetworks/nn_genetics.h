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
            enum class Limits_type {
                MIN,
                MAX
            };

            enum Data_genes {
                DATATYPE = 0,                   //тип фреймов данных
                DATA = 1                        //количество данных
            };

            enum General_genes {
                NEYROTYPE = 0,                  //тип нейронов
                ADRESS = 1,                     //адреса слоев
                NUMBER = 2,                     //количество нейронов в слое
                OUT1 = 3,                       //номер нейрона в слое, являющегося выходным
                OUT2 = 4,                       //номер выхода сети, которым является выходной нейрон
                CORE1 = 5,                      //весовое ядро слоя
                CORE2 = 6                       //параметрическое ядро слоя
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
            using general_slot_t = std::tuple<neurotype_gene, adress_gene, simple_gene<size_t>, simple_gene<int>, simple_gene<int>, core_gene, core_gene>;
            class chromosome;
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
                int slot_id = 0;
                data_slot_t* data_slot = nullptr;
                general_slot_t* general_slot = nullptr;
            public:
                adress() noexcept {};
                adress(uns::nn::genetic::layer_connection_type set_conn, int set_slot_id) noexcept : conn(set_conn), slot_id(set_slot_id) { Format(); };
                adress(const adress& moving_obj) noexcept { *this = moving_obj; };
                adress& operator=(const adress& copying_obj) noexcept { conn = copying_obj.conn; slot_id = copying_obj.slot_id; data_slot = nullptr; general_slot = nullptr; Format(); return *this; };
                adress(adress&& moving_obj) noexcept { *this = std::move(moving_obj); };
                adress& operator=(adress&& moving_obj) noexcept { conn = std::move(moving_obj.conn); slot_id = std::move(moving_obj.slot_id); data_slot = nullptr; general_slot = nullptr; Format(); return *this; };
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
                adress& operator=(int slot_num) noexcept {
                    slot_id = slot_num;
                    general_slot = nullptr;
                    data_slot = nullptr;
                    return *this;
                };
                ~adress() noexcept {};

                uns::nn::string_t String() const noexcept {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");

                    return uns::string_cast<uns::nn::string_t>(conn) + comma +
                        uns::string_cast<uns::nn::string_t>(slot_id);
                };

                bool Set(const uns::nn::string_t& str) noexcept {
                    const static auto comma = uns::string_cast<uns::nn::string_t>(",");

                    uns::nn::string_t::size_type seeker = 0;

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, comma, false, -1)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else conn = uns::string_cast<uns::nn::genetic::layer_connection_type>(temp);
                    slot_id = uns::string_cast<size_t>(str.substr(seeker));

                    data_slot = nullptr;
                    general_slot = nullptr;

                    Format();

                    return true;
                };

                void Format() noexcept {
                    if (general_slot != nullptr && data_slot != nullptr) data_slot = nullptr;
                };

                bool IsExcited() const noexcept { return (general_slot != nullptr || data_slot != nullptr); };

                uns::nn::genetic::layer_connection_type Connection() const noexcept { return conn; };
                uns::nn::genetic::layer_connection_type& Connection() noexcept { return conn; };

                int Slot() const noexcept { return slot_id; };
                int& Slot() noexcept { return slot_id; };

                bool operator==(data_slot_t* data_slot_ptr) const noexcept { return data_slot != nullptr && data_slot == data_slot_ptr; };
                bool operator==(general_slot_t* general_slot_ptr) const noexcept { return general_slot != nullptr && general_slot == general_slot_ptr; };
                bool operator==(const adress& comparing_adress) const noexcept {
                    if (IsExcited() && comparing_adress.IsExcited())
                        return (general_slot == comparing_adress.general_slot && data_slot == comparing_adress.data_slot);

                    return (slot_id == comparing_adress.slot_id);
                };
                bool operator==(int compared_arg) const noexcept { return slot_id == compared_arg; };

                template<typename compared_t>
                bool operator!=(compared_t compared_arg) const noexcept { return !(*this == compared_arg); };

                bool operator>(int compared_arg) const noexcept { return slot_id > compared_arg; };

                bool operator>=(int compared_arg) const noexcept { return *this == compared_arg || *this > compared_arg; };

                bool operator<(int compared_arg) const noexcept { return slot_id < compared_arg; };

                bool operator<=(int compared_arg) const noexcept { return *this == compared_arg || *this < compared_arg; };

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
            public:

                auto Left() const noexcept { return data.first; };
                auto& Left() noexcept { return data.first; };
                auto Right() const noexcept { return data.second; };
                auto& Right() noexcept { return data.second; };

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
                        data_to_set.push_back(adress());                                                                                //Возможно, в таких ситуациях следует ставить emplace
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
                MUTATION_CHROMOSOME = 0,//вероятность мутации для всей хромосомы
                MUTATION_GENE,//вероятность мутации для каждого гена
                BLOCK,//вероятность присваивания/удаления статуса блокированного гена
                DOMINATION,//вероятность изменения значения доминантности у гена
                BLOCK_MODIFICATOR,//декремент, меняющий все вероятности, если ген заблокирован (0-1)
                VARIABLE_MUTATION,//вероятность мутации одной переменной 
                RANDOM_MUTATION,//вероятность выбора типа мутации "рандомный"
                CASCADE_MUTATION,//вероятность мутации по каскаду
                PERCENT_MODIFICATOR,//максимальный процент (0-1), на который может измениться величина при процентной мутации
                GENE_REPLACE,//вероятность возникновения мутации перемены местами основного гена/гена данных на другое место
                GENE_COPY,//вероятность копирования основного гена/гена данных в другое место
                GENE_ADD,//вероятность возникновения мутации создания нового гена
                GENE_DELETE,//вероятность возникновения мутации удаления гена
                GENE_OUT,//вероятность присваивания/удаления статуса выходного гена
                GENE_MIX,//удельная вероятность мутации перемешивания на один ген
                GENE_NUMMAX,//вероятность наследования от родителя с большим числом генов
                GENE_ADDVATE,//вероятность аддвации генов
                GRUPPA_REPRODUCT,//вероятность наследования всей группы при размножении
                GRUPPA_SPLIT,//вероятность сплитования выбранного основного гена
                GRUPPA_ADDVATE,//вероятность аддвации выбранного основного гена
                PROPAGATION_ORDER,//вероятность сохранения порядка распространения сигнала при перемене местами генов
                VECTOR_TIMECURRENT,//вероятность того, что мутация вектора будет проведена вблизи от текущего момента
                VECTOR_RANGEUSE,//вероятность использования численного допуска при процентных мутациях векторов в основных генах
                PARAMETER_INVERTION,//вероятность возникновения инверсии параметра при рандомной мутации
                SOMETOEACHNORM,//норма перекрытия входов нейронов при соединении SomeToEach
                REPRODUCTION_CLONING,//вероятность клонирования родительской особи при размножении
                PORTIONNORM,//доля от общего числа генов которая берется в мутации перемешивания
                GRADIENTH_POWER,//весовой коэффициент, используемый при моделировании градиентов входных данных по векторам
                PARGENEMIN,//минимальное значение параметров в нейронах
                PARGENEMAX,//максимальное значение параметров в нейронах
                UNLIMITMUTATION_MODIFICATOR,//модификатор для определения предела рандомной мутации у неограниченных величин
                DELTA,//допуск для принятия решений
                DEAL_SCALE,//масштабирующее значение для главного сигнала сделок
                SINGULARITY,//заглушка, сообщающая значение, заменяющее собой сингулярности при расчетах
                DSTEP_EPSILON,//значение, равное модулю производной тета-функции на плоских участках
                OUTNEU_EPSILON,//значение позволяющее смоделировать градиент целевой функции по выходам нейросети
                ADAPT_KOEFF,//коэффициент, масштабирующий параметры нейрона при адаптации
                ADAPT_DEGREEZERO,//заглушечное значение критерия особи в случае отсутствия сделок, используемое при адаптации
                PERSONAL_FEED,//личное значение коэффициента кормления
                PERSONAL_FATAL1,//личное значение #1 коэффициента fatal_act
                PERSONAL_FATAL2,//личное значение #2 коэффициента fatal_act
                PERSONAL_MUTATION,//личное значение коэффициента мутаций
                MFACTOR//мутационный фактор
            );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Int_chars, size_t,
                CORE1_SIZE,
                CORE2_SIZE
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
                    for (auto enum_val : enum_t::_values()) {
                        if (IsLimited(enum_val, Limits_type::MIN, nch) && uns::math::Less(genes[enum_val].Value(), Limit(enum_val, Limits_type::MIN, nch)))
                            genes[enum_val].Value() = Limit(enum_val, Limits_type::MIN, nch);
                        if (IsLimited(enum_val, Limits_type::MAX, nch) && uns::math::More(genes[enum_val].Value(), Limit(enum_val, Limits_type::MAX, nch)))
                            genes[enum_val].Value() = Limit(enum_val, Limits_type::MAX, nch);
                    };
                };

                void Clear() noexcept {};

                template<class randomizer_t> void RandomFill(uns::nn::genetic::chromosome& nch, randomizer_t& rand) {
                    for (auto enum_val : enum_t::_values()) {
                        genes[enum_val].Block() = false;
                        genes[enum_val].Dominant() = 0.0F;
                        genes[enum_val].Value() = rand(
                            Limit(enum_val, Limits_type::MIN, nch),
                            Limit(enum_val, Limits_type::MAX, nch)
                        );
                    };
                };

                /*auto begin() { return genes.begin(); };

                auto end() { return genes.end(); };

                auto cbegin() { return genes.cbegin(); };

                auto cend() { return genes.cend(); };*/

                auto Gene(const enum_t& enum_val) const { return genes[enum_val]; };
                auto& Gene(const enum_t& enum_val) { return genes[enum_val]; };

                virtual uns::nn::string_t GroupName() const = 0;

                virtual bool IsLimited(const enum_t& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const = 0;

                virtual char_t Limit(const enum_t& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const = 0;

                template<class randomizer_t> void Mutate(uns::nn::genetic::chromosome& nch, randomizer_t& rand);

            };
            //:Группа генов характеристик


            //Производные группы генов характеристик:
            class real_char_group : public char_group<float, uns::nn::genetic::Real_chars> {
                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("FLOAT CHAR GROUP"); };

                virtual bool IsLimited(const uns::nn::genetic::Real_chars& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        if (limit_type == Limits_type::MIN) return false;
                        else return false;
                    };
                };

                virtual float Limit(const uns::nn::genetic::Real_chars& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        if (limit_type == Limits_type::MIN) return 0.0F;
                        else return 0.0F;
                    };
                };
            };

            class int_char_group : public char_group<int, uns::nn::genetic::Int_chars> {
                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("INT CHAR GROUP"); };

                virtual bool IsLimited(const uns::nn::genetic::Int_chars& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        if (limit_type == Limits_type::MIN) return false;
                        else return false;
                    };
                };

                virtual int Limit(const uns::nn::genetic::Int_chars& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        if (limit_type == Limits_type::MIN) return 0;
                        else return 0;
                    };
                };
            };

            class long_char_group : public char_group<long long int, uns::nn::genetic::Long_chars> {
                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("LONG CHAR GROUP"); };

                virtual bool IsLimited(const uns::nn::genetic::Long_chars& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        if (limit_type == Limits_type::MIN) return false;
                        else return false;
                    };
                };

                virtual long long int Limit(const uns::nn::genetic::Long_chars& enum_val, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                    switch (enum_val) {
                    default:
                        if (limit_type == Limits_type::MIN) return 0;
                        else return 0;
                    };
                };
            };
            //:Производные группы генов характеристик


            //Группа генов входных данных:
            class data_group {
            public:

                enum Data_values {
                    DATATYPE,
                    DATA_LEFT,
                    DATA_RIGHT
                };

            protected:
                std::vector<std::tuple<datatype_gene, data_gene>> slots;
            public:
                data_group() noexcept {};
                data_group(const data_group& copying_obj) noexcept : slots(copying_obj.slots) {};
                data_group& operator=(const data_group& copying_obj) noexcept { slots = copying_obj.slots; return *this; };
                data_group(data_group&& moving_obj) noexcept : slots(std::move(moving_obj.slots)) {};
                data_group& operator=(data_group&& moving_obj) noexcept { slots = std::move(moving_obj.slots); return *this; };
                ~data_group() noexcept { Clear(); };

                uns::nn::string_t String() const {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto tab = uns::string_cast<uns::nn::string_t>("\t");
                    const static auto space = uns::string_cast<uns::nn::string_t>(" ");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto DATA_GROUP = GroupName();

                    auto res = DATA_GROUP + colon + delimiter;
                    for (auto slot : slots)
                        res += tab +
                        std::get<Data_genes::DATATYPE>(slot).String() + space +
                        std::get<Data_genes::DATA>(slot).String() + delimiter;
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
                        else if (!std::get<Data_genes::DATATYPE>(next_data_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, colon + DATA_GROUP) || seeker > data_group) break;
                        else if (!std::get<Data_genes::DATA>(next_data_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        slots.push_back(next_data_slot);
                    }
                    while (seeker < data_group);

                    return true;
                };

                void Format(const uns::nn::genetic::chromosome& nch);

                void Clear() noexcept { slots.clear(); };

                template<class randomizer_t> void RandomFill(uns::nn::genetic::chromosome& nch, randomizer_t& rand) {
                    for (size_t slot_num = 0; slot_num < slots.size(); slot_num++) {
                        Gene<Data_genes::DATATYPE>(slot_num).Block() = false;
                        Gene<Data_genes::DATATYPE>(slot_num).Dominant() = 0.0F;
                        Gene<Data_genes::DATATYPE>(slot_num).Value() = rand(Limit<DATATYPE>(slot_num, Limits_type::MIN, nch), Limit<DATATYPE>(slot_num, Limits_type::MAX, nch));

                        Gene<Data_genes::DATA>(slot_num).Block() = false;
                        Gene<Data_genes::DATA>(slot_num).Dominant() = 0.0F;
                        Gene<Data_genes::DATA>(slot_num).Left() = 0;
                        Gene<Data_genes::DATA>(slot_num).Right() = rand(Limit<DATA_RIGHT>(slot_num, Limits_type::MIN, nch), Limit<DATA_RIGHT>(slot_num, Limits_type::MAX, nch));
                        Gene<Data_genes::DATA>(slot_num).Left() = rand(Limit<DATA_LEFT>(slot_num, Limits_type::MIN, nch), Limit<DATA_LEFT>(slot_num, Limits_type::MAX, nch));
                    };
                };

                size_t Size() const { return slots.size(); };

                /*auto begin() { return slots.begin(); };

                auto end() { return slots.end(); };

                auto cbegin() { return slots.cbegin(); };

                auto cend() { return slots.cend(); };*/

                template<Data_genes enum_val> auto Gene(const size_t& slot_num) const { return std::get<enum_val>(slots[slot_num]); };
                template<Data_genes enum_val> auto& Gene(const size_t& slot_num) { return std::get<enum_val>(slots[slot_num]); };

                template<data_group::Data_values data_value> inline bool IsLimited(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const;

                template<data_group::Data_values data_value> inline auto Limit(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const;

                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("DATA GROUP"); };

                template<class randomizer_t> void Mutate(uns::nn::genetic::chromosome& nch, randomizer_t& rand);
            };
            //:Группа генов входных данных


            //Группа основных данных:
            class general_group {
            public:
                enum General_values {
                    NEYROTYPE = 0,
                    NUMBER,
                    OUT1,
                    OUT2,
                    ADRESS,
                    CORE1_SIZE1,
                    CORE1_SIZE2,
                    CORE1_ELEM,
                    CORE2_SIZE1,
                    CORE2_SIZE2,
                    CORE2_ELEM
                };
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
                            std::get<General_genes::NEYROTYPE>(slot).String() + space +
                            std::get<General_genes::NUMBER>(slot).String() + space +
                            std::get<General_genes::OUT1>(slot).String() + space +
                            std::get<General_genes::OUT2>(slot).String() + space +
                            std::get<General_genes::ADRESS>(slot).String() + space +
                            std::get<General_genes::CORE1>(slot).String() + space +
                            std::get<General_genes::CORE2>(slot).String() + delimiter;
                    };
                    res += colon + GENERAL_GROUP;

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const static auto delimiter = uns::string_cast<uns::nn::string_t>("\n");
                    const static auto colon = uns::string_cast<uns::nn::string_t>(":");
                    const static auto space = uns::string_cast<uns::nn::string_t>(" ");
                    const static auto GENERAL_GROUP = GroupName();

                    uns::nn::string_t::size_type seeker = 0;
                    uns::nn::string_t temp = uns::nn::string_t();

                    if (!uns::string::SeekerSet(str, seeker, GENERAL_GROUP + colon + delimiter, false, -1, colon + GENERAL_GROUP)) { return false; };
                    const auto general_group = uns::string::SeekerFind(str, seeker, colon + GENERAL_GROUP);
                    if (general_group == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    auto next_slot = general_slot_t();
                    do {
                        next_slot = general_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<General_genes::NEYROTYPE>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<General_genes::NUMBER>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<General_genes::OUT1>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<General_genes::OUT2>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<General_genes::ADRESS>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, space, false, -1, delimiter) || seeker > general_group) break;
                        else if (!std::get<General_genes::CORE1>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, colon + GENERAL_GROUP) || seeker > general_group) break;
                        else if (!std::get<General_genes::CORE2>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        slots.push_back(next_slot);
                    }
                    while (seeker < general_group);

                    return true;
                };

                void Format(const uns::nn::genetic::chromosome& nch);

                void Clear() noexcept { slots.clear(); };

                template<class randomizer_t> void RandomFill(uns::nn::genetic::chromosome& nch, randomizer_t& rand);

                size_t Size() const { return slots.size(); };

                /*auto begin() { return slots.begin(); };

                auto end() { return slots.end(); };

                auto cbegin() { return slots.cbegin(); };

                auto cend() { return slots.cend(); };*/

                template<General_genes enum_val> auto Gene(const size_t& slot_num) const { return std::get<enum_val>(slots[slot_num]); };
                template<General_genes enum_val> auto& Gene(const size_t& slot_num) { return std::get<enum_val>(slots[slot_num]); };

                template<General_values value> inline bool IsLimited(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const;

                template<General_values value> inline auto Limit(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const;

                virtual uns::nn::string_t GroupName() const { return uns::string_cast<uns::nn::string_t>("GENERAL GROUP"); };

                template<class randomizer_t> void Mutate(uns::nn::genetic::chromosome& nch, randomizer_t& rand);
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
                //std::vector<general_group> general_groups;
                uns::nn::genetic::general_group general_slots;

            public:
                chromosome() {};
                chromosome(const chromosome& copying_obj) :
                    real_char_genes(copying_obj.real_char_genes),
                    int_char_genes(copying_obj.int_char_genes),
                    long_char_genes(copying_obj.long_char_genes),
                    data_slots(copying_obj.data_slots),
                    general_slots(copying_obj.general_slots) {
                };
                chromosome& operator=(const chromosome& copying_obj) {
                    real_char_genes = copying_obj.real_char_genes;
                    int_char_genes = copying_obj.int_char_genes;
                    long_char_genes = copying_obj.long_char_genes;
                    data_slots = copying_obj.data_slots;
                    general_slots = copying_obj.general_slots;
                };
                chromosome(chromosome&& moving_obj) :
                    real_char_genes(std::move(moving_obj.real_char_genes)),
                    int_char_genes(std::move(moving_obj.int_char_genes)),
                    long_char_genes(std::move(moving_obj.long_char_genes)),
                    data_slots(std::move(moving_obj.data_slots)),
                    general_slots(std::move(moving_obj.general_slots)) {
                };
                chromosome& operator=(chromosome&& moving_obj) {
                    real_char_genes = std::move(moving_obj.real_char_genes);
                    int_char_genes = std::move(moving_obj.int_char_genes);
                    long_char_genes = std::move(moving_obj.long_char_genes);
                    data_slots = std::move(moving_obj.data_slots);
                    general_slots = std::move(moving_obj.general_slots);
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

                    res += general_slots.String() + delimiter + delimiter;

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

                    if (!uns::string::SeekerRead(str, seeker, temp, delimiter + delimiter, false, -1, neurochromosome)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!general_slots.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    if (seeker > neurochromosome) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };

                void Format() {
                    real_char_genes.Format(*this);
                    int_char_genes.Format(*this);
                    long_char_genes.Format(*this);

                    data_slots.Format(*this);

                    general_slots.Format(*this);
                };

                //void Clear();

                template<class randomizer_t> void RandomFill(randomizer_t& rand) {
                    real_char_genes.RandomFill<randomizer_t>(*this, rand);
                    int_char_genes.RandomFill<randomizer_t>(*this, rand);
                    long_char_genes.RandomFill<randomizer_t>(*this, rand);

                    data_slots.RandomFill<randomizer_t>(*this, rand);

                    general_slots.RandomFill<randomizer_t>(*this, rand);
                };

                size_t NumberOfDataSlots() const { return data_slots.Size(); };

                size_t NumberOfGeneralSlots() const { return data_slots.Size(); };

                auto& Gene(const Real_chars& enum_val) { return real_char_genes.Gene(enum_val); };
                auto Gene(const Real_chars& enum_val) const { return real_char_genes.Gene(enum_val); };
                auto& Gene(const Int_chars& enum_val) { return int_char_genes.Gene(enum_val); };
                auto Gene(const Int_chars& enum_val) const { return int_char_genes.Gene(enum_val); };
                auto& Gene(const Long_chars& enum_val) { return long_char_genes.Gene(enum_val); };
                auto Gene(const Long_chars& enum_val) const { return long_char_genes.Gene(enum_val); };
                template<Data_genes enum_val> auto& Gene(const size_t& slot_num) { return data_slots.Gene<enum_val>(slot_num); };
                template<Data_genes enum_val> auto Gene(const size_t& slot_num) const { return data_slots.Gene<enum_val>(slot_num); };
                template<General_genes enum_val> auto& Gene(const size_t& slot_num) { return general_slots.Gene<enum_val>(slot_num); };
                template<General_genes enum_val> auto Gene(const size_t& slot_num) const { return general_slots.Gene<enum_val>(slot_num); };

                template<class var_t, class randomizer_t> void Variate(var_t& var, bool left_limited, bool right_limited, const var_t& left_limit, const var_t& right_limit, bool block, randomizer_t& rand) {
                    if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                        real_char_genes.Gene(Real_chars::RANDOM_MUTATION).Value() *
                        real_char_genes.Gene(Real_chars::MFACTOR).Value() *
                        block ? real_char_genes.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                    ))) {
                        if (rand(real_char_genes.Gene(Real_chars::PARAMETER_INVERTION).Value())) {
                            if (left_limited && right_limited) {
                                var = right_limit - (var - left_limit);
                            };
                            if (!left_limited && !right_limited) {
                                var = 0 - var;
                            };
                            if (left_limited && !right_limited) {
                                var = static_cast<var_t>(uns::math::Abs(var) * (1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value()))) - (var - left_limit);
                            };
                            if (!left_limited && right_limited) {
                                var = right_limit - (var - static_cast<var_t>(-1.0F * uns::math::Abs(var) * (1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value()))));
                            };
                        }
                        else {
                            if (left_limited && right_limited) {
                                var = rand(left_limit, right_limit);
                            };
                            if (!left_limited && !right_limited) {
                                var = uns::math::Equals(var, 0.0F) ? 1.0F : var;
                                var = static_cast<var_t>(uns::math::Abs(var) * rand(
                                    -(1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())),
                                    1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())
                                ));
                            };
                            if (left_limited && !right_limited) {
                                var = rand(
                                    left_limit,
                                    static_cast<var_t>(uns::math::Abs(var) * (1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value())))
                                );
                            };
                            if (!left_limited && right_limited) {
                                var = rand(
                                    static_cast<var_t>((0 - uns::math::Abs(var)) * (1.0F + uns::math::Abs(real_char_genes.Gene(Real_chars::UNLIMITMUTATION_MODIFICATOR).Value()))),
                                    right_limit
                                );
                            };
                        };
                    }
                    else {
                        if (left_limited && right_limited) {
                            var = rand(
                                fmax(left_limit, static_cast<var_t>(uns::math::Abs(var) * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()))),
                                fmin(right_limit, static_cast<var_t>(uns::math::Abs(var) * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())))
                            );
                        };
                        if (!left_limited && !right_limited) {
                            var = static_cast<var_t>(var * rand(1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value(), 1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()));
                        };
                        if (left_limited && !right_limited) {
                            var = rand(
                                static_cast<var_t>(fmax(left_limit, static_cast<var_t>(uns::math::Abs(var) * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())))),
                                static_cast<var_t>(uns::math::Abs(var) * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()))
                            );
                        };
                        if (!left_limited && right_limited) {
                            var = rand(
                                static_cast<var_t>(uns::math::Abs(var) * (1.0F - real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value())),
                                static_cast<var_t>(fmin(right_limit, static_cast<var_t>(uns::math::Abs(var) * (1.0F + real_char_genes.Gene(Real_chars::PERCENT_MODIFICATOR).Value()))))
                            );
                        };
                    };
                };



                /*bool Reproduct(const chromosome& parent1, const chromosome& parent2);

                bool Repair();*/

                template<class randomizer_t> bool Mutate(randomizer_t& rand) {
                    real_char_genes.Mutate<randomizer_t>(*this, rand);
                    int_char_genes.Mutate<randomizer_t>(*this, rand);
                    long_char_genes.Mutate<randomizer_t>(*this, rand);

                    data_slots.Mutate<randomizer_t>(*this, rand);

                    //for (auto& general_group : general_groups)
                    //    general_group.Mutate(*this, rand);

                    return true;
                };

                /*bool Express();

                bool Adapt();*/
            };


            //методы char_group:
            template<typename char_t, typename enum_t> template<class randomizer_t> void char_group<char_t, enum_t>::Mutate(uns::nn::genetic::chromosome& nch, randomizer_t& rand) {
                for (auto enum_val : enum_t::_values()) {
                    if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                        nch.Gene(Real_chars::MUTATION_GENE).Value() *
                        nch.Gene(Real_chars::MFACTOR).Value() *
                        Gene(enum_val).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                    ))) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::BLOCK).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            Gene(enum_val).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            Gene(enum_val).Block() = !Gene(enum_val).Block();

                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::DOMINATION).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            Gene(enum_val).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            nch.Variate<float, randomizer_t>(Gene(enum_val).Dominant(), false, false, 0.0F, 0.0F, Gene(enum_val).Block(), rand);

                        if (enum_val != static_cast<Real_chars>(Real_chars::MFACTOR) &&
                            rand(uns::math::HyperbolicAsympthoticTo1From0(
                                nch.Gene(Real_chars::VARIABLE_MUTATION).Value() *
                                nch.Gene(Real_chars::MFACTOR).Value() *
                                Gene(enum_val).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                            )))
                            nch.Variate<char_t, randomizer_t>(
                                Gene(enum_val).Value(),
                                IsLimited(enum_val, Limits_type::MIN, nch),
                                IsLimited(enum_val, Limits_type::MAX, nch),
                                Limit(enum_val, Limits_type::MIN, nch),
                                Limit(enum_val, Limits_type::MAX, nch),
                                Gene(enum_val).Block(),
                                rand
                                );
                    };
                };
            };
            //:методы char_group


            //методы data_group:
            template<data_group::Data_values data_value> inline bool data_group::IsLimited(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const { return true; };

            template<data_group::Data_values data_value> inline auto data_group::Limit(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                if constexpr (data_value == Data_values::DATATYPE) {
                    if (limit_type == Limits_type::MIN) return static_cast<size_t>(0);
                    else return ctx[CTX_CCASCADES::Frame].Total() > 0 ? ctx[CTX_CCASCADES::Frame].Total() - 1 : 0;
                }
                else if constexpr (data_value == Data_values::DATA_LEFT) {
                    if (limit_type == Limits_type::MIN) return 0;
                    else return 0;
                    //else return Gene<Data_genes::DATA>(slot_num).Right();
                }
                else {
                    if (limit_type == Limits_type::MIN) return Gene<Data_genes::DATA>(slot_num).Left();
                    else return ctx[CTX_CCASCADES::Frame].Width(Gene<Data_genes::DATATYPE>(slot_num).Value()) > 0 ? ctx[CTX_CCASCADES::Frame].Width(Gene<Data_genes::DATATYPE>(slot_num).Value()) - 1 : 0;
                };
            };

            void data_group::Format(const uns::nn::genetic::chromosome& nch) {
                if (ctx[CTX_CCASCADES::Frame].Total() == 0) return;

                for (size_t slot_num = 0; slot_num < slots.size(); slot_num++) {
                    if (IsLimited<DATATYPE>(slot_num, Limits_type::MAX, nch) &&
                        Gene<Data_genes::DATATYPE>(slot_num).Value() > Limit<DATATYPE>(slot_num, Limits_type::MAX, nch))
                        Gene<Data_genes::DATATYPE>(slot_num).Value() = Limit<DATATYPE>(slot_num, Limits_type::MAX, nch);
                    if (IsLimited<DATATYPE>(slot_num, Limits_type::MIN, nch) &&
                        Gene<Data_genes::DATATYPE>(slot_num).Value() < Limit<DATATYPE>(slot_num, Limits_type::MIN, nch))
                        Gene<Data_genes::DATATYPE>(slot_num).Value() = Limit<DATATYPE>(slot_num, Limits_type::MIN, nch);

                    if (IsLimited<DATA_RIGHT>(slot_num, Limits_type::MAX, nch) &&
                        Gene<Data_genes::DATA>(slot_num).Right() > Limit<DATA_RIGHT>(slot_num, Limits_type::MAX, nch))
                        Gene<Data_genes::DATA>(slot_num).Right() = Limit<DATA_RIGHT>(slot_num, Limits_type::MAX, nch);
                    if (IsLimited<DATA_RIGHT>(slot_num, Limits_type::MIN, nch) &&
                        Gene<Data_genes::DATA>(slot_num).Right() < Limit<DATA_RIGHT>(slot_num, Limits_type::MIN, nch))
                        Gene<Data_genes::DATA>(slot_num).Right() = Limit<DATA_RIGHT>(slot_num, Limits_type::MIN, nch);

                    if (IsLimited<DATA_LEFT>(slot_num, Limits_type::MIN, nch) &&
                        Gene<Data_genes::DATA>(slot_num).Left() < Limit<DATA_LEFT>(slot_num, Limits_type::MIN, nch))
                        Gene<Data_genes::DATA>(slot_num).Left() = Limit<DATA_LEFT>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<DATA_LEFT>(slot_num, Limits_type::MAX, nch) &&
                        Gene<Data_genes::DATA>(slot_num).Left() > Limit<DATA_LEFT>(slot_num, Limits_type::MAX, nch))
                        Gene<Data_genes::DATA>(slot_num).Left() = Limit<DATA_LEFT>(slot_num, Limits_type::MAX, nch);
                };
            };

            template<class randomizer_t> void data_group::Mutate(uns::nn::genetic::chromosome& nch, randomizer_t& rand) {
                size_t slot_num = 0;
                for (auto& slot : slots) {
                    if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                        nch.Gene(Real_chars::MUTATION_GENE).Value() *
                        nch.Gene(Real_chars::MFACTOR).Value() *
                        std::get<Data_genes::DATATYPE>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                    ))) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::BLOCK).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            std::get<Data_genes::DATATYPE>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            std::get<Data_genes::DATATYPE>(slot).Block() = !std::get<Data_genes::DATATYPE>(slot).Block();

                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::DOMINATION).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            std::get<Data_genes::DATATYPE>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            nch.Variate<float, randomizer_t>(std::get<Data_genes::DATATYPE>(slot).Dominant(), false, false, 0.0F, 0.0F, std::get<Data_genes::DATATYPE>(slot).Block(), rand);

                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::VARIABLE_MUTATION).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            std::get<Data_genes::DATATYPE>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            if (rand(nch.Gene(Real_chars::CASCADE_MUTATION).Value()))
                                std::get<Data_genes::DATATYPE>(slot).Value() = ctx[CTX_CCASCADES::Frame].Variate(std::get<Data_genes::DATATYPE>(slot).Value(), rand);
                            else
                                std::get<Data_genes::DATATYPE>(slot).Value() = rand(
                                    Limit<data_group::DATATYPE>(slot_num, Limits_type::MIN, nch),
                                    Limit<data_group::DATATYPE>(slot_num, Limits_type::MAX, nch)
                                );
                    };

                    if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                        nch.Gene(Real_chars::MUTATION_GENE).Value() *
                        nch.Gene(Real_chars::MFACTOR).Value() *
                        std::get<Data_genes::DATA>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                    ))) {
                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::BLOCK).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            std::get<Data_genes::DATA>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            std::get<Data_genes::DATA>(slot).Block() = !std::get<Data_genes::DATA>(slot).Block();

                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::DOMINATION).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            std::get<Data_genes::DATA>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        )))
                            nch.Variate<float, randomizer_t>(std::get<Data_genes::DATA>(slot).Dominant(), false, false, 0.0F, 0.0F, std::get<Data_genes::DATA>(slot).Block(), rand);

                        if (rand(uns::math::HyperbolicAsympthoticTo1From0(
                            nch.Gene(Real_chars::VARIABLE_MUTATION).Value() *
                            nch.Gene(Real_chars::MFACTOR).Value() *
                            std::get<Data_genes::DATA>(slot).Block() ? nch.Gene(Real_chars::BLOCK_MODIFICATOR).Value() : 1.0F
                        ))) {
                            nch.Variate<size_t, randomizer_t>(
                                std::get<Data_genes::DATA>(slot).Right(),
                                IsLimited<data_group::DATA_RIGHT>(slot_num, Limits_type::MIN, nch),
                                IsLimited<data_group::DATA_RIGHT>(slot_num, Limits_type::MAX, nch),
                                Limit<data_group::DATA_RIGHT>(slot_num, Limits_type::MIN, nch),
                                Limit<data_group::DATA_RIGHT>(slot_num, Limits_type::MAX, nch),
                                std::get<Data_genes::DATA>(slot).Block(),
                                rand
                                );
                            nch.Variate<size_t, randomizer_t>(
                                std::get<Data_genes::DATA>(slot).Left(),
                                IsLimited<data_group::DATA_LEFT>(slot_num, Limits_type::MIN, nch),
                                IsLimited<data_group::DATA_LEFT>(slot_num, Limits_type::MAX, nch),
                                Limit<data_group::DATA_LEFT>(slot_num, Limits_type::MIN, nch),
                                Limit<data_group::DATA_LEFT>(slot_num, Limits_type::MAX, nch),
                                std::get<Data_genes::DATA>(slot).Block(),
                                rand
                                );
                        };
                    };

                    slot_num++;
                };
            };
            //:методы data_group


            //методы general_group:
            template<general_group::General_values value> bool general_group::IsLimited(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                if constexpr (value == general_group::NEYROTYPE)
                    return true;
                else if constexpr (value == general_group::NUMBER)
                    return limit_type == Limits_type::MIN;
                else if constexpr (value == general_group::OUT1)
                    return limit_type == Limits_type::MIN;
                else if constexpr (value == general_group::OUT2)
                    return true;
                else if constexpr (value == general_group::ADRESS)
                    return true;
                else if constexpr (value == general_group::CORE1_SIZE1)
                    return true;
                else if constexpr (value == general_group::CORE1_SIZE2)
                    return true;
                else if constexpr (value == general_group::CORE1_ELEM)
                    return true;
                else if constexpr (value == general_group::CORE2_SIZE1)
                    return true;
                else if constexpr (value == general_group::CORE2_SIZE2)
                    return true;
                else if constexpr (value == general_group::CORE2_ELEM)
                    return true;
                else
                    return false;
            };

            template<general_group::General_values value> auto general_group::Limit(const size_t& slot_num, const Limits_type& limit_type, const uns::nn::genetic::chromosome& nch) const {
                if constexpr (value == general_group::NEYROTYPE) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<size_t>(0);
                    else
                        return ctx[CTX_CCASCADES::Neuro].Total() > 0 ? ctx[CTX_CCASCADES::Neuro].Total() - 1 : 0;
                }
                else if constexpr (value == general_group::NUMBER) {                                    //ограничения на количество нейронов в слое зависит от типа нейронов слоя
                    if (limit_type == Limits_type::MIN)
                        return static_cast<size_t>(1);
                    else
                        return static_cast<size_t>(fmax(static_cast<size_t>(1), static_cast<size_t>(nch.Gene(Real_chars::PARGENEMAX).Value())));
                }
                else if constexpr (value == general_group::OUT1) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<int>(-1);
                    else
                        return static_cast<int>(Gene<General_genes::NUMBER>(slot_num).Value()) - 1;
                }
                else if constexpr (value == general_group::OUT2) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<int>(-1);
                    else
                        return static_cast<int>(ctx[CTX_CUINT::Maximum_OutNeutons_Number]) - 1;
                }
                else if constexpr (value == general_group::ADRESS) {                                    //ограничение на адреса зависят от типа нейронов слоя
                    if (limit_type == Limits_type::MIN)
                        return -static_cast<int>(nch.NumberOfDataSlots());
                    else
                        return static_cast<int>(nch.NumberOfGeneralSlots()) - 1;
                }
                else if constexpr (value == general_group::CORE1_SIZE1) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE1_SIZE).Value());
                    else
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE1_SIZE).Value());
                }
                else if constexpr (value == general_group::CORE1_SIZE2) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE1_SIZE).Value());
                    else
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE1_SIZE).Value());
                }
                else if constexpr (value == general_group::CORE1_ELEM) {
                    if (limit_type == Limits_type::MIN)
                        return nch.Gene(Real_chars::PARGENEMIN).Value();
                    else
                        return nch.Gene(Real_chars::PARGENEMAX).Value();
                }
                else if constexpr (value == general_group::CORE2_SIZE1) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE2_SIZE).Value());
                    else
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE2_SIZE).Value());
                }
                else if constexpr (value == general_group::CORE2_SIZE2) {
                    if (limit_type == Limits_type::MIN)
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE2_SIZE).Value());
                    else
                        return static_cast<size_t>(nch.Gene(Int_chars::CORE2_SIZE).Value());
                }
                else if constexpr (value == general_group::CORE2_ELEM) {
                    if (limit_type == Limits_type::MIN)
                        return nch.Gene(Real_chars::PARGENEMIN).Value();
                    else
                        return nch.Gene(Real_chars::PARGENEMAX).Value();
                }
                else 
                    return 0;
            };

            void general_group::Format(const uns::nn::genetic::chromosome& nch) {
                if (ctx[CTX_CCASCADES::Neuro].Total() == 0) return;

                size_t core1_size1 = 0;
                size_t core1_size2 = 0;
                size_t core2_size1 = 0;
                size_t core2_size2 = 0;
                
                for (size_t slot_num = 0; slot_num < slots.size(); slot_num++) {
                    if (IsLimited<NEYROTYPE>(slot_num, Limits_type::MAX, nch) &&
                        Gene<General_genes::NEYROTYPE>(slot_num).Value() > Limit<NEYROTYPE>(slot_num, Limits_type::MAX, nch))
                        Gene<General_genes::NEYROTYPE>(slot_num).Value() = Limit<NEYROTYPE>(slot_num, Limits_type::MAX, nch);
                    if (IsLimited<NEYROTYPE>(slot_num, Limits_type::MIN, nch) &&
                        Gene<General_genes::NEYROTYPE>(slot_num).Value() < Limit<NEYROTYPE>(slot_num, Limits_type::MIN, nch))
                        Gene<General_genes::NEYROTYPE>(slot_num).Value() = Limit<NEYROTYPE>(slot_num, Limits_type::MIN, nch);

                    if (IsLimited<NUMBER>(slot_num, Limits_type::MIN, nch) &&
                        Gene<General_genes::NUMBER>(slot_num).Value() < Limit<NUMBER>(slot_num, Limits_type::MIN, nch))
                        Gene<General_genes::NUMBER>(slot_num).Value() = Limit<NUMBER>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<NUMBER>(slot_num, Limits_type::MAX, nch) &&
                        Gene<General_genes::NUMBER>(slot_num).Value() > Limit<NUMBER>(slot_num, Limits_type::MAX, nch))
                        Gene<General_genes::NUMBER>(slot_num).Value() = Limit<NUMBER>(slot_num, Limits_type::MAX, nch);

                    if (IsLimited<OUT1>(slot_num, Limits_type::MIN, nch) &&
                        Gene<General_genes::OUT1>(slot_num).Value() < Limit<OUT1>(slot_num, Limits_type::MIN, nch))
                        Gene<General_genes::OUT1>(slot_num).Value() = Limit<OUT1>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<OUT1>(slot_num, Limits_type::MAX, nch) &&
                        Gene<General_genes::OUT1>(slot_num).Value() > Limit<OUT1>(slot_num, Limits_type::MAX, nch))
                        Gene<General_genes::OUT1>(slot_num).Value() = Limit<OUT1>(slot_num, Limits_type::MAX, nch);

                    if (IsLimited<OUT2>(slot_num, Limits_type::MIN, nch) &&
                        Gene<General_genes::OUT2>(slot_num).Value() < Limit<OUT2>(slot_num, Limits_type::MIN, nch))
                        Gene<General_genes::OUT2>(slot_num).Value() = Limit<OUT2>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<OUT2>(slot_num, Limits_type::MAX, nch) &&
                        Gene<General_genes::OUT2>(slot_num).Value() > Limit<OUT2>(slot_num, Limits_type::MAX, nch))
                        Gene<General_genes::OUT2>(slot_num).Value() = Limit<OUT2>(slot_num, Limits_type::MAX, nch);

                    for (auto& adress : Gene<General_genes::ADRESS>(slot_num).Value()) {
                        if (IsLimited<ADRESS>(slot_num, Limits_type::MIN, nch) && adress < Limit<ADRESS>(slot_num, Limits_type::MIN, nch))
                            adress = Limit<ADRESS>(slot_num, Limits_type::MIN, nch);
                        if (IsLimited<ADRESS>(slot_num, Limits_type::MAX, nch) && adress > Limit<ADRESS>(slot_num, Limits_type::MAX, nch))
                            adress = Limit<ADRESS>(slot_num, Limits_type::MAX, nch);
                    };

                    core1_size1 = Gene<General_genes::CORE1>(slot_num).Value().size1();
                    core1_size2 = Gene<General_genes::CORE1>(slot_num).Value().size2();
                    if (IsLimited<CORE1_SIZE1>(slot_num, Limits_type::MIN, nch) &&
                        core1_size1 < Limit<CORE1_SIZE1>(slot_num, Limits_type::MIN, nch))
                        core1_size1 = Limit<CORE1_SIZE1>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<CORE1_SIZE1>(slot_num, Limits_type::MAX, nch) &&
                        core1_size1 > Limit<CORE1_SIZE1>(slot_num, Limits_type::MAX, nch))
                        core1_size1 = Limit<CORE1_SIZE1>(slot_num, Limits_type::MAX, nch);
                    if (IsLimited<CORE1_SIZE2>(slot_num, Limits_type::MIN, nch) &&
                        core1_size2 < Limit<CORE1_SIZE2>(slot_num, Limits_type::MIN, nch))
                        core1_size2 = Limit<CORE1_SIZE2>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<CORE1_SIZE2>(slot_num, Limits_type::MAX, nch) &&
                        core1_size2 > Limit<CORE1_SIZE2>(slot_num, Limits_type::MAX, nch))
                        core1_size2 = Limit<CORE1_SIZE2>(slot_num, Limits_type::MAX, nch);

                    if (core1_size1 != Gene<General_genes::CORE1>(slot_num).Value().size1() ||
                        core1_size2 != Gene<General_genes::CORE1>(slot_num).Value().size2())
                        uns::math::Resize(Gene<General_genes::CORE1>(slot_num).Value(), core1_size1, core1_size2, 0.0F);

                    for (size_t i1 = 0; i1 < Gene<General_genes::CORE1>(slot_num).Value().size1(); i1++)
                        for (size_t i2 = 0; i2 < Gene<General_genes::CORE1>(slot_num).Value().size2(); i2++) {
                            if (IsLimited<CORE1_ELEM>(slot_num, Limits_type::MIN, nch) &&
                                Gene<General_genes::CORE1>(slot_num).Value()(i1, i2) < Limit<CORE1_ELEM>(slot_num, Limits_type::MIN, nch))
                                Gene<General_genes::CORE1>(slot_num).Value()(i1, i2) = Limit<CORE1_ELEM>(slot_num, Limits_type::MIN, nch);
                            if (IsLimited<CORE1_ELEM>(slot_num, Limits_type::MAX, nch) &&
                                Gene<General_genes::CORE1>(slot_num).Value()(i1, i2) > Limit<CORE1_ELEM>(slot_num, Limits_type::MAX, nch))
                                Gene<General_genes::CORE1>(slot_num).Value()(i1, i2) = Limit<CORE1_ELEM>(slot_num, Limits_type::MAX, nch);
                        };

                    core2_size1 = Gene<General_genes::CORE2>(slot_num).Value().size1();
                    core2_size2 = Gene<General_genes::CORE2>(slot_num).Value().size2();
                    if (IsLimited<CORE2_SIZE1>(slot_num, Limits_type::MIN, nch) &&
                        core2_size1 < Limit<CORE2_SIZE1>(slot_num, Limits_type::MIN, nch))
                        core2_size1 = Limit<CORE2_SIZE1>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<CORE2_SIZE1>(slot_num, Limits_type::MAX, nch) &&
                        core2_size1 > Limit<CORE2_SIZE1>(slot_num, Limits_type::MAX, nch))
                        core2_size1 = Limit<CORE2_SIZE1>(slot_num, Limits_type::MAX, nch);
                    if (IsLimited<CORE2_SIZE2>(slot_num, Limits_type::MIN, nch) &&
                        core2_size2 < Limit<CORE2_SIZE2>(slot_num, Limits_type::MIN, nch))
                        core2_size2 = Limit<CORE2_SIZE2>(slot_num, Limits_type::MIN, nch);
                    if (IsLimited<CORE2_SIZE2>(slot_num, Limits_type::MAX, nch) &&
                        core2_size2 > Limit<CORE2_SIZE2>(slot_num, Limits_type::MAX, nch))
                        core2_size2 = Limit<CORE2_SIZE2>(slot_num, Limits_type::MAX, nch);

                    if (core2_size1 != Gene<General_genes::CORE2>(slot_num).Value().size1() ||
                        core2_size2 != Gene<General_genes::CORE2>(slot_num).Value().size2())
                        uns::math::Resize(Gene<General_genes::CORE2>(slot_num).Value(), core2_size1, core2_size2, 0.0F);

                    for (size_t i1 = 0; i1 < Gene<General_genes::CORE2>(slot_num).Value().size1(); i1++)
                        for (size_t i2 = 0; i2 < Gene<General_genes::CORE2>(slot_num).Value().size2(); i2++) {
                            if (IsLimited<CORE2_ELEM>(slot_num, Limits_type::MIN, nch) &&
                                Gene<General_genes::CORE2>(slot_num).Value()(i1, i2) < Limit<CORE2_ELEM>(slot_num, Limits_type::MIN, nch))
                                Gene<General_genes::CORE2>(slot_num).Value()(i1, i2) = Limit<CORE2_ELEM>(slot_num, Limits_type::MIN, nch);
                            if (IsLimited<CORE2_ELEM>(slot_num, Limits_type::MAX, nch) &&
                                Gene<General_genes::CORE2>(slot_num).Value()(i1, i2) > Limit<CORE2_ELEM>(slot_num, Limits_type::MAX, nch))
                                Gene<General_genes::CORE2>(slot_num).Value()(i1, i2) = Limit<CORE2_ELEM>(slot_num, Limits_type::MAX, nch);
                        };
                };
            };

            template<class randomizer_t> void general_group::RandomFill(uns::nn::genetic::chromosome& nch, randomizer_t& rand) {
                Clear();

                slots.push_back(uns::nn::genetic::general_slot_t());

                Gene<General_genes::NEYROTYPE>(0).Value() = rand(Limit<NEYROTYPE>(0, Limits_type::MIN, nch), Limit<NEYROTYPE>(0, Limits_type::MAX, nch));
                Gene<General_genes::NUMBER>(0).Value() = rand(Limit<NUMBER>(0, Limits_type::MIN, nch), Limit<NUMBER>(0, Limits_type::MAX, nch));
                Gene<General_genes::OUT1>(0).Value() = rand(Limit<OUT1>(0, Limits_type::MIN, nch), Limit<OUT1>(0, Limits_type::MAX, nch));
                Gene<General_genes::OUT2>(0).Value() = rand(Limit<OUT2>(0, Limits_type::MIN, nch), Limit<OUT2>(0, Limits_type::MAX, nch));
                Gene<General_genes::ADRESS>(0).Value().push_back(adress(layer_connection_type::OneToEach, rand(Limit<ADRESS>(0, Limits_type::MIN, nch), Limit<ADRESS>(0, Limits_type::MAX, nch))));    //по идее, здесь должен быть рандомный выбор соединения но с ограничением по типу нейронов
                
                size_t core1_size1 = rand(Limit<CORE1_SIZE1>(0, Limits_type::MIN, nch), Limit<CORE1_SIZE1>(0, Limits_type::MAX, nch));
                size_t core1_size2 = rand(Limit<CORE1_SIZE2>(0, Limits_type::MIN, nch), Limit<CORE1_SIZE2>(0, Limits_type::MAX, nch));
                size_t core2_size1 = rand(Limit<CORE2_SIZE1>(0, Limits_type::MIN, nch), Limit<CORE2_SIZE1>(0, Limits_type::MAX, nch));
                size_t core2_size2 = rand(Limit<CORE2_SIZE2>(0, Limits_type::MIN, nch), Limit<CORE2_SIZE2>(0, Limits_type::MAX, nch));

                Gene<General_genes::CORE1>(0).Value().resize(core1_size1, core1_size2, false);
                for (size_t i1 = 0; i1 < Gene<General_genes::CORE1>(0).Value().size1(); i1++)
                    for (size_t i2 = 0; i2 < Gene<General_genes::CORE1>(0).Value().size2(); i2++)
                        Gene<General_genes::CORE1>(0).Value()(i1, i2) = rand(Limit<CORE1_ELEM>(0, Limits_type::MIN, nch), Limit<CORE1_ELEM>(0, Limits_type::MAX, nch));

                Gene<General_genes::CORE2>(0).Value().resize(core2_size1, core2_size2, false);
                for (size_t i1 = 0; i1 < Gene<General_genes::CORE2>(0).Value().size1(); i1++)
                    for (size_t i2 = 0; i2 < Gene<General_genes::CORE2>(0).Value().size2(); i2++)
                        Gene<General_genes::CORE2>(0).Value()(i1, i2) = rand(Limit<CORE2_ELEM>(0, Limits_type::MIN, nch), Limit<CORE2_ELEM>(0, Limits_type::MAX, nch));
            };

            template<class randomizer_t> void general_group::Mutate(uns::nn::genetic::chromosome& nch, randomizer_t& rand) {
            
            };
            //:методы general_group

        };
    };
};

#undef UNS_FILENAME