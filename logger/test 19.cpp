// logger.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include "uniself/algorithms.h"
#include "logging.h"

using namespace std::chrono_literals;

template<typename string_t>
string_t TimeToString(const std::chrono::system_clock::time_point& moment) {
    return uns::string_cast<string_t>(moment.time_since_epoch().count());
};

using logger = uns::logger<std::wstring>;

int main() {
    std::cout << "START" << std::endl;

    uns::logger<std::wstring>::Setup(
        "D:/Документы/Test",
        "EmerLog",
        1000,
        2000,
        5,
        10,
        1ms,
        uns::subsystem::all,
        uns::urgency::none
    );

    logger::Start();

    std::this_thread::sleep_for(1s);

    for (int i = 0; i < 10; i++) {
        logger::ToLog(__FUNCTION__, __LINE__, 10, uns::urgency::low, uns::string_cast<std::string>(i));
    };
    logger::ToLogImmediate(__FUNCTION__, __LINE__, 10, uns::urgency::low, "");

    logger::Flush();

    logger::Finish();

    std::cout << "FINISH" << std::endl;
};