#pragma once

#include <array>
#include <tuple>
#include <random>
#include <utility>

#include "uniself/neuronetworks/nn_general.h"
#include "uniself/neuronetworks/nn_networks.h"
#include "uniself/neuronetworks/nn_neurons.h"

#define UNS_FILENAME "nn_genetics.h"

namespace uns {
    namespace nn {
        namespace genetic {


            //Рандомизатор
            template<typename floating_point_t, typename engine_t>
            class randomizer {
            protected:
                std::random_device random_device;
                engine_t engine;
                std::uniform_real_distribution<floating_point_t> distribution;
            public:
                using value_t = floating_point_t;

                randomizer() noexcept : engine(random_device()), distribution(0.0F, 1.0F) {};
                randomizer(const randomizer<floating_point_t, engine_t>& copying_obj) noexcept : engine(copying_obj.engine), distribution(copying_obj.distribution) {};
                randomizer& operator=(const randomizer<floating_point_t, engine_t>& copying_obj) noexcept { engine = copying_obj.engine; distribution = copying_obj.distribution; };
                randomizer(randomizer<floating_point_t, engine_t>&& moving_obj) noexcept : engine(std::move(moving_obj.engine)), distribution(std::move(moving_obj.distribution)) {};
                randomizer& operator=(randomizer<floating_point_t, engine_t>&& moving_obj) noexcept { engine = std::move(moving_obj.engine); distribution = std::move(moving_obj.distribution); };
                ~randomizer() noexcept {};

                void Refresh() noexcept {
                    engine.seed(random_device());
                };

                floating_point_t operator()() noexcept { return distribution(engine); };

                bool operator()(const floating_point_t& probability) noexcept { return probability > distribution(engine); };

                template<typename number_t>
                number_t operator()(const number_t& lower_limit, const number_t& upper_limit) noexcept { return static_cast<number_t>((upper_limit - lower_limit) * distribution(engine) + lower_limit); };
            };


            //СЛУЖЕБНЫЕ ПЕРЕЧИСЛЕНИЯ
            //Типы ограниченности переменных при вариации
            UNS_ENUM_DECLARATOR(VARIATION_LIMITS_TYPE, int,
                left,
                right
            );

            using dominancy_t = float;


            //Контейнер гена
            template<typename data_t>
            class gene {
            public:
                using value_t = data_t;
            protected:
                bool block = false;
                uns::nn::genetic::dominancy_t dominant = 0F;
                data_t data;
            public:
                gene() : block(false), dominant(0.0F), data() {};
                gene(const gene<data_t>& copying_obj) : block(copying_obj.block), dominant(copying_obj.dominant), data(copying_obj.data) {};
                gene<data_t>& operator=(const gene<data_t>& copying_obj) { block = copying_obj.block; dominant = copying_obj.dominant; data = copying_obj.data; };
                gene(gene<data_t>&& moving_obj) : block(moving_obj.block), dominant(moving_obj.dominant), data(moving_obj.data) {};
                gene<data_t>& operator=(gene<data_t>&& moving_obj) { block = moving_obj.block; dominant = moving_obj.dominant; data = std::move(moving_obj.data); };
                ~gene() {};

                bool& Block() noexcept { return block; };
                bool Block() const noexcept { return block; };

                uns::nn::genetic::dominancy_t& Dominant() noexcept { return dominant; };
                uns::nn::genetic::dominancy_t Dominant() const noexcept { return dominant; };

                data_t& Value() noexcept { return data; };
                data_t Value() const noexcept { return data; };
            };


            
            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Percent_gene_values, size_t,
                MUTATION_CHROMOSOME = 0,                //вероятность мутации для всей хромосомы
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
                GRADIENTH_POWER = 26                    //весовой коэффициент, используемый при моделировании градиентов входных данных по векторам
            );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Real_char_gene_values, size_t,
                real_0 = 0                //вероятность мутации для всей хромосомы
            );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Int_char_gene_values, size_t,
                int_0 = 0                //вероятность мутации для всей хромосомы
                );

            //Предназначения данных, кодируемых процентными генами
            UNS_ENUM_DECLARATOR(Long_char_gene_values, size_t,
                long_0 = 0                //вероятность мутации для всей хромосомы
                );

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

UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Percent_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Real_char_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Int_char_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::Long_char_gene_values);
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::genetic::layer_connection_type);

namespace uns {
    namespace nn {
        namespace genetic {


            using signal_t = float;

            class chromosome;

            template<typename gene_collection_t, typename gene_t>
            class gene_group {
            protected:

            };


            //НЕЙРОХРОМОСОМА
            class chromosome {
            protected:

                class adress;

                using percent_gene_t = uns::nn::genetic::gene<float>;
                using real_char_gene_t = uns::nn::genetic::gene<long double>;
                using int_char_gene_t = uns::nn::genetic::gene<int>;
                using long_char_gene_t = uns::nn::genetic::gene<long long>;
                class datatype_gene_t : public uns::nn::genetic::gene<size_t> {};
                using data_gene_t = uns::nn::genetic::gene<std::pair<size_t, size_t>>;
                class neurotype_gene_t : public uns::nn::genetic::gene<size_t> {};
                using adress_gene_t = uns::nn::genetic::gene<std::vector<adress>>;
                using core1_gene_t = uns::nn::genetic::gene<ublas::matrix<signal_t>>;
                using core2_gene_t = uns::nn::genetic::gene<ublas::matrix<signal_t>>;
                using neuron_number_gene_t = uns::nn::genetic::gene<size_t>;
                using out_neuron_gene_t = uns::nn::genetic::gene<int>;
                using data_slot_t = std::tuple<datatype_gene_t, data_gene_t>;
                using general_slot_t = std::tuple<neurotype_gene_t, adress_gene_t, core1_gene_t, core2_gene_t, neuron_number_gene_t, out_neuron_gene_t>;
                using general_group_t = std::vector<general_slot_t>;

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
                        if (general_slot != nullptr)  data_slot= nullptr;
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

                        data_slot = nullptr; general_slot = nullptr;

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

                enum data_gene_values {
                    DATATYPE = 0,
                    DATA
                };

                enum general_gene_values {
                    NEYROTYPE = 0,
                    ADRESS,
                    CORE1,
                    CORE2,
                    NUMBER,
                    OUT
                };

                std::array<percent_gene_t, Percent_gene_values::_size()> percent_genes;
                std::array<real_char_gene_t, Real_char_gene_values::_size()> real_char_genes;
                std::array<int_char_gene_t, Int_char_gene_values::_size()> int_char_genes;
                std::array<long_char_gene_t, Long_char_gene_values::_size()> long_char_genes;
                std::vector<data_slot_t> data_slots;
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
                ~chromosome() { Clear(); };

                uns::nn::string_t String() const {
                    uns::nn::string_t res = uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME:") + delimiter;

                    res +=
                        String(percent_genes, uns::string_cast<uns::nn::string_t>("PERCENT GROUP")) + delimiter +
                        String(real_char_genes, uns::string_cast<uns::nn::string_t>("REAL CHAR GROUP")) + delimiter +
                        String(int_char_genes, uns::string_cast<uns::nn::string_t>("INT CHAR GROUP")) + delimiter +
                        String(long_char_genes, uns::string_cast<uns::nn::string_t>("LONG CHAR GROUP")) + delimiter +
                        String(data_slots) + delimiter;
                    for (auto gen_group : general_groups)
                        res += String(gen_group) + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":NEUROCHROMOSOME");

                    return res;
                };

                bool Set(const uns::nn::string_t& str) {
                    const uns::nn::string_t name = uns::string_cast<uns::nn::string_t>("NEUROCHROMOSOME");
                    uns::nn::string_t::size_type seeker = 0;

                    if (!uns::string::SeekerSet(str, seeker, name + uns::string_cast<uns::nn::string_t>(":") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + name)) { ErrorFix(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":") + name) == uns::nn::string_t::npos) { ErrorFix(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    uns::nn::string_t group_name = uns::nn::string_t();
                    
                    if (!Set(percent_genes, uns::string_cast<uns::nn::string_t>("PERCENT GROUP"), str)) { ErrorFix(UNS_FILENAME, __LINE__); return false; };

                    if (!Set(real_char_genes, uns::string_cast<uns::nn::string_t>("REAL CHAR GROUP"), str)) { ErrorFix(UNS_FILENAME, __LINE__); return false; };

                    if (!Set(int_char_genes, uns::string_cast<uns::nn::string_t>("INT CHAR GROUP"), str)) { ErrorFix(UNS_FILENAME, __LINE__); return false; };

                    if (!Set(long_char_genes, uns::string_cast<uns::nn::string_t>("LONG CHAR GROUP"), str)) { ErrorFix(UNS_FILENAME, __LINE__); return false; };

                    if (!Set(data_slots, str)) { ErrorFix(UNS_FILENAME, __LINE__); return false; };
                    
                    uns::nn::string_t::size_type seeker = 0;
                    auto next_slot = general_group_t();
                    do {
                        next_slot = general_group_t();
                        if (!Set(seeker,next_slot,str)) break;

                        general_groups.push_back(next_slot);
                    }
                    while (!uns::string::SeekerSet(str, seeker, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + name));

                    return true;
                };

                void Format();

                void Clear();

                void RandomFill();

            protected:

                percent_gene_t& Gene(const Percent_gene_values& num) noexcept { return percent_genes[num]; };
                real_char_gene_t& Gene(const Real_char_gene_values& num) noexcept { return real_char_genes[num]; };
                int_char_gene_t& Gene(const Int_char_gene_values& num) noexcept { return int_char_genes[num]; };
                long_char_gene_t& Gene(const Long_char_gene_values& num) noexcept { return long_char_genes[num]; };
                template<general_gene_values val> decltype(auto) Gene(const adress& adress) { return std::get<val>(general_groups[adress.Group()][adress.Slot()]); };
                template<data_gene_values val> decltype(auto) Gene(const adress& adress) { return std::get<val>(data_slots[adress.Slot()]); };

                //Генные методы:
                template<typename gene_t> uns::nn::string_t GeneValueToString(const gene_t& gene) const { return uns::string_cast<uns::nn::string_t>(gene.Value()); };
                uns::nn::string_t GeneValueToString<datatype_gene_t>(const datatype_gene_t& gene) const { return ctx[CTX_CCASCADES::Frame].Id(gene.Value()); };
                uns::nn::string_t GeneValueToString<neurotype_gene_t>(const neurotype_gene_t& gene) const { return ctx[CTX_CCASCADES::Neuro].Id(gene.Value()); };
                uns::nn::string_t GeneValueToString<uns::nn::genetic::gene<std::pair<size_t, size_t>>>(const uns::nn::genetic::gene<std::pair<size_t, size_t>>& gene) const { return uns::string_cast<uns::nn::string_t>(gene.Value().first) + uns::string_cast<uns::nn::string_t>(",") + uns::string_cast<uns::nn::string_t>(gene.Value().second); };
                uns::nn::string_t GeneValueToString<uns::nn::genetic::gene<std::vector<adress>>>(const uns::nn::genetic::gene<std::vector<adress>>& gene) const {
                    uns::nn::string_t res = uns::nn::string_t();

                    for (auto one_adress : gene.Value())
                        res += one_adress.String() + uns::string_cast<uns::nn::string_t>(";");

                    return res;
                };
                uns::nn::string_t GeneValueToString<uns::nn::genetic::gene<ublas::matrix<signal_t>>>(const uns::nn::genetic::gene<ublas::matrix<signal_t>>& gene) const {
                    return uns::math::matrix_to_string<uns::nn::string_t>("\n") +
                        uns::math::matrix_to_string<uns::nn::string_t>(
                            gene.Value(),
                            uns::string_cast<uns::nn::string_t>("\n{{"),
                            uns::string_cast<uns::nn::string_t>(","),
                            uns::string_cast<uns::nn::string_t>("},\n{"),
                            uns::string_cast<uns::nn::string_t>("}}")) +
                        uns::math::matrix_to_string<uns::nn::string_t>("\n");
                };

                template<typename gene_t>
                bool FillGene(gene_t& gene, const uns::nn::string_t& str) { gene.Value() = uns::string_cast<typename gene::value_t>(str); return true; };
                bool FillGene<datatype_gene_t>(datatype_gene_t& gene, const uns::nn::string_t& str) { gene.Value() = ctx[CTX_CCASCADES::Frame].Find(str); return gene.Value() < ctx[CTX_CCASCADES::Frame].Total(); };
                bool FillGene<neurotype_gene_t>(neurotype_gene_t& gene, const uns::nn::string_t& str) { gene.Value() = ctx[CTX_CCASCADES::Neuro].Find(str); return gene.Value() < ctx[CTX_CCASCADES::Neuro].Total(); };
                bool FillGene<uns::nn::genetic::gene<std::pair<size_t, size_t>>>(uns::nn::genetic::gene<std::pair<size_t, size_t>>& gene, const uns::nn::string_t& str) {
                    uns::nn::string_t::size_type seeker = 0;

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(","), false, 0)) return false;
                    else gene.Value().first = uns::string_cast<size_t>(temp);
                    gene.Value().second = uns::string_cast<size_t>(str.substr(seeker));

                    return true;
                };
                bool FillGene<uns::nn::genetic::gene<std::vector<adress>>>(uns::nn::genetic::gene<std::vector<adress>>& gene, const uns::nn::string_t& str) {
                    uns::nn::string_t::size_type seeker = 0;

                    gene.Value().clear();
                    uns::nn::string_t temp = uns::nn::string_t();
                    while (uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(";"), false, 0)) {
                        gene.Value().push_back(adress());
                        if (!gene.Value()[gene.Value().size() - 1].Set(str)) return false;
                    };

                    return true;
                };
                bool FillGene<uns::nn::genetic::gene<ublas::matrix<signal_t>>>(uns::nn::genetic::gene<ublas::matrix<signal_t>>& gene, const uns::nn::string_t& str) { 
                    return uns::math::string_to_matrix(
                        gene.Value(), 
                        str, 
                        uns::string_cast<uns::nn::string_t>("\n{{"), 
                        uns::string_cast<uns::nn::string_t>(","), 
                        uns::string_cast<uns::nn::string_t>("},\n{"), 
                        uns::string_cast<uns::nn::string_t>("}}")); 
                };

                template<typename gene_t>
                uns::nn::string_t String(const gene_t& printing_obj) const {
                    return uns::string_cast<uns::nn::string_t>("[") +
                        uns::string_cast<uns::nn::string_t>(printing_obj.Block()) + uns::string_cast<uns::nn::string_t>(",") +
                        uns::string_cast<uns::nn::string_t>(printing_obj.Dominant()) +
                        uns::string_cast<uns::nn::string_t>("(") + GeneValueToString<gene_t>(printing_obj) + uns::string_cast<uns::nn::string_t>(")]");
                };

                template<typename gene_t>
                bool Set(gene_t& constructing_obj, const uns::nn::string_t& str) {
                    uns::nn::string_t::size_type seeker = 0;

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>("["), false, -1, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(")]")) == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(","), false, -1, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else constructing_obj.Block() = uns::string_cast<bool>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>("("), false, -1, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else constructing_obj.Dominant() = uns::string_cast<uns::nn::genetic::dominancy_t>(temp);
                    if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(")"), false, 0, uns::string_cast<uns::nn::string_t>(")]"))) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                    else if (!FillGene<gene_t>(constructing_obj, temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };
                //:Генные методы

                //Групповые методы:
                template<typename group_t> uns::nn::string_t String(const group_t& printing_obj, const uns::nn::string_t& group_name) const {
                    uns::nn::string_t res = group_name + uns::string_cast<uns::nn::string_t>(":") + delimiter;

                    for (auto gene : printing_obj)
                        res += String(gene) + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":") + group_name;

                    return res;
                };
                uns::nn::string_t String(const std::vector<data_slot_t>& printing_obj) const {
                    const uns::nn::string_t group_name = uns::string_cast<uns::nn::string_t>("DATA GROUP");
                    uns::nn::string_t res = group_name + uns::string_cast<uns::nn::string_t>(":") + delimiter;

                    for (auto slot : printing_obj)
                        res += 
                            String(std::get<0>(slot)) + uns::string_cast<uns::nn::string_t>(" ") + 
                            String(std::get<1>(slot)) + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":") + group_name;

                    return res;
                };
                uns::nn::string_t String(const general_group_t& printing_obj) const {
                    const uns::nn::string_t group_name = uns::string_cast<uns::nn::string_t>("GENERAL GROUP");
                    uns::nn::string_t res = group_name + uns::string_cast<uns::nn::string_t>(":") + delimiter;

                    for (auto slot : printing_obj)
                        res +=
                            String(std::get<0>(slot)) + uns::string_cast<uns::nn::string_t>(" ") +
                            String(std::get<1>(slot)) + uns::string_cast<uns::nn::string_t>(" ") +
                            String(std::get<2>(slot)) + uns::string_cast<uns::nn::string_t>(" ") +
                            String(std::get<3>(slot)) + uns::string_cast<uns::nn::string_t>(" ") +
                            String(std::get<4>(slot)) + uns::string_cast<uns::nn::string_t>(" ") +
                            String(std::get<5>(slot)) + delimiter;
                    res += uns::string_cast<uns::nn::string_t>(":") + group_name;

                    return res;
                };

                template<typename group_t> bool Set(group_t& constructing_obj, const uns::nn::string_t& group_name, const uns::nn::string_t& str) {
                    if (!uns::string::SeekerSet(str, seeker, group_name + uns::string_cast<uns::nn::string_t>(":") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + name)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":") + group_name) == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    for (auto& gene : constructing_obj) {
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + group_name)) { ErrorPush(UNS_FILENAME, __LINE__); return false; }
                        else Set(gene, temp);
                    };

                    return true;
                };
                bool Set(std::vector<data_slot_t>& constructing_obj, const uns::nn::string_t& str) {
                    const uns::nn::string_t group_name = uns::string_cast<uns::nn::string_t>("DATA GROUP");
                    uns::nn::string_t::size_type seeker = 0;

                    if (!uns::string::SeekerSet(str, seeker, group_name + uns::string_cast<uns::nn::string_t>(":") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + group_name)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":") + group_name) == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    auto next_slot = data_slot_t();
                    do {
                        next_slot = data_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter)) break;
                        else if (!Set(std::get<0>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, true, -1)) break;
                        else if (!Set(std::get<1>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        constructing_obj.push_back(next_slot);
                    }
                    while (uns::string::SeekerSet(str, seeker, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + group_name));

                    return true;
                };
                bool Set(uns::nn::string_t::size_type seeker, std::vector<general_slot_t>& constructing_obj, const uns::nn::string_t& str) {
                    const uns::nn::string_t group_name = uns::string_cast<uns::nn::string_t>("GENERAL GROUP");

                    if (!uns::string::SeekerSet(str, seeker, group_name + uns::string_cast<uns::nn::string_t>(":") + delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + group_name)) { return false; };
                    if (uns::string::SeekerFind(str, seeker, uns::string_cast<uns::nn::string_t>(":") + group_name) == uns::nn::string_t::npos) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    uns::nn::string_t temp = uns::nn::string_t();
                    auto next_slot = general_slot_t();
                    do {
                        next_slot = general_slot_t();
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter)) break;
                        else if (!Set(std::get<0>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter)) break;
                        else if (!Set(std::get<1>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter)) break;
                        else if (!Set(std::get<2>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter)) break;
                        else if (!Set(std::get<3>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, uns::string_cast<uns::nn::string_t>(" "), false, -1, delimiter)) break;
                        else if (!Set(std::get<4>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };
                        if (!uns::string::SeekerRead(str, seeker, temp, delimiter, true, -1)) break;
                        else if (!Set(std::get<5>(next_slot), temp)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                        constructing_obj.push_back(next_slot);
                    }
                    while (uns::string::SeekerSet(str, seeker, delimiter, false, -1, uns::string_cast<uns::nn::string_t>(":") + group_name));

                    if (!uns::string::SeekerSet(str, seeker, uns::string_cast<uns::nn::string_t>(":") + group_name, false, -1)) { ErrorPush(UNS_FILENAME, __LINE__); return false; };

                    return true;
                };

                void Format();

                void Clear();

                void RandomFill();
                //:Групповые методы

            public:

                bool Repair();

                bool Reproduct(const chromosome& parent1, const chromosome& parent2);

                bool Mutate();

                bool Express();

                bool Adapt();
            };

            const uns::nn::string_t chromosome::delimiter = uns::string_cast<uns::nn::string_t>("\n");


        };
    };
};

#undef UNS_FILENAME