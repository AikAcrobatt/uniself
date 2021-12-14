#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace uns::__service_algorihms::string {

	//функция установки искателя на заданное место в строке, относительно искомого символа
	//	возвращает true если искомый символ найден и при этом удалось поставить искатель в нужную позицию
	//	в результате искатель может быть установлен позже правой границы!
	template<typename string_t>
	bool SeekerSet(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		typename string_t::size_type							pos_sample_beg,								//положение начала строки-искомого символа в рабочей строке
		typename string_t::size_type							pos_sample_end,								//положение конца строки-искомого символа в рабочей строке
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		typename string_t::size_type							pos_right_border_beg = string_t::npos,		//положение начального символа правой границы
		typename string_t::size_type							pos_right_border_end = string_t::npos		//положение конечного символа правой границы
	) noexcept {
		using signed_size_t = int;

		signed_size_t new_seeker_position = 0;

		if (pos_sample_beg == string_t::npos) { return false; };
		if (pos_right_border_beg != string_t::npos && pos_sample_beg > pos_right_border_beg) return false;

		if (!from_begin) {
			if (pos_sample_end == string_t::npos || pos_sample_beg > pos_sample_end) pos_sample_end = pos_sample_beg;
			position_in_sample = static_cast<signed_size_t>(pos_sample_end - pos_sample_beg) - position_in_sample;
		};
		new_seeker_position = static_cast<signed_size_t>(pos_sample_beg) + position_in_sample;

		if (new_seeker_position >= 0 && (pos_right_border_end == string_t::npos || (new_seeker_position <= pos_right_border_end))) {
			seeker = static_cast<size_t>(new_seeker_position);
			return true;
		}
		else return false;
	};

};


namespace uns::string {

	//функция поиска любой из вектора подстрок-образцов в рабочей строке, начиная с положения искателя
	template<class string_t>
	typename string_t::size_type SeekerFind(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type							seeker,										//положение искателя, от которого ищем
		const std::vector<string_t>&							samples,									//вектор искомых строк-образцов
		typename std::vector<string_t>::size_type&				found_sample								//индекс найденного образца в векторе образцов (если не найден, то = samples.size())
	) noexcept {
		typename string_t::size_type found_sample_position = str.length();
		typename string_t::size_type actual_sample_position = found_sample_position;
		typename std::vector<string_t>::size_type actual_sample_number = 0;
		found_sample = samples.size();

		for (auto sample : samples) {
			if (!sample.empty()) {
				actual_sample_position = str.find(sample, seeker);
				if (actual_sample_position != string_t::npos && actual_sample_position < found_sample_position) {
					found_sample_position = actual_sample_position;
					found_sample = actual_sample_number;
				};
			};
			actual_sample_number++;
		};
		if (found_sample_position == str.length()) found_sample_position = string_t::npos;

		return found_sample_position;
	};
	template<class string_t>
	typename string_t::size_type SeekerFind(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type							seeker,										//положение искателя, от которого ищем
		const std::vector<string_t>&							samples										//вектор искомых строк-образцов
	) noexcept {
		typename std::vector<string_t>::size_type found_sample;

		return uns::string::SeekerFind<string_t>(str, seeker, samples, found_sample);
	};
	template<class string_t>
	typename string_t::size_type SeekerFind(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type							seeker,										//положение искателя, от которого ищем
		const string_t&											sample,										//искомыя строка
		size_t&													found_sample								//заглушечная переменная для шаблонов, всегда принимает значение длины рабочей строки
	) noexcept {
		found_sample = 0;
		return str.find(sample, seeker);
	};
	template<class string_t>
	typename string_t::size_type SeekerFind(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type							seeker,										//положение искателя, от которого ищем
		const string_t&											sample										//искомыя строка
	) noexcept {
		return str.find(sample, seeker);
	};


	//функция установки искателя на заданное место в строке, относительно искомого символа
	//	возвращает true если искомый символ найден и при этом удалось поставить искатель в нужную позицию
	template<typename string_t>
	bool SeekerSet(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		const string_t&											sample,										//искомая строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		typename string_t::size_type							seeking_right_limit = string_t::npos,		//положение крайнего символа поиска
		typename string_t::size_type							seeker_right_limit = string_t::npos			//положение крайнего символа перемещения искателя
	) noexcept {
		typename string_t::size_type pos_sample = uns::string::SeekerFind<string_t>(str, seeker, sample);
		if (!(pos_sample >= seeker && pos_sample < str.length())) return false;

		return uns::__service_algorihms::string::SeekerSet<string_t>(str, seeker, pos_sample, pos_sample + sample.length() - 1, from_begin, position_in_sample, seeking_right_limit, seeker_right_limit);
	};
	template<typename string_t>
	bool SeekerSet(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		const std::vector<string_t>&							samples,									//коллекция искомых строк
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		typename string_t::size_type							seeking_right_limit = string_t::npos,		//положение крайнего символа поиска
		typename string_t::size_type							seeker_right_limit = string_t::npos			//положение крайнего символа перемещения искателя
	) noexcept {
		typename std::vector<string_t>::size_type sample_number;

		typename string_t::size_type pos_sample = uns::string::SeekerFind<string_t>(str, seeker, samples, sample_number);
		if (!(pos_sample >= seeker && pos_sample < str.length())) return false;
		if (!(sample_number >= 0 && sample_number < samples.size())) return false;

		return uns::__service_algorihms::string::SeekerSet<string_t>(str, seeker, pos_sample, pos_sample + samples[sample_number].length() - 1, from_begin, position_in_sample, seeking_right_limit, seeker_right_limit);
	};
	template<typename string_t, class sample_t>
	bool SeekerSet(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		const sample_t&											sample,										//искомыя строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		const string_t&											right_border								//крайний символ поиска
	) noexcept {
		typename string_t::size_type pos_right_border = uns::string::SeekerFind<string_t>(str, seeker, right_border);

		return uns::string::SeekerSet<string_t>(str, seeker, sample, from_begin, position_in_sample, pos_right_border, (pos_right_border >= 0 && pos_right_border < str.length()) ? (pos_right_border + right_border.length() - 1) : string_t::npos);
	};
	template<typename string_t, class sample_t>
	bool SeekerSet(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		const sample_t&											sample,										//искомыя строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		const std::vector<string_t>&							right_borders								//коллекция крайних символов поиска
	) noexcept {
		typename std::vector<string_t>::size_type right_border_number;

		typename string_t::size_type pos_right_border = uns::string::SeekerFind<string_t>(str, seeker, right_borders, right_border_number);

		return uns::string::SeekerSet<string_t>(str, seeker, sample, from_begin, position_in_sample, pos_right_border, ((pos_right_border >= 0 && pos_right_border < str.length()) && (right_border_number >= 0 && right_border_number < right_borders.size())) ? (pos_right_border + right_borders[right_border_number].length() - 1) : string_t::npos);
	};


	//функция чтения заданной строки искателем от текущего положения искателя до символа-разделителя
	template<typename string_t>
	bool SeekerRead(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		string_t&												res,										//строка-результат чтения
		const string_t&											delimiter,									//строка-разделитель в рабочей строке, до начального символа которого читается рабочая строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-разделителе
		int														position_in_delimiter,						//положение искателя в строке-разделителе, на которое слудеут установить искатель после прочтения
		typename string_t::size_type							seeking_right_limit = string_t::npos,		//положение крайнего символа поиска
		typename string_t::size_type							seeker_right_limit = string_t::npos			//положение крайнего символа перемещения искателя
	) noexcept {
		
		res = string_t();

		typename string_t::size_type pos_delimiter = uns::string::SeekerFind<string_t>(str, seeker, delimiter);
		if (!(pos_delimiter > seeker && pos_delimiter < str.length())) return false;

		res = str.substr(seeker, pos_delimiter - seeker);

		return uns::__service_algorihms::string::SeekerSet<string_t>(str, seeker, pos_delimiter, pos_delimiter + delimiter.length() - 1, from_begin, position_in_delimiter, seeking_right_limit, seeker_right_limit);
	};
	template<typename string_t>
	bool SeekerRead(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		string_t&												res,										//строка-результат чтения
		const std::vector<string_t>&							delimiters,									//строка-разделитель в рабочей строке, до начального символа которого читается рабочая строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-разделителе
		int														position_in_delimiter,						//положение искателя в строке-разделителе, на которое слудеут установить искатель после прочтения
		typename string_t::size_type							seeking_right_limit = string_t::npos,		//положение крайнего символа поиска
		typename string_t::size_type							seeker_right_limit = string_t::npos			//положение крайнего символа перемещения искателя
	) noexcept {
		typename std::vector<string_t>::size_type delimiter_number;

		res = string_t();

		typename string_t::size_type pos_delimiter = uns::string::SeekerFind<string_t>(str, seeker, delimiters, delimiter_number);
		if (!(pos_delimiter > seeker && pos_delimiter < str.length())) return false;

		res = str.substr(seeker, pos_delimiter - seeker);

		return uns::__service_algorihms::string::SeekerSet<string_t>(str, seeker, pos_delimiter, pos_delimiter + delimiters[delimiter_number].length() - 1, from_begin, position_in_delimiter, seeking_right_limit, seeker_right_limit);
	};
	template<typename string_t, class delimiter_t>
	bool SeekerRead(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		string_t&												res,										//строка-результат чтения
		const delimiter_t&										delimiter,									//строка-разделитель в рабочей строке, до начального символа которого читается рабочая строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-разделителе
		int														position_in_delimiter,						//положение искателя в строке-разделителе, на которое слудеут установить искатель после прочтения
		const string_t&											right_border								//строка, позже начала которой читать не следует
	) noexcept {
		typename string_t::size_type pos_right_border = uns::string::SeekerFind<string_t>(str, seeker, right_border);

		return uns::string::SeekerRead<string_t>(str, seeker, res, delimiter, from_begin, position_in_delimiter, pos_right_border, (pos_right_border >= 0 && pos_right_border < str.length()) ? (pos_right_border + right_border.length() - 1) : string_t::npos);
	};
	template<typename string_t, class delimiter_t>
	bool SeekerRead(
		const string_t&											str,										//рабочая строка
		typename string_t::size_type&							seeker,										//искатель символа
		string_t&												res,										//строка-результат чтения
		const delimiter_t&										delimiter,									//строка-разделитель в рабочей строке, до начального символа которого читается рабочая строка
		bool													from_begin,									//флаг, указывающй порядок отсчета положения искателя в строке-разделителе
		int														position_in_delimiter,						//положение искателя в строке-разделителе, на которое слудеут установить искатель после прочтения
		const std::vector<string_t>&							right_borders								//коллекция крайних символов поиска
	) noexcept {
		typename std::vector<string_t>::size_type right_border_number;

		typename string_t::size_type pos_right_border = uns::string::SeekerFind<string_t>(str, seeker, right_borders, right_border_number);

		return uns::string::SeekerRead<string_t>(str, seeker, res, delimiter, from_begin, position_in_delimiter, pos_right_border, ((pos_right_border >= 0 && pos_right_border < str.length()) && (right_border_number >= 0 && right_border_number < right_borders.size() - 1)) ? (pos_right_border + right_borders[right_border_number].length() - 1) : string_t::npos);
	};


	//функция чтения из заданной строки участка от заданного образца до символа-разделителя
	template<typename string_t, class sample_t, class delimiter_t>
	bool ReadBySample(
		const string_t&											str,										//рабочая строка
		const sample_t&											sample,										//строка-искомый символ в рабочей строке
		bool													from_begin_of_sample,						//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		string_t&												res,										//строка-результат чтения
		const delimiter_t&										delimiter,									//строка-разделитель символ в рабочей строке, до начального символа которого читается рабочая строка
		typename string_t::size_type							pos_right_border = string_t::npos			//положение крайнего символа, позже которого читать не следует
	) noexcept {
		typename string_t::size_type seeker = 0;

		res = string_t();

		if (!uns::string::SeekerSet<string_t>(str, seeker, sample, from_begin_of_sample, position_in_sample, pos_right_border, string_t::npos)) return false;
		return uns::string::SeekerRead<string_t>(str, seeker, res, delimiter, true, 0, pos_right_border, string_t::npos);
	};
	template<typename string_t, class sample_t, class delimiter_t, class right_border_t>
	bool ReadBySample(
		const string_t&											str,										//рабочая строка
		const sample_t&											sample,										//строка-искомый символ в рабочей строке
		bool													from_begin_of_sample,						//флаг, указывающй порядок отсчета положения искателя в строке-искомом символе
		int														position_in_sample,							//положение искателя в строке-искомом символе, на которое следует установить искатель
		string_t&												res,										//строка-результат чтения
		const delimiter_t&										delimiter,									//строка-разделитель символ в рабочей строке, до начального символа которого читается рабочая строка
		const right_border_t&									right_border								//строка, позже начала которой читать не следует
	) noexcept {
		typename string_t::size_type seeker = 0;
		typename string_t::size_type pos_right_border = uns::string::SeekerFind<string_t>(str, seeker, right_border);

		res = string_t();

		if (!uns::string::SeekerSet<string_t>(str, seeker, sample, from_begin_of_sample, position_in_sample, pos_right_border, string_t::npos)) return false;
		return uns::string::SeekerRead<string_t>(str, seeker, res, delimiter, true, 0, pos_right_border, string_t::npos);
	};


	template<class out_string_t, class in_string_t>
	out_string_t UpperCase(const in_string_t& str) {
		out_string_t res = out_string_t();

		for (auto letter : str)
			res += static_cast<typename out_string_t::value_type>(toupper(letter));

		return res;
	};


	template<class out_string_t, class in_string_t>
	out_string_t LowerCase(const in_string_t& str) {
		out_string_t res = out_string_t();

		for (auto letter : str)
			res += static_cast<typename out_string_t::value_type>(tolower(letter));

		return res;
	};


	template<class out_string_t, class in_string_t, class delimiter_t>
	std::tuple<out_string_t, out_string_t> Split(const in_string_t& str, const delimiter_t& delimiter) {
		typename in_string_t::size_type seeker = 0;

		if (!uns::string::SeekerSet<in_string_t>(str, seeker, delimiter, true, 0)) return std::make_tuple<out_string_t, out_string_t>(out_string_t(), out_string_t());
			
		return std::make_tuple<out_string_t, out_string_t>(str.substr(0, seeker), str.substr(seeker + delimiter.length()));
	};
};


