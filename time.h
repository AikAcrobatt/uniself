#pragma once

#include <chrono>

namespace uns
{
	

	/*класс общего времени
	uns::flow

		uns::flow <-> std::chrono::time_point
		uns::flow <-> std::chrono::duration
		uns::flow <-> long double

	*/


	//класс таймера
	//засекает время создания объекта таймера и по требованию выдает прошедшее с тех пор время
	class timer	{
	protected:
		std::chrono::steady_clock::time_point creation_time;
	public:
		timer() : creation_time(std::chrono::steady_clock::now()) {};
		timer(const timer&)  noexcept = default;
		timer& operator=(const timer&) noexcept = default;
		timer(timer&&) noexcept = default;
		timer& operator=(timer&&)  noexcept = default;
		~timer() noexcept = default;

		template<typename time_period = std::chrono::steady_clock::duration>
		time_period Check() const noexcept {
			return std::chrono::duration_cast<time_period>(std::chrono::steady_clock::now() - creation_time);
		};

		template<class time_period = std::chrono::seconds>
		long double Count() const noexcept {
			using minimal_duration = std::chrono::steady_clock::duration;
			return (static_cast<long double>(Check<minimal_duration>().count()) / static_cast<long double>(time_period(1) / minimal_duration(1)));
		};
	};
	
};
