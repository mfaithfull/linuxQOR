// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_ONEOFARANGE
#define QOR_PP_H_DATA_PARSER_STATES_ONEOFARANGE

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //A single byte range match
    class qor_pp_module_interface(QOR_PARSER) OneOfARange : public ParserState
    {
    public:

        OneOfARange(Parser* parser, byte firstOctet, byte lastOctet, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~OneOfARange();
    private:
        Context* m_context;
        byte m_first;
        byte m_last;
    };

}}}

namespace qor{
    qor_pp_declare_source_of(data::parser::OneOfARange, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::OneOfARange>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_ONEOFARANGE
