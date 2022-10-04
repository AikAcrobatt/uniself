#pragma once

#include "uniself/strings.h"





namespace uns::utils {

#ifndef uns_cmdline_str_t
	using cmdline_str_t = std::u8string;
#endif



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
