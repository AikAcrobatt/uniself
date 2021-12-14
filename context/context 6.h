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

        BETTER_ENUM(CTX_CBASIC, size_t,
            basic_c_0 = 0
        );

        BETTER_ENUM(CTX_CDOUBLE, size_t,
            double_c_0 = 0
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

        //неконстантные
        BETTER_ENUM(CTX_BOOL, size_t,
            bool_0 = 0
        );

        BETTER_ENUM(CTX_BASIC, size_t,
            basic_0 = 0
        );

        BETTER_ENUM(CTX_DOUBLE, size_t,
            StartTime = 0
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

        template<typename basic_number_t, typename string_t>
        class context;

        template<typename basic_number_t, typename string_t>
        class frame_access_obj;

        template<typename basic_number_t, typename string_t>
        class neuro_access_obj;


        //объект контекста
        template<typename basic_number_t, typename string_t>
        class context final {
            friend frame_access_obj<basic_number_t, string_t>;
            friend neuro_access_obj<basic_number_t, string_t>;
        public:
            using cascade_frame = std::tuple<string_t, std::vector<size_t>, size_t>;
            using cascade_neuro = std::pair<string_t, std::vector<size_t>>;
		protected:
		    using double_t = double;
			
            enum {
                id = 0,
                variants = 1,
                width = 2
            };
		public:
            template<typename basic_number_t, typename string_t>
            class frame_access_obj {
            protected:
                context<basic_number_t, string_t>& ref;
            public:
				frame_access_obj() = delete;
                frame_access_obj(context<basic_number_t, string_t>& obj) noexcept : ref(obj) {};
				frame_access_obj(const frame_access_obj<basic_number_t, string_t>& copying_obj) = delete;
				frame_access_obj<basic_number_t, string_t>& operator=(const frame_access_obj<basic_number_t, string_t>& copying_obj) = delete;
				frame_access_obj(frame_access_obj<basic_number_t, string_t>&& moving_obj) = delete;
				frame_access_obj<basic_number_t, string_t>& operator=(frame_access_obj<basic_number_t, string_t>&& moving_obj) = delete;
                ~frame_access_obj() noexcept {};
                size_t Total() const noexcept { return ref.frame_types.size(); };
                string_t Id(size_t index) const noexcept { return std::get<id>(ref.frame_types[index]); };
                size_t Find(string_t str) const noexcept {
                    size_t idx = 0;
                    for (auto frame : ref.frame_types) {
                        if (std::get<id>(frame) == str)
                            return idx;
                        idx++;
                    };
                    return ref.frame_types.size();
                };
                size_t VariantsTotal(size_t index) const noexcept { return std::get<variants>(ref.frame_types[index]).size(); };
                size_t Width(size_t index) const noexcept { return std::get<width>(ref.frame_types[index]); };
            };

            template<typename basic_number_t, typename string_t>
            class neuro_access_obj {
            protected:
                context<basic_number_t, string_t>& ref;
            public:
				neuro_access_obj() = delete;
                neuro_access_obj(context<basic_number_t, string_t>& obj) noexcept : ref(obj) {};
                neuro_access_obj(const neuro_access_obj<basic_number_t, string_t>& copying_obj) = delete;
                neuro_access_obj<basic_number_t, string_t>& operator=(const neuro_access_obj<basic_number_t, string_t>& copying_obj) = delete;
                neuro_access_obj(neuro_access_obj<basic_number_t, string_t>&& moving_obj) = delete;  
                neuro_access_obj<basic_number_t, string_t>& operator=(neuro_access_obj<basic_number_t, string_t>&& moving_obj) = delete;
                ~neuro_access_obj() noexcept {};
                size_t Total() const noexcept { return ref.neuro_types.size(); };
                string_t Id(size_t index) const noexcept { return std::get<id>(ref.neuro_types[index]); };
                size_t Find(string_t str) const noexcept {
                    size_t idx = 0;
                    for (auto neuro : ref.neuro_types) {
                        if (std::get<id>(neuro) == str)
                            return idx;
                        idx++;
                    };
                    return ref.neuro_types.size();
                };
                size_t VariantsTotal(size_t index) const noexcept { return std::get<variants>(ref.neuro_types[index]).size(); };
            };

        protected:  
			//хранилища посто€нных параметров
            std::array<bool, CTX_CBOOL::_size()>                        c_bool_param;
            std::array<basic_number_t, CTX_CBASIC::_size()>             c_basic_param;
            std::array<double_t, CTX_CDOUBLE::_size()>                  c_double_param;
            std::array<int, CTX_CINT::_size()>                          c_int_param;
            std::array<string_t, CTX_CSTRING::_size()>                  c_str_param;
            std::array<unsigned int, CTX_CUINT::_size()>                c_uint_param;
            std::array<long long, CTX_CLLONG::_size()>                  c_long_param;
            std::array<size_t, CTX_CSIZE::_size()>                      c_size_param;
            std::vector<cascade_frame>                                  frame_types;
            std::vector<cascade_neuro>                                  neuro_types;
            //хранилища динамических параметров
            std::array<bool, CTX_BOOL::_size()>                         bool_param;
            std::array<basic_number_t, CTX_BASIC::_size()>              basic_param; 
            std::array<double_t, CTX_DOUBLE::_size()>                   double_param;
            std::array<int, CTX_INT::_size()>                           int_param;
            std::array<string_t, CTX_STRING::_size()>                   str_param;
            std::array<unsigned int, CTX_UINT::_size()>                 uint_param;
            std::array<long long, CTX_LLONG::_size()>                   long_param;
            std::array<size_t, CTX_SIZE::_size()>                       size_param;
        public:
            //вспомогательные члены
            frame_access_obj<basic_number_t, string_t>                  Frame;
            neuro_access_obj<basic_number_t, string_t>                  Neuro;
        public:
            context() noexcept : Frame(*this), Neuro(*this) {};
            context(const context<basic_number_t, string_t>& copying_obj) = delete;
            context<basic_number_t, string_t>& operator=(const context<basic_number_t, string_t>& copying_obj) = delete;
            context(context<basic_number_t, string_t>&& moving_obj) = delete;
            context<basic_number_t, string_t>& operator=(context<basic_number_t, string_t>&& moving_obj) = delete;
            ~context() {};
            bool                Get(CTX_CBOOL param) const { return c_bool_param[param]; };
            basic_number_t      Get(CTX_CBASIC param) const { return c_basic_param[param]; };
			double_t            Get(CTX_CDOUBLE param) const { return c_double_param[param]; };
            int                 Get(CTX_CINT param) const { return c_int_param[param]; };
            string_t            Get(CTX_CSTRING param) const { return c_str_param[param]; };
            unsigned int        Get(CTX_CUINT param) const { return c_uint_param[param]; };
            long long           Get(CTX_CLLONG param) const { return c_long_param[param]; };
            size_t              Get(CTX_CSIZE param) const { return c_size_param[param]; };
            bool&               Get(CTX_BOOL param) { return bool_param[param]; };
            basic_number_t&     Get(CTX_BASIC param) { return basic_param[param]; };
			double_t&           Get(CTX_DOUBLE param) { return double_param[param]; };
            int&                Get(CTX_INT param) { return int_param[param]; };
            string_t&           Get(CTX_STRING param) { return str_param[param]; };
            unsigned int&       Get(CTX_UINT param) { return uint_param[param]; };
            long long&          Get(CTX_LLONG param) { return long_param[param]; };
            size_t&             Get(CTX_SIZE param) { return size_param[param]; };
            std::string Set(const string_t& str, string_t delimiter, bool return_dynamic_data = true);
            string_t String(string_t delimiter, bool return_dynamic_data = true) const;
            void CheckCascades();
        };

        template<typename basic_number_t, typename string_t>
        std::string context<basic_number_t, string_t>::Set(const string_t& str, string_t delimiter, bool set_dynamic_data) {
            std::string mistakes_report = "";

            string_t value = string_t();
            size_t pos = 0;

            for (auto I : CTX_CSTRING::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_str_param[I] = value;
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            pos = 0;
            cascade_frame frame_value;
            if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("cascade_frame:") + delimiter, false, -1)) {
                while (true) {
                    std::get<variants>(frame_value).clear();
                    if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("->"), false, -1, delimiter)) {
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>("("), false, -1, delimiter)) {
                            std::get<id>(frame_value) = value;
                        }
                        else break;
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(")"), false, -1, delimiter)) {
                            std::get<width>(frame_value) = uns::unified_cast<size_t>(value);
                        }
                        else break;
                        while (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(","), false, -1, delimiter)) {
                            std::get<variants>(frame_value).push_back(uns::unified_cast<size_t>(value));
                        };
                        frame_types.push_back(frame_value);
                        uns::string::SeekerSet<string_t>(str, pos, delimiter, false, -1);
                    }
                    else break;
                };
            };
            if (frame_types.size() == 0) mistakes_report += "[frame_types.size() == 0];";

            pos = 0;
            cascade_neuro neuro_value;
            if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("cascade_neuro:") + delimiter, false, -1)) {
                while (true) {
                    std::get<variants>(neuro_value).clear();
                    if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("->"), false, -1, delimiter)) {
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>("()"), false, -1, delimiter)) {
                            std::get<id>(neuro_value) = value;
                        }
                        else break;
                        while (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(","), false, -1, delimiter)) {
                            std::get<variants>(neuro_value).push_back(uns::unified_cast<size_t>(value));
                        };
                        neuro_types.push_back(neuro_value);
                        uns::string::SeekerSet<string_t>(str, pos, delimiter, false, -1);
                    }
                    else break;
                };
            };
            if (neuro_types.size() == 0) mistakes_report += "[neuro_types.size() == 0];";

            for (auto I : CTX_CBOOL::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_bool_param[I] = uns::unified_cast<bool>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_CBASIC::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_basic_param[I] = uns::unified_cast<basic_number_t>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };
			
			for (auto I : CTX_CDOUBLE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_double_param[I] = uns::unified_cast<double_t>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };
			
            for (auto I : CTX_CINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_int_param[I] = uns::unified_cast<int>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_CUINT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_uint_param[I] = uns::unified_cast<unsigned int>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_CLLONG::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_long_param[I] = uns::unified_cast<long long>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_CSIZE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_size_param[I] = uns::unified_cast<size_t>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            if (set_dynamic_data) {
                for (auto I : CTX_STRING::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        str_param[I] = value;
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_BOOL::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        bool_param[I] = uns::unified_cast<bool>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_BASIC::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        basic_param[I] = uns::unified_cast<basic_number_t>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };
				
				for (auto I : CTX_DOUBLE::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        double_param[I] = uns::unified_cast<double_t>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };
				
                for (auto I : CTX_INT::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        int_param[I] = uns::unified_cast<int>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_UINT::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        uint_param[I] = uns::unified_cast<unsigned int>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_LLONG::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        long_param[I] = uns::unified_cast<long long>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_SIZE::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        size_param[I] = uns::unified_cast<size_t>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };
            };

            CheckCascades();

            return mistakes_report;
        };

        template<typename basic_number_t, typename string_t>
        string_t context<basic_number_t, string_t>::String(string_t delimiter, bool return_dynamic_data) const {
            string_t res = string_t();
            int count = 0;

            for (auto I : CTX_CSTRING::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + c_str_param[I] + delimiter;
            };

            count = 0;
            res = res + uns::unified_cast<string_t>("cascade_frame:") + delimiter;
            for (auto frame : frame_types) {
                res = res + uns::unified_cast<string_t>(count++) + uns::unified_cast<string_t>("->") + std::get<id>(frame) + uns::unified_cast<string_t>("(") + uns::unified_cast<string_t>(std::get<width>(frame)) + uns::unified_cast<string_t>(")");
                for (auto frame_mutations : std::get<variants>(frame))
                    res = res + uns::unified_cast<string_t>(frame_mutations) + uns::unified_cast<string_t>(",");
                res = res + delimiter;
            };
            res = res + delimiter;

            count = 0;
            res = res + uns::unified_cast<string_t>("cascade_neuro:") + delimiter;
            for (auto neuro : neuro_types) {
                res = res + uns::unified_cast<string_t>(count++) + uns::unified_cast<string_t>("->") + uns::unified_cast<string_t>(std::get<id>(neuro)) + uns::unified_cast<string_t>("()");
                for (auto neuro_mutations : std::get<variants>(neuro))
                    res = res + uns::unified_cast<string_t>(neuro_mutations) + uns::unified_cast<string_t>(",");
                res = res + delimiter;
            };
            res = res + delimiter;

            for (auto I : CTX_CBOOL::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_bool_param[I]) + delimiter;
            };

            for (auto I : CTX_CBASIC::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_basic_param[I]) + delimiter;
            };

            for (auto I : CTX_CDOUBLE::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_double_param[I]) + delimiter;
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
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + str_param[I] + delimiter;
                };

                for (auto I : CTX_BOOL::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(bool_param[I]) + delimiter;
                };

                for (auto I : CTX_BASIC::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(basic_param[I]) + delimiter;
                };

                for (auto I : CTX_DOUBLE::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(double_param[I]) + delimiter;
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

        template<typename basic_number_t, typename string_t>
        void context<basic_number_t, string_t>::CheckCascades() {
            size_t counter = 0;
            for (auto& frame : frame_types) {
                for (auto& frame_mutation : std::get<variants>(frame)) {
                    if (frame_mutation >= frame_types.size())
                        frame_mutation = counter;
                };
                if (std::get<variants>(frame).size() == 0) std::get<variants>(frame).push_back(counter);
                counter++;
            };

            counter = 0;
            for (auto& neuro : neuro_types) {
                for (auto& neuro_mutation : std::get<variants>(neuro)) {
                    if (neuro_mutation >= neuro_types.size())
                        neuro_mutation = counter;
                };
                if (std::get<variants>(neuro).size() == 0) std::get<variants>(neuro).push_back(counter);
                counter++;
            };
        };


        template<typename basic_number_t, typename string_t>
        class context_manager_interface
        {
        public:
            virtual std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data) = 0; //”станавливает валидный контекст по строке, если нет валидного контекста, иначе, возвращает сообщение об этом
            virtual string_t String(string_t delimiter, bool return_dynamic_data) = 0;                      // онвертирует текущий контекст в строку
            virtual context<basic_number_t, string_t>& Access() = 0;                                        //ѕредоставл€ет доступ к открытым методам и членам контекста, если контекст инвалиден, поведение неопределено
            virtual std::ofstream& OErr() = 0;                                                              //ѕредоставл€ет доступ к потоку ошибок текущего контекста, если контекст инвалиден, поведение неопределено
            virtual void Drop() = 0;                                                                        //ƒелает контекст инвалидным
        };


        template<typename basic_number_t, typename string_t>
        class singlethread_context_manager : public context_manager_interface<basic_number_t, string_t>
        {
        protected:
            bool context_is_valid = false;
            context<basic_number_t, string_t> ctx;
            std::ofstream errstream;
        public:
			singlethread_context_manager() noexcept {};
			singlethread_context_manager(const singlethread_context_manager<basic_number_t, string_t>& copying_obj) = delete;
			singlethread_context_manager<basic_number_t, string_t>& operator=(const singlethread_context_manager<basic_number_t, string_t>& copying_obj) = delete;
			singlethread_context_manager(singlethread_context_manager<basic_number_t, string_t>&& moving_obj) = delete;
			singlethread_context_manager<basic_number_t, string_t>& operator=(singlethread_context_manager<basic_number_t, string_t>&& moving_obj) = delete;
            ~singlethread_context_manager() { if (!errstream.bad()) errstream.close(); };
            std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data) override { 
                if (context_is_valid) return "There is some context already;";
                
                auto res = ctx.Set(str, delimiter, set_dynamic_data);
                if (res == "") context_is_valid = true;

                errstream.open(Access().Get(CTX_CSTRING::ErrorFileName), std::ios::binary | std::ios::app);//подумать про составное им€ файла ошибок (директори€ + сам файл)
                if (Access().errstream.bad()) res += "Opening ErrFile failed;";

                return res;
            };
            string_t String(string_t delimiter, bool return_dynamic_data) override { return ctx.String(delimiter, return_dynamic_data); };
            context<basic_number_t, string_t>& Access() override { return &ctx; };
            std::ofstream& OErr() override { return &errstream; };
            void Drop() override {
                context_is_valid = false;
                if (!OErr().bad()) OErr().close();
            };
        };


        template<typename basic_number_t, typename string_t>
        class multithread_context_manager : public context_manager_interface<basic_number_t, string_t>
        {
        protected:
            std::unordered_map<std::thread::id, uns::nn::context<basic_number_t, string_t>*> ctxs;
            std::unordered_map<std::thread::id, std::ofstream*> errs;
            std::recursive_mutex mtx;
        public:
			multithread_context_manager() noexcept {};
			multithread_context_manager(const multithread_context_manager<basic_number_t, string_t>& copying_obj) = delete;
			multithread_context_manager<basic_number_t, string_t>& operator=(const multithread_context_manager<basic_number_t, string_t>& copying_obj) = delete;
			multithread_context_manager(multithread_context_manager<basic_number_t, string_t>&& moving_obj) = delete;
			multithread_context_manager<basic_number_t, string_t>& operator=(multithread_context_manager<basic_number_t, string_t>&& moving_obj) = delete;
            ~multithread_context_manager() {
                for (auto ctx : ctxs)
					if(ctx.second != nullptr)
						delete ctx.second;
                for (auto err : errs)
                    if (err.second != nullptr) {
                        if (!err.second.bad()) err.second->close();
                        delete err.second;
                    };
            };
            std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data) {
                for (auto ctx : ctxs)
                    if (ctx.first == std::this_thread::get_id())
                        return "There is some context already;";

                std::lock_guard<std::recursive_mutex> lock(mtx);

                auto emplacing_result1 = ctxs.emplace(std::this_thread::get_id(), new uns::nn::context<basic_number_t, string_t>);
                if (!emplacing_result1.second) return "Context addition failed;";

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
                    if (!emplacing_result2.second || OErr().bad()) res += "Opening ErrFile failed;";
                }
                else {
                    auto emplacing_result2 = errs.emplace(std::this_thread::get_id(), errs.at(thread_id_with_same_errfile));
                    if (!emplacing_result2.second || OErr().bad()) res += "Opening ErrFile failed;";
                };

                return res;
            };
            string_t String(string_t delimiter, bool return_dynamic_data) override {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                return Access().String(delimiter, return_dynamic_data);
            };
            context<basic_number_t, string_t>& Access() override { return *(ctxs.at(std::this_thread::get_id())); };
            std::ofstream& OErr() override { return *(errs.at(std::this_thread::get_id())); };
            void Drop() override {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                delete ctxs.at(std::this_thread::get_id());
                ctxs.erase(std::this_thread::get_id());

                if (!OErr().bad()) OErr().close();
            };
        };

        namespace FRAME_ACS {
		
			enum FRAME_CASCADE_ACCESS_TYPE {
	            Frame
	        };

	        enum NEURO_CASCADE_ACCESS_TYPE {
	            Neuro
	        };

		};

        template<typename basic_number_t, typename string_t>
        class context_access_obj {
		public:
			using basic_t = basic_number_t;
			using string = string_t;
        protected:
            context_manager_interface<basic_number_t, string_t>* ctx;
        public:
            context_access_obj() noexcept : ctx(nullptr) {};
			context_access_obj(const context_access_obj<basic_number_t, string_t>& copying_obj) = delete;
			context_access_obj<basic_number_t, string_t>& operator=(const context_access_obj<basic_number_t, string_t>& copying_obj) = delete;
			context_access_obj(context_access_obj<basic_number_t, string_t>&& moving_obj) = delete;
			context_access_obj<basic_number_t, string_t>& operator=(context_access_obj<basic_number_t, string_t>&& moving_obj) = delete;
            ~context_access_obj() { if (ctx != nullptr) delete ctx; };
            template<template<typename type_of_basic_numbers,typename type_of_string> class context_manager_t>
            bool Define() noexcept {
                if (ctx != nullptr) return false;
                ctx = new context_manager_t<basic_number_t, string_t>;
                return true;
            };
            bool Valid() const noexcept { return (ctx != nullptr && !ctx->OErr().bad()); };
            std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data = false) {
                if (ctx == nullptr) return "The context is not defined;";
                auto res = ctx->Settle(str, delimiter, set_dynamic_data);
					
				return res;
            };
            string_t String(string_t delimiter, bool return_dynamic_data) {
                return ctx->Access().String(delimiter, return_dynamic_data);
            };
            void ErrorFix(const std::string& error, const std::string& message) {
                if (ctx->OErr().bad()) return;
                if (error == "" && message == "") return;
                ctx->OErr() << uns::unified_cast<std::string>(ctx->Access().Get(CTX_CSTRING::ProgramName)) << ":: time_from_start:" << uns::unified_cast<std::string>(uns::from_start::Double()) << " st_time:" << uns::unified_cast<std::string>(ctx->Access().Get(CTX_DOUBLE::StartTime)) << ((error != "") ? " err:" : "") << error << ((message != "") ? " msg:" : "") << message << std::endl; //нужно будет в time.h переделать методы from_start
            };
            template<typename enum_input_t>
            decltype(auto) operator()(const enum_input_t& value_enumerator) noexcept {
                return ctx->Access().Get(value_enumerator);
            };
            uns::nn::context<basic_number_t, string_t>::frame_access_obj<basic_number_t, string_t>& operator()(const FRAME_ACS::FRAME_CASCADE_ACCESS_TYPE& value_enumerator) noexcept {
                return ctx->Access().Frame;
            };
            uns::nn::context<basic_number_t, string_t>::neuro_access_obj<basic_number_t, string_t>& operator()(const FRAME_ACS::NEURO_CASCADE_ACCESS_TYPE& value_enumerator) noexcept {
                return ctx->Access().Neuro;
            };

        };

#ifdef UNS_NN_BASIC_T
#ifdef UNS_NN_STRING_T
		context_access_obj<UNS_NN_BASIC_T, UNS_NN_STRING_T> ctx;
#undef UNS_NN_BASIC_T
#undef UNS_NN_STRING_T
#endif
#endif

    };

};