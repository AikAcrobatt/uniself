#pragma once

#include "uniself/algorithms.h"
#include "uniself/time.h"
#include "uniself/mathematics.h"
#include "uniself/strings.h"

namespace uns {


	namespace ex_ex {

		using string_t = std::wstring;


		//структура для хранения информации об ошибке
		struct error {
			int type = 0;
			int spec = 0;
			double secstart = 0;
			error(int type_, int spec_, double secstart_) noexcept : type(type_), spec(spec_), secstart(secstart_) {};
		};


		//псевдоним для буфера ошибок исполняеых выражений
		using error_buffer_t = std::vector<error>;


		//базовый интерфейс для всех правил всех языков исполняемых выражений
		class base_rule {
		protected:
			virtual void SomeFunc() = 0;
		public:
			using input_data_t = double;
			using output_data_t = double;

		};


		/*class example_rule : public base_rule {
		protected:
			virtual void SomeFunc() noexcept override {};

		};*/


		//заглушечный класс для синтаксического дерева
		template<class ex_ex_rule_t>
		class dummy_class {
		public:
			dummy_class() {};
			dummy_class(const string_t& text, error_buffer_t& buffer) {};
			
		};


		//класс исполняемых выражений
		template<class ex_ex_rule_t>
		class executable_expression {
		protected:

			template<typename rule_t>
			using syntax_tree_t = dummy_class<rule_t>; //Смысл в том, что реальное синтаксическое дерево должно быть написано подобно dumy_class'у
			using ex_ex_input_data_t = typename ex_ex_rule_t::input_data_t;
			using ex_ex_output_data_t = typename ex_ex_rule_t::output_data_t;

		protected:
			string_t text;
			size_t num_of_inputs = 0;
			size_t num_of_outputs = 0;
			syntax_tree_t<ex_ex_rule_t> syntax_tree;
			static const int _execution_error_type = 101000001;
		public:
			executable_expression() : syntax_tree() { text = string_t(); };
			executable_expression(const string_t& text_to_compile, error_buffer_t& buffer) {
				text = text_to_compile;
				Compile(text_to_compile, buffer);
			};
			executable_expression(const executable_expression& ex_ex) {
				this = ex_ex;
			};
			executable_expression& operator=(const executable_expression& ex_ex) {
				text = ex_ex.text;
				syntax_tree = ex_ex.syntax_tree;
				num_of_inputs = ex_ex.num_of_inputs;
				num_of_outputs = ex_ex.num_of_outputs;
				return (*this);
			};
			executable_expression(executable_expression&& ex_ex) {
				this = std::move(ex_ex);
			};
			executable_expression& operator=(executable_expression&& ex_ex) {
				text = std::move(ex_ex.text);
				syntax_tree = std::move(ex_ex.syntax_tree);
				num_of_inputs = std::move(ex_ex.num_of_inputs);
				num_of_outputs = std::move(ex_ex.num_of_outputs);
				return (*this);
			};
			~executable_expression() { text = string_t(); };
			string_t Text() const noexcept { return text; };
		protected:
			bool Compile(const string_t& text_to_compile, error_buffer_t& buffer) {
				syntax_tree = syntax_tree_t<ex_ex_rule_t>(text_to_compile, buffer);

				//заглушечная реализация {{
				num_of_inputs = 1;
				num_of_outputs = 1;
				//}}

				return true;
			};
		public:
			bool Execute(
				const std::vector<ex_ex_input_data_t>& inputs,
				std::vector<ex_ex_output_data_t>& outputs,
				error_buffer_t& buffer
			) {
				
				if (inputs.size() < num_of_inputs) {
					buffer.push_back(error(_execution_error_type, __LINE__, uns::from_start::Count()));
					return false;
				}
				else {
					int to_add = num_of_outputs - outputs.size();
					for (int i = 0; i < to_add; i++)
						outputs.push_back(uns::unified_cast<ex_ex_output_data_t>(NULL));

					//заглушечная реализация {{
					outputs[0] = std::get<0>(inputs[0]);
					//}}
				};

				return true;
			};
			size_t Inputs() const noexcept { return num_of_inputs; };
			size_t Outputs() const noexcept { return num_of_outputs; };
			template<typename check_t>
			bool Check(
				const std::vector<check_t>& inputs,
				std::vector<check_t>& outputs,
				error_buffer_t& buffer
			) {
				if (inputs.size() < num_of_inputs) {
					buffer.push_back(error(_execution_error_type, __LINE__, uns::from_start::Count()));
					return false;
				}
				else {
					int to_add = num_of_outputs - outputs.size();
					for (int i = 0; i < to_add; i++)
						outputs.push_back(uns::unified_cast<check_t>(NULL));

					//заглушечная реализация {{
					outputs[0] = inputs[0];
					//}}
				};

				return true;
			};

		};

	}
};