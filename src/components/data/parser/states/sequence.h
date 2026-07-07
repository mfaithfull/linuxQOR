// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_SEQUENCE
#define QOR_PP_H_COMPONENTS_PARSER_STATES_SEQUENCE

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //Matches head followed by tail
    class qor_pp_module_interface(QOR_PARSER) Sequence : public ParserState
    {
    public:

        Sequence(Parser * parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~Sequence();

    private:

        unsigned int m_internalState{0};
        ref_of<ParserState>::type m_head;
        ref_of<ParserState>::type m_tail;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_SEQUENCE
