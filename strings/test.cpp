

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#pragma warning(default: 4668; default: 4365)

#include "strings.h"

#define BOOST_TEST_MODULE uns::strings::test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE(u8_cast_testing)

    BOOST_AUTO_TEST_SUITE(u8_strings)

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8"ABCD efgh !@#% АБВГдеёжзик_")
                }
            ),
            the_val
        ) {
            //direct testing results are very significant while testing at file ostreams
            BOOST_TEST(the_val == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::string>(the_val))));
            BOOST_TEST(the_val == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::wstring>(the_val))));
            BOOST_TEST(the_val == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::u16string>(the_val))));
            BOOST_TEST(the_val == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<std::u32string>(the_val))));

        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(bools)

        BOOST_DATA_TEST_CASE(std_u8string_to_bool_correct,
            boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8"true"),
                    uns::test::make_u8(u8"TRUE"),
                    uns::test::make_u8(u8"1"),
                    uns::test::make_u8(u8"True"),
                    uns::test::make_u8(u8"false"),
                    uns::test::make_u8(u8"False"),
                    uns::test::make_u8(u8"FALSE"),
                    uns::test::make_u8(u8"0"),
                    uns::test::make_u8(u8"fa lse"),
                    uns::test::make_u8(u8"_false"),
                    uns::test::make_u8(u8" false "),
                    uns::test::make_u8(u8"FalSe"),
                    uns::test::make_u8(u8"fal")
                }
            )
            ^ boost::unit_test::data::make(
                { 
                    true, 
                    true, 
                    true, 
                    true, 
                    false, 
                    false, 
                    false, 
                    false, 
                    false, 
                    false, 
                    false, 
                    false,
                    false
                }
            ),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string::u8_cast<bool>(in_str));
        };

        BOOST_DATA_TEST_CASE(std_u8string_to_bool_incorrect,
            boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8" true "),
                    uns::test::make_u8(u8"TRue"),
                    uns::test::make_u8(u8" 1 ") 
                }
            )
            ^ boost::unit_test::data::make(
                {
                    true,
                    true,
                    true
                }
            ),
            in_str, out_val
        ) {
            BOOST_TEST(out_val != uns::string::u8_cast<bool>(in_str));
        };

        BOOST_DATA_TEST_CASE(bool_to_std_u8string_correct,
            boost::unit_test::data::make(
                { 
                    true,
                    false 
                }
            )
            ^ boost::unit_test::data::make(
                { 
                    uns::test::make_u8(u8"true"),
                    uns::test::make_u8(u8"false")
                }
            ),
            in_val, out_str
        ) {
            BOOST_TEST(out_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(in_val)));
        };

        BOOST_DATA_TEST_CASE(forward_identical_cast,
            boost::unit_test::data::make(
                { 
                    true, 
                    false
                }
            ),
            the_val
        ) {
            BOOST_TEST(the_val == uns::string::u8_cast<bool>(uns::string::u8_cast<std::u8string>(the_val)));
        };

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make(
                { 
                    uns::test::make_u8(u8"true"),
                    uns::test::make_u8(u8"false")
                }
            ),
            the_str
        ) {
            BOOST_TEST(the_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<bool>(the_str))));
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(integers)

        BOOST_AUTO_TEST_SUITE(decimals)

            BOOST_DATA_TEST_CASE(std_u8string_to_dec_correct,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0"),
                        uns::test::make_u8(u8"1"),
                        uns::test::make_u8(u8"10423"),
                        uns::test::make_u8(u8"-88"),
                        uns::test::make_u8(u8"789"),
                        uns::test::make_u8(u8"22304568 "),
                        uns::test::make_u8(u8"-4444 "),
                        uns::test::make_u8(u8"0"),
                        uns::test::make_u8(u8" 0"),
                        uns::test::make_u8(u8"1 "),
                        uns::test::make_u8(u8"10_423"),
                        uns::test::make_u8(u8"8-8"),
                        uns::test::make_u8(u8"7J9"),
                        uns::test::make_u8(u8"2230 4568 "),
                        uns::test::make_u8(u8"-44 44 "),
                        uns::test::make_u8(u8" ")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        0, 
                        1,
                        10423, 
                        -88, 
                        789, 
                        22304568,
                        -4444, 
                        0, 
                        0,
                        1, 
                        10, 
                        8, 
                        7, 
                        2230,
                        -44, 
                        0 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(std_u8string_to_dec_incorrect,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"T1"),
                        uns::test::make_u8(u8"yhi")
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        1, 
                        1 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(dec_to_std_u8string_correct,
                boost::unit_test::data::make(
                    { 
                        0, 
                        1, 
                        10423, 
                        -88, 
                        789, 
                        22304568, 
                        -4444 
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0"),
                        uns::test::make_u8(u8"1"),
                        uns::test::make_u8(u8"10423"),
                        uns::test::make_u8(u8"-88"),
                        uns::test::make_u8(u8"789"),
                        uns::test::make_u8(u8"22304568"),
                        uns::test::make_u8(u8"-4444")
                    }
                ),
                in_val, out_str
            ) {
                BOOST_TEST(out_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(in_val)));
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make(
                    { 
                        0LL, 
                        1LL, 
                        10423LL, 
                        -88LL, 
                        789LL, 
                        22304568LL, 
                        -4444LL 
                    }
                ),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string::u8_cast<long long int>(uns::string::u8_cast<std::u8string>(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0"),
                        uns::test::make_u8(u8"1"),
                        uns::test::make_u8(u8"10423"),
                        uns::test::make_u8(u8"-88"),
                        uns::test::make_u8(u8"789"),
                        uns::test::make_u8(u8"22304568"),
                        uns::test::make_u8(u8"-4444")
                    }
                ),
                the_str
            ) {
                BOOST_TEST(the_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<long long int>(the_str))));
            };

        BOOST_AUTO_TEST_SUITE_END();

        BOOST_AUTO_TEST_SUITE(hexadecimals)

            BOOST_DATA_TEST_CASE(std_u8string_to_hex_correct,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0x3"),
                        uns::test::make_u8(u8"0X12"),
                        uns::test::make_u8(u8"0x1A"),
                        uns::test::make_u8(u8"0x-1B"),
                        uns::test::make_u8(u8"0x1C"),
                        uns::test::make_u8(u8"-0x1D "),
                        uns::test::make_u8(u8"0x1E "),
                        uns::test::make_u8(u8" 0x1F"),
                        uns::test::make_u8(u8"A8"),
                        uns::test::make_u8(u8"C0"),
                        uns::test::make_u8(u8"A5"),
                        uns::test::make_u8(u8"0xA1"),
                        uns::test::make_u8(u8"-FF")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        0x3,     
                        0x12,       
                        0x1A,       
                        -0x1B,       
                        0x1C,
                        -0x1D,
                        0x1E,
                        0x1F,
                        0xA8,
                        0xC0,
                        0xA5,
                        0xA1,
                        -0xFF 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(std_u8string_to_hex_incorrect,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0x 3"),
                        uns::test::make_u8(u8"X12"),
                        uns::test::make_u8(u8"1A"),
                        uns::test::make_u8(u8"0xK")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        3, 
                        18,  
                        26,   
                        27  
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

        BOOST_AUTO_TEST_SUITE(binaries)

            BOOST_DATA_TEST_CASE(std_u8string_to_bin_correct,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0b11"),
                        uns::test::make_u8(u8"0B10010"),
                        uns::test::make_u8(u8"0b-111"),
                        uns::test::make_u8(u8" 0b1000"),
                        uns::test::make_u8(u8" -0b1000")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        0b11, 
                        0b10010,   
                        -0b111,     
                        0b1000,     
                        -0b1000 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(stdstring_to_bin_incorrect,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0b 11"),
                        uns::test::make_u8(u8"B10"),
                        uns::test::make_u8(u8"- 0b1"),
                        uns::test::make_u8(u8"0b2")
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        3,
                        2,    
                        -1,    
                        2 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(floating_points)

        BOOST_AUTO_TEST_SUITE(doubles)

            BOOST_DATA_TEST_CASE(std_u8string_to_dbl_correct,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0.0"),
                        uns::test::make_u8(u8"1.012"),
                        uns::test::make_u8(u8"10.423"),
                        uns::test::make_u8(u8"-8.8"),
                        uns::test::make_u8(u8"7.8900e-10"),
                        uns::test::make_u8(u8"10.01234567890123456"),
                        uns::test::make_u8(u8".7"),
                        uns::test::make_u8(u8"452"),
                        uns::test::make_u8(u8"1744.")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        0.0,
                        1.012,  
                        10.423,  
                        -8.8,  
                        789.0e-12,  
                        10.012345678901234,  
                        0.7,  
                        452.0, 
                        1744.0 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<double>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(std_u8string_to_dbl_incorrect,
                boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"1. 012"),
                        uns::test::make_u8(u8"- 8.8")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        1.012, 
                        -8.8 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val != uns::string::u8_cast<double>(uns::string::u8_cast<std::u8string>(in_str)));
            };
            
            BOOST_DATA_TEST_CASE(dbl_to_std_u8string_correct,
                boost::unit_test::data::make(
                    {  
                        0.0,  
                        1.012, 
                        10.423,   
                        -8.8,  
                        789.0e-12,
                        10.012345678901234, 
                        0.7,  
                        452.0, 
                        1744.0 
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"0"),
                        uns::test::make_u8(u8"1.012"),
                        uns::test::make_u8(u8"10.423"),
                        uns::test::make_u8(u8"-8.8"),
                        uns::test::make_u8(u8"7.89e-10"),
                        uns::test::make_u8(u8"10.012345678901234"),
                        uns::test::make_u8(u8"0.7"),
                        uns::test::make_u8(u8"452"),
                        uns::test::make_u8(u8"1744")
                    }
                ),
                in_val, out_str
            ) {
                BOOST_TEST(out_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(in_val)));
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make(
                    { 
                        0.0, 
                        1.012,
                        10.423, 
                        -8.8, 
                        789.0e-12, 
                        10.012345678901234, 
                        0.7, 
                        452.0, 
                        1744.0
                    }
                ),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string::u8_cast<double>(uns::string::u8_cast<std::u8string>(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0"),
                        uns::test::make_u8(u8"1.012"),
                        uns::test::make_u8(u8"10.423"),
                        uns::test::make_u8(u8"-8.8"),
                        uns::test::make_u8(u8"7.89e-10"),
                        uns::test::make_u8(u8"10.012345678901234"),
                        uns::test::make_u8(u8"0.7"),
                        uns::test::make_u8(u8"452"),
                        uns::test::make_u8(u8"1744")
                    }
                ),
                the_str
            ) {
                BOOST_TEST(the_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(uns::string::u8_cast<double>(the_str))));
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

