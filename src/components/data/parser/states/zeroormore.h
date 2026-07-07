// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_ZEROORMORE
#define QOR_PP_H_COMPONENTS_PARSER_STATES_ZEROORMORE

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //Matches zero or more of the head state
    class qor_pp_module_interface(QOR_PARSER) ZeroOrMore : public ParserState
    {
    public:

        ZeroOrMore(Parser* parser, ref_of<ParserState>::type head, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~ZeroOrMore();

    private:

        ref_of<ParserState>::type m_head;
        bool m_first;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_ZEROORMORE
