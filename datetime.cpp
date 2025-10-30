
#include "uniself/datetime.hpp"

::uns::datetime::datetime(const ::std::chrono::system_clock::time_point& time_point) noexcept {
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::floor<::std::chrono::days>(time_point)
    };

    year = static_cast<int>(ymd.year());
    month = static_cast<int>(static_cast<unsigned int>(ymd.month()));
    day_of_month = static_cast<int>(static_cast<unsigned int>(ymd.day()));

    const auto hms = ::std::chrono::hh_mm_ss{
        ::std::chrono::floor<::std::chrono::seconds>(time_point.time_since_epoch() - ::std::chrono::floor<::std::chrono::days>(time_point).time_since_epoch())
    };

    hours = static_cast<int>(hms.hours().count());
    minutes = static_cast<int>(hms.minutes().count());
    seconds = static_cast<int>(hms.seconds().count());

    subseconds = static_cast<long double>(
        ::std::chrono::floor<::std::chrono::nanoseconds>(
            time_point.time_since_epoch()
            - ::std::chrono::floor<::std::chrono::seconds>(time_point).time_since_epoch()
        ).count()
    )
    / static_cast<long double>(
        ::std::chrono::seconds(1)
        / ::std::chrono::nanoseconds(1)
    );

    this->normalize();
};

::uns::datetime::operator uns::datetime::time_point() const noexcept {
    auto datetime_struct = *this;

    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(datetime_struct.year),
        ::std::chrono::month(datetime_struct.month),
        ::std::chrono::day(datetime_struct.day_of_month)
    };

    if(!ymd.ok()) return ::std::chrono::system_clock::time_point();

    auto time = ::std::chrono::system_clock::time_point{ ::std::chrono::sys_days(ymd) };

    time += ::std::chrono::hours(1) * datetime_struct.hours;
    time += ::std::chrono::minutes(1) * datetime_struct.minutes;
    time += ::std::chrono::seconds(1) * datetime_struct.seconds;

    auto subseconds = static_cast<long long int>(static_cast<long double>(::std::chrono::seconds(1) / ::std::chrono::nanoseconds(1)) * datetime_struct.subseconds);
    time += ::std::chrono::duration_cast<::std::chrono::system_clock::duration>(::std::chrono::nanoseconds(1) * subseconds);

    return time;
};

void ::uns::datetime::normalize() noexcept {
    constexpr auto subseconds_in_second = ::std::chrono::seconds(1) / ::std::chrono::nanoseconds(1);
    constexpr auto time_fraction_accuracy = 0.01L / static_cast<long double>(subseconds_in_second);

    if(auto is_invalid = (this->subseconds < -time_fraction_accuracy); is_invalid || this->subseconds > 1.0 - time_fraction_accuracy) {
        auto subseconds = static_cast<long long int>(static_cast<long double>(subseconds_in_second) * this->subseconds);
        this->seconds += static_cast<int>(subseconds / subseconds_in_second + (is_invalid ? -1 : 0));
        this->subseconds = static_cast<long double>((subseconds % subseconds_in_second) + (is_invalid ? subseconds_in_second : 0)) / static_cast<long double>(subseconds_in_second);
    };

    if(auto is_invalid = (this->seconds < 0); is_invalid || this->seconds > 59) {
        this->minutes += this->seconds / 60 + (is_invalid ? -1 : 0);
        this->seconds = (this->seconds % 60) + (is_invalid ? 60 : 0);
    };

    if(auto is_invalid = (this->minutes < 0); is_invalid || this->minutes > 59) {
        this->hours += this->minutes / 60 + (is_invalid ? -1 : 0);
        this->minutes = (this->minutes % 60) + (is_invalid ? 60 : 0);
    };

    if(auto is_invalid = (this->hours < 0); is_invalid || this->hours > 23) {
        this->day_of_month += this->hours / 24 + (is_invalid ? -1 : 0);
        this->hours = (this->hours % 24) + (is_invalid ? 24 : 0);
    };

    auto num_days_in_month_cur = 30;
    auto num_days_in_month_prev = 30;
    do {
        switch(this->month) {
            default:
            case 1:
            case 8:
            {
                num_days_in_month_cur = 31;
                num_days_in_month_prev = 31;
                break;
            }
            case 3:
            {
                num_days_in_month_cur = 31;
                num_days_in_month_prev = (::std::chrono::year(this->year).is_leap() ? 29 : 28);
                break;
            }
            case 5:
            case 7:
            case 10:
            case 12:
            {
                num_days_in_month_cur = 31;
                num_days_in_month_prev = 30;
                break;
            }
            case 4:
            case 6:
            case 9:
            case 11:
            {
                num_days_in_month_cur = 30;
                num_days_in_month_prev = 31;
                break;
            }
            case 2:
            {
                num_days_in_month_cur = (::std::chrono::year(this->year).is_leap() ? 29 : 28);
                num_days_in_month_prev = 31;
                break;
            };
        };

        if(this->day_of_month < 1) {
            this->month--;
            this->day_of_month += num_days_in_month_prev;
        };
        if(this->day_of_month > num_days_in_month_cur) {
            this->month++;
            this->day_of_month -= num_days_in_month_cur;
        };

        if(auto is_invalid = (this->month < 1); is_invalid || this->month > 12) {
            this->year += this->month / 12 + (is_invalid ? -1 : 0);
            this->month = (this->month % 12) + (is_invalid ? 12 : 0);
        };

    }
    while(
        !(
            this->day_of_month >= 1
            && this->day_of_month <= num_days_in_month_cur
            && this->month >= 1
            && this->month <= 12
        )
    );

    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(this->year),
        ::std::chrono::month(this->month),
        ::std::chrono::day(this->day_of_month)
    };

    const auto weekday = ::std::chrono::weekday{
        ::std::chrono::floor<::std::chrono::days>(::std::chrono::sys_days(ymd))
    };

    this->year_is_leap = ymd.year().is_leap();
    this->day_of_week = static_cast<int>(weekday.iso_encoding());
    this->day_of_year = (::std::chrono::sys_days(ymd) - ::std::chrono::sys_days(::std::chrono::year(this->year) / 1 / 1)).count() + 1;
    this->week_of_year = (this->day_of_year - this->day_of_week) / 7 + 1;
};

::uns::datetime::time_point uns::timestamp() noexcept {
    static const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(1970),
        ::std::chrono::month(1),
        ::std::chrono::day(1)
    };

    return static_cast<::std::chrono::system_clock::time_point>(::std::chrono::sys_days(ymd));
};
