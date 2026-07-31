// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_ANYONEOFSET
#define QOR_PP_H_DATA_PARSER_STATES_ANYONEOFSET

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //Must match at least one of the set to succeed
    //Succeeds on the first match without looking further
    class qor_pp_module_interface(QOR_PARSER) AnyOneOfSet : public ParserState
    {
    public:

        AnyOneOfSet(Parser * parser, std::vector<ref_of<ParserState>::type>*set, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~AnyOneOfSet();

    private:
        void Reset();
        std::vector<ref_of<ParserState>::type>* m_set;
        std::vector<ref_of<ParserState>::type>::iterator m_it;
        size_t m_index{0};
    };

}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::AnyOneOfSet, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::AnyOneOfSet>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_ANYONEOFSET
