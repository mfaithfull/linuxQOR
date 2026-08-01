// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"

#include "src/qor/essentials/datastructures/frozen/algorithm.h"
#include "src/qor/essentials/datastructures/frozen/string.h"
#include <string>

#ifdef qor_pp_has_string_view
#include <string_view>
#include <type_traits>
#include <tuple>
#endif

using namespace qor;
using namespace qor::frozen::string_literals;
using namespace std::literals;

template<typename Char>
void test_string_view() 
{
#ifdef qor_pp_has_string_view
    constexpr auto strings = []() -> std::tuple<
        const Char(&)[12], frozen::basic_string<Char>, std::basic_string_view<Char>,
        const Char(&)[23], frozen::basic_string<Char>, std::basic_string_view<Char>> 
    {
        if constexpr (std::is_same_v<char, Char>) 
        {
            return {
                "Let it go !", "Let it go !"_s, "Let it go !"sv,
                "Let it go, let it go !", "Let it go, let it go !"_s, "Let it go, let it go !"sv
            };
        } 
        else if constexpr (std::is_same_v<wchar_t, Char>) 
        {
            return {
            L"Let it go !", L"Let it go !"_s, L"Let it go !"sv,
            L"Let it go, let it go !", L"Let it go, let it go !"_s, L"Let it go, let it go !"sv
            };
        } 
        else if constexpr (std::is_same_v<char16_t, Char>) 
        {
            return {
            u"Let it go !", u"Let it go !"_s, u"Let it go !"sv,
            u"Let it go, let it go !", u"Let it go, let it go !"_s, u"Let it go, let it go !"sv
            };
        } 
        else if constexpr (std::is_same_v<char32_t, Char>) 
        {
            return {
            U"Let it go !", U"Let it go !"_s, U"Let it go !"sv,
            U"Let it go, let it go !", U"Let it go, let it go !"_s, U"Let it go, let it go !"sv
        };
        }
#ifdef qor_pp_has_char8_t
        else if constexpr (std::is_same_v<char8_t, Char>) 
        {
            return {
                u8"Let it go !", u8"Let it go !"_s, u8"Let it go !"sv,
                u8"Let it go, let it go !", u8"Let it go, let it go !"_s, u8"Let it go, let it go !"sv
            };
        }
#endif
    }();

    const auto [
        letitgo,
        letitgo_s,
        letitgo_sv,
        letitgoletitgo,
        letitgoletitgo_s,
        letitgoletitgo_sv
    ] = strings;

    {
        frozen::basic_string<Char> letItGo = letitgo_sv;
        qor_pp_assert_that(letItGo == letitgo);
        qor_pp_assert_that(letItGo == letitgo_s);
        qor_pp_assert_that(letItGo == letitgo_sv);

        letItGo = letitgoletitgo_sv;
        qor_pp_assert_that(letItGo == letitgoletitgo);
        qor_pp_assert_that(letItGo == letitgoletitgo_s);
        qor_pp_assert_that(letItGo == letitgoletitgo_sv);
    }

    {
        constexpr frozen::basic_string<Char> letItGo = std::get<2>(strings);
        static_assert(letItGo == std::get<0>(strings), "frozen::string constexpr");
        static_assert(letItGo == std::get<1>(strings), "frozen::string constexpr literal");
        static_assert(letItGo == std::get<2>(strings), "frozen::string constexpr string view");
    }
#endif
}

struct VariousStringOperationsTestSuite{};

qor_pp_test_suite_case(VariousStringOperationsTestSuite, string) 
{
    {
        frozen::string letItGo = "Let it go !";
        qor_pp_assert_that(letItGo == "Let it go !");
        qor_pp_assert_that(letItGo == "Let it go !"_s);

        letItGo = "Let it go, let it go !";
        qor_pp_assert_that(letItGo == "Let it go, let it go !");
        qor_pp_assert_that(letItGo == "Let it go, let it go !"_s);
    }

    {
        static const char seed[] = "hello";
        std::string ref(seed);
        constexpr frozen::string obj(seed);
        qor_pp_assert_that(ref.size() == obj.size());
        qor_pp_assert_that(ref.length() == obj.length());
        qor_pp_assert_that(ref[1] == obj[1]);
        qor_pp_assert_that((ref =="h") == (obj == "h"));
        qor_pp_assert_that((ref != "h") == (obj != "h"));
        qor_pp_assert_that((ref > "h") == (obj > "h"));
        qor_pp_assert_that((ref >= "h") == (obj >= "h"));
        qor_pp_assert_that((ref < "h") == (obj < "h"));
        qor_pp_assert_that((ref <= "h") == (obj <= "h"));
#ifdef qor_pp_has_string_view
        qor_pp_assert_that((std::string_view)ref == (std::string_view)obj);
#endif
    }

    {
        constexpr frozen::string letItGo = "Let it go !";
        static_assert(letItGo == "Let it go !",   "frozen::string constexpr");
        static_assert(letItGo == "Let it go !"_s, "frozen::string constexpr literal");
    }

    test_string_view<char>();
}

qor_pp_test_suite_case(VariousStringOperationsTestSuite, WstringOperation) 
{
    {
        frozen::wstring letItGo = L"Let it go !";
        qor_pp_assert_that(letItGo == L"Let it go !");
        qor_pp_assert_that(letItGo == L"Let it go !"_s);

        letItGo = L"Let it go, let it go !";
        qor_pp_assert_that(letItGo == L"Let it go, let it go !");
        qor_pp_assert_that(letItGo == L"Let it go, let it go !"_s);
    }

    {
        constexpr frozen::wstring letItGo = L"Let it go !";
        static_assert(letItGo == L"Let it go !",   "frozen::wstring constexpr");
        static_assert(letItGo == L"Let it go !"_s, "frozen::wstring constexpr literal");
    }

    test_string_view<wchar_t>();
}

qor_pp_test_suite_case(VariousStringOperationsTestSuite, u16stringOperation) 
{
    {
        frozen::u16string letItGo = u"Let it go !";
        qor_pp_assert_that(letItGo == u"Let it go !");
        qor_pp_assert_that(letItGo == u"Let it go !"_s);

        letItGo = u"Let it go, let it go !";
        qor_pp_assert_that(letItGo == u"Let it go, let it go !");
        qor_pp_assert_that(letItGo == u"Let it go, let it go !"_s);
    }

    {
        constexpr frozen::u16string letItGo = u"Let it go !";
        static_assert(letItGo == u"Let it go !",   "frozen::u16string constexpr");
        static_assert(letItGo == u"Let it go !"_s, "frozen::u16string constexpr literal");
    }

    test_string_view<char16_t>();
}

qor_pp_test_suite_case(VariousStringOperationsTestSuite, u32stringOperation) 
{
    {
        frozen::u32string letItGo = U"Let it go !";
        qor_pp_assert_that(letItGo == U"Let it go !");
        qor_pp_assert_that(letItGo == U"Let it go !"_s);

        letItGo = U"Let it go, let it go !";
        qor_pp_assert_that(letItGo == U"Let it go, let it go !");
        qor_pp_assert_that(letItGo == U"Let it go, let it go !"_s);
    }

    {
        constexpr frozen::u32string letItGo = U"Let it go !";
        static_assert(letItGo == U"Let it go !",   "frozen::u32string constexpr");
        static_assert(letItGo == U"Let it go !"_s, "frozen::u32string constexpr literal");
    }

    test_string_view<char32_t>();
}

#ifdef qor_pp_has_char8_t
qor_pp_test_suite_case(VariousStringOperationsTestSuite, u8stringOperation) 
{
    {
        frozen::u8string letItGo = u8"Let it go !";
        qor_pp_assert_that(letItGo == u8"Let it go !");
        qor_pp_assert_that(letItGo == u8"Let it go !"_s);

        letItGo = u8"Let it go, let it go !";
        qor_pp_assert_that(letItGo == u8"Let it go, let it go !");
        qor_pp_assert_that(letItGo == u8"Let it go, let it go !"_s);
    }

    {
        constexpr frozen::u8string letItGo = u8"Let it go !";
        static_assert(letItGo == u8"Let it go !",   "frozen::u8string constexpr");
        static_assert(letItGo == u8"Let it go !"_s, "frozen::u8string constexpr literal");
    }

    test_string_view<char8_t>();
}
#endif

qor_pp_test_suite_case(VariousStringOperationsTestSuite, KnuthMorrisPrattSearch) 
{
    {
        std::string haystack = "n";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("n"));
        qor_pp_assert_that(index == haystack.begin());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("nn"));
        qor_pp_assert_that(std::distance(haystack.begin(), index) == 2);
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("mm"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("n*"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("nnnn"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("nmnn*"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "ABC ABCDAB ABCDABCDABDE";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_knuth_morris_pratt_searcher("ABCDABD"));
        qor_pp_assert_that(std::distance(haystack.begin(), index) == 15);
    }
}

qor_pp_test_suite_case(VariousStringOperationsTestSuite, BoyerMooreSearch) 
{
    {
        std::string haystack = "n";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("n"));
        qor_pp_assert_that(index == haystack.begin());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("nn"));
        qor_pp_assert_that(std::distance(haystack.begin(), index) == 2);
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("mm"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("n*"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("nnnn"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "nmnn";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("nmnn*"));
        qor_pp_assert_that(index == haystack.end());
    }

    {
        std::string haystack = "ABC ABCDAB ABCDABCDABDE";
        auto index = frozen::search(haystack.begin(), haystack.end(), frozen::make_boyer_moore_searcher("ABCDABD"));
        qor_pp_assert_that(std::distance(haystack.begin(), index) == 15);
    }
}
