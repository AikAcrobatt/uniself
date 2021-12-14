
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <functional>

#include "uniself/strings.h"

#include "context.h"

namespace tns {     //testing namespace

    //константные
    UNS_ENUM_DECLARATOR(CTX_CBOOL, size_t,
        bool_c_0 = 0
    );

    UNS_ENUM_DECLARATOR(CTX_CSTRING, size_t,
        string_c_0 = 0,
        string_c_1 = 1
    );

    //неконстантные
    UNS_ENUM_DECLARATOR(CTX_BOOL, size_t,
        bool_0 = 0,
        bool_1 = 1
    );

    UNS_ENUM_DECLARATOR(CTX_INT, size_t,
        int_0 = 0,
        int_1 = 1,
        int_2 = 2
    );
};

UNS_ENUM_STRING_CAST_DECLARATOR(tns::CTX_CBOOL)
UNS_ENUM_STRING_CAST_DECLARATOR(tns::CTX_CSTRING)
UNS_ENUM_STRING_CAST_DECLARATOR(tns::CTX_BOOL)
UNS_ENUM_STRING_CAST_DECLARATOR(tns::CTX_INT)

namespace tns {


    //контейнер контекста
    class context {
    private:
        //хранилища постоянных параметров
        std::array<bool, CTX_CBOOL::_size()>                        c_bool_param;
        std::array<std::wstring, CTX_CSTRING::_size()>              c_str_param;
        //хранилища динамических параметров
        std::array<bool, CTX_BOOL::_size()>                         bool_param;
        std::array<int, CTX_INT::_size()>                           int_param;
        //вспомогательные поля
        bool is_valid = false;
    public:
        context() noexcept {};
        context(const context& copying_obj) = delete;
        context& operator=(const context& copying_obj) = delete;
        context(context&& moving_obj) = delete;
        context& operator=(context&& moving_obj) = delete;
        ~context() {};

        const bool& Get(CTX_CBOOL param) const { return c_bool_param[param]; };
        const std::wstring& Get(CTX_CSTRING param) const { return c_str_param[param]; };
        bool& Get(CTX_BOOL param) { return bool_param[param]; };
        int& Get(CTX_INT param) { return int_param[param]; };

        std::string Set(const std::wstring& str, const std::wstring& delimiter);
        std::string Set(const std::wstring& constructor_str) {
            std::wstring delimiter = std::wstring();

            if (constructor_str.size() > 0)
                delimiter = constructor_str[0];

            return Set(constructor_str.substr(1), delimiter);
        };

        std::wstring Get() const {
            return L"\n" + String(L"\n");
        };

        std::wstring String(const std::wstring& delimiter) const;

        bool Valid() const { return is_valid; };
    };

    std::string context::Set(const std::wstring& str, const std::wstring& delimiter) {
        std::string mistakes_report = "";

        using string_t = std::wstring;

        string_t value = string_t();
        size_t pos = 0;

        for (auto I : CTX_CBOOL::_values()) {
            if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                c_bool_param[I] = uns::string_cast<bool>(value);
            else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
        };

        for (auto I : CTX_CSTRING::_values()) {
            if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                c_str_param[I] = value;
            else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
        };

        for (auto I : CTX_BOOL::_values()) {
            if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                bool_param[I] = uns::string_cast<bool>(value);
            else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
        };

        for (auto I : CTX_INT::_values()) {
            if (uns::string::ReadBySample<string_t>(str, uns::string_cast<string_t>(I) + uns::string_cast<string_t>("="), false, -1, value, delimiter))
                int_param[I] = uns::string_cast<int>(value);
            else mistakes_report += uns::string_cast<std::string>(I) + " not found;";
        };

        is_valid = (mistakes_report == "");

        return mistakes_report;
    };

    std::wstring context::String(const std::wstring& delimiter) const {
        using string_t = std::wstring;
        string_t res = string_t();
        int count = 0;

        for (auto I : CTX_CBOOL::_values()) {
            res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_bool_param[I]) + delimiter;
        };

        for (auto I : CTX_CSTRING::_values()) {
            res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(c_str_param[I]) + delimiter;
        };

        for (auto I : CTX_BOOL::_values()) {
            res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(bool_param[I]) + delimiter;
        };

        for (auto I : CTX_INT::_values()) {
            res = res + uns::string_cast<string_t>(I) + uns::string_cast<string_t>("=") + uns::string_cast<string_t>(int_param[I]) + delimiter;
        };

        return res;
    };
};


uns::context::multithread_manager<tns::context, std::wstring> ctx;

int main()
{
    std::cout << "START" << std::endl;

    std::string str0 = "\n";

    str0 += "bool_c_0=true\n";
    str0 += "string_c_0=VERIFYED sting1\n";
    str0 += "string_c_1=vErIfYeD STRRRIIING2\n";
    str0 += "bool_0=false\n";
    str0 += "bool_1=true\n";
    str0 += "int_0=23456\n";
    str0 += "int_1=10102034\n";
    str0 += "int_2=20102034\n";

    std::cout << "Is valid: " << std::boolalpha << ctx.Valid() << std::endl;
    auto res = ctx.Settle(uns::string_cast<std::wstring>(str0));
    std::cout << "result:" << res << std::endl;
    std::cout << "Is valid: " << std::boolalpha << ctx.Valid() << std::endl;

    auto str1 = ctx.Constructor();
    //std::cout << str0 << std::endl;
    std::cout << std::boolalpha << (str0 == uns::string_cast<std::string>(str1)) << std::endl;
    std::wcout << str1 << std::endl;

    std::wcout << ctx[tns::CTX_CSTRING::string_c_1] << std::endl;

    //std::wcout << str1 << std::endl;

    /*uns::nn::ctx.ErrorFix("---", "Thread 0");

    std::thread t1([str0]() {
        auto res = uns::nn::ctx.Settle(uns::unified_cast<std::wstring>(str0), uns::unified_cast<std::wstring>("\n"), true);
        std::wcout << std::boolalpha << L"result:" << res + L"\n" << std::endl;
        uns::nn::ctx.ErrorFix("#1", "02/04/2020 Thread 1");
    });
    std::thread t2([str0]() {
        auto res = uns::nn::ctx.Settle(uns::unified_cast<std::wstring>(str0), uns::unified_cast<std::wstring>("\n"), true);
        std::wcout << std::boolalpha << L"result:" << res + L"\n" << std::endl;
        uns::nn::ctx.ErrorFix("#2", "02/04/2020 Thread 2");
        });
    t1.join();
    t2.join();*/

    std::cout << "FINISH" << std::endl;
}
