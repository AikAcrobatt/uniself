#pragma once

#include <chrono>

#include "uniself/math.h"

namespace uns::time {

	class date {
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

		date() = default;
		date(std::chrono::utc_clock::time_point time_point) {
			using base_timeunit = std::chrono::nanoseconds;
			static const int seconds_in_day = 86400;

			year = 1970;
			month = 1;

			auto duration = time_point - std::chrono::utc_clock::time_point{};
			day_of_month = 1 + (duration) / (std::chrono::seconds{ 1 } * seconds_in_day);

			duration -= std::chrono::seconds{ 1 } *seconds_in_day * (day_of_month - 1);
			seconds = std::chrono::floor<std::chrono::seconds>(duration).count();

			duration -= std::chrono::seconds{ 1 } * seconds;
			subseconds = static_cast<long double>(
				std::chrono::floor<base_timeunit>(duration).count()
				)
				/ static_cast<long double>(std::chrono::seconds{ 1 } / base_timeunit{ 1 });

			normalize();
		};
		date(const date&) = default;
		date& operator=(const date&) = default;
		date(date&&) = default;
		date& operator=(date&&) = default;
		~date() = default;

		operator std::chrono::utc_clock::time_point() {
			auto resulting_date = *this;

			resulting_date.normalize();

			auto res = std::chrono::utc_clock::time_point{};

			return res;
		};

		void normalize() {
			static const auto subseconds_in_second = static_cast<long long int>(std::chrono::seconds{ 1 } / std::chrono::utc_clock::duration{ 1 });

			if(auto is_invalid = uns::math::less(subseconds, 0.0L); is_invalid || uns::math::more(subseconds, 1.0L)) {
				auto extra_subseconds = static_cast<long long int>(static_cast<long double>(subseconds_in_second) * subseconds);
				seconds += extra_subseconds / subseconds_in_second + (is_invalid ? -1 : 0);
				subseconds = static_cast<long double>((extra_subseconds % subseconds_in_second) + (is_invalid ? subseconds_in_second : 0)) / static_cast<long double>(subseconds_in_second);
			};

			if(auto is_invalid = (seconds < 0); is_invalid || seconds > 59) {
				minutes += seconds / 60 + (is_invalid ? -1 : 0);
				seconds = (seconds % 60) + (is_invalid ? 60 : 0);
			};

			if(auto is_invalid = (minutes < 0); is_invalid || minutes > 59) {
				hours += minutes / 60 + (is_invalid ? -1 : 0);
				minutes = (minutes % 60) + (is_invalid ? 60 : 0);
			};

			if(auto is_invalid = (hours < 0); is_invalid || hours > 23) {
				day_of_month += hours / 24 + (is_invalid ? -1 : 0);
				hours = (hours % 24) + (is_invalid ? 24 : 0);
			};

			auto num_days_in_month_cur = 30;
			auto num_days_in_month_prev = 30;
			do {
				switch(month) {
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
						num_days_in_month_prev = (std::chrono::year(year).is_leap() ? 29 : 28);
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
						num_days_in_month_cur = (std::chrono::year(year).is_leap() ? 29 : 28);
						num_days_in_month_prev = 31;
						break;
					};
				};

				if(day_of_month < 1) {
					month--;
					day_of_month += num_days_in_month_prev;
				};
				if(day_of_month > num_days_in_month_cur) {
					month++;
					day_of_month -= num_days_in_month_cur;
				};

				if(auto is_invalid = (month < 1); is_invalid || month > 12) {
					year += month / 12 + (is_invalid ? -1 : 0);
					month = (month % 12) + (is_invalid ? 12 : 0);
				};

			}
			while(
				!(
					day_of_month >= 1
					&& day_of_month <= num_days_in_month_cur
					&& month >= 1
					&& month <= 12
					)
				);

			const auto ymd = std::chrono::year_month_day{
				std::chrono::year(year),
				std::chrono::month(month),
				std::chrono::day(day_of_month)
			};

			const auto weekday = std::chrono::weekday{
				std::chrono::floor<std::chrono::days>(std::chrono::sys_days(ymd))
			};

			year_is_leap = ymd.year().is_leap();
			day_of_week = static_cast<int>(weekday.iso_encoding());
			day_of_year = (std::chrono::sys_days(ymd) - std::chrono::sys_days(std::chrono::year(year) / 1 / 1)).count() + 1;
			week_of_year = (day_of_year - day_of_week) / 7 + 1;
		};
	};


	uns::time::date normalize(const uns::time::date& datetime_struct) {
		auto res = datetime_struct;
		res.normalize();
		return res;
	};


	/*
	std::chrono::system_clock::time_point to_time_point(uns::time::date datetime_struct) {
		uns::time::normalize(datetime_struct);

		auto time = static_cast<std::chrono::time_point<std::chrono::local_t, std::chrono::system_clock::duration>>(std::chrono::local_days(ymd));

		time += std::chrono::hours(1) * datetime_struct.hours;
		time += std::chrono::minutes(1) * datetime_struct.minutes;
		time += std::chrono::seconds(1) * datetime_struct.seconds;

		auto subseconds = static_cast<long long int>(static_cast<long double>(std::chrono::seconds(1) / std::chrono::nanoseconds(1)) * datetime_struct.subseconds);
		time += std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::nanoseconds(1) * subseconds);

		return std::chrono::current_zone()->to_sys(time);
	};


	std::chrono::system_clock::time_point timestamp() {
		static const auto ymd = std::chrono::year_month_day{
			std::chrono::year(1970),
			std::chrono::month(1),
			std::chrono::day(1)
		};

		return static_cast<std::chrono::system_clock::time_point>(std::chrono::sys_days(ymd));
	};


	void normalize(uns::time::date& datetime_struct) {
		static const auto subseconds_in_second = 1000000000L;
		static const auto time_fraction_accuracy = 0.01L / static_cast<long double>(subseconds_in_second);

		if(auto is_invalid = (datetime_struct.subseconds < -time_fraction_accuracy); is_invalid || datetime_struct.subseconds > 1.0 - time_fraction_accuracy) {
			auto subseconds = static_cast<long long int>(static_cast<long double>(subseconds_in_second) * datetime_struct.subseconds);
			datetime_struct.seconds += subseconds / subseconds_in_second + (is_invalid ? -1 : 0);
			datetime_struct.subseconds = static_cast<long double>((subseconds % subseconds_in_second) + (is_invalid ? subseconds_in_second : 0)) / static_cast<long double>(subseconds_in_second);
		};

		if(auto is_invalid = (datetime_struct.seconds < 0); is_invalid || datetime_struct.seconds > 59) {
			datetime_struct.minutes += datetime_struct.seconds / 60 + (is_invalid ? -1 : 0);
			datetime_struct.seconds = (datetime_struct.seconds % 60) + (is_invalid ? 60 : 0);
		};

		if(auto is_invalid = (datetime_struct.minutes < 0); is_invalid || datetime_struct.minutes > 59) {
			datetime_struct.hours += datetime_struct.minutes / 60 + (is_invalid ? -1 : 0);
			datetime_struct.minutes = (datetime_struct.minutes % 60) + (is_invalid ? 60 : 0);
		};

		if(auto is_invalid = (datetime_struct.hours < 0); is_invalid || datetime_struct.hours > 23) {
			datetime_struct.day_of_month += datetime_struct.hours / 24 + (is_invalid ? -1 : 0);
			datetime_struct.hours = (datetime_struct.hours % 24) + (is_invalid ? 24 : 0);
		};

		auto num_days_in_month_cur = 30;
		auto num_days_in_month_prev = 30;
		do {
			switch(datetime_struct.month) {
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
					num_days_in_month_prev = (std::chrono::year(datetime_struct.year).is_leap() ? 29 : 28);
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
					num_days_in_month_cur = (std::chrono::year(datetime_struct.year).is_leap() ? 29 : 28);
					num_days_in_month_prev = 31;
					break;
				};
			};

			if(datetime_struct.day_of_month < 1) {
				datetime_struct.month--;
				datetime_struct.day_of_month += num_days_in_month_prev;
			};
			if(datetime_struct.day_of_month > num_days_in_month_cur) {
				datetime_struct.month++;
				datetime_struct.day_of_month -= num_days_in_month_cur;
			};

			if(auto is_invalid = (datetime_struct.month < 1); is_invalid || datetime_struct.month > 12) {
				datetime_struct.year += datetime_struct.month / 12 + (is_invalid ? -1 : 0);
				datetime_struct.month = (datetime_struct.month % 12) + (is_invalid ? 12 : 0);
			};

		}
		while(
			!(
				datetime_struct.day_of_month >= 1
				&& datetime_struct.day_of_month <= num_days_in_month_cur
				&& datetime_struct.month >= 1
				&& datetime_struct.month <= 12
				)
			);

		const auto ymd = std::chrono::year_month_day{
			std::chrono::year(datetime_struct.year),
			std::chrono::month(datetime_struct.month),
			std::chrono::day(datetime_struct.day_of_month)
		};

		const auto weekday = std::chrono::weekday{
			std::chrono::floor<std::chrono::days>(std::chrono::sys_days(ymd))
		};

		datetime_struct.year_is_leap = ymd.year().is_leap();
		datetime_struct.day_of_week = static_cast<int>(weekday.iso_encoding());
		datetime_struct.day_of_year = (std::chrono::sys_days(ymd) - std::chrono::sys_days(std::chrono::year(datetime_struct.year) / 1 / 1)).count() + 1;
		datetime_struct.week_of_year = (datetime_struct.day_of_year - datetime_struct.day_of_week) / 7 + 1;
	};*/
};