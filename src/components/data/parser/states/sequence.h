// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_SEQUENCE
#define QOR_PP_H_DATA_PARSER_STATES_SEQUENCE

#include "../state.h"

namespace qor { namespace data { namespace parser {

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

}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::Sequence, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::Sequence>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_SEQUENCE
