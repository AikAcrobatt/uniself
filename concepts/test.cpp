
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <array>

#include "uniself/concepts.hpp"

#include "gtest/gtest.h"


namespace uns::tests::concepts::string {

    template<typename basic_string_candidate_t>
    static constexpr bool is_basic() noexcept {
        return ::std::same_as<basic_string_candidate_t, ::std::string>
            || ::std::same_as<basic_string_candidate_t, ::std::wstring>
            || ::std::same_as<basic_string_candidate_t, ::std::u8string>
            || ::std::same_as<basic_string_candidate_t, ::std::u16string>
            || ::std::same_as<basic_string_candidate_t, ::std::u32string>;
    };

    template<::uns::is_basic_string candidate_t>
    static constexpr bool test() noexcept {
        return true;
    };

    template<typename candidate_t>
    static constexpr bool test() noexcept {
        return false;
    };

    using types_list = ::testing::Types<
        void
        , int
        , char
        , ::std::string
        , char[]
        , ::std::wstring
        , ::std::u8string
        , ::std::u16string
        , ::std::u32string
    >;
};

template<typename basic_string_candidate_t>
class ConceptStringCheck : public ::testing::Test {};

TYPED_TEST_CASE(ConceptStringCheck, ::uns::tests::concepts::string::types_list);
TYPED_TEST(ConceptStringCheck, IsBasicString) {
    if (::uns::tests::concepts::string::is_basic<TypeParam>()) {
        ASSERT_TRUE(::uns::tests::concepts::string::test<TypeParam>());
    }
    else {
        ASSERT_FALSE(::uns::tests::concepts::string::test<TypeParam>());
    };
};


namespace uns::tests::concepts::collections {

    class collection_element_type {
    private:
        ::std::unique_ptr<void> m_noncopyable = nullptr;
    };


    template<::uns::const_iterable_collection<::uns::tests::concepts::collections::collection_element_type> collection_t>
    bool test_const_iter() {
        return true;
    };
    template<typename not_collection_t>
    bool test_const_iter() {
        return false;
    };

    template<::uns::iterable_collection<::uns::tests::concepts::collections::collection_element_type> collection_t>
    bool test_iter() {
        return true;
    };
    template<typename not_collection_t>
    bool test_iter() {
        return false;
    };


    template<typename elem_t>
    class nonconst_vector : public ::std::vector<elem_t> {
    protected:
        typename ::std::vector<elem_t>::const_iterator cbegin() const noexcept { return ::std::vector<elem_t>::cbegin(); };
        typename ::std::vector<elem_t>::const_iterator cend() const noexcept { return ::std::vector<elem_t>::cend(); };
    public:
        typename ::std::vector<elem_t>::iterator begin()  noexcept { return ::std::vector<elem_t>::begin(); };
        typename ::std::vector<elem_t>::iterator end()  noexcept { return ::std::vector<elem_t>::end(); };
        ::std::size_t size() const noexcept { return ::std::vector<elem_t>::size(); };
    };


    template<typename elem_t>
    class const_vector : public ::std::vector<elem_t> {
    public:
        typename ::std::vector<elem_t>::const_iterator cbegin() const noexcept { return ::std::vector<elem_t>::cbegin(); };
        typename ::std::vector<elem_t>::const_iterator cend() const noexcept { return ::std::vector<elem_t>::cend(); };
    public:
        typename ::std::vector<elem_t>::iterator begin() noexcept { return ::std::vector<elem_t>::begin(); };
        typename ::std::vector<elem_t>::iterator end() noexcept { return ::std::vector<elem_t>::end(); };
        ::std::size_t size() const noexcept { return ::std::vector<elem_t>::size(); };
    };

};


namespace uns::tests::concepts::collections {

    using const_types_list = ::testing::Types<
        ::std::vector<::uns::tests::concepts::collections::collection_element_type>
        , ::std::list<::uns::tests::concepts::collections::collection_element_type>
        , ::std::set<::uns::tests::concepts::collections::collection_element_type>
        , ::std::array<::uns::tests::concepts::collections::collection_element_type, 137>
        , ::uns::tests::concepts::collections::const_vector<::uns::tests::concepts::collections::collection_element_type>
    >;

};

template<typename collection_candidate_t>
class ConceptCollectionConstCorrect : public ::testing::Test {};

TYPED_TEST_CASE(ConceptCollectionConstCorrect, ::uns::tests::concepts::collections::const_types_list);
TYPED_TEST(ConceptCollectionConstCorrect, Test) {
    ASSERT_TRUE(::uns::tests::concepts::collections::test_const_iter<TypeParam>());
};


namespace uns::tests::concepts::collections {

    using fake_const_types_list = ::testing::Types<
        int
        , double
        , bool
        , ::std::tuple<
            ::std::vector<::uns::tests::concepts::collections::collection_element_type>
            , ::std::list<::uns::tests::concepts::collections::collection_element_type>
        >
        , ::std::map< //it is a collection of std::pair elements NOT collection_element_type
            ::std::string
            , ::uns::tests::concepts::collections::collection_element_type
        >
        , ::std::string //it is a collection of char elements NOT collection_element_type
        , ::uns::tests::concepts::collections::nonconst_vector<::uns::tests::concepts::collections::collection_element_type>
        , ::uns::tests::concepts::collections::collection_element_type
    >;

};

template<typename collection_candidate_t>
class ConceptCollectionConstInCorrect : public ::testing::Test {};

TYPED_TEST_CASE(ConceptCollectionConstInCorrect, ::uns::tests::concepts::collections::fake_const_types_list);
TYPED_TEST(ConceptCollectionConstInCorrect, Test) {
    ASSERT_FALSE(::uns::tests::concepts::collections::test_const_iter<TypeParam>());
};


namespace uns::tests::concepts::collections {

    using nonconst_types_list = ::testing::Types<
        ::std::vector<::uns::tests::concepts::collections::collection_element_type>
        , ::std::list<::uns::tests::concepts::collections::collection_element_type>
        , ::std::set<::uns::tests::concepts::collections::collection_element_type>
        , ::std::array<::uns::tests::concepts::collections::collection_element_type, 137>
        , ::uns::tests::concepts::collections::const_vector<::uns::tests::concepts::collections::collection_element_type>
        , ::uns::tests::concepts::collections::nonconst_vector<::uns::tests::concepts::collections::collection_element_type>
    >;

};

template<typename collection_candidate_t>
class ConceptCollectionCorrect : public ::testing::Test {};

TYPED_TEST_CASE(ConceptCollectionCorrect, ::uns::tests::concepts::collections::nonconst_types_list);
TYPED_TEST(ConceptCollectionCorrect, Test) {
    ASSERT_TRUE(::uns::tests::concepts::collections::test_iter<TypeParam>());
};


namespace uns::tests::concepts::collections {

    using fake_nonconst_types_list = ::testing::Types<
        int
        , double
        , bool
        , ::std::tuple<
            ::std::vector<::uns::tests::concepts::collections::collection_element_type>
            , ::std::list<::uns::tests::concepts::collections::collection_element_type>
        >
        , ::std::map< //it is a collection of std::pair elements NOT collection_element_type
            ::std::string
            , ::uns::tests::concepts::collections::collection_element_type
        >
        , ::std::string //it is a collection of char elements NOT collection_element_type
        , ::uns::tests::concepts::collections::collection_element_type
    >;

};

template<typename collection_candidate_t>
class ConceptCollectionInCorrect : public ::testing::Test {};

TYPED_TEST_CASE(ConceptCollectionInCorrect, ::uns::tests::concepts::collections::fake_nonconst_types_list);
TYPED_TEST(ConceptCollectionInCorrect, Test) {
    ASSERT_FALSE(::uns::tests::concepts::collections::test_iter<TypeParam>());
};
