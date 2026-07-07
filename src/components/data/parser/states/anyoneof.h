// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_ANYONEOF
#define QOR_PP_H_COMPONENTS_PARSER_STATES_ANYONEOF

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //Must match either head or tail to succeed
    class qor_pp_module_interface(QOR_PARSER) AnyOneOf : public ParserState
    {
    public:

        AnyOneOf(Parser * parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~AnyOneOf();

    private:

        unsigned int m_internalState;
        ref_of<ParserState>::type m_head;
        ref_of<ParserState>::type m_tail;
    };
}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_ANYONEOF
