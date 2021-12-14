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

       
    //интерфейс диспетчера контекста
    template<typename context_t>
    class context_manager_interface
    {
    public:
        virtual std::wstring Settle(const std::wstring& str, const std::wstring& delimiter, bool set_dynamic_data) = 0; //Устанавливает валидный контекст по строке, если нет валидного контекста, иначе, возвращает сообщение об этом
        virtual std::wstring String(const std::wstring& delimiter, bool return_dynamic_data) = 0;                       //Конвертирует текущий контекст в строку
        virtual context_t& Access() = 0;                                                                                  //Предоставляет доступ к открытым методам и членам контекста, если контекст инвалиден, поведение неопределено
        virtual std::ofstream& OErr() = 0;                                                                              //Предоставляет доступ к потоку ошибок текущего контекста, если контекст инвалиден, поведение неопределено
        virtual void Drop() = 0;                                                                                        //Делает контекст инвалидным
    };


    //диспетчер однопоточного контекста
    template<typename context_t>
    class singlethread_context_manager : public context_manager_interface<context_t>
    {
    protected:
        bool context_is_valid = false;
        context_t ctx;
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

            errstream.open(Access().Get(CTX_CSTRING::ErrorFileName), std::ios::binary | std::ios::app);//подумать про составное имя файла ошибок (директория + сам файл)
            if (errstream.bad()) res += L"Opening ErrFile failed;";

            return res;
        };

        std::wstring String(const std::wstring& delimiter, bool return_dynamic_data) override { return ctx.String(delimiter, return_dynamic_data); };

        context_t& Access() override { return ctx; };

        std::ofstream& OErr() override { return errstream; };
        void Drop() override {
            context_is_valid = false;
            if (!OErr().bad()) OErr().close();
        };
    };


    //диспетчер многопоточных контекстов
    template<typename context_t>
    class multithread_context_manager : public context_manager_interface<context_t>
    {
    protected:
        std::unordered_map<std::thread::id, context_t*> ctxs;
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
                if (ctx.second != nullptr)
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
                if (ctx.second->Get(CTX_CSTRING::ErrorFileName) == Access().Get(CTX_CSTRING::ErrorFileName) && ctx.first != std::this_thread::get_id()) {//подумать про составное имя файла ошибок (директория + сам файл)
                    thread_id_with_same_errfile = ctx.first;
                    match_flag = true;
                    break;
                };

            if (!match_flag) {
                auto emplacing_result2 = errs.emplace(std::this_thread::get_id(), new std::ofstream(Access().Get(CTX_CSTRING::ErrorFileName), std::ios::binary | std::ios::app));//подумать про составное имя файла ошибок (директория + сам файл)
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

        context_t& Access() override { return *(ctxs.at(std::this_thread::get_id())); };

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
            auto res = ctx->Settle(uns::string_cast<std::wstring>(str), uns::string_cast<std::wstring>(delimiter), set_dynamic_data);

            return res;
        };

        template<typename string_t> string_t String(const string_t& delimiter, bool return_dynamic_data) {
            return uns::string_cast<std::wstring>(ctx->Access().String(uns::string_cast<std::wstring>(delimiter), return_dynamic_data));
        };

        void ErrorFix(const std::string& error, const std::string& message) {
            if (ctx->OErr().bad()) return;
            if (error == "" && message == "") return;
            ctx->OErr() << uns::string_cast<std::string>(ctx->Access().Get(CTX_CSTRING::ProgramName)) << ":: time_from_start:" << uns::string_cast<std::string>(uns::from_start::Count<uns::flow_time_period>()) << " st_time:" << uns::string_cast<std::string>(ctx->Access().Get(CTX_DOUBLE::StartTime)) << ((error != "") ? " err:" : "") << error << ((message != "") ? " msg:" : "") << message << std::endl;
        };

        template<typename enum_input_t> decltype(auto) operator()(const enum_input_t& value_enumerator) noexcept {
            return ctx->Access().Get(value_enumerator);
        };

    };



};