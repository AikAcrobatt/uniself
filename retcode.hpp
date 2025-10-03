#pragma once

#ifndef UNS_LIB_RETCODE
#define UNS_LIB_RETCODE "retcode.hpp"

#include <iostream>
#include <vector>
#include <tuple>

#include "uniself/concepts.hpp"

namespace uns {

	using retcode = long long int;


	class response {
	protected:
		::uns::retcode m_retcode = 0;
		::std::u8string m_message = u8"";
	public:
		response(const ::uns::retcode& Retcode, const ::std::u8string& Message = u8"") noexcept;

		void swap(::uns::response& obj) noexcept;

		virtual ::uns::retcode retcode() const noexcept;
		virtual ::std::u8string message() const noexcept;

		virtual bool ok() const noexcept;
	};


	namespace filters {

		class basic {
		public:
			inline virtual bool match(const ::uns::response& SingleResponse) const noexcept {
				return false;
			};
		};

	};


	class responses {
	protected:
		::std::vector<::uns::response> m_responses;
		bool m_isok = true;
	public:
		class const_iterator {
		protected:
			::std::vector<::uns::response>::const_iterator m_obj;
			::std::vector<::uns::response>::const_iterator m_end;
		public:
			bool operator==(const ::uns::responses::const_iterator&) const noexcept;
			bool operator!=(const ::uns::responses::const_iterator&) const noexcept;
			::uns::responses::const_iterator operator++() noexcept;
			const ::uns::response& operator*() const;
			const ::uns::response& operator->() const;
		};
	public:
		class iterator {
		protected:
			::std::vector<::uns::response>::iterator m_obj;
			::std::vector<::uns::response>::iterator m_end;
		public:
			bool operator==(const ::uns::responses::iterator&) const noexcept;
			bool operator!=(const ::uns::responses::iterator&) const noexcept;
			::uns::responses::iterator operator++() noexcept;
			::uns::response& operator*();
			::uns::response& operator->();
		};
	public:
		bool ok() const noexcept;

		void operator+=(const ::uns::responses& resps) noexcept;
		void operator+=(const ::uns::response& resp) noexcept;

		const ::uns::responses::const_iterator cbegin() const noexcept;
		const ::uns::responses::const_iterator cend() const noexcept;
		::uns::responses::const_iterator begin() const noexcept;
		::uns::responses::const_iterator end() const noexcept;
		::uns::responses::iterator begin() noexcept;
		::uns::responses::iterator end() noexcept;

		::std::size_t size() const noexcept;
		const ::uns::response& operator[](::std::size_t idx) const noexcept;
		::uns::response& operator[](::std::size_t idx) noexcept;

		bool contains(const ::std::shared_ptr<::uns::filters::basic>& Filter) const noexcept;
		template<::uns::const_iterable_collection<::std::shared_ptr<::uns::filters::basic>> collection_t>
		bool contains(const collection_t& Filters) const noexcept {
			for(const auto& response : m_responses) {
				for(const auto& filter : Filters) {
					if(filter->match(response)) return true;
				};
			};

			return false;
		};
		template<::std::derived_from<::uns::filters::basic> filter_t>
		bool contains() const noexcept {
			for(const auto& response : m_responses) {
				if(filter_t::match(response)) return true;
			};

			return false;
		};
	};


	namespace filters {


		template<typename first_subfilter_t, typename ... subfilters_tt>
		class recursive;
		template<typename first_subfilter_t, typename ... subfilters_tt>
		class recursive : public ::uns::filters::recursive<subfilters_tt ...> {
		public:
			template<bool (*compose_f)(bool, bool) noexcept>
			static bool match(const ::uns::response& SingleResponse) noexcept {
				return compose_f(
					first_subfilter_t::match(SingleResponse)
					, ::uns::filters::recursive<subfilters_tt ...>::match<compose_f>(SingleResponse)
				);
			};
		};
		template<typename first_subfilter_t>
		class recursive<first_subfilter_t> {
		public:
			template<bool (*compose_f)(bool, bool)>
			static bool match(const ::uns::response& SingleResponse) noexcept {
				return first_subfilter_t::match(SingleResponse);
			};
		};


		template<typename ... subfilters_tt>
		class all_and : public ::uns::filters::basic {
		protected:
			static bool operand(bool arg1, bool arg2) noexcept { return arg1 && arg2; };
		public:
			static bool match(const ::uns::response& SingleResponse) noexcept {
				return ::uns::filters::recursive<subfilters_tt ...>::match<operand>(SingleResponse);
			};
			virtual bool match(const ::uns::response& SingleResponse) const noexcept override {
				return ::uns::filters::recursive<subfilters_tt ...>::match<operand>(SingleResponse);
			};
		};


		template<typename ... subfilters_tt>
		class all_or : public ::uns::filters::basic {
		protected:
			static bool operand(bool arg1, bool arg2) { return arg1 || arg2; };
		public:
			static bool match(const ::uns::response& SingleResponse) {
				return ::uns::filters::recursive<subfilters_tt ...>::match<operand>(SingleResponse);
			};
			virtual bool match(const ::uns::response& SingleResponse) const noexcept override {
				return ::uns::filters::recursive<subfilters_tt ...>::match<operand>(SingleResponse);
			};
		};


		template<typename first_subfilter_t>
		class is_not : public ::uns::filters::basic {
		protected:
			static bool operand(bool arg1, bool arg2) { return false; };
		public:
			static bool match(const ::uns::response& SingleResponse) {//TODO a conflict between static and non-static method
				return first_subfilter_t::match<operand>(SingleResponse);
			};
			virtual bool match(const ::uns::response& SingleResponse) const noexcept override {
				return first_subfilter_t::match<operand>(SingleResponse);
			};
		};


		namespace retcode {


			template<::uns::retcode RetcodeValue>
			class more : public ::uns::filters::basic {
			public:
				static bool match(const ::uns::response& SingleResponse) {
					return first_subfilter_t::match<operand>(SingleResponse);
				};
				virtual bool match(const ::uns::response& SingleResponse) const noexcept override {
					return first_subfilter_t::match<operand>(SingleResponse);
				};
			};


		}

	};

};
#endif
