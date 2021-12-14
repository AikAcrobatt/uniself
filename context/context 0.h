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

namespace uns {

    namespace nn {

        //константные
        BETTER_ENUM(CTX_PARAM_CONST_BOOL, size_t,
            bool_c_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_CONST_BASIC, size_t,
            basic_c_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_CONST_INT, size_t,
            int_c_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_CONST_STRING, size_t,
            str_c_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_CONST_UNSIGNED_INT, size_t,
            uint_c_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_CONST_LONG_LONG, size_t,
            ll_c_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_CONST_SIZE, size_t,
            size_c_0 = 0
        );

        //неконстантные
        BETTER_ENUM(CTX_PARAM_BOOL, size_t,
            bool_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_BASIC, size_t,
            basic_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_INT, size_t,
            int_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_STRING, size_t,
            str_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_UNSIGNED_INT, size_t,
            uint_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_LONG_LONG, size_t,
            ll_0 = 0
        );

        BETTER_ENUM(CTX_PARAM_SIZE, size_t,
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
        class context final
        {
            friend frame_access_obj<basic_number_t, string_t>;
            friend neuro_access_obj<basic_number_t, string_t>;
        public:
            using cascade_frame = std::tuple<string_t, size_t, std::vector<size_t>>;
            using cascade_neuro = std::pair<string_t, std::vector<size_t>>;

            template<typename basic_number_t, typename string_t>
            class frame_access_obj {
            protected:
                context<basic_number_t, string_t>& ref;
            public:
                frame_access_obj(context<basic_number_t, string_t>& obj) noexcept : ref(obj) {};
                ~frame_access_obj() noexcept {};
                size_t Total() const noexcept { return ref.frame_types.size(); };
                string_t Id(size_t index) const noexcept { return std::get<0>(ref.frame_types[index]); };
                size_t Find(string_t str) const noexcept {
                    size_t idx = 0;
                    for (auto frame : ref.frame_types) {
                        if (std::get<0>(frame) == str)
                            return idx;
                        idx++;
                    };
                    return ref.frame_types.size();
                };
                size_t VariantsTotal(size_t index) const noexcept { return std::get<2>(ref.frame_types[index]).size(); };
                size_t Width(size_t index) const noexcept { return std::get<1>(ref.frame_types[index]); };
            };

            template<typename basic_number_t, typename string_t>
            class neuro_access_obj {
            protected:
                context<basic_number_t, string_t>& ref;
            public:
                neuro_access_obj(context<basic_number_t, string_t>& obj) noexcept : ref(obj) {};
                neuro_access_obj<basic_number_t, string_t>& operator=(const neuro_access_obj<basic_number_t, string_t>& copying_obj) = delete;
                neuro_access_obj<basic_number_t, string_t>& operator=(neuro_access_obj<basic_number_t, string_t>&& mooving_obj) = delete;
                ~neuro_access_obj() noexcept {};
                size_t Total() const noexcept { return ref.neuro_types.size(); };
                string_t Id(size_t index) const noexcept { return std::get<0>(ref.neuro_types[index]); };
                size_t Find(string_t str) const noexcept {
                    size_t idx = 0;
                    for (auto neuro : ref.neuro_types) {
                        if (std::get<0>(neuro) == str)
                            return idx;
                        idx++;
                    };
                    return ref.neuro_types.size();
                };
                size_t VariantsTotal(size_t index) const noexcept { return std::get<1>(ref.neuro_types[index]).size(); };
            };

        protected:
            //хранилища постоянных параметров
            std::array<bool, CTX_PARAM_CONST_BOOL::_size()>                         c_bool_param;
            std::array<basic_number_t, CTX_PARAM_CONST_BASIC::_size()>              c_basic_param;
            std::array<int, CTX_PARAM_CONST_INT::_size()>                           c_int_param;
            std::array<string_t, CTX_PARAM_CONST_STRING::_size()>                   c_str_param;
            std::array<unsigned int, CTX_PARAM_CONST_UNSIGNED_INT::_size()>         c_uint_param;
            std::array<long long, CTX_PARAM_CONST_LONG_LONG::_size()>               c_long_param;
            std::array<size_t, CTX_PARAM_CONST_SIZE::_size()>                       c_size_param;
            std::vector<cascade_frame>                                              frame_types;
            std::vector<cascade_neuro>                                              neuro_types;
            //хранилища динамических параметров
            std::array<bool, CTX_PARAM_BOOL::_size()>                               bool_param;
            std::array<basic_number_t, CTX_PARAM_BASIC::_size()>                    basic_param;
            std::array<int, CTX_PARAM_INT::_size()>                                 int_param;
            std::array<string_t, CTX_PARAM_STRING::_size()>                         str_param;
            std::array<unsigned int, CTX_PARAM_UNSIGNED_INT::_size()>               uint_param;
            std::array<long long, CTX_PARAM_LONG_LONG::_size()>                     long_param;
            std::array<size_t, CTX_PARAM_SIZE::_size()>                             size_param;
        public:
            //вспомогательные члены
            std::ofstream                                                           errstream;
            frame_access_obj<basic_number_t, string_t>                              Frame;
            neuro_access_obj<basic_number_t, string_t>                              Neuro;
        public:
            context() : Frame(*this), Neuro(*this) {};
            context<basic_number_t, string_t>& operator=(const context<basic_number_t, string_t>& copying_obj) = delete;
            context<basic_number_t, string_t>& operator=(context<basic_number_t, string_t>&& mooving_obj) = delete;
            ~context() { if (!errstream.bad()) errstream.close(); };
            bool                Get(CTX_PARAM_CONST_BOOL param) const { return c_bool_param[param]; };
            basic_number_t      Get(CTX_PARAM_CONST_BASIC param) const { return c_basic_param[param]; };
            int                 Get(CTX_PARAM_CONST_INT param) const { return c_int_param[param]; };
            string_t            Get(CTX_PARAM_CONST_STRING param) const { return c_str_param[param]; };
            unsigned int        Get(CTX_PARAM_CONST_UNSIGNED_INT param) const { return c_uint_param[param]; };
            long long           Get(CTX_PARAM_CONST_LONG_LONG param) const { return c_long_param[param]; };
            size_t              Get(CTX_PARAM_CONST_SIZE param) const { return c_size_param[param]; };
            bool& Get(CTX_PARAM_BOOL param) { return bool_param[param]; };
            basic_number_t& Get(CTX_PARAM_BASIC param) { return basic_param[param]; };
            int& Get(CTX_PARAM_INT param) { return int_param[param]; };
            string_t& Get(CTX_PARAM_STRING param) { return str_param[param]; };
            unsigned int& Get(CTX_PARAM_UNSIGNED_INT param) { return uint_param[param]; };
            long long& Get(CTX_PARAM_LONG_LONG param) { return long_param[param]; };
            size_t& Get(CTX_PARAM_SIZE param) { return size_param[param]; };
            std::string Set(const string_t& str, string_t delimiter, bool return_dynamic_data = true);
            string_t String(string_t delimiter, bool return_dynamic_data = true) const;
            void CheckCascades();
        };

        template<typename basic_number_t, typename string_t>
        std::string context<basic_number_t, string_t>::Set(const string_t& str, string_t delimiter, bool set_dynamic_data) {
            std::string mistakes_report = "";

            string_t value = string_t();
            size_t pos = 0;

            for (auto I : CTX_PARAM_CONST_STRING::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_str_param[I] = value;
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            pos = 0;
            cascade_frame frame_value;
            if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("cascade_frame:") + delimiter, false, -1)) {
                while (true) {
                    std::get<2>(frame_value).clear();
                    if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("->"), false, -1, delimiter)) {
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>("("), false, -1, delimiter)) {
                            std::get<0>(frame_value) = value;
                        }
                        else break;
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(")"), false, -1, delimiter)) {
                            std::get<1>(frame_value) = uns::unified_cast<size_t>(value);
                        }
                        else break;
                        while (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(","), false, -1, delimiter)) {
                            std::get<2>(frame_value).push_back(uns::unified_cast<size_t>(value));
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
                    std::get<1>(neuro_value).clear();
                    if (uns::string::SeekerSet<string_t>(str, pos, uns::unified_cast<string_t>("->"), false, -1, delimiter)) {
                        if (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>("()"), false, -1, delimiter)) {
                            std::get<0>(neuro_value) = value;
                        }
                        else break;
                        while (uns::string::SeekerRead<string_t>(str, pos, value, uns::unified_cast<string_t>(","), false, -1, delimiter)) {
                            std::get<1>(neuro_value).push_back(uns::unified_cast<size_t>(value));
                        };
                        neuro_types.push_back(neuro_value);
                        uns::string::SeekerSet<string_t>(str, pos, delimiter, false, -1);
                    }
                    else break;
                };
            };
            if (neuro_types.size() == 0) mistakes_report += "[neuro_types.size() == 0];";

            for (auto I : CTX_PARAM_CONST_BOOL::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_bool_param[I] = uns::unified_cast<bool>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_PARAM_CONST_BASIC::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_basic_param[I] = uns::unified_cast<basic_number_t>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };
            for (auto I : CTX_PARAM_CONST_INT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_int_param[I] = uns::unified_cast<int>(value);
                else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_PARAM_CONST_UNSIGNED_INT::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_uint_param[I] = uns::unified_cast<unsigned int>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_PARAM_CONST_LONG_LONG::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_long_param[I] = uns::unified_cast<long long>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            for (auto I : CTX_PARAM_CONST_SIZE::_values()) {
                if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                    c_size_param[I] = uns::unified_cast<size_t>(value);
                else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
            };

            if (set_dynamic_data) {
                for (auto I : CTX_PARAM_STRING::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        str_param[I] = value;
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_PARAM_BOOL::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        bool_param[I] = uns::unified_cast<bool>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_PARAM_BASIC::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        basic_param[I] = uns::unified_cast<basic_number_t>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };
                for (auto I : CTX_PARAM_INT::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        int_param[I] = uns::unified_cast<int>(value);
                    else mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_PARAM_UNSIGNED_INT::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        uint_param[I] = uns::unified_cast<unsigned int>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_PARAM_LONG_LONG::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        long_param[I] = uns::unified_cast<long long>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };

                for (auto I : CTX_PARAM_SIZE::_values()) {
                    if (uns::string::ReadBySample<string_t>(str, uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("="), false, -1, value, delimiter))
                        size_param[I] = uns::unified_cast<size_t>(value);
                    else  mistakes_report += uns::unified_cast<std::string>(I._to_string()) + ";";
                };
            };

            return mistakes_report;
        };

        template<typename basic_number_t, typename string_t>
        string_t context<basic_number_t, string_t>::String(string_t delimiter, bool return_dynamic_data) const {
            string_t res = string_t();
            int count = 0;

            for (auto I : CTX_PARAM_CONST_STRING::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + c_str_param[I] + delimiter;
            };

            count = 0;
            res = res + uns::unified_cast<string_t>("cascade_frame:") + delimiter;
            for (auto frame : frame_types) {
                res = res + uns::unified_cast<string_t>(count++) + uns::unified_cast<string_t>("->") + std::get<0>(frame) + uns::unified_cast<string_t>("(") + uns::unified_cast<string_t>(std::get<1>(frame)) + uns::unified_cast<string_t>(")");
                for (auto frame_mutations : std::get<2>(frame))
                    res = res + uns::unified_cast<string_t>(frame_mutations) + uns::unified_cast<string_t>(",");
                res = res + delimiter;
            };
            res = res + delimiter;

            count = 0;
            res = res + uns::unified_cast<string_t>("cascade_neuro:") + delimiter;
            for (auto neuro : neuro_types) {
                res = res + uns::unified_cast<string_t>(count++) + uns::unified_cast<string_t>("->") + uns::unified_cast<string_t>(std::get<0>(neuro)) + uns::unified_cast<string_t>("()");
                for (auto neuro_mutations : std::get<1>(neuro))
                    res = res + uns::unified_cast<string_t>(neuro_mutations) + uns::unified_cast<string_t>(",");
                res = res + delimiter;
            };
            res = res + delimiter;

            for (auto I : CTX_PARAM_CONST_BOOL::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_bool_param[I]) + delimiter;
            };

            for (auto I : CTX_PARAM_CONST_BASIC::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_basic_param[I]) + delimiter;
            };

            for (auto I : CTX_PARAM_CONST_INT::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_int_param[I]) + delimiter;
            };

            for (auto I : CTX_PARAM_CONST_UNSIGNED_INT::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_uint_param[I]) + delimiter;
            };

            for (auto I : CTX_PARAM_CONST_LONG_LONG::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_long_param[I]) + delimiter;
            };

            for (auto I : CTX_PARAM_CONST_SIZE::_values()) {
                res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(c_size_param[I]) + delimiter;
            };

            if (return_dynamic_data) {
                for (auto I : CTX_PARAM_STRING::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + str_param[I] + delimiter;
                };

                for (auto I : CTX_PARAM_BOOL::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(bool_param[I]) + delimiter;
                };

                for (auto I : CTX_PARAM_BASIC::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(basic_param[I]) + delimiter;
                };

                for (auto I : CTX_PARAM_INT::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(int_param[I]) + delimiter;
                };

                for (auto I : CTX_PARAM_UNSIGNED_INT::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(uint_param[I]) + delimiter;
                };

                for (auto I : CTX_PARAM_LONG_LONG::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(long_param[I]) + delimiter;
                };

                for (auto I : CTX_PARAM_SIZE::_values()) {
                    res = res + uns::unified_cast<string_t>(I._to_string()) + uns::unified_cast<string_t>("=") + uns::unified_cast<string_t>(size_param[I]) + delimiter;
                };
            };

            return res;
        };

        template<typename basic_number_t, typename string_t>
        void context<basic_number_t, string_t>::CheckCascades() {
            size_t counter = 0;
            for (auto& frame : frame_types) {
                for (auto& frame_mutation : std::get<2>(frame)) {
                    if (frame_mutation >= frame_types.size())
                        frame_mutation = counter;
                };
                if (std::get<2>(frame).size() == 0) std::get<2>(frame).push_back(counter);
                counter++;
            };

            counter = 0;
            for (auto& neuro : neuro_types) {
                for (auto& neuro_mutation : std::get<1>(neuro)) {
                    if (neuro_mutation >= neuro_types.size())
                        neuro_mutation = counter;
                };
                if (std::get<1>(neuro).size() == 0) std::get<1>(neuro).push_back(counter);
                counter++;
            };
        };


        template<typename basic_number_t, typename string_t>
        class context_manager_interface
        {
        public:
            virtual std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data) = 0;
            virtual context<basic_number_t, string_t>& Access() = 0;
        };


        template<typename basic_number_t, typename string_t>
        class singlethread_context_manager : public context_manager_interface<basic_number_t, string_t>
        {
        protected:
            context<basic_number_t, string_t> ctx;
        public:
            std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data) override { return ctx.Set(str, delimiter, set_dynamic_data); };
            context<basic_number_t, string_t>& Access() override { return &ctx; };
        };


        template<typename basic_number_t, typename string_t>
        class multithread_context_manager : public context_manager_interface<basic_number_t, string_t>
        {
        protected:
            std::unordered_map<std::thread::id, uns::nn::context<basic_number_t, string_t>*> ctxs;
            std::recursive_mutex mtx;
        public:
            ~multithread_context_manager() {
                for (auto ctx : ctxs)
                    delete ctx.second;
            };
            std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data) {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                auto emplacing_result = ctxs.emplace(std::this_thread::get_id(), new uns::nn::context<basic_number_t, string_t>);
                if (!emplacing_result.second) return "Context addition failed;";

                return Access().Set(str, delimiter, set_dynamic_data);
            };
            context<basic_number_t, string_t>& Access() { return *(ctxs.at(std::this_thread::get_id())); };
            void Drop() {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                delete ctxs.at(std::this_thread::get_id());
                ctxs.erase(std::this_thread::get_id());
            };
        };


        enum FRAME_CASCADE_ACCESS_TYPE {
            Frame_Cascade
        };

        enum NEURO_CASCADE_ACCESS_TYPE {
            Neuro_Cascade
        };


        template<typename basic_number_t, typename string_t>
        class context_access_obj {
        protected:
            context_manager_interface<basic_number_t, string_t>* ctx;
        public:
            context_access_obj() noexcept : ctx(nullptr) {};
            ~context_access_obj() noexcept { if (ctx != nullptr) delete ctx; };
            template<template<typename type_of_basic_numbers,typename type_of_string> class context_manager_t>
            bool Define() noexcept {
                if (ctx != nullptr) return false;
                ctx = new context_manager_t<basic_number_t, string_t>;
                return true;
            };
            bool Valid() const noexcept { return (ctx != nullptr); };
            std::string Settle(const string_t& str, string_t delimiter, bool set_dynamic_data = false) {
                if (ctx == nullptr) return "The context is not defined";
                return ctx->Settle(str, delimiter, set_dynamic_data);
            };
            void ErrorFix(const std::string& error, const std::string& message) {
                if (error == "" && message == "") return;
                ctx->Access().errstream << "ИМЯ ЗАПИСЫВАЮЩЕГО ПОТОКА:" << " gl_time:" << "ГЛОБАЛЬНОЕ ВРЕМЯ" << " lc_time:" << "ЛОКАЛЬНОЕ ВРЕМЯ" << " st_time:" << "ВРЕМЯ ОТ СТАРТА" << ((error != "") ? " err:" : "") << error << ((message != "") ? " msg:" : "") << message << std::endl;
            };
            template<typename enum_input_t>
            decltype(auto) operator()(const enum_input_t& value_enumerator) noexcept {
                return ctx->Access().Get(value_enumerator);
            };
            uns::nn::context<basic_number_t, string_t>::frame_access_obj<basic_number_t, string_t>& operator()(const FRAME_CASCADE_ACCESS_TYPE& value_enumerator) noexcept {
                return ctx->Access().Frame;
            };
            uns::nn::context<basic_number_t, string_t>::neuro_access_obj<basic_number_t, string_t>& operator()(const NEURO_CASCADE_ACCESS_TYPE& value_enumerator) noexcept {
                return ctx->Access().Neuro;
            };

        };


        //context_access_obj<uns::nn::basic_t, uns::nn::string_t> ctx(new multithread_context_manager<uns::nn::basic_t, uns::nn::string_t>);

    };

};