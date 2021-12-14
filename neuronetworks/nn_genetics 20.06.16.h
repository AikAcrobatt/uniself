#pragma once

#include "uniself/neuronetworks/nn_general.h"
#include "uniself/neuronetworks/nn_networks.h"
#include "uniself/neuronetworks/nn_neurons.h"

#define UNS_FILENAME "nn_genetics.h"

namespace uns {
    namespace nn {
        namespace genetic {


            /*using adress_gene_t = uns::nn::genetic::gene<std::vector<adress>>;
            using core1_gene_t = uns::nn::genetic::gene<ublas::matrix<signal_t>>;
            using core2_gene_t = uns::nn::genetic::gene<ublas::matrix<signal_t>>;
            using neuron_number_gene_t = uns::nn::genetic::gene<size_t>;
            using out_neuron_gene_t = uns::nn::genetic::gene<int>;
            using data_slot_t = std::tuple<datatype_gene_t, data_gene_t>;
            using general_slot_t = std::tuple<neurotype_gene_t, adress_gene_t, core1_gene_t, core2_gene_t, neuron_number_gene_t, out_neuron_gene_t>;
            using general_group_t = std::vector<general_slot_t>;*/



            //СЛУЖЕБНЫЕ ПЕРЕЧИСЛЕНИЯ
            //Типы ограниченности переменных при вариации
            UNS_ENUM_DECLARATOR(VARIATION_LIMITS_TYPE, int,
                left,
                right
            );


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
                    return uns::string_cast<uns::nn::string_t>(conn) + uns::string_cast<uns::nn::string_t>(",") +
                        uns::string_cast<uns::nn::string_t>(group_id < -1 ? -1 : group_id) + uns::string_cast<uns::nn::string_t>(",") +
                        uns::string_cast<uns::nn::string_t>(slot_id);
                };

                bool Set(const uns::nn::string_t& str) noexcept {
                    uns::nn::string_t::size_type seeker = 0;

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(","), false, -1)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else conn = uns::string_cast<uns::nn::genetic::layer_connection_type>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(","), false, -1)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else group_id = uns::string_cast<int>(temp);
                    temp = str.substr(seeker);
                    slot_id = uns::string_cast<size_t>(temp);

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


            //Контейнер гена
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
                    return uns::string_cast<uns::nn::string_t>("[") +
                        uns::string_cast<uns::nn::string_t>(block) + uns::string_cast<uns::nn::string_t>(",") +
                        uns::string_cast<uns::nn::string_t>(dominant) +
                        uns::string_cast<uns::nn::string_t>("(") + ValueString(data) + uns::string_cast<uns::nn::string_t>(")]");
                };

                bool Set(const uns::nn::string_t& str) {
                    uns::nn::string_t::size_type seeker = 0;

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("["), false, -1, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(")]")) == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(","), false, -1, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else block = uns::string_cast<bool>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>("("), false, -1, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else dominant = uns::string_cast<uns::nn::genetic::dominancy_t>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(")"), false, 0, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
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
                    return uns::string_cast<uns::nn::string_t>(stringifying_data.first) +
                        uns::string_cast<uns::nn::string_t>(",") +
                        uns::string_cast<uns::nn::string_t>(stringifying_data.second);
                };

                virtual bool ValueSet(std::pair<size_t, size_t>& data_to_set, const uns::nn::string_t& str) override {
                    uns::nn::string_t::size_type seeker = 0;

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(","), false, -1)) return false;
                    else data_to_set.first = uns::string_cast<size_t>(temp);
                    data_to_set.second = uns::string_cast<size_t>(str.substr(seeker));

                    return true;
                };
            };


            class adress_gene : public uns::nn::genetic::base_gene<std::vector<adress>> {
            protected:
                virtual uns::nn::string_t ValueString(const std::vector<adress>& stringifying_data) const override {
                    uns::nn::string_t res = uns::nn::string_t();

                    for (auto one_adress : stringifying_data)
                        res += one_adress.String() + uns::string_cast<uns::nn::string_t>(";");

                    return res;
                };

                virtual bool ValueSet(std::vector<adress>& data_to_set, const uns::nn::string_t& str) override {
                    uns::nn::string_t::size_type seeker = 0;
                    
                    data_to_set.clear();
                    uns::nn::string_t temp = uns::nn::string_t();
                    while (uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(";"), false, -1)) {
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


            
            
            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Percent_gene_values, size_t,
                percent_0 = 0,
                percent_1 = 1
            );

            /*MUTATION_CHROMOSOME = 0,                //вероятность мутации для всей хромосомы
                MUTATION_GENE = 1,                      //вероятность мутации для каждого гена
                BLOCK = 2,                              //вероятность присваивания/удаления статуса блокированного гена
                DOMINATION = 3,                         //вероятность изменения значения доминантности у гена
                BLOCK_MODIFICATOR = 4,                  //декремент, меняющий все вероятности, если ген заблокирован (0-1)
                VARIABLE_MUTATION = 5,                  //вероятность мутации одной переменной 
                RANDOM_MUTATION = 6,                    //вероятность выбора типа мутации "рандомный"
                PERCENT_MODIFICATOR = 7,                //максимальный процент (0-1), на который может измениться величина при процентной мутации
                GENE_REPLACE = 8,                       //вероятность возникновения мутации перемены местами основного гена/гена данных на другое место
                GENE_COPY = 9,                          //вероятность копирования основного гена/гена данных в другое место
                GENE_ADD = 10,                          //вероятность возникновения мутации создания нового гена
                GENE_DELETE = 11,                       //вероятность возникновения мутации удаления гена
                GENE_OUT = 12,                          //вероятность присваивания/удаления статуса выходного гена
                GENE_MIX = 13,                          //удельная вероятность мутации перемешивания на один ген
                GENE_NUMMAX = 14,                       //вероятность наследования от родителя с большим числом генов
                GENE_ADDVATE = 15,                      //вероятность аддвации генов
                GRUPPA_REPRODUCT = 16,                  //вероятность наследования всей группы при размножении
                GRUPPA_SPLIT = 17,                      //вероятность сплитования выбранного основного гена
                GRUPPA_ADDVATE = 18,                    //вероятность аддвации выбранного основного гена
                PROPAGATION_ORDER = 19,                 //вероятность сохранения порядка распространения сигнала при перемене местами генов
                VECTOR_TIMECURRENT = 20,                //вероятность того, что мутация вектора будет проведена вблизи от текущего момента
                VECTOR_RANGEUSE = 21,                   //вероятность использования численного допуска при процентных мутациях векторов в основных генах
                PARAMETER_INVERTION = 22,               //вероятность возникновения инверсии параметра при рандомной мутации
                SOMETOEACHNORM = 23,                    //норма перекрытия входов нейронов при соединении SomeToEach
                REPRODUCTION_CLONING = 24,              //вероятность клонирования родительской особи при размножении
                PORTIONNORM = 25,                       //доля от общего числа генов которая берется в мутации перемешивания
                GRADIENTH_POWER = 26                    //весовой коэффициент, используемый при моделировании градиентов входных данных по векторам*/
            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Real_char_gene_values, size_t,
                real_0 = 0,
                real_1 = 1
            );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Int_char_gene_values, size_t,
                int_0 = 0,
                int_1 = 1
                );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Long_char_gene_values, size_t,
                long_0 = 0,
                long_1 = 1
                );


        };
    };
};

UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Percent_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Real_char_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Int_char_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Long_char_gene_values);


namespace uns {
    namespace nn {
        namespace genetic {



            //НЕЙРОХРОМОСОМА
            class chromosome {
            protected:

                using percent_gene_t = uns::nn::genetic::simple_gene<float>;
                using real_char_gene_t = uns::nn::genetic::simple_gene<long double>;
                using int_char_gene_t = uns::nn::genetic::simple_gene<int>;
                using long_char_gene_t = uns::nn::genetic::simple_gene<long long>;
                using general_group_t = std::vector<uns::nn::genetic::general_slot_t>;

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

                std::array<percent_gene_t, Percent_gene_values::_size()> percent_genes;
                std::array<real_char_gene_t, Real_char_gene_values::_size()> real_char_genes;
                std::array<int_char_gene_t, Int_char_gene_values::_size()> int_char_genes;
                std::array<long_char_gene_t, Long_char_gene_values::_size()> long_char_genes;
                std::vector<uns::nn::genetic::data_slot_t> data_slots;
                std::vector<general_group_t> general_groups;

                static const uns::nn::string_t delimiter;
            public:
                chromosome() {};
                chromosome(const chromosome& copying_obj) : 
                    percent_genes(copying_obj.percent_genes), 
                    real_char_genes(copying_obj.real_char_genes), 
                    int_char_genes(copying_obj.int_char_genes), 
                    long_char_genes(copying_obj.long_char_genes), 
                    data_slots(copying_obj.data_slots), 
                    general_groups(copying_obj.general_groups)
                {};
                chromosome& operator=(const chromosome& copying_obj) {
                    percent_genes = copying_obj.percent_genes; 
                    real_char_genes = copying_obj.real_char_genes; 
                    int_char_genes = copying_obj.int_char_genes;
                    long_char_genes = copying_obj.long_char_genes;
                    data_slots = copying_obj.data_slots;
                    general_groups = copying_obj.general_groups;
                };
                chromosome(chromosome&& moving_obj) : 
                    percent_genes(std::move(moving_obj.percent_genes)), 
                    real_char_genes(std::move(moving_obj.real_char_genes)), 
                    int_char_genes(std::move(moving_obj.int_char_genes)), 
                    long_char_genes(std::move(moving_obj.long_char_genes)), 
                    data_slots(std::move(moving_obj.data_slots)), 
                    general_groups(std::move(moving_obj.general_groups))
                {};
                chromosome& operator=(chromosome && moving_obj) {
                    percent_genes = std::move(moving_obj.percent_genes);
                    real_char_genes = std::move(moving_obj.real_char_genes);
                    int_char_genes = std::move(moving_obj.int_char_genes);
                    long_char_genes = std::move(moving_obj.long_char_genes);
                    data_slots = std::move(moving_obj.data_slots);
                    general_groups = std::move(moving_obj.general_groups);
                };
                ~chromosome() { /*Clear();*/ };

                uns::nn::string_t String() const {
                    uns::nn::string_t res = uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME:");
                    const uns::nn::string_t tab = uns::string_cast<uns::nn::string_t>("\t");

                    res += delimiter;
                    res += uns::string_cast<uns::nn::string_t>("PERCENT GROUP:") + delimiter;
                    for (auto gene : percent_genes)
                        res += tab + gene.String() + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":PERCENT GROUP");

                    res += delimiter;
                    res += uns::string_cast<uns::nn::string_t>("REAL CHAR GROUP:") + delimiter;
                    for (auto gene : real_char_genes)
                        res += tab + gene.String() + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":REAL CHAR GROUP");

                    res += delimiter;
                    res += uns::string_cast<uns::nn::string_t>("INT CHAR GROUP:") + delimiter;
                    for (auto gene : int_char_genes)
                        res += tab + gene.String() + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":INT CHAR GROUP");

                    res += delimiter;
                    res += uns::string_cast<uns::nn::string_t>("LONG CHAR GROUP:") + delimiter;
                    for (auto gene : long_char_genes)
                        res += tab + gene.String() + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":LONG CHAR GROUP");
                    
                    res += delimiter;
                    res += uns::string_cast<uns::nn::string_t>("DATA GROUP:") + delimiter;
                    for (auto slot : data_slots)
                        res += tab +
                        std::get<data_gene_values::DATATYPE>(slot).String() + uns::string_cast<uns::nn::string_t>(" ") +
                        std::get<data_gene_values::DATA>(slot).String() + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":DATA GROUP");


                    res += delimiter;
                    for (auto general_group : general_groups) {
                        res += uns::string_cast<uns::nn::string_t>("GENERAL GROUP:") + delimiter;
                        for (auto slot : general_group) {
                                res += tab +
                                std::get<general_gene_values::NEYROTYPE>(slot).String() + uns::string_cast<uns::nn::string_t>(" ") +
                                std::get<general_gene_values::NUMBER>(slot).String() + uns::string_cast<uns::nn::string_t>(" ") +
                                std::get<general_gene_values::OUT>(slot).String() + uns::string_cast<uns::nn::string_t>(" ") +
                                std::get<general_gene_values::ADRESS>(slot).String() + uns::string_cast<uns::nn::string_t>(" ") +
                                std::get<general_gene_values::CORE1>(slot).String() + uns::string_cast<uns::nn::string_t>(" ") +
                                std::get<general_gene_values::CORE2>(slot).String() + delimiter;                            
                        };
                        res += uns::string_cast<uns::nn::string_t>(":GENERAL GROUP") + delimiter;
                    };

                    res += uns::string_cast<uns::nn::string_t>(":NEUROCHROMOSOME");

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const uns::nn::string_t name = uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME");
                    uns::nn::string_t::size_type seeker = 0;

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":NEUROCHROMOSOME"))) { ErrorFix(UNS_FILENAME, __LINE__); return false; };
                    const auto nch_end = uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":NEUROCHROMOSOME"));
                    if (nch_end == uns::nn::string_t::npos) { ErrorFix(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    
                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("PERCENT GROUP:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":PERCENT GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":PERCENT GROUP")) == uns::nn::string_t::npos || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    for (auto& gene : percent_genes) {
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":PERCENT GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                        else if (!gene.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    };

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("REAL CHAR GROUP:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":REAL CHAR GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":REAL CHAR GROUP")) == uns::nn::string_t::npos || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    for (auto& gene : real_char_genes) {
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":REAL CHAR GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                        else if (!gene.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    };

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("INT CHAR GROUP:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":INT CHAR GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":INT CHAR GROUP")) == uns::nn::string_t::npos || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    for (auto& gene : int_char_genes) {
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":INT CHAR GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                        else if (!gene.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    };

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("LONG CHAR GROUP:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":LONG CHAR GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":LONG CHAR GROUP")) == uns::nn::string_t::npos || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    for (auto& gene : long_char_genes) {
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":LONG CHAR GROUP")) || seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                        else if (!gene.Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    };

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("DATA GROUP:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":DATA GROUP"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    const auto dg_end = uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":DATA GROUP"));
                    if (dg_end == uns::nn::string_t::npos || seeker >= dg_end || seeker >= nch_end || dg_end >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    auto next_data_slot = data_slot_t();
                    do {
                        next_data_slot = data_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter) || seeker > dg_end) break;
                        else if (!std::get<data_gene_values::DATATYPE>(next_data_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":DATA GROUP")) || seeker > dg_end) break;
                        else if (!std::get<data_gene_values::DATA>(next_data_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        data_slots.push_back(next_data_slot);
                    }
                    while (seeker < dg_end);
                    if(seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    
                    auto next_general_group = general_group_t();
                    do {
                        next_general_group = general_group_t();
                        if (!Set(seeker, next_general_group, str, nch_end)) break;

                        general_groups.push_back(next_general_group);
                    }
                    while (seeker < nch_end);
                    if (seeker >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };
            protected:
                bool Set(uns::nn::string_t::size_type& seeker, std::vector<general_slot_t>& constructing_obj, const uns::nn::string_t& str, const uns::nn::string_t::size_type& nch_end) {
                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("GENERAL GROUP:") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":GENERAL GROUP"))) { return false; };
                    const auto gg_end = uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":GENERAL GROUP"));
                    if (gg_end == uns::nn::string_t::npos || gg_end >= nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    auto next_slot = general_slot_t();
                    do {
                        next_slot = general_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter) || seeker > gg_end) break;
                        else if (!std::get<general_gene_values::NEYROTYPE>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter) || seeker > gg_end) break;
                        else if (!std::get<general_gene_values::NUMBER>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter) || seeker > gg_end) break;
                        else if (!std::get<general_gene_values::OUT>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter) || seeker > gg_end) break;
                        else if (!std::get<general_gene_values::ADRESS>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter) || seeker > gg_end) break;
                        else if (!std::get<general_gene_values::CORE1>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":GENERAL GROUP")) || seeker > gg_end) break;
                        else if (!std::get<general_gene_values::CORE2>(next_slot).Set(temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        constructing_obj.push_back(next_slot);
                    }
                    while (seeker < gg_end);

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>(":GENERAL GROUP"), false, -1, nch_end, nch_end) || seeker > nch_end) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };

            public:

                /*void Format();

                void Clear();

                void RandomFill();*/

            protected:

                percent_gene_t& Gene(const Percent_gene_values& num) noexcept { return percent_genes[num]; };
                real_char_gene_t& Gene(const Real_char_gene_values& num) noexcept { return real_char_genes[num]; };
                int_char_gene_t& Gene(const Int_char_gene_values& num) noexcept { return int_char_genes[num]; };
                long_char_gene_t& Gene(const Long_char_gene_values& num) noexcept { return long_char_genes[num]; };
                template<data_gene_values val> decltype(auto) Gene(const adress& adress) { return std::get<val>(data_slots[adress.Slot()]); };
                template<general_gene_values val> decltype(auto) Gene(const adress& adress) { return std::get<val>(general_groups[adress.Group()][adress.Slot()]); };

                //Групповые методы:
                /*void Format();

                void Clear();

                void RandomFill();*/
                //:Групповые методы

            public:

                /*bool Repair();

                bool Reproduct(const chromosome& parent1, const chromosome& parent2);

                bool Mutate();

                bool Express();

                bool Adapt();*/
            };

            const uns::nn::string_t chromosome::delimiter = uns::string_cast<uns::nn::string_t>("\n");


        };
    };
};

#undef UNS_FILENAME