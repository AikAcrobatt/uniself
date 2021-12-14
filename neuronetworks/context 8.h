#pragma once


#include <fstream>
#include <array>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "uniself/enum.h"
#include "uniself/algorithms.h"
#include "uniself/strings.h"
#include "uniself/time.h"

namespace uns {

    namespace nn {
		
        //константные
        BETTER_ENUM(CTX_CBOOL, size_t,
            bool_c_0 = 0
        );

        BETTER_ENUM(CTX_CDOUBLE, size_t,
            double_c_0 = 0
        );

        BETTER_ENUM(CTX_CLDOUBLE, size_t,
            ldouble_c_0 = 0
        );

        BETTER_ENUM(CTX_CINT, size_t,
            int_c_0 = 0
        );

        BETTER_ENUM(CTX_CSTRING, size_t,
            ErrorFileName = 0,
			ProgramName = 1
        );

        BETTER_ENUM(CTX_CUINT, size_t,
            uint_c_0 = 0
        );

        BETTER_ENUM(CTX_CLLONG, size_t,
            ll_c_0 = 0
        );

        BETTER_ENUM(CTX_CSIZE, size_t,
            size_c_0 = 0
        );

        BETTER_ENUM(CTX_CCASCADES, size_t,
            Frame = 0,
            Neuro = 1
        );

        //неконстантные
        BETTER_ENUM(CTX_BOOL, size_t,
            bool_0 = 0
        );

        BETTER_ENUM(CTX_DOUBLE, size_t,
            StartTime = 0
        );

        BETTER_ENUM(CTX_LDOUBLE, size_t,
            ldouble_0 = 0
        );

        BETTER_ENUM(CTX_INT, size_t,
            int_0 = 0
        );

        BETTER_ENUM(CTX_STRING, size_t,
            str_0 = 0
        );

        BETTER_ENUM(CTX_UINT, size_t,
            uint_0 = 0
        );

        BETTER_ENUM(CTX_LLONG, size_t,
            ll_0 = 0
        );

        BETTER_ENUM(CTX_SIZE, size_t,
            size_0 = 0
        );

        BETTER_ENUM(CTX_CASCADES, size_t,
            casc_0 = 0
        );


        //контейнер каскадов мутаций
        class cascades {
        public:
            using cascade_t = std::tuple<std::wstring, std::vector<size_t>, size_t>;
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

            std::wstring Set(std::wstring str, const std::wstring& delimiter) {
                using string_t = std::wstring;
                
                size_t pos = 0;
                string_t value = string_t();
                str += delimiter;
                while (true) {
                    cascade_t one_casc;
                    std::get<variants>(one_casc).clear();
                    if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("->"), false, -1, delimiter)) {
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>("("), false, -1, delimiter)) {
                            std::get<id>(one_casc) = value;
                        }
                        else break;
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(")"), false, -1, delimiter)) {
                            if (value.size() > 0)
                                std::get<width>(one_casc) = uns::unified_cast<size_t>(value);
                            else
                                std::get<width>(one_casc) = 0;
                        }
                        else break;
                        while (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(","), false, -1, delimiter)) {
                            std::get<variants>(one_casc).push_back(uns::unified_cast<size_t>(value));
                        };
                        casc.push_back(one_casc);
                        uns::string::SeekerSet<string_t>(str, pos, delimiter, false, -1);
                    }
                    else break;
                };
                if (Total() == 0) return L"[frame_types.size() == 0];";

                Check();

                return L"";
            };

            std::wstring String(const std::wstring& delimiter) const {
                using string_t = std::wstring;

                size_t count = 0;
                string_t res = string_t();
                for (auto one_casc : casc) {
                    res = res + uns::unified_cast<string_t>(count++) + uns::unified_cast<string_t>("->") + uns::unified_cast<string_t>(std::get<id>(one_casc)) + uns::unified_cast<string_t>("(") + uns::unified_cast<string_t>(std::get<width>(one_casc)) + uns::unified_cast<string_t>(")");
                    for (auto mutations : std::get<variants>(one_casc))
                        res = res + uns::unified_cast<string_t>(mutations) + uns::unified_cast<string_t>(",");
                    res = res + delimiter;
                };

                return res;
            };

            size_t Total() const noexcept { return casc.size(); };

            const std::wstring& Id(size_t index) const noexcept { return std::get<id>(casc[index]); };

            size_t Find(const std::wstring& str) const noexcept {
                size_t idx = 0;
                for (auto one_casc : casc) {
                    if (std::get<id>(one_casc) == str)
                        return idx;
                    idx++;
                };
                return casc.size();
            };

            size_t VariantsTotal(size_t index) const noexcept { return std::get<variants>(casc[index]).size(); };

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
        class context final {
		private:  
			//хранилища посто€нных параметров
            std::array<bool, CTX_CBOOL::_size()>                        c_bool_param;
            std::array<double, CTX_CDOUBLE::_size()>                    c_double_param;
            std::array<long double, CTX_CLDOUBLE::_size()>              c_ldouble_param;
            std::array<int, CTX_CINT::_size()>                          c_int_param;
            std::array<std::wstring, CTX_CSTRING::_size()>              c_str_param;
            std::array<unsigned int, CTX_CUINT::_size()>                c_uint_param;
            std::array<long long, CTX_CLLONG::_size()>                  c_long_param;
            std::array<size_t, CTX_CSIZE::_size()>                      c_size_param;
            std::array<cascades, CTX_CCASCADES::_size()>                c_casc_param;
            //хранилища динамических параметров
            std::array<bool, CTX_BOOL::_size()>                         bool_param;
            std::array<double, CTX_DOUBLE::_size()>                     double_param;
            std::array<long double, CTX_LDOUBLE::_size()>               ldouble_param; 
            std::array<int, CTX_INT::_size()>                           int_param;
            std::array<std::wstring, CTX_STRING::_size()>               str_param;
            std::array<unsigned int, CTX_UINT::_size()>                 uint_param;
            std::array<long long, CTX_LLONG::_size()>                   long_param;
            std::array<size_t, CTX_SIZE::_size()>                       size_param;
            std::array<cascades, CTX_CASCADES::_size()>                 casc_param;
        public:
            context() noexcept {};
            context(const context& copying_obj) = delete;
            context& operator=(const context& copying_obj) = delete;
            context(context&& moving_obj) = delete;
            context& operator=(context&& moving_obj) = delete;
            ~context() {};
            const bool&         Get(CTX_CBOOL param) const { return c_bool_param[param]; };
			const double&       Get(CTX_CDOUBLE param) const { return c_double_param[param]; };
            const long double&  Get(CTX_CLDOUBLE param) const { return c_ldouble_param[param]; };
            const int&          Get(CTX_CINT param) const { return c_int_param[param]; };
            const std::wstring& Get(CTX_CSTRING param) const { return c_str_param[param]; };
            const unsigned int& Get(CTX_CUINT param) const { return c_uint_param[param]; };
            const long long&    Get(CTX_CLLONG param) const { return c_long_param[param]; };
            const size_t&       Get(CTX_CSIZE param) const { return c_size_param[param]; };
            const cascades&     Get(CTX_CCASCADES param) const { return c_casc_param[param]; };
            bool&               Get(CTX_BOOL param) { return bool_param[param]; };
			double&             Get(CTX_DOUBLE param) { return double_param[param]; };
            long double&        Get(CTX_LDOUBLE param) { return ldouble_param[param]; };
            int&                Get(CTX_INT param) { return int_param[param]; };
            std::wstring&       Get(CTX_STRING param) { return str_param[param]; };
            unsigned int&       Get(CTX_UINT param) { return uint_param[param]; };
            long long&          Get(CTX_LLONG param) { return long_param[param]; };
            size_t&             Get(CTX_SIZE param) { return size_param[param]; };
            cascades&           Get(CTX_CASCADES param) { return casc_param[param]; };
            std::wstring Set(const std::wstring& str, const std::wstring& delimiter, bool return_dynamic_data = true);
            std::wstring String(const std::wstring& delimiter, bool return_dynamic_data = true) const;
        };

        std::wstring context::Set(const std::wstring& str, const std::wstring& delimiter, bool set_dynamic_data) {
            std::string mistakes_report = "";

            using string_t = std::wstring;

            string_t value = string_t();
            size_t pos = 0;

            for (auto I : CTX_CSTRING::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_str_param[I] = value;
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            for (auto I : CTX_CCASCADES::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>(":") + delimiter, false, -1, value, delimiter + delimiter))
                    mistakes_report += uns::unified_cast<std::string>(c_casc_param[I].Set(value, delimiter));
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            for (auto I : CTX_CBOOL::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_bool_param[I] = uns::unified_cast<bool>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };
			
			for (auto I : CTX_CDOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_double_param[I] = uns::unified_cast<double_t>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            for (auto I : CTX_CLDOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_ldouble_param[I] = uns::unified_cast<long double>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };
			
            for (auto I : CTX_CINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_int_param[I] = uns::unified_cast<int>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            for (auto I : CTX_CUINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_uint_param[I] = uns::unified_cast<unsigned int>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            for (auto I : CTX_CLLONG::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_long_param[I] = uns::unified_cast<long long>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            for (auto I : CTX_CSIZE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_size_param[I] = uns::unified_cast<size_t>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
            };

            if (set_dynamic_data) {
                for (auto I : CTX_STRING::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        str_param[I] = value;
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };

                for (auto I : CTX_CASCADES::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>(":") + delimiter, false, -1, value, delimiter + delimiter))
                        mistakes_report += uns::unified_cast<std::string>(casc_param[I].Set(value, delimiter));
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };

                for (auto I : CTX_BOOL::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        bool_param[I] = uns::unified_cast<bool>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };
				
				for (auto I : CTX_DOUBLE::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        double_param[I] = uns::unified_cast<double_t>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };

                for (auto I : CTX_LDOUBLE::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        ldouble_param[I] = uns::unified_cast<long double>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };
				
                for (auto I : CTX_INT::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        int_param[I] = uns::unified_cast<int>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };

                for (auto I : CTX_UINT::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        uint_param[I] = uns::unified_cast<unsigned int>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };

                for (auto I : CTX_LLONG::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        long_param[I] = uns::unified_cast<long long>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };

                for (auto I : CTX_SIZE::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        size_param[I] = uns::unified_cast<size_t>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + " not found;";
                };
            };

            return uns::unified_cast<std::wstring>(mistakes_report);
        };

        std::wstring context::String(const std::wstring& delimiter, bool return_dynamic_data) const {
            using string_t = std::wstring;
            string_t res = string_t();
            int count = 0;

            for (auto I : CTX_CSTRING::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_str_param[I]) + delimiter;
            };

            for (auto I : CTX_CCASCADES::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>(":") + delimiter + c_casc_param[I].String(delimiter) + delimiter;
            };

            for (auto I : CTX_CBOOL::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_bool_param[I]) + delimiter;
            };

            for (auto I : CTX_CDOUBLE::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_double_param[I]) + delimiter;
            };

            for (auto I : CTX_CLDOUBLE::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_ldouble_param[I]) + delimiter;
            };

            for (auto I : CTX_CINT::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_int_param[I]) + delimiter;
            };

            for (auto I : CTX_CUINT::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_uint_param[I]) + delimiter;
            };

            for (auto I : CTX_CLLONG::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_long_param[I]) + delimiter;
            };

            for (auto I : CTX_CSIZE::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_size_param[I]) + delimiter;
            };

            if (return_dynamic_data) {
                for (auto I : CTX_STRING::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(str_param[I]) + delimiter;
                };

                for (auto I : CTX_CASCADES::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>(":") + delimiter + casc_param[I].String(delimiter) + delimiter;
                };

                for (auto I : CTX_BOOL::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(bool_param[I]) + delimiter;
                };

                for (auto I : CTX_DOUBLE::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(double_param[I]) + delimiter;
                };

                for (auto I : CTX_LDOUBLE::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(ldouble_param[I]) + delimiter;
                };

                for (auto I : CTX_INT::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(int_param[I]) + delimiter;
                };

                for (auto I : CTX_UINT::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(uint_param[I]) + delimiter;
                };

                for (auto I : CTX_LLONG::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(long_param[I]) + delimiter;
                };

                for (auto I : CTX_SIZE::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(size_param[I]) + delimiter;
                };
            };

            return res;
        };


        //интерфейс диспетчера контекста
        class context_manager_interface
        {
        public:
            virtual std::wstring Settle(const std::wstring& str, const std::wstring& delimiter, bool set_dynamic_data) = 0; //”станавливает валидный контекст по строке, если нет валидного контекста, иначе, возвращает сообщение об этом
            virtual std::wstring String(const std::wstring& delimiter, bool return_dynamic_data) = 0;                              // онвертирует текущий контекст в строку
            virtual context& Access() = 0;                                                                                  //ѕредоставл€ет доступ к открытым методам и членам контекста, если контекст инвалиден, поведение неопределено
            virtual std::ofstream& OErr() = 0;                                                                              //ѕредоставл€ет доступ к потоку ошибок текущего контекста, если контекст инвалиден, поведение неопределено
            virtual void Drop() = 0;                                                                                        //ƒелает контекст инвалидным
        };


        //диспетчер однопоточного контекста
        class singlethread_context_manager : public context_manager_interface
        {
        protected:
            bool context_is_valid = false;
            context ctx;
            std::ofstream errstream;
        public:
			singlethread_context_manager() noexcept {};
			singlethread_context_manager(const singlethread_context_manager& copying_obj) = delete;
			singlethread_context_manager& operator=(const singlethread_context_manager& copying_obj) = delete;
			singlethread_context_manager(singlethread_context_manager&& moving_obj) = delete;
			singlethread_context_manager& operator=(singlethread_context_manager&& moving_obj) = delete;
            ~singlethread_context_manager() { if (!errstream.bad()) errstream.close(); };

            std::wstring Settle(const std::wstring& str, const std::wstring& delimiter, bool set_dynamic_data) override {
                if (context_is_valid) return L"There is some context already;";
                
                auto res = ctx.Set(str, delimiter, set_dynamic_data);
                if (res == L"") context_is_valid = true;

                errstream.open(Access().Get(CTX_CSTRING::ErrorFileName), std::ios::binary | std::ios::app);//подумать про составное им€ файла ошибок (директори€ + сам файл)
                if (errstream.bad()) res += L"Opening ErrFile failed;";

                return res;
            };

            std::wstring String(const std::wstring& delimiter, bool return_dynamic_data) override { return ctx.String(delimiter, return_dynamic_data); };

            context& Access() override { return ctx; };

            std::ofstream& OErr() override { return errstream; };
            void Drop() override {
                context_is_valid = false;
                if (!OErr().bad()) OErr().close();
            };
        };


        //диспетчер многопоточных контекстов
        class multithread_context_manager : public context_manager_interface
        {
        protected:
            std::unordered_map<std::thread::id, uns::nn::context*> ctxs;
            std::unordered_map<std::thread::id, std::ofstream*> errs;
            std::recursive_mutex mtx;
        public:
			multithread_context_manager() noexcept {};
			multithread_context_manager(const multithread_context_manager& copying_obj) = delete;
			multithread_context_manager& operator=(const multithread_context_manager& copying_obj) = delete;
			multithread_context_manager(multithread_context_manager&& moving_obj) = delete;
			multithread_context_manager& operator=(multithread_context_manager&& moving_obj) = delete;
            ~multithread_context_manager() {
                for (auto ctx : ctxs)
					if(ctx.second != nullptr)
						delete ctx.second;
                for (auto err : errs)
                    if (err.second != nullptr) {
                        if (!err.second->bad()) err.second->close();
                        delete err.second;
                    };
            };

            std::wstring Settle(const std::wstring& str, const std::wstring& delimiter, bool set_dynamic_data) override {
                for (auto ctx : ctxs)
                    if (ctx.first == std::this_thread::get_id())
                        return L"There is some context already;";

                std::lock_guard<std::recursive_mutex> lock(mtx);

                auto emplacing_result1 = ctxs.emplace(std::this_thread::get_id(), new uns::nn::context);
                if (!emplacing_result1.second) return L"Context addition failed;";

                auto res = Access().Set(str, delimiter, set_dynamic_data);

                std::thread::id thread_id_with_same_errfile;
                bool match_flag = false;
                for (auto ctx : ctxs)
                    if (ctx.second->Get(CTX_CSTRING::ErrorFileName) == Access().Get(CTX_CSTRING::ErrorFileName) && ctx.first != std::this_thread::get_id()) {//подумать про составное им€ файла ошибок (директори€ + сам файл)
                        thread_id_with_same_errfile = ctx.first;
                        match_flag = true;
                        break;
                    };

                if (!match_flag) {
                    auto emplacing_result2 = errs.emplace(std::this_thread::get_id(), new std::ofstream(Access().Get(CTX_CSTRING::ErrorFileName), std::ios::binary | std::ios::app));//подумать про составное им€ файла ошибок (директори€ + сам файл)
                    if (!emplacing_result2.second || OErr().bad()) res += L"Opening ErrFile failed;";
                }
                else {
                    auto emplacing_result2 = errs.emplace(std::this_thread::get_id(), errs.at(thread_id_with_same_errfile));
                    if (!emplacing_result2.second || OErr().bad()) res += L"Opening ErrFile failed;";
                };

                return res;
            };

            std::wstring String(const std::wstring& delimiter, bool return_dynamic_data) override {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                return Access().String(delimiter, return_dynamic_data);
            };

            context& Access() override { return *(ctxs.at(std::this_thread::get_id())); };

            std::ofstream& OErr() override { return *(errs.at(std::this_thread::get_id())); };

            void Drop() override {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                delete ctxs.at(std::this_thread::get_id());
                ctxs.erase(std::this_thread::get_id());

                if (!OErr().bad()) OErr().close();
            };
        };


        //механизм доступа к контексту
        class context_access_obj {
		protected:
            context_manager_interface* ctx;
        public:
            context_access_obj() noexcept : ctx(nullptr) {};
			context_access_obj(const context_access_obj& copying_obj) = delete;
			context_access_obj& operator=(const context_access_obj& copying_obj) = delete;
			context_access_obj(context_access_obj&& moving_obj) = delete;
			context_access_obj& operator=(context_access_obj&& moving_obj) = delete;
            ~context_access_obj() { if (ctx != nullptr) delete ctx; };

            template<class context_manager_t> bool Define() noexcept {
                if (ctx != nullptr) return false;
                ctx = new context_manager_t;
                return true;
            };

            bool Valid() const noexcept { return (ctx != nullptr && !ctx->OErr().bad()); };

            template<typename string_t> std::wstring Settle(const string_t& str, const std::wstring& delimiter, bool set_dynamic_data = false) {
                if (ctx == nullptr) return L"The context is not defined;";
                auto res = ctx->Settle(uns::unified_cast<std::wstring>(str), uns::unified_cast<std::wstring>(delimiter), set_dynamic_data);
					
				return res;
            };

            template<typename string_t> string_t String(const string_t& delimiter, bool return_dynamic_data) {
                return uns::unified_cast<std::wstring>(ctx->Access().String(uns::unified_cast<std::wstring>(delimiter), return_dynamic_data));
            };

            void ErrorFix(const std::string& error, const std::string& message) {
                if (ctx->OErr().bad()) return;
                if (error == "" && message == "") return;
                ctx->OErr() << uns::unified_cast<std::string>(ctx->Access().Get(CTX_CSTRING::ProgramName)) << ":: time_from_start:" << uns::unified_cast<std::string>(uns::from_start::Double()) << " st_time:" << uns::unified_cast<std::string>(ctx->Access().Get(CTX_DOUBLE::StartTime)) << ((error != "") ? " err:" : "") << error << ((message != "") ? " msg:" : "") << message << std::endl; //нужно будет в time.h переделать методы from_start
            };

            template<typename enum_input_t> decltype(auto) operator()(const enum_input_t& value_enumerator) noexcept {
                return ctx->Access().Get(value_enumerator);
            };

        };

    };

};