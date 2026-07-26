// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"
#include "src/framework/app/argparse/argumentparser.h"

using namespace qor;
using namespace qor::app;

struct ArgParserTestSuite{};

// Named argument accepting one-or-more ints is parsed into a vector.
qor_pp_test_suite_case(ArgParserTestSuite, test_named_int_vector)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument()
        .SetLongName("numbers")
        .SetAnyNumberOfArgumentsButAtLeastOne()
        .SetType(ArgTypeCast::e_int));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--numbers", "1", "2", "3" });
    qor_pp_assert_that(obj.IsArgValid());

    auto arg = obj.GetArg("numbers");
    qor_pp_assert_that(arg.GetArgumentExists());

    const std::vector<int>& v = arg.GetAsVecInt();
    qor_pp_assert_that(v.size() == 3);
    qor_pp_assert_that(v[0] == 1 && v[1] == 2 && v[2] == 3);
}

// The short name (-n) resolves to the same argument as the long name.
qor_pp_test_suite_case(ArgParserTestSuite, test_short_name_alias)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "numbers",
        kFromOneToInfiniteArgCount, ArgTypeCast::e_int));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "-n", "42" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("numbers").GetAsVecInt().at(0) == 42);
}

// A single required value is accessible via the scalar getter.
qor_pp_test_suite_case(ArgParserTestSuite, test_single_required_value)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("b", "b_key", 1,
        ArgTypeCast::e_int, true));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "-b", "7" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("b_key").GetAsInt() == 7);
}

// A missing required argument makes parsing invalid and yields an error string.
qor_pp_test_suite_case(ArgParserTestSuite, test_missing_required_is_invalid)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("b", "b_key", 1,
        ArgTypeCast::e_int, true));

    auto obj = parser.ParseArgs(std::vector<std::string>{});
    qor_pp_assert_that(!obj.IsArgValid());
    qor_pp_assert_that(!obj.GetErrorString().empty());
}

// Positional arguments are read from bare (unkeyed) tokens.
qor_pp_test_suite_case(ArgParserTestSuite, test_positional_argument)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreatePositionalArgument("int1")
        .SetType(ArgTypeCast::e_int)
        .SetRequired(false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "123" });
    qor_pp_assert_that(obj.IsArgValid());
    auto arg = obj.GetArg("int1");
    qor_pp_assert_that(arg.GetArgumentExists());
    qor_pp_assert_that(arg.GetAsInt() == 123);
}

// A value outside the declared choices is rejected.
qor_pp_test_suite_case(ArgParserTestSuite, test_choices_reject_out_of_set)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("o", "operation")
        .SetRequired(true)
        .SetChoices({ "+", "-", "*", "/" }));

    auto ok = parser.ParseArgs(std::vector<std::string>{ "-o", "+" });
    qor_pp_assert_that(ok.IsArgValid());
    qor_pp_assert_that(ok.GetArg("operation").GetAsString() == "+");

    auto parser2 = ArgumentParser("prog");
    parser2.AddArgument(CreateNamedArgument("o", "operation")
        .SetRequired(true)
        .SetChoices({ "+", "-", "*", "/" }));

    auto bad = parser2.ParseArgs(std::vector<std::string>{ "-o", "%" });
    qor_pp_assert_that(!bad.IsArgValid());
}

// Double parsing works end to end (mirrors the polish-notation example).
qor_pp_test_suite_case(ArgParserTestSuite, test_positional_doubles)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreatePositionalArgument("nums")
        .SetType(ArgTypeCast::e_double)
        .SetNumberOfArguments(2));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "1.5", "2.5" });
    qor_pp_assert_that(obj.IsArgValid());
    auto arg = obj.GetArg("nums");
    const std::vector<double>& nums = arg.GetAsVecDouble();
    qor_pp_assert_that(nums.size() == 2);
    qor_pp_assert_that(nums[0] == 1.5 && nums[1] == 2.5);
}

// An unknown key makes parsing invalid.
qor_pp_test_suite_case(ArgParserTestSuite, test_unknown_argument_is_invalid)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "numbers", 1,
        ArgTypeCast::e_int, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--nonexistent", "1" });
    qor_pp_assert_that(!obj.IsArgValid());
}

// Help text can be generated without throwing.
qor_pp_test_suite_case(ArgParserTestSuite, test_help_generation)
{
    auto parser = ArgumentParser("prog").SetDescription("desc");
    parser.AddArgument(CreateNamedArgument("n", "numbers", 1,
        ArgTypeCast::e_int, false, "a number"));

    std::string help = parser.GetUsage(80);
    qor_pp_assert_that(!help.empty());
    qor_pp_assert_that(help.find("numbers") != std::string::npos);
}

// A flag (0 values) records presence/absence; optional flags need SetRequired(false).
qor_pp_test_suite_case(ArgParserTestSuite, test_flag_presence)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("v", "verbose")
        .SetArgumentIsFlag().SetRequired(false));

    auto present = parser.ParseArgs(std::vector<std::string>{ "-v" });
    qor_pp_assert_that(present.IsArgValid());
    qor_pp_assert_that(present.GetArg("verbose").GetArgumentExists());

    auto absent = parser.ParseArgs(std::vector<std::string>{});
    qor_pp_assert_that(absent.IsArgValid()); // optional: omitting it is still valid
    qor_pp_assert_that(!absent.GetArg("verbose").GetArgumentExists());
}

// An omitted optional argument falls back to its SetDefault value.
qor_pp_test_suite_case(ArgParserTestSuite, test_default_value)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("j", "jobs", 1,
        ArgTypeCast::e_int, false).SetDefault(4));

    auto obj = parser.ParseArgs(std::vector<std::string>{});
    qor_pp_assert_that(obj.IsArgValid());
    auto jobs = obj.GetArg("jobs");
    qor_pp_assert_that(jobs.GetArgumentExists());
    qor_pp_assert_that(jobs.GetAsInt() == 4);
}

// Every argument is required by default; omitting a plain optional's value must fail.
qor_pp_test_suite_case(ArgParserTestSuite, test_required_by_default)
{
    auto parser = ArgumentParser("prog");
    // No SetRequired(false): this named argument is required by default.
    parser.AddArgument(CreateNamedArgument("n", "name", 1,
        ArgTypeCast::e_String));

    auto obj = parser.ParseArgs(std::vector<std::string>{});
    qor_pp_assert_that(!obj.IsArgValid());
}

// Two separate positional arguments each receive one token.
qor_pp_test_suite_case(ArgParserTestSuite, test_multiple_positionals)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreatePositionalArgument("source"));
    parser.AddArgument(CreatePositionalArgument("dest"));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "a.txt", "b.txt" });
    qor_pp_assert_that(obj.IsArgValid());
    auto src = obj.GetArg("source");
    auto dst = obj.GetArg("dest");
    qor_pp_assert_that(src.GetAsString() == "a.txt");
    qor_pp_assert_that(dst.GetAsString() == "b.txt");
}

// Positional and named arguments combine in a single parser.
qor_pp_test_suite_case(ArgParserTestSuite, test_mixed_positional_and_named)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreatePositionalArgument("path"));
    parser.AddArgument(CreateNamedArgument("f", "force")
        .SetArgumentIsFlag().SetRequired(false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "file.txt", "-f" });
    qor_pp_assert_that(obj.IsArgValid());
    auto path = obj.GetArg("path");
    qor_pp_assert_that(path.GetAsString() == "file.txt");
    qor_pp_assert_that(obj.GetArg("force").GetArgumentExists());
}

// kAnyArgCount accepts zero or more values.
qor_pp_test_suite_case(ArgParserTestSuite, test_any_arg_count)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "nums",
        kAnyArgCount, ArgTypeCast::e_int, false));

    auto many = parser.ParseArgs(std::vector<std::string>{ "--nums", "1", "2", "3" });
    qor_pp_assert_that(many.IsArgValid());
    qor_pp_assert_that(many.GetArg("nums").GetAsVecInt().size() == 3);

    auto none = parser.ParseArgs(std::vector<std::string>{});
    qor_pp_assert_that(none.IsArgValid()); // zero values is allowed
}

// A custom prefix character replaces the default '-'.
qor_pp_test_suite_case(ArgParserTestSuite, test_custom_prefix)
{
    auto parser = ArgumentParser("prog").SetPrefixChars('+');
    parser.AddArgument(CreateNamedArgument("n", "num", 1,
        ArgTypeCast::e_int, true));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "++num", "5" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("num").GetAsInt() == 5);
}

// Unknown options are tolerated when SetIgnoreUnknownArgs(true).
qor_pp_test_suite_case(ArgParserTestSuite, test_ignore_unknown_args)
{
    auto parser = ArgumentParser("prog").SetIgnoreUnknownArgs(true);
    parser.AddArgument(CreateNamedArgument("n", "num", 1,
        ArgTypeCast::e_int, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--num", "1", "--bogus", "2" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("num").GetAsInt() == 1);
}

// bool arguments accept the documented spellings and reject others.
qor_pp_test_suite_case(ArgParserTestSuite, test_bool_parsing)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("b", "bits",
        kFromOneToInfiniteArgCount, ArgTypeCast::e_bool, false));

    auto ok = parser.ParseArgs(std::vector<std::string>{ "--bits", "true", "False", "TRUE" });
    qor_pp_assert_that(ok.IsArgValid());
    auto bits = ok.GetArg("bits");
    const std::vector<bool>& v = bits.GetAsVecBool();
    qor_pp_assert_that(v.size() == 3);
    qor_pp_assert_that(v[0] == true && v[1] == false && v[2] == true);

    auto bad = parser.ParseArgs(std::vector<std::string>{ "--bits", "1" }); // "1" is not a bool
    qor_pp_assert_that(!bad.IsArgValid());
}

// Regression: long long + choices used to read the wrong (empty) vector and crash.
qor_pp_test_suite_case(ArgParserTestSuite, test_longlong_choices)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("x", "xval", 1,
        ArgTypeCast::e_longlong, true)
        .SetChoices(std::vector<long long>{ 10, 20, 30 }));

    auto ok = parser.ParseArgs(std::vector<std::string>{ "--xval", "20" });
    qor_pp_assert_that(ok.IsArgValid());
    qor_pp_assert_that(ok.GetArg("xval").GetAsLongLong() == 20);

    auto bad = parser.ParseArgs(std::vector<std::string>{ "--xval", "25" });
    qor_pp_assert_that(!bad.IsArgValid()); // 25 is out of choices
}

// Reference-returning getters now return by value, so calling them directly on
// the temporary from GetArg(...) no longer dangles.
qor_pp_test_suite_case(ArgParserTestSuite, test_getters_do_not_dangle)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreatePositionalArgument("name"));
    parser.AddArgument(CreateNamedArgument("n", "nums",
        kFromOneToInfiniteArgCount, ArgTypeCast::e_int, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "hello", "--nums", "1", "2" });
    qor_pp_assert_that(obj.IsArgValid());

    // Bind a value straight from the temporary ParsedArgument: previously dangling.
    std::string name = obj.GetArg("name").GetAsString();
    qor_pp_assert_that(name == "hello");

    std::vector<int> nums = obj.GetArg("nums").GetAsVecInt();
    qor_pp_assert_that(nums.size() == 2 && nums[0] == 1 && nums[1] == 2);
}

// Scalar getters throw std::out_of_range instead of invoking UB on empty access.
qor_pp_test_suite_case(ArgParserTestSuite, test_getter_throws_when_empty)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "num", 1,
        ArgTypeCast::e_int, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{}); // num absent
    auto num = obj.GetArg("num");
    qor_pp_assert_that(!num.GetArgumentExists());

    bool threw = false;
    try { (void)num.GetAsInt(); }
    catch (const std::out_of_range&) { threw = true; }
    qor_pp_assert_that(threw);
}

// The correctly-spelled kFromOneToInfiniteArgCount constant means "one or more".
qor_pp_test_suite_case(ArgParserTestSuite, test_from_one_to_infinite_constant)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "nums",
        kFromOneToInfiniteArgCount, ArgTypeCast::e_int, false));

    // At least one value is accepted...
    auto many = parser.ParseArgs(std::vector<std::string>{ "--nums", "1", "2" });
    qor_pp_assert_that(many.IsArgValid());
    qor_pp_assert_that(many.GetArg("nums").GetAsVecInt().size() == 2);

    // ...but zero values (the flag present with no args) is not enough.
    auto none = parser.ParseArgs(std::vector<std::string>{ "--nums" });
    qor_pp_assert_that(!none.IsArgValid());
}

// SetUsage overrides the auto-generated usage line (previously ignored).
qor_pp_test_suite_case(ArgParserTestSuite, test_set_usage_override)
{
    auto parser = ArgumentParser("tool").SetDescription("d")
        .SetUsage("tool [OPTIONS] FILE");
    parser.AddArgument(CreateNamedArgument("n", "name", 1,
        ArgTypeCast::e_String, false));

    std::string help = parser.GetUsage(80);
    qor_pp_assert_that(help.find("tool [OPTIONS] FILE") != std::string::npos);
    // The auto-generated form must not leak through.
    qor_pp_assert_that(help.find("tool -n,--name") == std::string::npos);
}

// A long-name-only argument renders without a stray leading comma.
// SetAllowAbbrev(false) so no short name is auto-generated -- otherwise the
// argument would gain a "-v" short name and no longer be long-only.
qor_pp_test_suite_case(ArgParserTestSuite, test_long_only_no_leading_comma)
{
    auto parser = ArgumentParser("prog").SetAllowAbbrev(false);
    parser.AddArgument(CreateNamedArgument("", "verbose", 0,
        ArgTypeCast::e_String, false));

    std::string help = parser.GetUsage(80);
    qor_pp_assert_that(help.find("--verbose") != std::string::npos);
    qor_pp_assert_that(help.find(",--verbose") == std::string::npos);
}

// SetAllowAbbrev auto-generates a single-char short name for a long-only option,
// which then works for parsing and appears in the help.
qor_pp_test_suite_case(ArgParserTestSuite, test_abbrev_generates_short_name)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("", "numbers", 1,
        ArgTypeCast::e_int, false));

    // Generated "-n" parses like the explicit long name.
    auto obj = parser.ParseArgs(std::vector<std::string>{ "-n", "7" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("numbers").GetAsInt() == 7);

    // And it is shown in the help paired with the long name.
    std::string help = parser.GetUsage(80);
    qor_pp_assert_that(help.find("-n,--numbers") != std::string::npos);
}

// When the first letter is already taken, the next free letter is used, and
// "h" stays reserved for the auto-added help option.
qor_pp_test_suite_case(ArgParserTestSuite, test_abbrev_generation_avoids_collisions)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "name", 1,
        ArgTypeCast::e_String, false));   // explicit -n
    parser.AddArgument(CreateNamedArgument("", "numbers", 1,
        ArgTypeCast::e_int, false));      // 'n' taken -> 'u'
    parser.AddArgument(CreateNamedArgument("", "host", 1,
        ArgTypeCast::e_String, false));   // 'h' reserved -> 'o'

    std::string help = parser.GetUsage(80);
    qor_pp_assert_that(help.find("-u,--numbers") != std::string::npos);
    qor_pp_assert_that(help.find("-o,--host") != std::string::npos);
    // The explicit -n is untouched and no second -n was generated.
    qor_pp_assert_that(help.find("-n,--name") != std::string::npos);
}

// A bool argument stores and returns the parsed value.
qor_pp_test_suite_case(ArgParserTestSuite, test_bool_scalar)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("d", "debug", 1,
        ArgTypeCast::e_bool, false));

    auto on = parser.ParseArgs(std::vector<std::string>{ "--debug", "true" });
    qor_pp_assert_that(on.IsArgValid());
    qor_pp_assert_that(on.GetArg("debug").GetAsBool() == true);

    auto off = parser.ParseArgs(std::vector<std::string>{ "--debug", "False" });
    qor_pp_assert_that(off.IsArgValid());
    qor_pp_assert_that(off.GetArg("debug").GetAsBool() == false);
}

// SetAllowAbbrev: an unambiguous prefix of a long option resolves to it.
qor_pp_test_suite_case(ArgParserTestSuite, test_abbrev_unambiguous)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("", "verbose", 0,
        ArgTypeCast::e_String, false));
    parser.AddArgument(CreateNamedArgument("", "output", 1,
        ArgTypeCast::e_String, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--verb", "--out", "f.txt" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("verbose").GetArgumentExists());
    auto output = obj.GetArg("output");
    qor_pp_assert_that(output.GetAsString() == "f.txt");
}

// SetAllowAbbrev: a prefix matching more than one long option is rejected.
qor_pp_test_suite_case(ArgParserTestSuite, test_abbrev_ambiguous)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("", "verbose", 0,
        ArgTypeCast::e_String, false));
    parser.AddArgument(CreateNamedArgument("", "version", 0,
        ArgTypeCast::e_String, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--ver" });
    qor_pp_assert_that(!obj.IsArgValid());
    qor_pp_assert_that(obj.GetErrorString().find("Ambiguous") != std::string::npos);
}

// SetAllowAbbrev(false) disables prefix matching; the abbreviation is unknown.
qor_pp_test_suite_case(ArgParserTestSuite, test_abbrev_disabled)
{
    auto parser = ArgumentParser("prog").SetAllowAbbrev(false);
    parser.AddArgument(CreateNamedArgument("", "verbose", 0,
        ArgTypeCast::e_String, false));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--verb" });
    qor_pp_assert_that(!obj.IsArgValid());
}

// An exact long-name match still wins even when it is a prefix of a longer option.
qor_pp_test_suite_case(ArgParserTestSuite, test_abbrev_exact_match_wins)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("", "ver", 0,
        ArgTypeCast::e_String, false));
    parser.AddArgument(CreateNamedArgument("", "verbose", 0,
        ArgTypeCast::e_String, false));

    // "--ver" is an exact name AND a prefix of "--verbose"; exact must win.
    auto obj = parser.ParseArgs(std::vector<std::string>{ "--ver" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(obj.GetArg("ver").GetArgumentExists());
}

// Help text no longer glues tokens together or leaves trailing spaces on lines.
qor_pp_test_suite_case(ArgParserTestSuite, test_help_formatting)
{
    auto parser = ArgumentParser("prog").SetDescription("desc");
    parser.AddArgument(CreateNamedArgument("v", "verbose")
        .SetArgumentIsFlag().SetRequired(false));
    parser.AddArgument(CreateNamedArgument("j", "jobs", 1,
        ArgTypeCast::e_int, false));

    std::string help = parser.GetUsage(80);

    // No line ends with a space (i.e. no " \n" and no trailing space at the end).
    qor_pp_assert_that(help.find(" \n") == std::string::npos);
    qor_pp_assert_that(!(help.size() && help.back() == ' '));

    // Adjacent option tokens are space-separated, not glued (e.g. "]-" or "e[").
    qor_pp_assert_that(help.find("][") == std::string::npos);
    qor_pp_assert_that(help.find("verbose[") == std::string::npos);
}

// Keyword-style spec structs build the same argument as the positional factory.
// Works in every standard via ordinary aggregate assignment.
qor_pp_test_suite_case(ArgParserTestSuite, test_spec_struct_named_and_positional)
{
    auto parser = ArgumentParser("prog");

    NamedArgSpec numsSpec;
    numsSpec.longName = "nums";
    numsSpec.nargs = kFromOneToInfiniteArgCount;
    numsSpec.type = ArgTypeCast::e_int;
    numsSpec.required = false;
    parser.AddArgument(CreateNamedArgument(numsSpec));

    PositionalArgSpec pathSpec;
    pathSpec.name = "path";
    pathSpec.required = false;
    parser.AddArgument(CreatePositionalArgument(pathSpec));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "file.txt", "--nums", "1", "2" });
    qor_pp_assert_that(obj.IsArgValid());
    auto path = obj.GetArg("path");
    qor_pp_assert_that(path.GetAsString() == "file.txt");
    qor_pp_assert_that(obj.GetArg("nums").GetAsVecInt().size() == 2);
}

#if __cplusplus >= 202002L || _MSVC_LANG >= 202002L
// C++20: the same spec structs enable Python-like keyword arguments via
// designated initializers.
qor_pp_test_suite_case(ArgParserTestSuite, test_designated_initializers_cpp20)
{
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument({
        .longName = "numbers",
        .nargs    = kFromOneToInfiniteArgCount,
        .type     = ArgTypeCast::e_int,
        .required = false,
        .help     = "some numbers"}));
    parser.AddArgument(CreatePositionalArgument({
        .name     = "path",
        .required = false}));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "out.txt", "--numbers", "3", "4", "5" });
    qor_pp_assert_that(obj.IsArgValid());
    auto path = obj.GetArg("path");
    qor_pp_assert_that(path.GetAsString() == "out.txt");
    qor_pp_assert_that(obj.GetArg("numbers").GetAsVecInt().size() == 3);
}
#endif

// BindTo writes parsed scalar values straight into the bound variables.
qor_pp_test_suite_case(ArgParserTestSuite, test_bind_scalar_values)
{
    int count = -1;
    std::string name = "unset";
    double ratio = 0.0;

    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("c", "count", 1).BindTo(&count));
    parser.AddArgument(CreateNamedArgument("n", "name", 1).BindTo(&name));
    parser.AddArgument(CreateNamedArgument("r", "ratio", 1).BindTo(&ratio));

    auto obj = parser.ParseArgs(std::vector<std::string>{
        "--count", "42", "--name", "world", "--ratio", "1.5" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(count == 42);
    qor_pp_assert_that(name == "world");
    qor_pp_assert_that(ratio == 1.5);
}

// BindTo infers the argument type from the bound variable (no SetType needed).
qor_pp_test_suite_case(ArgParserTestSuite, test_bind_infers_type)
{
    int count = 0;
    auto parser = ArgumentParser("prog");
    // No SetType call: BindTo(int*) must set the type to e_int so "7" parses
    // as an int rather than staying a string.
    parser.AddArgument(CreateNamedArgument("c", "count", 1).BindTo(&count));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--count", "7" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(count == 7);
}

// BindTo to a vector captures every parsed token.
qor_pp_test_suite_case(ArgParserTestSuite, test_bind_vector_values)
{
    std::vector<int> nums;
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("N", "nums")
        .SetAnyNumberOfArgumentsButAtLeastOne()
        .BindTo(&nums));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--nums", "1", "2", "3" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(nums.size() == 3);
    qor_pp_assert_that(nums[0] == 1 && nums[1] == 2 && nums[2] == 3);
}

// An absent optional bound argument leaves the variable at its prior value.
qor_pp_test_suite_case(ArgParserTestSuite, test_bind_absent_optional_untouched)
{
    std::string name = "default-name";
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreateNamedArgument("n", "name", 1)
        .SetRequired(false)
        .BindTo(&name));

    auto obj = parser.ParseArgs(std::vector<std::string>{});
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(name == "default-name");   // untouched, acts as the default
}

// Bindings are applied to positional arguments too.
qor_pp_test_suite_case(ArgParserTestSuite, test_bind_positional)
{
    int value = 0;
    auto parser = ArgumentParser("prog");
    parser.AddArgument(CreatePositionalArgument("num")
        .BindTo(&value));

    auto obj = parser.ParseArgs(std::vector<std::string>{ "99" });
    qor_pp_assert_that(obj.IsArgValid());
    qor_pp_assert_that(value == 99);
}

// A failed parse must NOT write through bindings (variables stay untouched).
qor_pp_test_suite_case(ArgParserTestSuite, test_bind_not_applied_on_parse_failure)
{
    int required = -1;
    int bound = -1;
    auto parser = ArgumentParser("prog");
    // Required argument that we will NOT supply, forcing an invalid parse.
    parser.AddArgument(CreateNamedArgument("r", "req", 1)
        .SetType(ArgTypeCast::e_int));
    parser.AddArgument(CreateNamedArgument("b", "bound", 1)
        .SetRequired(false)
        .BindTo(&bound));
    (void)required;

    auto obj = parser.ParseArgs(std::vector<std::string>{ "--bound", "5" });
    qor_pp_assert_that(!obj.IsArgValid());     // missing required arg
    qor_pp_assert_that(bound == -1);           // binding not applied on failure
}
