
#include "uniself/calendar.hpp"

#include <exception>
#include <tuple>

namespace uns::calendar::gregorian::auxiliary {

    constexpr int seconds_per_day = 86400;
    constexpr int days_per_year = 365;
    constexpr int days_per_fouryear = 4 * days_per_year + 1;
    constexpr int days_per_age = 25 * days_per_fouryear - 1;
    constexpr int days_per_fourage = 4 * days_per_age + 1;


    struct months_limits {
        int begin = 0;
        int end = 0;
    };
    constexpr months_limits months_hinnant_limits[12] = {
        { 306, 336 }    //january
        , { 337, 365 }  //february
        , { 0, 30 }     //march
        , { 31, 60 }    //april
        , { 61, 91 }    //may
        , { 92, 121 }   //june
        , { 122, 152 }  //july
        , { 153, 183 }  //august
        , { 184, 213 }  //september
        , { 214, 244 }  //october
        , { 245, 274 }  //november
        , { 275, 305 }  //december
    };


    ::std::tuple<
        ::uns::calendar::gregorian::month
        , ::uns::calendar::gregorian::day_of_month
    > get_month_n_day(
        ::std::size_t PivotMonthLowest
        , int HinnantDayOfYear
        , ::std::size_t PivotMonthHighest
    );
    ::std::tuple<
        ::uns::calendar::gregorian::month
        , ::uns::calendar::gregorian::day_of_month
    > get_month_n_day(
        ::std::size_t PivotMonthLowest
        , int HinnantDayOfYear
        , ::std::size_t PivotMonthHighest
    ) {
        constexpr ::uns::calendar::gregorian::month months_idxs[12] = {
            ::uns::calendar::gregorian::month::march
            , ::uns::calendar::gregorian::month::april
            , ::uns::calendar::gregorian::month::may
            , ::uns::calendar::gregorian::month::june
            , ::uns::calendar::gregorian::month::july
            , ::uns::calendar::gregorian::month::august
            , ::uns::calendar::gregorian::month::september
            , ::uns::calendar::gregorian::month::october
            , ::uns::calendar::gregorian::month::november
            , ::uns::calendar::gregorian::month::december
            , ::uns::calendar::gregorian::month::january
            , ::uns::calendar::gregorian::month::february
        };

        const auto pivot_month_idx = (PivotMonthHighest + PivotMonthLowest) / 2;
        const auto& pivot_month = ::uns::calendar::gregorian::auxiliary::months_hinnant_limits[
            months_idxs[pivot_month_idx]
        ];
        if (
            pivot_month.begin <= HinnantDayOfYear
            && HinnantDayOfYear <= pivot_month.end
            ) {
            return {
                months_idxs[pivot_month_idx]
                , static_cast<::uns::calendar::gregorian::day_of_month::enum_type>(
                    HinnantDayOfYear % pivot_month.begin
                )
            };
        }
        else if (pivot_month.begin > HinnantDayOfYear) {
            return ::uns::calendar::gregorian::auxiliary::get_month_n_day(
                PivotMonthLowest
                , HinnantDayOfYear
                , pivot_month_idx - 1
            );
        }
        else {
            return ::uns::calendar::gregorian::auxiliary::get_month_n_day(
                pivot_month_idx + 1
                , HinnantDayOfYear
                , PivotMonthHighest
            );
        };
    };
    ::std::tuple<
        ::uns::calendar::gregorian::month
        , ::uns::calendar::gregorian::day_of_month
    > get_month_n_day(int HinnantDayOfYear) {
        return ::uns::calendar::gregorian::auxiliary::get_month_n_day(
            0
            , HinnantDayOfYear
            , 11
        );
    };

};


namespace uns::calendar::hinnant::auxiliary {

    constexpr auto offset_posix_to_this_epoch = ::std::chrono::seconds{
        ::uns::calendar::gregorian::auxiliary::seconds_per_day
        * (
            7 * ::uns::calendar::gregorian::auxiliary::days_per_fouryear
            + 2 * ::uns::calendar::gregorian::auxiliary::days_per_year
            + 59    //january + february of 1970 year
        )
    };
    constexpr int offset_posix_to_hinnant_years = 2000;

    ::uns::calendar::gregorian::datetime get_epoch() noexcept {
        auto gregorian_hinnant_epoch = ::uns::calendar::gregorian::datetime{};

        gregorian_hinnant_epoch.year = ::uns::calendar::hinnant::auxiliary::offset_posix_to_hinnant_years;
        gregorian_hinnant_epoch.month = ::uns::calendar::gregorian::month::march;
        gregorian_hinnant_epoch.day = ::uns::calendar::gregorian::day_of_month::d01;

        gregorian_hinnant_epoch.hours = ::uns::calendar::gregorian::hour::h00;
        gregorian_hinnant_epoch.minutes = ::uns::calendar::gregorian::minute::m00;
        gregorian_hinnant_epoch.seconds = ::uns::calendar::gregorian::second::s00;

        gregorian_hinnant_epoch.nanoseconds = 0;

        return gregorian_hinnant_epoch;
    };


    bool is_leap_year(
        const ::uns::calendar::hinnant::age& Age
        , const ::uns::calendar::hinnant::fouryear& Fouryear
        , const ::uns::calendar::hinnant::year_of_four& Year
    ) noexcept {
        if (Year == ::uns::calendar::hinnant::year_of_four::y3) {
            if (Fouryear != ::uns::calendar::hinnant::fouryear::f24) {
                return true;
            }
            else {
                if (Age == ::uns::calendar::hinnant::age::a3) {
                    return true;
                };
            };
        };

        return false;
    };
    bool is_leap_year(
        const ::uns::calendar::hinnant::datetime& HinnantDateTime
    ) noexcept {
        return ::uns::calendar::hinnant::auxiliary::is_leap_year(
            HinnantDateTime.age
            , HinnantDateTime.fouryear
            , HinnantDateTime.year
        );
    };
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

::uns::calendar::hinnant::datetime::operator ::uns::calendar::hinnant::time_point() const {
    if (!ok()) {
        throw ::std::runtime_error{ "Hinnant datetime is invalid" };
    };

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
    const int leap_day = (
        ::uns::calendar::hinnant::auxiliary::is_leap_year(*this)
        ? 1
        : 0
    );

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


::uns::calendar::gregorian::datetime::datetime(const ::uns::calendar::gregorian::time_point& TimePoint) noexcept :
    datetime{
        ::uns::calendar::hinnant::datetime{ TimePoint }
    }
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
    if (thisobj.year <= 0) {
        thisobj.year -= 1;  //there wasn't a '0000' year of AC in gregorian calendar
    };

    ::std::tie(thisobj.month, thisobj.day) = ::uns::calendar::gregorian::auxiliary::get_month_n_day(HinnantDateTime.day);
    if (
        thisobj.month == ::uns::calendar::gregorian::month::january
        || thisobj.month == ::uns::calendar::gregorian::month::february
    ) {
        thisobj.year += 1;
    };

    thisobj.hours = static_cast<::uns::calendar::gregorian::hour::enum_type>(
        HinnantDateTime.seconds
        / ::uns::calendar::gregorian::minute::size()
        / ::uns::calendar::gregorian::second::size()
    );
    thisobj.minutes = static_cast<::uns::calendar::gregorian::minute::enum_type>(
        HinnantDateTime.seconds
        / ::uns::calendar::gregorian::second::size()
    );
    thisobj.seconds = static_cast<::uns::calendar::gregorian::second::enum_type>(
        HinnantDateTime.seconds
        % ::uns::calendar::gregorian::second::size()
    );

    thisobj.nanoseconds = HinnantDateTime.nanoseconds;
};

::uns::calendar::gregorian::datetime::operator ::uns::calendar::hinnant::datetime() const {
    if (!ok()) {
        throw ::std::runtime_error{ "Gregorian datetime is invalid" };
    };

    auto hinnant_datetime = ::uns::calendar::hinnant::datetime{};

    hinnant_datetime.nanoseconds = nanoseconds;

    hinnant_datetime.seconds = seconds
        + static_cast<int>(minutes)
            * static_cast<int>(::uns::calendar::gregorian::second::size())
        + static_cast<int>(hours)
            * static_cast<int>(::uns::calendar::gregorian::minute::size())
            * static_cast<int>(::uns::calendar::gregorian::second::size());

    int hinnant_year = year;
    if (hinnant_year < 0) {
        hinnant_year += 1;  //because there is no 0 year in gregorian calendar
    };
    hinnant_year = hinnant_year - ::uns::calendar::hinnant::auxiliary::offset_posix_to_hinnant_years
        - (month == ::uns::calendar::gregorian::month::january ? 1 : 0)
        - (month == ::uns::calendar::gregorian::month::february ? 1 : 0);

    hinnant_datetime.fourage =
        hinnant_year
        / static_cast<int>(::uns::calendar::hinnant::age::size())
        / static_cast<int>(::uns::calendar::hinnant::fouryear::size())
        / static_cast<int>(::uns::calendar::hinnant::year_of_four::size());
    if (hinnant_year < 0) {
        hinnant_datetime.fourage -= 1;
    };

    int hinnant_age = 
        hinnant_year
        / static_cast<int>(::uns::calendar::hinnant::fouryear::size())
        / static_cast<int>(::uns::calendar::hinnant::year_of_four::size());
    if (hinnant_year < 0) {
        hinnant_age += static_cast<int>(::uns::calendar::hinnant::age::size() - 1);
    };
    hinnant_datetime.age = static_cast<::uns::calendar::hinnant::age::enum_type>(hinnant_age);

    int hinnant_fouryear =
        hinnant_year
        / static_cast<int>(::uns::calendar::hinnant::year_of_four::size());
    if (hinnant_year < 0) {
        hinnant_fouryear += static_cast<int>(::uns::calendar::hinnant::fouryear::size() - 1);
    };
    hinnant_datetime.fouryear = static_cast<::uns::calendar::hinnant::fouryear::enum_type>(hinnant_fouryear);

    hinnant_year =
        hinnant_year
        % static_cast<int>(::uns::calendar::hinnant::year_of_four::size());
    if (hinnant_year < 0) {
        hinnant_year += static_cast<int>(::uns::calendar::hinnant::year_of_four::size() - 1);
    };
    hinnant_datetime.year = static_cast<::uns::calendar::hinnant::year_of_four::enum_type>(hinnant_year);
};

::uns::calendar::gregorian::datetime::operator time_point() const {
    return static_cast<::uns::calendar::hinnant::datetime>(*this);
};

bool ::uns::calendar::gregorian::datetime::ok() const noexcept {
    if (year == 0) return false;    //gregorian year cannot be 0 (-1 year was followed by 1 year)

    if (
        nanoseconds < 0
        || nanoseconds >= (
            ::std::chrono::seconds{ 1 }
            / ::std::chrono::nanoseconds{ 1 }
        )
    ) {
        return false;
    };

    /*auto hinnant_equivalent = uns::calendar::hinnant::datetime{};
    try {
        hinnant_equivalent = *this;//TODO !!!THIS IS AN ERROR! THIS WILL CAUSE STACK OVERFLOW!!!
    }
    catch (const ::std::runtime_error&) {
        return false;
    };*/

    const bool is_leap = ::uns::calendar::hinnant::auxiliary::is_leap_year(hinnant_equivalent);
};
