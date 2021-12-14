#pragma once


#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "uniself/algorithms.h"

namespace uns {

    namespace context {

        //интерфейс диспетчера контекста
        template<typename context_t, typename context_constructor_t>
        class manager_interface
        {
        protected:
            virtual context_t& Access() = 0;                                                                                                    //Предоставляет доступ к открытым методам и членам контекста, если контекст инвалиден, поведение неопределено
        public:
            virtual bool Valid() const = 0;                                                                                                     //Проверка валидности контекста
            virtual std::string Settle(const context_constructor_t& context_constructor) = 0;                                                  //Устанавливает валидный контекст на основе конструктора контекста
            virtual context_constructor_t Constructor() = 0;                                                                              //Конвертирует текущий контекст в строку
        };


        //диспетчер однопоточного контекста
        template<typename context_t, typename context_constructor_t>
        class singlethread_manager : public uns::context::manager_interface<context_t, context_constructor_t>
        {
        protected:
            context_t ctx;
        public:
            singlethread_manager() noexcept {};
            singlethread_manager(const singlethread_manager& copying_obj) = delete;
            singlethread_manager& operator=(const singlethread_manager& copying_obj) = delete;
            singlethread_manager(singlethread_manager&& moving_obj) = delete;
            singlethread_manager& operator=(singlethread_manager&& moving_obj) = delete;
            ~singlethread_manager() noexcept {};
        protected:
            context_t& Access() override { return ctx; };
        public:

            template<typename enum_input_t> auto operator[](const enum_input_t& value_enumerator) noexcept {
                return Access().Get(value_enumerator);
            };

            virtual bool Valid() const override { return ctx.Valid(); };

            virtual std::string Settle(const context_constructor_t& context_constructor) {
                return ctx.Set(context_constructor);
            };

            virtual context_constructor_t Constructor() {
                return ctx.Get();
            };
        };


        //диспетчер многопоточных контекстов
        template<typename context_t, typename context_constructor_t>
        class multithread_manager : public uns::context::manager_interface<context_t, context_constructor_t>
        {
        protected:
            std::unordered_map<std::thread::id, context_t*> ctxs;
            std::recursive_mutex mtx;
        public:
            multithread_manager() noexcept {};
            multithread_manager(const multithread_manager& copying_obj) = delete;
            multithread_manager& operator=(const multithread_manager& copying_obj) = delete;
            multithread_manager(multithread_manager&& moving_obj) = delete;
            multithread_manager& operator=(multithread_manager&& moving_obj) = delete;
            ~multithread_manager() {
                for (auto ctx : ctxs)
                    if (ctx.second != nullptr) {
                        delete ctx.second;
                    };
            };
        protected:
            context_t& Access() override { return *(ctxs.at(std::this_thread::get_id())); };
        public:
            bool Valid() const override {
                try {
                    return (ctxs.at(std::this_thread::get_id()) != nullptr && ctxs.at(std::this_thread::get_id())->Valid());
                }
                catch (const std::out_of_range& e) {
                    return false;
                };
            };

            std::string Settle(const context_constructor_t& context_constructor) override {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                context_t* context_ptr = new context_t;

                if (!ctxs.emplace(std::this_thread::get_id(), context_ptr).second) {
                    delete context_ptr;
                    return "Context addition failed;";
                };

                return Access().Set(context_constructor);
            };

            context_constructor_t Constructor() override {
                std::lock_guard<std::recursive_mutex> lock(mtx);

                return Access().Get();
            };

            template<typename enum_input_t> auto operator[](const enum_input_t& value_enumerator) noexcept {
                return Access().Get(value_enumerator);
            };

        };

    };


};