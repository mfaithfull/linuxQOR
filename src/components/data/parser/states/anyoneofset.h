// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_ANYONEOFSET
#define QOR_PP_H_COMPONENTS_PARSER_STATES_ANYONEOFSET

#include "../state.h"

namespace qor { namespace components { namespace parser {

    //Must match at least one of the set to succeed
    //Succeeds on the first match withtout looking further
    class qor_pp_module_interface(QOR_PARSER) AnyOneOfSet : public ParserState
    {
    public:

        AnyOneOfSet(Parser * parser, std::vector<ref_of<ParserState>::type>*set, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~AnyOneOfSet();

    private:

        std::vector<ref_of<ParserState>::type>* m_set;
        std::vector<ref_of<ParserState>::type>::iterator m_it;
        size_t m_index{0};
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_ANYONEOFSET
