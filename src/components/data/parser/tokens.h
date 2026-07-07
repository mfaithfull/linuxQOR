// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_TOKENS
#define QOR_PP_H_COMPONENTS_PARSER_TOKENS

#include <map>
#include <string>

namespace qor { namespace components { namespace parser {

    //RFC5234 tokens
    enum class eToken : uint64_t
    {
        Lexical = 1,
        Octet,
        CarriageReturn,
        LineFeed,
        HorizontalTab,
        DoubleQuote,
        Space,
        Digit,
        VisibleChar,
        Control,
        HexDigit,
        WhiteSpace,
        LinearWhiteSpace,
        CarriageReturnLineFeed,
        Bit,
        Char,
        Alpha,
        Max,
    };

    inline const std::map<eToken, std::string> tokenNames = {{
        {eToken::Lexical, "Lexical"},
        {eToken::Octet, "Octet"},
        {eToken::CarriageReturn, "CarriageReturn"},
        {eToken::LineFeed, "LineFeed"},
        {eToken::HorizontalTab, "HorizontalTab"},
        {eToken::DoubleQuote, "DoubleQuote"},
        {eToken::Space, "Space"},
        {eToken::Digit, "Digit"},
        {eToken::VisibleChar, "VisibleChar"},
        {eToken::Control, "Control"},
        {eToken::HexDigit, "HexDigit"},
        {eToken::WhiteSpace, "WhiteSpace"},
        {eToken::LinearWhiteSpace, "LinearWhiteSpace"},
        {eToken::CarriageReturnLineFeed, "CarriageReturnLineFeed"},
        {eToken::Bit, "Bit"},
        {eToken::Char, "Char"},
        {eToken::Alpha, "Alpha"},
    }};

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_TOKENS