// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_OPTIONAL
#define QOR_PP_H_COMPONENTS_PARSER_STATES_OPTIONAL

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //Always matches. Will parse one head if it's present
    class qor_pp_module_interface(QOR_PARSER) Optional : public ParserState
    {
    public:

        Optional(Parser* parser, ref_of<ParserState>::type head, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~Optional();

    private:

        ref_of<ParserState>::type m_head;
        bool m_first;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_OPTIONAL
