// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_ONEOFARANGE
#define QOR_PP_H_COMPONENTS_PARSER_STATES_ONEOFARANGE

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //A single byte range match
    class qor_pp_module_interface(QOR_PARSER) OneOfARange : public ParserState
    {
    public:

        OneOfARange(Parser* parser, byte firstOctet, byte lastOctet, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~OneOfARange();
    private:
        byte m_first;
        byte m_last;
    };

}}}

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_ONEOFARANGE
