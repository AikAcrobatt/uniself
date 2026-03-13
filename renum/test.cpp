#include "pch.h"

#include <tuple>

#include "uniself/renum.hpp"

namespace abyss {

    UNS_RENUM(layers, int,
        (l0, = 0),
        (l1, = 1),
        (l2, = 2),
        (l3, = 3),
        (l4, = 4),
        (l5, = 5),
        (l6, = 6),
        (l7, = 7),
        (l8, = 8),
        (l9, = 9)
    );

};

UNS_RENUM(times, int,
    (t1, = 0),
    (t2, = 10),
    (t3, )
);


class EnumToRenumCmp : public ::testing::TestWithParam<
        ::std::tuple<::abyss::layers, ::abyss::layers::enum_type>
    > {};


TEST_P(EnumToRenumCmp, EqualityCheck) {
    EXPECT_TRUE(::std::get<0>(GetParam()) == ::std::get<1>(GetParam()));
    EXPECT_TRUE(::std::get<1>(GetParam()) == ::std::get<0>(GetParam()));
};

INSTANTIATE_TEST_CASE_P(
    RenumGeneral, EnumToRenumCmp, 
    ::testing::Combine(
        ::testing::Values(
            static_cast<::abyss::layers>(::abyss::layers::l0),
            static_cast<::abyss::layers>(::abyss::layers::l1)/*,
            static_cast<::abyss::layers>(::abyss::layers::l2),
            static_cast<::abyss::layers>(::abyss::layers::l3),
            static_cast<::abyss::layers>(::abyss::layers::l4),
            static_cast<::abyss::layers>(::abyss::layers::l5),
            static_cast<::abyss::layers>(::abyss::layers::l6),
            static_cast<::abyss::layers>(::abyss::layers::l7),
            static_cast<::abyss::layers>(::abyss::layers::l8),
            static_cast<::abyss::layers>(::abyss::layers::l9)*/
        ),
        ::testing::Values(
            ::abyss::layers::l0,
            ::abyss::layers::l1/*,
            ::abyss::layers::l2,
            ::abyss::layers::l3,
            ::abyss::layers::l4,
            ::abyss::layers::l5,
            ::abyss::layers::l6,
            ::abyss::layers::l7,
            ::abyss::layers::l8,
            ::abyss::layers::l9*/
        )
    )
);
