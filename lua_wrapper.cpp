
#include "uniself/lua_wrapper.hpp"


//class ::uns::lua::error =>
::uns::lua::error::error(
	const ::uns::lua::errcode& code = ::uns::lua::errcode::ok,
	const ::uns::lua::errtype& type = ::uns::lua::errtype::ok,
	const ::std::string text = ""
) noexcept :
	m_text(text),
	m_code(code),
	m_type(type)
{};
::uns::lua::error::error(const ::uns::lua::error& obj) noexcept :
	m_text(obj.m_text),
	m_code(obj.m_code),
	m_type(obj.m_type)
{};
::uns::lua::error& ::uns::lua::error::operator=(const ::uns::lua::error& obj) noexcept {
	if(this != &obj) {
		m_text = obj.m_text;
		m_code = obj.m_code;
		m_type = obj.m_type;
	};
	return *this;
};
::uns::lua::error::error(::uns::lua::error&& obj) noexcept :
	m_text(std::move(obj.m_text)),
	m_code(std::move(obj.m_code)),
	m_type(std::move(obj.m_type))
{};
::uns::lua::error& ::uns::lua::error::operator=(::uns::lua::error&& obj) noexcept {
	if(this != &obj) {
		m_text = obj.m_text;
		m_code = obj.m_code;
		m_type = obj.m_type;
	};
	return *this;
};
::uns::lua::error::~error() noexcept {};

void ::uns::lua::error::swap(::uns::lua::error& obj) noexcept {
	::std::swap(m_text, obj.m_text);
	::std::swap(m_code, obj.m_code);
	::std::swap(m_type, obj.m_type);
};

bool ::uns::lua::error::is() const noexcept { return (m_code != ::uns::lua::errcode::ok) || (m_type != ::uns::lua::errtype::ok); };

::uns::lua::errcode uns::lua::error::code() const noexcept { return m_code; };
::uns::lua::errtype uns::lua::error::type() const noexcept { return m_type; };
::std::string uns::lua::error::text() const noexcept { return m_text; };

::std::u8string uns::lua::error::to_string() const {
	static const auto bracket_op = ::std::u8string{ u8"[" };
	static const auto bracket_cl = ::std::u8string{ u8"] " };
	static const auto comma = ::std::u8string{ u8", " };
	static const auto err_w = ::std::u8string{ u8"err = " };
	static const auto type_w = ::std::u8string{ u8"type = " };

	return bracket_op + err_w + ::uns::string::u8_cast<::std::u8string>(m_code) + comma
		+ type_w + ::uns::string::u8_cast<::std::u8string>(m_type) + bracket_cl
		+ ::uns::string::u8_cast<::std::u8string>(m_text);
};
//<= class ::uns::lua::error


namespace uns::lua::auxiliary {
	class table;

	class state {
	protected:
		lua_State* m_state = nullptr;
		bool m_copied = false;
	public:
		state() noexcept :
			m_state(luaL_newstate()),
			m_copied(false) {
			luaL_openlibs(m_state);
		};
		state(const ::uns::lua::auxiliary::state& obj) noexcept :
			m_state(lua_newthread(obj.m_state)),
			m_copied(true) {};
		::uns::lua::auxiliary::state& operator=(const ::uns::lua::auxiliary::state& obj) noexcept {
			if(this != &obj) {
				reset();
				m_state = lua_newthread(obj.m_state);
				m_copied = true;
			};
			return *this;
		};
		state(::uns::lua::auxiliary::state&& obj) noexcept :
			m_state(obj.m_state),
			m_copied(true) {
			obj.m_state = nullptr;
			obj.m_copied = false;
		};
		::uns::lua::auxiliary::state& operator=(::uns::lua::auxiliary::state&& obj) noexcept {
			if(this != &obj) {
				std::swap(m_state, obj.m_state);
				std::swap(m_copied, obj.m_copied);
			};
			return *this;
		};
		~state() {
			reset();
		};

		const lua_State* get() const noexcept { return m_state; };
		lua_State* get() noexcept { return m_state; };
	protected:
		void reset() noexcept {
			if(m_state != nullptr) {
				if(!m_copied) {
					lua_close(m_state);
				};
			};
		};
	};
};



//class ::uns::lua::type::table methods =>
::uns::lua::type::table::table() noexcept : m_ptr(::std::unique_ptr<::uns::lua::auxiliary::table>{ new ::uns::lua::auxiliary::table{} }) {};
::uns::lua::type::table::table(const ::uns::lua::type::table& obj) noexcept : m_ptr(new ::uns::lua::auxiliary::table{ *obj.m_ptr }) {};
::uns::lua::type::table& ::uns::lua::type::table::operator=(const ::uns::lua::type::table& obj) noexcept {
	if(this != &obj) {
		*m_ptr = *obj.m_ptr;
	};
	return *this;
};
::uns::lua::type::table::table(::uns::lua::type::table&& obj) noexcept : m_ptr(std::move(obj.m_ptr)) {};
::uns::lua::type::table& ::uns::lua::type::table::operator=(::uns::lua::type::table&& obj) noexcept {
	if(this != &obj) {
		m_ptr = std::move(obj.m_ptr);
	};
	return *this;
};
::uns::lua::type::table::~table() noexcept {};

bool ::uns::lua::type::table::operator==(const ::uns::lua::type::table&) const noexcept { return false; };
bool ::uns::lua::type::table::operator!=(const ::uns::lua::type::table&) const noexcept { return true; };

::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) const noexcept { return m_ptr->operator[](key); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::type::string& key) noexcept { return m_ptr->operator[](key); };
::uns::lua::value uns::lua::type::table::operator[] (const char* key) const noexcept { return m_ptr->operator[](::uns::lua::type::string{ key }); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const char* key) noexcept { return m_ptr->operator[](::uns::lua::type::string{ key }); };
::uns::lua::value uns::lua::type::table::operator[] (const ::std::u8string& key) const noexcept { return m_ptr->operator[](::uns::string::u8_cast<::uns::lua::type::string>(key)); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::std::u8string& key) noexcept { return m_ptr->operator[](::uns::string::u8_cast<::uns::lua::type::string>(key)); };
::uns::lua::value uns::lua::type::table::operator[] (const char8_t* key) const noexcept { return this->operator[](::std::u8string{ key }); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const char8_t* key) noexcept { return this->operator[](::std::u8string{ key }); };
::uns::lua::value uns::lua::type::table::operator[] (const ::uns::lua::value& key) const noexcept { return m_ptr->operator[](key); };
::uns::lua::value& ::uns::lua::type::table::operator[] (const ::uns::lua::value& key) noexcept { return m_ptr->operator[](key); };

::std::size_t uns::lua::type::table::size() const noexcept { return m_ptr->size(); };
//<= definitions of ::uns::lua::type::table methods