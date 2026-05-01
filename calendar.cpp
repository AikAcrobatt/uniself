
#include "uniself/calendar.hpp"

#include <exception>
#include <tuple>

::uns::calendar::gregorian::datetime::datetime(const ::uns::calendar::gregorian::time_point& TimePoint) noexcept {
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::floor<::std::chrono::days>(TimePoint)
    };

    year = static_cast<int>(ymd.year());
    month = static_cast<::uns::calendar::gregorian::month::enum_type>(static_cast<unsigned int>(ymd.month()) - 1);
    day = static_cast<::uns::calendar::gregorian::day_of_month::enum_type>(static_cast<unsigned int>(ymd.day()) - 1);

    const auto hms = ::std::chrono::hh_mm_ss{
        ::std::chrono::floor<::std::chrono::seconds>(TimePoint.time_since_epoch() - ::std::chrono::floor<::std::chrono::days>(TimePoint).time_since_epoch())
    };

    hours = static_cast<::uns::calendar::gregorian::hour::enum_type>(hms.hours().count());
    minutes = static_cast<::uns::calendar::gregorian::minute::enum_type>(hms.minutes().count());
    seconds = static_cast<::uns::calendar::gregorian::second::enum_type>(hms.seconds().count());

    nanoseconds = ::std::chrono::floor<::std::chrono::nanoseconds>(
            TimePoint.time_since_epoch()
            - ::std::chrono::floor<::std::chrono::seconds>(TimePoint).time_since_epoch()
        ).count();
};

::uns::calendar::gregorian::datetime::operator time_point() const {
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(year),
        ::std::chrono::month(static_cast<::uns::calendar::gregorian::month::integral_type>(month)),
        ::std::chrono::day(static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(day))
    };

    if (!ymd.ok()) return {};

    auto time = ::std::chrono::system_clock::time_point{ ::std::chrono::sys_days(ymd) };

    time += ::std::chrono::hours{ 1 } * static_cast<::uns::calendar::gregorian::hour::integral_type>(hours);
    time += ::std::chrono::minutes{ 1 } * static_cast<::uns::calendar::gregorian::minute::integral_type>(minutes);
    time += ::std::chrono::seconds{ 1 } * static_cast<::uns::calendar::gregorian::second::integral_type>(seconds);

    time += ::std::chrono::duration_cast<::std::chrono::system_clock::duration>(::std::chrono::nanoseconds{ 1 } * nanoseconds);

    return time;
};

bool ::uns::calendar::gregorian::datetime::ok() const noexcept {
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(year),
        ::std::chrono::month(static_cast<::uns::calendar::gregorian::month::integral_type>(month)),
        ::std::chrono::day(static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(day))
    };

    if (!ymd.ok()) return false;

    return (nanoseconds >= 0) && (nanoseconds < ::std::chrono::seconds{ 1 } / ::std::chrono::nanoseconds{ 1 });
};
