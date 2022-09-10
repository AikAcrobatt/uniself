

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#include <exception>
#include <vector>
#pragma warning(default: 4668; default: 4365)

#include "strings.h"

#define BOOST_TEST_MODULE uns_strings_test
#include "boost/test/included/unit_test.hpp"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/mpl/list.hpp>


UNS_BENUM_DECLARATOR(benum_test, int,
    t1 = 0,
    t2 = 10,
    t3
);


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
                    uns::test::make_u8(u8"0")
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
            BOOST_CHECK_THROW(out_val != uns::string::u8_cast<bool>(in_str), std::runtime_error);
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
                        uns::test::make_u8(u8"1 "),
                        uns::test::make_u8(u8"10_423"),
                        uns::test::make_u8(u8"8-8"),
                        uns::test::make_u8(u8"7J9"),
                        uns::test::make_u8(u8"2230 4568 "),
                        uns::test::make_u8(u8"-44 44 ")
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
                        1, 
                        10, 
                        8, 
                        7, 
                        2230,
                        -44
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
                        uns::test::make_u8(u8"yhi"),
                        uns::test::make_u8(u8" ")
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        1, 
                        1,
                        0
                    }
                ),
                in_str, out_val
            ) {
                BOOST_CHECK_THROW(out_val != uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)), std::runtime_error);
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
                        uns::test::make_u8(u8"-FF"),
                        uns::test::make_u8(u8"0x 3"),
                        uns::test::make_u8(u8"0xK")
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
                        -0xFF,
                        0x0,
                        0x0
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(std_u8string_to_hex_incorrect,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"X12")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        18  
                    }
                ),
                in_str, out_val
            ) {
                BOOST_CHECK_THROW(out_val != uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)), std::runtime_error);
            };
            
            BOOST_DATA_TEST_CASE(hex_to_std_u8string_correct,
                boost::unit_test::data::make(
                    {
                        0x3,
                        0x12,
                        0x1A,
                        0x1C,
                        -0x1D,
                        0xE100FA,
                        0xFF,
                        0xA1,
                        -0xFF
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"0x3"),
                        uns::test::make_u8(u8"0x12"),
                        uns::test::make_u8(u8"0x1a"),
                        uns::test::make_u8(u8"0x1c"),
                        uns::test::make_u8(u8"-0x1d"),
                        uns::test::make_u8(u8"0xe100fa"),
                        uns::test::make_u8(u8"0xff"),
                        uns::test::make_u8(u8"0xa1"),
                        uns::test::make_u8(u8"-0xff")
                    }
                ),
                in_val, out_str
            ) {
                BOOST_TEST(out_str == uns::test::make_u8(uns::string::hex_cast(in_val)));
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make(
                    {
                        0x3,
                        0x12,
                        0x1A,
                        0x1C,
                        -0x1D,
                        0xE100FA,
                        0xFF,
                        0xA1,
                        -0xFF
                    }
                ),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string::u8_cast<long long int>(uns::string::hex_cast(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"0x3"),
                        uns::test::make_u8(u8"0x12"),
                        uns::test::make_u8(u8"0x1a"),
                        uns::test::make_u8(u8"0x1c"),
                        uns::test::make_u8(u8"-0x1d"),
                        uns::test::make_u8(u8"0xe100fa"),
                        uns::test::make_u8(u8"0xff"),
                        uns::test::make_u8(u8"0xa1"),
                        uns::test::make_u8(u8"-0xff")
                    }
                ),
                the_str
            ) {
                BOOST_TEST(the_str == uns::test::make_u8(uns::string::hex_cast(uns::string::u8_cast<long long int>(the_str))));
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
                        uns::test::make_u8(u8" -0b1000 "),
                        uns::test::make_u8(u8"- 0b1")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        0b11, 
                        0b10010,   
                        -0b111,     
                        0b1000,     
                        -0b1000,
                        0b1
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(std_u8string_to_bin_incorrect,
                boost::unit_test::data::make(
                    { 
                        uns::test::make_u8(u8"0b 11"),
                        uns::test::make_u8(u8"0b2")
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        3,   
                        2 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_CHECK_THROW(out_val != uns::string::u8_cast<int>(uns::string::u8_cast<std::u8string>(in_str)), std::runtime_error);
            };

            BOOST_DATA_TEST_CASE(bin_to_std_u8string_correct,
                boost::unit_test::data::make(
                    {
                        0b11,
                        0b10010,
                        -0b111,
                        0b1000,
                        -0b1000,
                        0b1
                    }
                )
                ^ boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"0b11"),
                        uns::test::make_u8(u8"0b10010"),
                        uns::test::make_u8(u8"-0b111"),
                        uns::test::make_u8(u8"0b1000"),
                        uns::test::make_u8(u8"-0b1000"),
                        uns::test::make_u8(u8"0b1")
                    }
                ),
                in_val, out_str
            ) {
                BOOST_TEST(out_str == uns::test::make_u8(uns::string::bin_cast(in_val)));
            };

            BOOST_DATA_TEST_CASE(forward_identical_cast,
                boost::unit_test::data::make(
                    {
                        0b11,
                        0b10010,
                        -0b111,
                        0b1000,
                        -0b1000,
                        0b1
                    }
                ),
                the_val
            ) {
                BOOST_TEST(the_val == uns::string::u8_cast<long long int>(uns::string::bin_cast(the_val)));
            };

            BOOST_DATA_TEST_CASE(backward_identical_cast,
                boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"0b11"),
                        uns::test::make_u8(u8"0b10010"),
                        uns::test::make_u8(u8"-0b111"),
                        uns::test::make_u8(u8"0b1000"),
                        uns::test::make_u8(u8"-0b1000"),
                        uns::test::make_u8(u8"0b1")
                    }
                ),
                the_str
            ) {
                BOOST_TEST(the_str == uns::test::make_u8(uns::string::bin_cast(uns::string::u8_cast<long long int>(the_str))));
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
                        uns::test::make_u8(u8"1744."),
                        uns::test::make_u8(u8"12. 54")
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
                        1744.0,
                        12.0
                    }
                ),
                in_str, out_val
            ) {
                BOOST_TEST(out_val == uns::string::u8_cast<double>(uns::string::u8_cast<std::u8string>(in_str)));
            };

            BOOST_DATA_TEST_CASE(std_u8string_to_dbl_incorrect,
                boost::unit_test::data::make(
                    {
                        uns::test::make_u8(u8"O12.77"),
                        uns::test::make_u8(u8"- 8.8")
                    }
                )
                ^ boost::unit_test::data::make(
                    { 
                        12.77, 
                        -8.8 
                    }
                ),
                in_str, out_val
            ) {
                BOOST_CHECK_THROW(out_val != uns::string::u8_cast<double>(uns::string::u8_cast<std::u8string>(in_str)), std::runtime_error);
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

    BOOST_AUTO_TEST_SUITE(benums)

        BOOST_DATA_TEST_CASE(std_u8string_to_benum_correct,
            boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8"t1"),
                    uns::test::make_u8(u8"t2"),
                    uns::test::make_u8(u8"t3")
                }
            )
            ^ boost::unit_test::data::make(
                {
                    static_cast<benum_test>(benum_test::t1),
                    static_cast<benum_test>(benum_test::t2),
                    static_cast<benum_test>(benum_test::t3)
                }
            ),
            in_str, out_val
        ) {
            BOOST_TEST(out_val == uns::string::u8_cast<benum_test>(in_str));
        };

        BOOST_DATA_TEST_CASE(std_u8string_to_benum_incorrect,
            boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8" t1"),
                    uns::test::make_u8(u8"t 2"),
                    uns::test::make_u8(u8"t3 ")
                }
            )
            ^ boost::unit_test::data::make(
                {
                    static_cast<benum_test>(benum_test::t1),
                    static_cast<benum_test>(benum_test::t2),
                    static_cast<benum_test>(benum_test::t3)
                }
            ),
            in_str, out_val
        ) {
            BOOST_CHECK_THROW(out_val == uns::string::u8_cast<benum_test>(in_str), std::runtime_error);
        };

        BOOST_DATA_TEST_CASE(benum_to_std_u8string_correct,
            boost::unit_test::data::make(
                {
                    static_cast<benum_test>(benum_test::t1),
                    static_cast<benum_test>(benum_test::t2),
                    static_cast<benum_test>(benum_test::t3)
                }
            )
            ^ boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8"t1"),
                    uns::test::make_u8(u8"t2"),
                    uns::test::make_u8(u8"t3")
                }
            ),
            in_val, out_str
        ) {
            BOOST_TEST(out_str == uns::test::make_u8(uns::string::u8_cast<std::u8string>(in_val)));
        };

        BOOST_DATA_TEST_CASE(forward_identical_cast,
            boost::unit_test::data::make(
                {
                    static_cast<benum_test>(benum_test::t1),
                    static_cast<benum_test>(benum_test::t2),
                    static_cast<benum_test>(benum_test::t3)
                }
            ),
            the_val
        ) {
            BOOST_TEST(
                the_val == uns::string::u8_cast<benum_test>(
                    uns::string::u8_cast<std::u8string>(the_val)
                )
            );
        };

        BOOST_DATA_TEST_CASE(backward_identical_cast,
            boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8"t1"),
                    uns::test::make_u8(u8"t2"),
                    uns::test::make_u8(u8"t3")
                }
            ),
            the_str
        ) {
            BOOST_TEST(
                the_str == uns::test::make_u8(
                    uns::string::u8_cast<std::u8string>(
                        uns::string::u8_cast<benum_test>(
                            the_str
                        )
                    )
                )
            );
        };

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(string_seeker_methods)
    
    BOOST_AUTO_TEST_SUITE(find)

        BOOST_DATA_TEST_CASE(correct_1,
            boost::unit_test::data::make(
                {
                    0,
                    1,
                    2,
                    3,
                    4,
                    5,
                    6,
                    7,
                    8,
                    9,
                    10,
                    11,
                    12,
                    13,
                    14,
                    15,
                    16
                }
            )
            ^ boost::unit_test::data::make(
                {
                    4,
                    4,
                    4,
                    4,
                    4,
                    10,
                    10,
                    10,
                    10,
                    10,
                    10,
                    17,
                    17,
                    17,
                    17,
                    17,
                    17
                }
            )
            ^ boost::unit_test::data::make(
                {
                    0,
                    0,
                    0,
                    0,
                    0,
                    1,
                    1,
                    1,
                    1,
                    1,
                    1,
                    2,
                    2,
                    2,
                    2,
                    2,
                    2
                }
            ),
            seeker_shift, result_shift, found_sample_shift
        ) {
            auto target = uns::test::make_u8(u8"0123456789ABCГEF");  //a cyrillic symbol at a pos 13
            auto sample_4 = uns::test::make_u8(u8"45");
            auto sample_A = uns::test::make_u8(u8"ABCГ");
            auto samples = std::vector<uns::test::u8string_wrapper>{ sample_4, sample_A };
            auto found_sample = samples.cend();
            /*
            auto seeker_shift = 2;
            auto result_shift = 4;
            auto found_sample_shift = 0;*/

            BOOST_TEST(
                 result_shift == uns::string::find(target, target.cbegin() + seeker_shift, samples, found_sample) - target.cbegin()
            );

            BOOST_TEST(
                 found_sample_shift == found_sample - samples.cbegin()
            );
        };

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();