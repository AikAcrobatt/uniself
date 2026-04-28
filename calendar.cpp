
#include "uniself/calendar.hpp"

#include <exception>

namespace uns::calendar::hinnant::auxiliary {

    constexpr auto offset_posix_to_this_epoch = ::std::chrono::seconds{};

    ::uns::calendar::gregorian::datetime get_epoch() noexcept {
        auto gregorian_hinnant_epoch = ::uns::calendar::gregorian::datetime{};

        gregorian_hinnant_epoch.year = 2000;
        gregorian_hinnant_epoch.month = ::uns::calendar::gregorian::month::march;
        gregorian_hinnant_epoch.day = ::uns::calendar::gregorian::day_of_month::d01;

        gregorian_hinnant_epoch.hours = ::uns::calendar::gregorian::hour::h00;
        gregorian_hinnant_epoch.minutes = ::uns::calendar::gregorian::minute::m00;
        gregorian_hinnant_epoch.seconds = ::uns::calendar::gregorian::second::s00;

        gregorian_hinnant_epoch.nanoseconds = 0;

        return gregorian_hinnant_epoch;
    };

};

namespace uns::calendar::gregorian::auxiliary {

    constexpr int seconds_per_day = 86400;
    constexpr int days_per_year = 365;
    constexpr int days_per_leap_year = 365;
    constexpr int days_per_fouryear = days_per_leap_year + 3 * days_per_year;
    constexpr int days_per_age = 25 * days_per_fouryear - 1;
    constexpr int days_per_fourage = 4 * days_per_age + 1;

};


::uns::calendar::hinnant::datetime::datetime(const ::uns::calendar::hinnant::time_point& TimePoint) noexcept {
    const auto seconds_since_epoch = ::std::chrono::duration_cast<::std::chrono::seconds>(
        TimePoint.time_since_epoch() - ::uns::calendar::hinnant::auxiliary::offset_posix_to_this_epoch
    ).count();
    const int days_since_epoch =
        seconds_since_epoch
        / ::uns::calendar::gregorian::auxiliary::seconds_per_day;
    nanoseconds = (
        ::std::chrono::duration_cast<::std::chrono::microseconds>(
            TimePoint.time_since_epoch()
            % ::std::chrono::seconds{1}
        ).count()
    ) * (
        ::std::chrono::microseconds{ 1 }
        / ::std::chrono::nanoseconds{ 1 }
    );

    fourage =
        days_since_epoch
        / ::uns::calendar::gregorian::auxiliary::days_per_fourage;
    const int days_since_last_fourage =
        days_since_epoch
        - fourage * ::uns::calendar::gregorian::auxiliary::days_per_fourage;

    age = static_cast<::uns::calendar::hinnant::age::enum_type>(
        days_since_last_fourage
        / ::uns::calendar::gregorian::auxiliary::days_per_age
    );
    const int days_since_last_age =
        days_since_last_fourage
        - age * ::uns::calendar::gregorian::auxiliary::days_per_age;

    fouryear = static_cast<::uns::calendar::hinnant::fouryear::enum_type>(
        days_since_last_age
        / ::uns::calendar::gregorian::auxiliary::days_per_fouryear
    );
    const int days_since_last_fouryear =
        days_since_last_age
        - fouryear * ::uns::calendar::gregorian::auxiliary::days_per_fouryear;

    year = static_cast<::uns::calendar::hinnant::year_of_four::enum_type>(
        days_since_last_fouryear
        / ::uns::calendar::gregorian::auxiliary::days_per_year
    );
    const int days_since_last_year =
        days_since_last_fouryear
        - year * ::uns::calendar::gregorian::auxiliary::days_per_fouryear;

    day =
        days_since_last_fouryear
        - year * ::uns::calendar::gregorian::auxiliary::days_per_year;

    seconds = seconds_since_epoch % ::uns::calendar::gregorian::auxiliary::seconds_per_day;
};

::uns::calendar::hinnant::datetime::operator ::uns::calendar::hinnant::time_point() const noexcept {
    constexpr auto _1s = ::std::chrono::seconds{ 1 };
    constexpr auto _1d = _1s * ::uns::calendar::gregorian::auxiliary::seconds_per_day;

    auto time_point = ::uns::calendar::hinnant::time_point{};

    time_point += _1d * ::uns::calendar::gregorian::auxiliary::days_per_fourage * fourage;
    time_point += _1d * ::uns::calendar::gregorian::auxiliary::days_per_age * age;
    time_point += _1d * ::uns::calendar::gregorian::auxiliary::days_per_fouryear * fouryear;
    time_point += _1d * ::uns::calendar::gregorian::auxiliary::days_per_year * year;
    time_point += _1d * day;

    time_point += _1s * seconds;

    time_point += ::std::chrono::microseconds{
        nanoseconds
        / (
            ::std::chrono::microseconds{ 1 }
            / ::std::chrono::nanoseconds{ 1 }
        )
    };

    return time_point;
};

bool ::uns::calendar::hinnant::datetime::ok() const noexcept {
    int leap_day = 0;
    if (year == ::uns::calendar::hinnant::year_of_four::y3) {
        if (fourage != ::uns::calendar::hinnant::fouryear::f24) {
            leap_day = 1;
        }
        else {
            if (age == ::uns::calendar::hinnant::age::a3) {
                leap_day = 1;
            };
        };
    };

    if (day < 0 || day >= ::uns::calendar::gregorian::auxiliary::days_per_year + leap_day) {
        return false;
    };

    if (seconds < 0 || seconds >= ::uns::calendar::gregorian::auxiliary::seconds_per_day) {
        return false;
    };

    if (nanoseconds < 0 || nanoseconds >= ::std::chrono::seconds{ 1 } / ::std::chrono::nanoseconds{ 1 }) {
        return false;
    };

    return true;
};


::uns::calendar::gregorian::datetime::datetime(const ::uns::calendar::gregorian::time_point& TimePoint) :
    datetime(
        ::uns::calendar::hinnant::datetime{ TimePoint }
    )
{};
::uns::calendar::gregorian::datetime::datetime(const ::uns::calendar::hinnant::datetime& HinnantDateTime) {
    if (!HinnantDateTime.ok()) {
        throw ::std::runtime_error{ "Initial Hinnant datetime is invalid" };
    };

    auto thisobj = uns::calendar::hinnant::auxiliary::get_epoch();

    thisobj.year +=
        (
            (
                HinnantDateTime.fourage
                * static_cast<int>(::uns::calendar::hinnant::age::size())
                + static_cast<int>(HinnantDateTime.age)
            ) * static_cast<int>(::uns::calendar::hinnant::fouryear::size())
            + static_cast<int>(HinnantDateTime.fouryear)
        ) * static_cast<int>(::uns::calendar::hinnant::year_of_four::size())
        + static_cast<int>(HinnantDateTime.year);

    constexpr int first_day_of_month[] = {
        0       //first day of march
        , 31    //first day of april
        , 61    //first day of may
        , 92    //first day of june
        , 122   //first day of july
        , 153   //first day of august
        , 184   //first day of september
        , 214   //first day of october
        , 245   //first day of november
        , 275   //first day of december
        , 306   //first day of january
        , 337   //first day of februrary
    };
    constexpr int last_day_of_month[] = {
        30      //last day of march
        , 60    //last day of april
        , 91    //last day of may
        , 121   //last day of june
        , 152   //last day of july
        , 183   //last day of august
        , 213   //last day of september
        , 244   //last day of october
        , 274   //last day of november
        , 305   //last day of december
        , 336   //last day of january
        , 354   //last day of februrary
    };

    thisobj.nanoseconds = HinnantDateTime.nanoseconds;
};

::uns::calendar::gregorian::datetime::operator ::uns::calendar::hinnant::datetime() const noexcept {
};

::uns::calendar::gregorian::datetime::operator time_point() const noexcept {
};

bool ::uns::calendar::gregorian::datetime::ok() const noexcept {
};
