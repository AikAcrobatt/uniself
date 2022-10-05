#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>

#include "uniself/strings.h"





namespace uns::utils {

#ifndef uns_cmdline_str_t
	using cmdline_str_t = std::u8string;
#endif

	class cmd_parser {
	public:
		using string_type = std::string;
		using iterable_contigious_container = std::vector<string_type>;
		
		constexpr iterable_contigious_container key_prefixes() {
			return { string_type{} };
		};

		constexpr iterable_contigious_container key_value_delimiters() {
			return { string_type{} };
		};
	};

	class cmdline {
	public:
		using string_type = std::string;
		using cmd_parser_type = cmd_parser;
	protected:
		std::vector<string_type> keys;
		std::unordered_map<string_type, string_type> values;
		cmd_parser_type parser;
	public:
		cmdline(int argc, char* argv[]) {
			auto entry = string_type{};
			auto key = string_type{};
			auto val = string_type{};

			for(int arg_idx = 0; arg_idx < argc; arg_idx++) {
				entry = string_type{ argv[arg_idx] };

				if(
					auto seeker = entry.begin();
					uns::string::seeker_set(entry, seeker, parser.key_prefixes(), false, -1)
				) {
					if(
						uns::string::seeker_read(entry, seeker, key, parser.key_value_delimiters(), false, -1)
					) {

					};
				}
				else {
					key = entry;
				};

				keys.push_back(key);
			};
		};

		cmdline() = default;
		cmdline(const cmdline&) = default;
		cmdline& operator=(const cmdline&) = default;
		cmdline(cmdline&&) = default;
		cmdline& operator=(cmdline&&) = default;
		~cmdline() = default;

	protected:
		void entry_unpaired_add(const string_type& entry) {

		};


	};



	int main();

};


/*
to create a template cmdline class
requirements:
1) should be able to contain both entries of pairs key-value and also simple string values
2) should be modifyable by proper entry-prefix (single or plural)
3) should be template-parametrized by output string type
4) should contain a special member containing a filesystem addres of app
5) should contain an info about total number of entries
6) should contain info about relative order of entries in command line
7) should be able to get access to values by keys for at least entries of pairs key-value


*/

int main(int argc, char* argv[]) {


	return uns::utils::main();
};
