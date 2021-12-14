#pragma once

#include <chrono>

namespace uns
{
	
	using flow = long double;
	using flow_time_period = std::chrono::seconds;

	//функции, позвол€ющие получить врем€, прошедшее с момента запуска машины, в разных форматах
	namespace from_start
	{
		template<class time_period>
		time_period Duration() {
			return (std::chrono::duration_cast<time_period>(std::chrono::steady_clock::now().time_since_epoch()));
		};

		template<class time_period = flow_time_period, typename number_t = flow>
		number_t Count() {
			return (static_cast<number_t>(std::chrono::steady_clock::now().time_since_epoch().count()) / static_cast<number_t>(time_period(1) / std::chrono::steady_clock::duration(1)));
		};
	};


	//класс таймера
	//засекает врем€ создани€ объекта таймера и по требованию выдает прошедшее с тех пор врем€
	class timer	{
	protected:
		std::chrono::steady_clock::time_point creation_time;
	public:
		timer() : creation_time(std::chrono::steady_clock::now()) {};
		timer(const timer& copying_obj) : creation_time(copying_obj.creation_time) {};
		timer& operator=(const timer& copying_obj) { creation_time = copying_obj.creation_time; return *this; };
		timer(timer&& moving_obj) noexcept : creation_time(std::move(moving_obj.creation_time)) {};
		timer& operator=(timer&& moving_obj) noexcept { creation_time = std::move(moving_obj.creation_time); return *this; };
		~timer() {};

		template<typename time_period = std::chrono::milliseconds>
		time_period Check() {
			return std::chrono::duration_cast<time_period>(std::chrono::steady_clock::now() - creation_time);
		};

		template<class time_period = flow_time_period, typename number_t = flow>
		number_t Count() {
			return (static_cast<number_t>(Check<std::chrono::steady_clock::duration>().count()) / static_cast<number_t>(time_period(1) / std::chrono::steady_clock::duration(1)));
		};
	};
	
};
