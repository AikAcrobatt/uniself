#pragma once

#include <chrono>

#ifndef UNS_LIB_DATETIME
#define UNS_LIB_DATETIME

namespace uns {

    class datetime {
    public:
        using clock_type = ::std::chrono::system_clock;
        using time_point = typename clock_type::time_point;
    public:
        int year = 0;
        int month = 0;
        int day_of_month = 0;
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        int week_of_year = 0;
        int day_of_year = 0;
        int day_of_week = 0;
        bool year_is_leap = false;
        long double subseconds = 0.0;
    public:
        datetime() noexcept = default;
        explicit datetime(const time_point& time_point) noexcept;
        datetime(const ::uns::datetime&) noexcept = default;
        ::uns::datetime& operator=(const ::uns::datetime&) noexcept = default;
        datetime(::uns::datetime&&) noexcept = default;
        ::uns::datetime& operator=(::uns::datetime&&) noexcept = default;
        ~datetime() noexcept = default;

        void normalize() noexcept;

        operator time_point() const noexcept;
    };

    ::uns::datetime::time_point timestamp() noexcept;
};

#endif