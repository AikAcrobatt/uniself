#pragma once

#include <array>
#include <tuple>
#include <utility>
#include <random>
#include <memory>
#include <vector>

#include "uniself/algorithms.h"
#include "uniself/strings.h"
#include "uniself/context.h"
#include "uniself/errors.h"
#include "uniself/time.h"
#include "uniself/mathematics.h"
#include "uniself/matrix.h"


namespace uns {
    namespace nn {

        using string_t = std::wstring;


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
            number_t operator()(const number_t& lower_limit, const number_t& upper_limit) noexcept {
                if (upper_limit > lower_limit)
                    return static_cast<number_t>((upper_limit - lower_limit) * distribution(engine) + lower_limit);
                else
                    return upper_limit;
            };
        };


        //перечисления-идентификаторы параметров
        //константные
        UNS_ENUM_DECLARATOR(CTX_CBOOL, size_t,
            bool_c_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CFLOAT, size_t,
            PossMinimal = 0,                            //минимальное значение вероятности в вероятностных генах
            PersonalMutatorLowLimit                     //нижний предел мутации особи
        );

        UNS_ENUM_DECLARATOR(CTX_CDOUBLE, size_t,
            double_c_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CLDOUBLE, size_t,
            ldouble_c_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CINT, size_t,
            int_c_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CWSTRING, size_t,
            ErrorFileName = 0,                          //путь к файлу системных ошибок
            ProgramName = 1                             //имя приложения
        );

        UNS_ENUM_DECLARATOR(CTX_CUINT, size_t,
            Maximum_OutNeutons_Number = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CLLONG, size_t,
            ll_c_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CSIZE, size_t,
            MaximalCoreSize = 0                         //максимальное значение измерений ядер
        );

        UNS_ENUM_DECLARATOR(CTX_CCASCADES, size_t,
            Frame = 0,
            Neuro = 1
        );

        //неконстантные
        UNS_ENUM_DECLARATOR(CTX_BOOL, size_t,
            bool_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_FLOAT, size_t,
            StartTime = 0
        );

        UNS_ENUM_DECLARATOR(CTX_DOUBLE, size_t,
            float_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_LDOUBLE, size_t,
            ldouble_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_INT, size_t,
            int_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_STRING, size_t,
            ErrorBuffer = 0
        );

        UNS_ENUM_DECLARATOR(CTX_UINT, size_t,
            uint_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_LLONG, size_t,
            ll_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_SIZE, size_t,
            size_0 = 0
        );

        UNS_ENUM_DECLARATOR(CTX_CASCADES, size_t,
            casc_0 = 0
        );

    };
};

UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CBOOL)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CFLOAT)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CDOUBLE)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CLDOUBLE)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CINT)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CWSTRING)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CUINT)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CLLONG)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CSIZE)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CCASCADES)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_BOOL)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_FLOAT)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_DOUBLE)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_LDOUBLE)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_INT)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_STRING)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_UINT)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_LLONG)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_SIZE)
UNS_ENUM_STRING_CAST_DECLARATOR(uns::nn::CTX_CASCADES)

namespace uns {
    namespace nn {

        //контейнер каскадов мутаций
        class cascades {
        public:
            using cascade_t = std::tuple<uns::nn::string_t, std::vector<size_t>, size_t>;
        protected:
            enum {
                id = 0,
                variants = 1,
                width = 2
            };

            std::vector<cascade_t> casc;
        public:
            cascades() {};
            cascades(const cascades& copying_obj) noexcept { casc = copying_obj.casc; };
            cascades& operator=(const cascades& copying_obj) noexcept { casc = copying_obj.casc; };
            cascades(cascades&& moving_obj) noexcept { casc = std::move(moving_obj.casc); };
            cascades& operator=(cascades&& moving_obj) noexcept { casc = std::move(moving_obj.casc); };
            ~cascades() noexcept {};

            uns::nn::string_t Set(uns::nn::string_t str, const uns::nn::string_t& delimiter) {
                size_t pos = 0;
                string_t value = string_t();
                str += delimiter;
                while (true) {
                    cascade_t one_casc;
                    std::get<variants>(one_casc).clear();
                    if (uns::string::SeekerSet<string_t>(str, pos, uns::string_cast<string_t>("->"), false, -1, delimiter)) {
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::string_cast<string_t>("("), false, -1, delimiter)) {
                            std::get<id>(one_casc) = value;
                        }
                        else break;
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::string_cast<string_t>(")"), false, -1, delimiter)) {
                            if (value.size() > 0)
                                std::get<width>(one_casc) = uns::string_cast<size_t>(value);
                            else
                                std::get<width>(one_casc) = 0;
                        }
                        else break;
                        while (uns::string::SeekerRead<string_t>(str, pos, value, uns::string_cast<string_t>(","), false, -1, delimiter)) {
                            std::get<variants>(one_casc).push_back(uns::string_cast<size_t>(value));
                        };
                        casc.push_back(one_casc);
                        uns::string::SeekerSet<string_t>(str, pos, delimiter, false, -1);
                    }
                    else break;
                };
                if (Total() == 0) return uns::string_cast<string_t>("[cascades.size() == 0];");

                Check();

                return uns::string_cast<string_t>("");
            };

            uns::nn::string_t String(const uns::nn::string_t& delimiter) const {
                using string_t = uns::nn::string_t;

                size_t count = 0;
                string_t res = string_t();
                for (auto one_casc : casc) {
                    res = res + uns::string_cast<string_t>(count++) + uns::string_cast<string_t>("->") + uns::string_cast<string_t>(std::get<id>(one_casc)) + uns::string_cast<string_t>("(") + uns::string_cast<string_t>(std::get<width>(one_casc)) + uns::string_cast<string_t>(")");
                    for (auto mutations : std::get<variants>(one_casc))
                        res = res + uns::string_cast<string_t>(mutations) + uns::string_cast<string_t>(",");
                    res = res + delimiter;
                };

                return res;
            };

            size_t Total() const noexcept { return casc.size(); };

            const uns::nn::string_t& Id(size_t index) const noexcept { return std::get<id>(casc[index]); };

            size_t Find(const uns::nn::string_t& str) const noexcept {
                size_t idx = 0;
                for (auto one_casc : casc) {
                    if (std::get<id>(one_casc) == str)
                        return idx;
                    idx++;
                };
                return casc.size();
            };

            size_t VariantsTotal(size_t index) const noexcept { return std::get<variants>(casc[index]).size(); };

            size_t Variant(size_t index, size_t num_of_variant) const noexcept { return std::get<variants>(casc[index])[num_of_variant]; };

            template<typename randomizer_t> size_t Variate(size_t index, randomizer_t& rand) const noexcept {
                return Variant(index, rand(size_t(0), VariantsTotal(index) - 1));
            };

            size_t Width(size_t index) const noexcept { return std::get<width>(casc[index]); };

        protected:

            void Check() {
                size_t counter = 0;
                for (auto& one_casc : casc) {
                    for (auto& mutation : std::get<variants>(one_casc)) {
                        if (mutation >= casc.size())
                            mutation = counter;
                    };
                    if (std::get<variants>(one_casc).size() == 0) std::get<variants>(one_casc).push_back(counter);
                    counter++;
                };
            };
        };


        //контейнер контекста
        class context {
        private:
            //хранилища постоянных параметров
            std::array<bool,                CTX_CBOOL::_size()>         c_bool_param;
            std::array<double,              CTX_CFLOAT::_size()>        c_float_param;
            std::array<double,              CTX_CDOUBLE::_size()>       c_double_param;
            std::array<long double,         CTX_CLDOUBLE::_size()>      c_ldouble_param;
            std::array<int,                 CTX_CINT::_size()>          c_int_param;
            std::array<std::wstring,        CTX_CWSTRING::_size()>       c_str_param;
            std::array<unsigned int,        CTX_CUINT::_size()>         c_uint_param;
            std::array<long long,           CTX_CLLONG::_size()>        c_long_param;
            std::array<size_t,              CTX_CSIZE::_size()>         c_size_param;
            std::array<cascades,            CTX_CCASCADES::_size()>     c_casc_param;
            //хранилища динамических параметров
            std::array<bool,                CTX_BOOL::_size()>          bool_param;
            std::array<double,              CTX_FLOAT::_size()>         float_param;
            std::array<double,              CTX_DOUBLE::_size()>        double_param;
            std::array<long double,         CTX_LDOUBLE::_size()>       ldouble_param;
            std::array<int,                 CTX_INT::_size()>           int_param;
            std::array<std::string,         CTX_STRING::_size()>        str_param;
            std::array<unsigned int,        CTX_UINT::_size()>          uint_param;
            std::array<long long,           CTX_LLONG::_size()>         long_param;
            std::array<size_t,              CTX_SIZE::_size()>          size_param;
            std::array<cascades,            CTX_CASCADES::_size()>      casc_param;
            //вспомогательные поля
            bool is_valid = false;
        public:
            context() noexcept {};
            context(const context& copying_obj) = delete;
            context& operator=(const context& copying_obj) = delete;
            context(context&& moving_obj) = delete;
            context& operator=(context&& moving_obj) = delete;
            ~context() {};

            const bool&                 Get(CTX_CBOOL param) const { return c_bool_param[param]; };
            const double&               Get(CTX_CFLOAT param) const { return c_float_param[param]; };
            const double&               Get(CTX_CDOUBLE param) const { return c_double_param[param]; };
            const long double&          Get(CTX_CLDOUBLE param) const { return c_ldouble_param[param]; };
            const int&                  Get(CTX_CINT param) const { return c_int_param[param]; };
            const std::wstring&         Get(CTX_CWSTRING param) const { return c_str_param[param]; };
            const unsigned int&         Get(CTX_CUINT param) const { return c_uint_param[param]; };
            const long long&            Get(CTX_CLLONG param) const { return c_long_param[param]; };
            const size_t&               Get(CTX_CSIZE param) const { return c_size_param[param]; };
            const cascades&             Get(CTX_CCASCADES param) const { return c_casc_param[param]; };
            bool&                       Get(CTX_BOOL param) { return bool_param[param]; };
            double&                     Get(CTX_FLOAT param) { return float_param[param]; };
            double&                     Get(CTX_DOUBLE param) { return double_param[param]; };
            long double&                Get(CTX_LDOUBLE param) { return ldouble_param[param]; };
            int&                        Get(CTX_INT param) { return int_param[param]; };
            std::string&                Get(CTX_STRING param) { return str_param[param]; };
            unsigned int&               Get(CTX_UINT param) { return uint_param[param]; };
            long long&                  Get(CTX_LLONG param) { return long_param[param]; };
            size_t&                     Get(CTX_SIZE param) { return size_param[param]; };
            cascades&                   Get(CTX_CASCADES param) { return casc_param[param]; };

            std::wstring Set(const std::wstring& str, const std::wstring& delimiter);

            std::wstring String(const std::wstring& delimiter) const;

            bool Valid() const { return is_valid; };
        };

        std::wstring context::Set(const std::wstring& str, const std::wstring& delimiter) {
            std::string mistakes_report = "";

            using string_t = std::wstring;

            string_t value = string_t();
            size_t pos = 0;

            for (auto I : CTX_CCASCADES::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + delimiter, false, -1, value, delimiter + delimiter))
                    mistakes_report += uns::string_cast<std::string>(c_casc_param[I].Set(value, delimiter));
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CWSTRING::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_str_param[I] = uns::string_cast<std::wstring>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CBOOL::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_bool_param[I] = uns::string_cast<bool>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CFLOAT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_float_param[I] = uns::string_cast<double_t>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CDOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_double_param[I] = uns::string_cast<double_t>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CLDOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_ldouble_param[I] = uns::string_cast<long double>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_int_param[I] = uns::string_cast<int>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CUINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_uint_param[I] = uns::string_cast<unsigned int>(value);
                else  mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CLLONG::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_long_param[I] = uns::string_cast<long long>(value);
                else  mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CSIZE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    c_size_param[I] = uns::string_cast<size_t>(value);
                else  mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_CASCADES::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + delimiter, false, -1, value, delimiter + delimiter))
                    mistakes_report += uns::string_cast<std::string>(casc_param[I].Set(value, delimiter));
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_STRING::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    str_param[I] = uns::string_cast<std::string>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_BOOL::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    bool_param[I] = uns::string_cast<bool>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_FLOAT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    float_param[I] = uns::string_cast<double_t>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_DOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    double_param[I] = uns::string_cast<double_t>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_LDOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    ldouble_param[I] = uns::string_cast<long double>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_INT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    int_param[I] = uns::string_cast<int>(value);
                else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_UINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    uint_param[I] = uns::string_cast<unsigned int>(value);
                else  mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_LLONG::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    long_param[I] = uns::string_cast<long long>(value);
                else  mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            for (auto I : CTX_SIZE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                    size_param[I] = uns::string_cast<size_t>(value);
                else  mistakes_report += uns::string_cast<std::string>(I) + " not found;";
            };

            is_valid = (mistakes_report == "");

            return uns::string_cast<std::wstring>(mistakes_report);
        };

        std::wstring context::String(const std::wstring& delimiter) const {
            using string_t = std::wstring;
            string_t res = string_t();
            int count = 0;

            for (auto I : CTX_CCASCADES::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + delimiter + c_casc_param[I].String(delimiter) + delimiter;
            };

            for (auto I : CTX_CWSTRING::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_str_param[I]) + delimiter;
            };

            for (auto I : CTX_CBOOL::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_bool_param[I]) + delimiter;
            };

            for (auto I : CTX_CFLOAT::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_float_param[I]) + delimiter;
            };

            for (auto I : CTX_CDOUBLE::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_double_param[I]) + delimiter;
            };

            for (auto I : CTX_CLDOUBLE::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_ldouble_param[I]) + delimiter;
            };

            for (auto I : CTX_CINT::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_int_param[I]) + delimiter;
            };

            for (auto I : CTX_CUINT::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_uint_param[I]) + delimiter;
            };

            for (auto I : CTX_CLLONG::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_long_param[I]) + delimiter;
            };

            for (auto I : CTX_CSIZE::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_size_param[I]) + delimiter;
            };

            for (auto I : CTX_CASCADES::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + delimiter + casc_param[I].String(delimiter) + delimiter;
            };

            for (auto I : CTX_STRING::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(str_param[I]) + delimiter;
            };

            for (auto I : CTX_BOOL::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(bool_param[I]) + delimiter;
            };

            for (auto I : CTX_FLOAT::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(float_param[I]) + delimiter;
            };

            for (auto I : CTX_DOUBLE::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(double_param[I]) + delimiter;
            };

            for (auto I : CTX_LDOUBLE::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(ldouble_param[I]) + delimiter;
            };

            for (auto I : CTX_INT::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(int_param[I]) + delimiter;
            };

            for (auto I : CTX_UINT::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(uint_param[I]) + delimiter;
            };

            for (auto I : CTX_LLONG::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(long_param[I]) + delimiter;
            };

            for (auto I : CTX_SIZE::_values()) {
                res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(size_param[I]) + delimiter;
            };

            return res;
        };


        //менеджмент ошибок нейросетей
        bool ErrorSettle() {
            return uns::errors::Settle(ctx[CTX_CWSTRING::ErrorFileName]);
        };

        void ErrorPush(std::string error_localization_file, int line_number, std::string message = "") {
            ctx[CTX_STRING::ErrorBuffer] += error_localization_file + "|" + uns::string_cast<std::string>(line_number) + "[" + message + "] << ";
        };

        void ErrorFix(std::string error_localization_file,int line_number, std::string message = "") {
            /*uns::errors::Fix(
                uns::string_cast<std::string>(ctx[CTX_CWSTRING::ProgramName]) + ":" + uns::string_cast<std::string>(uns::from_start::Count()),
                (ctx[CTX_STRING::ErrorBuffer] != "" ? ctx[CTX_STRING::ErrorBuffer] : "") + error_localization_file + "|" + uns::string_cast<std::string>(line_number),
                message
            );*/

            std::cout << "ERRORS:" << (ctx[CTX_STRING::ErrorBuffer] != "" ? ctx[CTX_STRING::ErrorBuffer] : "") + error_localization_file + "|" + uns::string_cast<std::string>(line_number) << "[" + message + "]" << std::endl;
            ctx[CTX_STRING::ErrorBuffer] = "";
        };


        

    };
};