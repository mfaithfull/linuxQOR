// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_TOKENS
#define QOR_PP_H_DATA_TEXT_PARSER_TOKENS

#include <map>
#include <string>

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/states/optional.h"
#include "src/components/data/parser/states/deferred.h"
#include "src/components/data/parser/tokens.h"
#include "src/components/data/parser/parser.h"

namespace qor { namespace data { namespace parser { namespace text {

    enum class textToken : uint64_t
    {
        utf8BOM = static_cast<uint64_t>(eToken::Max) + 1ull,
        utf16BEBOM,
        utf16LEBOM,
        utf32BEBOM,
        utf32LEBOM,
        utf8char,
        utf8leadbyte,
        utf8continuationbyte,        
        utf16char,
        utf16lowbyte,
        utf16highbyte,
        utf32char,
        utf16BEchar,
        utf16LEchar,
        utf32BEchar,
        utf32LEchar,
        lebom,
        bom,
        text,
    };

    static const std::map<const textToken, const std::string> textTokenNames = {{
        {textToken::utf8BOM, "UTF-8 BOM"},
        {textToken::utf16BEBOM, "UTF-16BE BOM"},
        {textToken::utf16LEBOM, "UTF-16LE BOM"},
        {textToken::utf32BEBOM, "UTF-32BE BOM"},
        {textToken::utf32LEBOM, "UTF-32LE BOM"},
        {textToken::utf8char,   "UTF-8 char"},
        {textToken::utf16BEchar,"UTF-16BE char"},
        {textToken::utf16LEchar,"UTF-16LE char"},
        {textToken::utf32BEchar,"UTF-32BE char"},
        {textToken::utf32LEchar,"UTF-32LE char"},
        {textToken::text,       "text"}
    }};

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_TOKENS

