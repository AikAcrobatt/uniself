
#include "uniself/retcode.hpp"

::uns::response::response(const ::uns::retcode& Retcode, const ::std::u8string& Message = u8"") noexcept :
    m_retcode{ Retcode },
    m_message{ Message }
{};

void ::uns::response::swap(::uns::response& obj) noexcept {
    ::std::swap(m_retcode, obj.m_retcode);
    ::std::swap(m_message, obj.m_message);
};

::uns::retcode uns::response::retcode() const noexcept { return m_retcode; };
::std::u8string uns::response::message() const noexcept { return m_message; };

bool ::uns::filter::basic::match(const ::uns::response& SingleResponse) const noexcept { return false; };


bool ::uns::responses::ok() const noexcept {
    return m_isok || (m_responses.size() == 0);
};


bool ::uns::responses::contains(const ::std::shared_ptr<::uns::filter::basic>& Filter) const noexcept {
	for(const auto& response : m_responses) {
		if(Filter->match(response)) return true;
	};

	return false;
};