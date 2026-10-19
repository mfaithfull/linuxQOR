// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_TOKENS
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_TOKENS

#include <map>
#include <string>

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/states/optional.h"
#include "src/components/data/parser/states/deferred.h"
#include "src/components/data/parser/tokens.h"
#include "src/components/data/parser/parser.h"

namespace qor { namespace protocols { namespace yaml {

    using namespace qor::data;
    using namespace qor::data::parser;

    enum class yamlToken : uint64_t
    {
        unescaped = static_cast<uint64_t>(eToken::Max) + 1ull,
        quotation_mark,
        escape,
        character,
        string,
        zero,
        plus,
        minus,
        decimal_point,
        digit1_9,
        _e,
        integer,
        fraction,
        exp,
        number,
        _true,
        _false,
        _null,
        ws,
        value_separator,
        name_separator,
        end_mapping,
        end_sequence,
        begin_mapping,
        begin_sequence,
        pair,
        mapping,
        sequence,
        value,
        escape_sequence,
        unicode_sequence,
        plain_scalar,
        single_quote,
        double_quote,
        document,
    };

    static const std::map<const yamlToken, const std::string> yamlTokenNames = {{
        {yamlToken::unescaped, "unescaped"},
        {yamlToken::quotation_mark, "quotation_mark"},
        {yamlToken::escape, "escape"},
        {yamlToken::character, "character"},
        {yamlToken::string, "string"},
        {yamlToken::zero, "zero"},
        {yamlToken::plus, "plus"},
        {yamlToken::minus, "minus"},
        {yamlToken::decimal_point, "decimal_point"},
        {yamlToken::digit1_9, "digit1_9"},
        {yamlToken::_e, "_e"},
        {yamlToken::integer, "integer"},
        {yamlToken::fraction, "fraction"},
        {yamlToken::exp, "exp"},
        {yamlToken::number, "number"},
        {yamlToken::_true, "_true"},
        {yamlToken::_false, "_false"},
        {yamlToken::_null, "_null"},
        {yamlToken::ws, "ws"},
        {yamlToken::value_separator, "value_separator"},
        {yamlToken::name_separator, "name_separator"},
        {yamlToken::end_mapping, "end_mapping"},
        {yamlToken::end_sequence, "end_sequence"},
        {yamlToken::begin_mapping, "begin_mapping"},
        {yamlToken::begin_sequence, "begin_sequence"},
        {yamlToken::pair, "pair"},
        {yamlToken::mapping, "mapping"},
        {yamlToken::sequence, "sequence"},
        {yamlToken::value, "value"},
        {yamlToken::escape_sequence, "escape_sequence"},
        {yamlToken::unicode_sequence, "unicode_sequence"},
        {yamlToken::plain_scalar, "plain_scalar"},
        {yamlToken::single_quote, "single_quote"},
        {yamlToken::double_quote, "double_quote"},
        {yamlToken::document, "document"}
    }};

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_TOKENS