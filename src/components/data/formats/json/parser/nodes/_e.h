// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_E
#define QOR_PP_H_DATA_JSON_PARSER_NODES_E

#include <string>
#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_e.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class _ENode : public NodeAdapter< model::json::_E >
    {
    public:
        _ENode() : NodeAdapter< model::json::_E >(static_cast<uint64_t>(jsonToken::_e)){};
        virtual ~_ENode() = default;
        virtual std::string ToString() const override { return "e"; };
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_E