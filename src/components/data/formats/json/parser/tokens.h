// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_TOKENS
#define QOR_PP_H_DATA_JSON_PARSER_TOKENS

#include <map>
#include <string>

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/states/optional.h"
#include "src/components/data/parser/states/deferred.h"
#include "src/components/data/parser/tokens.h"
#include "src/components/data/parser/parser.h"

namespace qor { namespace data { namespace parser { namespace json {

    enum class jsonToken : uint64_t
    {
        unescaped = static_cast<uint64_t>(eToken::Max) + 1ull,//19
        quotation_mark,
        escape,
        character,
        string,
        zero,
        plus,//25
        minus,
        decimal_point,
        digit1_9,
        _e,
        integer,//30
        fraction,
        exp,
        number,
        _true,
        _false,//35
        _null,
        ws,
        value_separator,
        name_separator,
        end_object,//40
        end_array,
        begin_object,
        begin_array,
        member,
        object,//45
        array,
        value,
        escape_sequence,
        unicode_sequence,
    };

    static const std::map<const jsonToken, const std::string> jsonTokenNames = {{
        {jsonToken::unescaped, "unescaped"},
        {jsonToken::quotation_mark, "quotation_mark"},
        {jsonToken::escape, "escape"},
        {jsonToken::character, "character"},
        {jsonToken::string, "string"},
        {jsonToken::zero, "zero"},
        {jsonToken::plus, "plus"},
        {jsonToken::minus, "minus"},
        {jsonToken::decimal_point, "decimal_point"},
        {jsonToken::digit1_9, "digit1_9"},
        {jsonToken::_e, "_e"},
        {jsonToken::integer, "integer"},
        {jsonToken::fraction, "fraction"},
        {jsonToken::exp, "exp"},
        {jsonToken::number, "number"},
        {jsonToken::_true, "_true"},
        {jsonToken::_false, "_false"},
        {jsonToken::_null, "_null"},
        {jsonToken::ws, "ws"},
        {jsonToken::value_separator, "value_separator"},
        {jsonToken::name_separator, "name_separator"},
        {jsonToken::end_object, "end_object"},
        {jsonToken::end_array, "end_array"},
        {jsonToken::begin_object, "begin_object"},
        {jsonToken::begin_array, "begin_array"},
        {jsonToken::member, "member"},
        {jsonToken::object, "object"},
        {jsonToken::array, "array"},
        {jsonToken::value, "value"},
        {jsonToken::escape_sequence, "escape_sequence"},
        {jsonToken::unicode_sequence, "unicode_sequence"}
    }};

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_TOKENS

