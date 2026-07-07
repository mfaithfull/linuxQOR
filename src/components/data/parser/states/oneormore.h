// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_ONEORMORE
#define QOR_PP_H_COMPONENTS_PARSER_ONEORMORE

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //matches with at least 1 of head. Will match any number.
    class qor_pp_module_interface(QOR_PARSER) OneOrMore : public ParserState
    {
    public:
        OneOrMore(Parser* parser, ref_of<ParserState>::type head, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~OneOrMore();

    private:        
        ref_of<ParserState>::type m_head;
        bool m_first;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_ONEORMORE
