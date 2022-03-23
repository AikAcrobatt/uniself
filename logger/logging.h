#pragma once

#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <mutex>
#include <filesystem>
#include <atomic>
#include <chrono>
#include <limits>

#include "uniself/algorithms.h"
#include "uniself/strings.h"
#include "uniself/exceptions.h"


#define UNS_MODULENAME "logging.h"


template<typename string_t>
string_t TimeToString(const std::chrono::system_clock::time_point& moment);

/*
* It's supposed to be realized as:
* 
template<typename string_t>
string_t TimeToString(const std::chrono::system_clock::time_point& moment) {
    return uns::string_cast<string_t>(moment.time_since_epoch().count());
};
* or smth more complicated
*/



namespace uns {

#pragma warning(disable : 26812)
    UNS_ENUM_DECLARATOR(subsystem, int,
        none = 0,
        logging = 0x1,
        all = std::numeric_limits<int>::max()
    );

    UNS_ENUM_DECLARATOR(urgency, int,
        none = -1,
        low = 0,
        aver = 1,
        very = 2,
        high = 3
    );
#pragma warning(default : 26812)

};

UNS_ENUM_STRING_CAST_DECLARATOR(uns::subsystem);

UNS_ENUM_STRING_CAST_DECLARATOR(uns::urgency);


namespace uns::log {

    template<typename string_type>
    class message {
    public:
        using string_t = string_type;
        using time_t = std::chrono::system_clock::time_point;
    protected:
        time_t moment = time_t();
        std::thread::id thread_id = std::thread::id();
        string_t module_name = uns::string_cast<string_t>("unknown");
        bool module_name_is_empty = false;
        size_t line_number = 0;
        bool line_number_is_empty = false;
        size_t subsystem_flags = 0;
        uns::urgency verbal_type = uns::urgency::very;
        string_t msg = string_t();
        std::hash<std::thread::id> hasher;
    public:
        message() noexcept {};
        template<typename module_name_t>
        message(
            time_t moment_,
            std::thread::id thread_id_,
            module_name_t module_,
            size_t line,
            size_t subsyst,
            uns::urgency urgency,
            string_t message_ = string_t()
        ) noexcept :
            moment(moment_),
            thread_id(thread_id_),
            module_name(uns::string_cast<string_t>(module_)),
            line_number(line),
            subsystem_flags(subsyst),
            verbal_type(urgency),
            msg(message_)
        {
            module_name_is_empty = (module_name.empty());
            line_number_is_empty = (line_number == 0);
        };
        message(const message& obj) {
            moment = obj.moment;
            thread_id = obj.thread_id;
            module_name = obj.module_name;
            module_name_is_empty = obj.module_name_is_empty;
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
                module_name = obj.module_name;
                module_name_is_empty = obj.module_name_is_empty;
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
            module_name = std::move(obj.module_name);
            module_name_is_empty = std::move(obj.module_name_is_empty);
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
                module_name = std::move(obj.module_name);
                module_name_is_empty = std::move(obj.module_name_is_empty);
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

            res += TimeToString<string_t>(moment) + space + colon + colon;
            res += space + uns::string_cast<string_t>(hasher(thread_id)) + space + colon + colon;
            res += space + double_quote + uns::string_cast<string_t>(msg) + double_quote;
            res += space + at + space + uns::string_cast<string_t>(subsystem_flags);
            if (!module_name_is_empty) res += space + in + space + uns::string_cast<string_t>(module_name);
            if (!line_number_is_empty) res += round_bracket_op + uns::string_cast<string_t>(line_number) + round_bracket_cl;
            res += space + dash + space + uns::string_cast<string_t>(verbal_type);

            return res;
        };

        time_t Time() const noexcept { return moment; };

        std::thread::id ThreadId() const noexcept { return thread_id; };

        string_t FunctionName() const noexcept { return module_name; };

        size_t Line() const noexcept { return line_number; };

        size_t Subsystems() const noexcept { return subsystem_flags; };

        uns::urgency Status() const noexcept { return verbal_type; };

        string_t Message() const noexcept { return msg; };
    };


    template<typename string_type>
    class thread_message_queue {
    public:
        using string_t = string_type;
        using message_t = uns::log::message<string_t>;
    protected:

        template<typename message_type>
        class node {
        public:
            using message_t = message_type;

            message_t message;
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

        node<message_t>* begin = nullptr;
        node<message_t>* end = nullptr;     //All the time while the queue is existing, its end should be not nullptr
        size_t size = 0;
    public:
        thread_message_queue() {
            end = new node<message_t>;
            begin = end;
        };
        thread_message_queue(const thread_message_queue& obj) {
            end = new node<message_t>;
            begin = end;

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
            end = new node<message_t>;
            begin = end;

            *this = std::move(obj);
        };
        thread_message_queue& operator=(thread_message_queue&& obj) noexcept {
            if (this == &obj) return *this;

            Clear();

            if (obj.begin == nullptr || obj.end == nullptr) return *this;

            if (end != nullptr)
                delete end;

            end = obj.end;
            begin = obj.begin;
            size = obj.size;

            obj.begin = obj.end = new node<message_t>;
            obj.size = 0;

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

            if (end != nullptr) {
                end->prev = nullptr;
                end->next = nullptr;
            };
            size = 0;
        };

        void Push(const message_t& message) {
            if (begin != nullptr) {
                begin->prev = new node<message_t>;
                size++;
                begin->prev->next = begin;
                begin->prev->message = message;
                begin = begin->prev;
            };
        };

        bool Top(message_t& return_message) const {
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

    template<typename string_type, typename time_period = std::chrono::milliseconds>
    class logger {
    public:
        UNS_EXCEPTION_DECLARE(exception, uns::exceptions::basic);

        using string_t = string_type;
        using fstream_t = std::basic_fstream<typename string_t::value_type>;
        using time_t = std::chrono::steady_clock::time_point;
        using period_t = time_period;
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
        static std::future<void> central_thread;
        static std::atomic<bool> proceeding;
        static std::atomic<bool> to_flush;
        static std::recursive_mutex proc_mtx;
        static bool proceeding_;
        static std::recursive_mutex mutex;
        static fstream_t errfile;
        static size_t errfile_size;
        static std::thread::id central_id;
        static size_t subsystem_flags;
        static uns::urgency verbosity;
        static size_t fileopen_num_of_tryes;

        //переменные центрального потока
        static uns::log::thread_message_queue<string_t> central_buffer;
        static time_period forced_push_timeout;
        static time_t last_push_moment;

        //переменные клиентских потоков
        thread_local static std::unique_ptr<uns::log::thread_message_queue<string_t>> client_buffer;
    protected:
        static string_t CreateErrFileName(const string_t file_extention) {
            return TimeToString<string_t>(std::chrono::system_clock::now())
                + uns::string_cast<string_t>(".") + file_extention;
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
            const uns::urgency verbosity_level = uns::urgency::very,    //уровень болтливости
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
                    throw exception("Fail to create log directory");
            };
        };

        static bool Start() {//false - означает, что возникли ошибки при попытке запуска, true - означает, что ошибок не было, даже если запуск логгера не произведен
            bool central_thread_launched = false;
            {
                std::lock_guard lock(mutex);
                central_thread_launched = (central_id != std::thread::id());
            };

            if(central_thread_launched)
                return true;      //центральный поток можно запустить одновременно только один
            else {
                try {
                    central_thread = std::async(
                        std::launch::async,
                        [] {
                            //это и есть центральный поток
                            {
                                std::lock_guard lock(mutex);
                                if(central_id == std::thread::id()) {
                                    central_id = std::this_thread::get_id();
                                    proceeding = true;
                                    last_push_moment = std::chrono::steady_clock::now();
                                }
                                else {
                                    ToLog(UNS_MODULENAME, __LINE__, uns::subsystem::logging, uns::urgency::high, "");
                                    return;
                                };
                            };

                            ToLog(UNS_MODULENAME, __LINE__, uns::subsystem::logging, uns::urgency::high, "START");

                            Process();
                            do {
                                std::this_thread::sleep_for(central_thread_periodicity);

                                if(!proceeding)
                                    ToLogImmediate(UNS_MODULENAME, __LINE__, uns::subsystem::logging, uns::urgency::high, "FINISH");
                            }
                            while (Process());

                            {
                                std::lock_guard lock(mutex);
                                if (central_id == std::this_thread::get_id()) {
                                    central_id = std::thread::id();
                                };
                            };
                        }
                    );
                }
                catch(std::exception& e) {
                    ReportToEmergencyErrorFile(UNS_MODULENAME, __LINE__, e.what());
                    return false;
                };
            };

            return true;
        };
    protected:
        static void Check() {
            if(client_buffer == nullptr) client_buffer = std::make_unique<uns::log::thread_message_queue<string_t>>();
        };

        static bool Process() {//true - если центральный поток может продолжать выполнение
            const bool proceeding_instruction = proceeding; 
            const bool order_to_flush = to_flush;
            bool result = proceeding_instruction;
            if(std::this_thread::get_id() == central_id) {
                if(
                    (central_buffer.Size() >= central_buffer_optimal_size
                        || std::chrono::steady_clock::now() > last_push_moment + forced_push_timeout
                        || order_to_flush
                        || !proceeding_instruction
                    )
                    && ErrFileIsValid()
                ) {
                    auto central_buffer_copy = uns::log::thread_message_queue<string_t>();
                    size_t central_buffer_previous_size = 0;

                    ToLog(UNS_MODULENAME, __LINE__, uns::subsystem::logging, uns::urgency::low, "PROCESS");

                    {
                        std::lock_guard lock(mutex);
                        central_buffer_previous_size = central_buffer.Size();
                        central_buffer_copy = std::move(central_buffer);
                        central_buffer = uns::log::thread_message_queue<string_t>();
                        last_push_moment = std::chrono::steady_clock::now();

                        if(order_to_flush) to_flush = false;
                    };

                    errfile << central_buffer_copy.Text();
                    errfile.flush();
                    errfile_size += central_buffer_previous_size;
                };

                if((errfile_size >= errfile_optimal_size || !proceeding_instruction) && ErrFileIsValid()) {
                    errfile.close();
                };

                if(proceeding_instruction) {
                    if(!errfile.is_open() || !ErrFileIsValid()) {
                        try {
                            for(size_t num_of_try = 0; num_of_try < fileopen_num_of_tryes; num_of_try++) {
                                errfile = fstream_t((folder / CreateErrFileName(uns::string_cast<string_t>("txt"))).c_str(), std::ios::app | std::ios::binary | std::ios::out);
                                if(ErrFileIsValid())
                                    break;
                            };
                        }
                        catch(std::exception& e) {
                            ReportToEmergencyErrorFile(UNS_MODULENAME, __LINE__, e.what());
                            result = false;
                        };
                        if(!ErrFileIsValid()) {
                            ReportToEmergencyErrorFile(UNS_MODULENAME, __LINE__, "ErrFile is invalid\n");
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
            if (central_thread.valid()) {
                central_thread.get();
            };
        };

        static void Flush() {
            to_flush = true;
        };

        static void ToLogDelayed(const uns::log::message<string_t>& message) {
            Check();
            if((message.Subsystems() & subsystem_flags) != 0 && message.Status() >= verbosity) {
                client_buffer->Push(message);
            };
        };
        template<typename message_t, typename func_t = string_t>
        static void ToLogDelayed(
            const func_t module_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLogDelayed(Message(module_name, line_number, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLogDelayed(
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLogDelayed(Message(std::string(), 0, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLogDelayed(const message_t message) {
            ToLogDelayed(Message(std::string(), 0, uns::subsystem::all, uns::urgency::all, message));
        };


        static void ToLogTry() {
            if(mutex.try_lock()) {
                central_buffer.MergeWith(*client_buffer);
                mutex.unlock();
            };
        };
        static void ToLogTry(const uns::log::message<string_t>& message) {
            ToLogDelayed(message);
            ToLogTry();
        };
        template<typename message_t, typename func_t = string_t>
        static void ToLogTry(
            const func_t module_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLogTry(Message(module_name, line_number, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLogTry(
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLogTry(Message(std::string(), 0, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLogTry(const message_t message) {
            ToLogTry(Message(std::string(), 0, uns::subsystem::all, uns::urgency::all, message));
        };

        static void ToLogImmediate() {
            std::lock_guard lock(mutex);
            central_buffer.MergeWith(*client_buffer);
        };
        static void ToLogImmediate(const uns::log::message<string_t>& message) {
            ToLogDelayed(message);
            ToLogImmediate();
        };
        template<typename message_t, typename func_t = string_t>
        static void ToLogImmediate(
            const func_t module_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLogImmediate(Message(module_name, line_number, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLogImmediate(
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLogImmediate(Message(std::string(), 0, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLogImmediate(const message_t message) {
            ToLogImmediate(Message(std::string(), 0, uns::subsystem::all, uns::urgency::all, message));
        };

        static void ToLog() {
            if(client_buffer->Size() >= client_buffer_maximal_size || !proceeding)
                ToLogImmediate();
            else if(client_buffer->Size() >= client_buffer_optimal_size)
                ToLogTry();
        };
        static void ToLog(const uns::log::message<string_t>& message) {
            ToLogDelayed(message);
            ToLog();
        };
        template<typename message_t, typename func_t = string_t>
        static void ToLog(
            const func_t module_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLog(Message(module_name, line_number, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLog(
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = message_t()
        ) {
            ToLog(Message(std::string(), 0, subsystems, urgency, message));
        };
        template<typename message_t>
        static void ToLog(const message_t message) {
            ToLog(Message(std::string(), 0, uns::subsystem::all, uns::urgency::all, message));
        };

        template<typename message_t, typename func_t = string_t>
        static uns::log::message<string_t> Message(
            const func_t module_name,
            const size_t line_number,
            const size_t subsystems,
            const uns::urgency urgency = uns::urgency::very,
            const message_t message = ""
        ) {
            return uns::log::message<string_t>(
                std::chrono::system_clock::now(),
                std::this_thread::get_id(),
                uns::string_cast<string_t>(module_name),
                line_number,
                subsystems,
                urgency,
                uns::string_cast<string_t>(message)
            );
        };

        template<typename message_t, typename func_t = string_t>
        static void ReportToEmergencyErrorFile(const func_t module_name, const size_t line_number, const message_t& str) {
            static const auto newline = uns::string_cast<string_t>("\n");

            for(size_t num_of_try = 0; num_of_try < fileopen_num_of_tryes; num_of_try++) {
                fstream_t emergency_errfile((folder / emergency_errfile_name).c_str());
                if(!emergency_errfile.bad() && !emergency_errfile.fail() && emergency_errfile.is_open()) {
                    uns::log::message message = Message(
                        module_name,
                        line_number,
                        uns::subsystem::logging,
                        uns::urgency::high,
                        uns::string_cast<string_t>(str)
                    );
                    emergency_errfile << message.String() + newline;
                    break;
                };
            };
        };

    };

    template<typename string_type, typename time_period> typename std::filesystem::path uns::logger<string_type, time_period>::folder;
    template<typename string_type, typename time_period> std::string uns::logger<string_type, time_period>::emergency_errfile_name;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::central_buffer_optimal_size = 100;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::errfile_optimal_size = 50;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::client_buffer_optimal_size = 10;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::client_buffer_maximal_size = 20;
    template<typename string_type, typename time_period> typename uns::logger<string_type, time_period>::period_t uns::logger<string_type, time_period>::central_thread_periodicity = typename uns::logger<string_type, time_period>::period_t(10);
    template<typename string_type, typename time_period> std::future<void> uns::logger<string_type, time_period>::central_thread;
    template<typename string_type, typename time_period> std::atomic<bool> uns::logger<string_type, time_period>::proceeding = false;
    template<typename string_type, typename time_period> std::atomic<bool> uns::logger<string_type, time_period>::to_flush = false;
    template<typename string_type, typename time_period> std::recursive_mutex uns::logger<string_type, time_period>::mutex;
    template<typename string_type, typename time_period> typename uns::logger<string_type, time_period>::fstream_t uns::logger<string_type, time_period>::errfile;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::errfile_size = 0;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::fileopen_num_of_tryes = 3;
    template<typename string_type, typename time_period> typename std::thread::id uns::logger<string_type, time_period>::central_id;
    template<typename string_type, typename time_period> size_t uns::logger<string_type, time_period>::subsystem_flags = uns::subsystem::none;
    template<typename string_type, typename time_period> uns::urgency uns::logger<string_type, time_period>::verbosity = uns::urgency::very;
    template<typename string_type, typename time_period> uns::log::thread_message_queue<string_type> uns::logger<string_type, time_period>::central_buffer;
    template<typename string_type, typename time_period> typename uns::logger<string_type, time_period>::period_t uns::logger<string_type, time_period>::forced_push_timeout = typename uns::logger<string_type, time_period>::period_t(10000);
    template<typename string_type, typename time_period> typename uns::logger<string_type, time_period>::time_t uns::logger<string_type, time_period>::last_push_moment = uns::logger<string_type, time_period>::time_t();
    template<typename string_type, typename time_period> thread_local std::unique_ptr<uns::log::thread_message_queue<string_type>> uns::logger<string_type, time_period>::client_buffer = nullptr;
    template<typename string_type, typename time_period> std::recursive_mutex uns::logger<string_type, time_period>::proc_mtx;
    template<typename string_type, typename time_period> bool uns::logger<string_type, time_period>::proceeding_ = true;

};


#undef UNS_MODULENAME





