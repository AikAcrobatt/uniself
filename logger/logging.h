#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>
#include <exception>
#include <mutex>
#include <atomic>
#include <chrono>
#include <limits>
//#include <list>
//#include <future>

#include "uniself/algorithms.h"
#include "uniself/strings.h"
#include "uniself/time.h"

namespace uns {

#pragma warning(disable : 26812)
    UNS_ENUM_DECLARATOR(subsystem, int,
        none = 0,
        logging = 0x1,
        all = std::numeric_limits<int>::max()
    );

    UNS_ENUM_DECLARATOR(message_status, int,
        all = -1,
        info = 0,
        warning = 1,
        issue = 2,
        alert = 3
    );
#pragma warning(default : 26812)

};

UNS_ENUM_STRING_CAST_DECLARATOR(uns::subsystem);

UNS_ENUM_STRING_CAST_DECLARATOR(uns::message_status);


namespace uns::log {

    class message {
    public:
        using string_t = std::wstring;
        using time_t = std::chrono::system_clock::time_point;
    protected:
        time_t moment = time_t();
        std::thread::id thread_id = std::thread::id();
        string_t function_name = uns::string_cast<string_t>("unknown_function");
        bool function_name_is_empty = false;
        size_t line_number = 0;
        bool line_number_is_empty = false;
        size_t subsystem_flags = 0;
        uns::message_status verbal_type = uns::message_status::issue;
        string_t msg = string_t();
        std::hash<std::thread::id> hasher;
    public:
        message() noexcept {};
        message(
            time_t moment_,
            std::thread::id thread_id_,
            string_t function,
            size_t line,
            size_t subsyst,
            uns::message_status message_status,
            string_t message_ = string_t()
        ) noexcept :
            moment(moment_),
            thread_id(thread_id_),
            function_name(function),
            line_number(line),
            subsystem_flags(subsyst),
            verbal_type(message_status),
            msg(message_)
        {
            function_name_is_empty = (function_name.empty());
            line_number_is_empty = (line_number == 0);
        };
        message(const message& obj) {
            moment = obj.moment;
            thread_id = obj.thread_id;
            function_name = obj.function_name;
            function_name_is_empty = obj.function_name_is_empty;
            line_number = obj.line_number;
            line_number_is_empty = obj.line_number_is_empty;
            subsystem_flags = obj.subsystem_flags;
            verbal_type = obj.verbal_type;
            msg = obj.msg;
        };
        message& operator=(const message& obj) {
            if (this != &obj) {
                moment = obj.moment;
                thread_id = obj.thread_id;
                function_name = obj.function_name;
                function_name_is_empty = obj.function_name_is_empty;
                line_number = obj.line_number;
                line_number_is_empty = obj.line_number_is_empty;
                subsystem_flags = obj.subsystem_flags;
                verbal_type = obj.verbal_type;
                msg = obj.msg;
            };

            return *this;
        };
        message(message&& obj) noexcept {
            moment = std::move(obj.moment);
            thread_id = std::move(obj.thread_id);
            function_name = std::move(obj.function_name);
            function_name_is_empty = std::move(obj.function_name_is_empty);
            line_number = std::move(obj.line_number);
            line_number_is_empty = std::move(obj.line_number_is_empty);
            subsystem_flags = std::move(obj.subsystem_flags);
            verbal_type = std::move(obj.verbal_type);
            msg = std::move(obj.msg);
        };
        message& operator=(message&& obj) noexcept {
            if (this != &obj) {
                moment = std::move(obj.moment);
                thread_id = std::move(obj.thread_id);
                function_name = std::move(obj.function_name);
                function_name_is_empty = std::move(obj.function_name_is_empty);
                line_number = std::move(obj.line_number);
                line_number_is_empty = std::move(obj.line_number_is_empty);
                subsystem_flags = std::move(obj.subsystem_flags);
                verbal_type = std::move(obj.verbal_type);
                msg = std::move(obj.msg);
            };

            return *this;
        };
        ~message() noexcept {};

        string_t String() const noexcept {
            string_t res = string_t();

            static const string_t empty = uns::string_cast<string_t>("");
            static const string_t colon = uns::string_cast<string_t>(":");
            static const string_t space = uns::string_cast<string_t>(" ");
            static const string_t double_quote = uns::string_cast<string_t>("\"");
            static const string_t at = uns::string_cast<string_t>("at");
            static const string_t in = uns::string_cast<string_t>("in");
            static const string_t round_bracket_op = uns::string_cast<string_t>("(");
            static const string_t round_bracket_cl = uns::string_cast<string_t>(")");
            static const string_t dash = uns::string_cast<string_t>("-");

            res += uns::string_cast<string_t>(moment.time_since_epoch().count()) + space + colon + colon;   //TODO сделать, чтобы было нормальное время
            res += space + uns::string_cast<string_t>(hasher(thread_id)) + space + colon + colon;
            res += space + double_quote + uns::string_cast<string_t>(msg) + double_quote;
            res += space + at + space + uns::string_cast<string_t>(subsystem_flags);
            if (!function_name_is_empty) res += space + in + space + uns::string_cast<string_t>(function_name);
            if (!line_number_is_empty) res += round_bracket_op + uns::string_cast<string_t>(line_number) + round_bracket_cl;
            res += space + dash + space + uns::string_cast<string_t>(verbal_type);

            return res;
        };

        time_t Time() const noexcept { return moment; };

        std::thread::id ThreadId() const noexcept { return thread_id; };

        string_t FunctionName() const noexcept { return function_name; };

        size_t Line() const noexcept { return line_number; };

        size_t Subsystems() const noexcept { return subsystem_flags; };

        uns::message_status Status() const noexcept { return verbal_type; };

        string_t Message() const noexcept { return msg; };
    };


    class thread_message_queue {
    public:
        using string_t = std::wstring;
    protected:

        class node {
        public:
            uns::log::message message;
            node* prev = nullptr;
            node* next = nullptr;

            node() :
                message(),
                prev(nullptr),
                next(nullptr)
            {};
            node(const node&) = delete;
            node& operator=(const node&) = delete;
            node(node&&) = delete;
            node& operator=(node&&) = delete;
            ~node() {};
        };

        node* begin = nullptr;
        node* end = nullptr;
        size_t size = 0;
    public:
        thread_message_queue() : end(new node) {
            begin = end;
        };
        thread_message_queue(const thread_message_queue& obj) {
            *this = obj;
        };
        thread_message_queue& operator=(const thread_message_queue& obj) {
            if (this == &obj) return *this;

            Clear();

            if (obj.begin == nullptr || obj.end == nullptr) return *this;

            for (auto iter = obj.end->prev; iter != obj.begin->prev && iter != nullptr; iter = iter->prev) {
                Push(iter->message);
            };

            return *this;
        };
        thread_message_queue(thread_message_queue&& obj) noexcept {
            *this = std::move(obj);
        };
        thread_message_queue& operator=(thread_message_queue&& obj) noexcept {
            if (this == &obj) return *this;

            Clear();

            if (obj.begin == nullptr || obj.end == nullptr) return *this;

            end = obj.end;
            begin = obj.begin;
            size = obj.size;

            return *this;
        };
        ~thread_message_queue() {
            Clear();
            if (end != nullptr) delete end;
        };

        string_t Text() const {
            static const string_t new_line = uns::string_cast<string_t>("\n");
            auto res = string_t();

            for (auto iter = end; iter != begin && iter != nullptr && iter->prev != nullptr; iter = iter->prev)
                res += iter->prev->message.String() + new_line;

            return res;
        };

        size_t Size() const { return size; };

        void Clear() {
            auto iter = begin;
            while (begin != end && begin != nullptr && begin->next != nullptr) {
                begin = begin->next;
                delete iter;
                size--;
                iter = begin;
            };

            end->prev = nullptr;
            end->next = nullptr;
            size = 0;
        };

        void Push(const uns::log::message& message) {
            if (begin != nullptr) {
                begin->prev = new node;
                size++;
                begin->prev->next = begin;
                begin->prev->message = message;
                begin = begin->prev;
            };
        };

        bool Top(uns::log::message& return_message) const {
            if (begin != end && end != nullptr && end->prev != nullptr) {
                return_message = end->prev->message;
                return true;
            };
            return false;
        };

        void Pop() {
            if (begin != end && end != nullptr && end->prev != nullptr) {
                end->prev = end->prev->prev;
                size = (size > 1 ? --size : 0);
                if (end->prev != nullptr) {
                    delete end->prev->next;
                    end->prev->next = end;
                }
                else if (begin != nullptr) {
                    delete begin;
                    begin = end;
                }
                else {
                    begin = end;
                };
            };
        };

        void MergeWith(thread_message_queue& queue) {
            if (
                queue.begin != queue.end
                && queue.begin != nullptr
                && queue.end != nullptr
                && queue.end->prev != nullptr
                && begin != nullptr
                ) {
                begin->prev = queue.end->prev;
                begin->prev->next = begin;
                begin = queue.begin;
                size += queue.size;
            };

            if (queue.end != nullptr) {
                queue.end->prev = nullptr;
                queue.end->next = nullptr;
            };

            queue.size = 0;
            queue.begin = queue.end;
        };
    };
};

namespace uns {

    class logger {
    public:
        using time_period = std::chrono::milliseconds;
        using time_t = std::chrono::system_clock::time_point;//TODO возможно имеет смысл в самом логгере завязаться на steady_clock
    protected:

        //настройки механизма логирования, относящиеся к файловой системе
        static std::filesystem::path folder;
        static std::string emergency_errfile_name;

        //настройки механизма логирования, востребованные в единственном потоке (и потому не требующие параллельности)
        static size_t central_buffer_optimal_size;   //не меняется во время работы системы
        static size_t errfile_optimal_size;   //не меняется во время работы системы
        static size_t client_buffer_optimal_size;   //не меняется во время работы системы
        static size_t client_buffer_maximal_size;   //не меняется во время работы системы
        static time_period central_thread_periodicity;   //не меняется во время работы системы

        //переменные механизма логирования, относящиеся к центральному потоку или общие
        static std::thread central_thread;
        static std::atomic<bool> proceeding;
        static std::recursive_mutex mutex;
        static std::wfstream errfile;
        static size_t errfile_size;
        static std::thread::id central_id;
        static size_t subsystem_flags;
        static uns::message_status verbosity;
        static size_t fileopen_num_of_tryes;

        //переменные центрального потока
        static uns::log::thread_message_queue central_buffer;
        static time_period forced_push_timeout;
        static time_t last_push_moment;

        //переменные клиентских потоков
        thread_local static std::unique_ptr<uns::log::thread_message_queue> client_buffer;
    protected:
        static std::string CreateErrFileName(const std::string file_extention) {
            return uns::string_cast<std::string>(std::chrono::system_clock::now().time_since_epoch().count()) //TODO сделать, чтобы было нормальное время
                + "." + file_extention;
        };

        static bool ErrFileIsValid() {
            return !errfile.bad() && errfile.is_open();
        };
    public:
        static void Setup(
            const std::filesystem::path& log_folder,                                   //полный путь к папке, в которую будут помещаться файлы логирования
            const std::string& emergency_log_errfile_name,                             //имя файла, который будет использоваться для записи сообщений о критических ошибках самого механизма
            const size_t central_buffer_optimal_size_,                                 //размер «центрального» буфера (количество сообщений), при достижении которого «центральный поток» отправляет эти данные в лог
            const size_t errfile_optimal_size_,                                        //размер файла логов (количество сообщений), при превышении которого должен быть создан следующий файл
            const size_t client_buffer_optimal_size_,                                  //размер (количество сообщений) клиентского буфера сообщений, при котором, делается попытка выгрузки в «центральный» буфер
            const size_t client_buffer_maximal_size_,                                  //размер (количество сообщений) клиентского буфера, при достижении которого клиентский поток встает в очередь на запись в «центрлаьный» буфер
            const time_period central_thread_periodicity_,                             //периодичность запуска «центрального» потока (в миллисекундах)
            const size_t subsystem_flags_ = uns::subsystem::none,                      //флаги подсистем
            const uns::message_status verbosity_level = uns::message_status::issue,    //уровень болтливости
            const time_period forced_push_timeout_ = std::chrono::milliseconds(10000), //интервал времени, раз в который центральный буффер будет выгружаться в лог-файл
            const size_t fileopen_num_of_tryes_ = 3                                    //количество попыток отведенное на открытие лог-файла, после его истечения логгер остановится
        ) {
            folder = log_folder;
            emergency_errfile_name = emergency_log_errfile_name;
            central_buffer_optimal_size = central_buffer_optimal_size_;
            errfile_optimal_size = errfile_optimal_size_;
            client_buffer_optimal_size = client_buffer_optimal_size_;
            client_buffer_maximal_size = client_buffer_maximal_size_;
            central_thread_periodicity = central_thread_periodicity_;
            subsystem_flags = subsystem_flags_;
            verbosity = verbosity_level;
            forced_push_timeout = forced_push_timeout_;
            fileopen_num_of_tryes = fileopen_num_of_tryes_;

            if(!std::filesystem::exists(folder)) {
                bool directory_created = false;

                for(size_t num_of_try = 0; num_of_try < fileopen_num_of_tryes; num_of_try++) {
                    if(std::filesystem::create_directory(folder)) {
                        directory_created = true;
                        break;
                    };
                };

                if(!directory_created)
                    throw std::exception("Fail to create log directory");      //TODO написать собственную библиотеку исключений
            };
        };

        static bool Start() {//false - означает, что возникли ошибки при попытке запуска, true - означает, что ошибок не было, даже если запуск логгера не произведен
            bool central_thread_launched = false;
            {
                std::lock_guard lock(uns::logger::mutex);
                central_thread_launched = (central_id != std::thread::id());
            };

            if(central_thread_launched)
                return true;      //центральный поток можно запустить одновременно только один
            else {
                try {
                    central_thread = std::thread(//TODO переделать на std::future
                        [] {
                            //это и есть центральный поток
                            {
                                std::lock_guard lock(uns::logger::mutex);
                                if(uns::logger::central_id == std::thread::id()) {
                                    uns::logger::central_id = std::this_thread::get_id();
                                    uns::logger::proceeding = true;
                                    uns::logger::last_push_moment = std::chrono::system_clock::now();
                                }
                                else {
                                    uns::logger::ToLog(__FUNCTION__, __LINE__, uns::subsystem::logging, uns::message_status::warning);
                                    return;
                                };
                            };

                            uns::logger::ToLog(__FUNCTION__, __LINE__, uns::subsystem::logging, uns::message_status::info);

                            while(Proceed()) {
                                std::this_thread::sleep_for(uns::logger::central_thread_periodicity);
                            };

                            {
                                std::lock_guard lock(uns::logger::mutex);
                                if(uns::logger::central_id == std::this_thread::get_id()) {
                                    uns::logger::central_id = std::thread::id();
                                };
                            };
                        }
                    );
                }
                catch(std::exception& e) {
                    ReportToEmergencyErrorFile(__FUNCTION__, __LINE__, e.what());
                    return false;
                };
            };

            return true;
        };
    protected:
        static void Check() {
            if(client_buffer == nullptr) client_buffer = std::make_unique<uns::log::thread_message_queue>();
        };

        static bool Proceed() {//true - если центральный поток может продолжать выполнение
            const bool proceeding_instruction = proceeding;
            bool result = proceeding_instruction;
            if(std::this_thread::get_id() == central_id) {
                if(
                    (central_buffer.Size() >= central_buffer_optimal_size
                        || std::chrono::system_clock::now() > last_push_moment + forced_push_timeout
                        || !proceeding_instruction)
                    && ErrFileIsValid()
                ) {
                    std::wstring central_buffer_text = L""; //TODO завязаться на тип чара из файлового потока
                    size_t central_buffer_previous_size = 0;

                    ToLog(__FUNCTION__, __LINE__, uns::subsystem::logging, uns::message_status::info);

                    {
                        std::lock_guard lock(mutex);
                        central_buffer_text = central_buffer.Text();
                        central_buffer_previous_size = central_buffer.Size();
                        central_buffer.Clear();
                        last_push_moment = std::chrono::system_clock::now();
                    };

                    errfile << central_buffer_text;
                    errfile_size += central_buffer_previous_size;
                };

                if((errfile_size >= errfile_optimal_size || !proceeding_instruction) && ErrFileIsValid()) {
                    errfile.close();
                };

                if(proceeding_instruction) {
                    if(!errfile.is_open() || !ErrFileIsValid()) {
                        try {
                            for(size_t num_of_try = 0; num_of_try < fileopen_num_of_tryes; num_of_try++) {
                                errfile.open((folder / CreateErrFileName("txt")).c_str(), std::ios::app | std::ios::binary | std::ios::out);
                                if(ErrFileIsValid())
                                    break;
                            };
                        }
                        catch(std::exception& e) {
                            ReportToEmergencyErrorFile(__FUNCTION__, __LINE__, e.what());
                            result = false;
                        };
                        if(!ErrFileIsValid()) {
                            ReportToEmergencyErrorFile(__FUNCTION__, __LINE__, "ErrFile is invalid\n");
                            result = false;
                        };

                        errfile_size = 0;
                    };
                    result = result && true;
                };
            };
            if(!result) Stop();
            return result;
        };
    public:
        static void Stop() {  //команда на остановку логирования
            proceeding = false;
        };

        static void Finish() {  // процедура завершения логирования
            Stop();
            if(central_thread.joinable()) {
                central_thread.join();
                
                std::lock_guard lock(mutex);
                central_id = std::thread::id();
            };
        };

        static void ToLogDelayed(const uns::log::message& message) {
            Check();
            if((message.Subsystems() & subsystem_flags) != 0 && message.Status() >= verbosity) {
                client_buffer->Push(message);
            };
        };
        static void ToLogDelayed(
            const std::string function_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = std::string()
        ) {
            ToLogDelayed(Message(function_name, line_number, subsystems, message_status, message));
        };
        static void ToLogDelayed(
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = std::string()
        ) {
            ToLogDelayed(Message(std::string(), 0, subsystems, message_status, message));
        };
        static void ToLogDelayed(const std::string message = std::string()) {
            ToLogDelayed(Message(std::string(), 0, uns::subsystem::all, uns::message_status::all, message));
        };


        static void ToLogTry() {
            if(mutex.try_lock()) {
                central_buffer.MergeWith(*client_buffer);
                mutex.unlock();
            };
        };
        static void ToLogTry(const uns::log::message& message) {
            ToLogDelayed(message);
            ToLogTry();
        };
        static void ToLogTry(
            const std::string function_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = ""
        ) {
            ToLogTry(Message(function_name, line_number, subsystems, message_status, message));
        };
        static void ToLogTry(
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = std::string()
        ) {
            ToLogTry(Message(std::string(), 0, subsystems, message_status, message));
        };
        static void ToLogTry(const std::string message) {
            ToLogTry(Message(std::string(), 0, uns::subsystem::all, uns::message_status::all, message));
        };

        static void ToLogImmediate() {
            std::lock_guard lock(mutex);
            central_buffer.MergeWith(*client_buffer);
        };
        static void ToLogImmediate(const uns::log::message& message) {
            ToLogDelayed(message);
            ToLogImmediate();
        };
        static void ToLogImmediate(
            const std::string function_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = ""
        ) {
            ToLogImmediate(Message(function_name, line_number, subsystems, message_status, message));
        };
        static void ToLogImmediate(
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = std::string()
        ) {
            ToLogImmediate(Message(std::string(), 0, subsystems, message_status, message));
        };
        static void ToLogImmediate(const std::string message) {
            ToLogImmediate(Message(std::string(), 0, uns::subsystem::all, uns::message_status::all, message));
        };

        static void ToLog() {
            if(client_buffer->Size() >= client_buffer_maximal_size || !proceeding)
                ToLogImmediate();
            else if(client_buffer->Size() >= client_buffer_optimal_size)
                ToLogTry();
        };
        static void ToLog(const uns::log::message& message) {
            ToLogDelayed(message);
            ToLog();
        };
        static void ToLog(
            const std::string function_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = ""
        ) {
            ToLog(Message(function_name, line_number, subsystems, message_status, message));
        };
        static void ToLog(
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = std::string()
        ) {
            ToLog(Message(std::string(), 0, subsystems, message_status, message));
        };
        static void ToLog(const std::string message) {
            ToLog(Message(std::string(), 0, uns::subsystem::all, uns::message_status::all, message));
        };

        static uns::log::message Message(
            const std::string function_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::message_status message_status = uns::message_status::issue,
            const std::string message = ""
        ) {
            return uns::log::message(
                std::chrono::system_clock::now(),
                std::this_thread::get_id(),
                uns::string_cast<std::wstring>(function_name),
                line_number,
                subsystems,
                message_status,
                uns::string_cast<std::wstring>(message)
            );
        };

        template<typename string_t>
        static void ReportToEmergencyErrorFile(const std::string function_name, const size_t line_number, const string_t& str) {
            for(size_t num_of_try = 0; num_of_try < fileopen_num_of_tryes; num_of_try++) {
                std::wfstream emergency_errfile((folder / emergency_errfile_name).c_str());
                if(!emergency_errfile.bad() && !emergency_errfile.fail() && emergency_errfile.is_open()) {
                    uns::log::message message = Message(
                        function_name,
                        line_number,
                        uns::subsystem::logging,
                        uns::message_status::alert,
                        uns::string_cast<std::wstring>(str)
                    );
                    emergency_errfile << message.String() + L"\n";
                    break;
                };
            };
        };

    };

    std::filesystem::path uns::logger::folder;
    std::string uns::logger::emergency_errfile_name;
    size_t uns::logger::central_buffer_optimal_size = 100;
    size_t uns::logger::errfile_optimal_size = 50;
    size_t uns::logger::client_buffer_optimal_size = 10;
    size_t uns::logger::client_buffer_maximal_size = 20;
    uns::logger::time_period uns::logger::central_thread_periodicity = std::chrono::milliseconds(10);
    std::thread uns::logger::central_thread;
    std::atomic<bool> uns::logger::proceeding = false;
    std::recursive_mutex uns::logger::mutex;
    std::wfstream uns::logger::errfile;
    size_t uns::logger::errfile_size = 0;
    size_t uns::logger::fileopen_num_of_tryes = 3;
    std::thread::id uns::logger::central_id;
    size_t uns::logger::subsystem_flags = uns::subsystem::none;
    uns::message_status uns::logger::verbosity = uns::message_status::issue;
    uns::log::thread_message_queue uns::logger::central_buffer;
    uns::logger::time_period uns::logger::forced_push_timeout = std::chrono::milliseconds(10000);
    uns::logger::time_t uns::logger::last_push_moment = uns::logger::time_t();
    thread_local std::unique_ptr<uns::log::thread_message_queue> uns::logger::client_buffer = nullptr;

};








