#pragma once

#include <exception>
#include <string>
#include <vector>

#include "uniself\strings.h"

//конструируем синтаксическое дерево
namespace uns
{
	
	class parser_exception : public std::exception
	{
	protected:
		std::string message;
		std::string fragment;
		int place = -1;
	public:
		parser_exception() noexcept {};
		parser_exception(const std::string& what, const std::string& where, int on_place = -1) noexcept : message(what), fragment(where), place(on_place) {};
		parser_exception(const parser_exception& set_ex) noexcept : message(set_ex.message), fragment(set_ex.fragment), place(set_ex.place) {};
		parser_exception& operator=(const parser_exception& set_ex) noexcept { message = set_ex.message; fragment = set_ex.fragment; return *this; };
		~parser_exception() noexcept { message = ""; fragment = ""; };
		std::string to_string() noexcept { return message + "::in fragment::" + fragment + (place >= 0 ? std::to_string(place) : ""); };
	};
	

	enum supported_types_mask {
		_none = 0,
		_char = 1 << 1,
		_wchar = 1 << 2,
		_short = 1 << 3,
		_int = 1 << 4,
		_long = 1 << 5,
		_longlong = 1 << 6,
		_ushort = 1 << 7,
		_uint = 1 << 8,
		_ulong = 1 << 9,
		_ulonglong = 1 << 10,
		_float = 1 << 11,
		_double = 1 << 12,
		_ldouble = 1 << 13,
		_bool = 1 << 14,
		_undef = 16383
	};

	class supported_value
	{
	protected:
		union {
			char					ch;
			wchar_t					wch;
			short					sh;
			int						i;
			long					l;
			long long				ll;
			unsigned short			ush;
			unsigned int			ui;
			unsigned long			ul;
			unsigned long long		ull;
			float					fl;
			double					db;
			long double				ldb;
			bool					b;
		} value;
		struct {
			unsigned type : 16;
		};
	public:
		supported_value() : type(0) {};
		bool f() {
			return (type & _float) == _float;
		}
	};


	class basic_operation
	{
	protected:
		std::string name = "";
	public:
		basic_operation() noexcept {};
		basic_operation(const std::string& set_name) noexcept : name(set_name) {};
		basic_operation(const basic_operation& set_operation) noexcept : name(set_operation.name) {};
		basic_operation(basic_operation&& set_operation) noexcept : name(std::move(set_operation.name)) {};
		basic_operation& operator=(const basic_operation& set_operation) noexcept { name = set_operation.name; return *this; };
		basic_operation& operator=(basic_operation&& set_operation) noexcept { name = std::move(set_operation.name); return *this; };
		~basic_operation() noexcept { Delete(); };
		void Delete() { name = ""; };
		std::string Name() const noexcept { return name; };
		void Name(const std::string& set_name) noexcept { name = set_name; };
		bool IsEmpty() const noexcept {};
	};

	enum node_t {
		forward,
		is_meta_identifier,
		is_literal,
		is_instruction,
		is_integer,
		is_character,
		is_commentary,
		is_skipped_symbol,
		weak_concatenation,
		strong_concatenation,
		weak_alternation,
		strong_alternation,
		weak_exceptation,
		strong_exceptation,
		weak_repetition,
		strong_repetition,
		node_t_Total = 16
	};

	node_t StringTonode_t(const std::string& str) noexcept {
		if (str == "is_skipped_symbol")		return is_skipped_symbol;
		if (str == "is_commentary")			return is_commentary;
		if (str == "is_literal")			return is_literal;
		if (str == "is_meta_identifier")	return is_meta_identifier;
		if (str == "is_instruction")		return is_instruction;
		if (str == "weak_concatenation")	return weak_concatenation;
		if (str == "strong_concatenation")	return strong_concatenation;
		if (str == "weak_alternation")		return weak_alternation;
		if (str == "strong_alternation")	return strong_alternation;
		if (str == "is_character")			return is_character;
		if (str == "is_integer")			return is_integer;
		if (str == "weak_repetition")		return weak_repetition;
		if (str == "strong_repetition")		return strong_repetition;
		if (str == "weak_exceptation")		return weak_exceptation;
		if (str == "strong_exceptation")	return strong_exceptation;
		return forward;
	};

	std::string node_tToString(node_t type) noexcept {
		switch (type)
		{
			case uns::is_skipped_symbol: {
				return "is_skipped_symbol";
			};
			case uns::is_commentary: {
				return "is_commentary";
			};
			case uns::is_literal: {
				return "is_literal";
			};
			case uns::is_meta_identifier: {
				return "is_meta_identifier";
			};
			case uns::is_instruction: {
				return "is_instruction";
			};
			case uns::weak_concatenation: {
				return "weak_concatenation";
			};
			case uns::strong_concatenation: {
				return "strong_concatenation";
			};
			case uns::weak_alternation: {
				return "weak_alternation";
			};
			case uns::strong_alternation: {
				return "strong_alternation";
			};
			case uns::is_character: {
				return "is_character";
			};
			case uns::is_integer: {
				return "is_integer";
			};
			case uns::weak_repetition: {
				return "weak_repetition";
			};
			case uns::strong_repetition: {
				return "strong_repetition";
			};
			case uns::weak_exceptation: {
				return "weak_exceptation";
			};
			case uns::strong_exceptation: {
				return "strong_exceptation";
			};
			default:
			case uns::forward: {
				return "forward";
			};
		};
	};


	class node
	{
		friend basic_operation;
		friend std::string to_string(node& tree, const std::string level_symbol, const std::string level_filler, const std::string delimiter, const std::string branch_end_symbol);
		friend node make_syntax_tree(std::string& str, const std::string level_symbol, const std::string level_filler, const std::string delimiter, const std::string branch_end_symbol);
		using id_t = int;

	protected:
		id_t id = -1;
		std::string fragment = "";
		node* up = nullptr;
		node* next = nullptr;
		node* prew = nullptr;
		std::vector<std::unique_ptr<node>> lowers;
		node_t type = forward;
		basic_operation instruction;
		size_t location = 0;
	public:
		node() noexcept { Delete(); };
		explicit node(const std::string& str) noexcept : instruction("") { Fragment(str); };
		node(const node& set_node) noexcept { Set(set_node); };
		node(node&& set_node) noexcept { Set(std::move(set_node)); };
		node& operator=(const node& set_node) noexcept { Set(set_node); return *this; };
		node& operator=(node&& set_node) noexcept { Set(std::move(set_node)); return *this; };
		~node() noexcept { Delete(); };
		void Set(const node& set_node) noexcept {
			Delete(set_node.Fragment());
			type = set_node.type;
			instruction = set_node.instruction;
			id = set_node.id;
			location = set_node.location;
			for (size_t l = 0; l < set_node.lowers.size(); l++) {
				lowers.push_back(std::make_unique<node>(*set_node.lowers[l]));
				Connect(l);
			};
		};
		void Set(node&& set_node) noexcept {
			Delete(std::move(set_node.Fragment()));
			type = set_node.type;
			instruction = std::move(set_node.instruction);
			id = set_node.id;
			location = set_node.location;
			for (size_t l = 0; l < set_node.lowers.size(); l++) {
				lowers.push_back(std::move(set_node.lowers[l]));
				Connect(l);
			};
		};
		void Delete(const std::string& set_fragment = "") noexcept {
			id = -1;
			fragment = set_fragment; 
			up = nullptr; 
			next = nullptr; 
			prew = nullptr; 
			lowers.clear();
			type = forward;
			instruction.Delete();
			location = 0;
		};
		void Delete(std::string&& set_fragment) noexcept {
			id = -1;
			fragment = std::move(set_fragment);
			up = nullptr;
			next = nullptr;
			prew = nullptr;
			lowers.clear();
			type = forward;
			instruction.Delete();
			location = 0;
		};
	protected:
		void Connect(size_t num_of_node) noexcept {
			if (num_of_node < 0 || num_of_node >= Size()) num_of_node = Size() - 1;
			lowers[num_of_node]->up = this;
			if (num_of_node < Size() - 1) {
				lowers[num_of_node]->next = lowers[num_of_node + 1].get();
				lowers[num_of_node + 1]->prew = lowers[num_of_node].get();
			}
			else
				lowers[num_of_node]->next = nullptr;
			if (num_of_node > 0) {
				lowers[num_of_node]->prew = lowers[num_of_node - 1].get();
				lowers[num_of_node - 1]->next = lowers[num_of_node].get();
			}
			else
				lowers[num_of_node]->prew = nullptr;
		};
	public:
		size_t Size() const noexcept { return lowers.size(); };
		void ID(id_t set_id) noexcept { id = set_id; };
		id_t ID() const noexcept { return id; };
		void Location(size_t set_location) noexcept { location = set_location; };
		size_t Location() const noexcept { return location; };
		void Instruction(basic_operation& set_instruction) { instruction = set_instruction; };
		std::string Instruction() const { return instruction.Name(); };
		std::string Fragment() const noexcept { return fragment; };
		void Fragment(std::string set_fragment) noexcept { fragment = set_fragment; };
		void AddNode(node* node_to_place, size_t place, const bool add_a_copy) {
			if (place < 0 || place > Size()) place = Size();
			if (add_a_copy)
				lowers.insert(lowers.begin() + place, std::unique_ptr<node>(new node(*node_to_place)));
			else
				lowers.insert(lowers.begin() + place, std::unique_ptr<node>(node_to_place));
			Connect(place);
		};
		bool DeleteNode(size_t place) {
			if (place < 0 || place >= Size()) place = Size() - 1;
			lowers.erase(lowers.begin() + place);
			if (place >= Size()) place = Size() - 1;
			Connect(place);
		};
		bool Parse(const std::string& str, const size_t seeker_start, size_t& seeker_finish, size_t pos_right_border) const {
			seeker_finish = seeker_start;
			return (fragment == "") || uns::StringSeekerSet(str, seeker_finish, fragment, false, -1, pos_right_border);
		};




	};


	using syntax_tree = node;



	std::string to_string(syntax_tree& tree, const std::string level_symbol, const std::string level_filler, const std::string delimiter, std::string branch_end_symbol) {
		int level_now = 0;
		int level_bef = -1;
		node* cursor = &tree;
		std::string res = "";

		int s = 0;
		while (true) {
			if (level_now >= level_bef) {
				res += std::to_string(level_now) + level_symbol;
				for (s = 0; s < level_now; s++) res += level_filler;
				res += cursor->Fragment();
			};

			if (cursor->lowers.size() > 0 && level_now >= level_bef) {
				res += delimiter;
				//-
				cursor = cursor->lowers[0].get();
				level_bef = level_now++;
			}
			else {
				if (level_now >= level_bef) {
					res += branch_end_symbol;
				};
				if (cursor->next != nullptr) {
					cursor = cursor->next;
					level_bef = level_now;
				}
				else if (cursor->up != nullptr) {
					cursor = cursor->up;
					level_bef = level_now--;
				}
				else break;
			};
		};

		return res;
	};


	syntax_tree make_syntax_tree(std::string& str, const std::string level_symbol, const std::string level_filler, const std::string delimiter, const std::string branch_end_symbol) {
		node root;
		node* cursor_now = nullptr;
		node* cursor_bef = nullptr;

		int level_now = -1;
		int level_bef = -1;
		size_t seeker1 = 0;
		std::string temp1 = "";
		size_t seeker2 = 0;
		std::string temp2 = "";
		std::vector<std::string> ends_of_node;
		ends_of_node.push_back(delimiter);
		ends_of_node.push_back(branch_end_symbol);
		bool need_to_break_cicle = false;

		while (true) {
			cursor_bef = cursor_now;
			level_bef = level_now;

			try {
				if (!StringSeekerRead(str + level_symbol, seeker1, temp1, ends_of_node, false, -1)) break; //конец дерева
			}
			catch (...) {
				throw parser_exception("Syntax tree constructing error::vector memory allocation error #1", "");
			};

			seeker2 = 0;
			if (!StringSeekerRead(temp1, seeker2, temp2, level_symbol, false, -1)) throw parser_exception("Syntax tree string format error::level_symbol(" + level_symbol + ") not found", temp1);
			else {
				try {
					level_now = std::stoi(temp2);
				}
				catch (...) {
					throw parser_exception("Syntax tree string format error::number of level is invalid", temp2 + " in " + temp1);
				};
			};

			if (level_now < 0) throw parser_exception("Syntax tree string format error::number of level is < 0", temp2 + " in " + temp1);

			if (level_filler != "")
				while (StringSeekerSet(temp1, seeker2, level_filler, false, -1)) {};
			temp2 = temp1.substr(seeker2, temp2.length() - seeker2);

			if (level_now > level_bef + 1) throw parser_exception("Syntax tree string error::number of level too high", std::to_string(level_now) + " compare to " + std::to_string(level_bef));

			for (int diff = 0; diff < level_bef - level_now + 1; diff++)
				if (cursor_bef == nullptr || cursor_bef->up == nullptr) {
					throw parser_exception("Syntax tree constructing error::unexpected null-node", std::to_string(cursor_bef == nullptr) + std::to_string(cursor_bef->up == nullptr));
				}
				else {
					cursor_bef = cursor_bef->up;
				};

			if (cursor_bef != nullptr) {
				cursor_now = new node(temp2);
				try {
					cursor_bef->AddNode(cursor_now, -1, false);
				}
				catch (...) {
					throw parser_exception("Syntax tree constructing error::vector memory allocation error #2", "");
				};
			}
			else {
				root.Fragment(temp2);
				cursor_now = &root;
			};

		};


		return root;
	}







};




















//
//
////конструируем синтаксическое дерево
//namespace uns
//{
//
//	class parser_exception : public std::exception
//	{
//	protected:
//		std::string message;
//		std::string fragment;
//	public:
//		parser_exception() noexcept {};
//		parser_exception(const std::string& what, const std::string &where) noexcept : message(what), fragment(where) {};
//		parser_exception(const parser_exception& set_ex) noexcept : message(set_ex.message), fragment(set_ex.fragment) {};
//		parser_exception& operator=(const parser_exception& set_ex) noexcept { message = set_ex.message; fragment = set_ex.fragment; return *this; };
//		~parser_exception() noexcept { message = ""; fragment = ""; };
//		std::string what() noexcept { return message + "::in fragment::" + fragment; };
//	};
//
//
//	enum supported_types_mask {
//		_none = 0,
//		_char = 1 << 1,
//		_wchar = 1 << 2,
//		_short = 1 << 3,
//		_int = 1 << 4,
//		_long = 1 << 5,
//		_longlong = 1 << 6,
//		_ushort = 1 << 7,
//		_uint = 1 << 8,
//		_ulong = 1 << 9,
//		_ulonglong = 1 << 10,
//		_float = 1 << 11,
//		_double = 1 << 12,
//		_ldouble = 1 << 13,
//		_bool = 1 << 14,
//		_undef = 16383
//	};
//
//	class supported_value
//	{
//	protected:
//		union {
//			char					ch;
//			wchar_t					wch;
//			short					sh;
//			int						i;
//			long					l;
//			long long				ll;
//			unsigned short			ush;
//			unsigned int			ui;
//			unsigned long			ul;
//			unsigned long long		ull;
//			float					fl;
//			double					db;
//			long double				ldb;
//			bool					b;
//		} value;
//		struct {
//			unsigned type : 16;
//		};
//	public:
//		supported_value() : type(0) {};
//		bool f() {
//			return (type & _float) == _float;
//		}
//	};
//
//
//	class basic_operation
//	{
//	protected:
//		std::string name = "";
//	public:
//		basic_operation() noexcept {};
//		basic_operation(const std::string& set_name) noexcept : name(set_name) {};
//		basic_operation(const basic_operation& set_operation) noexcept : name(set_operation.name) {};
//		basic_operation(basic_operation&& set_operation) noexcept : name(std::move(set_operation.name)) {};
//		basic_operation& operator=(const basic_operation& set_operation) noexcept { name = set_operation.name; return *this; };
//		basic_operation& operator=(basic_operation&& set_operation) noexcept { name = std::move(set_operation.name); return *this; };
//		~basic_operation() noexcept { name = ""; };
//		std::string Name() const noexcept { return name; };
//		void Name(const std::string& set_name) noexcept { name = set_name; };
//	};
//
//
//	class token
//	{
//		friend basic_operation;
//		friend std::string to_string(token& token_tree, const std::string level_symbol, const std::string level_filler, const std::string delimiter, const std::string branch_end_symbol);
//		friend token make_syntax_tree(std::string& str, const std::string level_symbol, const std::string level_filler, const std::string delimiter, const std::string branch_end_symbol);
//
//	protected:
//		std::string fragment = "";
//		token* up = nullptr;
//		token* next = nullptr;
//		token* prew = nullptr;
//		std::vector<std::unique_ptr<token>> lowers;
//	public:
//		token() noexcept { Delete(); };
//		explicit token(const std::string& str) noexcept { Fragment(str); };
//		token(const token& set_token) noexcept { Set(set_token); };
//		token(token&& set_token) noexcept { Set(std::move(set_token)); };
//		token& operator=(const token& set_token) noexcept { Set(set_token); return *this; };
//		token& operator=(token&& set_token) noexcept { Set(std::move(set_token)); return *this; };
//		~token() noexcept { Delete(); };
//		void Set(const token& set_token) noexcept {
//			Delete(set_token.Fragment());
//			for (size_t l = 0; l < set_token.lowers.size(); l++) {
//				lowers.push_back(std::make_unique<token>(*set_token.lowers[l]));
//				Connect(l);
//			};
//		};
//		void Set(token&& set_token) noexcept {
//			Delete(std::move(set_token.Fragment()));
//			for (size_t l = 0; l < set_token.lowers.size(); l++) {
//				lowers.push_back(std::move(set_token.lowers[l]));
//				Connect(l);
//			};
//		};
//		void Delete(const std::string& set_fragment = "") noexcept { fragment = set_fragment; up = nullptr; next = nullptr; prew = nullptr; lowers.clear(); };
//		void Delete(std::string&& set_fragment) noexcept { fragment = std::move(set_fragment); up = nullptr; next = nullptr; prew = nullptr; lowers.clear(); };
//	protected:
//		void Connect(size_t num_of_node) noexcept {
//			if (num_of_node < 0 || num_of_node >= Size()) num_of_node = Size() - 1;
//			lowers[num_of_node]->up = this;
//			if (num_of_node < Size() - 1) {
//				lowers[num_of_node]->next = lowers[num_of_node + 1].get();
//				lowers[num_of_node + 1]->prew = lowers[num_of_node].get();
//			}
//			else
//				lowers[num_of_node]->next = nullptr;
//			if (num_of_node > 0) {
//				lowers[num_of_node]->prew = lowers[num_of_node - 1].get();
//				lowers[num_of_node - 1]->next = lowers[num_of_node].get();
//			}
//			else
//				lowers[num_of_node]->prew = nullptr;
//		};
//	public:
//		size_t Size() const noexcept { return lowers.size(); };
//		std::string Fragment() const noexcept { return fragment; };
//		void Fragment(std::string set_fragment) noexcept { fragment = set_fragment; };
//		void AddNode(token* token_to_place, size_t place, const bool add_a_copy) {
//			if (place < 0 || place > Size()) place = Size();
//			if (add_a_copy)
//				lowers.insert(lowers.begin() + place, std::unique_ptr<token>(new token(*token_to_place)));
//			else
//				lowers.insert(lowers.begin() + place, std::unique_ptr<token>(token_to_place));
//			Connect(place);
//		};
//		bool DeleteNode(size_t place) {
//			if (place < 0 || place >= Size()) place = Size() - 1;
//			lowers.erase(lowers.begin() + place);
//			if (place >= Size()) place = Size() - 1;
//			Connect(place);
//		};
//		bool Parse(const std::string& str, const size_t seeker_start, size_t& seeker_finish, size_t pos_right_border) const {
//			seeker_finish = seeker_start;
//			return (fragment == "") || uns::StringSeekerSet(str, seeker_finish, fragment, false, -1, pos_right_border);
//		};
//
//	public:
//		class driver
//		{
//		protected:
//			enum { Start, Straight, Reverse } direction = Straight;
//		public:
//			driver() = delete;
//			driver(token& root_token) noexcept { Set(root_token); };
//			driver(const driver& new_driver) noexcept { Set(new_driver); };
//			driver(driver&& new_driver) noexcept { Set(std::move(new_driver)); };
//			driver& operator=(const driver& new_driver) noexcept { Set(new_driver); };
//			driver& operator=(driver&& new_driver) noexcept { Set(std::move(new_driver)); };
//			~driver() noexcept { Delete(); };
//			void Set(token& root_token) noexcept { direction = Start; };
//			void Set(const driver& new_driver) noexcept { direction = new_driver.direction; };
//			void Set(driver&& new_driver) noexcept { direction = new_driver.direction; };
//			void Delete() noexcept { direction = Start; };
//			bool Next(token*& cursor) noexcept {
//				if (direction == Start) {
//					direction = Straight;
//					return true;
//				}
//				else if (cursor->lowers.size() > 0 && direction == Straight) {
//					cursor = cursor->lowers[0].get();
//					direction = Straight;
//					return true;
//				}
//				else if (cursor->next != nullptr) {
//					cursor = cursor->next;
//					direction = Straight;
//					return true;
//				}
//				else if (cursor->up != nullptr) {
//					cursor = cursor->up;
//					direction = Reverse;
//					return true;
//				}
//				else return false;
//			};
//			bool Prew(token*& cursor) noexcept {
//				if (direction == Start) {
//					direction = Straight;
//					return true;
//				}
//				else if (cursor->lowers.size() > 0 && direction == Straight) {
//					cursor = cursor->lowers[cursor->lowers.size() - 1].get();
//					direction = Straight;
//					return true;
//				}
//				else if (cursor->prew != nullptr) {
//					cursor = cursor->prew;
//					direction = Straight;
//					return true;
//				}
//				else if (cursor->up != nullptr) {
//					cursor = cursor->up;
//					direction = Reverse;
//					return true;
//				}
//				else return false;
//			};
//		};
//	};
//
//	std::string to_string(token& token_tree, const std::string level_symbol, const std::string level_filler, const std::string delimiter, std::string branch_end_symbol) {
//		int level_now = 0;
//		int level_bef = -1;
//		token* cursor = &token_tree;
//		std::string res = "";
//
//		int s = 0;
//		while (true) {
//			if (level_now >= level_bef) {
//				res += std::to_string(level_now) + level_symbol;
//				for (s = 0; s < level_now; s++) res += level_filler;
//				res += cursor->Fragment();
//			};
//
//			if (cursor->lowers.size() > 0 && level_now >= level_bef) {
//				res += delimiter;
//				//-
//				cursor = cursor->lowers[0].get();
//				level_bef = level_now++;
//			}
//			else {
//				if (level_now >= level_bef) {
//					res += branch_end_symbol;
//				};
//				if (cursor->next != nullptr) {
//					cursor = cursor->next;
//					level_bef = level_now;
//				}
//				else if (cursor->up != nullptr) {
//					cursor = cursor->up;
//					level_bef = level_now--;
//				}
//				else break;
//			};
//		};
//
//		return res;
//	};
//
//	token make_syntax_tree(std::string& str, const std::string level_symbol, const std::string level_filler, const std::string delimiter, const std::string branch_end_symbol) {
//		token root;
//		token* cursor_now = nullptr;
//		token* cursor_bef = nullptr;
//
//		int level_now = -1;
//		int level_bef = -1;
//		size_t seeker1 = 0;
//		std::string temp1 = "";
//		size_t seeker2 = 0;
//		std::string temp2 = "";
//		std::vector<std::string> ends_of_token;
//		ends_of_token.push_back(delimiter);
//		ends_of_token.push_back(branch_end_symbol);
//		bool need_to_break_cicle = false;
//
//		while (true) {
//			cursor_bef = cursor_now;
//			level_bef = level_now;
//
//			try {
//				if (!StringSeekerRead(str + level_symbol, seeker1, temp1, ends_of_token, false, -1)) break; //конец дерева
//			}
//			catch (...) {
//				throw parser_exception("Syntax tree constructing error::vector memory allocation error #1", "");
//			};
//
//			seeker2 = 0;
//			if (!StringSeekerRead(temp1, seeker2, temp2, level_symbol, false, -1)) throw parser_exception("Syntax tree string format error::level_symbol(" + level_symbol + ") not found", temp1);
//			else {
//				try {
//					level_now = std::stoi(temp2);
//				}
//				catch (...) {
//					throw parser_exception("Syntax tree string format error::number of level is invalid", temp2 + " in " + temp1);
//				};
//			};
//
//			if (level_now < 0) throw parser_exception("Syntax tree string format error::number of level is < 0", temp2 + " in " + temp1);
//
//			if (level_filler != "")
//				while (StringSeekerSet(temp1, seeker2, level_filler, false, -1)) {};
//			temp2 = temp1.substr(seeker2, temp2.length() - seeker2);
//
//			if (level_now > level_bef + 1) throw parser_exception("Syntax tree string error::number of level too high", std::to_string(level_now) + " compare to " + std::to_string(level_bef));
//
//			for (int diff = 0; diff < level_bef - level_now + 1; diff++)
//				if (cursor_bef == nullptr || cursor_bef->up == nullptr) {
//					throw parser_exception("Syntax tree constructing error::unexpected null-token", std::to_string(cursor_bef == nullptr) + std::to_string(cursor_bef->up == nullptr));
//				}
//				else {
//					cursor_bef = cursor_bef->up;
//				};
//
//			if (cursor_bef != nullptr) {
//				cursor_now = new token(temp2);
//				try {
//					cursor_bef->AddNode(cursor_now, -1, false);
//				}
//				catch (...) {
//					throw parser_exception("Syntax tree constructing error::vector memory allocation error #2", "");
//				};
//			}
//			else {
//				root.Fragment(temp2);
//				cursor_now = &root;
//			};
//
//		};
//
//
//		return root;
//	}
//
//
//
//
//
//
//
//};