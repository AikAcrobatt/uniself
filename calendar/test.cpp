
#include "uniself/calendar.hpp"
#include "uniself/renum.hpp"
#include "uniself/strings.hpp"

#include <chrono>
#include <vector>

#include "gtest/gtest.h"

namespace uns::tests::calendar {

    template<typename time_point_t, typename clock_t>
    class trivial_tests : public ::testing::TestWithParam<time_point_t> {
    public:
        using param_type = time_point_t;
        using clock = clock_t;
    public:
        static ::std::vector<param_type> generate_tests() {
            auto result = ::std::vector<param_type>{};

            constexpr auto step = ::std::chrono::seconds{ 123457 };
            for (param_type point = {}; point < clock::now(); point += step) {
                result.emplace_back(point);
            };

            return result;
        };
    };


    bool has_additive_days(const ::uns::calendar::gregorian::datetime& DateTime) noexcept {
        return DateTime.day < ::uns::calendar::gregorian::day_of_month::d28;
    };
    bool has_additive_hours(const ::uns::calendar::gregorian::datetime& DateTime) noexcept {
        return DateTime.hours < ::uns::calendar::gregorian::hour::h23;
    };
    bool has_additive_minutes(const ::uns::calendar::gregorian::datetime& DateTime) noexcept {
        return DateTime.minutes < ::uns::calendar::gregorian::minute::m59;
    };
    bool has_additive_seconds(const ::uns::calendar::gregorian::datetime& DateTime) noexcept {
        return DateTime.seconds < ::uns::calendar::gregorian::second::s59;
    };
    bool has_additive_nanoseconds(const ::uns::calendar::gregorian::datetime& DateTime) noexcept {
        return DateTime.nanoseconds < ::std::chrono::seconds{ 1 } / ::std::chrono::nanoseconds{ 1 } - 2000;
    };
};

using GregorianTrivialTests = ::uns::tests::calendar::trivial_tests<
    ::uns::calendar::gregorian::time_point
    , ::uns::calendar::gregorian::clock_type
>;

template<>
::std::string testing::PrintToString(const ::GregorianTrivialTests::param_type& Param) {
    return ::testing::PrintToString(Param.time_since_epoch().count());
};

TEST_P(GregorianTrivialTests, FwdCast) {
    const auto time_point = GetParam();

    const auto datetime = ::uns::calendar::gregorian::datetime(time_point);

    ASSERT_EQ(
        time_point
        , static_cast<::uns::calendar::gregorian::time_point>(datetime)
    );
};
TEST_P(GregorianTrivialTests, AdditiveCheck) {
    const auto time_point = GetParam();
    auto time_point1 = time_point;

    const auto datetime = ::uns::calendar::gregorian::datetime(time_point);
    auto datetime1 = datetime;

    {
        time_point1 = time_point;
        datetime1 = datetime;

        if (::uns::tests::calendar::has_additive_days(datetime)) {
            datetime1.day = static_cast<::uns::calendar::gregorian::day_of_month::enum_type>(
                static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(
                    datetime1.day
                ) + 1
            );

            time_point1 += ::std::chrono::seconds{ 86400 };
        }
        else {
            datetime1.day = static_cast<::uns::calendar::gregorian::day_of_month::enum_type>(
                static_cast<::uns::calendar::gregorian::day_of_month::integral_type>(
                    datetime1.day
                ) - 1
            );

            time_point1 -= ::std::chrono::seconds{ 86400 };
        };

        ASSERT_EQ(
            time_point1
            , static_cast<::uns::calendar::gregorian::time_point>(datetime1)
        );
        ASSERT_TRUE(datetime1.ok());
    };
    {
        time_point1 = time_point;
        datetime1 = datetime;

        if (::uns::tests::calendar::has_additive_hours(datetime)) {
            datetime1.hours = static_cast<::uns::calendar::gregorian::hour::enum_type>(
                static_cast<::uns::calendar::gregorian::hour::integral_type>(
                    datetime1.hours
                ) + 1
            );

            time_point1 += ::std::chrono::seconds{ 3600 };
        }
        else {
            datetime1.hours = static_cast<::uns::calendar::gregorian::hour::enum_type>(
                static_cast<::uns::calendar::gregorian::hour::integral_type>(
                    datetime1.hours
                ) - 1
            );

            time_point1 -= ::std::chrono::seconds{ 3600 };
        };

        ASSERT_EQ(
            time_point1
            , static_cast<::uns::calendar::gregorian::time_point>(datetime1)
        );
        ASSERT_TRUE(datetime1.ok());
    };
    {
        time_point1 = time_point;
        datetime1 = datetime;

        if (::uns::tests::calendar::has_additive_minutes(datetime)) {
            datetime1.minutes = static_cast<::uns::calendar::gregorian::minute::enum_type>(
                static_cast<::uns::calendar::gregorian::minute::integral_type>(
                    datetime1.minutes
                ) + 1
            );

            time_point1 += ::std::chrono::seconds{ 60 };
        }
        else {
            datetime1.minutes = static_cast<::uns::calendar::gregorian::minute::enum_type>(
                static_cast<::uns::calendar::gregorian::minute::integral_type>(
                    datetime1.minutes
                ) - 1
            );

            time_point1 -= ::std::chrono::seconds{ 60 };
        };

        ASSERT_EQ(
            time_point1
            , static_cast<::uns::calendar::gregorian::time_point>(datetime1)
        );
        ASSERT_TRUE(datetime1.ok());
    };
    {
        time_point1 = time_point;
        datetime1 = datetime;

        if (::uns::tests::calendar::has_additive_seconds(datetime)) {
            datetime1.seconds = static_cast<::uns::calendar::gregorian::second::enum_type>(
                static_cast<::uns::calendar::gregorian::second::integral_type>(
                    datetime1.seconds
                ) + 1
            );

            time_point1 += ::std::chrono::seconds{ 1 };
        }
        else {
            datetime1.seconds = static_cast<::uns::calendar::gregorian::second::enum_type>(
                static_cast<::uns::calendar::gregorian::second::integral_type>(
                    datetime1.seconds
                ) - 1
            );

            time_point1 -= ::std::chrono::seconds{ 1 };
        };

        ASSERT_EQ(
            time_point1
            , static_cast<::uns::calendar::gregorian::time_point>(datetime1)
        );
        ASSERT_TRUE(datetime1.ok());
    };
    {
        time_point1 = time_point;
        datetime1 = datetime;

        if (::uns::tests::calendar::has_additive_nanoseconds(datetime)) {
            datetime1.nanoseconds += 1000;

            time_point1 += ::std::chrono::microseconds{ 1 };
        }
        else {
            datetime1.nanoseconds -= 1000;

            time_point1 -= ::std::chrono::microseconds{ 1 };
        };

        ASSERT_EQ(
            time_point1
            , static_cast<::uns::calendar::gregorian::time_point>(datetime1)
        );
        ASSERT_TRUE(datetime1.ok());
    };
};
TEST_P(GregorianTrivialTests, OkCkeck) {
    const auto time_point = GetParam();

    auto datetime = ::uns::calendar::gregorian::datetime(time_point);
    ASSERT_TRUE(datetime.ok());

    datetime.nanoseconds = ::std::chrono::seconds{ 1 } / ::std::chrono::nanoseconds{ 1 };
    ASSERT_FALSE(datetime.ok());
};
INSTANTIATE_TEST_CASE_P(Calendar, GregorianTrivialTests,
    ::testing::ValuesIn(
        ::GregorianTrivialTests::generate_tests()
    )
);
