#pragma once

#include <exception>
#include <stdexcept>

#include "uniself/algorithms.h"

namespace uns::exceptions {

	class basic : public std::exception {
	protected:
		std::string msg = "";
	public:
		basic() noexcept {};
		basic(const std::string& message) noexcept : msg(message) {};

		virtual const char* what() const override {
			if (msg.empty()) return std::exception::what();
			return msg.c_str();
		};
	};
};

#define UNS_EXCEPTION_DECLARE(issue_name, exception_parent_full_name)                       \
class issue_name : public exception_parent_full_name {									\
public:                                                                                 \
	using base_t = typename exception_parent_full_name;									\
																						\
	issue_name() noexcept {};                                                           \
	issue_name(const std::string& message) noexcept : base_t(message) {};				\
};