

#pragma warning(disable: 4668; disable: 4365)
#include <iostream>
#include <exception>
#include <vector>
#include <list>
#pragma warning(default: 4668; default: 4365)

#include "uniself/strings.h"

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
            boost::unit_test::data::xrange(17)
            ^ (boost::unit_test::data::make(std::vector<int>(5, 4)) + std::vector<int>(6, 10) + std::vector<int>(6, 17))
            ^ (boost::unit_test::data::make(std::vector<int>(5, 0)) + std::vector<int>(6, 1) + std::vector<int>(6, 3)),
            seeker_shift, result_shift, found_sample_shift
        ) {
            auto target = uns::test::make_u8(u8"0123456789ABCГEF");  //a cyrillic symbol at a pos 13
            auto samples = std::vector<uns::test::u8string_wrapper>{
                uns::test::make_u8(u8"45"), 
                uns::test::make_u8(u8"ABCГ"),
                uns::test::make_u8(u8"46Г")     //there is no such a sample in the target string at all
            };
            auto found_sample = samples.cend();

            BOOST_TEST(
                 result_shift == uns::string::find(target, target.cbegin() + seeker_shift, samples, found_sample) - target.cbegin()
            );

            BOOST_TEST(
                 found_sample_shift == found_sample - samples.cbegin()
            );
        };

        BOOST_DATA_TEST_CASE(incorrect_0,
            boost::unit_test::data::xrange(17),
            seeker_shift
        ) {
            auto target = uns::test::make_u8(u8"0123456789ABCГEF");  //a cyrillic symbol at a pos 13
            auto samples = std::vector<uns::test::u8string_wrapper>{ 
                uns::test::make_u8(u8"Should not be found"),
                uns::test::make_u8(u8"This too")
            };
            auto found_sample = samples.cend();

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find(target, target.cbegin() + seeker_shift, samples, found_sample) - target.cbegin()
            );

            BOOST_TEST(
                static_cast<int>(samples.cend() - samples.cbegin()) == found_sample - samples.cbegin()
            );
        };

        BOOST_DATA_TEST_CASE(incorrect_1,
            boost::unit_test::data::xrange(17),
            seeker_shift
        ) {
            auto target = uns::test::make_u8(u8"0123456789ABCГEF");  //a cyrillic symbol at a pos 13
            auto samples = std::vector<uns::test::u8string_wrapper>{ uns::test::make_u8(u8"") };
            auto found_sample = samples.cend();

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find(target, target.cbegin() + seeker_shift, samples, found_sample) - target.cbegin()
            );

            BOOST_TEST(
                static_cast<int>(samples.cend() - samples.cbegin()) == found_sample - samples.cbegin()
            );
        };

        BOOST_AUTO_TEST_CASE(incorrect_2) {
            auto target = uns::test::make_u8(u8"");  //a cyrillic symbol at a pos 13
            auto samples = std::vector<uns::test::u8string_wrapper>{
                uns::test::make_u8(u8"45"),
                uns::test::make_u8(u8"ABCГ"),
                uns::test::make_u8(u8"46Г")
            };
            auto found_sample = samples.cend();

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find<std::u8string>(target, target.cbegin(), samples, found_sample) - target.cbegin()
            );

            BOOST_TEST(
                static_cast<int>(samples.cend() - samples.cbegin()) == found_sample - samples.cbegin()
            );
        };

        BOOST_AUTO_TEST_CASE(incorrect_3) {
            auto target = uns::test::make_u8(u8"");
            auto samples = std::vector<uns::test::u8string_wrapper>{
                uns::test::make_u8(u8"")
            };
            auto found_sample = samples.cend();

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find<std::u8string>(target, target.cbegin(), samples, found_sample) - target.cbegin()
            );

            BOOST_TEST(
                static_cast<int>(samples.cend() - samples.cbegin()) == found_sample - samples.cbegin()
            );
        };

        BOOST_DATA_TEST_CASE(correct_2,
            boost::unit_test::data::xrange(48)
            ^ (boost::unit_test::data::make(std::vector<int>(22, 21)) + boost::unit_test::data::make(std::vector<int>(26, 49))),
            seeker_shift, result_shift
        ) {
            auto target = uns::test::make_u8(u8"Это большая русская строка");
            auto sample = uns::test::make_u8(u8" ру");

            BOOST_TEST(
                result_shift == uns::string::find(target, target.cbegin() + seeker_shift, sample) - target.cbegin()
            );
        };

        BOOST_DATA_TEST_CASE(incorrect_4,
            boost::unit_test::data::xrange(48),
            seeker_shift
        ) {
            auto target = uns::test::make_u8(u8"Это большая русская строка");
            auto sample = uns::test::make_u8(u8"нет такой буквы в этом слове!");

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find(target, target.cbegin() + seeker_shift, sample) - target.cbegin()
            );
        };

        BOOST_DATA_TEST_CASE(incorrect_5,
            boost::unit_test::data::xrange(48),
            seeker_shift
        ) {
            auto target = uns::test::make_u8(u8"Это большая русская строка");
            auto sample = uns::test::make_u8(u8"");

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find(target, target.cbegin() + seeker_shift, sample) - target.cbegin()
            );
        };

        BOOST_AUTO_TEST_CASE(incorrect_6) {
            auto target = std::u8string{ u8"" };
            auto sample = std::u8string{ u8" ру" };

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find<std::u8string>(target, target.cbegin(), sample) - target.cbegin()
            );
        };

        BOOST_AUTO_TEST_CASE(incorrect_7) {
            auto target = uns::test::make_u8(u8"");
            auto sample = uns::test::make_u8(u8"");

            BOOST_TEST(
                static_cast<int>(target.cend() - target.cbegin()) == uns::string::find<std::u8string>(target, target.cbegin(), sample) - target.cbegin()
            );
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(seeker_set)

        BOOST_DATA_TEST_CASE(correct_1,
            boost::unit_test::data::xrange(100, ( boost::unit_test::data::step = 10 ))
            * boost::unit_test::data::xrange(100, ( boost::unit_test::data::step = 5 ))
            * boost::unit_test::data::xrange(100, ( boost::unit_test::data::step = 5 )),
            seeker_shift, pos_rborder_beg, pos_rborder_end
        ) {
            auto target = std::u8string(u8"start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");
            auto samples = std::vector<uns::test::u8string_wrapper>{
                uns::test::make_u8(u8"some_key1"),
                uns::test::make_u8(u8"некий_ключ2"),
                uns::test::make_u8(u8"some_key3")
            };

            auto pos_of_some_key1 = uns::string::find(target, target.begin(), u8"some_key1");
            auto pos_of_some_key2 = uns::string::find(target, target.begin(), u8"некий_ключ2");
            auto pos_of_some_key3 = uns::string::find(target, target.begin(), u8"some_key3");

            auto seeker = target.cbegin() + seeker_shift;

            auto seeking_result1 =
                (target.cbegin() + seeker_shift <= pos_of_some_key1)
                && (pos_of_some_key1 + samples[0].size() + 3 <= target.cbegin() + pos_rborder_end)
                && (pos_of_some_key1 <= target.cbegin() + pos_rborder_beg);
            auto seeking_result2 = 
                (target.cbegin() + seeker_shift > pos_of_some_key1)
                && (target.cbegin() + seeker_shift <= pos_of_some_key2)
                && (pos_of_some_key2 + samples[1].size() + 3 <= target.cbegin() + pos_rborder_end)
                && (pos_of_some_key2 <= target.cbegin() + pos_rborder_beg);
            auto seeking_result3 =
                (target.cbegin() + seeker_shift > pos_of_some_key1)
                && (target.cbegin() + seeker_shift > pos_of_some_key2)
                && (target.cbegin() + seeker_shift <= pos_of_some_key3)
                && (pos_of_some_key3 + samples[2].size() + 3 <= target.cbegin() + pos_rborder_end)
                && (pos_of_some_key3 <= target.cbegin() + pos_rborder_beg);


            BOOST_TEST(
                (seeking_result1 || seeking_result2 || seeking_result3) == uns::string::seeker_set(target, seeker, samples, false, -4, target.begin() + pos_rborder_beg, target.begin() + pos_rborder_end)
            );

            BOOST_TEST(
                (
                    seeking_result1
                    ? (pos_of_some_key1 + samples[0].size() + 3) - target.begin()
                    : (
                        seeking_result2
                        ? (pos_of_some_key2 + samples[1].size() + 3) - target.begin()
                        : (
                            seeking_result3
                            ? (pos_of_some_key3 + samples[2].size() + 3) - target.begin()
                            : seeker_shift
                        )
                    )
                ) == seeker - target.begin()
            );
        };

        BOOST_DATA_TEST_CASE(correct_2,
            boost::unit_test::data::xrange(100, (boost::unit_test::data::step = 10))
            * boost::unit_test::data::make(
                {
                    uns::test::make_u8(u8"some_key1"),
                    uns::test::make_u8(u8"некий_ключ2"),
                    uns::test::make_u8(u8"some_key3"),
                    uns::test::make_u8(u8"unpresented_key4")
                }
            )
            * boost::unit_test::data::xrange(100, (boost::unit_test::data::step = 5))
            * boost::unit_test::data::xrange(100, (boost::unit_test::data::step = 5)),
            seeker_shift, sample, pos_rborder_beg, pos_rborder_end
        ) {
            auto target = std::u8string(u8"start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");

            auto pos_of_sample = uns::string::find(target, target.begin(), sample);

            auto seeker = target.cbegin() + seeker_shift;

            auto seeking_result =
                (target.cbegin() + seeker_shift <= pos_of_sample)
                && (pos_of_sample <= target.cbegin() + pos_rborder_beg)
                && (pos_of_sample + sample.size() + 3 <= target.cbegin() + pos_rborder_end);

            BOOST_TEST(
                seeking_result == uns::string::seeker_set(target, seeker, sample, false, -4, target.begin() + pos_rborder_beg, target.begin() + pos_rborder_end)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? (pos_of_sample + sample.size() + 3) - target.begin()
                    : seeker_shift
                ) == seeker - target.begin()
            );
        };

        BOOST_AUTO_TEST_CASE(correct_3) {
            auto target = std::string("start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");
            auto samples = std::list<std::string>{
                "some_key1",
                "некий_ключ2",
                "some_key3"
            };

            auto pos_of_sample1 = uns::string::find(target, target.begin(), static_cast<std::string>("some_key1"));
            auto seeker = target.begin();

            BOOST_TEST(
                true == uns::string::seeker_set(target, seeker, samples, true, 3)
            );

            BOOST_TEST(
                (pos_of_sample1 - target.begin() + 3) == seeker - target.begin()
            );
        };

        BOOST_AUTO_TEST_CASE(correct_4) {
            auto target = std::wstring(L"start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");

            auto pos_of_sample1 = uns::string::find(target, target.begin(), static_cast<std::wstring>(L"some_key1"));
            auto seeker = target.begin();

            BOOST_TEST(
                true == uns::string::seeker_set(target, seeker, L"some_key1", true, 3)
            );

            BOOST_TEST(
                (pos_of_sample1 - target.begin() + 3) == seeker - target.begin()
            );
        };

        BOOST_AUTO_TEST_CASE(correct_5) {
            auto target = std::string("start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");
            auto samples = std::list<std::string>{
                "some_key1",
                "некий_ключ2",
                "some_key3"
            };
            auto borders = std::list<std::string>{
                "some_val1",
                "некое_значение2",
                "some_val3"
            };

            auto pos_of_sample = uns::string::find(target, target.begin(), samples);
            auto seeker = target.begin();

            BOOST_TEST(
                true == uns::string::seeker_set(target, seeker, samples, true, 3, borders)
            );

            BOOST_TEST(
                (pos_of_sample - target.begin() + 3) == seeker - target.begin()
            );
        };

        BOOST_AUTO_TEST_CASE(correct_6) {
            auto target = std::string("start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");
            auto sample = std::string("некий_ключ2");
            auto borders = std::list<std::string>{
                "некое_значение2",
                "some_val3"
            };

            auto pos_of_sample = uns::string::find(target, target.begin(), sample);
            auto seeker = target.begin();

            BOOST_TEST(
                true == uns::string::seeker_set(target, seeker, sample, true, 3, borders)
            );

            BOOST_TEST(
                (pos_of_sample - target.begin() + 3) == seeker - target.begin()
            );
        };

        BOOST_AUTO_TEST_CASE(correct_7) {
            auto target = std::string("start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");
            auto samples = std::list<std::string>{
                "some_key1",
                "некий_ключ2",
                "some_key3"
            };
            auto border = std::string{ "some_val1" };

            auto pos_of_sample = uns::string::find(target, target.begin(), samples);
            auto seeker = target.begin();

            BOOST_TEST(
                true == uns::string::seeker_set(target, seeker, samples, true, 3, border)
            );

            BOOST_TEST(
                (pos_of_sample - target.begin() + 3) == seeker - target.begin()
            );
        };

        BOOST_AUTO_TEST_CASE(correct_8) {
            auto target = std::string("start:some_key1 = some_val1; некий_ключ2 = некое_значение2; some_key3 = some_val3; xvx");
            auto sample = std::string("некий_ключ2");
            auto border = std::string{ "некое_значение2" };

            auto pos_of_sample = uns::string::find(target, target.begin(), sample);
            auto seeker = target.begin();

            BOOST_TEST(
                true == uns::string::seeker_set(target, seeker, sample, true, 3, border)
            );

            BOOST_TEST(
                (pos_of_sample - target.begin() + 3) == seeker - target.begin()
            );
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(seeker_read)

        BOOST_DATA_TEST_CASE(correct_1,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 5))
            * boost::unit_test::data::xrange(35, (boost::unit_test::data::begin = 20, boost::unit_test::data::step = 1))
            * boost::unit_test::data::xrange(35, (boost::unit_test::data::begin = 20, boost::unit_test::data::step = 1)),
            seeker_shift, pos_rborder_beg, pos_rborder_end
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiters = std::vector<std::string>{
                ";x;",
                ";y;",
                ";z;"
            };

            auto seeker = target.cbegin() + seeker_shift;

            auto delimiter_found = delimiters.end();
            auto pos_delim = uns::string::find(target, seeker, delimiters, delimiter_found);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (target.cbegin() + pos_rborder_beg >= pos_delim)
                && (pos_delim - target.cbegin() <= pos_rborder_end - delimiter_found->size())
                && (pos_delim > seeker_shift + target.cbegin());

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiters, false, -1, target.begin() + pos_rborder_beg, target.begin() + pos_rborder_end)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? (pos_delim + delimiter_found->size()) - target.begin()
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

        BOOST_DATA_TEST_CASE(correct_2,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 5))
            * boost::unit_test::data::xrange(35, (boost::unit_test::data::begin = 20, boost::unit_test::data::step = 1))
            * boost::unit_test::data::xrange(35, (boost::unit_test::data::begin = 20, boost::unit_test::data::step = 1)),
            seeker_shift, pos_rborder_beg, pos_rborder_end
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiter = std::string(";y;");

            auto seeker = target.cbegin() + seeker_shift;

            auto pos_delim = uns::string::find(target, seeker, delimiter);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (target.cbegin() + pos_rborder_beg >= pos_delim)
                && (pos_delim - target.cbegin() <= pos_rborder_end - delimiter.size())
                && (pos_delim > seeker_shift + target.cbegin());

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiter, false, -1, target.begin() + pos_rborder_beg, target.begin() + pos_rborder_end)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? ((pos_delim + delimiter.size()) - target.begin())
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

        BOOST_DATA_TEST_CASE(correct_3,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 1)),
            seeker_shift
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiters = std::vector<std::string>{
                ";x;",
                ";y;",
                ";z;"
            };

            auto seeker = target.cbegin() + seeker_shift;

            auto delimiter_found = delimiters.end();
            auto pos_delim = uns::string::find(target, seeker, delimiters, delimiter_found);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (pos_delim != target.end())
                && (pos_delim - target.begin() > seeker_shift);

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiters, true, 0)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? pos_delim - target.begin()
                    : seeker_shift
                    ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

        BOOST_DATA_TEST_CASE(correct_4,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 1)),
            seeker_shift
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiter = std::string(";y;");

            auto seeker = target.cbegin() + seeker_shift;

            auto pos_delim = uns::string::find(target, seeker, delimiter);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (pos_delim != target.end())
                && (pos_delim - target.begin() > seeker_shift);

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiter, true, 1)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? (pos_delim - target.begin() + 1)
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };
        
        BOOST_DATA_TEST_CASE(correct_5,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 1)),
            seeker_shift
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiters = std::vector<std::string>{
                ";x;",
                ";y;",
                ";z;"
            };
            auto rborders = std::list<std::string>{
                ";y;",
                "al2"
            };

            auto seeker = target.cbegin() + seeker_shift;

            auto delimiter_found = delimiters.end();
            auto pos_delim = uns::string::find(target, seeker, delimiters, delimiter_found);
            auto rborder_found = rborders.end();
            auto pos_rborder = uns::string::find(target, seeker, rborders, rborder_found);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (pos_delim != target.end())
                && (pos_delim - target.begin() > seeker_shift)
                && (pos_rborder - target.begin() > seeker_shift)
                && (pos_rborder >= pos_delim);

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiters, true, 0, rborders)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? pos_delim - target.begin()
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

        BOOST_DATA_TEST_CASE(correct_6,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 1)),
            seeker_shift
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiter = std::string(";y;");
            auto rborders = std::list<std::string>{
                ";y;",
                "al2"
            };

            auto seeker = target.cbegin() + seeker_shift;

            auto pos_delim = uns::string::find(target, seeker, delimiter);
            auto rborder_found = rborders.end();
            auto pos_rborder = uns::string::find(target, seeker, rborders, rborder_found);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (pos_delim != target.end())
                && (pos_delim - target.begin() > seeker_shift)
                && (pos_rborder >= pos_delim);

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiter, true, 1, rborders)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? (pos_delim - target.begin() + 1)
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

        BOOST_DATA_TEST_CASE(correct_7,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 1)),
            seeker_shift
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiters = std::vector<std::string>{
                ";x;",
                ";y;",
                ";z;"
            };
            auto rborder = std::string{ "al2" };

            auto seeker = target.cbegin() + seeker_shift;

            auto delimiter_found = delimiters.end();
            auto pos_delim = uns::string::find(target, seeker, delimiters, delimiter_found);
            auto pos_rborder = uns::string::find(target, seeker, rborder);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (pos_delim != target.end())
                && (pos_delim - target.begin() > seeker_shift)
                && (pos_rborder - target.begin() > seeker_shift)
                && (pos_rborder >= pos_delim);

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiters, true, 0, rborder)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? pos_delim - target.begin()
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

        BOOST_DATA_TEST_CASE(correct_8,
            boost::unit_test::data::xrange(35, (boost::unit_test::data::step = 1)),
            seeker_shift
        ) {
            auto target = std::string("key1=val1;x;key2=val2;y;key3=val3;z;");
            auto delimiter = std::string(";y;");
            auto rborder = std::list<std::string>{"key3"};

            auto seeker = target.cbegin() + seeker_shift;

            auto pos_delim = uns::string::find(target, seeker, delimiter);
            auto pos_rborder = uns::string::find(target, seeker, rborder);

            auto result = std::string();

            auto ethalon = std::string(seeker, pos_delim);

            auto seeking_result =
                (pos_delim != target.end())
                && (pos_delim - target.begin() > seeker_shift)
                && (pos_rborder >= pos_delim);

            BOOST_TEST(
                seeking_result == uns::string::seeker_read(target, seeker, result, delimiter, true, 1, rborder)
            );

            BOOST_TEST(
                (
                    seeking_result
                    ? (pos_delim - target.begin() + 1)
                    : seeker_shift
                ) == seeker - target.begin()
            );

            BOOST_TEST(
                ((seeking_result && (ethalon == result)) || !seeking_result) == true
            );
        };

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(sample_read)

        BOOST_AUTO_TEST_CASE(correct_1) {
            auto target = std::string("key1 = val1;x;key2 =  val2;y; key3=val3;z;");
            auto delimiters = std::list<std::string>{
                ";x;",
                ";y;",
                ";z;"
            };

            auto result = std::string();

            result.clear();
            BOOST_TEST(
                true == uns::string::sample_read<std::string>(target, "key1", false, -4, result, delimiters, uns::string::find(target, target.begin(), "ey3"))
            );
            BOOST_TEST( "val1" == result );

            result.clear();
            BOOST_TEST(
                true == uns::string::sample_read(target, std::string("key2"), false, -5, result, delimiters, uns::string::find(target, target.begin(), "ey3"))
            );
            BOOST_TEST("val2" == result);

            result.clear();
            BOOST_TEST(
                false == uns::string::sample_read<std::string>(target, "key3", false, -2, result, delimiters, uns::string::find(target, target.begin(), "ey3"))
            );
        };
    
        BOOST_AUTO_TEST_CASE(correct_2) {
            auto target = std::string("key1 = val1;o;key2 =  val2;o; key3=val3;o;");
            auto delimiter = std::string(";o;");

            auto result = std::string();

            result.clear();
            BOOST_TEST(
                true == uns::string::sample_read(target, std::string("key1"), false, -4, result, delimiter, uns::string::find(target, target.begin(), "ey3"))
            );
            BOOST_TEST("val1" == result);

            result.clear();
            BOOST_TEST(
                true == uns::string::sample_read<std::string>(target, "key2", false, -5, result, delimiter, uns::string::find(target, target.begin(), "ey3"))
            );
            BOOST_TEST("val2" == result);

            result.clear();
            BOOST_TEST(
                false == uns::string::sample_read<std::string>(target, "key3", false, -2, result, ";o;", uns::string::find(target, target.begin(), "ey3"))
            );
        };

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();