// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_ONEORMORE
#define QOR_PP_H_DATA_PARSER_ONEORMORE

#include "../state.h"

namespace qor { namespace data { namespace parser {

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

}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::OneOrMore, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::OneOrMore>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_ONEORMORE
