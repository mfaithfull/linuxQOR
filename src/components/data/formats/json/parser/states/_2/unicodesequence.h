// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_UNICODESEQUENCE
#define QOR_PP_H_DATA_JSON_PARSER_UNICODESEQUENCE

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) unicodeSequence : public 
        Sequence_t<Specific<uint32_t>, 
            Sequence_t<HEXDIGIT<uint32_t>, 
                Sequence_t<HEXDIGIT<uint32_t>, 
                    Sequence_t<HEXDIGIT<uint32_t>, 
                        HEXDIGIT<uint32_t>>>>>
    {
    public: 
        unicodeSequence(Parser* parser);
        virtual ~unicodeSequence();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        HEXDIGIT<uint32_t> h1;
        HEXDIGIT<uint32_t> h2;
        HEXDIGIT<uint32_t> h3;
        HEXDIGIT<uint32_t> h4;        
        Specific<uint32_t> s1;
        Sequence_t<HEXDIGIT<uint32_t>, Sequence_t<HEXDIGIT<uint32_t>, Sequence_t<HEXDIGIT<uint32_t>, HEXDIGIT<uint32_t>>>>s2;
        Sequence_t<HEXDIGIT<uint32_t>, Sequence_t<HEXDIGIT<uint32_t>, HEXDIGIT<uint32_t>>> s3;
        Sequence_t<HEXDIGIT<uint32_t>, HEXDIGIT<uint32_t>> s4;
    };

}}}}//qor::data::parser::json

namespace qor{
    qor_pp_declare_source_of(data::parser::json::unicodeSequence, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::json::unicodeSequence>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_JSON_PARSER_UNICODESEQUENCE
