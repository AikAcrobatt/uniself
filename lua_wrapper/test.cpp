
#include <iostream>

#include "uniself/lua_wrapper.hpp"
#include "uniself/strings.hpp"

#include <gtest/gtest.h>


template<>
::std::string testing::PrintToString(const ::std::u32string& Str) {
    return "u32\""
        + ::uns::string::cast<::std::string>(Str)
        + "\"";
};

namespace uns::tests {

    ::std::string to_string(const ::uns::lua::value& Value);

};

template<>
::std::string testing::PrintToString(const ::uns::lua::type::table& Table) {
    auto result = ::std::string{};

    result += "{";

    for (
        auto iterator = Table.cbegin<::uns::lua::type::number>();
        iterator != Table.cend<::uns::lua::type::number>();
        ++iterator
    ) {
        result += ::uns::tests::to_string(iterator->second);
    };

    for (
        auto iterator = Table.cbegin<::uns::lua::type::integer>();
        iterator != Table.cend<::uns::lua::type::integer>();
        ++iterator
    ) {
        result += ::uns::tests::to_string(iterator->second);
    };

    for (
        auto iterator = Table.cbegin<::uns::lua::type::boolean>();
        iterator != Table.cend<::uns::lua::type::boolean>();
        ++iterator
    ) {
        result += ::uns::tests::to_string(iterator->second);
    };

    for (
        auto iterator = Table.cbegin<::uns::lua::type::string>();
        iterator != Table.cend<::uns::lua::type::string>();
        ++iterator
    ) {
        result += ::uns::tests::to_string(iterator->second);
    };

    result += "}";

    return result;
};

namespace uns::tests {

    ::std::string to_string(const ::uns::lua::value& Value) {
        switch (Value.type()) {
            default:
            case ::uns::lua::value_type::nil:
            {
                return "nil";
            }
            case ::uns::lua::value_type::number:
            {
                return ::uns::string::cast<::std::string>(static_cast<::uns::lua::type::number>(Value));
            }
            case ::uns::lua::value_type::integer:
            {
                return ::uns::string::cast<::std::string>(static_cast<::uns::lua::type::integer>(Value));
            }
            case ::uns::lua::value_type::boolean:
            {
                return ::uns::string::cast<::std::string>(static_cast<::uns::lua::type::boolean>(Value));
            }
            case ::uns::lua::value_type::string:
            {
                return "\"" + ::uns::string::cast<::std::string>(static_cast<::uns::lua::type::string>(Value)) + "\"";
            }
            case ::uns::lua::value_type::table:
            {
                return ::uns::string::cast<::std::string>(static_cast<const ::uns::lua::type::table&>(Value).to_string());
            }
            case ::uns::lua::value_type::userdata:
            {
                return "userdata " + ::uns::string::cast<::std::string>(reinterpret_cast<uint64_t>(static_cast<const ::uns::lua::type::userdata&>(Value)));
            }
        };
    };

};

template<>
::std::string testing::PrintToString(const ::uns::lua::value& Value) {
    return ::uns::tests::to_string(Value);
};


namespace uns::tests {

    class lua_script_fixture: public ::testing::Test {
    public:
        ::uns::lua::script script;
    public:
        ::uns::lua::error set_script(const ::std::u32string& ScriptText) {
            script = ::uns::lua::script{ ScriptText };
            if (script.error().is()) {
                return script.error();
            };

            script.run();
            if (script.error().is()) {
                return script.error();
            };

            return {};
        };
        ::uns::lua::error set_script(
            const ::std::u32string& ScriptText
            , const ::uns::lua::library& UnsLuaLibrary
        ) {
            script = ::uns::lua::script{ UnsLuaLibrary };
            if (script.error().is()) {
                return script.error();
            };

            script.load(ScriptText);
            if (script.error().is()) {
                return script.error();
            };

            script.run();
            if (script.error().is()) {
                return script.error();
            };

            return {};
        };
    };

};

using LuaScriptLoading = ::uns::tests::lua_script_fixture;
TEST_F(LuaScriptLoading, CorrectScript) {
    const auto loading_error = set_script(
        UR"^^(
            function some_action(arg)
                return arg + 1
            end

            some_action(7)
        )^^"
    );

    ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());
};
TEST_F(LuaScriptLoading, IncorrectScript) {
    const auto loading_error = set_script(
        UR"^^(
            function some_action(arg)
                ++  --syntactic error
                return arg + 1
            end

            some_action(7)
        )^^"
    );

    ASSERT_TRUE(loading_error.is());
    ASSERT_EQ(loading_error.code(), ::uns::lua::errcode::errsyntax);
    ASSERT_EQ(loading_error.type(), ::uns::lua::errtype::lua_specific);
};
TEST_F(LuaScriptLoading, RaisingScript) {
    const auto error_msg = ::std::u32string{ U"Emitted error in TEST purposes" };

    const auto loading_error = set_script(
        UR"^^(
            error( ")^^" + error_msg + UR"^^(", 1)
        )^^"
    );

    ASSERT_TRUE(loading_error.is());
    EXPECT_EQ(loading_error.code(), ::uns::lua::errcode::errrun);
    EXPECT_EQ(loading_error.type(), ::uns::lua::errtype::lua_specific);
    EXPECT_NE(
        loading_error.text().find(::uns::string::cast<::std::string>(error_msg))
        , loading_error.text().npos
    );
};


using LuaFunctionCall = ::uns::tests::lua_script_fixture;
TEST_F(LuaFunctionCall, InvalidHandler) {
    {
        const auto loading_error = set_script(
            UR"^^(
                function test(...)
                    return ...
                end
            )^^"
        );
        ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());
    };

    auto test = script.get_function(U"NOT_DECLARED_test");
    ASSERT_TRUE(test.error().is());

    ASSERT_EQ(test.error().code(), ::uns::lua::errcode::errcall);
    ASSERT_EQ(test.error().type(), ::uns::lua::errtype::not_found);
    ASSERT_EQ(test.error().text(), "");

    const auto arg = ::uns::lua::value{ 123456789 };
    test(0, arg);
    ASSERT_TRUE(test.error().is());

    ASSERT_EQ(test.error().code(), ::uns::lua::errcode::errcall);
    ASSERT_EQ(test.error().type(), ::uns::lua::errtype::not_found);
    ASSERT_EQ(test.error().text(), "");
};
TEST_F(LuaFunctionCall, PositiveTest) {
    {
        const auto loading_error = set_script(
            UR"^^(
                function return_all_args(...)
                    return ...
                end
            )^^"
        );
        ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());
    };

    auto return_all_args = script.get_function(U"return_all_args");
    ASSERT_FALSE(return_all_args.error().is()) << ::uns::string::cast<::std::string>(return_all_args.error().to_string());

    auto arg1 = ::uns::lua::make_table();
    const auto arg2 = ::uns::lua::value{ 0.0045006 };
    const auto arg3 = ::uns::lua::value{ true };
    const auto arg4 = ::uns::lua::value{ U"It's working!" };

    static_cast<::uns::lua::type::table&>(arg1)[arg2] = arg3;
    static_cast<::uns::lua::type::table&>(arg1)[::uns::lua::value{ "What?" }] = arg4;
    static_cast<::uns::lua::type::table&>(arg1)[U"Some subtable"] = ::uns::lua::make_table();
    static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(arg1)["Some subtable"])[123] = "Yeah baby!";

    const auto results = return_all_args(4, arg1, arg2, arg3, arg4);
    ASSERT_FALSE(return_all_args.error().is()) << ::uns::string::cast<::std::string>(return_all_args.error().to_string());

    const auto returned_values = results.get_all();
    ASSERT_EQ(returned_values.size(), 4);

    ASSERT_EQ(returned_values[0], arg1);
    ASSERT_EQ(returned_values[1], arg2);
    ASSERT_EQ(returned_values[2], arg3);
    ASSERT_EQ(returned_values[3], arg4);
};
TEST_F(LuaFunctionCall, RaisingCall) {
    const auto error_msg = ::std::u32string{ U"Emitted error in TEST purposes" };

    const auto loading_error = set_script(
        UR"^^(
            function execute(arg)
                error( ")^^" + error_msg + UR"^^(", 2)
            end
        )^^"
    );

    ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());

    auto execute = script.get_function(U"execute");
    ASSERT_FALSE(execute.error().is()) << ::uns::string::cast<::std::string>(execute.error().to_string());

    execute(0);
    ASSERT_TRUE(execute.error().is());
    EXPECT_EQ(execute.error().code(), ::uns::lua::errcode::errrun);
    EXPECT_EQ(execute.error().type(), ::uns::lua::errtype::lua_specific);
    EXPECT_EQ(execute.error().text(), ::uns::string::cast<::std::string>(error_msg));

};


using LuaGlobals = ::uns::tests::lua_script_fixture;
TEST_F(LuaGlobals, CompareWithExtracted) {
    {
        const auto loading_error = set_script(
            UR"^^(
                global = {}
                global[0.0045006] = true
                global["What?"] = "It's working!"

                global["Some subtable"] = {}
                global["Some subtable"][123] = "Yeah baby!"
            )^^"
        );
        ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());
    };

    auto lua_global = script.get_global(U"global");
    ASSERT_FALSE(lua_global.error().is()) << ::uns::string::cast<::std::string>(lua_global.error().to_string());

    auto global = ::uns::lua::make_table();
    const auto some_numeric_key = ::uns::lua::value{ 0.0045006 };
    const auto some_boolean_value = ::uns::lua::value{ true };
    const auto some_string_value = ::uns::lua::value{ U"It's working!" };

    static_cast<::uns::lua::type::table&>(global)[some_numeric_key] = some_boolean_value;
    static_cast<::uns::lua::type::table&>(global)[::uns::lua::value{ "What?" }] = some_string_value;
    static_cast<::uns::lua::type::table&>(global)[U"Some subtable"] = ::uns::lua::make_table();
    static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(global)["Some subtable"])[123] = "Yeah baby!";

    ASSERT_EQ(lua_global.get(), global) << ::uns::string::cast<::std::string>(lua_global.error().to_string());
};
using LuaGlobals = ::uns::tests::lua_script_fixture;
TEST_F(LuaGlobals, CompareWithPushed) {
    {
        const auto loading_error = set_script(
            UR"^^(
                global1 = {}
                global1[0.0045006] = true
                global1["What?"] = "It's working!"

                global1["Some subtable"] = {}
                global1["Some subtable"][123] = "Yeah baby!"

                function compare_globals()
                    local function one_side_cmp(this, that)
                        for key, value in pairs(this) do
                            if that[key] == nil then
                                return false        --TODO to place an error message here
                            end
                            if type(this[key]) == "table" and type(that[key]) == "table" then
                                if not one_side_cmp(this[key], that[key]) then
                                    return false    --TODO to place an error message here
                                end
                            else
                                if this[key] ~= that[key] then
                                    return false    --TODO to place an error message here
                                end
                            end
                        end

                        return true
                    end

                    return one_side_cmp(global1, global2) and one_side_cmp(global2, global1)
                end
            )^^"
        );
        ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());
    };

    auto lua_global = script.get_global(U"global2");
    ASSERT_FALSE(lua_global.error().is()) << ::uns::string::cast<::std::string>(lua_global.error().to_string());

    auto global = ::uns::lua::make_table();
    const auto some_numeric_key = ::uns::lua::value{ 0.0045006 };
    const auto some_boolean_value = ::uns::lua::value{ true };
    const auto some_string_value = ::uns::lua::value{ U"It's working!" };

    static_cast<::uns::lua::type::table&>(global)[some_numeric_key] = some_boolean_value;
    static_cast<::uns::lua::type::table&>(global)[::uns::lua::value{ "What?" }] = some_string_value;
    static_cast<::uns::lua::type::table&>(global)[U"Some subtable"] = ::uns::lua::make_table();
    static_cast<::uns::lua::type::table&>(static_cast<::uns::lua::type::table&>(global)["Some subtable"])[123] = "Yeah baby!";

    lua_global.set(global);

    auto compare_globals = script.get_function(U"compare_globals");
    ASSERT_FALSE(compare_globals.error().is()) << ::uns::string::cast<::std::string>(compare_globals.error().to_string());

    const auto results = compare_globals(1);
    ASSERT_FALSE(compare_globals.error().is()) << ::uns::string::cast<::std::string>(compare_globals.error().to_string());

    const auto returned_values = results.get_all();
    ASSERT_EQ(returned_values.size(), 1);

    const auto expected_comparison_tables_result = ::uns::lua::value{ true };
    ASSERT_EQ(returned_values[0], expected_comparison_tables_result);
};


namespace uns::tests {

    const auto exception_text = ::std::string{ "Expected exception" };

    void lib_func_0_0(::uns::lua::thread&) {
        throw ::std::exception{ ::uns::tests::exception_text.c_str()};
    };

    ::std::array<::uns::lua::value, 5> lib_func_5_5(::uns::lua::thread&, const ::uns::lua::value& arg1, const ::uns::lua::value& arg2, const ::uns::lua::value& arg3, const ::uns::lua::value& arg4, const ::uns::lua::value& arg5) {
        return { arg1, arg2, arg3, arg4, arg5 };
    };

};

using LuaLibraries = ::uns::tests::lua_script_fixture;
TEST_F(LuaLibraries, NamelessLib) {
    const auto loading_error = set_script(
        U""
        , ::uns::lua::library{
            U""
            , {
                ::uns::lua::lib_entry::make<5, ::uns::tests::lib_func_5_5>(
                    ::std::u32string{ U"lib_func" }
                )
            }
            , U""
        }
    );
    ASSERT_TRUE(loading_error.is());
    EXPECT_EQ(loading_error.code(), ::uns::lua::errcode::errsyntax);
    EXPECT_EQ(loading_error.type(), ::uns::lua::errtype::invalid);
    EXPECT_EQ(loading_error.text(), "Module name is empty");
};
TEST_F(LuaLibraries, RaisingTest) {
    const auto loading_error = set_script(
        UR"^^(
            test = require "test_api"

            function execute()
                return test.lib_raise()
            end
        )^^"
        , ::uns::lua::library{
            U"test"
            , {
                ::uns::lua::lib_entry::make<::uns::tests::lib_func_0_0>(
                    ::std::u32string{ U"lib_raise" }
                )
            }
            , U""
        }
    );
    ASSERT_FALSE(loading_error.is()) << ::uns::string::cast<::std::string>(loading_error.to_string());

    auto execute = script.get_function(U"execute");
    ASSERT_FALSE(execute.error().is()) << ::uns::string::cast<::std::string>(execute.error().to_string());

    execute(0);
    ASSERT_TRUE(execute.error().is());
    EXPECT_EQ(execute.error().code(), ::uns::lua::errcode::errrun);
    EXPECT_EQ(execute.error().type(), ::uns::lua::errtype::lua_specific);
    EXPECT_NE(
        execute.error().text().find(::uns::tests::exception_text)
        , loading_error.text().npos
    );
};
TEST_F(LuaLibraries, PositiveTest) {
    const auto loading_error = set_script(
        UR"^^(
            test = require "test_api"

            function execute(arg1, arg2, arg3, arg4, arg5)
                return test.get_first_arg(), test.lib_func(arg2, arg4, arg3, arg5, arg1)
            end
        )^^"
        , ::uns::lua::library{
            U"test"
            , {
                ::uns::lua::lib_entry::make<5, ::uns::tests::lib_func_5_5>(
                    ::std::u32string{ U"lib_func" }
                )
            }
            , UR"^^(
                package["loaded"]["test_api"].get_first_arg = function() return "__arg_first__" end
            )^^"
        }
    );
    ASSERT_FALSE(loading_error.is());

    auto execute = script.get_function(U"execute");
    ASSERT_FALSE(execute.error().is()) << ::uns::string::cast<::std::string>(execute.error().to_string());

    const auto arg1 = ::uns::lua::value{ 1 };
    const auto arg2 = ::uns::lua::value{ 2 };
    const auto arg3 = ::uns::lua::value{ 3 };
    const auto arg4 = ::uns::lua::value{ 4 };
    const auto arg5 = ::uns::lua::value{ 5 };

    const auto results = execute(6, arg1, arg2, arg3, arg4, arg5);
    ASSERT_FALSE(execute.error().is()) << ::uns::string::cast<::std::string>(execute.error().to_string());

    const auto returned_values = results.get_all();
    ASSERT_EQ(returned_values.size(), 6);

    ASSERT_EQ(returned_values[0], "__arg_first__");
    ASSERT_EQ(returned_values[1], arg2);
    ASSERT_EQ(returned_values[2], arg4);
    ASSERT_EQ(returned_values[3], arg3);
    ASSERT_EQ(returned_values[4], arg5);
    ASSERT_EQ(returned_values[5], arg1);
};
