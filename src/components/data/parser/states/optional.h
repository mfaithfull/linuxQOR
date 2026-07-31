// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_OPTIONAL
#define QOR_PP_H_DATA_PARSER_STATES_OPTIONAL

#include "../state.h"

namespace qor { namespace data { namespace parser {

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

}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::Optional, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::Optional>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_OPTIONAL
