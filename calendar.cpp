
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

::uns::calendar::gregorian::datetime::operator uns::calendar::gregorian::time_point() const {
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(year),
        ::std::chrono::month(static_cast<::uns::calendar::gregorian::month::integral_type>(month) + 1),
        ::std::chrono::day(static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(day) + 1)
    };

    if (!ymd.ok()) return {};

    auto time = ::uns::calendar::gregorian::time_point{ ::std::chrono::sys_days(ymd) };

    time += ::std::chrono::hours{ 1 } * static_cast<::uns::calendar::gregorian::hour::integral_type>(hours);
    time += ::std::chrono::minutes{ 1 } * static_cast<::uns::calendar::gregorian::minute::integral_type>(minutes);
    time += ::std::chrono::seconds{ 1 } * static_cast<::uns::calendar::gregorian::second::integral_type>(seconds);

    time += ::std::chrono::duration_cast<::std::chrono::system_clock::duration>(::std::chrono::nanoseconds{ 1 } * nanoseconds);

    return time;
};

bool ::uns::calendar::gregorian::datetime::ok() const noexcept {
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year(year),
        ::std::chrono::month(static_cast<::uns::calendar::gregorian::month::integral_type>(month) + 1),
        ::std::chrono::day(static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(day) + 1)
    };

    if (!ymd.ok()) return false;

    return (nanoseconds >= 0) && (nanoseconds < ::std::chrono::seconds{ 1 } / ::std::chrono::nanoseconds{ 1 });
};


::uns::calendar::gregorian::traits::traits(const ::uns::calendar::gregorian::datetime& DateTime) :
    m_isok(DateTime.ok())
{
    const auto ymd = ::std::chrono::year_month_day{
        ::std::chrono::year{ DateTime.year }
        , ::std::chrono::month{
            static_cast<unsigned int>(
                static_cast<::uns::calendar::gregorian::month::integral_type>(DateTime.month)
            ) + 1
        }
        , ::std::chrono::day{
            static_cast<unsigned int>(
                static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(DateTime.day)
            ) + 1
        }
    };

    const auto weekday = ::std::chrono::weekday{
        ::std::chrono::floor<::std::chrono::days>(::std::chrono::sys_days(ymd))
    };

    m_year_is_leap = ymd.year().is_leap();
    m_weekday = static_cast<::uns::calendar::gregorian::weekday::enum_type>(weekday.iso_encoding() - 1);
    m_day_of_year = (::std::chrono::sys_days(ymd) - ::std::chrono::sys_days(::std::chrono::year(DateTime.year) / 1 / 1)).count() + 1;
};
::uns::calendar::gregorian::weekday uns::calendar::gregorian::traits::weekday() const noexcept {
    return m_weekday;
};
int ::uns::calendar::gregorian::traits::day_of_year() const noexcept {
    return m_day_of_year;
};
bool ::uns::calendar::gregorian::traits::year_is_leap() const noexcept {
    return m_year_is_leap;
};
bool ::uns::calendar::gregorian::traits::ok() const noexcept {
    return m_isok;
};
