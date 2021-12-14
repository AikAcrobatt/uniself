#pragma once

#include <fstream>

namespace uns {

	namespace errors {

		namespace __service_algorihms {
			std::ofstream errf;
		};


		bool Settle(const std::wstring& error_file_name) {
			if (uns::errors::__service_algorihms::errf.is_open()) return true;

			uns::errors::__service_algorihms::errf.open(error_file_name, std::ios::binary | std::ios::app);

			return !uns::errors::__service_algorihms::errf.bad();
		};


		void Fix(const std::string& signature, const std::string& error, const std::string& message) {
			if (error == "" && message == "") return;
			uns::errors::__service_algorihms::errf << signature << "::" << error << "[" << message << "]" << std::endl;
		};

	};
};