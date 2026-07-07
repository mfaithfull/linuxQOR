// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_SPECIFIC
#define QOR_PP_H_COMPONENTS_PARSER_STATES_SPECIFIC

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //matches one specific octet only
    class qor_pp_module_interface(QOR_PARSER) Specific : public ParserState
    {
    public:
        Specific(Parser* parser, byte matchingOctet, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~Specific();
        virtual void Emit();
    private:
        byte m_matchingOctet;
    };
}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_SPECIFIC
